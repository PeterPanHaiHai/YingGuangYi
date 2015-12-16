#ifndef GRADIENT_H
#define GRADIENT_H
//算法思想，先求梯度，再对梯度作平滑，后对梯度数据作二值化处理，最后作分析计算

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <QString>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>


class gradient
{

public:
    explicit gradient(double *);
    ~gradient();
    int *Calcuate(double *Savedata);
    double *Getheight(double *);
    void save_to_file(double *Savedata,QString name);
private:
    //Ui::gradient *ui;
    //QFileInfo info;
    int MaxLength;
    int GAP;
    //QString file;
    double *Savedata;//存储原始数据
    double *finaldata;//存储求导后的数据
    double *SmoothSaveData;//存储平滑后的数据
    double *Binaridata;//存储二值化数据
    int *SixStrips;
    int widthThreahold;
    //double *GetData(double *);
    //int *position;
    double average(double *data,int i,int count);
    double *finditsbelongs(double *,double *,int *,int *,int *,int length);//判断每一个点属于哪个区域，属于第几条带
    int findbaseline(int *,double *);
    //int *StandArea;
    int Max;
    int Min;
    int Strips;
    bool belongsArea(int data,int First,int End);
    bool *existcurves;
    //QString OpenFile();
    void Savefinaldata(double *data,QString name);
    void smooth();
    double *ascentdata(double *);//对所有离散数据求到，使用差分法
    void smoothascentdata(int GAP);//将梯度化处理后的数据滤波平滑 from i-GAP---->i+GAP
    double *BInarization();//将平滑后的数据二值化处理
    int *GetPeakPoint(double *);
    double *Getheight(double *,double *);
    bool judgeCurves(int position,double height,int width,int leftwidth,int rightwidth);
    //bool NegativeCurves();//如果检测发现了两个负的波行，这两个负的波形临近在一起，则会形成一个错误的波峰

    //void ShowAnSwer();
};

#endif // GRADIENT_H
