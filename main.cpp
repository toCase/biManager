#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
// #include <QThread>
#include "websocket_manager.h"
#include "databasemanager.h"
#include "binance_manager.h"
#include "service_manager.h"
#include "m_account.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    // Включаем отладочные сообщения
    qSetMessagePattern("[%{time h:mm:ss.zzz}] [%{type}] %{message}");

    WebSocket_Manager *websocketManager = new WebSocket_Manager();
    DatabaseManager *db_manager;
    BinanceManager *binanceManager = new BinanceManager();
    DatabaseWorker *databaseWorker = new DatabaseWorker("app");
    ServiceManager *serviceManager = new ServiceManager(websocketManager, binanceManager, databaseWorker);
    QQmlApplicationEngine engine;

    ModelAccount *modelAccount = new ModelAccount(binanceManager, databaseWorker, serviceManager);
    engine.rootContext()->setContextProperty("modelAccount", modelAccount);

    engine.rootContext()->setContextProperty("websocketManager", websocketManager);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("TestBinance", "Main");

    return app.exec();
}
