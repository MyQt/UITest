#include "xmldatahandle.h"
#include <QMessageBox>
#include <QDomDocument>
#include <QApplication>
#include <QTextStream>
#include <QFile>

XmlDataHandle::XmlDataHandle()
{

}

bool XmlDataHandle::init(QString& infoName)
{
    mInfoName = infoName;

    return true;
}

bool XmlDataHandle::readInfo(QVector<foodInfo>& vecFoodInfo)
{
    QDomDocument reader;
    QFile file(mInfoName);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }
    if (!reader.setContent(&file)) {
        file.close();
        return false;
    }

    file.close();
    QDomElement root = reader.documentElement(); // 读取根节点
    QDomNode node = root.firstChild(); // 读取第一个子节点
    while(!node.isNull()) {
        QDomElement foodEle = node.toElement();

        if (foodEle.tagName().compare("FOOD")==0) {
            QDomAttr attrName = foodEle.attributeNode("NAME");
            QDomAttr attrIcon = foodEle.attributeNode("ICON");
            QDomAttr attrNote = foodEle.attributeNode("NOTE");
            foodInfo _info;
            _info.strName = attrName.value();
            _info.strIcon = attrIcon.value();
            _info.strNote = attrNote.value();
            vecFoodInfo.push_back(_info);
        }
        node = node.nextSibling(); // 读取下一个FOOD
    }


    return true;
}

bool XmlDataHandle::writeInfo(foodInfo &info)
{
    QDomDocument writer;
    QFile file(mInfoName);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }
    if (!writer.setContent(&file)) {
      file.close();
      return false;
    }
    file.close();
    /* 创建节点
     * <FOOD>
     *  <NAME></NAME>
     *  <ICON></ICON>
     *  <NOTE></NOTE>
     * </FOOD>
    */
    QDomElement root = writer.firstChildElement();
    QDomElement data = writer.createElement("FOOD");
    QDomAttr name = writer.createAttribute("NAME");
    name.setValue(info.strName);
    data.setAttributeNode(name);
    QDomAttr icon = writer.createAttribute("ICON");
    icon.setValue(info.strIcon);
    data.setAttributeNode(icon);

    QDomAttr note = writer.createAttribute("NOTE");
    note.setValue(info.strNote);
    data.setAttributeNode(note);

    root.appendChild(data);

    // 保存xml文件
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    QTextStream out(&file);
    writer.save(out, 4);
    file.close();

    return true;
}

bool XmlDataHandle::updateInfo(foodInfo &info, QString strOldName)
{
    QDomDocument reader;
    QFile file(mInfoName);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }
    if (!reader.setContent(&file)) {
        file.close();
        return false;
    }

    file.close();
    QDomElement root = reader.documentElement(); // 读取根节点
    QDomNode node = root.firstChild(); // 读取第一个子节点
    bool bUpdated = false;
    while(!node.isNull()) {
        QDomElement foodEle = node.toElement();

        if (foodEle.tagName().compare("FOOD")==0) {
            QDomAttr attrName = foodEle.attributeNode("NAME");
            if (attrName.value() == strOldName) {
                foodEle.setAttribute("ICON", info.strIcon);
                foodEle.setAttribute("NOTE", info.strNote);
                bUpdated = true;
                break;
            }
        }
        node = node.nextSibling(); // 读取下一个FOOD
    }
    if (bUpdated) {
        // 保存xml文件
        if (!file.open(QIODevice::WriteOnly)) {
            return false;
        }
        QTextStream out(&file);
        reader.save(out, 4);
        file.close();
    }

    return true;
}
