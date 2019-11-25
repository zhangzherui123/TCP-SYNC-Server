#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "filebase.h"
#include <qdebug.h>
#include <QFileInfo>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void onFile(const QString& filename, const QByteArray& data);
    void onDel(const QString& filename);
    void onRename(const QString& filename1, const QString& filename2);

private:
    Ui::Widget *ui;
    FileBase* filebase;
    QString base;

};

#endif // WIDGET_H
