#ifndef FORMLISTITEMCREATE_H
#define FORMLISTITEMCREATE_H

#include <QWidget>

namespace Ui {
class Formlistitemcreate;
}

enum eItemCreateType
{
    EICT_Create = 0,
    EICT_Update = 1
};

class Formlistitemcreate : public QWidget
{
    Q_OBJECT

public:
    explicit Formlistitemcreate(QWidget *parent = nullptr);
    ~Formlistitemcreate();
    void setCreateType(eItemCreateType eType);
    void setResource(QString strName, QString strIcon, QString strNote);
    void disableKey();
private slots:
    void on_pushButton_icon_clicked();

    void on_pushButton_add_clicked();
signals:
    void addNewItem(QString, QString, QString);
    void updateItem(QString, QString, QString);

public:
    Ui::Formlistitemcreate *ui;
    QString mName;
    QString mIcon;
    QString mNote;
    eItemCreateType mType;
};

#endif // FORMLISTITEMCREATE_H
