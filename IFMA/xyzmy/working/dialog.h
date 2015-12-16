//dialog is the class of the main form,it include checkshow Testshow,and setshow
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QBitmap>
#include <QPushButton>
#include <QBitmap>
#include <QRect>
#include <QDateTime>
#include <sys/time.h>
#include <init_class.h>
#include <time.h>
#include <stdio.h>
#include <sqlite.h>
#include "checkoption.h"
#include "connector.h"

/***************
  wanghai add
  *****************/
#include <QtSerialPort/qserialport.h>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "sqlite.h"
#include "commserial.h"

#define PORTNAME "/dev/ttyO2"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString inputUserName;//the User's name
    void setSoftWareVersion(QString);
private:
    Ui::Dialog *ui;
    QPushButton *xyz;
    QString SoftWareVersion;
    int timerEvent_Id;
    CheckOption *OptionShow;
    CheckShow *checkshow;
     Connector connector;

    double runingTime;
    bool judgeadmin();

    //wanghai add
    QSerialPort      serialport;
    QString          hostString;
    QByteArray       m_readData; //接收命令数组
    QList<QString>   allDataList;  //存放查询所有数据的数组
    QList<QString>   tilNowDataList; //存放查询某个时间点到现在的所有数据的数组
    QString          result;  //要上传的字符串

    CommSerial *commSerial;

    void openPort();
    void uploadAllData();
    void uploadTilNowData(QString, QString, QString, QString);
    void emptyAllData();

private slots:
    void showCheck();//show the CheckShow
    void showTest();//show the TestShow
    void showSet();//show the SetShow
    void showBackupandDelete();//show the BackupandDelete
    void timerEvent(QTimerEvent *e);
    void showoption();
    void close();
    static void logout();

    //wanghai add
    void handleCommand(QByteArray);  //命令处理
    void handleClearArray();  //清空m_readData数组
    void handleReadyRead();


signals:
    //wanghai add
    void ReceiveCommand(QByteArray); //命令处理信号
    void ClearArray();  //清空m_readData数组信号，以便接收下一个命令
};

#endif // DIALOG_H
