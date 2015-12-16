#include "genotype.h"

Genotype::Genotype()
{
}
char *Genotype::MainFunction(double *peaks)
{
    bool Control=JudgeControl(peaks[5]);
    if(Control)
    {
        bool HbsAg=JudgeHbsAg(peaks[4]);
        if(HbsAg)
        {
            PeakPoint *Max=JudgePeakPosition(peaks,6);
            if(Max[0].position!=-1)
            {
                char *type=StartPartion(Max);
                return type;
            }
            else
            {
                char *type=new char(50);
                strcpy(type,"UnCalissified");
                return type;
            }
        }
        else
        {
            char *type=new char(50);
            strcpy(type,"Low(-)");
            return type;
        }
    }
    else
    {
        char *type=new char(50);
        strcpy(type,"Error");
        return type;
    }
}
bool Genotype::JudgeControl(double Control)
{
    if(Control<200)
        return false;
    else
        return true;
}

bool Genotype::JudgeHbsAg(double HBSag)
{
    if(HBSag<200.0)
        return false;
    else
        return true;
}
PeakPoint *Genotype::JudgePeakPosition(double *Peaks,int length)
{
    PeakPoint *MaxPeak=(PeakPoint *)malloc(sizeof(PeakPoint)*(length-2));
    PeakPoint error;
    error.data=-1;
    error.position=-1;
    for(int i=0;i<length-2;i++)
    {
        MaxPeak[i].data=Peaks[i];
        MaxPeak[i].position=i;
    }
    for(int i=0;i<length-2;i++)//最后两个波峰是Control和HBsag不参与运算
    {
        for(int j=0;j<i;j++)
        {
            if(MaxPeak[i].data>MaxPeak[j].data)
            {
                int temp=MaxPeak[j].data;
                int TempPosition=MaxPeak[j].position;
                MaxPeak[j].data=MaxPeak[i].data;
                MaxPeak[j].position=MaxPeak[i].position;
                MaxPeak[i].data=temp;
                MaxPeak[i].position=TempPosition;

            }
        }
    }
    //将4个波峰点的数据进行排序从大到小排序，从中取出前三个，并保存其对应的位置，返回，如果最高值《100，则返回错误
    if(MaxPeak[0].data<100)//最大值和100比较
        MaxPeak[0].position=-1;
    return MaxPeak;
}

char *Genotype::StartPartion(PeakPoint *Peak)
{
    //从JudgePeakPosition中找到前三高波峰位置，开始分型判断
    //需要分型的波峰是0-3，起对应从右到左分别是A，B，C，D
    //做个简单区分判断
    //A==3,B==2,C==1,D==0
    char *type=(char *)malloc(sizeof(char)*50);
    memset(type,0,50);
    if(3==Peak[0].position)//A 型
    {
        strcpy(type,"A");//A型
        bool matchA=JudgeType_A(Peak);
        if(matchA)
            return type;
        else
        {
            strcpy(type,"UnClassified");//A型
            return type;
        }
    }
    if(2==Peak[0].position)
    {
        strcpy(type,"B");//B型
        bool matchB=JudgeType_B(Peak);
        if(matchB)
            return type;
        else
        {
            strcpy(type,"UnClassified");//A型
            return type;
        }
    }
    if(1==Peak[0].position)
    {
        strcpy(type,"C");//C型
        int matchC=JudgeType_C(Peak);
        if(1==matchC)
            return type;
        if(2==matchC)
        {
            strcpy(type,"D");//C型
            return type;
        }
        if(-1==matchC)
        {
            strcpy(type,"UnClassified");//C型
            return type;
        }
    }
    if(0==Peak[0].position)//D
    {
        strcpy(type,"D");//C型
        int matchC=JudgeType_D(Peak);
        if(0==matchC)
            return type;
        else
            if(-1==matchC)
            {
            strcpy(type,"UnClassified");//C型
            return type;
        }
        else
            if(3==matchC)
            {
            strcpy(type,"A");//C型
            return type;
        }
    }
}

bool Genotype::JudgeType_A(PeakPoint *Peaks)
{
    if(0!=Peaks[1].position)//次峰类型不是D
    {
        if(0==Peaks[1].data)//数值为0
            return true;
        else
            if(((Peaks[0].data)/(Peaks[1].data))>2)
                return true;
        else
            return false;
    }
    else
    {
        if(0==Peaks[2].data)//1st/3nd
            return true;
        else
            if((Peaks[0].data)/(Peaks[2].data)>2)
                return true;
        else
            return false;

    }
}

bool Genotype::JudgeType_B(PeakPoint *Peaks)
{
    if(0==Peaks[1].data)//次峰数值是0的话
        return true;
    else
        if(((Peaks[0].data)/(Peaks[1].data))>2)
            return true;
    else
        return false;
}

int Genotype::JudgeType_C(PeakPoint *Peaks)
{
    if(0!=Peaks[1].position)//次峰类型不是D
    {
        if(0==Peaks[1].data)
            return 1;//C
        else
            if(((Peaks[0].data)/(Peaks[1].data))>2)
                return 1;//C
        else
            return -1;//UN
    }
    else//次峰类型是D
    {
        if(0==Peaks[1].data)
            return 1;//C
        if(((Peaks[0].data)/(Peaks[1].data))>5)
            return 1;//C
        else
            return 2;//D
    }
}

int Genotype::JudgeType_D(PeakPoint *Peaks)
{
    if(1!=Peaks[1].position)//次峰类型不是C
    {
        //判断次峰是否A,如果是A，主峰比次峰>2，D,否则为A
        if(0==Peaks[1].data)
            return 0;
        else
            if(3==Peaks[1].position)
            {
            if(((Peaks[0].data)/(Peaks[1].data))>2)
                return 0;
            else
                return 3;//返回A
        }
    }
    else//次峰类型是C
    {
        if(0==Peaks[2].data)//1st/3nd
            return 0;
        else
            if((Peaks[0].data)/(Peaks[2].data)>2)
                return 0;
        else
            return -1;
    }
}
