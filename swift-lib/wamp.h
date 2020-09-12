#ifndef WAMP_H
#define WAMP_H

#include <QMetaMethod>
#include <QTime>
#include <QVariant>
#include <QWebSocket>

#include <functional>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
class QIODevice;

namespace Wamp {

  /** statistics of calling single endpoints, statistics collection must be manually activated in session */
  struct CallStatistics {
      int callCount = 0;      /**< number of endpoint calls since session start */
      int averageTime = 0;    /**< average execution time of single endpoint call */
  };

  /** function for calculate hash in wampcra authorization */
  QString HmacSHA256(const QString &secret, const QString &key);


  /** Handler type for use with session::subscribe(const QString&, Handler) */
  using Handler = std::function<void(const QVariantList&, const QVariantMap&)>;

  /// Endpoint type for use with session::provide(const QString&, Endpoint)
  struct Endpoint {
    typedef std::function<QVariant(const QVariantList&, const QVariantMap&)> Function;
    enum Type { Sync, Async };

    Function function;
    Type type;
  };

  /** Function that can be used for wrapping all endpoints, ie for some high level API implementation */
  typedef std::function<QVariant(const QVariantList &, const QVariantMap &, Wamp::Endpoint::Function)> EndpointWrapper;

  /// Represents a procedure registration.
  struct Registration {
      inline Registration(quint64 id, const QString &procedure) : id(id), procedure(procedure) {}

      quint64 id;
      QString procedure;
  };

  /// An outstanding WAMP registration request.
  struct RegisterRequest {
      inline RegisterRequest(const QString &procedure, Endpoint endpoint) : procedure(procedure), endpoint(endpoint) {}

      QString procedure;
      Endpoint endpoint;
  };


  /// Represents a topic subscription.
  struct Subscription {
      inline Subscription(quint64 id, const QString &topic) : id(id), topic(topic) {}

      quint64 id;
      QString topic;
  };

  /// An outstanding WAMP subscribe request.
  struct SubscribeRequest {
      inline SubscribeRequest(const QString &topic, Handler handler) : topic(topic), handler(handler) {}

      QString topic;
      Handler handler;
  };

  struct CallRequest {
      inline CallRequest() : ready(false) {}

      bool ready;
      QVariant result;
      QString ex;
  };

  /** authentication request */
  struct Challenge {
      QString authid;
      QString authrole;
      QString authmethod;
      QString authprovider;
      QString nonce;
      QDateTime timestamp;
      qint64 session;
  };

  /**
   * A WAMP session.
   */
  class Session : public QObject {

      Q_OBJECT

    Q_SIGNALS:
      void started();
      void joined(quint64);
      void left(QString);
      void subscribed(Subscription);
      void registered(Registration);
      void challenge(const QString &, const QString &, const Challenge &);

  public slots:
      void publishMessage( const QString& topic, const QJsonObject& j_msg ) {
          const QString r( QJsonDocument( j_msg ).toJson( QJsonDocument::Compact ) );
          publish(topic, QVariantList({r}) );
      }
    private Q_SLOTS:
      void readData();
      void readMessage(const QString &message);

    public:

      enum class MessageFormat { Msgpack, Json };
      Q_ENUM(MessageFormat)
      enum class TransportType { WebSocket, RawSocket };
      Q_ENUM(TransportType)

      /**
       * Create a new WAMP session.
       *
       * @param in - input stream to run this session on
       * @param out - output stream to run this session on
       * @param transport - transport type
       * @param debug - whether log every procedure call and every published message
       *
       */
      Session(QIODevice &in, QIODevice &out, MessageFormat messageFormat = MessageFormat::Msgpack, bool debug = false);

      /**
       * Overloaded previous function with single in/out stream
       *
       * @param inout - stream to run this session on
       * @param transport - transport type
       * @param debug - whether log every procedure call and every published message
       */
      Session(QIODevice &inout, MessageFormat transport = MessageFormat::Msgpack, bool debug = false);

      /**
       * Create a new named WAMP session.
       *
       * @param name - session name, can be part of procedure names and topic names (like myapp.someproc())
       * @param in - input stream to run this session on.
       * @param out - output stream to run this session on.
       * @param transport - transport type
       * @param debug - whether log every procedure call and every published message
       */
      Session(const QString &name, QIODevice &in, QIODevice &out, MessageFormat transport = MessageFormat::Msgpack, bool debug = false);

