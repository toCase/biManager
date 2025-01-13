#include "websocker_worker.h"

WebSocker_Worker::WebSocker_Worker(const QString &apiKey, const QString &secretKey, QObject *parent)
    : QObject{parent}
    , m_apiKey(apiKey)
    , m_secretKey(secretKey)
    , m_webSocket(nullptr)
    , m_isRunning(false)
{}

WebSocker_Worker::~WebSocker_Worker()
{
    stop();
}

void WebSocker_Worker::connectToStream()
{
    // Пример URL для spot рынка
    QString wsUrl = "wss://stream.binance.com:9443/ws/btcusdt@trade";
    m_webSocket->open(QUrl(wsUrl));
}

void WebSocker_Worker::disconnect()
{
    if (m_webSocket && m_webSocket->state() == QAbstractSocket::ConnectedState) {
        m_webSocket->close();
    }
}

void WebSocker_Worker::start()
{
    if (!m_isRunning) {
        m_isRunning = true;
        setupWebSocker();
        connectToStream();
    }
}

void WebSocker_Worker::stop()
{
    if (m_isRunning) {
        m_isRunning = false;
        disconnect();
    }
}

void WebSocker_Worker::onConnected()
{
    // Подписка на стрим после подключения
    QJsonObject subscribeMsg;
    subscribeMsg["method"] = "SUBSCRIBE";
    subscribeMsg["params"] = QJsonArray{"btcusdt@trade"};
    subscribeMsg["id"] = 1;

    m_webSocket->sendTextMessage(QJsonDocument(subscribeMsg).toJson());
    emit connected();
}

void WebSocker_Worker::onDisconnected()
{
    emit disconnected();

    // Попытка переподключения если работа не остановлена
    if (m_isRunning) {
        QThread::sleep(5); // Пауза перед переподключением
        connectToStream();
    }
}

void WebSocker_Worker::onTextMessageReceived(const QString &message)
{
    emit messageReceived(message);
}

void WebSocker_Worker::onError(QAbstractSocket::SocketError error)
{
    emit this->error(m_webSocket->errorString());
}

void WebSocker_Worker::setupWebSocker()
{
    if (m_webSocket) {
        delete m_webSocket;
    }

    m_webSocket = new QWebSocket();

    connect(m_webSocket, &QWebSocket::connected, this, &WebSocker_Worker::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &WebSocker_Worker::onDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocker_Worker::onTextMessageReceived);
    connect(m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &WebSocker_Worker::onError);
}

QString WebSocker_Worker::generateSignature(const QString &data)
{
    QByteArray signature = QMessageAuthenticationCode::hash(
                               data.toUtf8(),
                               m_secretKey.toUtf8(),
                               QCryptographicHash::Sha256
                               ).toHex();
    return QString(signature);
}
