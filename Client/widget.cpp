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

void Widget::print_msg(QString text)
{
    ui->textBrowser->append(text+" : "+QDateTime::currentDateTime().toString());
}

void Widget::on_SEND_clicked()
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out<<ui->lineEdit->text();
    socket->write(data);
    ui->textBrowser->setTextBackgroundColor(QColor("white"));
    ui->textBrowser->setTextColor(QColor("red"));
    print_msg(ui->lineEdit->text());
    qDebug()<<"Message to "<<socket->socketDescriptor()<<" : "<<ui->lineEdit->text()<<"\n";
    ui->lineEdit->clear();
}

void Widget::readFromServer()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if(in.status()==QDataStream::Ok)
    {
        QString msg;
        in>>msg;
        ui->textBrowser->setTextColor(QColor("blue"));
        ui->textBrowser->setTextBackgroundColor(QColor("light gray"));
        print_msg(msg);
        qDebug()<<"Message from "<<socket->socketDescriptor()<<" : "<<msg<<"\n";
    }
    else
    {
        ui->textBrowser->setTextColor(QColor("blue"));
        ui->textBrowser->append("Failed to receive");
    }
}
