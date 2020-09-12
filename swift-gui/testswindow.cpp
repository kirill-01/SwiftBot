#include "testswindow.h"
#include "ui_testswindow.h"
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>

TestsWindow::TestsWindow(QWidget *parent, Wamp::Session * sess ) :
    QMainWindow(parent),
    ui(new Ui::TestsWindow)
{
    ui->setupUi(this);
    session = sess;
    QSqlQuery q("SELECT * FROM assets.exchanges");
    if ( q.exec() ) {
        while ( q.next() ) {
            ui->targetExchange->addItem( q.value("name").toString() );
            ui->withdrawTargetExchange->addItem( q.value("name").toString() );
        }
    }
}

TestsWindow::~TestsWindow()
{
    delete ui;
}

void TestsWindow::on_pushButton_clicked()
{

    qWarning() << "Starting tests" << getTestParams();
}

QJsonObject TestsWindow::getTestParams() {
    QJsonObject j_ret;
    j_ret["exchange"] = QString::number( ui->targetExchange->currentIndex() );
    j_ret["withdraw_exchange"] = QString::number( ui->withdrawTargetExchange ->currentIndex() );
    j_ret["order_size"] = QString::number( ui->orderSize->value(), 'f', 8 );
    j_ret["withdraw_amount"] = QString::number( ui->withdrawAmount->value(), 'f', 8 );
    j_ret["withdraw_amount_usdt"] = QString::number( ui->withdrawAmountUsd->value(), 'f', 8 );
    return j_ret;
}
