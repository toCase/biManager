#include "m_account.h"

ModelAccount::ModelAccount(BinanceManager *bm, DatabaseWorker *dbw, ServiceManager *sm, QObject *parent)
    : binanceManager(bm),
    databaseWorker(dbw),
    serviceManager(sm), QAbstractListModel{parent}
{

    DATA.clear();
    QHash<QString, QVariant> filter;
    DATA = databaseWorker->getData<D_Account*>(1, filter);


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

    connect(serviceManager, &ServiceManager::readyAccountStatus, this, &ModelAccount::processAccountStatus);
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
    DATA = databaseWorker->getData<D_Account*>(1, filter);


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
    qDebug() << "SELECT: "<< row;
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
            if (acc->type() == "binance"){
                acc->setConnected(true);
                CONNECTED_LIST.append(acc);
            }
        }
    }

    if (!CONNECTED_LIST.isEmpty()){
        binanceManager->updateAccounts(CONNECTED_LIST);
    }


    emit endResetModel();
}

void ModelAccount::disconnectItem()
{
    emit beginResetModel();
    for (auto acc : DATA){
        if (acc->selected() && CONNECTED_LIST.contains(acc)) {
            acc->setConnected(false);
            CONNECTED_LIST.removeOne(acc);
        }
    }

    if (!CONNECTED_LIST.isEmpty()){
        binanceManager->updateAccounts(CONNECTED_LIST);
    }
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
    binanceManager->updateAccounts(CONNECTED_LIST);

    emit endResetModel();
}

D_Account* ModelAccount::getCard(int row)
{
    return DATA[row];
}

bool ModelAccount::saveItem(QVariantMap card)
{
    bool r = databaseWorker->setData(1, card);
    if (r){
        load();
    }

    // for (auto acc : DATA){
    //     if (CONNECTED_LIST.contains(acc->idx())) {
    //         acc->setConnected(false);
    //         CONNECTED_LIST.removeOne(acc->idx());
    //     }
    // }
    return true;
}

void ModelAccount::processAccountStatus(int accountID, const QString &status, double balance)
{
    emit beginResetModel();
    for (auto acc : DATA){
        if (acc->idx() == accountID) {
            acc->setStatus(status);
            acc->setBalance(QString("USDT: %1").arg(balance));
        }
    }
    emit endResetModel();
}
