#ifndef URTCLASS_H
#define URTCLASS_H

#include <QObject>
#include <QSocketNotifier>
#include <QMessageBox>
#include <QString>
#include <termios.h>
#include <iostream>
#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      /*文件控制定义*/
#include <errno.h>      /*错误号定义*/
#include <math.h>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QTextStream>


class UrtClass : public QObject
{
    Q_OBJECT
public:
    explicit UrtClass(QObject *parent = 0);
    int fd,ret;
    QSocketNotifier * my_notifier;
    QString str1;
    QString tempstr;
    char *data;
    QTimer *getTimer;
    int direction;
    char * tempbuf;
    char * tempbuf1;
    char * tempbuf2;
    void Init_Dev();
    void Send_Dev(char *);
    void Close_Dev();
    int getDirection();
    void clearSeral();
signals:
    void sendNoti(char *,int);
    void sendID(char *);
public slots:
    void remoteDataIncoming();
    int DataIn(int mode);
};

#endif // URTCLASS_H
