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
        //Make a new message
    Message msg;
    msg.sender = 0;     //fix this one
    msg.receiver = ui->comboBox->currentText().toInt();
    msg.text = ui->lineEdit->text();
    msg.time = QDateTime::currentDateTime();
        //send it to server
    out<<msg.sender<<msg.receiver<<msg.text<<msg.time;
    socket->write(data);
        //show result in textbrowser
    ui->textBrowser->setTextBackgroundColor(QColor("white"));
    ui->textBrowser->setTextColor(QColor("red"));
    print_msg(ui->lineEdit->text());
        //cleat input
    ui->lineEdit->clear();
    qDebug()<<"Message to "<<msg.receiver<<" : "<<ui->lineEdit->text()<<"\n";
}

void Widget::readFromServer()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if(in.status()==QDataStream::Ok)
    {
        Message msg;
        while(in.status()==QDataStream::Ok)
        {
            in>>msg.sender>>msg.receiver>>msg.text>>msg.time;
            if(msg.sender==-1)      //section for new users notifier
            {
                if(ui->comboBox->findText(msg.text)==-1 && msg.text!="")
                {
                    ui->comboBox->addItem(msg.text);
                }
            }
            if(msg.text!="")        //fix this later
            {
                ui->textBrowser->setTextColor(QColor("blue"));
                ui->textBrowser->setTextBackgroundColor(QColor("light gray"));
                print_msg(msg.text);
                qDebug()<<"Message from "<<msg.sender<<" : "<<msg.text<<"\n";
            }
        }
    }
    else
    {
        ui->textBrowser->setTextColor(QColor("blue"));
        ui->textBrowser->append("Failed to receive");
    }
}
