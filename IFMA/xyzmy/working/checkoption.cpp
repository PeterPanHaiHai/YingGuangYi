//when you pressed option button
//you choose 'formula' by project and bachid  'CRP+BACHID.txt'

//add a print arrangement,(0-10ng/ml)
//DATE:2013-06-08
//INTRODUCTION:
//in CheckOption class ,when you test in project and Bach ID,if in flash,there has no such formula ,warning

#include "checkoption.h"
#include "ui_checkoption.h"
typedef unsigned long DWORD;
typedef unsigned char UCHAR;
typedef unsigned short WCHAR;
#define BUFFERSIZE 1024

//extern "C" void miniPrintWriteUnicode(int fd,char *pwchello);
CheckOption::CheckOption(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CheckOption)
{
    ui->setupUi(this);
    Key=false;
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    warning=new Mywarning;
}

CheckOption::~CheckOption()
{
    delete ui;
}


void CheckOption::home()
{
    Resultfile->close();
}


void CheckOption::acitveButton(QString )
{
    if(ui->lineEdit_ID->isModified()&&ui->lineEdit_Name->isModified())
        ui->pushButton_OK->setEnabled(true);
}

//press enter,then send instruction to linear
int CheckOption::OK()
{
    Key=true;

    CurrentDate=QDateTime::currentDateTime();
    YearDate=CurrentDate.toString("yyyy:MM:dd");
    patientID=ui->lineEdit_ID->text();
    TestType=ui->comboBox->currentText();
    TypeItem=ui->comboBox->currentIndex();
    PatientName=ui->lineEdit_Name->text();
    PatientAge=ui->lineEdit_Age->text();
    PatientGender=ui->comboBox_gender->currentText();
    GenderItem=ui->comboBox_gender->currentIndex();

    emit(Pressed_ok(true));
    linearStarted.fill('A');
    if(CheckIfhasFormula())
        close();
    return 1;
}

int CheckOption::code_convert(char *from_charset,char *to_charset,char *inbuf,unsigned int inlen,char *outbuf,unsigned int outlen)
{
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);
    if (cd==0) return -1;
    memset(outbuf,0,outlen);
    if (iconv(cd,pin,&inlen,pout,&outlen)==-1)
        return -1;
    iconv_close(cd);
    return 0;
}

int CheckOption::u2g(char *inbuf,int inlen,char *outbuf,int outlen)                        //UNICODEÂë×ªÎªGB2312Âë
{
    return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}

void CheckOption::ResultSave()
{
    //save the serialnum in text file,so it's infomatioin will not droped when power off
    //save the file to a database;
    int i=0;
    update();
    emit shouldupdate();
    weatherSaved=true;//pressed save ,then weatherSaved true
    //serial number should add when we save the data
    //At here,read the serialnumber from text file
    QFile serialnumberfile("serialnumber");
    if(serialnumberfile.open(QIODevice::ReadOnly|QIODevice::WriteOnly))
    {
        QTextStream stream(&serialnumberfile);
        serialNumber=stream.readAll();//serialNumber contains the serialNumber
        if(serialNumber.length()==0)//at first
        {  //serialNumber.clear();
            serialNumber="000000";
        }
    }
    long serinum=serialNumber.toFloat();

    serinum++;

    const QChar fillChar='0';
    serialNumber=QString("%1").arg(serinum,6,10,fillChar);
    QTextStream seri(&serialnumberfile);
    seri.seek(0);//lseek to the first place
    seri<<serinum;

    savefileText();
}

