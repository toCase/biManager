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

void ServiceManager::getAccountOrders(int accountID)
{
    m_BinanceManager->addTask(new BinanceTask(TypeTask::GET_ALL_ORDERS, accountID));
}

void ServiceManager::handleBinanceResponce(int accountID, const QJsonObject &result, const TypeTask& tt)
{
    if (tt == TypeTask::GET_BALANCE) {
        // qDebug() << "Task completed for account: " << accountID << " : " << result;
        processAccountStatus(accountID, result);
    } else if (tt == TypeTask::GET_ALL_ORDERS) {
        processAccountOrders(accountID, result);
    }

}

void ServiceManager::handleBinanceError(int accountID, const QString &error, const TypeTask &tt)
{
    qDebug() << "Task error for account: " << accountID << " : " << error;
    if (tt == TypeTask::GET_BALANCE) {
        emit readyAccountStatus(accountID, "Invalid", 0.00);
    } if (tt == TypeTask::GET_ALL_ORDERS) {
        qDebug() << "GET ORDER ACCOUNT ID: " << accountID << " ERROR: " << error;
    }
}

void ServiceManager::processAccountStatus(int accountID, const QJsonObject &result)
{
    double balance = 0.00;
    QString status = "Invalid";


    if (!result.contains("balance") || result["balances"].isArray()) {
        emit readyAccountStatus(accountID, status, balance);
    }

    //del assets
    m_DatabaseWorker->delData(Tables::ASSETS, accountID);

    QJsonArray js_balances = result["balances"].toArray();
    for (const QJsonValue& item : js_balances) {
        QJsonObject js_item = item.toObject();

        QString item_asset = js_item["asset"].toString();
        double item_balance = js_item["free"].toString().toDouble();

        if (item_balance > 0.00){
            D_Asset card(accountID, item_asset, item_balance, 0.00);
            m_DatabaseWorker->setData(Tables::ASSETS, card.getMap());
        }

        if (js_item["asset"].toString() == "USDT"){
            balance = js_item["free"].toString().toDouble();
            status = "";
        }
    }

    emit readyAccountStatus(accountID, status, balance);

}

void ServiceManager::processAccountOrders(int accountID, const QJsonObject &result)
{
    qDebug() << "ORDERS FROM ACCOUNT ID: " << accountID;
    qDebug() << QJsonDocument(result).toJson(QJsonDocument::Compact);
    qDebug() << "-----------------------------------------";

}

