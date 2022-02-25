#ifndef FORMLIST_H
#define FORMLIST_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include "formlistitemcreate.h"
#include "common.h"
#include "DataHandle.h"

namespace Ui {
class FormList;
}

enum eItemType {
  EIT_Show = 0,
  EIT_Create,
  EIT_Update
};

struct roleData{
    foodInfo _foodInfo;
    int nType;

    void setData(foodInfo& info, int _type)
    {
        _foodInfo = info;
        nType = _type;
    }
};

Q_DECLARE_METATYPE(roleData)

class FormList : public QWidget
{
    Q_OBJECT

public slots:
    void addNewItemSlot(QString, QString, QString);
    void UpdateItemSlot(QString, QString, QString);
public:
    explicit FormList(QWidget *parent = nullptr);
    ~FormList();
    void initUI();
    bool initHandleFactory();
    bool readInfo();
    bool writeInfo(foodInfo& info);
    bool addAllItem();
    bool insertItem(foodInfo& info, int index);
    bool insertItemCreate(int index);
    void insertItemUpdate(foodInfo& info,int nIndex);
private slots:
    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::FormList *ui;
    QVector<foodInfo> mVecFoodInfo;
    Formlistitemcreate mItemCreate;
    Formlistitemcreate* mItemUpdate;
    QMap<handleType, DataHandle*> mMapDataHandle;
    int mUpdateIndex; // 更新的列表项
};

#endif // FORMLIST_H
