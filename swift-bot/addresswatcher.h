#ifndef ADDRESSWATCHER_H
#define ADDRESSWATCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QTimer>

class AddressWatcher : public QObject
{
    Q_OBJECT
public:
    explicit AddressWatcher(const QString& address, QObject *parent = nullptr);

signals:
    void transactionFound( const QJsonObject& transaction );
    void transactionConfirmed( const QJsonObject& transaction );

public slots:
    void sendRequest() {
        QNetworkRequest r( QUrl("https://api.blockcypher.com/v1/btc/main/addrs/"+_address) );
        netman->get( r );
    }
    /*
{
  "address": "1rundZJCMJhUiWQNFS5uT3BvisBuLxkAp",
  "total_received": 254847811176,
  "total_sent": 254847811176,
  "balance": 0,
  "unconfirmed_balance": 0,
  "final_balance": 0,
  "n_tx": 300,
  "unconfirmed_n_tx": 0,
  "final_n_tx": 300,
  "txrefs": [
    {
      "tx_hash": "b7e5a1d0e7b780e1afc7d48132cef60c10a967e0b3853c0b0e6a9b823e301dcb",
      "block_height": 504696,
      "tx_input_n": 82,
      "tx_output_n": -1,
      "value": 389000000,
      "ref_balance": 0,
      "confirmations": 102055,
      "confirmed": "2018-01-17T18:05:32Z",
      "double_spend": false
    },
*/
    void onResponse( QNetworkReply * reply ) {
        const QByteArray data( reply->readAll().constData() );
        const QJsonObject res( QJsonDocument::fromJson( data ).object() );
        transactions_count = res.value("n_tx").toInt();
        transactions_unconfirmed_count = res.value("unconfirmed_n_tx").toInt();
        transactions_confirmed_count = res.value("final_n_tx").toInt();
        const QJsonArray txs( res.value("txrefs").toArray() );
        if ( !txs.isEmpty() ) {
            for ( auto it = txs.begin(); it != txs.end(); it++ ) {
                const QJsonObject tr( it->toObject() );
                QHash<QString, QString> _tdata;
                _tdata["hash"] =  tr.value("tx_hash").toString();
                _tdata["confirmations"] =  tr.value("confirmations").toInt();
                _tdata["value"] =  QString::number( tr.value("value").toInt() / 10000000, 'f', 8 );
                _transactions[ tr.value("tx_hash").toString() ] = _tdata;
            }
        }
        reply->deleteLater();
    }
private:
    QString _address;
    quint32 transactions_count;
    quint32 transactions_unconfirmed_count;
    quint32 transactions_confirmed_count;
    QHash<QString, QHash<QString, QString>> _transactions;
    QNetworkAccessManager * netman;
    QTimer * t;
};

#endif // ADDRESSWATCHER_H
