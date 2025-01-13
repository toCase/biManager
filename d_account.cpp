#include "d_account.h"

D_Account::D_Account(int idx, const QString& name, const QString& type,
                     const QString& api, const QString& secret, bool selected,
                     bool connected, const QString& status, const QString& balance, QObject *parent)
    : _idx(idx), _name(name), _type(type), _api(api), _secret(secret),
    _selected(selected), _connected(connected), _status(status), _balance(balance), QObject{parent}
{}

void D_Account::setid(int idx)
{
    if (idx != _idx) {_idx = idx; emit idChanged(); }
}

void D_Account::setName(const QString &name)
{
    if (name != _name) {_name = name; emit nameChanged(); }
}

void D_Account::setType(const QString &type)
{
    if (type != _type) {_type = type; emit typeChanged(); }
}

void D_Account::setApi(const QString &api)
{
    if (api != _api) {_api = api; emit apiChanged(); }
}

void D_Account::setSecret(const QString &secret)
{
    if (secret != _secret) {_secret = secret; emit secretChanged(); }
}

void D_Account::setSelected(bool selected)
{
    if (selected != _selected) {_selected = selected; emit selectedChanged(); }
}

void D_Account::setConnected(bool connected)
{
    if (connected != _connected) {_connected = connected; emit connectedChanged(); }
}

void D_Account::setStatus(const QString &status)
{
    if (status != _status) {_status = status; emit statusChanged(); }
}

void D_Account::setBalance(const QString &balance)
{
    if (balance != _balance) {_balance = balance; emit balanceChanged(); }
}

QVariantMap D_Account::getMap()
{
    QVariantMap map;
    map.insert("id", _idx);
    map.insert("name", _name);
    map.insert("type", _type);
    map.insert("api", _api);
    map.insert("secret", _secret);
    map.insert("selected", _selected);
    map.insert("connected", _connected);
    map.insert("status", _status);
    map.insert("balance", _balance);
    return map;
}

void D_Account::fromMap(QVariantMap map)
{
    _idx = map.value("id").toInt();
    _name = map.value("name").toString();
    _type = map.value("type").toString();
    _api = map.value("api").toString();
    _secret = map.value("secret").toString();
    _selected = map.value("selected", false).toBool();
    _connected = map.value("connected", false).toBool();
    _status = map.value("status").toString();
    _balance = map.value("balance").toString();
}
