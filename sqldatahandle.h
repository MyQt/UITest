#ifndef SQLDATAHANDLE_H
#define SQLDATAHANDLE_H
#include "DataHandle.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class SqlDataHandle : public DataHandle
{
public:
    SqlDataHandle();
    ~SqlDataHandle();
    virtual bool init(QString& infoName) override;
    virtual bool readInfo(QVector<foodInfo>& vecFoodInfo) override;
    virtual bool writeInfo(foodInfo &info) override;
    virtual void close() override;
    bool initDatabase();
private:
    QSqlDatabase mDatabase;
    QSqlQuery*   mSqlQuery;
};

#endif // SQLDATAHANDLE_H
