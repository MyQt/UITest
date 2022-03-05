#include "mainwindow.h"
#include "ui_mainwindow.h"

const int dockOffsetX = 32;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    setWindowFlags(Qt::Window | Qt::WindowTitleHint    | Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::Window );
    ui->setupUi(this);
    showRightUI(e_convert);
    ui->lcdNumber_2->setDigitCount(8);
    ui->lcdNumber_2->setPalette(Qt::cyan);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    initPropertyAnimation();

}

void MainWindow::initPropertyAnimation()
{
    mAnimationFlag = false;
    // dock animation
    mDockAnimation = new QPropertyAnimation(ui->dockWidget_2,"geometry");
    mDockAnimation->setEasingCurve(QEasingCurve::InOutQuint);
    mDockAnimation->setDuration(1000);
    // right animation
    mRightAnimation = new QPropertyAnimation(ui->widget_right,"geometry");
    mRightAnimation->setEasingCurve(QEasingCurve::InOutQuint);
    mRightAnimation->setDuration(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showRightUI(EUI_Type euiType)
{
    ui->widget_convert->setVisible(euiType == e_convert);
    ui->calendarWidget->setVisible(euiType == e_calander);
    ui->widget_player->setVisible(euiType == e_mediaplayer);
    ui->widget_list->setVisible(euiType == e_complex_list);

}

void MainWindow::showTime()
{
    QTime tim = QTime::currentTime(); // 获取当地时间
    QString timeStr = tim.toString(("hh:mm:ss")); // 设置时间格式
    ui->lcdNumber_2->display(timeStr); // 显示时间
}

void MainWindow::on_pushButton_convert_2_clicked()
{
    showRightUI(e_convert);
}

void MainWindow::on_pushButton_Calander_2_clicked()
{
    showRightUI(e_calander);
}


void MainWindow::on_pushButton_player_2_clicked()
{
    showRightUI(e_mediaplayer);
}

void MainWindow::on_pushButton_list_2_clicked()
{
    showRightUI(e_complex_list);
}

void MainWindow::startPropertyAnimation()
{
    if(mAnimationFlag) //侧边栏隐藏状态，显示
    {
        mDockAnimation->setStartValue(QRect(-ui->dockWidget_2->width()+dockOffsetX,0,ui->dockWidget_2->width(),height()));
        mDockAnimation->setEndValue(QRect(0,0,ui->dockWidget_2->width(),height()));
        mRightAnimation->setStartValue(QRect(dockOffsetX,0,ui->widget_right->width(), height()));
        mRightAnimation->setEndValue(QRect(ui->dockWidget_2->width(),0,ui->widget_right->width()-ui->dockWidget_2->width()+dockOffsetX,height()));
    }
    else //侧边栏显示状态，隐藏
    {
        mDockAnimation->setStartValue(QRect(0,0,ui->dockWidget_2->width(),height()));
        mDockAnimation->setEndValue(QRect(-ui->dockWidget_2->width()+dockOffsetX,0,ui->dockWidget_2->width(),height()));
        mRightAnimation->setStartValue(QRect(ui->dockWidget_2->width(),0,ui->widget_right->width(), height()));
        mRightAnimation->setEndValue(QRect(dockOffsetX,0,ui->dockWidget_2->width()+ui->widget_right->width()-dockOffsetX, height()));
    }
    mDockAnimation->start();
    mRightAnimation->start();
    mAnimationFlag = !mAnimationFlag;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    startPropertyAnimation();
}
