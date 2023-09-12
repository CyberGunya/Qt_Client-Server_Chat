#include <QCoreApplication>
#include "chat.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ChatServer chat;
    return a.exec();
}
