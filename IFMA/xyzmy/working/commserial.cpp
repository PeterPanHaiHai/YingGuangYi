#include "commserial.h"

CommSerial::CommSerial()
{
    connectionStatus = Offline;
}

CommSerial::CommSerial(QSerialPort *port)
{
    connectionStatus = Offline;
    this->port = port;

    this->delimiter = CRLF;
    this->SetTimeout(100, 3000);
    this->checkArea = STX_TO_ETX;
    this->checkByteUse = true;
    this->is2CheckBytelen = true;
}

CommSerial::~CommSerial()
{

}

/*******************************************
 * 功  能：发送和接收超时时间设定
 * 函数名：SetTimeout()
 * 参  数:initTimeout    等待时间,直到接收到
 *        byteInterval  发送接收等待的时间
 * 返回值: 无
 * *******************************************/
void CommSerial::SetTimeout(qint32 initTimeout, qint32 byteInterval)
{
    this->initTimeoutForReceive = initTimeout;
    this->byteIntervalForReceive = byteInterval;
    this->ReadTimeOUt = byteInterval;
    this->WriteTimeOut = byteInterval;
}

/*******************************************
 * 功  能：文本传输
 * 函数名：SendText
 * 参  数:要发送的文本
 * 返回值: >1 发送的字符数
 *        0  未连接
 *        -1 发送异常
 *        -2 ENQ和ENQ撞到
 * *******************************************/
qint32 CommSerial::SendText(QString data)
{
    const qint32 OPENSESSION_CROSS = -1;
    const qint32 OPENSESSION_FAILURE = 0;
    const qint32 OPENSESSION_SUCCESS = 1;

    qint32 sendTextLen = 0;
    qint32 sendSuccess = 0;


    bool trySend = true;
    bool waitFromHostCommandEnd = false;
    while(trySend)
    {
        trySend = false;

        commMutex.lock();

        qint32 openSessionResult = this->OpenSession();  //先进行握手连接

        switch (openSessionResult) {
        case OPENSESSION_CROSS:
            //ENQ和ENQ撞到的话
            waitFromHostCommandEnd = true;
            trySend = true;
            sendSuccess = -2;
            break;

        case OPENSESSION_FAILURE:
            sendSuccess = -1;
            break;

        case OPENSESSION_SUCCESS:
                    qDebug() << "In commserial.cpp line82. Opensession success.";
            //握手连接成功，发送数据
            sendTextLen = this->Send(data);
            if(sendTextLen == 0 || sendTextLen == -1)
            {
                //握手发送数据出错
                qDebug() << "Wrong to send data in Commserial::SendText(QSting data) function!!";
            }
            else
            {
                //若数据发送成功，关闭对话
                if(this->CloseSession())
                {
                    qDebug() << "Success to send data and close session.";
                }
                sendSuccess = sendTextLen;
            }
            break;
        default:
            break;
        }// end switch

        commMutex.unlock();

        if(waitFromHostCommandEnd)
        {
            //暂停3500ms
            this->Sleep(3500);
        }
    }//end while


    return sendSuccess;
}


/*******************************************
 * 功  能：文本接收
 * 函数名：RecvText()
 * 参  数:接收的文本
 * 返回值: >1 接收的字符数
 *        -1 接收异常
 *        -2 线程终止
 *        -3 参数异常
 * *******************************************/
qint32 CommSerial::RecvText(QString &data)
{
    qDebug() << "In commserial.cpp line 129 RecvText() functiton";
    qint32 receiveSuccess = 0;
    data.clear();

    commMutex.lock();
    qint32 ret = ReceiveForHost(data);
    commMutex.unlock();

    if(ret != 0)
    {
        receiveSuccess = ret;
    }

    return receiveSuccess;
}

/*******************************************
 * 功  能：文本接收（对于主机）
 * 函数名：ReceiveForHost()
 * 参  数: 接收的文本
 * 返回值: 接收的字符数
 *        0 失败
 * *******************************************/
