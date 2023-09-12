#ifndef CHAT_H
#define CHAT_H

#include <QTcpServer>
#include <QTcpSocket>
#include <set>

class ChatServer : QTcpServer
{
private:
    Q_OBJECT
public:
    ChatServer();
    ~ChatServer();
};

#endif // CHAT_H
