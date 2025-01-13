#include "service_manager.h"

ServiceManager::ServiceManager(WebSocket_Manager *websocketManager,
                               BinanceManager *binanceManager,
                               DatabaseWorker *databaseWorker, QObject *parent)
    : m_DatabaseWorker(databaseWorker),
    m_WebSocketManager(websocketManager),
    m_BinanceManager(binanceManager), QObject(parent)
{
    connect(m_BinanceManager, &BinanceManager::taskCompleted, this, &ServiceManager::handleBinanceResponce);
    connect(m_BinanceManager, &BinanceManager::taskError, this, &ServiceManager::handleBinanceError);
}

void ServiceManager::getAccountStatus(int accountID)
{
    m_BinanceManager->addTask(new BinanceTask(TypeTask::GET_BALANCE, accountID));
}

void ServiceManager::handleBinanceResponce(int accountID, const QJsonObject &result, const TypeTask& tt)
{
    if (tt == TypeTask::GET_BALANCE) {
        // qDebug() << "Task completed for account: " << accountID << " : " << result;
        processAccountStatus(accountID, result);


    }

}

void ServiceManager::handleBinanceError(int accountID, const QString &error, const TypeTask &tt)
{
    qDebug() << "Task error for account: " << accountID << " : " << error;
    if (tt == TypeTask::GET_BALANCE) {
        emit readyAccountStatus(accountID, "Invalid", 0.00);
    }
}

void ServiceManager::processAccountStatus(int accountID, const QJsonObject &result)
{
    double balance = 0.00;
    QString status = "Invalid";


    if (!result.contains("balance") || result["balances"].isArray()) {
        emit readyAccountStatus(accountID, status, balance);
    }

    QJsonArray js_balances = result["balances"].toArray();
    for (const QJsonValue& item : js_balances) {
        QJsonObject js_item = item.toObject();
        if (js_item["asset"].toString() == "USDT"){
            balance = js_item["free"].toString().toDouble();
            status = "";
            break;
        }
    }

    emit readyAccountStatus(accountID, status, balance);

}

