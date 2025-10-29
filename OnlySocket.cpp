#include "OnlySocket.h"

OnlySocket* OnlySocket::instance = nullptr;

OnlySocket *OnlySocket::getInstance()
{
    if(instance == nullptr)
    {
        instance = new OnlySocket;
    }
    return instance;
}

void OnlySocket::releaseInstance()
{
    if(instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

OnlySocket::OnlySocket()
{
    socket = new QTcpSocket;
    socket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, 268);
}

OnlySocket::OnlySocket(const OnlySocket &other)
{
    Q_UNUSED(other)
}

OnlySocket::~OnlySocket()
{
    delete socket;
}
