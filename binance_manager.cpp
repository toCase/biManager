#include "binance_manager.h"


TaskWorker::TaskWorker(D_Account *account, QObject *parent)
    : m_account(account), QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
}

TaskWorker::~TaskWorker()
{
    m_manager->deleteLater();
}

void TaskWorker::processTask(BinanceTask *task)
{
    switch (task->typeTask()) {
    case TypeTask::GET_BALANCE:
        handleGetBalance();
        break;
    case TypeTask::GET_ALL_ORDERS:
        handleGetOrders();
        break;
    default:
        break;
    }
    task->deleteLater();
}

QString TaskWorker::generateSignature(const QString &queryString)
{
    QByteArray signature = QMessageAuthenticationCode::hash(
                               queryString.toUtf8(),
                               m_account->secret().toUtf8(),
                               QCryptographicHash::Sha256
                               ).toHex();
    return QString(signature);
}

QNetworkRequest TaskWorker::createAuthrizedRequest(const QString &endpoint, const QString &queryString)
{
    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    QString fullQueryString = queryString;
    if (!fullQueryString.isEmpty()) {
        fullQueryString +="&";
    }
    fullQueryString += "timestamp=" + timestamp;

    QString signature = generateSignature(fullQueryString);
    QString url = BASE_URL + endpoint + "?" + fullQueryString + "&signature=" + signature;

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-MBX-APIKEY", m_account->api().toUtf8());
    return request;

}

void TaskWorker::handleGetBalance()
{
    QNetworkRequest request = createAuthrizedRequest("/api/v3/account");

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply](){
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responce = reply->readAll();
            QJsonDocument jdoc = QJsonDocument::fromJson(responce);
            emit taskCompleted(m_account->idx(), jdoc.object(), TypeTask::GET_BALANCE);
        } else {
            emit taskError(m_account->idx(), reply->errorString(), TypeTask::GET_BALANCE);
        }
        reply->deleteLater();
    });
}

void TaskWorker::handleGetOrders()
{
    QNetworkRequest request = createAuthrizedRequest("/api/v3/allOrders", "symbol=ETHUSDT");

    qDebug() << request.url();

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply](){
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responce = reply->readAll();
            QJsonDocument jdoc = QJsonDocument::fromJson(responce);
            emit taskCompleted(m_account->idx(), jdoc.object(), TypeTask::GET_ALL_ORDERS);
        } else {
            qDebug() << reply->errorString();
            emit taskError(m_account->idx(), reply->errorString(), TypeTask::GET_ALL_ORDERS);
        }
        reply->deleteLater();
    });
}


BinanceManager::BinanceManager(QObject *parent) : QObject(parent), m_isProcessing(false)
{

}

BinanceManager::~BinanceManager()
{
    for (auto thread : m_threads) {
        thread->quit();
        thread->wait();
        delete thread;
    }
    qDeleteAll(m_workers);
}

bool BinanceManager::initialize(const QList<D_Account *> accounts)
{

    qDebug() << "Initializing manager with" << accounts.size() << "accounts";

    m_accounts = accounts;

    return createWorkers(accounts);
}

bool BinanceManager::updateAccounts(const QList<D_Account *> accounts)
{
    qDebug() << "Updating account list with " << accounts.size() << " accounts";
    try {
        clearWorkers();

        m_accounts = accounts;

        if (createWorkers(accounts)){
            emit accountUpdated();
            return true;
        } else {
            throw std::runtime_error("Failed to create workers");
        }

    } catch (const std::exception& e) {
        emit accountUpdatedError(QString("Fail to update accounts: %1").arg(e.what()));
        return false;
    }
}

void BinanceManager::addTask(BinanceTask *task)
{
    QMutexLocker locker(&m_queueMutex);
    qDebug() << "Adding task for account" << task->accountID();

    if (!m_workers.contains(task->accountID())){
        qDebug() << "Error: No worker found for account" << task->accountID();
        emit taskError(task->accountID(), "Worker not found", task->typeTask());
        task->deleteLater();
        return;
    }

    m_taskQueue.enqueue(task);

    if (!m_isProcessing) {
        m_isProcessing = true;
        QMetaObject::invokeMethod(this, "processNextTask", Qt::QueuedConnection);
    }
}

void BinanceManager::processNextTask()
{
    QMutexLocker locker(&m_queueMutex);
    if (m_taskQueue.isEmpty()) {
        m_isProcessing = false;
        return;
    }
    BinanceTask *task = m_taskQueue.dequeue();
    auto worker = m_workers.value(task->accountID());

    qDebug() << "Processing task for account" << task->accountID()
             << "Worker:" << (worker ? "found" : "not found");

    if (worker) {
        QMetaObject::invokeMethod(worker, "processTask", Qt::QueuedConnection, Q_ARG(BinanceTask*, task));
    } else {
        emit taskError(task->accountID(), "Worker not found", task->typeTask());
        task->deleteLater();
    }

    // Планируем обработку следующей задачи
    if (!m_taskQueue.isEmpty()) {
        QMetaObject::invokeMethod(this, "processNextTask", Qt::QueuedConnection);
    } else {
        m_isProcessing = false;
    }

}

void BinanceManager::handleTaskCompleted(int accountID, const QJsonObject &result, const TypeTask &tt)
{
    emit taskCompleted(accountID, result, tt);
}

void BinanceManager::handleTaskError(int accountID, const QString &error, const TypeTask &tt)
{
    emit taskError(accountID, error, tt);
}

void BinanceManager::clearWorkers()
{
    qDebug() << "Clearing existing workers...";

    m_isProcessing = false;

    QMutexLocker locker(&m_queueMutex);
    while(!m_taskQueue.isEmpty()){
        auto task = m_taskQueue.dequeue();
        task->deleteLater();
    }
    locker.unlock();

    for (auto thread : m_threads){
        thread->quit();
        thread->wait(1000);
        delete thread;
    }

    qDeleteAll(m_workers);
    m_workers.clear();
    m_threads.clear();
}

bool BinanceManager::createWorkers(const QList<D_Account *> &accounts)
{
    for (const auto& account : accounts){
        qDebug() << "Creating worker for account" << account->idx();

        auto worker = new TaskWorker(account);
        auto thread = new QThread(this);

        worker->moveToThread(thread);

        connect(worker, &TaskWorker::taskCompleted, this, &BinanceManager::taskCompleted);
        connect(worker, &TaskWorker::taskError, this, &BinanceManager::taskError);

        m_workers[account->idx()] = worker;
        m_threads[account->idx()] = thread;

        thread->start();
    }
    return true;
}
