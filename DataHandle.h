#ifndef DATAHANDLE_H
#define DATAHANDLE_H
#include "common.h"

class DataHandle
{
protected:
    DataHandle(){};
public:
    virtual ~DataHandle() {};
public:
    virtual bool init(QString& infoName) = 0;
    virtual bool readInfo(QVector<foodInfo>& vecFoodInfo) = 0;
    virtual bool writeInfo(foodInfo& info) = 0;
    virtual bool updateInfo(foodInfo& info, QString strOldName) = 0;
    virtual void close() {};
protected:
    QString mInfoName;
};

#endif // DATAHANDLE_H