void CheckOption::printInfo()
{
    update();
    emit shouldupdate();
    seriport_print=openSerialPort("/dev/ttySAC2");
    if (seriport_print < 0) {
        warning->setText("Fail to open serial port!");
        warning->move(200,200);
        warning->show();
    }

    QString Reallyserinumber;
    if(weatherSaved)//if saved;
        Reallyserinumber=serialNumber;
    else
        Reallyserinumber="";
    inputUserName=*MyInputUsername;
    char *print;
    print=(char *)malloc(500*sizeof(char));
    int printlength;
    int i;

    strcpy(print,"\nMagiCare RTR-FS100 打印报告");
    strcat(print,"\n");
    strcat(print,"\n");
    strcat(print,"\n签名：");
    strcat(print,"\n检测结果(a.u.)：");
    strcat(print,data.toUtf8().data());
    strcat(print,"\n检测项目：");
    strcat(print,ui->comboBox->currentText().toUtf8().data());
    strcat(print,"\n年龄：");
    strcat(print,ui->lineEdit_Age->text().toUtf8().data());
    strcat(print,"\n性别：");
    strcat(print,ui->comboBox_gender->currentText().toUtf8().data());
    strcat(print,"\n姓名：");
    strcat(print,ui->lineEdit_Name->text().toUtf8().data());
    strcat(print,"\n检测编号：");
    strcat(print,ui->lineEdit_ID->text().toUtf8().data());
    strcat(print,"\n批次号：");
    strcat(print,ui->lineEdit_Bach->text().toUtf8().data());//no need to show Serialnumber
    strcat(print,"\n时间：");
    strcat(print,CurrentDate.time().toString().toUtf8().data());
    strcat(print,"\n日期：");
    strcat(print,YearDate.toUtf8().data());
    strcat(print,"\n");
    strcat(print,"\n");
    strcat(print,"\n");
    strcat(print,"\n");

    printlength=strlen(print);
    miniPrintWriteUnicode(seriport_print,print);
}

void CheckOption::miniPrintWriteAscall(int fd,char data)
{
    //know the detail of DWORD and UCHAR
    DWORD dwWrote = 0;
    UCHAR szPrint[128] = {0};
    if(fd==-1) perror("error!\n");

    memset(szPrint, 0, sizeof(szPrint));
    szPrint[0] = 0x1b;
    szPrint[1] = 0x36;
    write(fd,szPrint,2);
    dwWrote = 0;
    int back=write(fd,&data,sizeof(char));
}

void CheckOption::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Enter)
    {
        focusNextChild();
    }
}


void CheckOption::createProjecttable()//create a table to include project file
{
    QSqlQuery query;

    query.exec("create table Project (ProjectName varchar primary key)");
    query.exec("insert into login values ('CRP')");
}



void CheckOption::ShowProjectName()
{
    QFile ProjectFile("Project.txt");
    if(ProjectFile.open(QIODevice::ReadOnly)<0)
    {
        warning->setText(QString::fromUtf8("没有项目文件"));
        warning->move(200,200);
        warning->show();
    }
    QTextStream stream(&ProjectFile);
    QString line;
    while(!stream.atEnd())
    {
        line=stream.readLine();//read the txt file line by line
        if(!line.isEmpty())
            ui->comboBox->addItem(line);
    }
    ProjectFile.close();
}


void CheckOption::OptionTestShow()
{
    this->show();
    activateWindow();
    weatherSaved=false;
    ui->pushButton_OK->show();
    ui->pushButton_Cancel->show();
    ui->lineEdit_Age->setText("");
    ui->lineEdit_ID->setText("");
    ui->lineEdit_Name->setText("");
    ui->lineEdit_ID->setFocus();
}

void CheckOption::OptionResultShow()
{
    ui->lineEdit_ID->setText(patientID);
    ui->lineEdit_Name->setText(PatientName);
    ui->comboBox_gender->setItemText(GenderItem,PatientGender);
    ui->lineEdit_Age->setText(PatientAge);
    ui->comboBox->setItemText(TypeItem,TestType);
    ui->lineEdit_ID->setText(patientID);
    ui->lineEdit_Name->setText(PatientName);
    ui->comboBox_gender->setItemText(GenderItem,PatientGender);
    ui->lineEdit_Age->setText(PatientAge);
    ui->comboBox->setItemText(TypeItem,TestType);
    this->show();
}

