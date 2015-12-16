#include "backupanddelete.h"
#include "ui_backupanddelete.h"

BackUpandDelete::BackUpandDelete(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::BackUpandDelete)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    ui->pushButton_Delete->setHidden(false);

    timerEvent(0);
    timerEvent_Id=startTimer(1000);
    ui->Label_UserName->setText(inputUserName);
    warning=new Mywarning(this);
    warning->move((800-warning->geometry().width())/2,(480-warning->geometry().height())/2);
    warning->hide();

    ui->pushButton_LogOut->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_BackUp->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Delete->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Exit->setFocusPolicy(Qt::NoFocus);
    connect(ui->pushButton_BackUp,SIGNAL(clicked()),this,SLOT(BackUpData()));
    connect(ui->pushButton_Delete,SIGNAL(clicked()),this,SLOT(DeleteData()));
    connect(ui->pushButton_Exit,SIGNAL(clicked()),this,SLOT(home()));
    connect(ui->pushButton_LogOut,SIGNAL(clicked()),this,SLOT(LogOut()));
}

BackUpandDelete::~BackUpandDelete()
{
    delete ui;
}

bool BackUpandDelete::BackUPandDelete()
{
    //check out the rest space of the disk file
    // if no enough space,alert Users to backUp and delete
    //use linux c
    struct statfs DiskInfo;
    QMessageBox warning;
    warning.setToolTip("warning");
    warning.setText("Space is not enough!\nBackup the data,then delete it\nAre you sure!");
    warning.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    char *path="/linear";
    statfs(path,&DiskInfo);
    float percent=float(DiskInfo.f_bfree)/float(DiskInfo.f_blocks);
    //when you Pressed Save button,check out the Used percent
    if((1-percent)>0.65)
    {
        if(warning.exec()==QMessageBox::Yes)
            return true;
    }
    else
        return false;
    //here create a filedoalog to alert users to backup and delete file
}

void BackUpandDelete::DeleteData()
{
    Deleteupwarning=new warning_two(this);
    Deleteupwarning->move((800-Deleteupwarning->geometry().width())/2,(480-Deleteupwarning->geometry().height())/2);
    Deleteupwarning->setText(QString::fromUtf8("删除数据？"));
    connect(Deleteupwarning,SIGNAL(Pressed_Yes()),this,SLOT(unlinkdata()));
    disconnect(Deleteupwarning,SIGNAL(Pressed_Yes()));

    Deleteupwarning->exec();
}

void BackUpandDelete::BackUpData()
{
    Backupwarning=new warning_two(this);
    Backupwarning->move((800-Backupwarning->geometry().width())/2,(480-Backupwarning->geometry().height())/2);

    connect(Backupwarning,SIGNAL(Pressed_Yes()),this,SLOT(ReallyBackup()));
    disconnect(Backupwarning,SIGNAL(Pressed_Yes()));
    Backupwarning->setText(QString::fromUtf8("备份数据？"));
    Backupwarning->exec();
}

void BackUpandDelete::timerEvent(QTimerEvent *e)
{
    QDateTime datatime=QDateTime::currentDateTime();
    QString Stringcurrentime=datatime.time().toString();
    QDateTime CurrentDate=QDateTime::currentDateTime();
    QString YearDate=CurrentDate.toString("yyyy.MM.dd");
    ui->label_Time->setText(YearDate);
    ui->label_date->setText(Stringcurrentime);
}
void BackUpandDelete::LogOut()
{
    qApp->exit(773);
}

void BackUpandDelete::unlinkdata()
{
    if(unlink("Result/Result.txt")<0)
    {
        warning->setText(QString::fromUtf8("信息删除失败!"));
        warning->show();
    } else {
        warning->setRight();
        warning->setText(QString::fromUtf8("信息删除成功!"));
        warning->show();
        ui->pushButton_Delete->setEnabled(false);
    }
}

void BackUpandDelete::home()
{
    this->hide();
}

void BackUpandDelete::ReallyBackup()
{
    ui->pushButton_Delete->setEnabled(true);
    QFile copyFile;
    QString SourceFile="Result/Result.txt";
    //judge if it has sdcard
    QFileInfo sdcard("/media/mmcblk0p1");
    if(!sdcard.exists())
    {
        warning->setText(QString::fromUtf8("请插入SD卡"));
        warning->show();
        return ;
    }
    mkdir("/media/mmcblk0p1/BackUp",S_IRWXU);
    QString DestFile="/media/mmcblk0p1/BackUp/Result.csv";
    bool ret;
    ret=copyFile.copy(SourceFile,DestFile);
    if(!ret)
    {
        warning->setText(QString::fromUtf8("备份失败"));
        warning->show();
    }
    else
    {
        warning->setRight();
        warning->setText(QString::fromUtf8("备份成功"));
        warning->show();
    }
}

void BackUpandDelete::SetSoftWareVersion(QString Version,QString username)
{
    ui->Label_UserName->setText(username);
    ui->Label_SoftWareVersion->setText(Version);
    ui->Label_SoftWareVersion->show();
}
