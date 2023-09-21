#ifndef CHAT_H
#define CHAT_H

#include <QTcpServer>
#include <QTcpSocket>
#include <set>
#include "../Message/Message.h"

class ChatServer : public QTcpServer
{
    Q_OBJECT
    QTcpSocket* socket_sender;
    QTcpSocket* socket_reciever;
    QByteArray data;
    QList<QTcpSocket*> users;
public:
    ChatServer();
    ~ChatServer();
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void readFromClient();
    void Notify(qintptr newbie);
    void Send(Message msg);
    QTcpSocket* Find(qintptr descriptor);
    void disconnect();
 };

#endif // CHAT_H
