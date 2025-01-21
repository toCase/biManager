#pragma once

#include <QObject>
#include <QVariantHash>

enum class TypeTask {
    GET_STATUS,
    GET_BALANCE,
    GET_ALL_ORDERS,
    GET_HISTORY,
};


class BinanceTask : public QObject {
    Q_OBJECT
public:


    BinanceTask(TypeTask typeTask, int accountID, const QVariantHash& params = QVariantHash())
        : m_typeTask(typeTask), m_accountID(accountID), m_params(params) {}

    TypeTask typeTask() const { return m_typeTask; }
    int accountID() const { return m_accountID; }
    QVariantHash params() const { return m_params; }

private:
    TypeTask m_typeTask;
    int m_accountID;
    QVariantHash m_params;
};
