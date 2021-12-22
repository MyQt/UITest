#include "mymediaplayer.h"
#include "ui_mymediaplayer.h"
#include <QStandardItemModel>

MyMediaPlayer::MyMediaPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyMediaPlayer)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(this);

    for (int i = 0; i < 10; i++) {
        QStandardItem* item = new QStandardItem("列表文件"+QString::number(i));
        model->appendRow(item);
    }
    ui->listView_play->setModel(model);
}

MyMediaPlayer::~MyMediaPlayer()
{
    delete ui;
}
