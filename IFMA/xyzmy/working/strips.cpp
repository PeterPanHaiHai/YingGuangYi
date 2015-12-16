#include "strips.h"
#include "QFile"
#include "QDebug"

strips::strips()
{
}

strips::~strips()
{
    delete Gradient;
}

int strips::Two_Curves_Algorithm()
{}


int *strips::Six_Curves_Algorithm(double *SmoothSaveData)
{


    //对六条带波峰作定性判断
    //算法思想：
    /*
      Date:2013-10-14
      现在思路是：根据宋提供的反馈信息，进行区域划分，在每个区域中搜索对应信息
      只需要找到对应六个波峰的波峰点即可
      下一阶段：进行波峰搜索算法的修改，使其匹配六条带检测

      */
    //int MAXNUMBER=100;//it can be modified
    int *peakpoint=(int *)malloc(sizeof(int)*6);

    //peakpoint[0]=findlargest(SmoothSaveData,200,300);
    //peakpoint[1]=findlargest(SmoothSaveData,300,450);
    //peakpoint[2]=findlargest(SmoothSaveData,450,600);
    //peakpoint[3]=findlargest(SmoothSaveData,600,750);
    // peakpoint[4]=findlargest(SmoothSaveData,750,950);
    // peakpoint[5]=findlargest(SmoothSaveData,950,1100);
    Gradient=new gradient(SmoothSaveData);
    peakpoint=Gradient->Calcuate(SmoothSaveData);
    printf("First %d\n",peakpoint[0]);
    printf("Second %d\n",peakpoint[1]);
    printf("Third %d\n",peakpoint[2]);
    printf("Fourth %d\n",peakpoint[3]);
    printf("Fifth %d\n",peakpoint[4]);
    printf("Six %d\n",peakpoint[5]);


    return peakpoint;
}
double *strips::Six_Curves_Height(double *data)
{//SmoothSaveData = data;
    double *heightpoint;

    gradient heightGradient(data);
    heightpoint = heightGradient.Getheight(data);

    return heightpoint;
}

int *strips::findposition(double *SmoothSaveData)
{
    int *peakpoint=(int *)malloc(sizeof(int)*6);

    Gradient=new gradient(SmoothSaveData);
    printf("First %d\n",peakpoint[0]);
    printf("Second %d\n",peakpoint[1]);
    printf("Third %d\n",peakpoint[2]);
    printf("Fourth %d\n",peakpoint[3]);
    printf("Fifth %d\n",peakpoint[4]);
    printf("Six %d\n",peakpoint[5]);
    return peakpoint;
}

int strips::findlargest(double *data,int begin, int end)
{
    int i=begin+1;
    int max=0;
    int point=0;
    for(i=begin;i<end;i++)
    {
        if(data[i]>max)
        {
            max=data[i];
            point=i;
        }
    }
    return point;
}

int strips::GetNumber(QString fileName)
{
    fileName = "/opt/formula/" + fileName + ".txt" ;
    QFile file(fileName) ;//text.txt放在可执行文件夹下
    //若要在text/text.txt文件夹下，就要用file("text/text.txt")
    file.open(QIODevice::ReadOnly);
    QString line ;
    QTextStream stream(&file);//此处的file有'＆',勿忘记
    while(!stream.atEnd())
    {
        line = stream.readLine();
    }
    line = line.section(' ',0,0);
    qDebug()<<"line="<<line;
    return line.toInt() ;
}
