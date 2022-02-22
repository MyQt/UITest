#include "formlist.h"
#include "ui_formlist.h"
#include <QDomDocument>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "formlistitem.h"


FormList::FormList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormList)
{
    ui->setupUi(this);
    initUI();
}

void FormList::initUI()
{
    if (!readInfoXml()) {
        return;
    }
    if (!addAllItem()) {
        return;
    }
    connect(&mItemCreate, &Formlistitemcreate::addNewItem, this, &FormList::addNewItemSlot);
}

void FormList::addNewItemSlot(QString strName, QString strIcon, QString strNote)
{
    // 插入新的食物条目
    // 保存新的食物条目到配置文件
    foodInfo info;
    info.strName = strName;
    info.strIcon = strIcon;
    info.strNote = strNote;
    insertItem(info, ui->listWidget->count()-1);
    writeInfoXml(info);
}

bool FormList::readInfoXml()
{
    QDomDocument reader;
    QFile file(QApplication::applicationDirPath()+"/listitemicons/info.xml");
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::critical(this,tr("Error"),
                              tr("读取配置文件%1失败").arg(file.fileName()));
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
            QDomAttr attrName = foodEle.attributeNode(tr("NAME"));
            QDomAttr attrIcon = foodEle.attributeNode(tr("ICON"));
            QDomAttr attrNote = foodEle.attributeNode(tr("NOTE"));
            foodInfo _info;
            _info.strName = attrName.value();
            _info.strIcon = QApplication::applicationDirPath()+"/listitemicons/"+attrIcon.value();
            _info.strNote = attrNote.value();
            mVecFoodInfo.push_back(_info);
        }
        node = node.nextSibling(); // 读取下一个FOOD
    }

    return true;
}

bool FormList::writeInfoXml(foodInfo& info)
{
    QDomDocument writer;
    QFile file(QApplication::applicationDirPath()+"/listitemicons/info.xml");
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::critical(this,tr("Error"),
                              tr("读取配置文件%1失败").arg(file.fileName()));
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
        QMessageBox::critical(this,tr("Error"),
                              tr("读取配置文件%1失败").arg(file.fileName()));
        return false;
    }
    QTextStream out(&file);
    writer.save(out, 4);
    file.close();

    return true;
}

bool FormList::insertItem(foodInfo& info, int index)
{
    // 添加新建条目框
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(QSize(852,128));
    ui->listWidget->insertItem(index, pItem);
    Formlistitem* pListItem = new Formlistitem();
    pListItem->setResource(info.strName, QApplication::applicationDirPath()+"/listitemicons/"+info.strIcon, info.strNote);
    ui->listWidget->setItemWidget(pItem, pListItem);
}

bool FormList::insertItemCreate(int index)
{
    // 添加新建条目框
    QListWidgetItem* pItem = new QListWidgetItem(ui->listWidget);
    pItem->setSizeHint(QSize(852,170));
    ui->listWidget->insertItem(index, pItem);

    ui->listWidget->setItemWidget(pItem, &mItemCreate);
}

bool FormList::addAllItem()
{
    // 添加所有食物配置项到列表显示
    foreach(auto item, mVecFoodInfo) {
       QListWidgetItem* pItem = new QListWidgetItem(ui->listWidget);
       pItem->setSizeHint(QSize(852,128));
       ui->listWidget->addItem(pItem);
       Formlistitem* pListItem = new Formlistitem(ui->listWidget);
       pListItem->setResource(item.strName, item.strIcon, item.strNote);
       ui->listWidget->setItemWidget(pItem, pListItem);
    }

    insertItemCreate(ui->listWidget->count());

    return true;
}

FormList::~FormList()
{
    delete ui;
}
