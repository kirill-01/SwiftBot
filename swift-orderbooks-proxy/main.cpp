#include <QCoreApplication>
#include <QThread>
#include "rateswriter.h"
#include "orderbooksnode.h"
#include <QSharedPointer>
#include <QSettings>
#include "nodescontroller.h"
#include "wampclient.h"
#include "swiftlib.h"
#define APP_NAME "Swift exchange API node"
#define APP_VERSION "2.0.2016"

#define CONFIG_FILENAME "/opt/swiftbot/config2.conf"

static QVector<QSharedPointer<OrderbooksNode>> _orderbooks_nodes;
static QVector<QSharedPointer<QThread>> _orderbooks_threads;

static QSqlDatabase db;

typedef QPair<QString,quint32> TradePair;
typedef QHash<QString, QList<TradePair>> ActivePairs;

ActivePairs getActualPairs() {
    QSqlQuery q("SELECT p.id, p.name, e.name as exchange_name, p.base_currency_id, p.market_currency_id, p.arbitrage_pair_id FROM pairs p LEFT JOIN exchanges e ON e.id=p.exchange_id LEFT JOIN arbitrage_pairs a ON a.id = p.arbitrage_pair_id WHERE a.is_enabled=1 AND p.is_enabled=1 AND e.is_enabled=1");
    ActivePairs _ret;
    if ( q.exec() ) {
        while ( q.next() ) {
            _ret[ q.value("exchange_name").toString() ].push_back( TradePair({q.value("name").toString(), q.value("id").toUInt() }) );
        }
    }
    q.finish();
    return _ret;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QLockFile lockFile("/opt/swiftbot/orderbooks.lock");
    if(!lockFile.tryLock(100)){
        qWarning() << "Another instance is running";
        return 1;
    }

    QSettings settings( CONFIG_FILENAME, QSettings::IniFormat );
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
    db.setPort( 3306 );
    db.setUserName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERNAME ).toString() );
    db.setPassword( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERPASSWORD ).toString() );
    db.setDatabaseName( SwiftLib::getUserDbName() );
    if ( !db.open() ) {
        qWarning() << db.lastError().text();
        return 1;
    } else {
        qWarning() << "Success connection to MySQL db";
    }

    RatesWriter * writer = new RatesWriter();

    QThread * ratesThread = new QThread();
    ratesThread->setObjectName("ratesThread");

    QObject::connect( ratesThread, &QThread::started, writer, &RatesWriter::onThreadStarted );
    ratesThread->start( QThread::Priority::NormalPriority );

    WampClient * wamp_client = new WampClient( "orderbooks" );
    QObject::connect( wamp_client, &WampClient::isExited, &a, &QCoreApplication::quit );

    NodesController * node_controller = new NodesController();
    QObject::connect( node_controller, &NodesController::pairsRate, writer, &RatesWriter::onRates, Qt::QueuedConnection );
    QObject::connect( node_controller, &NodesController::clientExited, &a, &QCoreApplication::quit );
    QObject::connect( &a, &QCoreApplication::aboutToQuit, ratesThread, &QThread::quit );
    QObject::connect( node_controller, &NodesController::orderbooksAccumulated, wamp_client, &WampClient::publishMessage, Qt::QueuedConnection );
    wamp_client->startClient();


    const ActivePairs pairs( getActualPairs() );
    const QStringList exchanges( pairs.keys() );

    for ( auto it = exchanges.begin(); it != exchanges.end(); it++ ) {
        if ( !pairs.value( *it ).isEmpty() ) {
            QThread * _nodethread = new QThread();
            _nodethread->setObjectName( *it );
            OrderbooksNode * _nodeitem  = new OrderbooksNode();
            QObject::connect( _nodeitem, &OrderbooksNode::nodeError,  node_controller, &NodesController::onNodeError );

            QObject::connect( _nodethread, &QThread::started, _nodeitem, &OrderbooksNode::startNode);
            QObject::connect( _nodethread, &QThread::started, node_controller, &NodesController::nodeThreadStarted );
            QObject::connect( _nodethread, &QThread::finished, node_controller, &NodesController::nodeThreadFinished );

            QObject::connect( node_controller, &NodesController::startNodes, _nodethread, &QThread::start );

            QObject::connect( node_controller, &NodesController::stopNodes, _nodeitem, &OrderbooksNode::stopNode );
            QObject::connect( _nodeitem, &OrderbooksNode::orderbooks,  node_controller, &NodesController::onOrderBooks );

            _nodeitem->setExchange(*it, pairs.value( *it ) );

            _nodeitem->moveToThread( _nodethread );
            QSharedPointer<QThread> _nodethread_ptr( std::move(_nodethread) );
            QSharedPointer<OrderbooksNode> _nodeitem_ptr( std::move( _nodeitem ) );
            _orderbooks_threads.push_back( std::move( _nodethread_ptr ) );
            _orderbooks_nodes.push_back( std::move( _nodeitem_ptr ) );
        }
    }
    node_controller->startAllNodes();

    return a.exec();
}
