#include "dialog.h"
#include "ui_dialog.h"

extern SQL *sql;
Dialog::Dialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->pushButton_checkshow->setFlat(true);
    ui->pushButton_test->setFlat(true);
    ui->pushButton_Set->setFlat(true);
    ui->pushButton_DeleteBackup->setFlat(true);
    ui->pushButton_checkshow->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_test->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Set->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_DeleteBackup->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_LogOut->setFocusPolicy(Qt::NoFocus);

    timerEvent(0);
    timerEvent_Id=startTimer(100);

    judgeadmin();

    //get current date time
    QDateTime CurrentDate=QDateTime::currentDateTime();
    QString YearDate=CurrentDate.toString("yyyy:MM:dd");
    ui->label_date->setText(YearDate);
    //Here Let QLabel be son of QPushbutton,so you pressed Label ,button effected
    ui->label_Delete->setParent(ui->pushButton_DeleteBackup);
    ui->label_Delete->setGeometry(QRect(50,40,111,101));
    ui->label_Search->setParent(ui->pushButton_test);
    ui->label_Search->setGeometry(QRect(50,40,111,101));
    ui->label_Test->setParent(ui->pushButton_checkshow);
    ui->label_Test->setGeometry(QRect(50,40,111,101));
    ui->label_SET->setParent(ui->pushButton_Set);
    ui->label_SET->setGeometry(QRect(50,40,111,101));
    //****************************************************
    ui->Label_UserName->setText(*MyInputUsername);
    connect(ui->pushButton_checkshow,SIGNAL(clicked()),this,SLOT(showCheck()));
    connect(ui->pushButton_test,SIGNAL(clicked()),this,SLOT(showTest()));
    connect(ui->pushButton_Set,SIGNAL(clicked()),this,SLOT(showSet()));
    connect(ui->pushButton_DeleteBackup,SIGNAL(clicked()),this,SLOT(showBackupandDelete()));
    connect(ui->pushButton_LogOut,SIGNAL(clicked()),this,SLOT(logout()));

    //wanghai add
//    this->openPort(); //打开串口
    commSerial = new CommSerial(&serialport);

    connect(&serialport, SIGNAL(readyRead()), this, SLOT(handleReadyRead())); //监听串口
    connect(this, SIGNAL(ReceiveCommand(QByteArray)), this, SLOT(handleCommand(QByteArray)));
    connect(this, SIGNAL(ClearArray()), this, SLOT(handleClearArray()));
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::showCheck()
{
    checkshow=new CheckShow(this);
    checkshow->SetSoftWareVersion(SoftWareVersion,*MyInputUsername);
    checkshow->initshow();
    checkshow->show();
    checkshow->inputUserName=inputUserName;
}

void Dialog::showTest()
{
    testshow->SetSoftWareVersion(SoftWareVersion);
    testshow->show();
    testshow->initiatemove();
}

void Dialog::showSet()
{
    setshow->SetSoftWareVersion(SoftWareVersion,*MyInputUsername);
    setshow->show();
}

void Dialog::timerEvent(QTimerEvent *e)
{
    QDateTime datatime=QDateTime::currentDateTime();
    QString Stringcurrentime=datatime.time().toString();
    QDateTime CurrentDate=QDateTime::currentDateTime();
    QString YearDate=CurrentDate.toString("yyyy.MM.dd");
    ui->label_Time->setText(YearDate);
    ui->label_date->setText(Stringcurrentime);
}
void Dialog::showoption()
{
    OptionShow=new CheckOption(this);
    OptionShow->show();
}
void Dialog::close()
{
    exit(1);
}

bool Dialog::judgeadmin()
{
    qDebug()<<*MyInputUsername;
    if(SQLITE::sql->Select(*MyInputUsername,"",1)=="1"||SQLITE::sql->Select(*MyInputUsername,"",1)=="2")
    {//判断是不是管理员
        ui->pushButton_Set->setEnabled(true);
        ui->label_SET->setEnabled(true);
        ui->pushButton_DeleteBackup->setEnabled(true);
    }else{
        ui->pushButton_Set->setEnabled(false);
        ui->label_SET->setEnabled(false);
        ui->pushButton_DeleteBackup->setEnabled(false);
    }
    return true;
}


void Dialog::showBackupandDelete()
{
    backupDelete->SetSoftWareVersion(SoftWareVersion,*MyInputUsername);
    backupDelete->show();
}

void Dialog::logout()
{
    qApp->exit(773);
}
void Dialog::setSoftWareVersion(QString currentSoftWareVersion)
{
    SoftWareVersion=currentSoftWareVersion;
    ui->Label_SoftWareVersion->raise();
    ui->Label_SoftWareVersion->setText(SoftWareVersion);
    ui->Label_SoftWareVersion->show();
}


