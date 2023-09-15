#ifndef CHAT_H
#define CHAT_H

#include <QTcpServer>
#include <QTcpSocket>
#include <set>

class ChatServer : public QTcpServer
{
    Q_OBJECT
    QTcpSocket* socket_sender;
    QTcpSocket* socket_reciever;
    QByteArray data;
public:
    ChatServer();
    ~ChatServer();
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void readFromClient();
    void sendToClient(QString str);
 };

#endif // CHAT_H
