#include "swiftapiclient.h"
#include "swiftlib.h"
#include <QtConcurrent/QtConcurrent>
#include "systemevent.h"

SwiftApiClient::SwiftApiClient(QObject *parent) : SwiftWorker(parent),
    limits_timer(new QTimer() ),
    wamp_connected(false),
    _queues_timer(new QTimer() ),
    _state_timer( new QTimer()),
    _uuids_counter( QDateTime::currentMSecsSinceEpoch() ),request_pause(false)
{
    debug = false;
    qRegisterMetaType<SwiftApiClient::AsyncMethods>("SwiftApiClient::AsyncMethods");
    netman = new QNetworkAccessManager(this);
    connect( netman, &QNetworkAccessManager::finished, this, &SwiftApiClient::onNetworkReply );
}

QString SwiftApiClient::getMethodName(const SwiftApiClient::AsyncMethods &method) {
    return SwiftLib::getMethodName( static_cast<SwiftLib::WampExchangesRpcMethods>( method ) );
}

quint32 SwiftApiClient::getExchangeId() {
    return SwiftLib::getAssets()->getExchangeId( getExchangeName() );
}

QString SwiftApiClient::getApiVersionString() {
    return "cpp-2.3";
}

QString SwiftApiClient::getExchangeApiKey() const {
    if ( api_key.isEmpty() ) {
        return SwiftLib::getSettings()->value(  SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"api_key"), "" ).toString().trimmed();
    } else {
        return api_key;
    }
}

QString SwiftApiClient::getExchangeApiSecret() const {
    if ( api_secret.isEmpty() ) {
        return SwiftLib::getSettings()->value(  SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"api_secret"), "" ).toString().trimmed();
    } else {
        return api_secret;
    }
}

QString SwiftApiClient::getExchangeApiAdditional() const {
    if ( api_user.isEmpty() ) {
        return SwiftLib::getSettings()->value(  SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"username"), "" ).toString().trimmed();
    } else {
        return api_user;
    }
}

void SwiftApiClient::customParser(const SwiftApiClient::AsyncMethods &method, const QJsonObject &j_result) {
    Q_UNUSED(j_result)
    Q_UNUSED(method)
}

quint64 SwiftApiClient::createAsyncWaiter(const SwiftApiClient::AsyncMethods &method, const QJsonObject &j_params) {

    if ( getExchangeApiKey().isEmpty() || getExchangeApiKey() == "" ) {
        QJsonObject j_infoitem;
        j_infoitem["uniquekey"] = "api_credential_required_"+getExchangeName();
        j_infoitem["exchange"] = getExchangeName();
        j_infoitem["title"] = "API ключ не валиден";
        j_infoitem["text"] = "Не установлен или просрочен API ключ. Сгенерируйте и установите в настройках новый.";
        j_infoitem["code"] = "";
        j_infoitem["url"] = "";
        j_infoitem["ts"] = QDateTime::currentDateTime().toString( Qt::ISODate );
        QString msg( QJsonDocument( j_infoitem ).toJson( QJsonDocument::Compact ) );
        session->call("swiftbot.components.actions.insert", {"api_credential_required_"+getExchangeName(), msg});

        addError("Api key or secret is empty: " + getExchangeName() + " Key: " + getExchangeApiKey()+ " Secret: " + getExchangeApiSecret() );
        return 1;
    }
    if ( request_pause ) {
        return 2;
    }


    const quint64 uuid = registerAsyncCall( method );
    if ( method == SwiftApiClient::AsyncMethods::OrderPlace ) {
        registerOrder( j_params, uuid );
    }
    _async_params.insert( uuid, j_params );
    _async_queue.enqueue( uuid );
    if ( debug ) {
        qWarning() << getExchangeName() << getMethodName( method ) << "ASYNC REQUEST REGISTERED" << uuid << "PARAMS" << j_params;
    }
    return uuid;
}

QNetworkAccessManager *SwiftApiClient::getManager() {
    return netman;
}

void SwiftApiClient::sendApiStateReport() {
}

