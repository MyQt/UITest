#include "formlistitemcreate.h"
#include "ui_formlistitemcreate.h"
#include <QMessageBox>
#include <QFileDialog>

Formlistitemcreate::Formlistitemcreate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formlistitemcreate)
{
    ui->setupUi(this);

    mType = EICT_Create;
}

void Formlistitemcreate::setCreateType(eItemCreateType eType)
{
    mType = eType;
    if (mType == EICT_Update) {
        ui->pushButton_add->setText("修改");
        disableKey();
    } else {
        ui->pushButton_add->setText("添加");
    }
}

Formlistitemcreate::~Formlistitemcreate()
{
    delete ui;
}

void Formlistitemcreate::setResource(QString strName, QString strIcon, QString strNote)
{
    ui->lineEdit_name->setText(strName);
    ui->textEdit_note->setText(strNote);
    QPixmap icon(strIcon);
    icon = icon.scaled(ui->pushButton_icon->width(), ui->pushButton_icon->height());
    ui->pushButton_icon->setText("");
    ui->pushButton_icon->setIcon(icon);
    mName = strName;
    mNote = strNote;
    mIcon = strIcon;
}

void Formlistitemcreate::disableKey()
{
    ui->lineEdit_name->setDisabled(true);
}

void Formlistitemcreate::on_pushButton_icon_clicked()
{
    // 添加食物图标
    QString file_name = QFileDialog::getOpenFileName(nullptr, tr("选择图标"),".", "*.png");
    if (file_name.isEmpty()) {
        QMessageBox::warning(nullptr, tr("警告"), tr("未选择合法图标"));
        return;
    }

    QIcon icon(file_name);
    ui->pushButton_icon->setText("");
    ui->pushButton_icon->setIcon(icon);
    mIcon = file_name.split("/").last();
}

void Formlistitemcreate::on_pushButton_add_clicked()
{
    // 添加新的食物条目到列表和配置文件
    mName = ui->lineEdit_name->text();
    mNote = ui->textEdit_note->toPlainText();
    if (mName.isEmpty()||mIcon.isEmpty()||mNote.isEmpty()) {
        QMessageBox::warning(nullptr, tr("警告"), tr("所有属性都不能为空"));
        return;
    }
    if (mType == EICT_Create) {
        emit addNewItem(mName, mIcon, mNote);
    } else {
        emit updateItem(mName, mIcon, mNote);
    }
    mName.clear();
    mNote.clear();
    mIcon.clear();
    ui->lineEdit_name->clear();
    ui->textEdit_note->clear();
    ui->pushButton_icon->setIcon(QIcon(""));
    ui->pushButton_icon->setText(tr("点击添加图标"));
    ui->lineEdit_name->setFocus();
}
