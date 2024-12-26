QT       += core gui
QT    += sql
QT += multimedia multimediawidgets
QT += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bejeweled_Client

CONFIG += c++17

CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientthread.cpp \
    Jewel.cpp \
    board.cpp \
    information.cpp \
    logicworker.cpp \
    play.cpp \
    dialog.cpp \
    main.cpp \
    music.cpp\
    mainwindow.cpp \
    menu.cpp

HEADERS += \
    information.h \
    logicworker.h \
    mainwindow.h \
    music.h\
    clientthread.h \
    Jewel.h \
    board.h \
    dialog.h \
    mainwindow.h \
    menu.h \
    play.h

FORMS += \
    dialog.ui \
    mainwindow.ui \
    play.ui \
    menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
QMAKE_CXXFLAGS += -fno-inline -fno-builtin -fno-lto



