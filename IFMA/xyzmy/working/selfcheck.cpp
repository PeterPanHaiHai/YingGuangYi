#include "selfcheck.h"
#include "ui_selfcheck.h"
#include "positioncalibrate.h"
SelfCheck::SelfCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelfCheck)
{
     ui->setupUi(this);
     setWindowFlags(Qt::FramelessWindowHint);
     updatecount=0;

     QMovie *movie=new QMovie(":/ui/waiting.gif");
     movie->start();
}

SelfCheck::~SelfCheck()
{
    delete ui;
}

void SelfCheck::startCheck()
{
    inandout=new CheckShow(this);//生成一个检测类
    inandout->hide();
    inandout->m3352.Motor_out();
    ::sleep(4);
    inandout->isSelfCheck=true;
    inandout->set_linear_in();
    finished_SelfCheck();
}

void SelfCheck::SteppernotMovingWarning()
{
    warning=new Mywarning;
    warning->move((800-warning->width())/2,(480-warning->height())/2);
    warning->setText(QString::fromUtf8("仪器工作不正常，插拔电源线重启"));
    warning->exec();
}

void SelfCheck::setVersion(QString Version)
{
    SoftwareVersion = Version ;
}

void SelfCheck::finished_SelfCheck()
{
    JudgeMachine();
    this->close();
    MyLogin=new Login;
    MyLogin->SetVersion(SoftwareVersion);
    MyLogin->show();
}

void SelfCheck::paintEvent(QPaintEvent *event)
{}

void SelfCheck::JudgeMachine()
{
    warning=new Mywarning;
    warning->move((800-warning->width())/2,(480-warning->height())/2);
    warning->setText(QString::fromUtf8("仪器工作正常"));
    warning->exec();
}
