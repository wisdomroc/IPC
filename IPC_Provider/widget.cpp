#include "widget.h"
#include "ui_widget.h"
#include <QSharedMemory>
#include <QFileDialog>
#include <QBuffer>
#include <QMessageBox>
#include <QDebug>

const char *keyWord = "wanghuipeng";

Widget::Widget(QWidget *parent) :
    QWidget(parent),sharedMemory(new QSharedMemory(keyWord, this)),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(1000, 660);
    setWindowTitle("Provider");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    if(sharedMemory->isAttached())
    {
        sharedMemory->detach();
    }

    QString pixmapPath = QFileDialog::getOpenFileName(this, tr("please choose a pixmap"), "", "JPG(*.jpg);;PNG(*.png)");
    QPixmap pixmap(pixmapPath);
    pixmap = pixmap.scaledToWidth(ui->label->width());
    ui->label->setPixmap(pixmap);
    QBuffer buffer;
    QDataStream stream(&buffer);
    buffer.open(QIODevice::ReadWrite);
    stream << pixmap;

    int bufferSize = buffer.size();
    if(!sharedMemory->create(bufferSize))
    {
        QMessageBox::information(this, "warning", "sharedMemory create failure");
        qDebug() << sharedMemory->errorString();
        return;
    }
    else
    {
        sharedMemory->lock();
        char *to = static_cast<char *>(sharedMemory->data());
        const char *from = buffer.data().constData();
        memcpy(to, from, qMin(bufferSize, sharedMemory->size()));
        sharedMemory->unlock();
    }
}
