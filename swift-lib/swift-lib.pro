QT -= gui
QT += network sql websockets
TEMPLATE = lib


CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS


VERSION = 2.1
SOURCES += \
    arbitragemanager.cpp \
    arbitragepair.cpp \
    assetsstorage.cpp \
    asyncrcpcallerinterface.cpp \
    orderbooksclient.cpp \
    ordermodel.cpp \
    orderparams.cpp \
    ordersstorage.cpp \
    swiftapiclient.cpp \
    swiftapiclientbinance.cpp \
    swiftapiclientbitfinex.cpp \
    swiftapiclientbittrex.cpp \
    swiftapiclientcexio.cpp \
    swiftapiclientexmo.cpp \
    swiftapiclienthitbtc.cpp \
    swiftapiclienthuobi.cpp \
    swiftapiclientidcm.cpp \
    swiftapiclientkraken.cpp \
    swiftapiclientkucoin.cpp \
    swiftapiclientlakebtc.cpp \
    swiftapiclientlbank.cpp \
    swiftapiclientlivecoin.cpp \
    swiftapiclientokcoin.cpp \
    swiftapiclientzb.cpp \
    swiftapiparser.cpp \
    swiftapiparserbinance.cpp \
    swiftapiparserbitfinex.cpp \
    swiftapiparserbittrex.cpp \
    swiftapiparsercexio.cpp \
    swiftapiparserexmo.cpp \
    swiftapiparserhitbtc.cpp \
    swiftapiparserhuobi.cpp \
    swiftapiparseridcm.cpp \
    swiftapiparserkraken.cpp \
    swiftapiparserkucoin.cpp \
    swiftapiparserlakebtc.cpp \
    swiftapiparserlbank.cpp \
    swiftapiparserlivecoin.cpp \
    swiftapiparserokcoin.cpp \
    swiftapiparserzb.cpp \
    swiftlib.cpp \
    systemevent.cpp \
    wamp.cpp \
    wampclient.cpp \
    apircpclient.cpp \
    arbitragewindow.cpp \
    withdrawparams.cpp \
    swiftworker.cpp

HEADERS += \
    arbitragemanager.h \
    arbitragepair.h \
    assetsstorage.h \
    asyncrcpcallerinterface.h \
    orderbooksclient.h \
    ordermodel.h \
    orderparams.h \
    ordersstorage.h \
    swiftapiclient.h \
    swiftapiclientbinance.h \
    swiftapiclientbitfinex.h \
    swiftapiclientbittrex.h \
    swiftapiclientcexio.h \
    swiftapiclientexmo.h \
    swiftapiclienthitbtc.h \
    swiftapiclienthuobi.h \
    swiftapiclientidcm.h \
    swiftapiclientkraken.h \
    swiftapiclientkucoin.h \
    swiftapiclientlakebtc.h \
    swiftapiclientlbank.h \
    swiftapiclientlivecoin.h \
    swiftapiclientokcoin.h \
    swiftapiclientzb.h \
    swiftapiparser.h \
    swiftapiparserbinance.h \
    swiftapiparserbitfinex.h \
    swiftapiparserbittrex.h \
    swiftapiparsercexio.h \
    swiftapiparserexmo.h \
    swiftapiparserhitbtc.h \
    swiftapiparserhuobi.h \
    swiftapiparseridcm.h \
    swiftapiparserkraken.h \
    swiftapiparserkucoin.h \
    swiftapiparserlakebtc.h \
    swiftapiparserlbank.h \
    swiftapiparserlivecoin.h \
    swiftapiparserokcoin.h \
    swiftapiparserzb.h \
    swiftlib.h \
    systemevent.h \
    wamp.h \
    wampclient.h \
    apircpclient.h \
    arbitragewindow.h \
    withdrawparams.h \
    swiftworker.h


LIBS += -L$$OUT_PWD/../qmsgpack/bin/ -lqmsgpack

INCLUDEPATH += $$PWD/../qmsgpack/src  $$OUT_PWD/../qmsgpack/bin
DEPENDPATH += $$PWD/../qmsgpack/src $$OUT_PWD/../qmsgpack/bin



