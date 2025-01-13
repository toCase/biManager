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
