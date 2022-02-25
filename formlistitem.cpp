#include "formlistitem.h"
#include "ui_formlistitem.h"
#include <QPixmap>

Formlistitem::Formlistitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formlistitem)
{
    ui->setupUi(this);
}

Formlistitem::~Formlistitem()
{
    delete ui;
}

QString Formlistitem::getKey()
{
    return ui->label_name->text();
}

void Formlistitem::setResource(QString strName, QString strIcon, QString strNote)
{
    ui->label_name->setText(strName);
    ui->textBrowser_note->setText(strNote);
    QPixmap icon(strIcon);
    icon = icon.scaled(ui->label_icon->width(), ui->label_icon->height());
    ui->label_icon->setPixmap(icon);
}
