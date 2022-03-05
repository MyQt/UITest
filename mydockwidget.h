#ifndef MYDOCKWIDGET_H
#define MYDOCKWIDGET_H
#include<QDockWidget>
#include<QPushButton>
#include <QTimer>
#include <QResizeEvent>
#include <QtWidgets>
#include <QPropertyAnimation>

class MyDockWidget : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize mDockSize READ dockSize WRITE setDockSize)
public:
    MyDockWidget(QWidget* parent = nullptr);
    void initUI();
private slots:
    void onTitleReleased();
signals:
    void dockFold(bool bFold);
private:
    void setTitleButtonIcon();
    void setDockSize(const QSize& _size);
    QSize dockSize();
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
private:
    QPushButton* mTitleButton; // 标题栏
    bool mMaxState;
    QPropertyAnimation*     mPropertyAnimation;
    QSize mDockSize;
};

#endif // MYDOCKWIDGET_H
