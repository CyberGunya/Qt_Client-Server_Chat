#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Widget::connection_check);
    //socket->connectToHost("192.168.0.103",7000);
    socket->connectToHost("127.0.0.1",7000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connection_check()
{
    qDebug()<<"Successfuly connected to server"<<"\n";
}

