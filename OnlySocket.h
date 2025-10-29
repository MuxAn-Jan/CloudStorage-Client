#ifndef ONLYSOCKET_H
#define ONLYSOCKET_H

#include <QTcpSocket>

//单例模式 保存全局唯一的socket对象和用户的id
class OnlySocket
{
public:
    static OnlySocket* getInstance();
    static void releaseInstance();
    inline QTcpSocket* getSocket(){return socket;}
    //设置和获得id
    inline void setId(const char id[]){strcpy(this->id, id);}
    inline void getId(char id[]){strcpy(id, this->id);}
private:
    OnlySocket();
    OnlySocket(const OnlySocket& other);
    ~OnlySocket();
    
private:
    static OnlySocket* instance;
    QTcpSocket* socket;//全局唯一的socket
    char id[20];//用户的id
};

#endif // ONLYSOCKET_H
