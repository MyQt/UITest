#include "formlist.h"
#include "ui_formlist.h"
#include <QMessageBox>
#include <QTextStream>
#include "formlistitem.h"
#include "handlefactory.h"
#include <QApplication>
#include <QPair>

const QString strConfigPath = "/config/";
const QString strIconPath = "/listitemicons/";

FormList::FormList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormList)
{
    ui->setupUi(this);
    initUI();
}

bool FormList::initHandleFactory()
{
    for(int i = EHT_XML; i < EHT_END; i++) {
        mMapDataHandle.insert((handleType)i, HandleFactory::getInstance()->createProduct((handleType)i));
    }

    return true;
}

void FormList::initUI()
{
    QString strInfoNames[EHT_END-1] = {"info.xml", "info.db"};
    bool bRead = false;
    initHandleFactory();
    for (int i = EHT_XML;i<EHT_END;i++) {
            QString strInfoName = QApplication::applicationDirPath()+strConfigPath+strInfoNames[i-1];
            auto iter = mMapDataHandle.find((handleType)i);
            if (!iter.value()->init(strInfoName)) {
                QMessageBox::critical(this, tr("致命错误"), tr("初始化或读取配置信息失败"));
                return;
            }
            if (/*!bRead && */i==EHT_SQL && !iter.value()->readInfo(mVecFoodInfo)) {
                QMessageBox::critical(this, tr("致命错误"), tr("初始化或读取配置信息失败"));
                return;
            }

            bRead = true;
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
    writeInfo(info);
}

bool FormList::readInfo()
{
    return true;
}

bool FormList::writeInfo(foodInfo& info)
{
    for (auto iter = mMapDataHandle.begin(); iter != mMapDataHandle.end(); iter++) {
        if(!iter.value()->writeInfo(info)) {
            QMessageBox::critical(this, tr("致命错误"), tr("写入条目失败"));
            return false;
        }
    }

    return true;
}

bool FormList::insertItem(foodInfo& info, int index)
{
    // 添加新建条目框
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(QSize(852,128));
    ui->listWidget->insertItem(index, pItem);
    Formlistitem* pListItem = new Formlistitem();
    pListItem->setResource(info.strName, QApplication::applicationDirPath()+strIconPath+info.strIcon, info.strNote);
    ui->listWidget->setItemWidget(pItem, pListItem);

    return true;
}

bool FormList::insertItemCreate(int index)
{
    // 添加新建条目框
    QListWidgetItem* pItem = new QListWidgetItem(ui->listWidget);
    pItem->setSizeHint(QSize(852,170));
    ui->listWidget->insertItem(index, pItem);

    ui->listWidget->setItemWidget(pItem, &mItemCreate);

    return true;
}

bool FormList::addAllItem()
{
    // 添加所有食物配置项到列表显示
    foreach(auto item, mVecFoodInfo) {
       QListWidgetItem* pItem = new QListWidgetItem(ui->listWidget);
       pItem->setSizeHint(QSize(852,128));
       ui->listWidget->addItem(pItem);
       Formlistitem* pListItem = new Formlistitem(ui->listWidget);
       pListItem->setResource(item.strName, QApplication::applicationDirPath()+strIconPath+item.strIcon, item.strNote);
       ui->listWidget->setItemWidget(pItem, pListItem);
    }

    insertItemCreate(ui->listWidget->count());

    return true;
}

FormList::~FormList()
{
    delete ui;
    for (auto iter = mMapDataHandle.begin(); iter != mMapDataHandle.end(); iter++) {
        iter.value()->close();
        delete iter.value();
    }
}
