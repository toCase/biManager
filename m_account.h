#pragma once

#include <QAbstractListModel>
#include <QObject>

#include "databasemanager.h"
#include "binance_manager.h"
#include "service_manager.h"

class ModelAccount : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ModelAccount(BinanceManager *bm, DatabaseWorker *dbw, ServiceManager *sm, QObject *parent = nullptr);

private:
    enum RoleNames : int {
        ID = 0,
        NAME = 1,
        TYPE = 2,
        STATUS = 3,
        CONNECTED = 4,
        BALANCE = 5,
        SELECTED = 6
    };

    QList<D_Account*> DATA;
    QList<D_Account*> CONNECTED_LIST;
    DatabaseWorker *databaseWorker;
    BinanceManager *binanceManager;
    ServiceManager *serviceManager;

    int statusCount = 0;

public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;


    Q_INVOKABLE void load();
    Q_INVOKABLE void selectItem(int row, bool s);
    Q_INVOKABLE void connectItem();
    Q_INVOKABLE void disconnectItem();
    Q_INVOKABLE void disconnectAll();
    Q_INVOKABLE D_Account *getCard(int row);
    Q_INVOKABLE bool saveItem(QVariantMap card);
    Q_INVOKABLE void deleteItem();
    Q_INVOKABLE void updateModel();

    Q_INVOKABLE void getHistory();

private slots:
    void processAccountStatus(int accountID, const QString& status, double balance);

signals:
    void changeConnectList(QList<D_Account*> connect_list);


};
