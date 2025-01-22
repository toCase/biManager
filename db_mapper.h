#pragma once

#include <QSqlRecord>
#include <QVariant>
#include "d_account.h"
#include "d_asset.h"
#include "d_order.h"

template <typename T>
class DataMapper {
public:
    static T map(const QSqlRecord& record);
};


template<>
class DataMapper<D_Account*> {
public:
    static D_Account* map(const QSqlRecord& record);
};

template<>
class DataMapper<D_Asset*> {
public:
    static D_Asset* map(const QSqlRecord& record);
};

template<>
class DataMapper<D_Order*> {
    static D_Order* map(const QSqlRecord& record);
};
