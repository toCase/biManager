#pragma once

#include <QObject>
#include <QJsonObject>

enum class TypeTask {
    GET_STATUS,
    GET_BALANCE,
};


class BinanceTask : public QObject {
    Q_OBJECT
public:


    BinanceTask(TypeTask typeTask, int accountID, const QJsonObject& params = QJsonObject())
        : m_typeTask(typeTask), m_accountID(accountID), m_params(params) {}

    TypeTask typeTask() const { return m_typeTask; }
    int accountID() const { return m_accountID; }
    QJsonObject params() const { return m_params; }

private:
    TypeTask m_typeTask;
    int m_accountID;
    QJsonObject m_params;
};
