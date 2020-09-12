#ifndef NODESCONTROLLER_H
#define NODESCONTROLLER_H

#include <QObject>
#include "orderbooksnode.h"
#include <QSqlDatabase>

class NodesController : public QObject
{
    Q_OBJECT
public:
    explicit NodesController(QObject *parent = nullptr);

signals:
    void startNodes(QThread::Priority = QThread::HighPriority);
    void stopNodes();
    void orderbooksAccumulated( const QString& topic,  const QJsonObject& j_stat );
    void summaryStats( const QString& topic,  const QJsonObject& j_stat );
    void clientExited();
    void pairsRate( const QMap<quint32,double> &rates );
public slots:
    void sendRates();
    void nodeThreadStarted();
    void nodeThreadFinished();
    void startAllNodes();

    void onOrderBooks( const QJsonArray&a, const QJsonArray& b );
    void onNodeError( OrderbooksNode::NodeErrors err );

    void findOutdatedPairs();
    void secStatCalc();

    void sendStats();
    void sendOrderbooks();
private:
    QSqlDatabase _ch;
    QMutex mutex;
    QHash<quint32, quint64> _last_pair_update;
    QHash<quint32, quint64> _pair_max_outdated;
    QHash<quint32, QList<quint64>> _pair_outdates;
    QList<quint32> _current_outdated_list;
    QHash<quint32, QJsonArray> _asks;
    QHash<quint32, QJsonArray> _bids;

    QHash<quint32, double> _min_ask;
    QHash<quint32, double> _max_bid;
    QTimer *trestimer;
    QTimer *acttimer;
    QTimer *pubtimer;
    QTimer *stattimer;
    QTimer *ratestimer;
    QList<quint64> _results;
    quint64 results_per_sec;
    quint64 midrate;

};

#endif // NODESCONTROLLER_H
