#ifndef TESTSWINDOW_H
#define TESTSWINDOW_H

#include <QMainWindow>
#include "../swift-lib/wamp.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>

namespace Ui {
class TestsWindow;
}

class TestsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestsWindow(QWidget *parent = nullptr, Wamp::Session * sess = nullptr );
    ~TestsWindow();
    Wamp::Session * session;
private slots:
    void on_pushButton_clicked();

private:
    QJsonObject getTestParams();
    Ui::TestsWindow *ui;
};

#endif // TESTSWINDOW_H
