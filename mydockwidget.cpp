#include "mydockwidget.h"
#include <QApplication>

MyDockWidget::MyDockWidget(QWidget* parent)
    :QDockWidget(parent),
     mTitleButton(nullptr),
     mFold(false)
{

}

void MyDockWidget::initUI()
{

    mTitleButton = new QPushButton(this);
    connect(mTitleButton, SIGNAL(released()), this, SLOT(onTitleReleased()));
    mTitleButton->setMinimumSize(96, 96);
    mTitleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setTitleBarWidget(mTitleButton);
    setTitleButtonIcon();
}



void MyDockWidget::setTitleButtonIcon()
{
    QIcon icon;
    if (!mFold) { // 收起
         icon = QIcon(QString::fromUtf8(":/icons/fold_96px.png"));
    } else { // 展开
        icon = QIcon(QString::fromUtf8(":/icons/unfold_96px.png"));
    }
    mTitleButton->setIcon(icon);
    mTitleButton->setIconSize(QSize(width(),96));
}

void MyDockWidget::onTitleReleased()
{
    emit dockFold(mFold);
    mFold = !mFold;
}


void MyDockWidget::enterEvent(QEvent *event)
{
    show();
    QDockWidget::enterEvent(event);

}

void MyDockWidget::leaveEvent(QEvent *event)
{
    showMinimized();
    QDockWidget::leaveEvent(event);
}

void MyDockWidget::resizeEvent(QResizeEvent *event)
{
//    int width = this->width();

//    mTitleButton->setIconSize(QSize(width, 96));
    QDockWidget::resizeEvent(event);
}

