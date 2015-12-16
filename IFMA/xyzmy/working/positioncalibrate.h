#ifndef POSITIONCALIBRATE_H
#define POSITIONCALIBRATE_H
#include<stdio.h>
//在自检功能界面中，首先进行一次测试，并获取到一次检测结果，对这个检测结果作一次分析判断，计算它，
//分析当前检测结果偏移多少？记录之
//找出这个偏移量，在检测界面中，通过sleep(N)函数来进行校正，找出偏移量和N的关系


class PositionCalibrate
{
public:
    PositionCalibrate();

    int OffsetLength(int);
    int offsetTime(int);
    void SaveTimeTofile(int);
};

#endif // POSITIONCALIBRATE_H
