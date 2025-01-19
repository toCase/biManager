#include "db_mapper.h"

D_Account *DataMapper<D_Account *>::map(const QSqlRecord &record)
{
    return new D_Account(
        record.value("id").toInt(),
        record.value("name").toString(),
        record.value("type").toString(),
        record.value("api").toString(),
        record.value("secret").toString(),
        false,
        false,
        QString(),
        QString()
    );
}

D_Asset *DataMapper<D_Asset *>::map(const QSqlRecord &record)
{
    return new D_Asset(
        record.value("acc_id").toInt(),
        record.value("asset").toString(),
        record.value("amount").toDouble(),
        record.value("price").toDouble()
    );
}