void CheckOption::SetFinalConsistency(float Consistency)
{
}

bool CheckOption::BackUPandDelete()
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
        // QMessageBox::warning(this,tr("warning"),tr("No enough Space,Please BackUp and Delete"));
    {
        if(warning.exec()==QMessageBox::Yes)
            deleteandBackupfunction();

        return true;
    }
    else
        return false;
    //here create a filedoalog to alert users to backup and delete file
}

void CheckOption::deleteandBackupfunction()
{
    pushButton_BackUp = new QPushButton(this);
    pushButton_BackUp->setText("BackUp");
    pushButton_BackUp->setObjectName(QString::fromUtf8("pushButton_BackUp"));
    pushButton_BackUp->setGeometry(QRect(10, 190, 161, 121));
    pushButton_BackUp->setStyleSheet(QString::fromUtf8("background-image: url(:/pictures/rectButton.png);\n"
                                                       "font-size:26px bold;\n"
                                                       "\n"
                                                       "border-radius:20px;"));
    pushButton_BackUp->show();
    Treeview_BackUp = new QTreeView(this);
    Treeview_BackUp->setObjectName(QString::fromUtf8("frame_BackUp"));
    Treeview_BackUp->setGeometry(QRect(20, 10, 601, 191));
    Treeview_BackUp->setFrameShape(QFrame::StyledPanel);
    Treeview_BackUp->setFrameShadow(QFrame::Raised);
    //frame_BackUp->setStyleSheet("color: rgb(0, 0, 0);");
    Treeview_BackUp->show();
    pushButton_Cancel = new QPushButton(this);
    pushButton_Cancel->setText("Cancel");
    pushButton_Cancel->setObjectName(QString::fromUtf8("pushButton_Cancel"));
    pushButton_Cancel->setGeometry(QRect(470, 190, 161, 121));
    pushButton_Cancel->setStyleSheet(QString::fromUtf8("background-image: url(:/pictures/rectButton.png);\n"
                                                       "font-size:26px bold;\n"
                                                       "\n"
                                                       "border-radius:20px;"));

    pushButton_Cancel->show();
    pushButton_Delete = new QPushButton(this);
    pushButton_Delete->setText("Delete");
    pushButton_Delete->setObjectName(QString::fromUtf8("pushButton_Delete"));
    pushButton_Delete->setGeometry(QRect(245, 190, 161, 121));
    pushButton_Delete->setStyleSheet(QString::fromUtf8("background-image: url(:/pictures/rectButton.png);\n"
                                                       "font-size:26px bold;\n"
                                                       "\n"
                                                       "border-radius:20px;"));
    pushButton_Delete->show();
    connect(pushButton_Cancel,SIGNAL(clicked()),this,SLOT(SwitchToinput()));
    connect(pushButton_Delete,SIGNAL(clicked()),this,SLOT(removeDirectory()));
    connect(pushButton_BackUp,SIGNAL(clicked()),this,SLOT(BackUpData()));
    showDirectories();
}


void CheckOption::SwitchToinput()
{
    Treeview_BackUp->hide();
    pushButton_BackUp->hide();
    pushButton_Cancel->hide();
    pushButton_Delete->hide();
}

void CheckOption::showDirectories()
{
    DModel=new QDirModel;
    DModel->setReadOnly(true);
    DModel->setSorting(QDir::DirsFirst|QDir::IgnoreCase|QDir::Name);
    Treeview_BackUp->setModel(DModel);
    Treeview_BackUp->header()->setStretchLastSection(true);
    Treeview_BackUp->header()->setSortIndicator(0,Qt::AscendingOrder);
    Treeview_BackUp->header()->setSortIndicatorShown(true);
    Treeview_BackUp->header()->setClickable(true);
    Treeview_BackUp->setRootIndex(DModel->index("/linear"));
    QModelIndex index=DModel->index("/linear");
    Treeview_BackUp->expand(index);
    Treeview_BackUp->scrollTo(index);
    Treeview_BackUp->resizeColumnToContents(0);
}

