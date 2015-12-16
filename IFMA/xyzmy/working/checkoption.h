//CheckOption is the class that used for patient to input their information

#ifndef CHECKOPTION_H
#define CHECKOPTION_H
#include <QDialog>
#include <QFile>
#include <unistd.h>
#include <QPainter>
#include <QTextStream>
#include <QTextCodec>
//#include "checkshow.h"
#include <QDateEdit>
#include <sys/vfs.h>
#include <QMessageBox>
#include <QTimer>
#include <unistd.h>
#include <QKeyEvent>
#include <fcntl.h>
#include <QLineEdit>
#include <QValidator>
#include <QDataStream>
#include <QSqlQuery>
#include "iconv.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <QByteArray>
#include <unistd.h>
#include <string.h>
#include <QtGui/QHBoxLayout>
#include <fcntl.h>
#include <termio.h>
#include <QBitmap>
#include <dirent.h>
#include <QTreeView>
#include <QSortFilterProxyModel>//used to filter dir to shio just linear
#include <QDirModel>
#include <QDesktopWidget>
#include "mywarning.h"
#include "print.h"
#include "common.h"
namespace Ui {
class CheckOption;
}

class CheckOption : public QDialog
{
    Q_OBJECT

public:
    explicit CheckOption(QWidget *parent = 0);
    ~CheckOption();

    bool Key;
    bool weatherSaved;//weather the serialnum saved
    QString patientID;
    QString inputUserName;
    QDateTime CurrentDate;
    QString YearDate;
    QString linearStarted;
    QString TestType;
    QString serialNumber;
    QString PatientName;
    QString PatientAge;
    QString PatientGender;
    QValidator *validator;
    QString data;
    QString Result;
    int GenderItem;
    int TypeItem;
    Ui::CheckOption *ui;
    int seriport_print;//used to print
    //below elements used to backUp and delete
    QPushButton *pushButton_BackUp;
    QTreeView *Treeview_BackUp;
    QPushButton *pushButton_Cancel;
    QPushButton *pushButton_Delete;
    void OptionTestShow();
    void setData(QString );
    void SetFinalConsistency(float Consistency);
    QString getProjectName();
    QString getBach_IDName();
    Mywarning *warning;
signals:
    void Pressed_ok(bool);
    void Pressed_cancel(bool);
    void active2();
    void linear_in();
    void shouldupdate();
private:
    QSqlQuery query;
    //void paintEvent(QPaintEvent *);
    int timerEvent_Id;
    QTimer *timer;
    QLineEdit *lineEdit_ID;
    void createProjecttable();
    void ShowProjectName();
    //QPushButton *RepeatButton;
    QString Bach_ID;
    //void GetbachID();
    QDirModel *DModel;
    QSortFilterProxyModel *filtermodel;
    QFile *Resultfile;
    //QDataStream *writestream;
    QTextStream *WriteResult;
    bool BackUPandDelete();
    void delayFiveMinutes();
    void showDirectories();
    void savefileText();
    void openResultFile();
    int  openSerialPort(const char *Seri_dev);
    void miniPrintWriteAscall(int ,char );
    void showResult();
    bool CheckIfhasFormula();
    //void miniPrintWriteUnicode(int fd,QString );
    void miniPrintWriteUnicode(int fd,char *pwchello);
    int code_convert(char *from_charset,char *to_charset,char *inbuf,unsigned int inlen,char *outbuf,unsigned int outlen);
    int u2g(char *inbuf,int inlen,char *outbuf,int outlen);

     bool judgeweatherSaved(QString ID);
    //sqlite3 *db;
    //void opensqlite3();
    //void Sqlite3_savemessage();
private slots:
    void deleteandBackupfunction();
    void SwitchToinput();
    void removeDirectory();
    void BackUpData();
    int OK();
    void CancelOption();

protected:
    // bool eventFilter(QObject *, QEvent *);
    void keyPressEvent(QKeyEvent *);
public slots:
    void home();
    void acitveButton(QString);
    //int Enter();
    void ResultSave();//save the information of the patient
    void printInfo();//print information of the patient

    void OptionResultShow();
    void initiatemove();
    //void RePeatTest();

};

#endif // CHECKOPTION_H
