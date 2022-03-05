#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include<QPropertyAnimation>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef enum
    {
        e_convert = 0,
        e_calander = 1,
        e_mediaplayer = 2,
        e_complex_list = 3,
        e_end
    }EUI_Type;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void showTime();
    void on_pushButton_Calander_2_clicked();

    void on_pushButton_convert_2_clicked();

    void on_pushButton_player_2_clicked();

    void on_pushButton_list_2_clicked();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:
    void showRightUI(EUI_Type euiType);
    void initPropertyAnimation();
    void startPropertyAnimation();
private:
    Ui::MainWindow *ui;
    QTimer* timer;
    bool mAnimationFlag;
    QPropertyAnimation* mDockAnimation;
    QPropertyAnimation* mRightAnimation;

};
#endif // MAINWINDOW_H
