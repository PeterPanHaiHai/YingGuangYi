//setshow is the class that used to make some enactment,now is empty,//
//we can add functions when we need
//here some special set for admin,ou can change admin's pwd,while you can not delete it

#ifndef SETSHOW_H
#define SETSHOW_H
#include <QDialog>
#include <QBitmap>
#include <QDateTime>
#include <QDateEdit>
#include <QFrame>
#include <QComboBox>
#include <QLineEdit>
#include <QSqlQuery>
#include <QLabel>
#include <QRegExp>
#include <QFile>
#include <QProcess>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <sys/stat.h>
#include "mywarning.h"
#include <sqlite.h>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include "common.h"
#include "warning_two.h"
#include "calibrate_six.h"
#include "onekeyimport.h"
#include "onekeyimport.h"
namespace Ui {
    class SetShow;
}

class SetShow : public QDialog {
    Q_OBJECT
public:
    SetShow(QWidget *parent = 0);
    ~SetShow();

    void initiatemove();
    void SetSoftWareVersion(QString,QString);
    bool judge_subadmin();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::SetShow *ui;
    void timerEvent(QTimerEvent *);
    int timerEvent_Id;
    QString inputUserName;
    QString Current_user ;
    OneKeyImport *Import;
    Mywarning *warning;
    warning_two *DelProwarning;
    QPushButton *superbutton;
    QFrame *TimeFrame;
    QDateEdit *dateEdit;
    QTimeEdit *timeEdit;
    QPushButton *ChangeButton;
    QPushButton *CancelButton;
    void initTimeSet();
    void initUserSet();
    void initProSet();
    void initTime();
    //here below is Project element
    QFrame *Frame_Project;
    QLineEdit *lineEdit_CurrnetProject;
    QPushButton *pushButton_add;
    QPushButton *pushButton_Delete;
    QPushButton *pushButton_Exit;
    QComboBox *Project_box;
    QComboBox *Choose_box;
    QLabel *label_Group;
    QLabel *label_idFrame;
    QLabel *label_pwdframe;
    QLabel *label_input;
    QPushButton *Button_Back;
    QLabel *label_Project;
    //here below is User element
    QFrame *Frame_User;
    QLabel *label_User;
    QComboBox *comboBox_User;
    QLabel *label_CurrentUsernamet;
    QLineEdit *lineEdit_CurrentUsername;
    QPushButton *pushButton_UserAdd;
    QPushButton *pushButton_UserSave;
    QPushButton *pushButton_UserDelete;
    QPushButton *pushButton_UserExit;
    QLineEdit *lineEdit_CurrenttPwd;
    QLabel *label_CurrentPasswd;

private slots:
    void home();
    void show_exit();
    void superBehavior();
    void showTimeSet();
    void initShowProject();
    void SetMachineTime();
    void TimeSetHide();
    void addProjectRecord();
    void realAddPro();
    void deleteProjectRecord();
    void realDelPro();
    void quitSetProject();
    void initShowUser();
    void Choose_Add_Del();

    //here below is User set
    void addUserRecord();
    void deleteUserRecord();
    void ChangeUserRecord();
    void quitSetUser();
    void addUserrecordCommobox();
    void serachRealtimepwd(QString Firstusername);
    void logout();
    void calibrate();
    void touchscreenCalibrate();
    void realCalibrate();

};

#endif // SETSHOW_H
