#ifndef STRIPS_H
#define STRIPS_H
#include <stdio.h>
#include <malloc.h>
#include "gradient.h"

/*

两条带和六条带的区别主要是：
1:检测速度PWM不同－－－－－》检测区域不一致
2：finished_test算法不一样－－－－－－－》最后处理算法不一样
所以可以编写一个类，该类含有检测带条数，检测带pwm，检测处理算法

对于不同条带处理，则对该类赋不同值

  */
//该C＋＋类用于判断当前是几条带检测机器
class strips
{
public:
    strips();
    ~strips();
    int SetNumber(int setnumber)
    {
        number=setnumber;
        return number;
    }
    int GetNumber(QString fileName);

    int SetPwm(int setpwm)
    {
        pwm=setpwm;
        return pwm;
    }
    int GetPwm()
    {

        return pwm;
    }
    //int Curves_Algorithm(int number=2);//algorithm to calicuate the curves
    int Two_Curves_Algorithm();
    int *Six_Curves_Algorithm(double *);
    double *Six_Curves_Height(double *);
    int *findposition(double *);
    int findlargest(double *data,int begin, int end);
private:
    int number;//how many strips
    int pwm;//speed of stepper
    gradient *Gradient;
    //int Curves_Algorithm;
};

#endif // STRIPS_H
