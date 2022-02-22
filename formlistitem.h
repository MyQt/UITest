#ifndef FORMLISTITEM_H
#define FORMLISTITEM_H

#include <QWidget>

namespace Ui {
class Formlistitem;
}

class Formlistitem : public QWidget
{
    Q_OBJECT

public:
    explicit Formlistitem(QWidget *parent = nullptr);
    ~Formlistitem();
    void setResource(QString strName, QString strIcon, QString strNote);
private:
    Ui::Formlistitem *ui;
};

#endif // FORMLISTITEM_H
