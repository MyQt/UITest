#include "handlefactory.h"
#include "xmldatahandle.h"
#include "sqldatahandle.h"

HandleFactory::HandleFactory()
{

}

DataHandle* HandleFactory::createProduct(handleType type)
{
    switch (type) {
    case EHT_XML:
        return new XmlDataHandle();
        break;
    case EHT_SQL:
        return new SqlDataHandle();
        break;
    default:
        break;
    }

    return  nullptr;
}