qint32 CommSerial::ReceiveForHost(QString &text)
{
    qDebug() << "In commserial.cpp line 154 ReceiveForHost() function.";
    qint32 len = 0;
    len = ReceiveOnHandshakeForHost(text);
    return len;
}

/*******************************************
 * 功  能：握手文本接收(对于主机)
 * 函数名：ReceiveOnHandshakeForHost()
 * 参  数:接收的的文本
 * 返回值: 接收的字符数
 *        0 失败
 * *******************************************/
qint32 CommSerial::ReceiveOnHandshakeForHost(QString &text)
{
    qDebug() << "In commserial.cpp line 169 ReceiveOnHandshakeForHost() function.";
    qint32 sum = 0;  //计检验
    qint32 comp = 0; //接收到的校验码
    qint32 check_counter = 0; //检验计数器
    qint32 data_counter = 0;  //数据计数器
    bool check_flag = false;  //校验计数结束标志
    bool data_ok = false; //是否正确接收到数据标志
    bool recv_enq = false; //是否接收到ENQ标志
    QByteArray checkbyte;
    checkbyte.clear();
    char *c = new char[1];
    QByteArray databuf;  //数据缓冲区
    databuf.clear();
    bool need_to_change = true;

    char ack[2];
    ack[0] = (char)CommSerial::ACK;
    ack[1] = '\0';
    char nak[2];
    nak[0] = (char)CommSerial::NAK;
    nak[1] = '\0';

    text = "";

    qint32 checkLen;
    if(!this->is2CheckBytelen)
    {
        checkLen = 4;
    }
    else
    {
        checkLen = 2;
    }

    while(true)
    {
        //qDebug() << "In commserial.cpp line 205 first while function.";
        if(need_to_change)
        {
            if(!recv_enq)
            {
                this->ReadTimeOUt = initTimeoutForReceive;
            }
            else
            {
                this->ReadTimeOUt = byteIntervalForReceive;
                need_to_change = false;
            }
        }

        while(true)
        {
             //qDebug() << "In commserial.cpp line 221 second while function.";
            //等待数据 超过3S返回0
            if(!this->ReadChar(c))
            {
                //读取失败
                return 0;
            }
            else
            {
                break;
            }
        }

        //控制码的检查
        switch (*c) {

            case (char)ENQ:
                qDebug() << "Recieve ENQ..(in commserial.cpp line238)";
                if(!recv_enq)
                {
                    //接收到ENQ返回ACK
                    port->write(ack);
                    recv_enq = true;
                }
                break;

            case (char)STX:
                qDebug() << "Recieve STX..(in commserial.cpp line248)";
                check_flag = false;
                check_counter = 0;
                data_counter = 0;
                if(this->checkArea == STX_TO_ETX)
                {
                    sum = (qint32)STX;
                }
                else
                {
                    sum = 0;
                }
                break;

            case (char)ETX:
                qDebug() << "Recieve ETX..(in commserial.cpp line263)";
                if(checkLen == 2)
                {
                    sum ^= (qint32)ETX;
                }
                else
                {
                    sum += (qint32)ETX;
                }

                if(this->checkByteUse)
                {
                    check_flag = true;
                }
                else
                {
                    port->write(ack);

                    text = QString(databuf);

                    data_ok = true;
                }
                break;

            case (char)EOT:
                qDebug() << "Recieve EOT..(in commserial.cpp line288)";
                //数据的终结
                //如果还没有收到ENQ，不成立
                if(data_ok && recv_enq)
                {
                    return data_counter;
                }
                return 0;

            default:
                if(check_flag) //ETX接收后
                {
                    text = QString(databuf);
                    qDebug() << "(In commserial.cpp line301)text is: " << text;

                    //并设置校验字节码
                    checkbyte.append(*c);
                    check_counter++;
                    //如果为2字节校验
                    if(check_counter == checkLen)
                    {
                        check_flag = false;

                        QString CheckByteString;
                        for (int var = 0; var < 2; ++var) {
                            qint8 outChar = checkbyte.at(var);
                            QString str = QString("%1").arg(outChar&&0xFF, 2, 16, QLatin1Char('0'));
                            CheckByteString += str;
                        }

                        qDebug() << "CheckByteString is: " << CheckByteString;

                        bool ok;
                        comp = CheckByteString.toInt(&ok, 16);  //接收到的校验码

                        qDebug() << "(In commserial.cpp line323) comp is: " << comp;

                        qint32 mask;
                        if(this->is2CheckBytelen)
                        {
                            mask = 0xff;
                            sum = sum & mask;  //自己算的校验码

                            qDebug() << "(In commserial.cpp line331) sum is: " << sum;
                        }
                        else
                        {
                            mask = 0xffff;
                            sum = sum & mask;
                        }

                        qDebug() << "(In commserial.cpp line339) sum is: " << sum;

                        //校验码比较
                        if(sum == comp)
                        {
                            //匹配成功，则返回ACK
                            port->write(ack);

                            text = QString(databuf);

                            data_ok = true;
                            continue;
                        }
                        else
                        {
                            qDebug() << "(In commserial.cpp line354) ";
                            //发送NAK重试
                            port->write(nak);
                            check_counter = 0;
                            data_counter = 0;
                            continue;
                        }
                    }
                }
                else    //STX接收后,ETX接收前
                {
                    if(*c >= 0x20)
                    {
                        if(checkLen == 2)
                        {
                            sum ^= (qint32)(*c);
                        }
                        else
                        {
                            sum += (qint32)(*c);
                        }
                        //databuf[data_counter++] = readData.at(0);
                        databuf.append(*c);
                        data_counter++;
                    }
                }
                break;
        }// end switch

        //如果数据数量超过最大允许范围，则返回失败
        if(data_counter >= MAX_BLOCKSIZE)
        {
            return 0;
        }
    }
}

