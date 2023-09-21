#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QDateTime>
#include <map>
#include "../Message/Message.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class ChatClient : public QWidget
{
    Q_OBJECT

public:
    ChatClient(QWidget *parent = nullptr);
    ~ChatClient();
    void print_msg(Message msg);

public slots:
    void connection_check();
    void on_SEND_clicked();
    void readFromServer();
    void updateBrowser(const QString& text);

private:
    Ui::Widget *ui;
    QTcpSocket* socket;
    QByteArray data;
    std::map<qintptr,QList<Message>> history;
    qintptr my_id = -1;
};
#endif // WIDGET_H
