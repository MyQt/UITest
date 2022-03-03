#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>

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
    void on_pushButton_convert_clicked();

    void on_pushButton_Calander_clicked();

    void on_pushButton_player_clicked();

    void on_pushButton_list_clicked();
    void onDockFold(bool bFold);
private:
    void showRightUI(EUI_Type euiType);

private:
    Ui::MainWindow *ui;
    QTimer* timer;


};
#endif // MAINWINDOW_H
