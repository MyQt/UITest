#include "sqldatahandle.h"
#include <QVariant>
#include <QDebug>

SqlDataHandle::SqlDataHandle()
{
    mSqlQuery = nullptr;
}

SqlDataHandle::~SqlDataHandle()
{
}

bool SqlDataHandle::init(QString &infoName)
{
    mInfoName = infoName;
    if (!initDatabase()) {
        return false;
    }

    return true;
}

bool SqlDataHandle::initDatabase()
{
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.setDatabaseName(mInfoName);
    if (!mDatabase.open()) {
        // 创建或打开失败
        return false;
    }
    ; // 连接数据库
    QString strQuery = "CREATE TABLE IF NOT EXISTS FOOD("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "NAME VARCHAR(256) NOT NULL,"
                       "ICON VARCHAR(256) NOT NULL,"
                       "NOTE VARCHAR(2048) NOT NULL)";
    mSqlQuery = new QSqlQuery(mInfoName); // 连接数据库
    if (!mSqlQuery->exec(strQuery)) {
        qDebug()<<"last error\n";
        qDebug()<<mSqlQuery->lastError();
        return false;
    }

    return true;
}

bool SqlDataHandle::readInfo(QVector<foodInfo> &vecFoodInfo)
{
    // 查询数据
    if (!mSqlQuery->exec("select* from FOOD")) {
        return false;
    } else {
        while (mSqlQuery->next()) {
            foodInfo info;
            info.strName = mSqlQuery->value("NAME").toString();
            info.strIcon = mSqlQuery->value("ICON").toString();
            info.strNote = mSqlQuery->value("NOTE").toString();
            vecFoodInfo.push_back(info);
            qDebug()<<info.strName<<" "<<info.strIcon<<" "<<info.strNote;
        }
    }

    return true;
}

bool SqlDataHandle::writeInfo(foodInfo &info)
{
    QString strInsert = QString("insert into FOOD(NAME,ICON,NOTE) VALUES('%1','%2','%3')").arg(info.strName,info.strIcon,info.strNote);
    if (!mSqlQuery->exec(strInsert)) {
        qDebug()<<"insert failure: "<<mSqlQuery->lastError();
        return false;
    }

    return true;
}

bool SqlDataHandle::updateInfo(foodInfo &info, QString strOldName)
{
    QString strUpdate = QString("update FOOD set NAME='%1',ICON='%2',NOTE='%3' WHERE NAME = '%4'").arg(info.strName, info.strIcon, info.strNote, strOldName);
    if (!mSqlQuery->exec(strUpdate)) {
        qDebug()<<"update failure: "<<mSqlQuery->lastError();
        return false;
    }
    return true;
}

void SqlDataHandle::close()
{
    if (mSqlQuery != nullptr) {
        mSqlQuery->clear();
        delete mSqlQuery;
    }
    mDatabase.close();
}
