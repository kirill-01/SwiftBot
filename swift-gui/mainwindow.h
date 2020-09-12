#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../swift-lib/wamp.h"
#include <QFile>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void connectRealm( const QString& realmname );
public slots:
    void onWampSession( Wamp::Session * sess );

    void onTextAppend( const QString& text );
    void onCommandEntered( const QString& command );
    void on_actionKaa_triggered();
    void on_actionPchel_triggered();
    void on_lineEdit_returnPressed();

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_actionOpen_tests_window_triggered();


protected:
    void changeEvent(QEvent *e) {

    }
    bool eventFilter(QObject* obj, QEvent *event);
private:
    quint64 logs_items;
    quint64 errs_items;
    Ui::MainWindow *ui;
    Wamp::Session * session;
    bool is_connected;
    bool async_subscribed;
    quint64 lastwaiting;
    QFile * commandshistory;
    QStringList _commands;
    quint64 commandindex;
};
#endif // MAINWINDOW_H