/*******************************************
 * 功  能：从串口读取一个字符数据
 * 函数名：ReadChar()
 * 参  数:读到的字符数
 * 返回值: true: 读取成功
 *       false: 读取失败
 * *******************************************/
bool CommSerial::ReadChar(char *data)
{
    *data = ' ';
    qint32 timeOut = 3000; //ms
    qint32 timeOutDiv = 10;
    qint32 waitSum = 0;

    while(this->port->bytesAvailable() == 0)
    {
        this->Sleep(timeOutDiv);

        waitSum += timeOutDiv;
        if(waitSum >= timeOut)
        {
            return false;  //超过3秒
        }
    }

    this->port->read(data, 1);  //读取一个字符
    //qDebug() << "(In commerserial.cpp line417)char is: " << *data;

    return true;
}

/*******************************************
 * 功  能：实现以毫秒为单位的延时
 * 函数名：Sleep();
 * 参  数: 毫秒
 * 返回值: 无
 * *******************************************/
void CommSerial::Sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/*******************************************
 * 功  能：向主机请求数据连接
 * 函数名：OpenSession()
 * 参  数:无
 * 返回值:1  成功
 *       0  失败
 *       -1 ENQ和ENQ撞到的话
 * *******************************************/
qint32 CommSerial::OpenSession()
{
    qint32 retry = 1;
    char* r_c = new char[1];
    char enq[2];
    enq[0] = (char)ENQ;
    enq[1] = '\0';

    this->ReadTimeOUt = byteIntervalForReceive;

    while(true)
    {
        //发送ENQ
        if(!port->write(enq))
        {
            return 0;
        }

        while(true)
        {
            //等待响应(ACK)
            if(!ReadChar(r_c))
            {
                break;
            }
            else
            {
                if(*r_c == (char)ENQ)
                {
                    return -1;
                }
                else if(*r_c != (char)ACK)
                {
                    break;
                }
                else
                {
                    return 1;
                }
            }
        }

        if(++retry > RETRY_TIMES)
        {
            return 0;
        }
    }
}

/*******************************************
 * 功  能：断开数据连接
 * 函数名：CloseSession()
 * 参  数:无
 * 返回值:true   成功
 *       false  失败
 * *******************************************/
