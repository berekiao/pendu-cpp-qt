QT += core widgets

CONFIG += c++11

TARGET = Pendu
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gamewindow.cpp \
    scoremanager.cpp

HEADERS += \
    mainwindow.h \
    gamewindow.h \
    scoremanager.h

RESOURCES += \
    resources.qrc