void SwiftApiClient::onWampSession_(Wamp::Session *session ) {

    // Updating settings from worker
    QStringList _set = session->call("swift.assets.rpc.settings.get", {
      SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"api_key"),
      SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"api_secret"),
      SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"username")
      }).toStringList();

    api_key = _set.at(0).split("=").at(1);
    api_secret = _set.at(1).split("=").at(1);
    api_user = _set.at(2).split("=").at(1);

    connect( this, &SwiftApiClient::pubWamp, session, &Wamp::Session::publishMessage, Qt::QueuedConnection );
    wamp_connected = true;
    _state_timer->setInterval( 25000 );
    limits_timer->setInterval( 60000 );
    _req_total_limit = 60;
    _req_total_counter = 0;

    connect( limits_timer, &QTimer::timeout, this, &SwiftApiClient::resetLimits );
    limits_timer->start();
    _queues_timer->setInterval( 250 );

    connect( _queues_timer, &QTimer::timeout, this, &SwiftApiClient::processAsyncQueue );
    connect( _queues_timer, &QTimer::timeout, this, &SwiftApiClient::processAsyncQueueResult );
    _queues_timer->start();
    _started_at = QDateTime::currentDateTime();
    _last_request_at = QDateTime::currentDateTime();
    connect( this, &SwiftApiClient::apiParsedResponse, this, &SwiftApiClient::onApiResponseParsed );


    QMetaObject meta = SwiftLib::staticMetaObject;
    QMetaEnum tmpObj = meta.enumerator(1);
    for (int i = 0; i < tmpObj.keyCount(); i++)
    {
        const QString rpc_path( SwiftLib::getRpcPath( getExchangeId(), static_cast<SwiftLib::WampExchangesRpcMethods>( i ) ));
        const SwiftApiClient::AsyncMethods met( static_cast<SwiftApiClient::AsyncMethods> ( i ) );
        _methods_minimum_delay.insert( met, 1000 );
        QVariantMap options;
        options["invoke"] = "last";
        session->provide(rpc_path, [=](const QVariantList &args, const QVariantMap &kwargs ) {
                Q_UNUSED(kwargs)
                if ( request_pause ) {
                    return quint64(0);
                }
                if ( args.isEmpty() ) {
                    const QJsonObject j_params;
                    const quint64 uuid =  createAsyncWaiter( static_cast<SwiftApiClient::AsyncMethods>( met ), j_params );
                    return uuid;
                } else {
                    const QJsonObject j_params( QJsonDocument::fromJson( args.at(0).toString().toUtf8() ).object() );
                    const quint64 uuid =  createAsyncWaiter( static_cast<SwiftApiClient::AsyncMethods>( met ), j_params );
                    return uuid;
                }
            });
    }
    addLog("Api client connected to wamp. RPC Methods registered");


}

