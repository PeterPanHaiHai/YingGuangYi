#ifndef YIQISUANFA_H
#define YIQISUANFA_H

#include <QDebug>
#include <QList>
#include "FLOUR_v1.0/qtest.h"
extern int feng1;
extern int feng2;
class Peak
{
public :
    Peak()
    {
        start = 0;
        end = 0;
        jiange = 0;
        max = 0;
    }

    ~Peak()
    {

    }
public:
    int start;
    int end;
    int jiange;
    int max;
};
void sortPeakListByJiange(QList<Peak*>* tmpPeakList,int l ,int r)
{
        if(l < r)
        {
            int i = l,j = r;
            Peak* tmpPeak = tmpPeakList->at(l);
            while(i < j)
            {
                while(i < j &&tmpPeakList->at(j)->max <= tmpPeak->max)
                {
                    j--;
                }
                if(i < j)
                {
                    tmpPeakList->replace(i++,tmpPeakList->at(j));
                }
                while(i < j && tmpPeakList->at(i)->max > tmpPeak->max)
                {
                    i++;
                }
                if(i < j)
                {
                    tmpPeakList->replace(j--,tmpPeakList->at(i));
                }
            }
            tmpPeakList->replace(i,tmpPeak);
            sortPeakListByJiange(tmpPeakList,l,i-1);
            sortPeakListByJiange(tmpPeakList,i+1,r);
        }
}

