#pragma once

#include <QSqlRecord>
#include <QVariant>
#include "d_account.h"

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
