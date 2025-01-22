#include "m_account.h"
#include <exception>

ModelAccount::ModelAccount(BinanceManager *bm, DatabaseWorker *dbw, ServiceManager *sm, QObject *parent)
    : binanceManager(bm),
    databaseWorker(dbw),
    serviceManager(sm), QAbstractListModel{parent}
{

    DATA.clear();
    QHash<QString, QVariant> filter;
    DATA = databaseWorker->getData<D_Account*>(Tables::ACCOUNTS, filter);


    for (auto acc : DATA){
        if (acc->type() == "binance"){
            CONNECTED_LIST.append(acc);
        }
    }
    binanceManager->initialize(CONNECTED_LIST);

    for (auto acc : DATA){
        if (acc->type() == "binance"){
            serviceManager->getAccountStatus(acc->idx());
            statusCount += 1;
        }
    }

    connect(serviceManager, &ServiceManager::readyAccountStatus, this, &ModelAccount::processAccountStatus);
    connect(this, &ModelAccount::changeConnectList, binanceManager, &BinanceManager::updateAccounts);
}


int ModelAccount::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return DATA.count();
}

QVariant ModelAccount::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if (DATA.isEmpty()) return res;

    D_Account* acc = DATA.at(index.row());
    switch (role) {
    case ID:
        res = acc->idx();
        break;
    case NAME:
        res = acc->name();
        break;
    case TYPE:
        res = acc->type();
        break;
    case STATUS:
        res = acc->status();
        break;
    case CONNECTED:
        res = acc->connected() ? "Connected" : "";
        break;
    case BALANCE:
        res = acc->balance();
        break;
    case SELECTED:
        res = acc->selected();
        break;
    default:
        break;
    }
    return res;
}


QHash<int, QByteArray> ModelAccount::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ID] = "m_idx";
    roles[NAME] = "m_name";
    roles[TYPE] = "m_type";
    roles[STATUS] = "m_status";
    roles[CONNECTED] = "m_connected";
    roles[BALANCE] = "m_balance";
    roles[SELECTED] = "m_selected";
    return roles;
}

void ModelAccount::load()
{
    emit beginResetModel();
    DATA.clear();
    QHash<QString, QVariant> filter;
    DATA = databaseWorker->getData<D_Account*>(Tables::ACCOUNTS, filter);


    QList<D_Account*> tempList;
    for (auto acc : DATA){
        if (acc->type() == "binance"){
            tempList.append(acc);
        }
    }
    binanceManager->initialize(tempList);

    for (auto acc : DATA){
        if (acc->type() == "binance"){
            serviceManager->getAccountStatus(acc->idx());
        }
    }

    emit endResetModel();
}

void ModelAccount::selectItem(int row, bool s)
{
    D_Account* acc = DATA[row];
    acc->setSelected(s);
    DATA[row] = acc;
}

void ModelAccount::connectItem()
{
    emit beginResetModel();
    for (auto acc : DATA){
        if (acc->selected() && !CONNECTED_LIST.contains(acc) && acc->status() != "Invalid") {

            //TODO ONLY BINANCE
            acc->setSelected(false);
            if (acc->type() == "binance"){
                acc->setConnected(true);
                CONNECTED_LIST.append(acc);
            }
        }
    }

    emit changeConnectList(CONNECTED_LIST);
    for (auto acc: CONNECTED_LIST){
        serviceManager->getAccountOrders(acc->idx());
    }
    emit endResetModel();
}

void ModelAccount::disconnectItem()
{
    emit beginResetModel();
    for (auto acc : DATA){
        if (acc->selected() && CONNECTED_LIST.contains(acc)) {
            acc->setSelected(false);
            acc->setConnected(false);
            CONNECTED_LIST.removeOne(acc);

        }
    }
    emit changeConnectList(CONNECTED_LIST);
    emit endResetModel();
}

void ModelAccount::disconnectAll()
{
    emit beginResetModel();
    for (auto acc : DATA){
        if (CONNECTED_LIST.contains(acc)) {
            acc->setConnected(false);
        }
    }

    CONNECTED_LIST.clear();
    emit changeConnectList(CONNECTED_LIST);
    emit endResetModel();
}

D_Account* ModelAccount::getCard(int row)
{
    return DATA[row];
}

bool ModelAccount::saveItem(QVariantMap card)
{
    bool r = databaseWorker->setData(Tables::ACCOUNTS, card);
    if (r){
        int id = card.value("id").toInt();

        if (id == 0){

            // get id
            id = databaseWorker->lastId();
            card.insert("id", id);
            // make acc
            D_Account* acc = new D_Account(card);
            qDebug() << "Acc ID: " << acc->idx();
            // add acc in model
            DATA.append(acc);
            CONNECTED_LIST.append(acc);
        } else {
            for (auto acc : DATA) {
                if (acc->idx() == id) {
                    acc = new D_Account(card);
                    CONNECTED_LIST.append(acc);
                    qDebug() << "Acc ID: " << acc->idx();
                }
            }
        }
        // add acc to connected list
        binanceManager->updateAccounts(CONNECTED_LIST);
        statusCount += 1;
        serviceManager->getAccountStatus(id);

    }
    return true;
}

void ModelAccount::deleteItem()
{
    emit beginResetModel();
    bool changeConnected = false;

    QList<D_Account*> del_list;

    for (auto acc : DATA) {
        if (acc->selected()) {
            if (CONNECTED_LIST.contains(acc)){
                CONNECTED_LIST.removeOne(acc);
                changeConnected = true;
            }

            del_list.append(acc);

            bool r = databaseWorker->delData(Tables::ACCOUNTS, acc->idx());
            if (!r) {
                qDebug() << "ACCOUNT DELETE ERROR: " << databaseWorker->error();
                del_list.removeOne(acc);
            }
        }
    }

    for (auto acc : del_list) {
        DATA.removeOne(acc);
    }

    emit endResetModel();
    if (changeConnected){ changeConnectList(CONNECTED_LIST); }

}

void ModelAccount::updateModel()
{
    CONNECTED_LIST.clear();
    DATA.clear();
    QHash<QString, QVariant> filter;
    DATA = databaseWorker->getData<D_Account*>(1, filter);


    for (auto acc : DATA){
        if (acc->type() == "binance"){
            CONNECTED_LIST.append(acc);
        }
    }

    emit changeConnectList(CONNECTED_LIST);

    for (auto acc : DATA){
        if (acc->type() == "binance"){
            serviceManager->getAccountStatus(acc->idx());
            statusCount += 1;
        }
    }

}

void ModelAccount::getHistory()
{
    for (auto acc : DATA) {
        if (acc->selected()){
            if (acc->type() == "binance"){
                serviceManager->getAccountHistory(acc->idx());
            }
        }
    }
}

void ModelAccount::processAccountStatus(int accountID, const QString &status, double balance)
{
    emit beginResetModel();
    for (auto acc : DATA){
        if (acc->idx() == accountID) {
            qDebug() << "Acc - " << acc->name() << " - getted status";
            acc->setStatus(status);
            acc->setBalance(QString("USDT: %1").arg(balance));
            CONNECTED_LIST.removeOne(acc);
            statusCount -= 1;
            qDebug() << "Acc - " << acc->name() << " - delete from CONNECTED";
            qDebug() << "Acc is waiting count : " << statusCount;
        }
    }
    emit endResetModel();

    if (statusCount == 0) {
        emit changeConnectList(CONNECTED_LIST);
        qDebug() << "Finish getting status";
    }
}