void SwiftApiClient::onNetworkReply(QNetworkReply *reply) {
    const quint64 rep_uuid = reply->property("uuid").toString().toULongLong();
    const QByteArray data( reply->readAll().constData() );

    if ( isDebug() ) {
        addLog( QString( data ) );
    }

    if ( !reply->property("customRequest").isNull() ) {
        emit customRequestResult( reply->property("customRequest").toString(), data );
    } else {
        QVariant statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        const quint32 http_code = statusCode.toUInt();
        if ( http_code != 200 && http_code != 201) {
            if ( http_code == 521 ) {
                // Server down
                addError("API response 521 - Freeze requestst and try again later");
                request_pause = true;
                QTimer::singleShot( 30000, this, &SwiftApiClient::unfreeze );
                return;
            } else if ( http_code == 400 ) {
                // Request error
                if ( isDebug() ) {
                    addError( getExchangeName() + http_code + data );
                }
                addError("API response 400 - Bad request" + QString::number( _async_dictionary.value( rep_uuid ) ) + data );
               // request_pause = true;
               // QTimer::singleShot( 30000, this, &SwiftApiClient::unfreeze );
                return;
            } else if ( http_code == 403 ) {
                if ( isDebug() ) {
                   addError( getExchangeName() + http_code + data );
                }
                 const SwiftApiClient::AsyncMethods method( _async_dictionary.value(rep_uuid) );
                addError("API response 403 " +  getMethodName( method ) + "- Freeze requestst and try again later: "+ data);
                request_pause = true;
                QTimer::singleShot( 30000, this, &SwiftApiClient::unfreeze );
                QJsonObject jret;
                jret["success"] = false;
                jret["async_uuid"] = QString::number( rep_uuid );
                jret["nonce"] = QString::number( rep_uuid );
                jret["method"] = getMethodName( method );
                jret["exchange"] = getExchangeName();
                jret["error"] = QString::number( http_code );
                onApiResponseParsed( rep_uuid, jret );
                return;
                // API KEY BROKE
            } else if ( http_code == 429 ) {
                if ( isDebug() ) {
                   addError( getExchangeName() + http_code + data );
                }
                addError("API response 429 - Freeze requestst and try again later");
                request_pause = true;
                QTimer::singleShot( 120000, this, &SwiftApiClient::unfreeze );
                return;
                // DDOS!
            } else {
                const SwiftApiClient::AsyncMethods method( _async_dictionary.value(rep_uuid) );

                if ( getExchangeId() == 11 ) {
                    QJsonArray bitfinex_err_code_check( QJsonDocument::fromJson( data ).array() );
                    if ( bitfinex_err_code_check.contains( "error" ) && http_code == 500 ) {
                        addError( "Response code: " + QString::number( http_code ) + " Message: " + bitfinex_err_code_check.at(2).toString() );
                    }
                } else {
                    if ( isDebug() ) {
                        addError( getExchangeName() + http_code + data );
                    }
                }
                QJsonObject jret;
                jret["success"] = false;
                jret["async_uuid"] = QString::number( rep_uuid );
                jret["nonce"] = QString::number( rep_uuid );
                jret["method"] = getMethodName( method );
                jret["exchange"] = getExchangeName();
                jret["error"] = QString::number( http_code );
                if ( isDebug() ) {
                    addError( getExchangeName() + getMethodName( method ) + "NETWORK WRONG RESPONSE RECEIVED: " + data );
                }
                onApiResponseParsed( rep_uuid, jret );
            }
        } else {

                const SwiftApiClient::AsyncMethods method( _async_dictionary.value(rep_uuid) );
                _last_request_at = QDateTime::currentDateTime();
                if ( isDebug() ) {
                    addLog( getExchangeName() + getMethodName( method ) + "NETWORK RESPONSE RECEIVED: " + data );
                }
                emit parseApiResponse( rep_uuid, method, data );
        }
    }
    reply->deleteLater();
}

void SwiftApiClient::processAsyncQueueResult() {
    if ( !_async_results_queue.isEmpty() ) {
        const quint64 next_uuid = _async_results_queue.dequeue();
        if ( !sendAsyncResult( next_uuid ) ) {
            if ( isDebug() ) {
                addError( getExchangeName() + QString::number( next_uuid ) + "CANT PROCESS ASYNC RESULT" );
            }
        }
    }
}

void SwiftApiClient::processAsyncQueue() {
    if ( !_async_queue.isEmpty() && !request_pause) {
        const quint64 next_uuid = _async_queue.dequeue();
        if ( _async_dictionary.contains( next_uuid ) ) {
            const SwiftApiClient::AsyncMethods next_method( _async_dictionary.value( next_uuid ) );
            const QJsonObject j_params( _async_params.value( next_uuid ) );
            _req_total_counter++;
            _methods_counter[next_method] = QDateTime::currentSecsSinceEpoch();
                    if (next_method == GetCurrencies) {
                        getCurrencies( j_params, next_uuid );
                    } else if (next_method == GetMarkets) {
                        getMarkets( j_params, next_uuid );
                    } else if (next_method == OrderPlace) {
                        orderPlace(j_params, next_uuid );
                    } else if (next_method == OrderCancel) {
                        //registerOrder( j_params, next_uuid  );
                        orderCancel(j_params, next_uuid );

                    } else if (next_method == OrderGet) {
                        registerOrder( j_params, next_uuid  );
                        orderGet(j_params, next_uuid );
                    } else if (next_method == OrderReplace) {
                        //registerOrder( j_params, next_uuid  );
                        orderReplace(j_params, next_uuid );
                    } else if (next_method == WithdrawGetLimits) {
                        withdrawGetLimits(j_params, next_uuid );

                    } else if (next_method == WithdrawList || next_method == WithdrawHistory) {
                        withdrawHistory(j_params, next_uuid );

                    } else if (next_method == WithdrawCancel) {
                        withdrawCancel(j_params, next_uuid );

                    } else if (next_method == WithdrawCreate) {
                        withdrawCreate(j_params, next_uuid );

                    } else if (next_method == WithdrawInner) {
                        withdrawInner(j_params, next_uuid );

                    } else if (next_method == WithdrawGetFee) {
                        withdrawGetFee(j_params, next_uuid );

                    } else if (next_method == GetBalances) {
                        getBalances(j_params, next_uuid );

                    } else if (next_method == GetDeposits) {
                        getDeposits(j_params, next_uuid );

                    } else if (next_method == GetDepositAddress) {
                        getDepositAddress(j_params, next_uuid );

                    } else if (next_method == TradeHistory) {
                        tradeHistory(j_params, next_uuid );

                    } else if (next_method == TradeOpenOrders) {
                        tradeOpenOrders(j_params, next_uuid );

                    } else if (next_method ==TradeGetFee) {
                        tradeGetFee(j_params, next_uuid );

                    } else if (next_method ==TradeGetLimits) {
                        tradeGetLimits(j_params, next_uuid );
                    } else if ( next_method == TimeSyncMethod ) {
                        customMethod( j_params, next_uuid );
                    } else {
                        addError("Unknown method to process: " + getMethodName( next_method ) );
                    }
        }
    }
}

