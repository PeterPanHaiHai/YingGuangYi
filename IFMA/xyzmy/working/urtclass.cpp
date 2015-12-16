#include "urtclass.h"
#include "QMessageBox"

UrtClass::UrtClass(QObject *parent) :
    QObject(parent)
{
    //getTimer=new QTimer(this);
    //QObject::connect(getTimer,SIGNAL(timeout()),this,SLOT(DataIn()));
    //getTimer->setInterval(1000);
    //getTimer->start();

}

void UrtClass::Init_Dev()
{
    tempbuf=new char [10000];
    data=new char[10000];
    tempbuf1=new char[30];
    tempbuf2=new char[30];
    fd=::open("/dev/ttyO1",O_RDWR | O_NOCTTY );

    struct termios options;  // 串口配置结构体
    tcgetattr(fd,&options); //获取当前设置
    bzero(&options,sizeof(options));
    options.c_cflag  |= B9600 | CLOCAL | CREAD; // 设置波特率，本地连接，接收使能
    options.c_cflag &= ~CSIZE; //屏蔽数据位
    options.c_cflag  |= CS8; // 数据位为 8 ，CS7 for 7
    options.c_cflag &= ~CSTOPB; // 一位停止位， 两位停止为 |= CSTOPB
    options.c_cflag &= ~PARENB; // 无校验
    options.c_cc[VMIN]=4;//新增
    options.c_cc[VTIME]=0;
    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &options); // TCSANOW立即生效；

    //    my_notifier=new  QSocketNotifier(fd,QSocketNotifier::Read,this);
    //    QObject::connect(my_notifier,SIGNAL(activated(int)),this,SLOT(DataIn()));

    direction=0;//默认外侧光耦
}

void UrtClass::Send_Dev(char *tempchar)
{
    //int n = ::write(fd,"linux\n", 6);
    int n=::write(fd,tempchar,13);
    ::usleep(200000);
    //下面是输出具体传送的命令
    QString msg;
    for(int i=0;i<13;i++)
    {
        qint8 outChar = tempchar[i];
        QString str = QString("%1").arg(outChar&0xFF, 2, 16, QLatin1Char('0'));
        msg += str;
        msg+= " ";
    }
    //qDebug()<<msg+QString::number(n,10);
    //QMessageBox::about(NULL,"mm",msg);
}

void UrtClass::Close_Dev()
{
    ::close(fd);
}
void UrtClass::clearSeral()
{
    ::tcflush(fd,TCIOFLUSH);
}
int UrtClass::getDirection()
{//0：外； 1：内
    return direction;//默认方向是向外
}

void UrtClass::remoteDataIncoming()
{
    ::usleep(200000);
    int n = ::read(fd,tempbuf,25);
    if(n>10)
    {
        int former1=0,last1=0;
        QString str;
        for(int i=5;i<=6;i++)
        {
            if((tempbuf[i]>=48)&&(tempbuf[i]<=57))
            {
                former1+=((int)tempbuf[i]-48)*(::pow(16,(6-i)));
            }
            else
            {
                former1+=((int)tempbuf[i]-55)*(::pow(16,(6-i)));
            }
        }
        for (int j = 7; j <= 10; j++)
        {
            if ((tempbuf[j] >= 48) && (tempbuf[j] <= 57))
            {
                last1 += (tempbuf[j] - 48) * pow(16, (10 - j));
            }
            else
            {
                last1 += (tempbuf[j] - 55) * pow(16, (10 - j));
            }
        }

        if (last1 >= 10000)
        {
            str = QString::number(former1, 10) + QString::number(last1, 10);
        }
        else
        {
            str = QString::number(former1, 10) + "0" + QString::number(last1, 10);
        }

        QByteArray ba = str.toLatin1();
        tempbuf1= ba.data();
        QMessageBox::about(NULL,"m",QString(QLatin1String(tempbuf1)));
        sendNoti(tempbuf1,1);
    }
}


/*
*函数名：DataIn
*作 用：从串口中取数据
*参 数：mode=1标志是读取SPI数据；mode=0标志读取光耦
*返回值：1--碰到内侧光耦
*       2--碰到内侧光耦
*/
int UrtClass::DataIn(int mode)
{
    int n=::read(fd,tempbuf,5000);
//    QFile file("Result/data.txt");
//    file.open(QIODevice::Append);
//    QTextStream writein(&file);
    if(n>0)
    {//处理得到的数据
        QString msg;
        msg.clear();
        for(int i=0;i<n;i++)
        {
            qint8 outChar = tempbuf[i];
            QString str = QString("%1").arg(outChar&0xFF, 2, 16, QLatin1Char('0'));
            if(mode==1)
            {
                data[i]=tempbuf[i];
//                if(i%2==0 && i>0)
//                    writein<<endl;
//                writein<<str;
            }
            msg += str;
        }
//        file.close();
        if(mode == 0)
        {
            if(msg == "ff0f0fff ")//碰到外侧光耦
            {
                return 0;
            }
            else if(msg.contains("fff0f0ff"))//碰到内侧光耦
            {
                return 1;
            }
        }

        if(n!=1)
            return n;//字节的个数
        else
            return 0;
    }
}
