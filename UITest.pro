QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# 配置file_copies
CONFIG += file_copies

# 创建examples变量并配置
# 配置需要复制的文件或目录(支持通配符)
drivers.files += $$PWD/listitemicons\
                 $$PWD/config
# 配置需要复制的目标目录, $$OUT_PWD为QMake内置变量，含义为程序输出目录
CONFIG(debug, debug|release){
    drivers.path = $$OUT_PWD/debug
} else {
    drivers.path = $$OUT_PWD/release
}


# 配置COPIES
COPIES += drivers
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ConvertInfo.cpp \
    formlist.cpp \
    formlistitem.cpp \
    formlistitemcreate.cpp \
    handlefactory.cpp \
    main.cpp \
    mainwindow.cpp \
    mydockwidget.cpp \
    mymediaplayer.cpp \
    sqldatahandle.cpp \
    videoconvert.cpp \
    xmldatahandle.cpp

HEADERS += \
    ConvertInfo.h \
    DataHandle.h \
    common.h \
    formlist.h \
    formlistitem.h \
    formlistitemcreate.h \
    handlefactory.h \
    mainwindow.h \
    mydockwidget.h \
    mymediaplayer.h \
    sqldatahandle.h \
    videoconvert.h \
    xmldatahandle.h

FORMS += \
    formlist.ui \
    formlistitem.ui \
    formlistitemcreate.ui \
    mainwindow.ui \
    mymediaplayer.ui \
    videoconvert.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    copy


