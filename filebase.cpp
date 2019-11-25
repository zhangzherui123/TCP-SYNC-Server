#include "filebase.h"


FileBase::FileBase(QObject *parent) : QObject(parent)
{
    server = new QTcpServer();
    LastsizePack = 0;
    if(server->listen(QHostAddress("192.168.81.1"),6000))
    {
        qDebug() << "监听成功";
    }
    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}


// 有新客户端的连接的时候
void FileBase::onNewConnection()
{
    // 为新用户分配一个socket, 用socket通信
    qDebug() << "有客户端请求连接.";
    QTcpSocket* socket = server->nextPendingConnection();
    clients.append(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisConnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}


// 连接成功
void FileBase::onConnected()
{
    qDebug() << "客户端连接成功.";
}
// 连接断开
void FileBase::onDisConnected()
{
    QObject* obj = this->sender();
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(obj);
    clients.removeAll(socket);  // 先从socket池中删除掉
    // 然后释放掉
//    socket->deleteLater();
    if(socket == 0)
        return;
    socket->close();
    qDebug() << "有客户端连接断开.";
}
// 发生错误
void FileBase::onError(QAbstractSocket::SocketError socketError)
{
    QObject* obj = this->sender();
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(obj);
    clients.removeAll(socket);
    // 然后释放掉
    socket->deleteLater();
    qDebug() << "有客户端连接发生错误.";
}

void FileBase::onReadReady()
{

    // 可以得到是谁触发了这个槽函数
    QObject* obj = this->sender();
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(obj);
    qint64 sizeNow = 0;
    do
    {
        sizeNow = socket->bytesAvailable();  // 这个函数可以获取当前数据包的大小
        QDataStream stream(socket);
        if(LastsizePack == 0)
        {
            if(sizeNow < sizeof(quint32))
            {
                // 如果还没有包头长的话直接返回
                return;
            }

            stream >> LastsizePack;  // 当前包的大小, 注意这里不一定等于缓存区中数据的大小

        }



        if(sizeNow >= LastsizePack - 4)  // 可以读了
        {
            LastsizePack = 0;
            quint32 msg;
            stream >> msg;
            sizeNow = socket->bytesAvailable();

            switch (msg)
            {
                case MsgTypeFile:
                {
                    QString filename;
                    QByteArray datafilecontent;
                    stream >> filename >> datafilecontent;
                    emit doFile(filename, datafilecontent);
                    break;
                }
                case MsgTypeDel:
                {
                    QString filedel;
                    stream >> filedel;
                    emit doDel(filedel);
                    break;
                }
                case MsgTypeRename:
                {
                    QString file1;
                    QString file2;
                    stream >> file1 >> file2;
                    emit doRename(file1, file2);
                    break;
                }

                default:
                    break;
            }
        }
        else
            return;

    }
    while(sizeNow > 0);
}

