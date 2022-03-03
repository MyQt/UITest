#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    setWindowFlags(Qt::Window | Qt::WindowTitleHint    | Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::Window );
    ui->setupUi(this);
    ui->dockWidget->initUI();
    showRightUI(e_convert);
    ui->lcdNumber->setDigitCount(8);
    ui->lcdNumber->setPalette(Qt::cyan);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    connect(ui->dockWidget, &MyDockWidget::dockFold, this, &MainWindow::onDockFold);
    timer->start(1000);
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
    ui->lcdNumber->display(timeStr); // 显示时间
}

void MainWindow::on_pushButton_convert_clicked()
{
    showRightUI(e_convert);
}

void MainWindow::on_pushButton_Calander_clicked()
{
    showRightUI(e_calander);
}


void MainWindow::on_pushButton_player_clicked()
{
    showRightUI(e_mediaplayer);
}

void MainWindow::on_pushButton_list_clicked()
{
    showRightUI(e_complex_list);
}

void MainWindow::onDockFold(bool bFold)
{
    int offsetX = bFold ? -100 : 100;
    QList<QDockWidget*> listDocks{ui->dockWidget};
    QList<int> listSizes{offsetX};
    resizeDocks(listDocks, listSizes, Qt::Horizontal);
//    ui->dockWidget->resize(ui->dockWidget->width()+offsetX, height());
//    ui->centralwidget->resize(ui->centralwidget->width() - offsetX, ui->centralwidget->height());
//    ui->centralwidget->move(offsetX, 0);
}

