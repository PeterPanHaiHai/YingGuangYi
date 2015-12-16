#include "countdown.h"
#include "ui_countdown.h"
int MyStartTimer=200;
CountDown::CountDown(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CountDown)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    CountDownTime=30*1000/MyStartTimer;//set to 10seconds
    //update timer after 1 seconds
    timerEvent(0);
    timerEvent_Id=startTimer(MyStartTimer);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(send_signals()));

}

CountDown::~CountDown()
{
    delete ui;
}

//count down for 10 seconds
void CountDown::timerEvent(QTimerEvent *e) //To calcuate the cost Time
{
    if(CountDownTime==0)
    {
        CountDownTime=30*1000/MyStartTimer;
        //times out,let stepper in,or you pressed 'ok' button
        emit timeOutOK();
        this->close();
    }
    if(CountDownTime/5)
    ui->label_Seconds->setText(QString("%1").arg(CountDownTime/5));
    CountDownTime--;
    //here change the label color
    if(CountDownTime%2)
        ui->label_warning->setStyleSheet(QString::fromUtf8(
       "\n"
       "\n"
       "font: 14pt \"helvetica\" bold;\n"
       "\n"
       "color: rgb(0,255, 0);\n"
       "background:transparent;"));
    else
        ui->label_warning->setStyleSheet(QString::fromUtf8(
       "\n"
       "\n"
       "font: 14pt \"helvetica\" bold;\n"
       "\n"
       "color: rgb(255,0, 0);\n"
       "background:transparent;"));
}

void CountDown::send_signals()
{
    emit(insert_ok());
    this->close();
}
