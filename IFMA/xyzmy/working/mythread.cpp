//update function: void FinihedTestDraw(QPainter *paint);d drop the max and min,and average the next
//Date:2013-5-7


#include "mythread.h"
#include <QDebug>
#define NUMBER 1200
int SPIStatus;
int dataCount=0;
int receivedata[1200];
//#define REPEATCOUNT 6;
int RepeatCount=4;//repeat for 1000 times,repeat could be changed
int MAXDrdyRepeat=0;
int SerachDrdyNumber=100;//search Drdy of 1 point for so many times,by test,100 is enough
QString MAXDrdyCount;
//QTimer *Drdy_Timer;
MyThread::MyThread()
{
    //Drdy_Timer=new QTimer(this);
    //Drdy_Timer->start(10);
    //connect(Drdy_Timer,SIGNAL(timeout()),this,SLOT(getDrdy()));


}
MyThread::~MyThread()
{
::close(SPIStatus);

}
void MyThread::run()
{
    int count;
    SPIStatus=openSPI();
    dataCount=0;

    AD7707_init();//initiate AD7707
    int i=0;

    int j=0;
    //int MyStatus=1;
    int sentpoint=0;
    while(dataCount<NUMBER)
    {
 //   sentpoint=average();//we get the average
        sentpoint=getOnePoint();
    emit transferData(dataCount,sentpoint);
    dataCount++;

    }
    if(::close(SPIStatus)<0)
        perror("close spi error");
   // qDebug()<<"receive over";

    //send the MAXDrdyRepeat
    emit MAXDrdyRepeatCount(QString::number(MAXDrdyRepeat));
    //qDebug()<<MAXDrdyRepeat;
    emit finishedDataReceived();


}

int MyThread::openSPI()
{

    int fd =::open("/dev/spidev1.0", O_RDWR);
    if (fd < 0) {

        perror("open device spi");

        exit(1);

    }
    return fd;

}
void MyThread::delay(unsigned char second)
{

    int i,j=0;
    while(second--)
    {

        for(i=0;i<33;i++)
        {
            j=1;
        }
    }
}


void MyThread::AD7707_init()
{
    int i=0;
    char buf1[]={0x23};
    int count;
    count=write(SPIStatus,buf1,sizeof(buf1)); //AIN3/HICOM ,next CLOCK Register
    for(i=0;i<10;i++)
    {
        delay(100);
    }

    char buf2[]={0x1B};
    count=write(SPIStatus,buf2,sizeof(buf2)); //MASter Clock,2.4576MHz Clock,input Rate 500Hz
    for(i=0;i<10;i++)
    {
        delay(100);
    }

    char buf3[]={0x13};
    count=write(SPIStatus,buf3,sizeof(buf3)); //AIN3/HICOM ,next SetUp register
    for(i=0;i<10;i++)
    {
        delay(100);
    }

    char buf4[]={0x4E};
    //GAIN=2 Unipolar module,Open BUffer
    count=write(SPIStatus,buf4,sizeof(buf4));

    //usleep(100);
    for(i=0;i<10;i++)
    {
        delay(100);
    }


}

int MyThread::getOnePoint()
{
       int MyStatus=1;

        //detect MyStatus ,if it is 0,then get data,else repeat get drdy for several times
        //while(MyStatus!=0&&j<RepeatCount)
      int i=0;
       int waitDrdyTimes=0;
      int count=0;
        while(MyStatus!=0&&waitDrdyTimes<SerachDrdyNumber)
        {
            char buf6[]={0x0B};//get the DRDY

            count=write(SPIStatus,buf6,sizeof(buf6));

            for(i=0;i<10;i++)
            {
                delay(100);
            }

            char DrdyStatus;
            read(SPIStatus,&DrdyStatus,sizeof(DrdyStatus));
            MyStatus=DrdyStatus;
            MyStatus=MyStatus&0x80;//communction register 's First bit is DRDY

            //FinalStatus=MyStatus>>7;
            //qDebug()<<"in run";
            //printf("DRDy is %0x\n",FinalStatus);
            //emit transferData(FinalStatus,DrdyStatus);
            MAXDrdyCount=QString("%1").arg(MyStatus);
            emit sendDrdy(MAXDrdyCount);
            for(i=0;i<10;i++)
            {
                delay(100);
            }

            waitDrdyTimes++;
        }
        QString temp=QString::number(waitDrdyTimes);
         emit senddata(temp);
         //and send the max waitDrdyTimes to checkshow
         if(waitDrdyTimes>MAXDrdyRepeat)
             MAXDrdyRepeat=waitDrdyTimes;
        // QString temp=QString("%1").arg(MyStatus);
        //emit sendDrdy(temp);
        //Use hardware to control Drdy
         if(MyStatus==0)
         {
            char buf5[]={0x3B};

            count=write(SPIStatus,buf5,sizeof(buf5));

           for(i=0;i<10;i++)
            {
                delay(100);
            }

            char buf[2]={0x00,0x00};

            int readCount=0;
            readCount=read(SPIStatus,buf,2);

            int high=buf[0];
            int low=buf[1];

            //printf("%d,%d\n",high,low);
            int AValue=0;
            AValue=high*256+low;
            //printf("Data:%d\n",AValue);
            receivedata[dataCount]=AValue;
            //qDebug()<<AValue;
           // emit transferData(dataCount,receivedata[dataCount]);
            //dataCount++;

         //  msleep(6);
            return AValue;
}

}


int MyThread::average()//find the max and min,drop max and min,and average the next
{

int i=0;
int point=0;
int max=0;
int min=0;
int SendData=0;
int Total=0;
for(i=0;i<RepeatCount;i++)
{
point=getOnePoint();//get one point
if(point>max)
    max=point;
if(point<min)
    min=point;
Total+=point;

}
SendData=(Total-max-min)/(RepeatCount-2);/*
for(i=0;i<10;i++)
{
    delay(100);
}*/
//msleep(1);
return SendData;

}