void SwiftApiClient::onApiResponseParsed(const quint64 &uuid, const QJsonObject &j_result) {
    if ( _asyncs.contains( uuid ) ) {
        QJsonObject result( j_result );
        const SwiftApiClient::AsyncMethods method( _async_dictionary.value(uuid) );
        if ( method == SwiftApiClient::AsyncMethods::GetBalances )
        {
         if ( result.value("success").toBool(false) == true ) {
             QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",result.value("balances")}});
             const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
             session->publish("swift.balance.feed.events", {"UPDATE", _event_data});
             QJsonArray j_arr( result.value("balances").toArray() );
             for( auto it = j_arr.begin(); it != j_arr.end(); it++ ) {
                 setCachedBalance( it->toObject().value("currency_id").toString().toUInt(), it->toObject().value("total").toString().toDouble(), it->toObject().value("reserved").toString().toDouble() );
             }
         }
        } else if ( method == SwiftApiClient::AsyncMethods::OrderPlace
             || method == SwiftApiClient::AsyncMethods::OrderCancel
             || method ==  SwiftApiClient::AsyncMethods::OrderGet
             || method == SwiftApiClient::AsyncMethods::OrderReplace )
        {
            if ( _regitered_orders.contains( uuid ) ) {
                QJsonObject j_cached_obj(  getRegisteredOrder( uuid ) );
                result["local_id"] = j_cached_obj.value("local_id").toString();
                result["arb_window"] = j_cached_obj.value("arb_window");

                QStringList _params({"market_id","base_currency_id","market_currency_id","type","amount","amount_left","rate","fee"});

                for( auto it = _params.begin(); it != _params.end(); it++ ) {
                    if ( j_cached_obj.contains( *it ) ) {
                        if ( !result.contains( *it ) ) {
                            result[*it] = j_cached_obj.value( *it );
                        }
                    }
                }
            }

            if ( result.value("success").toBool(false) == true ) {
                // Success
                if ( method == SwiftApiClient::AsyncMethods::OrderPlace ) {
                    addLog("Order placed");
                    if ( result.value("status").toString().toUInt() == 1 ) {
                        QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",QJsonArray({result})}});
                        const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                        session->publish("swift.orders.feed.events", {"PLACED", _event_data});
                    } else if ( result.value("status").toString().toUInt() == 2 ) {
                        QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",QJsonArray({result})}});
                        const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                        session->publish("swift.orders.feed.events", {"COMPLETED", _event_data});
                    }
                } else if ( method == SwiftApiClient::AsyncMethods::OrderCancel ) {
                    QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",QJsonArray({result})}});
                    const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                    session->publish("swift.orders.feed.events", {"CANCELED", _event_data});
                } else if ( method == SwiftApiClient::AsyncMethods::OrderGet ) {
                    if ( result.value("status").toString().toUInt() >=2 ) {
                        QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",QJsonArray({result})}});
                        const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                        session->publish("swift.orders.feed.events", {"COMPLETED", _event_data});
                    } else {
                        QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",QJsonArray({result})}});
                        const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                        session->publish("swift.orders.feed.events", {"UPDATED", _event_data});
                    }
                }
            } else {
                if ( method == SwiftApiClient::AsyncMethods::OrderCancel ) {
                    QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",QJsonArray({result})}});
                    const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                    session->publish("swift.orders.feed.events", {"ERROR", _event_data});
                } else if ( method == SwiftApiClient::AsyncMethods::OrderPlace ) {
                    addError("Order place error");
                    QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",QJsonArray({result})}});
                    const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                    session->publish("swift.orders.feed.events", {"ERROR", _event_data});
                }
                // Fail
            }
            if ( debug ) {
                qWarning() << getExchangeName() << getMethodName( method ) << "ASYNC RESULT" << uuid << "ORDER DATA" << result;
            }
        } else if ( method == SwiftApiClient::AsyncMethods::TradeHistory ) {
            QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",result.value("orders").toArray()}});
            const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
            session->publish("swift.orders.feed.events", {"HISTORY", _event_data});
        } else if ( method == SwiftApiClient::AsyncMethods::TradeOpenOrders
               ) {
            QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",result.value("orders").toArray()}});
            const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
            session->publish("swift.orders.feed.events", {"ACTIVE", _event_data});


             result["exchange_id"] = QString::number( getExchangeId() );
        } else if ( method == SwiftApiClient::AsyncMethods::WithdrawList || method == SwiftApiClient::AsyncMethods::WithdrawHistory
              ) {
            QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",result.value("withdraws")}});
            const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
            session->publish("swift.balance.feed.events", {"WITHDRAWS", _event_data});

            if ( result.contains("deposits") ) {
                QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",result.value("deposits")}});
                const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                session->publish("swift.balance.feed.events", {"DEPOSITS", _event_data});
            }

        } else if(  method == SwiftApiClient::AsyncMethods::GetDeposits ) {
            QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",result.value("deposits")}});
            const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
            session->publish("swift.balance.feed.events", {"DEPOSITS", _event_data});

            if ( result.contains("withdraws") ) {
                QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",result.value("withdraws")}});
                const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                session->publish("swift.balance.feed.events", {"WITHDRAWS", _event_data});

            }

        } else if ( method == SwiftApiClient::WithdrawCreate ) {
            if ( result.value("success").toBool(false) == true ) {
                QJsonObject j_event({{"exchange_id",QString::number( getExchangeId() )},{"items",QJsonArray({result})}});
                const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                session->publish("swift.balance.feed.events", {"WITHDRAW", _event_data});
            } else {

            }
        }


         if ( j_result.value("success").toBool(false) == false ) {

             result["async_uuid"] = QString::number( uuid );
             if ( getExchangeId() == 3 ) {

                 if ( j_result.value("error").toString().contains("10101") ) {
                     // Daily limit reached. Wait for UTC 00:00 to reset
                     QJsonObject j_infoitem;
                     j_infoitem["uniquekey"] = "daily_withdraw_limit_reached"+getExchangeName();
                     j_infoitem["exchange"] = getExchangeName();
                     j_infoitem["title"] = "Блокировка вывода средств";
                     j_infoitem["text"] = "Лимит количества выводов за сутки достигнут. Необходимо обратиться в службу поддержки";
                     j_infoitem["code"] = "10101";
                     j_infoitem["url"] = "https://support.exmoney.com/hc/ru/requests/new";
                     j_infoitem["ts"] = QDateTime::currentDateTime().toString( Qt::ISODate );
                     QString msg( QJsonDocument( j_infoitem ).toJson( QJsonDocument::Compact ) );
                     session->call("swiftbot.components.actions.insert", {"daily_withdraw_limit_reached"+getExchangeName(), msg});
                 } else if (j_result.value("error").toString().contains("40033")  ) {
                     QJsonObject j_infoitem;
                     j_infoitem["uniquekey"] = "locked_withdraw_security"+getExchangeName();
                     j_infoitem["exchange"] = getExchangeName();
                     j_infoitem["title"] = "Блокировка вывода средств";
                     j_infoitem["text"] = "Необходимо обратиться в службу поддержки. Вероятно, будут запрошены подтверждения правомерности использования средств.";
                     j_infoitem["code"] = "40033";
                     j_infoitem["url"] = "https://support.exmoney.com/hc/ru/requests/new";
                     j_infoitem["ts"] = QDateTime::currentDateTime().toString( Qt::ISODate );
                     QString msg( QJsonDocument( j_infoitem ).toJson( QJsonDocument::Compact ) );
                     session->call("swiftbot.components.actions.insert", {"locked_withdraw_security"+getExchangeName(), msg});
                 } else if (j_result.value("error").toString().contains("40017")  ) {
                     QJsonObject j_infoitem;
                     j_infoitem["uniquekey"] = "api_key_error"+getExchangeName();
                     j_infoitem["exchange"] = getExchangeName();
                     j_infoitem["title"] = "Неверный API ключ";
                     j_infoitem["text"] = "Необходимо обновить ключ API.";
                     j_infoitem["code"] = "40017";
                     j_infoitem["url"] = "";
                     j_infoitem["ts"] = QDateTime::currentDateTime().toString( Qt::ISODate );
                     QString msg( QJsonDocument( j_infoitem ).toJson( QJsonDocument::Compact ) );
                     session->call("swiftbot.components.actions.insert", {"api_key_error"+getExchangeName(), msg});
                     request_pause = true;
                 }
             } else if ( getExchangeId() == 8 ) {
                 if(j_result.value("error").toObject().value("code") == "260325" ) {
                     QString addr = _async_params.contains( uuid ) ? _async_params.value( uuid ).value("address").toString():"#";
                     QJsonObject j_infoitem;
                     j_infoitem["uniquekey"] = "motin_withdraw_whitelists"+getExchangeName();
                     j_infoitem["exchange"] = getExchangeName();
                     j_infoitem["title"] = "Адрес вывода не в вайтлисте";
                     j_infoitem["text"] = "Попытка вывода средств не удалась, т.к. указанный адрес не добавлен в вайт лист на бирже.";
                     j_infoitem["code"] = "260325";
                     j_infoitem["url"] = addr;
                     j_infoitem["ts"] = QDateTime::currentDateTime().toString( Qt::ISODate );
                     QString msg( QJsonDocument( j_infoitem ).toJson( QJsonDocument::Compact ) );
                     session->call("swiftbot.components.actions.insert", {"motin_withdraw_whitelists"+getExchangeName(), msg});
                 }
             } else if ( getExchangeId() == 1 ) {
                 if ( j_result.value("error") == "WHITELIST_VIOLATION_WITHDRAWAL_ADDRESS" ) {
                     QString addr = _async_params.contains( uuid ) ? _async_params.value( uuid ).value("address").toString():"#";
                     QJsonObject j_infoitem;
                     j_infoitem["uniquekey"] = "motin_withdraw_whitelists"+getExchangeName();
                     j_infoitem["exchange"] = getExchangeName();
                     j_infoitem["title"] = "Адрес вывода не в вайтлисте";
                     j_infoitem["text"] = "Попытка вывода средств не удалась, т.к. указанный адрес не добавлен в вайт лист на бирже.";
                     j_infoitem["code"] = "WHITELIST_VIOLATION_WITHDRAWAL_ADDRESS";
                     j_infoitem["url"] = addr;
                     j_infoitem["ts"] = QDateTime::currentDateTime().toString( Qt::ISODate );
                     QString msg( QJsonDocument( j_infoitem ).toJson( QJsonDocument::Compact ) );
                     session->call("swiftbot.components.actions.insert", {"motin_withdraw_whitelists"+getExchangeName(), msg});
                 }
             }
             addError( QJsonDocument( result ).toJson(QJsonDocument::Compact ) );
         }
         result["method"] = getMethodName( method );
         registerAsyncResult( uuid, result );
    }
}

