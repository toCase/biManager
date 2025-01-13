#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include "websocker_worker.h"

class WebSocket_Manager : public QObject
{
    Q_OBJECT
public:
    explicit WebSocket_Manager(QObject *parent = nullptr);


    void handleMessage(const QString& message);
    void handleError(const QString& error);
signals:

    void updateBTC(double price);

};
