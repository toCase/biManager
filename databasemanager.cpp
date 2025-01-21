#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent}
{
    VERSION = 1; //settingManager.get("database", "version").toInt();
    NET = false; //settingManager.get("database", "net").toBool();

    if (NET){
        db = QSqlDatabase::addDatabase("QMARIADB");
        // setting for connect to MARIA
    } else {
        QString filePath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/db.db3");
        db = QSqlDatabase::addDatabase("QSQLITE", "app");
        db.setDatabaseName(filePath);
        db.open();
    }
    load();
}

void DatabaseManager::load()
{
    QStringList queryList;
    queryList.append("CREATE TABLE IF NOT EXISTS db_version (version INTEGER UNIQUE);");

    DB_VERSION = getDBVersion();

    qDebug() << "DB: " << DB_VERSION;

    if (DB_VERSION > 0 && DB_VERSION < VERSION){
        // change db

    } else {
        queryList.append("CREATE TABLE IF NOT EXISTS Accounts (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "name TEXT, type TEXT, api TEXT, secret TEXT);");

        queryList.append("CREATE TABLE IF NOT EXISTS AccountAssets (acc_id INTEGER, asset TEXT, amount TEXT, price TEXT);");

        // create all actual tables


    }

    queryList.append(QString("INSERT OR REPLACE INTO db_version (version) VALUES (\'%1\');").arg(VERSION));

    for (int i = 0; i < queryList.count(); i++){
        QSqlQuery query(queryList.at(i), db);
        query.exec();
        if (query.lastError().isValid()) {
            qDebug() << query.lastError().text();
            qDebug() << query.lastQuery();
        }
    }

    disconnect();

}

void DatabaseManager::disconnect()
{
    db = QSqlDatabase::database("app");
    if (db.isOpen()){
        db.close();
    }
}

int DatabaseManager::getDBVersion()
{
    QSqlQuery query_version("SELECT version FROM db_version", db);
    if (query_version.next()) {
        return query_version.value(0).toInt();
    } else {
        return 0;
    }
}


DatabaseWorker::DatabaseWorker(QString connection, QObject *parent): QObject{parent} {
    NET = false; //settingManager.get("database", "net").toBool();

    if (NET){
        db = QSqlDatabase::addDatabase("QMARIADB");
        // setting for connect to MARIA
    } else {
        QString filePath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/db.db3");
        db = QSqlDatabase::addDatabase("QSQLITE", connection);
        db.setDatabaseName(filePath);
        db.open();
    }
}

bool DatabaseWorker::setData(int table, QVariantMap data)
{
    if (!db.isOpen()) return false;

    int id = data.value("id").toInt();
    QSqlQuery query(db);
    if (id == 0) {
        switch (table) {
        case Tables::ACCOUNTS:
            query.prepare(R"(INSERT INTO Accounts (name, type, api, secret) VALUES (:name, :type, :api, :secret))");
            query.bindValue(":name", data.value("name"));
            query.bindValue(":type", data.value("type"));
            query.bindValue(":api", data.value("api"));
            query.bindValue(":secret", data.value("secret"));
            break;

        default:
            break;
        }
    } else {
        switch (table) {
        case Tables::ACCOUNTS:
            query.prepare(R"(UPDATE Accounts AS T SET name = :name, type = :type, api = :api, secret = :secret WHERE T.id = :id )");
            query.bindValue(":name", data.value("name"));
            query.bindValue(":type", data.value("type"));
            query.bindValue(":api", data.value("api"));
            query.bindValue(":secret", data.value("secret"));
            query.bindValue(":id", data.value("id"));
            break;
        default:
            break;
        }
    }

    if (table == Tables::ASSETS) {
        query.prepare(R"(INSERT INTO AccountAssets (acc_id, asset, amount, price)
                            VALUES (:acc_id, :asset, :amount, :price))");
        query.bindValue(":acc_id", data.value("acc_idx"));
        query.bindValue(":asset", data.value("asset"));
        query.bindValue(":amount", data.value("amount"));
        query.bindValue(":price", data.value("price"));
    }

    bool res = query.exec();    
    if (!res) {
        setError(QString("DB Save error: %1").arg(query.lastError().text()));
    } else {
        if (id == 0) {
            setLastId(query.lastInsertId().toInt());
        } else {
            setLastId(id);
        }
    }
    return res;
}

bool DatabaseWorker::delData(int table, int id)
{
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    switch (table) {
    case Tables::ACCOUNTS:
        query.prepare(R"(DELETE FROM Accounts AS T WHERE T.id = :id)");
        query.bindValue(":id", id);
        break;
    case Tables::ASSETS:
        query.prepare(R"(DELETE FROM AccountAssets AS T WHERE T.acc_id = :acc_id)");
        query.bindValue(":acc_id", id);
        break;
    default:
        break;
    }

    bool res = query.exec();
    if (!res) {
        setError(QString("DB Delete error: %1").arg(query.lastError().text()));
    }
    return res;
}

void DatabaseWorker::setError(const QString &e)
{
    if (e != _error) {
        _error = e;
        emit errorChanged(_error);
    }
}

QString DatabaseWorker::error()
{
    return _error;
}

void DatabaseWorker::setLastId(int lastId)
{
    if (lastId != _lastId){
        _lastId = lastId;
        emit lastIdChanged(_lastId);
    }
}

int DatabaseWorker::lastId()
{
    return _lastId;
}

