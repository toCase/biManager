#pragma once

#include <QObject>
#include <QVariantMap>

class D_Asset : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int acc_idx READ acc_idx WRITE setAcc_idx NOTIFY acc_idxChanged FINAL)
    Q_PROPERTY(QString asset READ asset WRITE setAsset NOTIFY assetChanged FINAL)
    Q_PROPERTY(double amount READ amount WRITE setAmount NOTIFY amountChanged FINAL)
    Q_PROPERTY(double price READ price WRITE setPrice NOTIFY priceChanged FINAL)


public:
    explicit D_Asset(int acc_idx, const QString& asset, double amount, double price, QObject *parent = nullptr);


    int _acc_idx = 0;
    QString _asset;
    double _amount = 0.00;
    double _price = 0.00;

    int acc_idx() const { return _acc_idx; }
    QString asset() const { return _asset; }
    double amount() const { return _amount; }
    double price() const { return _price; }

    void setAcc_idx(int acc_idx);
    void setAsset(const QString& asset);
    void setAmount(double amount);
    void setPrice(double price);

    QVariantMap getMap();
    void fromMap(QVariantMap map);

signals:

    void acc_idxChanged();
    void assetChanged();
    void amountChanged();
    void priceChanged();

};
