#pragma once

#include <QObject>
#include <QWebSocket>
#include <QThread>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageAuthenticationCode>

class WebSocker_Worker : public QObject
{
    Q_OBJECT
public:
    explicit WebSocker_Worker(const QString &apiKey, const QString &secretKey, QObject *parent = nullptr);
    ~WebSocker_Worker();

    void connectToStream();
    void disconnect();

public slots:
    void start();
    void stop();

signals:
    void connected();
    void disconnected();
    void messageReceived(const QString &message);
    void error(const QString &errorString);

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString& message);
    void onError(QAbstractSocket::SocketError error);

private:
    QString m_apiKey;
    QString m_secretKey;
    QWebSocket *m_webSocket;
    bool m_isRunning;

    void setupWebSocker();
    QString generateSignature(const QString& data);
};