      /**
       * Overloaded previous function with single in/out stream
       *
       * @param name - session name, can be part of procedure names and topic names (like myapp.someproc())
       * @param inout - stream to run this session on
       * @param transport - transport type
       * @param debug - whether log every procedure call and every published message
       */
      Session(const QString &name, QIODevice &inout, MessageFormat transport = MessageFormat::Msgpack, bool debug = false);

      Session(const QString &name, QWebSocket &websocket, MessageFormat messageFormat = MessageFormat::Json, bool debug = false);
      Session(QWebSocket &websocket, MessageFormat messageFormat = MessageFormat::Json, bool debug = false);

      /**
       * Gets session name
       * @return
       */
      const QString &name() const;

      /**
       * Sets the session name (it appears as a prefix in published methods and topics)
       * @param name - name of session
       */
      void setName(const QString &name);

      /** Gets session debugging state */
      inline bool debug() const { return m_debug; }

      /** Sets/unsets session debugging */
      inline void setDebug(bool b) { m_debug = b; }

      /**
       * Start listening on the input stream provided to the constructor
       * of this session.
       */
      void start();

      /**
       * Closes the input stream and the output stream provided to the constructor
       * of this session.
       */
      void stop();

      /**
       * Join a realm with this session. On sucessfull is emitted signal "joined()"
       *
       * @param realm - realm to join on the WAMP router connected to
       * @param authid - login name if required
       * @param authmethods - list of supported methods
       */
      void join(const QString &realm, const QString &authid = QString(), const QStringList &authmethods = QStringList());

      /**
       * Whether session is already joined or not
       */
      inline bool isJoined() const  { return m_isJoined; }

      /**
       * Leave the realm.
       *
       * @param reason An optional WAMP URI providing a reason for leaving.
       */
      void leave(const QString &reason = QString("wamp.error.close_realm"));


      /**
       * Subscribe a handler to a topic to receive events.
       *
       * @param topic The URI of the topic to subscribe to.
       * @param handler The handler that will receive events under the subscription.
       */
      void subscribe(const QString &topic, Handler handler);

      void authenticate(const QString &credentials);

      /**
       * Calls a remote procedure with positional and keyword arguments.
       *
       * @param procedure The URI of the remote procedure to call.
       * @param args The positional arguments for the call.
       * @param kwargs The keyword arguments for the call.
       * @return QVariant containing remote procedure call result.
       */
      QVariant call(const QString &procedure, const QVariantList &args = QVariantList(), const QVariantMap &kwargs = QVariantMap());


      /**
       * Register an endpoint as a procedure that can be called remotely.
       *
       * @param procedure The URI under which the procedure is to be exposed.
       * @param endpoint The endpoint to be exposed as a remotely callable procedure.
       * @param options Options when registering a procedure.
       */
      void provide(const QString& procedure, Endpoint::Function endpointFunction, Endpoint::Type endpointType = Endpoint::Sync, const QVariantMap& options = QVariantMap());
      void provideStatistics();
      void provideGetMethods();

      const QHash<QString, CallStatistics> &callStatistics() const { return m_callStatistics; }
      inline void setEndpointWrapper(EndpointWrapper w) { endpointWrapper = w; }

    public Q_SLOTS:
      /**
       * Publish an event with both positional and keyword payload to a topic.
       *
       * @param topic The URI of the topic to publish to.
       * @param args The positional payload for the event.
       * @param kwargs The keyword payload for the event.
       */
      void publish(const QString &topic, const QVariantList &args = QVariantList(), const QVariantMap &kwargs = QVariantMap());

    private:
      QString makeName(const QString &name) const;
      bool isUint64(const QVariant &v);
      QVariant convertParam(const QVariant &arg);
      QVariantList convertParams(const QVariantList &args);
      void init();

      /// Map of outstanding WAMP calls (request ID -> call).
      typedef QHash<quint64, CallRequest> CallRequests;

      /// Map of WAMP call ID -> call
      CallRequests callRequests;

      /// Map of outstanding WAMP subscribe requests (request ID -> subscribe request).
      typedef QHash<quint64, SubscribeRequest> SubscribeRequests;

