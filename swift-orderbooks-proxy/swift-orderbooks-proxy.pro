QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

QT += network websockets sql

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
        main.cpp \
        nodescontroller.cpp \
        orderbooksnode.cpp \
        rateswriter.cpp

INCLUDEPATH += $$PWD/../swift-lib $$PWD/../qmsgpack/src $$OUT_PWD/../qmsgpack/bin
DEPENDPATH += $$PWD/../swift-lib  $$PWD/../qmsgpack/src $$OUT_PWD/../qmsgpack/bin

LIBS += -L$$OUT_PWD/../qmsgpack/bin -lqmsgpack  -L$$OUT_PWD/../swift-lib/ -lswift-lib

# Default rules for deployment.
target.path = /opt/swiftbot/bin
INSTALLS += target

HEADERS += \
    nodescontroller.h \
    orderbooksnode.h \
    rateswriter.h