void SwiftApiClient::customMethodParsers(const SwiftApiClient::AsyncMethods &method, const QJsonObject &j_result) {
    if ( method == SwiftApiClient::AsyncMethods::GetBalances ) {
        _cached_balances = j_result.value("balances").toArray();
        for( auto it = _cached_balances.begin(); it != _cached_balances.end(); it++ ) {
            for( auto it2 = _cached_currencies.begin(); it2 != _cached_currencies.end(); it2++ ) {
                if( it2->toObject().value("currency_id").toString().toUInt() == it->toObject().value("currency_id").toString().toUInt() ) {
                    QJsonObject re( it2->toObject() );
                    re["deposit_address"] = it->toObject().value("deposit_address").toString();
                    _cached_currencies.replace( it2.i, re );
                    break;
                }
            }
        }
    } else if ( method == SwiftApiClient::AsyncMethods::GetMarkets  ){
        _cached_markets = j_result.value("markets").toArray();
        for( auto it = _cached_markets.begin(); it != _cached_markets.end(); it++ ) {
            _trade_fees[ it->toObject().value("market_id").toString().toUInt() ] = it->toObject().value("trade_fee").toString().toDouble();
            _min_trade_sizes[ it->toObject().value("market_id").toString().toUInt() ] = it->toObject().value("min_trade_size").toString().toDouble();
        }
    } else if ( method == SwiftApiClient::AsyncMethods::GetCurrencies  ){
        _cached_currencies = j_result.value("currencies").toArray();
        for( auto it = _cached_currencies.begin(); it != _cached_currencies.end(); it++ ) {
            _withdraw_fees[ it->toObject().value("currency_id").toString().toUInt() ] = it->toObject().value("withdraw_fee").toString().toDouble();
        }
    } else if ( method == SwiftApiClient::AsyncMethods::TradeOpenOrders  ) {
        _cached_orders = j_result.value("orders").toArray();
    }
    customParser( method, j_result );
}

