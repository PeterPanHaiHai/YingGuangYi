#ifndef SELFCHECK_H
#define SELFCHECK_H

#include <QDialog>
#include <QTimer>
#include <Qt/qmovie.h>
#include "login.h"
#include "checkshow.h"
#include "mywarning.h"
//#include "mthread.h"
//Time:2013-03005
//UPDATE:判度当前仪器是否存在问题，如果有问题，该怎么办
namespace Ui {
    class SelfCheck;
}

class SelfCheck : public QDialog
{
    Q_OBJECT

public:
    explicit SelfCheck(QWidget *parent = 0);
    ~SelfCheck();

private:
    Ui::SelfCheck *ui;
    Login *MyLogin;
    QTimer *update;
    int updatecount;
    QString SoftwareVersion ;
    CheckShow *inandout;//for once in and out
    void JudgeMachine();//

    Mywarning *warning;

    // QTimer *progresstimer;
     //int count;
   //mthread *inandoutthread;
    // mthread *inandoutthread;
   // QTimer *inandouttimer;
       // MyLogin.show();
protected:
     void paintEvent(QPaintEvent *event);
public slots:
    void startCheck();
    void finished_SelfCheck();
    void SteppernotMovingWarning();
    void setVersion(QString Version);
    //void updateProgress();
    //void updateprogress();

};

#endif // SELFCHECK_H
