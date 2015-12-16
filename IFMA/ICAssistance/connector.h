#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QSerialPort>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDebug>

class Connector : public QSerialPort
{
    Q_OBJECT
public:
    explicit Connector();
private:
    char readOp[10]; //读取指令
    char waitOp[2]; //待机指令
    char writeOp[10];//写入指令
    char returnHead[4];//返回头
    QByteArray ba;  //接收缓冲
    int exceptByte; //期望接收字节数
    qint8 start;
    qint8 num;

signals:
    void readAllData();
    void writeOK();
    void readOK(QByteArray readData);

private slots:
    void handleReadyRead();
    void handleReadData();

public slots:
    void callForData(qint8 start, qint8 num);   //申请从start开始的num个扇区的内容
    void callForWait(); //申请待机
    void startRead(qint8 start, qint8 num);    //开始读取从start开始的num个扇区的内容
    void stopRead();    //停止读取
    void startWrite(qint8 start, qint8 num, QByteArray data);   //将data写入从start开始的num个扇区
};

#endif // CONNECTOR_H
