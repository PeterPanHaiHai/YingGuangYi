#include "login.h"
#include "ui_login.h"
#include <QtGui>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->lineEdit_UserName->setFocus();
    ui->lineEdit_PassWd->setFont(QFont("unifont",10));//As helvetica has no number so i use unifont
    ui->lineEdit_PassWd->setEchoMode(QLineEdit::Password);

    DefaultUser();//用默认帐号即上一次登录用户和密码登录
    QFont font("helvetica",24,QFont::Bold);
    ui->pushButton_Login->setFont(font);
    ui->pushButton_Login->setFocusPolicy(Qt::NoFocus);
    connect(ui->pushButton_Login,SIGNAL(clicked()),this,SLOT(judgeText()));
}

Login::~Login()
{
    delete ui;
}
//if you are legitimate,then show the dialog
void Login::loginin()
{
    UpdateDefaultUser();//更新默认帐号里面的信息，换成上次登录帐户
    dialog=new Dialog();
    dialog->show();
    dialog->setSoftWareVersion(SoftwareVersion);
    dialog->inputUserName=inputUserName;
    this->close();
}

void Login::judgeText()
{
    //loginin();
    Mywarning *warning=new Mywarning(this);
    warning->move((800-warning->width())/2,(480-warning->height())/2);
    QString inputUserName=ui->lineEdit_UserName->text();
    QString inputPassWd=ui->lineEdit_PassWd->text();
    *MyInputUsername=ui->lineEdit_UserName->text();
    if(inputUserName=="" || inputPassWd=="")
    {
        warning->setText(QString::fromUtf8("请输入用户名或密码"));
        warning->exec();
    }else if(SQLITE::sql->Select(inputUserName,inputPassWd,0)!="0")
    {
        loginin();
    }else{
        warning->setText(QString::fromUtf8("用户名或密码错误"));
        warning->exec();
        ui->lineEdit_PassWd->setText("");
    }
}

void Login::SetVersion(QString currentVersion)
{
    SoftwareVersion=currentVersion;
}

void Login::DefaultUser()
{
    QFile defaultUserFile("user/defaultuser.txt");
    QFileInfo info(defaultUserFile);
    if(defaultUserFile.open(QIODevice::ReadOnly)<0)
    {
        perror("open error!\n");
    }
    if(!info.exists())
        printf("no defaultUser file");
    else
    {
        QTextStream stream(&defaultUserFile);
        QString line;
        line=stream.readAll();
        QString DefaultUserName=line.section(',',0,0);
        QString DefaultUserPwd=line.section(',',1,1);
        ui->lineEdit_UserName->setText(DefaultUserName);
        ui->lineEdit_PassWd->setText(DefaultUserPwd);

    }
    defaultUserFile.close();
}

void Login::UpdateDefaultUser()
{
    QFile defaultUserFile("user/defaultuser.txt");

    if(defaultUserFile.open(QIODevice::WriteOnly)<0)
    {
        perror("open error!\n");
        QMessageBox::information(this,tr("error"),tr("open Result.txt file error"));
    }
    QTextStream out(&defaultUserFile);
    out<<ui->lineEdit_UserName->text()<<","<<ui->lineEdit_PassWd->text();
    defaultUserFile.close();
}
