//checkshow is the class that used to receive the points and show the curves

#ifndef CHECKSHOW_H
#define CHECKSHOW_H
#include <QBuffer>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <qthread.h>
#include <QLineEdit>
#include <sys/select.h>
#include <QObject>
#include <termios.h>
#include <QPainterPath>
#include <QDialog>
#include <m3352.h>
#include <QPixmap>
#include <QBitmap>
#include <QDateEdit>
#include <QProcess>
//Add a seriport function
#include <qsocketnotifier.h>//用于监听串口的类
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>
#include <termio.h>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <qpainter.h>
#include <QFile>
#include <fcntl.h>
#include <unistd.h>
#include <QTextStream>
#include <iostream>
#include <QColor>
#include <QBrush>
#include <QEvent>
#include <QDateTime>
#include <sys/stat.h>
#define R&D
//here is QScript head file
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QFileDialog>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <QRegExp>
#include <QPointF>
#include <signal.h>
#include <sys/types.h>
#include <signal.h>
#include "onekeyimport.h"
#include "checkoption.h"
#include "mywarning.h"
#include "countdown.h"
#include "genotype.h"
#include "strips.h"
#include "ui_checkoption.h"
#include "peak/rt_nonfinite.h"
#include "peak/findpeakcore.h"
#include "peak/fpeak.h"
#include "peak/findpeakcore_emxutil.h"
#include "peak/diff.h"
#include "qdebug.h"
#include "sqlite.h"
#define SIX_POINT_MACHINE //define it as six point calcuate
typedef unsigned long DWORD;
typedef unsigned char UCHAR;
typedef unsigned short WCHAR;
namespace Ui {
    class CheckShow;
}
class CheckShow : public QDialog {
    Q_OBJECT
public:
    CheckShow(QWidget *parent = 0);
    ~CheckShow();
    int seriport_linear;//used to control the linear
    //int seriport_print;//used to print
    char linearStart;
    void SetSoftWareVersion(QString,QString);
    QString inputUserName;
    QDateTime CurrentDate;
    QString Bach_ID;
    QString YearDate;
    QString FinalResult;
    QString finalAreadivided;
    QString Consistency;
    void initshow();
    bool isCalibrate;//标志是否是在校准
    bool isSelfCheck;//标志是否自检
    bool TestFinished;//weather one Test finished
    void hideui();
    int GetCurrentPosition();
    int Lastmovedirection;//where linear goes next
    int NextMoveDirection;
    bool HAS;
    OneKeyImport Test;
    M3352 m3352 ;
    bool weatherCalibrate;
    int saveFileName;

    bool WeatherLinearMoving;
protected:
    void changeEvent(QEvent *e);

private:
    Ui::CheckShow *ui;
    bool isDraw;
    int myTimerId;
    bool weatherSaved;
    void ShowProjectName(QString);
    void ShowID();
    bool areyoucalibrate;
    void setCalibrate(bool);
    int seriport_print;//used to print
    void miniPrintWriteUnicode(int fd,char *pwchello);
    int  openSerialPort(const char *Seri_dev);
    int code_convert(char *from_charset,char *to_charset,char *inbuf,unsigned int inlen,char *outbuf,unsigned int outlen);
    int u2g(char *inbuf,int inlen,char *outbuf,int outlen);

    bool judgeweatherSaved(QString ID);
    void showResult();
    void deal_paintdata();//平滑要显示的数据，平滑头尾部
    bool stayincheckshow;
    int CurrentStatus;
    int SwitchFlag;
    QString Result;

    Mywarning *warning;
    strips *TestStrips;

    int err;
    void *tret;
    QPixmap pixmap;
    QPixmap temppix;
    bool calibrateTest;
    QSocketNotifier *m_notifier;//used to monitor the serial port,judge weather it recives data
    void timerEvent(QTimerEvent *);
    int timerEvent_Id;
    int timerEvent_Id2;

    int CountDownTime;
    bool movelinear;
    CountDown *count;
    QString patientID;
    QString TestType;
    QString TypeItem;
    QString Testoption;
    bool weatherdrawsmooth;
    bool OPtionopen;
    int Savecount;

    QString FirstArea;
    QString SecondArea;
    QString QDivided;
    QString PatientName;
    QString PatientAge;
    QString PatientGender;
    QString GenderItem;
    QString password;
    float switchToConsistency(float );//judge the curves,and calcuate it by a formula
    float switchToConsistency_AT(float );
    bool weatherDrawBlack;
    QString bach_ID_show;
    QString Project_Calcuate;
    void SaveAllPoints(double * data,QString name, int total);//save 300 points in disk
    void SaveAllPointsByCPP(double *data, QString name, int total);
    void createdirectory();
    void initiateInputDialog();
    bool linearAtOut;
    bool linearAtInside;
    QByteArray byteArray;
    QBuffer dataBuffer;
    bool showPixtureScreen;
    int showSmooth;
    float showSmoothNumber[300];//Used to save show curves number
    double searchPeak(double *);
    void NewAlgorithm();//listen to MIN xiaoPing 's advice,i changed my algorithm

    //***************************below is AD7707
    void delay(unsigned char);
    int average();
    int dataCount;
    int SerachDrdyNumber;
    QString GetDelayTime();
    QString MAXDrdyCount;
    int MAXDrdyRepeat;
    Mywarning *warningerror;
    int linear;
    int photodiodeds;
    float StandardValue[6];
    float RealValue[6];
    bool Second_Curves;
    //******************************


    // add by marxsu

    int mTestCount;
    int mTestFreq;
    void SaveAllArea(QString name);
signals:
    void begintodraw();
    void CheckShowFinished();
    void finishedDataReceived();
    void RepeattestValue(QString);
    void currentSelfCheck();
    void Steppernotmoving();
private slots:

    void smooth(int);
    void ResultSave();
    void printInfo();
    void initshowStrips();//initiate strips TWO or SIX
    void drawpoint(QPainter *);
    float average(int i,int count);
    void paintEvent(QPaintEvent *event);
    void drawaxis(QPainter *);//draw the axis
    void drawfinalshow(QPainter *);
    void OptionShow();
    void Step_in_or_out();
    void drawCurves(QPainter *paint);
    void ShowCountDown();//show a countdown class
    void drawBlack(QPainter *paint);
    int CheckStatusBefore();
    void finished_Test();
    void BeginRepeatTest();
    void showAverage(int);
    void FinihedTestDraw(QPainter *paint,double *);
    void SaveFileToSD(float *);
    void SaveConsistency(QString ConsistencyName);
    int automaticallyScales();
    float correct(float);//to ensure each mahine have the same data when you test with same cassette
    float correct_3(float);
    void sendFinished();
    void ReadCalibrateFile();//read calibrate file
    void LogOut();
    void Pro_change();//项目变了，批次号也要相应的变化

    bool whetherNegative(int,int);

public slots:
    void acitveButton(QString);
    void set_linear_in();
    //****************************************

    void home();//back to last interface
    float gettheaverage();
    void deleteCurves();
    void exceptionjudgement(float AC,float AT);//judge weather exception cassette

    //test by MarxSu
    void onTestButtonOnClick();
    void onTestOkButtonOnClick(int,int,int,int,int,int,int,int,int,int);
    void onChangeAppButtonClick();

    void onSaveButtonOnClick();
    void removefilesindir(const QString& path);
    void onDeleteButtonOnClick();
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);

    void onTestCountSet();

    qint64 dirSize(QString );
};

#endif // CHECKSHOW_H
