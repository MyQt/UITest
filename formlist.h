#ifndef FORMLIST_H
#define FORMLIST_H

#include <QWidget>
#include <QVector>
#include "formlistitemcreate.h"

namespace Ui {
class FormList;
}

class FormList : public QWidget
{
    Q_OBJECT
public:
    struct foodInfo
    {
        QString strName;
        QString strIcon;
        QString strNote;
    };
public slots:
    void addNewItemSlot(QString, QString, QString);
public:
    explicit FormList(QWidget *parent = nullptr);
    ~FormList();
    void initUI();
    bool readInfoXml();
    bool writeInfoXml(foodInfo& info);
    bool addAllItem();
    bool insertItem(foodInfo& info, int index);
    bool insertItemCreate(int index);
private:
    Ui::FormList *ui;
    QVector<foodInfo> mVecFoodInfo;
    Formlistitemcreate mItemCreate;
};

#endif // FORMLIST_H
