#ifndef XMLDATAHANDLE_H
#define XMLDATAHANDLE_H
#include "DataHandle.h"

class XmlDataHandle : public DataHandle
{
public:
    XmlDataHandle();
    virtual bool init(QString& infoName) override;
    virtual bool readInfo(QVector<foodInfo>& vecFoodInfo) override;
    virtual bool writeInfo(foodInfo &info) override;
    virtual bool updateInfo(foodInfo& info, QString strOldName) override;
};

#endif // XMLDATAHANDLE_H
