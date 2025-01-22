#pragma once

#include <QObject>
#include <QVariantMap>

class D_Order : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString orderId READ orderId WRITE setOrderId NOTIFY orderIdChanged FINAL);
    Q_PROPERTY(int accId READ accId WRITE setAccId NOTIFY accIdChanged FINAL);
    Q_PROPERTY(QString  createTime READ createTime WRITE setCreateTime NOTIFY createTimeChanged FINAL);
    Q_PROPERTY(QString symbol READ symbol WRITE setSymbol NOTIFY symbolChanged FINAL);
    Q_PROPERTY(QString orderType READ orderType WRITE setOrderType NOTIFY orderTypeChanged FINAL);
    Q_PROPERTY(QString orderSide READ orderSide WRITE setOrderSide NOTIFY orderSideChanged FINAL);
    Q_PROPERTY(QString orderPrice READ orderPrice WRITE setOrderPrice NOTIFY orderPriceChanged FINAL);
    Q_PROPERTY(QString execQty READ execQty WRITE setExecQty NOTIFY execQtyChanged FINAL);
    Q_PROPERTY(QString cumQty READ cumQty WRITE setCumQty NOTIFY cumQtyChanged FINAL);
    Q_PROPERTY(QString orderFee READ orderFee WRITE setOrderFee NOTIFY orderFeeChanged FINAL);
    Q_PROPERTY(QString orderStatus READ orderStatus WRITE setOrderStatus NOTIFY orderStatusChanged FINAL);
    Q_PROPERTY(QString orderStopPrice READ orderStopPrice WRITE setOrderStopPrice NOTIFY orderStopPriceChanged FINAL);


public:
    explicit D_Order(const QString &orderId, int accId, const QString &createTime, const QString &symbol,
                     const QString &orderType, const QString &orderSide, const QString &orderPrice, const QString &execQty,
                     const QString &cumQty, const QString &orderFee, const QString &orderStatus, const QString &orderStopPrice,
                     QObject *parent = nullptr);

    explicit D_Order(const QVariantMap &map, QObject *parent = nullptr);

    QString orderId() const;
    void setOrderId(const QString &newOrderId);

    int accId() const;
    void setAccId(int newAccId);

    QString createTime() const;
    void setCreateTime(const QString &newCreateTime);

    QString symbol() const;
    void setSymbol(const QString &newSymbol);

    QString orderType() const;
    void setOrderType(const QString &newOrderType);

    QString orderSide() const;
    void setOrderSide(const QString &newOrderSide);

    QString orderPrice() const;
    void setOrderPrice(const QString &newOrderPrice);

    QString execQty() const;
    void setExecQty(const QString &newExecQty);

    QString cumQty() const;
    void setCumQty(const QString &newCumQty);

    QString orderFee() const;
    void setOrderFee(const QString &newOrderFee);

    QString orderStatus() const;
    void setOrderStatus(const QString &newOrderStatus);

    QString orderStopPrice() const;
    void setOrderStopPrice(const QString &newOrderStopPrice);

    QVariantMap getMap();

signals:
    void orderIdChanged();
    void accIdChanged();
    void createTimeChanged();
    void symbolChanged();
    void orderTypeChanged();
    void orderSideChanged();
    void orderPriceChanged();
    void execQtyChanged();
    void cumQtyChanged();
    void orderFeeChanged();
    void orderStatusChanged();
    void orderStopPriceChanged();

private:
    QString m_orderId;
    int m_accId;
    QString m_createTime;
    QString m_symbol;
    QString m_orderType;
    QString m_orderSide;
    QString m_orderPrice;
    QString m_execQty;
    QString m_cumQty;
    QString m_orderFee;
    QString m_orderStatus;
    QString m_orderStopPrice;
};