void SwiftApiClient::methodState(const SwiftApiClient::AsyncMethods &method, const bool &result) {
    const QString& prefix("swift.api.rpc."+QString::number( getExchangeId() )+".");
    if ( session != nullptr &&  session->isJoined() ) {
        session->call("swift.system.rpc.methodstate", QVariantList({result, prefix+SwiftApiClient::getMethodName( method )}) );
    }

}

bool SwiftApiClient::publishWamp(const SwiftLib::WampTopics &topic, const QJsonObject &obj, const quint64 &async_uuid) {

    if ( wamp_connected && session != nullptr && session->isJoined() ) {

        if ( async_uuid > 0 ) {
            QJsonObject obj2( obj );
            obj2["async_uuid"] = QString::number( async_uuid );
            const QString res( QJsonDocument( obj2 ).toJson( QJsonDocument::Compact ) );
            const QString topic_( SwiftLib::getTopic( topic ) );
            session->publish( topic_, QVariantList{res});
        } else {
            const QString res( QJsonDocument( obj ).toJson( QJsonDocument::Compact ) );
            const QString topic_( SwiftLib::getTopic( topic ) );
            session->publish( topic_, QVariantList{res});
        }

        return true;
    } else {
        const QString res( QJsonDocument( obj ).toJson( QJsonDocument::Compact ) );
        return false;
    }
}

