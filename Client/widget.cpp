#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Widget::connection_check);
    connect(socket, &QTcpSocket::readyRead, this, &Widget::readFromServer);
    socket->connectToHost("127.0.0.1",7000);
}

Widget::~Widget()
{
    delete ui;
    delete socket;
}

void Widget::connection_check()
{
    qDebug()<<"Successfuly connected to server"<<"\n";
}

void Widget::on_SEND_clicked()
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out<<ui->lineEdit->text();
    socket->write(data);
    qDebug()<<"Message to "<<socket->socketDescriptor()<<" : "<<ui->lineEdit->text()<<"\n";
}

void Widget::readFromServer()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if(in.status())
    {
        QString msg;
        in>>msg;
        qDebug()<<"Message from "<<socket->socketDescriptor()<<" : "<<msg<<"\n";
    }
}
