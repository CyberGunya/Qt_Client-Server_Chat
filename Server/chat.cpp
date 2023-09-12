#include "chat.h"
#include <iostream>

ChatServer::ChatServer()
{
    if(!this->listen(QHostAddress::Any,7000))
    {
        qDebug()<<"Failed to create a server"<<"\n";
    }
    else
    {
        qDebug()<<"Server successfully created"<<"\n";
    }
}
ChatServer::~ChatServer()
{
    this->close();
}
