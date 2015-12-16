#include "gradient.h"
gradient::gradient(double *savedata)
{
    MaxLength=1200;
    Max=1;
    Min=0;
    GAP=1;
    Strips=6;
    widthThreahold=4;
    finaldata=(double *)malloc(sizeof(double)*MaxLength);
    Savedata=(double *)malloc(sizeof(double)*MaxLength);
    SmoothSaveData=(double *)malloc(sizeof(double)*MaxLength);
    Binaridata=(double *)malloc(sizeof(double)*MaxLength);

    existcurves=(bool *)malloc(sizeof(bool)*Strips);
    memset(existcurves,false,sizeof(existcurves));
    SixStrips=(int *)malloc(sizeof(int)*Strips);
    memset(SixStrips,0,sizeof(int)*Strips);
    ascentdata(savedata);//对平滑后的数据进行求导--finaldata
    smooth();//对求导后的数据进行平滑：幅度为4--SmoothSaveData
    BInarization();//二值化求导后的数据--Binaridata
    int *point=GetPeakPoint(Binaridata);
}

gradient::~gradient()
{
    free(finaldata);
    free(Savedata);
    free(SmoothSaveData);
    free(Binaridata);
    free(SixStrips);
    free(existcurves);
}

double *gradient::ascentdata(double *Savedata)
{
    int i=0;
    while(i<MaxLength)
    {
        finaldata[i]=(Savedata[i+GAP]-Savedata[i])/GAP;//calcuate gradient
        i++;
    }
}

void gradient::save_to_file(double *Savedata,QString name)
{//保存数据：求导后、平滑、二值化
    QString fileName;
    int i=0;
    fileName="points/" + name + ".txt";//create the file

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly)<0)
        printf("open error!\n");
    QTextStream in(&file);
    for(i=0;i<1200;i++)
        in<<(Savedata[i])<<endl;
}
void gradient::Savefinaldata(double *data,QString name)
{
    QString FileName=name;
    if(!FileName.isNull())
    {
        QFile Sfile(FileName);

        if(!Sfile.open(QIODevice::WriteOnly))
        {
        }
        QTextStream out(&Sfile);//QtextStream is used to handle the text
        int i=0;
        while(i<MaxLength)
        {
            out<<data[i]<<endl;
            i++;
        }
        Sfile.close();
    }
}

void gradient::smoothascentdata(int count)
{
    for(int i= 0;i<MaxLength;i++)
        SmoothSaveData[i]=average(finaldata,i,count);//average 函数用来求从i->(i+n)处像 平均值，SmoothSaveData就是平滑以后的数据
}

double gradient::average(double *data,int i,int count)
{
    double temp=0.0;
    int j;
    int max=0;
    int min=0;
    if(i-count<0||i+count>MaxLength)
    {
        temp=data[i];
    }
    else//i+count没有超过数值边界
    {
        for(j=i-count;j<i+count;j++)
        {
            temp+=data[j];
            //Drop the max and min
            if(data[j]<min)
                min=data[j];
            if(data[j]>max)
                max=data[j];
        }
        temp=(temp)/(2*count);
    }
    return temp;
}

void gradient::smooth()
{
    smoothascentdata(4);
}

double *gradient::BInarization()
{
    double Threshold=0.8;//1 is too small
    int i=0;
    for(;i<MaxLength;i++)
    {
        if(SmoothSaveData[i]>Threshold)
            Binaridata[i]=Max;
        if((SmoothSaveData[i]<Threshold)&&(SmoothSaveData[i]>(-Threshold)))
            Binaridata[i]=Min;
        if(SmoothSaveData[i]+Threshold<0)
            Binaridata[i]=0-Max;
    }
}

int *gradient::GetPeakPoint(double *data)
{
    int *point=(int *)malloc(sizeof(int)*MaxLength);//save the point its number is 1
    memset(point,0,MaxLength);
    int number=0;
    int FirstDrop=50;//drop the first and last point almost '50'
    int LastDrop=70;
    int i=FirstDrop;
    while(i<(MaxLength-LastDrop))//from all point get the point which its number is 1
    {
        if((Max==data[i])&&(Min==data[i+1]))
        {
            point[number++]=i;
        }
        i++;
    }
    printf("in peakpoint search number is%d\n",number);
    return point;
}

