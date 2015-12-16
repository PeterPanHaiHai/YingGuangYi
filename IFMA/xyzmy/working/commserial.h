#ifndef COMMSERIAL_H
#define COMMSERIAL_H

#include <QObject>
#include <QMutex>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QtSerialPort/qserialport.h>

#define RETRY_TIMES 6
#define SIZE_CHECK_BYTE 4
#define MAX_BLOCKSIZE 15360
#define BLOCK_LEN 1024

class CommSerial
{
public:

    //连接状态
    enum ConnectionStatus
    {
        Offline,
        Online
    };

    //控制字符定义
    enum  RsChar
    {   SOH = 0x01,
        STX = 0x02,
        ETX = 0x03,
        EOT = 0x04,
        ENQ = 0x05,
        ACK = 0x06,
        NAK = 0x15,
        CR = 0x0d,
        LF = 0x0a
    };

    enum Delimiter
    {
        NONE,
        Cr,
        CRLF  // CR/LF
    };

    //检验区
    enum CheckArea
    {
        STX_TO_ETX,  //从STX到ETX计算
        TO_ETX       //STX后面到ETX计算
    };

    CommSerial();
    CommSerial(QSerialPort *port);
    ~CommSerial();

    QSerialPort* port;
    ConnectionStatus connectionStatus;
    QMutex commMutex;

    int WriteTimeOut;
    int ReadTimeOUt;
    qint32 initTimeoutForReceive;
    qint32 byteIntervalForReceive;
    Delimiter delimiter;
    CheckArea checkArea;
    bool checkByteUse;
    bool is2CheckBytelen;

    qint32 SendText(QString data);
    qint32 RecvText(QString &data);
    qint32 ReceiveForHost(QString &text);
    qint32 ReceiveOnHandshakeForHost(QString &text);
    bool ReadChar(char *data);
    void SetTimeout(qint32 initTimeout, qint32 byteInterval);
    void Sleep(unsigned int msec);
    qint32 OpenSession();
    bool CloseSession();
    qint32 Send(QString text);
    qint32 SendOnHandshake(QString text);
};

#endif // COMMSERIAL_H
