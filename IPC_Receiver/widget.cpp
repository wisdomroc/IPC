#include "widget.h"
#include "ui_widget.h"
#include <QSharedMemory>
#include <QMessageBox>
#include <QBuffer>



const char *keyWord = "wanghuipeng";

Widget::Widget(QWidget *parent) :
    QWidget(parent),sharedMemory(new QSharedMemory(keyWord, this)),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(1000,660);
    setWindowTitle("Receiver");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    if(!sharedMemory->attach())
    {
        QMessageBox::information(this, "warning", "sharedMemory attach failure");
        return;
    }

    QBuffer buffer;
    QDataStream stream(&buffer);
    QPixmap pixmap;


    sharedMemory->lock();
    buffer.setData(static_cast<const char *>(sharedMemory->constData()), sharedMemory->size());
    buffer.open(QIODevice::ReadOnly);
    stream >> pixmap;
    sharedMemory->unlock();
    sharedMemory->detach();
//    pixmap = pixmap.scaledToWidth(ui->label->width());
    ui->label->setPixmap(pixmap);
}
