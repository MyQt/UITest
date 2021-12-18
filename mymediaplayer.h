#ifndef MYMEDIAPLAYER_H
#define MYMEDIAPLAYER_H

#include <QWidget>

namespace Ui {
class MyMediaPlayer;
}

class MyMediaPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MyMediaPlayer(QWidget *parent = nullptr);
    ~MyMediaPlayer();

private:
    Ui::MyMediaPlayer *ui;
};

#endif // MYMEDIAPLAYER_H
