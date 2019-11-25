#ifndef FILEBASE_H
#define FILEBASE_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QDataStream>
#include <QDateTime>

enum MsgType
{
    MsgTypeInvalid = 0,
    MsgTypeFile,
    MsgTypeDel,
    MsgTypeRename
};


class FileBase : public QObject
{
    Q_OBJECT
public:
    explicit FileBase(QObject *parent = nullptr);

signals:
    void doFile(const QString& filename, const QByteArray& data);
    void doDel(const QString& filename);
    void doRename(const QString& filename1, const QString& filename2);
public slots:
    // 新客户端请求处理函数
    void onNewConnection();
    // socket相关
    void onConnected();
    void onDisConnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadReady();

private:
    QTcpServer* server;
    QList<QTcpSocket*> clients;
    quint32 LastsizePack = 0;
};

#endif // FILEBASE_H