      /// Map of WAMP subscribe request ID -> subscribe request
      SubscribeRequests subscribeRequests;

      /// Map of subscribed handlers (subscription ID -> handler)
      typedef QMultiHash<quint64, Handler> Handlers;

      /// Map of WAMP subscription ID -> handler
      Handlers handlers;


      //////////////////////////////////////////////////////////////////////////////////////
      /// Callee

      /// An outstanding WAMP register request.

      /// Map of outstanding WAMP register requests (request ID -> register request).
      typedef QHash<quint64, RegisterRequest> RegisterRequests;

      /// Map of WAMP register request ID -> register request
      RegisterRequests registerRequests;

      typedef QHash<quint64, Endpoint> Endpoints;
      /// Map of WAMP registration ID -> endpoint
      Endpoints endpoints;

      /// Process a WAMP ERROR message.
      void processError(const QVariantList &msg);

      /// Process a WAMP HELLO message.
      void processWelcome(const QVariantList &msg);

      /// Process a WAMP CHALLENGE message.
      void processChallenge(const QVariantList &msg);

      /// Process a WAMP RESULT message.
      void processCallResult(const QVariantList &msg);

      /// Process a WAMP SUBSCRIBED message.
      void processSubscribed(const QVariantList &msg);

      /// Process a WAMP EVENT message.
      void processEvent(const QVariantList &msg);

      /// Process a WAMP REGISTERED message.
      void processRegistered(const QVariantList &msg);

      /// Process a WAMP INVOCATION message.
      void processInvocation(const QVariantList &msg);

      /// Process a WAMP GOODBYE message.
      void processGoodbye(const QVariantList &msg);


      /// Send out message serialized in serialization buffer to ostream.
      void send(const QVariantList &message);

      void getHandshakeReply();
      void getMsgHeader();
      void getMsgBody();

      void gotMsg(const QVariant &obj);

      bool m_debug;
      bool m_stopped;

      /// Input stream this session runs on.
      QIODevice &m_in;

      /// Output stream this session runs on.
      QIODevice &m_out;

      QWebSocket &m_websocket;

      bool m_isJoined;
      char m_bufferMsgLen[4];
      quint32 m_msgLen;
      quint32 m_msgRead;

      /// MsgPack serialization buffer.
      QByteArray readBuffer;

      /// WAMP session ID (if the session is joined to a realm).
      quint64 m_sessionId;

      /// Last request ID of outgoing WAMP requests.
      quint64 m_requestId;

      bool m_goodbyeSent;
      QString m_name;
      QHash<QString, CallStatistics> m_callStatistics;
      QMap<QString, QStringList> m_methods;

      EndpointWrapper endpointWrapper;
      MessageFormat m_messageFormat;
      TransportType m_transportType;

      enum State {
        Initial,
        Started,
        ReadingMessage
      };

      State state;

      /// WAMP message type codes.
      enum class WampMsgCode : int {
        HELLO = 1,
        WELCOME = 2,
        ABORT = 3,
        CHALLENGE = 4,
        AUTHENTICATE = 5,
        GOODBYE = 6,
        HEARTBEAT = 7,
        ERROR = 8,
        PUBLISH = 16,
        PUBLISHED = 17,
        SUBSCRIBE = 32,
        SUBSCRIBED = 33,
        UNSUBSCRIBE = 34,
        UNSUBSCRIBED = 35,
        EVENT = 36,
        CALL = 48,
        CANCEL = 49,
        RESULT = 50,
        REGISTER = 64,
        REGISTERED = 65,
        UNREGISTER = 66,
        UNREGISTERED = 67,
        INVOCATION = 68,
        INTERRUPT = 69,
        YIELD = 70
      };
  };

  class ProtocolError : public std::runtime_error {
    public:
      inline ProtocolError(const QString &msg) : std::runtime_error(msg.toStdString()) {}
  };

  class NoSessionError : public std::runtime_error {
    public:
      inline NoSessionError() : std::runtime_error("session not joined") {}
  };

  class AuthorizationError : public std::runtime_error {
    public:
      inline AuthorizationError(const QString &msg) : std::runtime_error(msg.toStdString()) {}
  };
}

Q_DECLARE_METATYPE(Wamp::Session::MessageFormat)
Q_DECLARE_METATYPE(Wamp::Session::TransportType)

#endif // WAMP_H
