#include "mymediaplayer.h"
#include "ui_mymediaplayer.h"

MyMediaPlayer::MyMediaPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyMediaPlayer)
{
    ui->setupUi(this);
}

MyMediaPlayer::~MyMediaPlayer()
{
    delete ui;
}
