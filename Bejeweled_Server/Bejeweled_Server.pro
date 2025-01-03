QT       += core gui
QT    += sql
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bejeweled_Server

CONFIG += c++17

CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clienttask.cpp \
    information.cpp \
    main.cpp \
    mainwindow.cpp \
    server.cpp \
    sql.cpp

HEADERS += \
    clienttask.h \
    information.h \
    mainwindow.h \
    server.h \
    sql.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \

    resources/database.db
    QMAKE_CXXFLAGS += -fno-inline -fno-builtin
