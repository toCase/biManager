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

D_Order *DataMapper<D_Order *>::map(const QSqlRecord &record)
{
    return new D_Order(
        record.value("order_id").toString(),
        record.value("acc_id").toInt(),
        record.value("create_time").toString(),
        record.value("symbol").toString(),
        record.value("order_type").toString(),
        record.value("order_side").toString(),
        record.value("order_price").toString(),
        record.value("exec_qty").toString(),
        record.value("cum_qty").toString(),
        record.value("order_fee").toString(),
        record.value("order_status").toString(),
        record.value("order_stop_price").toString()
    );
}
