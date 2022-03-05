#include "mydockwidget.h"
#include <QApplication>

MyDockWidget::MyDockWidget(QWidget* parent)
    :QDockWidget(parent),
     mTitleButton(nullptr),
     mPropertyAnimation(nullptr)
{

}

void MyDockWidget::initUI()
{
    setDockSize(QSize(minimumWidth(), height()));
    setMouseTracking(true);
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
    if (mMaxState) { // 收起
         icon = QIcon(QString::fromUtf8(":/icons/fold_96px.png"));
    } else { // 展开
        icon = QIcon(QString::fromUtf8(":/icons/unfold_96px.png"));
    }
    mTitleButton->setIcon(icon);
    mTitleButton->setIconSize(QSize(width(),96));
}

void MyDockWidget::onTitleReleased()
{

}


void MyDockWidget::enterEvent(QEvent *event)
{

    if (mPropertyAnimation != nullptr) {
        return;
    }
    mMaxState = true;
    QSize start = mDockSize;
    QSize end = QSize(maximumWidth(), height());
    mPropertyAnimation = new QPropertyAnimation(this, "dockSize");
    mPropertyAnimation->setDuration(150);
    mPropertyAnimation->setStartValue(start);
    mPropertyAnimation->setEndValue(end);
    mPropertyAnimation->setEasingCurve(QEasingCurve::Linear);
    connect(mPropertyAnimation, &QPropertyAnimation::finished, [=]() {
        delete mPropertyAnimation;
        mPropertyAnimation = nullptr;
    });
    mPropertyAnimation->start();
    QDockWidget::enterEvent(event);

}

void MyDockWidget::leaveEvent(QEvent *event)
{
    if (!mMaxState || mPropertyAnimation != nullptr) {
        return;
    }
    QSize startSize = mDockSize;
    QSize endSize = QSize(minimumWidth(), height());
    mPropertyAnimation = new QPropertyAnimation(this, "dockSize");
    mPropertyAnimation->setDuration(150);
    mPropertyAnimation->setStartValue(startSize);
    mPropertyAnimation->setEndValue(endSize);
    mPropertyAnimation->setEasingCurve(QEasingCurve::Linear);
    connect(mPropertyAnimation, &QPropertyAnimation::finished, [=]() {
        delete mPropertyAnimation;
        mPropertyAnimation = nullptr;
        mMaxState = false;
    });
    mPropertyAnimation->start();
    QDockWidget::leaveEvent(event);
}

QSize MyDockWidget::dockSize() {
    return mDockSize;
}

void MyDockWidget::setDockSize(const QSize &_size)
{
    mDockSize = _size;
    setMinimumSize(mDockSize);
    setMaximumWidth(mDockSize.width());
    update(rect());
}

void MyDockWidget::showEvent(QShowEvent *event)
{
    QWidget* dockParent = qobject_cast<QWidget*>(parent());
    if (dockParent == nullptr) {
        return;
    }
    mDockSize = QSize(32, dockParent->height());
    setMinimumHeight(mDockSize.height());
}

void MyDockWidget::paintEvent(QPaintEvent *event)
{
    QWidget* dockParent = qobject_cast<QWidget*>(parent());
    if (dockParent == nullptr) {
        return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    if (mMaxState) {
        painter.setBrush(QColor(59,58, 64));
    } else {
        painter.setPen(Qt::transparent);
    }
    painter.drawRect(0,0, mDockSize.width(), mDockSize.height());
    painter.restore();

}
