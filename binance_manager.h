#pragma once
#include <QObject>
#include <QList>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageAuthenticationCode>
#include <QJsonDocument>

#include "d_account.h"
#include "BinanceTask.h"


class TaskWorker : public QObject {
    Q_OBJECT
public:
    explicit TaskWorker(D_Account *account, QObject *parent = nullptr);
    ~TaskWorker();

public slots:
    void processTask(BinanceTask *task);

signals:
    void taskCompleted(int accountID, const QJsonDocument& result, const TypeTask& tt);
    void taskError(int accountID, const QString& error, const TypeTask& tt);

private:
    D_Account *m_account;
    QNetworkAccessManager *m_manager;
    // const QString BASE_URL = "https://testnet.binance.vision";
    const QString BASE_URL = "https://api.binance.com";

    QString generateSignature(const QString& queryString);
    QNetworkRequest createAuthrizedRequest(const QString& endpoint, const QString& queryString = QString());

    void handleGetBalance();
    void handleGetOrders();
    void handleGetHistory(const QVariantHash& params);

};


class BinanceManager : public QObject
{
    Q_OBJECT
public:
    explicit BinanceManager(QObject *parent = nullptr);
    ~BinanceManager();

    bool initialize(const QList<D_Account*> accounts);
    bool updateAccounts(const QList<D_Account*> accounts);
    void addTask(BinanceTask* task);

signals:
    void taskCompleted(int accountID, const QJsonDocument& result, const TypeTask& tt);
    void taskError(int accountID, const QString& error, const TypeTask& tt);
    void workerError(int accountID, const QString& error);

    void accountUpdated();
    void accountUpdatedError(const QString& error);

private slots:
    void processNextTask();
    void handleTaskCompleted(int accountID, const QJsonDocument& result, const TypeTask& tt);
    void handleTaskError(int accountID, const QString& error, const TypeTask& tt);

private:
    QList<D_Account*> m_accounts;
    QMap<int, TaskWorker*> m_workers;
    QMap<int, QThread*> m_threads;
    QQueue<BinanceTask*> m_taskQueue;
    QMutex m_queueMutex;
    bool m_isProcessing;

    void clearWorkers();
    bool createWorkers(const QList<D_Account*>& accounts);
};

