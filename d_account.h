#pragma once
#include <QObject>
#include <QVariantMap>

class D_Account : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int idx READ idx WRITE setid NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged FINAL)
    Q_PROPERTY(QString api READ api WRITE setApi NOTIFY apiChanged FINAL)
    Q_PROPERTY(QString secret READ secret WRITE setSecret NOTIFY secretChanged FINAL)


    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged FINAL)
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged FINAL)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(QString balance READ balance WRITE setBalance NOTIFY balanceChanged FINAL)

public:
    explicit D_Account(int idx, const QString& name, const QString& type,
                       const QString& api, const QString& secret, bool selected,
                       bool connected, const QString& status, const QString& balance, QObject *parent = nullptr);

    int _idx = 0;
    QString _name;
    QString _type;
    QString _api;
    QString _secret;
    bool _selected;
    bool _connected;
    QString _status;
    QString _balance;

    int idx() const { return _idx; };
    QString name() const { return _name; }
    QString type() const { return _type; }
    QString api() const { return _api; }
    QString secret() const { return _secret; }
    bool selected() const { return _selected; }
    bool connected() const { return _connected; }
    QString status() const { return _status; }
    QString balance() const { return _balance; }

    void setid(int idx);
    void setName(const QString& name);
    void setType(const QString& type);
    void setApi(const QString& api);
    void setSecret(const QString& secret);
    void setSelected(bool selected);
    void setConnected(bool connected);
    void setStatus(const QString& status);
    void setBalance(const QString& balance);

    QVariantMap getMap();
    void fromMap(QVariantMap map);

signals:
    void idChanged();
    void nameChanged();
    void typeChanged();
    void apiChanged();
    void secretChanged();
    void selectedChanged();
    void connectedChanged();
    void statusChanged();
    void balanceChanged();
};
