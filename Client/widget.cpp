#include "widget.h"
#include "ui_widget.h"

ChatClient::ChatClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &ChatClient::connection_check);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::readFromServer);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &ChatClient::updateBrowser);
    socket->connectToHost("127.0.0.1",7000);
}

ChatClient::~ChatClient()
{
    delete ui;
    delete socket;
}

void ChatClient::connection_check()
{
    qDebug()<<"Successfuly connected to server"<<"\n";
}

void ChatClient::print_msg(Message msg)
{
    if(msg.sender==my_id)   //outcoming message
    {
        ui->textBrowser->setTextBackgroundColor(QColor("white"));
        ui->textBrowser->setTextColor(QColor("red"));
    }
    else            //incoming message
    {
        ui->textBrowser->setTextColor(QColor("blue"));
        ui->textBrowser->setTextBackgroundColor(QColor("light gray"));
    }
    ui->textBrowser->append(msg.text+" : "+msg.time.toString());
}

void ChatClient::on_SEND_clicked()
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
        //Make a new message
    Message msg;
    msg.sender = my_id;
    msg.receiver = ui->comboBox->currentText().toInt();
    msg.text = ui->lineEdit->text();
    msg.time = QDateTime::currentDateTime();
        //send it to server
    out<<msg.sender<<msg.receiver<<msg.text<<msg.time;
    socket->write(data);
        //update chat history
    history[msg.receiver].push_back(msg);
    if(msg.receiver==ui->comboBox->currentText().toInt())
    {
        print_msg(msg);
    }
        //clear input
    ui->lineEdit->clear();
    qDebug()<<"Message to "<<msg.receiver<<" : "<<ui->lineEdit->text()<<"\n";
}

void ChatClient::readFromServer()
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
                    if(my_id==-1)       //remember socket descriptor for this user
                    {
                        my_id=msg.text.toInt();
                    }
                }
            }
            if(msg.text!="")        //fix this later
            {
                //update chat history only if it's not a message to yourself
                if(msg.sender!=msg.receiver)
                {
                    history[msg.sender].push_back(msg);
                    if(msg.sender==ui->comboBox->currentText().toInt())
                    {
                        print_msg(msg);
                    }
                    qDebug()<<"Message from "<<msg.sender<<" : "<<msg.text<<"\n";
                }
            }
        }
    }
    else
    {
        ui->textBrowser->setTextColor(QColor("blue"));
        ui->textBrowser->append("Failed to receive");
    }
}

void ChatClient::updateBrowser(const QString &text)
{
    ui->textBrowser->clear();
    for(auto i:history[text.toInt()])
    {
        print_msg(i);
    }
}