void CheckOption::removeDirectory()
{
    QModelIndex index=Treeview_BackUp->currentIndex();
    if(!index.isValid())
        printf("index is not valid");
    bool ok;
    if(DModel->fileInfo(index).isDir())
    {
        ok=DModel->rmdir(index);
    }
    else
        ok=DModel->remove(index);
    if(!ok)
        QMessageBox::information(this,tr("Remove"),tr("fail to remove %1").arg(DModel->fileName(index)));    
}

void CheckOption::BackUpData()
{
    //when you pressed Backup button ,then software automatically copy a file to SD card
    int count;
    char buf[BUFFERSIZE];
    int sourceFile=::open("/linear/database/database.db",O_RDWR);
    int destFile=::open("/disk/database.db",O_RDWR|O_CREAT);//if not exist,then create
    while((count=::read(sourceFile,buf,BUFFERSIZE))>0)
    {
        if(::write(destFile,buf,count)!=count)
            QMessageBox::about(NULL,"checkoption","write error");
    }
}

void CheckOption::savefileText()
{
    QFile Resultfile("Result/Result.txt");
    if(Resultfile.open(QIODevice::WriteOnly|QIODevice::Append)<0)
    {
        perror("open error!\n");
        QMessageBox::information(this,tr("error"),tr("open Result.txt file error"));
    }
    QTextStream out(&Resultfile);

    CurrentDate=QDateTime::currentDateTime();
    Bach_ID=ui->lineEdit_Bach->text();
    YearDate=CurrentDate.toString("yyyy:MM:dd");
    patientID=ui->lineEdit_ID->text();
    TestType=ui->comboBox->currentText();
    TypeItem=ui->comboBox->currentIndex();
    PatientName=ui->lineEdit_Name->text();
    PatientAge=ui->lineEdit_Age->text();
    PatientGender=ui->comboBox_gender->currentText();
    GenderItem=ui->comboBox_gender->currentIndex();
    bool saved=judgeweatherSaved(patientID);
    if(!saved)
    {
        out<<patientID<<","<<TestType<<","<<PatientName<<","<<PatientAge<<","<<PatientGender<<","<<YearDate<<","<<CurrentDate.time().toString()<<","<<inputUserName<<","<<serialNumber<<","<<data<<","<<Result<<","<<Bach_ID<<endl;

        warning->setText(QString::fromUtf8("信息保存成功"));
        warning->move(200,200);
        warning->show();
        Resultfile.close();
    }
    else
    {
        warning->setText(QString::fromUtf8("信息保存失败"));
        warning->move(200,200);
        warning->show();
    }
}

void CheckOption::openResultFile()
{
    QString saveFilename="Result/Result.txt";
    Resultfile=new QFile(saveFilename);
    if(Resultfile->open(QIODevice::WriteOnly|QIODevice::Append)<0)
        perror("open error!\n");
}
int CheckOption::openSerialPort(const char *Seri_dev)
{
    int s_port = -1;
    s_port = ::open(Seri_dev, O_RDWR|O_NONBLOCK);
    if (s_port < 0) {
        perror("open seriport error");
    }
    termios serialAttr;
    memset(&serialAttr, 0, sizeof serialAttr);
    serialAttr.c_iflag = IGNPAR;
    serialAttr.c_cflag = B9600 | HUPCL | CS8 | CREAD | CLOCAL;
    serialAttr.c_cc[VMIN] = 1;
    tcflush(s_port,TCIFLUSH);
    if (tcsetattr(s_port, TCSANOW, &serialAttr) != 0) {
        return -1;
    }
    return s_port;
}


QString CheckOption::getProjectName()
{
    return ui->comboBox->currentText();
}

QString CheckOption::getBach_IDName()
{
    return ui->lineEdit_Bach->text();
}

void CheckOption::showResult()
{
}


