#include "d_asset.h"

D_Asset::D_Asset(int acc_idx, const QString &asset, double amount, double price, QObject *parent) :
    _acc_idx(acc_idx), _asset(asset), _amount(amount), _price(price), QObject(parent)
{}

void D_Asset::setAcc_idx(int acc_idx)
{
    if (acc_idx != _acc_idx) { _acc_idx = acc_idx; emit acc_idxChanged(); }
}

void D_Asset::setAsset(const QString &asset)
{
     if (asset != _asset) {_asset = asset; emit assetChanged(); }
}

void D_Asset::setAmount(double amount)
{
     if (amount != _amount) {_amount = amount; emit amountChanged(); }
}

void D_Asset::setPrice(double price)
{
     if (price != _price) {_price = price; emit priceChanged(); }
}

QVariantMap D_Asset::getMap()
{
    QVariantMap map;
    map.insert("acc_idx", _acc_idx);
    map.insert("asset", _asset);
    map.insert("amount", _amount);
    map.insert("price", _price);
    return map;
}

void D_Asset::fromMap(QVariantMap map)
{
    _acc_idx = map.value("acc_id").toInt();
    _asset = map.value("asset").toString();
    _amount = map.value("amount", 0.00).toDouble();
    _price = map.value("price", 0.00).toDouble();
}
