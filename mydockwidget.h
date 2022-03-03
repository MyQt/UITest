#ifndef MYDOCKWIDGET_H
#define MYDOCKWIDGET_H
#include<QDockWidget>
#include<QPushButton>
#include <QTimer>
#include <QResizeEvent>

class MyDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    MyDockWidget(QWidget* parent = nullptr);
    void initUI();
private slots:
    void onTitleReleased();
signals:
    void dockFold(bool bFold);
private:
    void setTitleButtonIcon();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    virtual void resizeEvent(QResizeEvent* event);
private:
    QPushButton* mTitleButton; // 标题栏
    bool mFold;
};

#endif // MYDOCKWIDGET_H
