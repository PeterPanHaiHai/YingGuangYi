//testshow is the class that used to search the information of the patient,and show the information
#ifndef TESTSHOW_H
#define TESTSHOW_H

#include <QDialog>
#include <QBitmap>
#include <QDateTime>
#include <QFile>
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardItemModel>
#include <sqlite.h>
#include <QTableView>
#include <QBrush>
#include <QComboBox>
#include <QTableView>
#include <QProcess>
#include <QTableWidgetItem>
#include <QCalendarWidget>
#include <QValidator>
#include <QRegExp>
#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "mywarning.h"
#include "iconv.h"
#include <QStringList>
#include <QList>
#include <checkoption.h>
#include <qdebug.h>


typedef unsigned long DWORD;
typedef unsigned char UCHAR;
typedef unsigned short WCHAR;
namespace Ui {
    class TestShow;
}

class TestShow : public QDialog {
    Q_OBJECT
public:
    TestShow(QWidget *parent = 0);
    ~TestShow();
    void initiatemove();
    void SetSoftWareVersion(QString);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TestShow *ui;
    int timerEvent_Id;
    void timerEvent(QTimerEvent *);
    QStandardItemModel *model;

    Mywarning *warning;
    int seriport_open;
    int seriport_print;
    int page ;//页码
    int pageTotal;
    QString patientID;
    QString Testoption;
    QString Bach_ID;
    QString PatientName;
    QString PatientAge;
    QString PatientGender;
    QString data;
    QString YearDate;
    QString CurrentDate;
    QString inputUser;
    QString serialNumber;
    QString FinalResult;
    QString testtime;
    QTableView *tableView;
    QFrame *frame;
    QLabel *label_Name;
    QDateEdit *dateEdit;
    QLabel *label_Id;
    QLabel *label_T;
    QLabel *label_Batch;
    QLabel *label_G;
    QLabel *label_operator;
    QLabel *label_Age;
    QLabel *label_Time;
    QLabel *label_Result;
    QLabel *label_Data;
    QPushButton *CloseButton;
    QList<QString> list ;//保存所有查询得到的结果
    void initShowResult();
    void getCurrentData();
    void setTableViewBlankColor();
    void initComboxname();
private slots:
    void page_up();
    void page_down();
    void page_go();
    void home();
    void find_info();//find the information of the patient
    void showResult(QList<QString>);
    void initialize(int,QStringList);
    void PrintItem();//print the information of the patient
    void addtop();//show the name of the data,such as PaitnetName ,Age,Gender
    void setPrintTrue(QModelIndex );//when we choose a item,then print button is enabled
    int  openSerialPort(const char *Seri_dev);
    void miniPrintWriteAscall(int fd,char data);
    void miniPrintWriteUnicode(int fd,char *pwchello);
    int code_convert(char *from_charset,char *to_charset,char *inbuf,unsigned int inlen,char *outbuf,unsigned int outlen);
    int u2g(char *inbuf,int inlen,char *outbuf,int outlen);

    //int MYCall_Back(void *pdata,int argc,char *value,char *name[]);

    void view_clicked();
    void logout();
    void on_pushButton_clicked();
};

#endif // TESTSHOW_H
