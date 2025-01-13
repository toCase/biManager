#include "websocket_manager.h"

WebSocket_Manager::WebSocket_Manager(QObject *parent)
    : QObject{parent}
{

    QString key = "by7wJZ30NwQMCAlEtrBDvNCFQpx5yZFBHfHMoncLpQxwvMEZhihshZcebHldfyN7";
    QString secret = "Ha4ao8jGxCI4cRHfBsq0vndDfOW8LxIE8UsYXtN9DRjBKw8SoPshWds4PhUb7Vnp";

    // В главном классе
    QThread* wsThread = new QThread(this);
    WebSocker_Worker* worker = new WebSocker_Worker(key, secret);
    worker->moveToThread(wsThread);

    // Подключение сигналов и слотов
    connect(wsThread, &QThread::started, worker, &WebSocker_Worker::start);
    connect(wsThread, &QThread::finished, worker, &WebSocker_Worker::deleteLater);
    connect(worker, &WebSocker_Worker::messageReceived, this, &WebSocket_Manager::handleMessage);
    connect(worker, &WebSocker_Worker::error, this, &WebSocket_Manager::handleError);

    // Запуск потока
    wsThread->start();
}

void WebSocket_Manager::handleMessage(const QString &message)
{
    // qDebug() << "Message: " << message;

    QByteArray dataMessage = message.toUtf8();
    QJsonParseError parser;
    QJsonDocument jdoc = QJsonDocument::fromJson(dataMessage, &parser);

    if (parser.error != QJsonParseError::NoError) {
        qDebug() << "Error parse message: " << parser.errorString();
        return;
    }

    if (!jdoc.isObject()){
        qDebug() << "Message is no json ";
    }

    QJsonObject jobj = jdoc.object();

    double price = jobj["p"].toString().toDouble();
    emit updateBTC(price);

}

void WebSocket_Manager::handleError(const QString &error)
{
    qDebug() << "Error: " << error;
}