void SwiftApiClient::addError(const QString &message) {
    insertError("WARNING", message );
}

void SwiftApiClient::registerOrder(const QJsonObject &j, const quint64 &uuid) {
    if ( debug ) {
        qWarning() << getExchangeName() << "REGISTERING ORDER IN CACHE" << uuid << "ORDER DATA" << j;
    }
    if ( j.contains("arb_window") && j.value("arb_window").toString().toUInt() > 0 ) {
        _wait_order_window[ j.value("amount").toString().toDouble() ] = j.value("arb_window").toString().toUInt();
    }
    _regitered_orders_objects[ j.value("local_id").toString() ] = j;
    _regitered_orders[uuid] =  j.value("local_id").toString();
}

QJsonObject SwiftApiClient::getRegisteredOrder(const quint64 &uuid) {
    if ( _regitered_orders.contains( uuid ) ) {
        return _regitered_orders_objects.take( _regitered_orders.take( uuid ) );
    } else {
        if ( debug ) {
            qWarning() << getExchangeName() << "NO ORDER FOUND IN CACHE" << uuid;
        }
        return QJsonObject();
    }
}

bool SwiftApiClient::isDebug() const {
    return debug;
}

void SwiftApiClient::setCachedBalance(const quint32 &currency_id, const double &total, const double &inner) {
    _balances_inner_cached[currency_id] = inner;
    _balances_total_cached[currency_id] = total;
}

double SwiftApiClient::getTotalCachedBalance(const quint32 &currency_id) const {
    return _balances_total_cached.value( currency_id, 0 );
}

double SwiftApiClient::getInnerCachedBalance(const quint32 &currency_id) const {
    return _balances_inner_cached.value( currency_id, 0 );
}

void SwiftApiClient::resetLimits() {

    _req_total_counter = 0;
}

void SwiftApiClient::unfreeze() {
    request_pause = false;
}

void SwiftApiClient::addLog(const QString& message) {
    insertLog("INFO", message );
}

quint64 SwiftApiClient::registerAsyncCall(const SwiftApiClient::AsyncMethods &method) {
    const quint64 uuid = getNextUuid();
    _asyncs.push_back( uuid );
    _async_dictionary.insert( uuid, method );
    return uuid;
}