double *gradient::Getheight(double *src)
{
    double *Height=(double *)malloc(sizeof(double)*Strips);//save the point its number is 1
    memset(Height,0.0,sizeof(double)*Strips);
    int start[100];
    int up[100];
    int end[100];
    int widelength[100];//每个波峰的峰宽

    int FirstDrop=50;//drop the first  '50'
    int LastDrop=70;//drop last '70'
    int i=FirstDrop;
    int curvesnumber=0;

    for(int i=0;i<Strips;i++)
    {
        existcurves[i]=false;
    }
    //查找＝＝Max的区域，找到了可以确定起始点和波峰位置
    //查找＝＝Min的区域，找到了可以确定终止点位置
    while(i<(MaxLength-LastDrop))//from all point get the point which its number is 1
    {
        if(Max==Binaridata[i])//找到了第一个>0的点
        {
            int first=0;
            int peak=0;
            int final=0;

            first=i;//保存起始点
            while(Max==Binaridata[i])
            {
                i++;
            }//一直往后查找，找到>0的终止点
            peak=i;
            while(0==Binaridata[i])
            {
                i++;
            }
            //必须存在—Max的区域
            if(0==(Max+Binaridata[i]))//—Max区域起始位置
            {
                while(0==(Max+Binaridata[i]))
                {
                    i++;
                }
                final=i;
            }
            else
                continue;
            //现在Binaridata[i]=-Max,从这点开始查找整个区域
            int width=final-first;
            if(judgeCurves(peak,src[peak]-(src[first]+src[final])/2,width,peak-first,final-peak))//判断是否合法波峰通过波高和波宽
            {   start[curvesnumber]=first;
                up[curvesnumber]=peak;
                end[curvesnumber]=final;
                widelength[curvesnumber]=final-first;//峰宽
                curvesnumber++;//找到了一个波峰
            }
        }
        i++;//继续往后查找
    }
    finditsbelongs(Height,src,up,start,end,curvesnumber);
    int j=0;

    return Height;
}

int *gradient::Calcuate(double *savedata)
{
    return SixStrips;
}

double *gradient::finditsbelongs(double *Height,double *src,int *up,int *start,int *end,int length)
{
    int i=0;
    int position[Strips];

    for(i=0;i<length;i++)
    {
        if(belongsArea(up[i],160,320))//(1.33--2.66)
        {
            SixStrips[0]=up[i];
            existcurves[0]=true;
            Height[0]=src[up[i]]-src[start[i]];
            if(Height[0]<100)
                Height[0]=0;
            printf("376\n");
            printf("379\n");
            continue;
        }
        if(belongsArea(up[i],320,470))//(2.66-3.99)
        {
            SixStrips[1]=up[i];
            Height[1]=src[up[i]]-src[start[i]];
            if(Height[1]<100)
                Height[1]=0;
            existcurves[1]=true;
            printf("392\n");
            continue;
        }
        if(belongsArea(up[i],470,620))//(3.99-5.16)
        {
            SixStrips[2]=up[i];
            Height[2]=src[up[i]]-src[start[i]];
            if(Height[2]<100)
                Height[2]=0;
            existcurves[2]=true;
            printf("404\n");
            continue;
        }
        if(belongsArea(up[i],620,770))//(5.16-6.41)
        {
            SixStrips[3]=up[i];
            existcurves[3]=true;
            Height[3]=src[up[i]]-src[start[i]];
            if(Height[3]<100)
                Height[3]=0;
            printf("416\n");
            continue;
        }
        if(belongsArea(up[i],770,980))//(6.41-8.16)
        {
            SixStrips[4]=up[i];
            existcurves[4]=true;
            Height[4]=src[up[i]]-src[start[i]];
            if(Height[4]<100)
                Height[4]=0;
            printf("428\n");
            continue;
        }
        if(belongsArea(up[i],980,1130))//(8.16-9.41)
        {
            SixStrips[5]=up[i];
            existcurves[5]=true;
            Height[5]=src[up[i]]-src[start[i]];
            if(Height[5]<100)
                Height[5]=0;
            printf("436\n");
            continue;
        }
    }
    printf("442did\n");
    return Height;
}

int gradient::findbaseline(int *point,double *baseline)
{
    int i=0;
    for(i=0;i<Strips;i++)
    {
        printf("baseline %d is%f\n",i,baseline[i]);
        if(belongsArea(point[i],160,320))
        { SixStrips[0]=baseline[i];
            continue;
        }
        if(belongsArea(point[i],320,470))
        {
            SixStrips[1]=baseline[i];
            continue;
        }
        if(belongsArea(point[i],470,620))
        {
            SixStrips[2]=baseline[i];
            continue;
        }
        if(belongsArea(point[i],620,770))
        {
            SixStrips[3]=baseline[i];
            continue;
        }
        if(belongsArea(point[i],770,980))
        {
            SixStrips[4]=baseline[i];
            continue;
        }
        if(belongsArea(point[i],980,1130))
        {
            SixStrips[5]=baseline[i];
            continue;
        }
    }
}

bool gradient::belongsArea(int data,int First, int End)
{

    if((data<End&&data>First)||data==End||data==First)
        return true;
    else
        return false;
}

bool gradient::judgeCurves(int position,double height,int width,int leftwidth,int rightwidth)//判断是否合法波峰
{
    if((width<widthThreahold)||(height<0)||(leftwidth<widthThreahold)||(rightwidth<widthThreahold))
    {
        printf("position:%d is error\n",position);
        return false;
    }
    else
        return true;
}
