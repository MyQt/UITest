#ifndef COMMON_H
#define COMMON_H
#include <QString>

struct foodInfo
{
    QString strName;
    QString strIcon;
    QString strNote;
};

enum handleType
{
    EHT_BEGIN = 0,
    EHT_XML,
    EHT_SQL,
    EHT_END
};

#endif // COMMON_H
