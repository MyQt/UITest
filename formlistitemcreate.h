#ifndef FORMLISTITEMCREATE_H
#define FORMLISTITEMCREATE_H

#include <QWidget>

namespace Ui {
class Formlistitemcreate;
}

class Formlistitemcreate : public QWidget
{
    Q_OBJECT

public:
    explicit Formlistitemcreate(QWidget *parent = nullptr);
    ~Formlistitemcreate();

private slots:
    void on_pushButton_icon_clicked();

    void on_pushButton_add_clicked();
signals:
    void addNewItem(QString, QString, QString);

public:
    Ui::Formlistitemcreate *ui;
    QString mName;
    QString mIcon;
    QString mNote;
};

#endif // FORMLISTITEMCREATE_H
