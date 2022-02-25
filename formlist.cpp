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
    mUpdateIndex = -1;
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

void FormList::UpdateItemSlot(QString strName, QString strIcon, QString strNote)
{
    foodInfo info;
    info.strName = strName;
    info.strIcon = strIcon.split("/").last();
    info.strNote = strNote;

    // 更新修改的控件
   QWidget*pWidget = ui->listWidget->itemWidget(ui->listWidget->item(mUpdateIndex-1));
   QString strOldName;
   if (pWidget != nullptr) {
       strOldName = ((Formlistitem*)pWidget)->getKey();
       ((Formlistitem*)pWidget)->setResource(strName, strIcon, strNote);
   }
   // 删除更新编辑控件
   QListWidgetItem* pRemoveItem = ui->listWidget->takeItem(mUpdateIndex);
   delete pRemoveItem;
   pRemoveItem = nullptr;
   mUpdateIndex = -1;
   // 写入修改的数据
   foreach(auto item, mMapDataHandle) {
       item->updateInfo(info, strOldName);
   }
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
    QVariant var;
    roleData role;
    role.setData(info, EIT_Show);
    var.setValue(role);
    pItem->setData(Qt::UserRole, var);
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
    pItem->setData(Qt::UserRole, EIT_Create);
    ui->listWidget->insertItem(index, pItem);

    ui->listWidget->setItemWidget(pItem, &mItemCreate);

    return true;
}

void FormList::insertItemUpdate(foodInfo& info, int nIndex)
{
    // 将更新条目放入选中行的下面
    QVariant var;
    roleData role;
    role.setData(info, EIT_Update);
    var.setValue(role);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(QSize(852,170));
    pItem->setData(Qt::UserRole, var);
    ui->listWidget->insertItem(nIndex, pItem);
    mItemUpdate = new Formlistitemcreate();
    mItemUpdate->setCreateType(EICT_Update);
    mItemUpdate->setResource(info.strName, QApplication::applicationDirPath()+strIconPath+info.strIcon, info.strNote);
    connect(mItemUpdate, &Formlistitemcreate::updateItem, this, &FormList::UpdateItemSlot);

    ui->listWidget->setItemWidget(pItem, mItemUpdate);

}

bool FormList::addAllItem()
{
    // 添加所有食物配置项到列表显示
    for(int i = 0; i < mVecFoodInfo.size(); i++) {
        insertItem(mVecFoodInfo[i], i);
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

void FormList::on_listWidget_currentRowChanged(int currentRow)
{

    roleData _data = ui->listWidget->currentItem()->data(Qt::UserRole).value<roleData>();

    if (_data.nType == EIT_Show && mUpdateIndex-1 != currentRow) {

        insertItemUpdate(_data._foodInfo, currentRow+1);
        if (mUpdateIndex != -1) {
            if (mUpdateIndex > currentRow) { // 插入到了之前更新元素的前面
                QListWidgetItem* pRemoveItem = ui->listWidget->takeItem(mUpdateIndex+1);
                delete pRemoveItem;
                pRemoveItem = nullptr;
                mUpdateIndex = currentRow + 1;
            } else { // 插入到了后面
                QListWidgetItem* pRemoveItem = ui->listWidget->takeItem(mUpdateIndex);
                delete pRemoveItem;
                pRemoveItem = nullptr;
                mUpdateIndex = currentRow;
            }
        } else {
            mUpdateIndex = currentRow + 1;
        }
    }

}