//wanghai add below

/*******************************************
 * 功  能：打开串口
 * *******************************************/
void Dialog::openPort()
{
    serialport.setPortName(PORTNAME);
    if(!serialport.open(QIODevice::ReadWrite))
        qDebug() <<  tr("Failed to open port %1").arg(PORTNAME);
}

/*******************************************
 * 功  能：接收槽函数
 * *******************************************/
void Dialog::handleReadyRead()
{
    commSerial->RecvText(hostString);
    if(!hostString.isEmpty())
    {
        m_readData = hostString.toAscii();
        emit ReceiveCommand(m_readData);  //发射命令处理信号
    }
    qDebug() << "hostString: " << hostString;
}

/*******************************************
 * 功  能：接收到的命令处理
 * *******************************************/
void Dialog::handleCommand(QByteArray command)
{
    if(command.at(0) == 'R')
    {
       qDebug() << "upload command";
       switch (command.at(1)) {
       case 'a':
           //upload all data;
           uploadAllData();
           break;
       case 'b':
       {
           //upload data that time is from fromdate to enddate
           QString fromDate, fromTime, toDate, toTime;
           for(int i=2; i<=9; i++)
           {
               fromDate.append(command.at(i));
               toDate.append(command.at(i+14));
           }
           for(int j=10; j<=15; j++)
           {
               fromTime.append(command.at(j));
               toTime.append(command.at(j+14));
           }
           uploadTilNowData(fromDate, fromTime, toDate, toTime);
           break;
       }
       case 'c':
       {
           //upload data that time is from fromdate to now
           QString startDate, startTime, nowDate, nowTime;
           for(int i=2; i<=9; i++)
           {
               startDate.append(command.at(i));
               nowDate.append(command.at(i+14));
           }
           for(int j=10; j<=15; j++)
           {
               startTime.append(command.at(j));
               nowTime.append(command.at(j+14));
           }
           qDebug() << "startDate: " << startDate << " startTime: " << startTime;
           qDebug() << "nowDate: " << nowDate << "nowTime: " << nowTime;
           uploadTilNowData(startDate, startTime, nowDate, nowTime);
           break;
       }
       case 'd':
           //upload specified number data

           break;
       default:
           break;
       }
    }
    else if(command.size() == 1 && command.at(0) == 'E')
    {
        qDebug() << "empty data command";
        emptyAllData();
    }
}


/*******************************************
 * 功  能：命令数组清空
 * *******************************************/
void Dialog::handleClearArray()
{
    m_readData.clear();
}

/*******************************************
 * 功  能：上传所有数据
 * 备  注：1 sql查询
 *        2 串口发送查询结果
 *        3 发射信号
 * *******************************************/
void Dialog::uploadAllData()
{
    qDebug() << "in mainwidow.cpp line128 uploadAllData() function";
    SQLITE::sql->DBOpen();
    allDataList = SQLITE::sql->selectAllRecord();
    QString str;
    result.clear();
    foreach (str, allDataList) {
        result += str;
        qDebug() << result;
        //serialPort.write(str.toAscii());
    }
    commSerial->SendText(result);
    //数据处理完后,发送清空m_readData数组信号，以便从串口接收到下一个命令
    emit ClearArray();
}

/*******************************************
 * 功  能：上传某个时间点到现在的所有数据
 * 备  注：1 sql查询
 *        2 串口发送查询结果
 *        3 发射信号
 * *******************************************/
void Dialog::uploadTilNowData(QString fromDate, QString fromTime, QString toDate, QString toTime)
{
    qDebug() << "in uploadTilNowData() function";
    SQLITE::sql->DBOpen();
    tilNowDataList = SQLITE::sql->selectTilNowRecord(fromDate, fromTime, toDate, toTime);
    QString str;
    result.clear();
    foreach (str, allDataList) {
        result += str;
        qDebug() << result;
        //serialPort.write(str.toAscii());
    }
    commSerial->SendText(result);
    //数据处理完后,发送清空m_readData数组信号，以便从串口接收到下一个命令
    emit ClearArray();
}

/*******************************************
 * 功  能：数据清空
 * 备  注：1 sql语句清空
 *        2 发射信号
 * *******************************************/
void Dialog::emptyAllData()
{
    qDebug() << "in emptyAllData() function";
    if(!SQLITE::sql->emptyRecordTable())
        qDebug() << "Faied to empty data";
    else
        QMessageBox::about(this, "Empty data", "Congratulations! You have empty all data successfully!");
    //数据处理完后,发送清空m_readData数组信号，以便从串口接收到下一个命令
    emit ClearArray();
}

