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

bool FormList::reloadInfo()
{
    mUpdateIndex = -1;
    DataHandle* pHandle = nullptr;
    mVecFoodInfo.clear();
    if (ui->radioButton_database->isChecked()) {
        pHandle = mMapDataHandle.find(EHT_SQL).value();
    } else if (ui->radioButton_xml->isChecked()) {
        pHandle = mMapDataHandle.find(EHT_XML).value();
    }
    if (pHandle == nullptr) {
        QMessageBox::critical(this, tr("致命错误"), tr("不支持的读取方式"));
        return false;
    }
    if (!pHandle->readInfo(mVecFoodInfo)) {
        QMessageBox::critical(this, tr("致命错误"), tr("读取配置信息失败"));
        return false;
    }


    if (!addAllItem()) {
        return false;
    }
    EditElementVisible();

    return true;
}

bool FormList::initHandleFactory()
{
    QString strInfoNames[EHT_END-1] = {"info.xml", "info.db"};
    for(int i = EHT_XML; i < EHT_END; i++) {
        DataHandle* pHandle = HandleFactory::getInstance()->createProduct((handleType)i);
        QString strInfoName = QApplication::applicationDirPath()+strConfigPath+strInfoNames[i-1];
        if (!pHandle->init(strInfoName)) {
            QMessageBox::critical(this, tr("致命错误"), tr("初始化配置信息失败"));
            delete pHandle;
            return false;
        }
        mMapDataHandle.insert((handleType)i, pHandle);

    }

    return true;
}

void FormList::initUI()
{
    if (!initHandleFactory() || !reloadInfo()) {
        QApplication::exit(0);
        return;
    }

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
    QVariant var;
    roleData role;
    foodInfo info;
    role.setData(info, EIT_Create);
    var.setValue(role);
    pItem->setData(Qt::UserRole, var);
    ui->listWidget->insertItem(index, pItem);
    mItemCreate = new Formlistitemcreate();
    connect(mItemCreate, &Formlistitemcreate::addNewItem, this, &FormList::addNewItemSlot);

    ui->listWidget->setItemWidget(pItem, mItemCreate);

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
    ui->listWidget->clear();
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
    if (currentRow == -1) {
        return;
    }
    roleData _data = ui->listWidget->currentItem()->data(Qt::UserRole).value<roleData>();
    if (ui->checkBox_bEdit->isChecked()) {
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
}

void FormList::EditElementVisible()
{
    QListWidgetItem* pUpdateItem = ui->listWidget->item(mUpdateIndex);
    QListWidgetItem* pCreateItem = ui->listWidget->item(ui->listWidget->count()-1);
    if (ui->checkBox_bEdit->isChecked()) { // 允许编辑
        pCreateItem->setHidden(false);
        if (pUpdateItem != nullptr) {
            pUpdateItem->setHidden(false);
        }
    } else {
        pCreateItem->setHidden(true);
        if (pUpdateItem != nullptr) {
            pUpdateItem->setHidden(true);
        }
    }
}

void FormList::on_checkBox_bEdit_stateChanged(int arg1)
{
    EditElementVisible();
}

void FormList::on_radioButton_database_clicked()
{
    reloadInfo();
}

void FormList::on_radioButton_xml_clicked()
{
    reloadInfo();
}
