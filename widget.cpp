#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    filebase = new FileBase();
    connect(filebase, SIGNAL(doFile(QString,QByteArray)),
            this, SLOT(onFile(QString,QByteArray)));
    connect(filebase, SIGNAL(doDel(QString)),
            this, SLOT(onDel(QString)));
    connect(filebase, SIGNAL(doRename(QString,QString)),
            this, SLOT(onRename(QString,QString)));
    base = "C:/Users/Administrator/Desktop/base/";
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onFile(const QString &filename, const QByteArray &data)
{
    QFileInfo fileInfo(filename);
    QString LocalFileName = base + fileInfo.fileName();
    QFile file(LocalFileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        // 打开失败
        qDebug()  << LocalFileName << " 打开失败.";
        return;
    }
    file.write(data);
    file.close();
}

void Widget::onDel(const QString &filename)
{
    QFileInfo fileInfo(filename);
    QString LocalFileName = base + fileInfo.fileName();
    QFile::remove(LocalFileName);
    //qDebug() << filename << " 文件已经被删除.";
}

void Widget::onRename(const QString &filename1, const QString &filename2)
{
    QFileInfo fileInfo1(filename1);
    QString LocalFileName1 = base + fileInfo1.fileName();

    QFileInfo fileInfo2(filename2);
    QString LocalFileName2 = base + fileInfo2.fileName();

    QFile::rename(LocalFileName1, LocalFileName2);
//    qDebug() << "文件 " << filename1 << "被更名为." << filename2;
}


