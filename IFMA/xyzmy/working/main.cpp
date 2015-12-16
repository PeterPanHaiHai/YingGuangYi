#include <QtGui/QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <qfont.h>
#include "init_class.h"
#include "login.h"
#include "myinputpanelcontext.h"
#include "common.h"
#include "warning_two.h"
#include "backupanddelete.h"
#include "initiate.h"
#include "selfcheck.h"
#include "sqlite.h"
#include "gradient.h"
#include "onekeyimport.h"

extern QString *MyInputUsername=new QString();
TestShow *testshow ;
SetShow *setshow ;
BackUpandDelete *backupDelete ;
bool weatherpoweron();
void Init_Class();
static QString SoftwareVersion="V1.0.11.26";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Init_Class();//声明全局变量的类

    createDirectory();
    QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    MyInputPanelContext *ic = new MyInputPanelContext;
    a.setInputContext(ic);
    //在所有输入文本框中调用软键盘
    if(weatherpoweron())//判断仪器是否上电
    {
        unlink("/opt/power");
        SelfCheck *check=new SelfCheck;
        check->show();
        check->startCheck();
        check->setVersion(SoftwareVersion);//设置版本号
        //如果仪器上电，则进行开机自检
    }else{
        Login *MyLogin=new Login;
        MyLogin->SetVersion(SoftwareVersion);
        MyLogin->show();
        //  “退出”重新进入软件，则直接登录
    }
    int ret=a.exec();

    if(ret==773)
    {
        //“点击退出”按钮，进行程序重启
        QProcess::startDetached("/usr/share/zhiyuan/zylauncher/start_zylauncher",QStringList());
        return 0;
    }
    return ret;
}
//用一个文件去判断是不是上电了
bool weatherpoweron()
{
    QFileInfo power("/opt/power");
    if(!power.exists())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Init_Class()
{
    SQLITE::sql->DBOpen();
    SQLITE::sql->createTable("User");
    SQLITE::sql->createTable("Record");
    testshow=new TestShow();
    setshow=new SetShow();
    backupDelete=new BackUpandDelete();
}