void checkTest(double data[],int i,double result[], double originData[])
{
    QList<Peak*> *peakList = new QList<Peak*>();

    Peak* peak = new Peak();

    peakList->push_back(peak);
    double *pinghuadata = new double[i];
    double *daoshudata = new double[i];
    for(int j=0;j<10/2;j++)
    {
      pinghuadata[j]=data[j];

    }
    for(int j=10/2;j<i-10/2;j++)
    {
       double avg = 0,sum = 0;
       for(int k=j-10/2;k<10+j-10/2+1;k++)
       {
           sum += data[k];
       }
       avg = sum/(11.0);
      pinghuadata[j]=avg;

    }
    for(int j=i-10/2;j<i;j++)
    {
      pinghuadata[j]=data[j];
    }

    for(int j=0;j<i;j++)
    {
       if(j==0)
       {
          daoshudata[j]=pinghuadata[j+1]-pinghuadata[j];
       }
       else if(j==i-1)
       {
          daoshudata[j]=pinghuadata[j]-pinghuadata[j-1];
       }
       else
       {
          daoshudata[j]=(pinghuadata[j+1]-pinghuadata[j-1])/2.0;

       }
    }
    qDebug()<<"testwindowsize=10";
    int testwindowsize=10;

    double avg=0.0;
    bool flag1 = false;
    bool flag2 = false;
    int feng_max[2] = {0};
    int feng[4]={0};
    data[0] = -1;//避免数据线一开始处于较高的位置,影响峰值的判断.

    for(int j=0;j<i-testwindowsize+1;j++)
    {
       //if(c==4) break;
       double sum = 0.0;
       for(int k=j;k<testwindowsize+j;k++)
       {
            sum += daoshudata[k];
       }
       avg=sum/testwindowsize;
       if(avg>=0.4 && !flag1)
       {
          flag1 = true;   //找到波峰起始点
          //feng[c] = j;
          //c++;
          peak->start = j;
       }
       else if(flag1 & !flag2)
       {

         if(avg<-0.5)
         {
             flag2 = true;
         }
         if(data[j]>data[peak->max])
         {
             peak->max = j;
         }
//         if(originData[j] > originData[peak->max])    //正确找到最高点位置
//          {
//                peak->max = j;
//          }
          if(j==500 || j==900)
         {
             flag2 = true;//因为下降过缓,导致没有正确判断时,当到达第500和第900点时,开始判断终止点
         }
       }
       else if(flag1 && flag2)
       {
           if(data[j]>data[peak->max])
           {
               peak->max = j;
           }
//           if(originData[j] > originData[peak->max])  //正确找到最高点位置
//             {
//                  peak->max = j;
//             }
           if ((avg>=-0.5|| j==1100))  //找到波峰终止点
             {
                 flag1 = false;
                 flag2 = false;
                 peak->end = j;
                 peak->jiange = peak->end - peak->start;
                 peak = new Peak();
                 peakList->push_back(peak);
             }
       }
    }


    //按波峰宽度进行降序排序
    //快速排序
    sortPeakListByJiange(peakList,0,peakList->count() - 1);
    //波峰判断
    if(peakList->count() >= 2)
        {
          if(peakList->at(0)->max < peakList->at(1)->max)
          {
              feng[0] = peakList->at(0)->start;
              feng[1] = peakList->at(0)->end;
              feng_max[0] = peakList->at(0)->max;

              feng[2] = peakList->at(1)->start;
              feng[3] = peakList->at(1)->end;
              feng_max[1] = peakList->at(1)->max;
          }else
          {
              feng[0] = peakList->at(1)->start;
              feng[1] = peakList->at(1)->end;
              feng_max[0] = peakList->at(1)->max;

              feng[2] = peakList->at(0)->start;
              feng[3] = peakList->at(0)->end;
              feng_max[1] = peakList->at(0)->max;
          }
        }

//    double bendi=0;
//    double sum_bendi=0;
    qDebug()<<"feng_max[0]"<<feng_max[0];
    qDebug()<<"feng_max[1]"<<feng_max[1];
    qDebug()<<"data[feng_max[0]]"<<data[feng_max[0]];
    qDebug()<<"data[feng_max[1]]"<<data[feng_max[1]];
//    for(int j=(feng_max[0]+feng_max[1])/2-20;j<(feng_max[0]+feng_max[1])/2+21;j++)
//    {
//         sum_bendi+=data[j];
//    }
//    bendi=sum_bendi/(41.0);

    for(int j=0;j<2;j++)
    {
      if(j==0)
      {
//        double mianji=0;
//        if(feng[j+1]-feng[j]>=50)
//        {
//            for(int x=feng_max[j]-20;x<feng_max[j]+21;x++)
//            {

//               mianji+=(data[x]-bendi);
//            }
//            result[0]=mianji;
//        }
//        else
//        {
//            result[0]=0;
//        }

          if((data[feng_max[0]]-data[feng[1]])>200)  //峰高度最少要有200，否则这个峰就认为为0
          {
              result[0] = data[feng_max[0]];  //直接取峰值，不算面积
          }
          else
          {
              result[0] = 0;
          }
      }
      else if(j==1)
      {
//        double mianji=0;
//        if(feng[j+2]-feng[j+1]>=50)
//        {
//            for(int x=feng_max[j]-20;x<feng_max[j]+21;x++)
//            {
//               mianji+=(data[x]-bendi);
//            }
//            result[1]=mianji;
//        }
//        else
//        {
//            result[1]=0;
//        }
          if((data[feng_max[1]]-data[feng[3]])>200)  //峰高度最少要有200，否则这个峰就认为为0
          {
              result[1] = data[feng_max[1]];  //直接取峰值，不算面积
          }
          else
          {
              result[1] = 0;
          }
      }
    }
    delete []pinghuadata;
    delete []daoshudata;
    qDeleteAll(*peakList);
    peakList->clear();
}

void qCheckTest(double data[],int i,double result[])
{
    int feng_max[2] = {0};
    double bendi=0;
    double sum_bendi=0;
    qTest(data);
    feng_max[0] = feng1;
    feng_max[1] = feng2;
    for(int j=(feng_max[0]+feng_max[1])/2-20;j<(feng_max[0]+feng_max[1])/2+21;j++)
    {
         sum_bendi+=data[j];
    }
    for(int j=0;j<2;j++)
    {
      if(j==0)
      {
        double mianji=0;
        for(int x=feng_max[j]-20;x<feng_max[j]+21;x++)
         {
               mianji+=(data[x]-bendi);
         }
            result[0]=mianji;
        }
      else if(j==1)
      {
        double mianji=0;
        for(int x=feng_max[j]-20;x<feng_max[j]+21;x++)
        {
             mianji+=(data[x]-bendi);
         }
            result[1]=mianji;
       }
    }

}

#endif // YIQISUANFA_H
