#ifndef WITHDRAWPARAMS_H
#define WITHDRAWPARAMS_H

#include <QObject>

class WithdrawParams : public QObject
{
    Q_OBJECT
public:
    explicit WithdrawParams(QObject *parent = nullptr);

signals:

};

#endif // WITHDRAWPARAMS_H
