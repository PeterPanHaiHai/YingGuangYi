#include "connector.h"

Connector::Connector()
{
    //readOp = {0x09, 0xAC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x02};
    readOp[0] = 0x09;   // 数据总数
    readOp[1] = 0xAC;   // 读数据
    readOp[2] = 0xFF;   // 密码
    readOp[3] = 0xFF;
    readOp[4] = 0xFF;
    readOp[5] = 0xFF;
    readOp[6] = 0xFF;
    readOp[7] = 0xFF;
    readOp[8] = 0x01;   // 起始扇区
    readOp[9] = 0x02;   // 扇区数（最多5）
    //waitOp = {0x01, 0xAD};
    waitOp[0] = 0x01;   // 数据总数
    waitOp[1] = 0xAD;   // 关射频
    //writeOp = {0x09, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x02};
    writeOp[0] = 0x09;  // 数据总数
    writeOp[1] = 0xAB;  // 写数据
    writeOp[2] = 0xFF;  // 密码
    writeOp[3] = 0xFF;
    writeOp[4] = 0xFF;
    writeOp[5] = 0xFF;
    writeOp[6] = 0xFF;
    writeOp[7] = 0xFF;
    writeOp[8] = 0x01;  // 起始扇区
    writeOp[9] = 0x02;  // 扇区数
    //后面跟数据，最长240byte
    //读取数据的返回头
    returnHead[0] = 0x5A;
    returnHead[1] = 0x59;
    returnHead[2] = 0x48;
    returnHead[3] = 0x02;
}

void Connector::callForData(qint8 start, qint8 num)
{
    QByteArray data(readOp, 10);
    data[8] = start;
    data[9] = num;
    write(data);
    waitForBytesWritten(200);
}

void Connector::callForWait()
{
    QByteArray data(waitOp, 2);
    write(data);
    waitForBytesWritten(200);
}

void Connector::startRead(qint8 start, qint8 num)
{
    exceptByte = 5 + 48 * num;  // 数据头+数据
    qDebug()<<QString("start = %1, num = %2").arg(start).arg(num);
    stopRead();
    connect(this, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(this, SIGNAL(readAllData()), this, SLOT(handleReadData()));
    this->start = start;
    this->num = num;
    callForData(start, num);
}

void Connector::stopRead()
{
    disconnect(this, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    disconnect(this, SIGNAL(readAllData()), this, SLOT(handleReadData()));
    //清空
    ba.clear();
    clear();
}

void Connector::handleReadyRead()
{
    ba.append(readAll());
    if (ba.size() >= exceptByte) {
        emit readAllData();
    }
}

void Connector::handleReadData()
{
    int dataStart = ba.indexOf(QByteArray(returnHead, 4), 0);
    if(dataStart != -1) {
        // 找到返回头
        // 下一个是数据长度
        if(dataStart + 4 >= ba.size()) {
            return; // 数据长度不够
        }
        int dataLen = ba[dataStart + 4];
        qDebug()<<dataStart + 5 + dataLen<<" "<<ba.size();
        if(dataStart + 5 + dataLen > ba.size()) {
            return; // 数据长度不够
        } else {
            emit readOK(ba.mid(dataStart + 6, dataLen));
            //结束
            stopRead();
        }
    } else {
        callForData(start, num);
    }
}

void Connector::startWrite(qint8 start, qint8 num, QByteArray data)
{
    QByteArray toWrite(writeOp, 10);
    toWrite[0] = toWrite[0] + data.size();
    toWrite[8] = start;
    toWrite[9] = num;
    toWrite.append(data);
    write(toWrite);
    waitForBytesWritten(500);
    emit writeOK();
}
