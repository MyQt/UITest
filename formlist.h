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

class FormList : public QWidget
{
    Q_OBJECT

public slots:
    void addNewItemSlot(QString, QString, QString);
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
private:
    Ui::FormList *ui;
    QVector<foodInfo> mVecFoodInfo;
    Formlistitemcreate mItemCreate;
    QMap<handleType, DataHandle*> mMapDataHandle;
};

#endif // FORMLIST_H