bool CommSerial::CloseSession()
{
    char eot[2];
    eot[0] = (char)EOT;
    eot[1] = '\0';
    if(!port->write(eot))
    {
        return false;
    }
    return true;
}

/*******************************************
 * 功  能：文本传输
 * 函数名：Send
 * 参  数:要发送的文本
 * 返回值: 发送的字符数
 *        -1/0 失败
 * *******************************************/
qint32 CommSerial::Send(QString text)
{
    qint32 lenToSend = 0;
    qint32 len = 0;
    qint32 block;  //第几块
    QString buf;

    lenToSend = text.length();
    for(block=0; ; block++)
    {
        if(lenToSend <= BLOCK_LEN)
        {
            buf = text.mid(block*BLOCK_LEN, lenToSend);
        }
        else
        {
            buf = text.mid(block*BLOCK_LEN, BLOCK_LEN);
        }

        len = SendOnHandshake(buf);

        if(len == 0)
        {
            return len;
        }

        if(lenToSend <= BLOCK_LEN)
        {
            break;
        }
        lenToSend -= BLOCK_LEN;
    }

    return len;
}

/*******************************************
 * 功  能：握手文本传输
 * 函数名：SendOnHandshake
 * 参  数:要发送的文本
 * 返回值: 发送的字符数
 *        -1 失败
 * *******************************************/
qint32 CommSerial::SendOnHandshake(QString text)
{
    qint32 len = 0;
    qint32 retry = 1;
    qint32 sum;  //检验和
    char* r_c = new char[1];
//    char[] check_byte = new char[SIZE_CHECK_BYTE+1];
    QString w_str;
    QByteArray check_byte;
    check_byte.clear();
    QByteArray c_Text = text.toAscii();

    char stx[2];
    stx[0] = (char)STX;
    stx[1] = '\0';
    char etx[2];
    etx[0] = (char)ETX;
    etx[1] = '\0';

    this->ReadTimeOUt = this->byteIntervalForReceive;

    while(true)
    {
        if(this->checkArea == STX_TO_ETX)
        {
            //sum = (qint32)URs232c::STX + (qint32)URs232c::ETX;
            sum = (qint32)STX;
            sum ^= (qint32)ETX;
        }
        else
        {
            sum = (qint32)ETX;
        }

        //发送STX
        if(!port->write(stx))
        {
            return -1;
        }

        //发送数据文本
        if(!port->write(c_Text))
        {
            return -1;
        }

        //计算校验和
        len = c_Text.length();
        for(int i=0; i<len; i++)
        {
            if(this->is2CheckBytelen)
            {
                sum ^= (qint32)c_Text[i];
            }
            else
            {
                sum += (qint32)c_Text[i];
            }
        }

        //发送ETX
        if(!port->write(etx))
        {
            return -1;
        }

        if(!this->is2CheckBytelen)
        {
            w_str = QString("%1").arg(sum&0xFFFF, 4, 16, QLatin1Char('0'));
            check_byte = w_str.toAscii();
            qDebug()<<"(In urscom.cpp line248) check_byte is: "<<check_byte;
        }
        else
        {
            w_str = QString("%1").arg(sum&0xFF, 2, 16, QLatin1Char('0'));
            check_byte = w_str.toAscii();
            qDebug()<<"(In commserial.cpp line637) check_byte is: "<<check_byte;
        }

        //发送计算出来的校验和
        if(this->checkByteUse)
        {
            port->write(check_byte);
        }

        //等待响应(ACK)
        while(true)
        {
            if(!ReadChar(r_c))
            {
                break;
            }
            else
            {
                if(*r_c != (char)ACK)
                {
                    //跳出while循环重试
                    break;
                }
                else
                {
                    //收到的是ACK，则返回发送文本数据长度
                    return len;
                }
            }
        }

        //如果没有返回ACK，则重试
        if(++retry > RETRY_TIMES)
        {
            return -1;
        }
        else
        {
            continue;
        }
    }
}
