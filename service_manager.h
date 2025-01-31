#pragma once

#include "binance_manager.h"
#include "databasemanager.h"
#include "websocket_manager.h"
#include <QObject>

class ServiceManager : public QObject
{
    Q_OBJECT
public:
    explicit ServiceManager(WebSocket_Manager *websocketManager, BinanceManager *binanceManager, DatabaseWorker *databaseWorker, QObject *parent = nullptr);

    void getAccountStatus(int accountID);
    void getAccountOrders(int accountID);
    void getAccountHistory(int accountID);

signals:
    void readyAccountStatus(int accountID, const QString& status, double balance);
    void readyAccountOrders();

private slots:
    void handleBinanceResponce(int accountID, const QJsonDocument& result, const TypeTask& tt);
    void handleBinanceError(int accountID, const QString& error, const TypeTask& tt);

private:
    WebSocket_Manager *m_WebSocketManager;
    DatabaseWorker *m_DatabaseWorker;
    BinanceManager *m_BinanceManager;

    void processAccountStatus(int accountID, const QJsonDocument& result);
    void processAccountOrders(int accountID, const QJsonDocument& result);
    void processAccountHistory(int accountID, const QJsonDocument& result);


};
