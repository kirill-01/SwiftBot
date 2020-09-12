#include "swiftlib.h"
#include "apircpclient.h"
SwiftLib::SwiftLib(QObject *parent) : QObject( parent ) {

}

AssetsStorage *SwiftLib::getAssets() {

    static AssetsStorage* _i = nullptr;
    if ( _i == nullptr ) {
        _i = new AssetsStorage();
        _i->loadAssets();
    }
    return _i;
}

QSettings *SwiftLib::getSettings() {
    static QSharedPointer<QSettings> _inst;
    if ( QFile("/opt/swiftbot/config2.conf").exists() ) {
        _inst.reset( new QSettings( "/opt/swiftbot/config2.conf", QSettings::IniFormat ) );
    }
    else {
        if ( !QFile(QString(SWIFT_BASEDIR)+"/"+QString(SWIFT_SETTINGS_FILENAME) ).isWritable() ) {
        QFile(QString(SWIFT_BASEDIR)+"/"+QString(SWIFT_SETTINGS_FILENAME) ).copy("/opt/swiftbot/config2.conf");
        _inst.reset( new QSettings( "/opt/swiftbot/config2.conf", QSettings::IniFormat ));
        } else {
            _inst.reset( new QSettings( QString(SWIFT_BASEDIR)+"/"+QString(SWIFT_SETTINGS_FILENAME), QSettings::IniFormat ) );
        }
    }
    return _inst.data();
}

double SwiftLib::getExchangeFee(const quint32 &id) {
    return getAssets()->getExchangeTradeFee( id );
}

QString SwiftLib::getRpcPath(const quint32 &exchange_id, const SwiftLib::WampExchangesRpcMethods &method) {
    return QString("swift.api.rpc."+QString::number( exchange_id )+"."+SwiftLib::getMethodName( method ) );
}

QString SwiftLib::getTopic(const SwiftLib::WampTopics &topic) {
    if ( topic == WampTopics::ApiRpcAsyncResults ) {
        return "swift.api.feed.results";
    } else if( topic == WampTopics::ErrorsTopic ) {
        return "swift.system.feed.errors";
    } else if( topic == WampTopics::LogsTopic ) {
        return "swift.system.feed.logs";
    } else {
        qWarning() << "Getting unknown topic";
        return "unknown.topic.enum";
    }
}

QString SwiftLib::getMethodName(const SwiftLib::WampExchangesRpcMethods &method) {
    static QHash<WampExchangesRpcMethods,QString> _dict;
    if ( _dict.isEmpty() ) {
        _dict[CustomMethod]="custom.method";
        _dict[GetCurrencies]="dict.currencies";
        _dict[GetMarkets]="dict.markets";
        _dict[GetOrderbooks]="dict.orderbooks";
        _dict[OrderPlace]="trade.place";
        _dict[OrderCancel]="trade.cancel";
        _dict[OrderGet]="trade.get";
        _dict[OrderReplace]="trade.replace";

        _dict[WithdrawGetLimits]="balance.withdraw.limits";
        _dict[WithdrawList]="balance.withdraws.list";
        _dict[WithdrawHistory]="balance.withdraws";
        _dict[WithdrawCancel]="withdraw.cancel";
        _dict[WithdrawCreate]="balance.withdraw";
        _dict[WithdrawInner]="balance.withdraw.inner";
        _dict[WithdrawGetFee]="balance.withdraw.fee";
        _dict[GetBalances]="balance.get";
        _dict[GetDeposits]="balance.deposits";
        _dict[GetDepositAddress]="balance.address";
        _dict[TradeHistory]="trade.history";
        _dict[TradeOpenOrders]="trade.active";
        _dict[TradeGetFee]="trade.fee";
        _dict[TradeGetLimits]="trade.limits";
    }
    return _dict.value( method, "unknown"+ QString::number( method ) );
}


void SwiftLib::setBalances(const QHash<quint32, double> &balances) {
    // qWarning() << balances << "applying balance";
    SwiftLib::getAssets()->setBalances( balances );
}

void SwiftLib::setBalance(const QJsonObject &j_balance) {
    //qWarning() << j_balance << "applying balance";
    SwiftLib::getAssets()->setBalance( j_balance );
}

void SwiftLib::setRequested(const QHash<QString, QHash<quint32, double> > &req) {
    SwiftLib::getAssets()->setRequested( req );
}

double SwiftLib::getBalance(const quint32 &currency) {
    return SwiftLib::getAssets()->getBalance( currency );
}

QHash<QString, QHash<quint32, double> > SwiftLib::getRequested(){
    return SwiftLib::getAssets()->getRequested();
}


QString SwiftLib::exchangeParamPath(const QString &exchange, const QString &param) {
    return QStringList({"api_nodes", exchange, param }).join("/");
}

bool SwiftLib::hasSettings(const QString &settingsname) {
    return SwiftLib::getSettings()->value( settingsname, QString("UNDEFINED")) != QString("UNDEFINED");
}

ApiRcpClient *SwiftLib::getRpcClient() {
    static ApiRcpClient * inst = nullptr;
    if ( inst == nullptr ) {
        inst = new ApiRcpClient();
    }
    return inst;
}

SwiftLib::SwiftLib()
{
}

QSqlQuery SwiftLib::createQuery(const QString &dbname) {
    static QHash<QString, QSqlDatabase> _dbs;
    if ( _dbs.contains( dbname ) ) {
         if ( _dbs[dbname].isOpen()  ) {
              return QSqlQuery( _dbs[dbname] );
         }
    }
    QSqlDatabase db = _dbs[dbname];
    if( QSqlDatabase::contains( dbname ) )
    {
        db = QSqlDatabase::database( dbname );
        if ( !db.isOpen() ) {
          db.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
            db.setPort( 3306 );
            db.setUserName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERNAME ).toString() );
            db.setPassword( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERPASSWORD ).toString() );
            db.setDatabaseName( dbname );
            if ( !db.open() ) {
                qWarning() << db.lastError().text();
                qApp->exit( 1 );
            }
        }
    }
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL", dbname);
        db.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
        db.setPort( 3306 );
        db.setUserName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERNAME ).toString() );
        db.setPassword( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERPASSWORD ).toString() );
        db.setDatabaseName( dbname );
        if ( !db.open() ) {
            qWarning() << db.lastError().text();
            qApp->exit( 1 );
        }
    }
    _dbs[dbname] = db;
    return QSqlQuery( db );
}

void SwiftLib::loadActualConfig() {
    QString dbname( SwiftLib::getSettings()->value("MYSQL_DBNAME").toString().trimmed() );
    QSqlQuery q("SELECT * FROM `"+dbname.toUtf8()+"`.`settings_storage`");
    if ( q.exec() ) {
        QSettings newsettings("/opt/swiftbot/config2.conf", QSettings::IniFormat );
        while ( q.next() ) {
            newsettings.setValue( q.value("param").toString(), q.value("value").toString() );
        }
        newsettings.sync();
    }
    qWarning() << "Settings restored from db";
}

QString SwiftLib::getUserDbName() {
    static QString r("");
    if ( r == "" ) {
        r = QString( SwiftLib::getSettings()->value("MYSQL_DBNAME", "swiftbot").toString().trimmed() );
    }
    return r;
}
