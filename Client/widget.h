#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "../Message/Message.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void connection_check();
    void on_SEND_clicked();
    void readFromServer();

private:
    Ui::Widget *ui;
    QTcpSocket* socket;
    QByteArray data;
};
#endif // WIDGET_H
