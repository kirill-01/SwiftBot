#include "rateswriter.h"
#include "swiftlib.h"
RatesWriter::RatesWriter(QObject *parent) : QObject(parent)
{

}

void RatesWriter::onRates( const QMap<quint32,double> &rates_ ) {
    QMutexLocker lock(&m);

    if ( !_db.isOpen() ) {
        if ( !_db.open() ) {
            qWarning() << _db.lastError().text();
            return;
        }
    }
    const QMap<quint32,double> rates( rates_ );
    QStringList parts;
    QList<quint32> _inserted;
    for( auto it = rates.begin(); it != rates.end(); it++ ) {
        if( _last_inserted_values.contains( it.key() ) ) {
            if ( _last_inserted_values.value( it.key(), 0 ) != it.value() ) {
                if ( !_inserted.contains( it.key() ) ) {
                    parts.push_back( "("+QStringList({ QString::number( it.key() ), QString::number( it.value(), 'f', 8 )}).join(",")+")" );
                    _last_inserted_values[it.key()] = it.value();
                    _inserted.push_back( it.key() );
                }
            }
        } else {
            if ( !_inserted.contains( it.key() ) ) {
                _last_inserted_values.insert( it.key(), it.value() );
                parts.push_back( "("+QStringList({ QString::number( it.key() ), QString::number( it.value(), 'f', 8 )}).join(",")+")" );
                _inserted.push_back( it.key() );
            }
        }
    }
    if ( !parts.isEmpty() ) {
        QSqlQuery q( "INSERT IGNORE INTO `rates` (`pair_id`,`rate`) VALUES "+parts.join(","), _db );
        if ( !q.exec( ) ) {
            qWarning() << q.lastError().text();
        }
        q.finish();
    }

}

void RatesWriter::onThreadStarted() {


    _db = QSqlDatabase::addDatabase("QMYSQL", "sepThreadwriter");
    _db.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
    _db.setPort( 3306 );

    _db.setUserName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERNAME ).toString() );
    _db.setPassword( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERPASSWORD ).toString() );
    _db.setDatabaseName( SwiftLib::getUserDbName() );

    if ( !_db.open() ) {
        qWarning() << _db.lastError().text();
    } else {
        qWarning() << "Success connection to MySQL db";
        QSqlQuery q(_db);
        if ( !q.exec("DELETE FROM rates WHERE date(ts) >= DATE(NOW()) - INTERVAL 7 DAY") ) {
            qWarning() << q.lastError().text();
        }
    }
}
