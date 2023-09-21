#ifndef MESSAGE_H
#define MESSAGE_H

#include <QTcpSocket>
#include <QDateTime>

class Message
{
public:
    qintptr sender;
    qintptr receiver;
    QString text;
    QDateTime time;
};

#endif // MESSAGE_H
