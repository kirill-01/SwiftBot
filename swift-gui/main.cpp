#include <QApplication>
#include "../swift-lib/wampclient.h"
#include <QSqlDatabase>
#include "../swift-lib/swiftlib.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const QString dbname( SwiftLib::getUserDbName() );
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
    db.setPort( 3306 );

    db.setUserName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERNAME ).toString() );
    db.setPassword( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERPASSWORD ).toString() );
    db.setDatabaseName( SwiftLib::getUserDbName() );
    if ( !db.open() ) {
        qWarning() << (db.lastError().text());
        return 1;
    }

    MainWindow w;
    WampClient * wamp = new WampClient();
    QObject::connect( &w, &MainWindow::connectRealm, wamp, &WampClient::connectRealm );
    QObject::connect( wamp, &WampClient::systemText, &w, &MainWindow::onTextAppend );

    QObject::connect( wamp, &WampClient::clientConnected, &w, &MainWindow::onWampSession );
    w.show();
    return a.exec();
}
