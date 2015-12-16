#include "positioncalibrate.h"
#include <QFile>
#include <QTextStream>
PositionCalibrate::PositionCalibrate()
{
}

int PositionCalibrate::OffsetLength(int position)
{
    //分析数据，并找出当前波峰中点距离真正的中点差距多少
    const int TOTAL=1200;//1200 个点
    printf("offset point is %d\n",position-TOTAL/2);
    if((position-TOTAL/2<0)||(position>TOTAL))
        return 0;
    else
    return position-TOTAL/2;
}

int PositionCalibrate::offsetTime(int length)
{
    //偏移这些距离需要多少个点
    //采集一个点大概需要3955us，采集1200个点大概4.75s
    //那么总共偏移length个位置，对应需要的时间为Ys
    // 1200.t/1200=Y/length;Y=length*t;
    //Y=length*3955us;
    int time=(float(length)*3955);//us
    printf("offset time is%d\n",time);
    if(length>600)
        return 0;
    SaveTimeTofile(time);
    return time;//ms
}

void PositionCalibrate::SaveTimeTofile(int time)
{
    QString fileName="DelayTime.txt";
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly)<0)
        printf("open error!\n");
    QTextStream in(&file);
    in<<time;
    file.close();
}
