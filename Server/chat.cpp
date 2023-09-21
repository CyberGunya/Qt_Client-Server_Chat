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
    delete socket_sender;
    delete socket_reciever;
    this->close();
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<socketDescriptor<<" connected"<<"\n";
    socket_sender = new QTcpSocket;
    socket_sender->setSocketDescriptor(socketDescriptor);

    connect(socket_sender, &QTcpSocket::destroyed, socket_sender, &QTcpSocket::deleteLater);
    connect(socket_sender, &QTcpSocket::readyRead, this, &ChatServer::readFromClient);
    connect(socket_sender, &QTcpSocket::disconnected, this, &ChatServer::disconnect);

    users.push_back(socket_sender);
    Notify(socket_sender->socketDescriptor());
}

void ChatServer::readFromClient()
{
    QTcpSocket* temp = (QTcpSocket*)sender();
    QDataStream in(temp);
    in.setVersion(QDataStream::Qt_6_5);
    if(in.status()==QDataStream::Ok)
    {
        Message msg;
        in>>msg.sender>>msg.receiver>>msg.text>>msg.time;
        msg.sender=temp->socketDescriptor();
        Send(msg);
    }
    else
    {
        qDebug()<<"Failed to read"<<"\n";
    }
}

QTcpSocket* ChatServer::Find(qintptr descriptor)
{
    for(auto i:users)
    {
        if(i->socketDescriptor()==descriptor)
        {
            return i;
        }
    }
    return nullptr;
}

void ChatServer::Send(Message msg)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    auto temp = Find(msg.receiver);
        //check if receiver is still connected
    if(temp!=nullptr)
    {
        out<<msg.sender<<msg.receiver<<msg.text<<msg.time;
        temp->write(data);
        qDebug()<<"Message to "<<msg.receiver<<" : "<<msg.text<<"\n";
    }
    else
    {
        qDebug()<<"Receiver is not connected!"<<"\n";
    }
}

void ChatServer::Notify(qintptr newbie)     //Notify all users about new user
{
    Message msg;
    msg.sender=-1;
    msg.text = QString::number(newbie);
    msg.time = QDateTime::currentDateTime();
        //Notify all users about new one
    for(auto i:users)
    {
        msg.receiver=i->socketDescriptor();
        Send(msg);
    }
        //Send to the newbie list of other available users
    msg.receiver=newbie;
    for(size_t i = 0;i<users.count()-1;++i)
    {
        msg.text = QString::number(users[i]->socketDescriptor());
        Send(msg);
    }
}

void ChatServer::disconnect()
{
    QTcpSocket* temp = (QTcpSocket*)sender();
    users.removeAll(temp);
    // TODO: notify all users, that this particular one is disconnected
}
