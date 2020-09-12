QT -= gui
QT += websockets network sql
CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += $$PWD/../swift-lib  $$PWD/../qmsgpack/src
DEPENDPATH += $$PWD/../swift-lib  $$PWD/../qmsgpack/src  $$PWD/../qmsgpack/bin

LIBS += -L$$OUT_PWD/../qmsgpack/bin/ -lqmsgpack -L$$OUT_PWD/../swift-lib/ -lswift-lib

SOURCES += \
        balancesworker.cpp \
        balanceworker.cpp \
        dbwriter.cpp \
        logsworker.cpp \
        main.cpp \
        ordersworker.cpp \
        orderworker.cpp \
        settingshostworker.cpp \
        settingsworker.cpp \
        strategyworker.cpp \
        userhistoryworker.cpp

HEADERS += \
    balancesworker.h \
    balanceworker.h \
    dbwriter.h \
    logsworker.h \
    ordersworker.h \
    orderworker.h \
    settingshostworker.h \
    settingsworker.h \
    strategyworker.h \
    userhistoryworker.h