void CheckOption::CancelOption()
{
    emit(Pressed_cancel(false));
    this->close();//just hide not close

}
bool CheckOption::CheckIfhasFormula()
{
    QString CRP_BACHIDNAme;
    CRP_BACHIDNAme="formula/"+ui->comboBox->currentText()+"_"+ui->lineEdit_Bach->text()+".txt";
    QFileInfo formulainfo(CRP_BACHIDNAme);
    if(!formulainfo.exists())
    {
        Mywarning *warning=new Mywarning(this);
        warning->move(100,50);
        warning->setText(QString::fromUtf8("公式不存在"));

        warning->show();
        return false;
    }
    else
        return true;
}
void CheckOption::setData(QString value)
{
    data=value;
}
void CheckOption::initiatemove()
{
    ui->lineEdit_Bach->setText("111");
    ui->lineEdit_Operator->setReadOnly(true);
    weatherSaved=false;//default false
    inputUserName=*MyInputUsername;
    ui->lineEdit_ID->setText("");
    ui->lineEdit_ID->setFocus();//get the focus


    ui->comboBox_gender->addItem("M");
    ui->comboBox_gender->addItem("F");
    ui->lineEdit_ID->setText("");
    if(ui->lineEdit_ID->text().isEmpty())
        ui->pushButton_OK->setEnabled(false);
    ShowProjectName();
    ui->lineEdit_Operator->setText(inputUserName);

    connect(ui->lineEdit_ID,SIGNAL(textChanged(QString)),this,SLOT(acitveButton(QString)));
    connect(ui->lineEdit_Name,SIGNAL(textChanged(QString)),this,SLOT(acitveButton(QString)));
    connect(ui->pushButton_OK,SIGNAL(clicked()),this,SLOT(OK()));
    connect(ui->pushButton_Cancel,SIGNAL(clicked()),this,SLOT(CancelOption()));

    ui->pushButton_Cancel->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_OK->setFocusPolicy(Qt::NoFocus);

}

bool CheckOption::judgeweatherSaved(QString ID)
{
    //when you decide to save a list of message,you should judge weather it have saved before
    QFile ResultFile("Result/Result.txt");
    if(ResultFile.open(QIODevice::ReadOnly)<0)
    {
        warning->setText("No Reuslt.txt file");
        warning->move(200,200);
        warning->show();
    }
    QTextStream stream(&ResultFile);
    QString line;
    QString UserID;
    bool weathersaved=false;
    while(!stream.atEnd())
    {
        line=stream.readLine();//read the txt file line by line
        UserID=line.section(',',0,0);
        if(UserID==ID)
        {
            weathersaved=true;
            break;
        }
    }
    ResultFile.close();
    return weathersaved;
}

//用来打印中文
void CheckOption::miniPrintWriteUnicode(int fd,char *pwchello)
{
    //know the detail of DWORD and UCHAR
    DWORD dwWrote = 0;
    char szPrint[500] = {0};
    if(fd==-1) printf("error!\n");
    char szTmp[500] = {0};
    memset(szPrint, 0, sizeof(szPrint));
    szPrint[0] = 0x1c;
    szPrint[1] = 0x26;
    write(fd,szPrint,2);
    dwWrote = 0;

    memset(szPrint, 0, sizeof(szPrint));
    szPrint[0] = 0x1b;
    szPrint[1] = 0x38;
    szPrint[2] = 0x3;

    write(fd,szPrint,3);

    memset(szPrint, 0, sizeof(szPrint));
    memset(szTmp, 0, sizeof(szTmp));

    int i=1;
    i=u2g(pwchello,500,szTmp,sizeof(szTmp));//Here DID not work
    sprintf(szPrint, "%s", szTmp);
    write(fd,szPrint,sizeof(szPrint));
    memset(szPrint, 0, sizeof(szPrint));
    szPrint[0] = 0x1c;
    szPrint[1] = 0x2e;
    write(fd,szPrint,2);
}
