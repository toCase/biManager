#pragma once

#include <QObject>
#include <QtSql/QtSql>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

#include "app_namespaces.h"
#include "db_mapper.h"

using namespace App;

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);



private:
    int VERSION = 0;
    int DB_VERSION = 0;
    bool NET = false;

    // SettingManager settingManager;
    QSqlDatabase db;

signals:

protected slots:
    void load();
    void disconnect();
    int getDBVersion();
};


class DatabaseWorker : public QObject {
    Q_OBJECT

public:
    explicit DatabaseWorker(QString connection, QObject *parent = nullptr);


    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged FINAL);
    Q_PROPERTY(int lastId READ lastId WRITE setLastId NOTIFY lastIdChanged FINAL);


    bool NET = false;

    // AppEnums Enums;
    // SettingManager settingManager;
    int _lastId = 0;
    QSqlDatabase db;
    QString _error;


    template <typename T>
    QList<T> getData(int table, QHash<QString, QVariant> filter) {
        QList<T> list;

        if (db.isOpen()) {
            QSqlQuery query(db);
            switch (table) {
            case Tables::ACCOUNTS:
                query.prepare(R"(SELECT * FROM Accounts)");
                break;
            case Tables::ASSETS:
                query.prepare(R"(SELECT * FROM AccountAssets AS T WHERE T.acc_id = :acc_id)");
                query.bindValue(":acc_id", filter.value("acc_id"));
                break;
            default:
                break;
            }
            if (!query.exec()) return list;

            while (query.next()) {
                list.append(DataMapper<T>::map(query.record()));
            }
        }
        return list;
    }

    bool setData(int table, QVariantMap data);
    bool delData(int table, int id);

    void setError(const QString& e);
    QString error();

    void setLastId(int lastId);
    int lastId();

signals:
    void errorChanged(const QString& error);
    void lastIdChanged(int lastId);
};
