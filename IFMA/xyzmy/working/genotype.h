#ifndef GENOTYPE_H
#define GENOTYPE_H
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
typedef struct peakpoint
{
    float data;//波峰点的值
    int position;//波峰点的位置
}PeakPoint;

class Genotype
{
public:
    Genotype();
    char *MainFunction(double *);
    bool JudgeControl(double);//判断Control峰是否大于200
    bool JudgeHbsAg(double);//判断HbsAg是否大于200
    char *StartPartion(PeakPoint *);//若以上两条件满足，则进行分型
    PeakPoint *JudgePeakPosition(double *,int);//判断主峰是否满足>100,以及次峰和第三高峰的位置
    //char *FindFirstPeak(int *,int);//寻找主峰
    bool JudgeType_A(PeakPoint*);
    bool JudgeType_B(PeakPoint *);
    int JudgeType_C(PeakPoint *);
    int JudgeType_D(PeakPoint *);
    int ReturnType();//返回类型

};

#endif // GENOTYPE_H
