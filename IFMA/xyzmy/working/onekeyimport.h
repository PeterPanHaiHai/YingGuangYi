#ifndef ONEKEYIMPORT_H
#define ONEKEYIMPORT_H
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <QFile>
#include <QCoreApplication>
#include <QStringList>
#include <malloc.h>
#include <QFileInfo>
#include <QScriptEngine>
#include <QScriptValue>
#include <QTextStream>
#include "qmessagebox.h"
#include <QScriptValueList>
class OneKeyImport
{
public:
    OneKeyImport();
    bool WeatherHaveSD();//是否存在SD卡
    QStringList FindFormulaFileInSD(char *);//查看SD卡中信息文件个数
    bool ImportFormula(QString);//导入信息文件
    bool WriteFiletoFlash();
    QString FormulaPath[100];
    QString showContent(QString,QString,int i);//从信息文件中返回第i位的信息
    //QString showContentFromSD(QString,QString,int i);//从信息文件中返回第i位的信息
    int showstrips(QString,QString);//返回当前检测项目条带数
    float DetermineFormula(QString,QString,float);//根据给定参数来唯一确定公式
    QString DetermineCalibrateParameters(QString Project,QString Batch);//根据项目号和批次号来确定对应校准参数

    float *showCalibrateparameters(QString,QString);//返回当前检测项目校准参数
    void SaveProjectFile(QStringList);
    QString Show_decimal_Unit(QString Project, QString Batch);
    QString Show_Qualitative(QString Project, QString Batch);
    QString ShowLimit(QString Project, QString Batch);
private:
       int count;
};

#endif // ONEKEYIMPORT_H
