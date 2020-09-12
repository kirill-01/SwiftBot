#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include "testswindow.h"
#include <QMessageBox>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      is_connected(false),
      async_subscribed(false)
{
    ui->setupUi(this);
    commandshistory = new QFile("/opt/swiftbot/gui_cmds.history");
    commandshistory->open( QFile::ReadWrite );

    ui->lineEdit->installEventFilter( this );
    _commands = QString(commandshistory->readAll()).split("\n");
    if ( !_commands.isEmpty() ) {
        ui->lineEdit->setText( _commands.last() );
        commandindex = _commands.length() - 1;
    }

    onTextAppend( "Ready to work" );
    onTextAppend( "Choose realm to connect" );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onWampSession(Wamp::Session *sess) {
    session = sess;
    ui->errorsText->clear();
    ui->logsText->clear();
    ui->asyncText->clear();
    ui->labelStatus->setText("Connected");
    onTextAppend("Session started");
    session->subscribe("swiftbot.system.errors",[=](const QVariantList& v, const QVariantMap&m ) {
        if ( errs_items >= 5000 ) {
            ui->errorsText->clear();
            errs_items=0;
        }
        ui->errorsText->append( v.at(0).toString() );
        errs_items++;
    });

    session->subscribe("swiftbot.system.logs",[=](const QVariantList& v, const QVariantMap&m ) {
        if ( logs_items >= 5000 ) {
            ui->logsText->clear();
            logs_items=0;
        }
        ui->logsText->append( v.at(0).toString() );
        logs_items++;
    });
    session->subscribe("swiftbot.api.rpc.asyncresults",[&]( const QVariantList&l, const QVariantMap& m ){
        Q_UNUSED( m )
        const QJsonObject jrec( QJsonDocument::fromJson( l.at(0).toString().toUtf8() ).object() );
        const quint64 uuid = jrec.value("async_uuid").toVariant().toULongLong();
        if ( uuid == lastwaiting ) {
            QMessageBox::information( this, "Асинхронный результат", l.at(0).toString() );
        }
        ui->asyncText->append( l.at(0).toString() );
    });
    is_connected=true;
}

void MainWindow::onTextAppend(const QString &text) {
    ui->textBrowser->append( "* " + text );
}

void MainWindow::onCommandEntered(const QString &command) {
    ui->textBrowser->append( QTime::currentTime().toString() + " >> " + command );
}


void MainWindow::on_actionKaa_triggered()
{
    is_connected=false;
    ui->actionKaa->setChecked(true);
    ui->actionPchel->setChecked(false);
    emit connectRealm("kaa");
}

void MainWindow::on_lineEdit_returnPressed()
{  
    QTextStream str(commandshistory);

    str << ui->lineEdit->text() << "\n";
    _commands.insert( commandindex+1 ,  ui->lineEdit->text() );

    onCommandEntered( ui->lineEdit->text() );
    if ( is_connected ) {
        const QStringList separated( ui->lineEdit->text().split(" ") );
        qWarning() << separated;
        if ( separated.size() == 2 ) {

            const QString res = session->call( separated.at(0), {separated.at(1)} ).toString();
            qWarning() << QJsonDocument::fromJson( res.toUtf8() ).toJson( QJsonDocument::Indented );
            if ( res.toULongLong() > 0 ) {
                lastwaiting =  res.toULongLong();
            } else {
                QMessageBox::information( this, "Результат", QJsonDocument::fromJson( res.toUtf8() ).toJson( QJsonDocument::Indented ) );
            }

            onTextAppend( res );
        } else {
            const QString res = session->call( ui->lineEdit->text() ).toString();
            if ( res.toULongLong() > 0 ) {
                lastwaiting =  res.toULongLong();
            } else {
                QMessageBox::information( this, "Результат", QJsonDocument::fromJson( res.toUtf8() ).toJson( QJsonDocument::Indented )  );
            }
            qWarning() << QJsonDocument::fromJson( res.toUtf8() ).toJson( QJsonDocument::Indented );
            onTextAppend( res );
        }
    }
     ui->lineEdit->clear();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Up) {
                if ( commandindex > 0 ) {
                    commandindex--;
                } else {
                    commandindex = _commands.size() - 1;
                }
                ui->lineEdit->setText( _commands.at( commandindex ) );
                return true;
            } else  if (keyEvent->key() == Qt::Key_Down ) {
               if ( commandindex < _commands.size() -1) {
                   commandindex++;
               } else {
                   commandindex = 0;
               }
               ui->lineEdit->setText( _commands.at( commandindex ) );
               return true;
            }
        }

    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{

}

void MainWindow::on_actionOpen_tests_window_triggered()
{
    TestsWindow * testwin = new TestsWindow(this);
    testwin->show();
}

void MainWindow::on_actionPchel_triggered()
{
    is_connected=false;
    ui->actionKaa->setChecked(false);
    ui->actionPchel->setChecked(true);
    emit connectRealm("pchel");
}
