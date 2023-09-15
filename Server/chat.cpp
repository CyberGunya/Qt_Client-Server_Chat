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

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<socketDescriptor<<" connected"<<"\n";
    socket_sender = new QTcpSocket;
    socket_sender->setSocketDescriptor(socketDescriptor);
    connect(socket_sender, &QTcpSocket::destroyed, socket_sender, &QTcpSocket::deleteLater);
    connect(socket_sender, &QTcpSocket::readyRead, this, &ChatServer::readFromClient);
    this->readFromClient();
}

void ChatServer::readFromClient()
{
    QDataStream in(socket_sender);
    in.setVersion(QDataStream::Qt_6_5);
    if(in.status()==QDataStream::Ok)
    {
        QString msg;
        in>>msg;
        qDebug()<<"Message from "<<socket_sender->socketDescriptor()<<" : "<<msg<<"\n";
    }
    else
    {
        qDebug()<<"Failed to read"<<"\n";
    }
}

void ChatServer::sendToClient(QString str)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out<<str;
    socket_sender->write(data);
    qDebug()<<"Message to "<<socket_sender->socketDescriptor()<<" : "<<str<<"\n";
}