quint64 SwiftApiClient::getNextUuid() {
    if ( QDateTime::currentMSecsSinceEpoch() - _uuids_counter > 300  )  {
        _uuids_counter =  QDateTime::currentMSecsSinceEpoch();
    }
    const quint64 last_assigned =  _uuids_counter;
    quint64 new_variant = ++_uuids_counter;

    if ( last_assigned < new_variant ) {
        return new_variant;
    } else {
        while ( last_assigned <= new_variant ) {
            new_variant = ++_uuids_counter;
        }
        return  ++_uuids_counter;
    }
}

void SwiftApiClient::registerAsyncResult(const quint64 &uuid, const QJsonObject &j_result) {
    _async_results.insert( uuid, j_result );
    _async_results_queue.enqueue( uuid );
}


bool SwiftApiClient::sendAsyncResult(const quint64 &uuid) {
    if ( _async_results.contains( uuid ) ) {
        if ( publishWamp( SwiftLib::WampTopics::ApiRpcAsyncResults,  _async_results.value( uuid ),  uuid ) ) {
            const AsyncMethods method = _async_dictionary.value( uuid );
            const QJsonObject res( _async_results.value( uuid ) );
            methodState( method, res.value("success").toBool(false) );
            emit apiParsedResponse( method, res );
            _async_params.remove( uuid );
            _async_dictionary.remove( uuid );
            _async_results.remove( uuid );
            _asyncs.removeOne( uuid );
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void SwiftApiClient::unrealizedMethod(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    ret["method"] = getMethodName( _async_dictionary.value( async_uuid ) );
    ret["exchange_name"] = getExchangeName();
    ret["exchange_id"] = QString::number( getExchangeId() );
    registerAsyncResult( async_uuid, ret );
}

QString SwiftApiClient::paramMapToStr(const QMap<QString, QString>& params) const {
    QStringList result;
    for(auto it = params.cbegin(); it != params.cend(); ++it)
        result.append(it.key() + "=" + it.value());
    return result.join("&");
}

bool SwiftApiClient::isParamExist(const QStringList& params, const QJsonObject& j_params, QString& error) const {
    QStringList absentParams;
    for(auto& p : params){
        if(!j_params.contains(p))
            absentParams.append(p);
    }
    if(absentParams.isEmpty()){
        error = "";
        return true;
    }
    else {
        error = "Absent parameters: <" + absentParams.join(",") + ">";
        return false;
    }
}

bool SwiftApiClient::isUintValid(quint32 id, QString& error, const QString& id_name) const {
    if(id > 0)
        return true;
    error = "Invalid parameter - " + id_name;
    return false;
}

bool SwiftApiClient::isDoublePlus(double val, QString& error, const QString& id_name) const {
    if(val > 0)
        return true;
    error = "Invalid parameter - " + id_name;
    return false;
}

bool SwiftApiClient::isSideValid(const QString &side, QString &error) const
{
    if(side == "sell" || side == "buy")
        return true;
    error = "Invalid parameter - type";
    return false;
}

bool SwiftApiClient::isUintInRange(quint32 val, quint32 r_min, quint32 r_max, bool inclusive) const
{
    if(inclusive)
        return (val >= r_min && val <= r_max) ? true : false;
    else
        return (val > r_min && val < r_max) ? true : false;
}



void SwiftApiClient::initWorker(Wamp::Session *sess)
{
    onWampSession_(sess);
}

QString SwiftApiClient::getWorkerName() const
{
    return "api-"+getExchangeName();
}


QStringList SwiftApiClient::listenParams()
{
    return QStringList({
       SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"api_key"),
       SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"api_secret"),
       SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"username"),
       "EXCHANGES"
    });
}

void SwiftApiClient::onParamChanged(const QString &param, const QString &value)
{
    if ( param == SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"api_key") ) {
         api_key = value;
    } else if ( param == SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"api_secret") ) {
         api_secret = value;
    } else if ( param == SwiftLib::exchangeParamPath( getExchangeName().toLower() ,"username") ) {
        api_user = value;
    } else if ( param == "EXCHANGES" ) {
        QStringList _e( value.split(",") );
        if ( _e.contains( getExchangeName() ) ) {
            setStatus( 1 );
        } else {
            setStatus( 0 );
        }
    }
}
