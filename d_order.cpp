#include "d_order.h"

D_Order::D_Order(const QString &orderId, int accId, const QString &createTime, const QString &symbol,
                 const QString &orderType, const QString &orderSide, const QString &orderPrice,
                 const QString &execQty, const QString &cumQty, const QString &orderFee, const QString &orderStatus,
                 const QString &orderStopPrice, QObject *parent) :
    m_orderId(orderId), m_accId(accId), m_createTime(createTime), m_symbol(symbol),
    m_orderType(orderType), m_orderSide(orderSide), m_orderPrice(orderPrice),
    m_execQty(execQty), m_cumQty(cumQty), m_orderFee(orderFee), m_orderStatus(orderStatus),
    m_orderStopPrice(orderStopPrice), QObject(parent)
{}

D_Order::D_Order(const QVariantMap &map, QObject *parent)
    : QObject{parent}
{
    m_orderId = map.value("orderId").toString();
    m_accId = map.value("accId").toInt();
    m_createTime = map.value("createTime").toString();
    m_symbol = map.value("symbol").toString();
    m_orderType = map.value("orderType").toString();
    m_orderSide = map.value("orderSide").toString();
    m_orderPrice = map.value("orderPrice").toString();
    m_execQty = map.value("execQty").toString();
    m_cumQty = map.value("cumQty").toString();
    m_orderFee = map.value("orderFee").toString();
    m_orderStatus = map.value("orderStatus").toString();
    m_orderStopPrice = map.value("orderStopPrice").toString();
}

QString D_Order::orderId() const
{
    return m_orderId;
}

void D_Order::setOrderId(const QString &newOrderId)
{
    if (m_orderId == newOrderId)
        return;
    m_orderId = newOrderId;
    emit orderIdChanged();
}

int D_Order::accId() const
{
    return m_accId;
}

void D_Order::setAccId(int newAccId)
{
    if (m_accId == newAccId)
        return;
    m_accId = newAccId;
    emit accIdChanged();
}

QString D_Order::createTime() const
{
    return m_createTime;
}

void D_Order::setCreateTime(const QString &newCreateTime)
{
    if (m_createTime == newCreateTime)
        return;
    m_createTime = newCreateTime;
    emit createTimeChanged();
}

QString D_Order::symbol() const
{
    return m_symbol;
}

void D_Order::setSymbol(const QString &newSymbol)
{
    if (m_symbol == newSymbol)
        return;
    m_symbol = newSymbol;
    emit symbolChanged();
}

QString D_Order::orderType() const
{
    return m_orderType;
}

void D_Order::setOrderType(const QString &newOrderType)
{
    if (m_orderType == newOrderType)
        return;
    m_orderType = newOrderType;
    emit orderTypeChanged();
}

QString D_Order::orderSide() const
{
    return m_orderSide;
}

void D_Order::setOrderSide(const QString &newOrderSide)
{
    if (m_orderSide == newOrderSide)
        return;
    m_orderSide = newOrderSide;
    emit orderSideChanged();
}

QString D_Order::orderPrice() const
{
    return m_orderPrice;
}

void D_Order::setOrderPrice(const QString &newOrderPrice)
{
    if (m_orderPrice == newOrderPrice)
        return;
    m_orderPrice = newOrderPrice;
    emit orderPriceChanged();
}

QString D_Order::execQty() const
{
    return m_execQty;
}

void D_Order::setExecQty(const QString &newExecQty)
{
    if (m_execQty == newExecQty)
        return;
    m_execQty = newExecQty;
    emit execQtyChanged();
}

QString D_Order::cumQty() const
{
    return m_cumQty;
}

void D_Order::setCumQty(const QString &newCumQty)
{
    if (m_cumQty == newCumQty)
        return;
    m_cumQty = newCumQty;
    emit cumQtyChanged();
}

QString D_Order::orderFee() const
{
    return m_orderFee;
}

void D_Order::setOrderFee(const QString &newOrderFee)
{
    if (m_orderFee == newOrderFee)
        return;
    m_orderFee = newOrderFee;
    emit orderFeeChanged();
}

QString D_Order::orderStatus() const
{
    return m_orderStatus;
}

void D_Order::setOrderStatus(const QString &newOrderStatus)
{
    if (m_orderStatus == newOrderStatus)
        return;
    m_orderStatus = newOrderStatus;
    emit orderStatusChanged();
}

QString D_Order::orderStopPrice() const
{
    return m_orderStopPrice;
}

void D_Order::setOrderStopPrice(const QString &newOrderStopPrice)
{
    if (m_orderStopPrice == newOrderStopPrice)
        return;
    m_orderStopPrice = newOrderStopPrice;
    emit orderStopPriceChanged();
}

QVariantMap D_Order::getMap()
{
    QVariantMap map;
    map.insert("orderId", m_orderId);
    map.insert("accId", m_accId);
    map.insert("createTime", m_createTime);
    map.insert("symbol", m_symbol);
    map.insert("orderType", m_orderType);
    map.insert("orderSide", m_orderSide);
    map.insert("orderPrice", m_orderPrice);
    map.insert("execQty", m_execQty);
    map.insert("cumQty", m_cumQty);
    map.insert("orderFee", m_orderFee);
    map.insert("orderStatus", m_orderStatus);
    map.insert("orderStopPrice", m_orderStopPrice);
    return map;
}
