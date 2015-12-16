#include "testshow.h"
#include "ui_testshow.h"
#include "math.h"
//int MYCall_Back(void *pdata,int argc,char *value[],char *name[]);
TestShow::TestShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestShow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    page = -1;
    //初始化显示框
    model=new QStandardItemModel(this);
    model->setColumnCount(1);//ID option name age gender  Result time,add some ,totally 11
    ui->lineEdit_Year->setValidator(new QIntValidator(0,10000,this));
    ui->lineEdit_Month->setValidator(new QIntValidator(0,12,this));
    ui->lineEdit_Day->setValidator(new QIntValidator(0,30,this));
    ui->pushButton_Exit->setFocusPolicy(Qt::NoFocus);
    connect(ui->pushButton_up,SIGNAL(clicked()),this,SLOT(page_up()));
    connect(ui->pushButton_down,SIGNAL(clicked()),this,SLOT(page_down()));
    connect(ui->pushButton_go,SIGNAL(clicked()),this,SLOT(page_go()));
    connect(ui->pushButton_Exit,SIGNAL(clicked()),this,SLOT(home()));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(setPrintTrue(QModelIndex)));
    connect(ui->pushButton_print,SIGNAL(clicked()),this,SLOT(PrintItem()));
    connect(ui->pushButton_Search,SIGNAL(clicked()),this,SLOT(find_info()));
    connect(ui->pushButton_LogOut,SIGNAL(clicked()),this,SLOT(logout()));
    connect(ui->pushButton_Edit,SIGNAL(clicked()),this,SLOT(view_clicked()));
    QRegExp rx("^[0-9]{1,10}$");//这个10就是最大长度
    QValidator *validator = new QRegExpValidator(rx,0);
    ui->lineEdit_Page->setValidator(validator);
    dateEdit=new QDateEdit(this);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setGeometry(QRect(413,90,163,29));
    dateEdit->setAlignment(Qt::AlignCenter);
    dateEdit->setDate(QDateTime::currentDateTime().date());
}

void TestShow::initComboxname()
{
    ui->pushButton_down->setVisible(false);
    ui->pushButton_go->setVisible(false);
    ui->pushButton_up->setVisible(false);
    ui->lineEdit_Page->setVisible(false);
    ui->label_page->setVisible(false);
    ui->comboBox_pro->clear();

    ui->comboBox_pro->addItems(SQLITE::sql->Select(1));
}

TestShow::~TestShow()
{
    ::close(seriport_print);
    delete ui;
}

void TestShow::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void TestShow::home()
{
    //    close();
    //    delete this;
    list.clear();
    model->clear();//clear the info of lasttime
    page=-1;
    //在隐藏之前进行一些必要的初始化
    this->hide();
}
void TestShow::timerEvent(QTimerEvent *e) //update time,make time moving
{
    QDateTime datatime=QDateTime::currentDateTime();
    QString Stringcurrentime=datatime.time().toString();
    QDateTime CurrentDate=QDateTime::currentDateTime();
    QString YearDate=CurrentDate.toString("yyyy.MM.dd");
    ui->label_date->setText(YearDate);
    ui->label_Time->setText(Stringcurrentime);
}

void TestShow::find_info()
{//搜索
    //初始化
    list.clear();
    page=-1;
    model->clear();//clear the info of lasttime
    ui->label_page->setText("");

    list=SQLITE::sql->Select(ui->comboBox_pro->currentText(),ui->lineEdit_Name->text(),ui->lineEdit_ID->text(),dateEdit->text(),0);
    page=0;
    pageTotal=ceil(SQLITE::sql->getRows(ui->comboBox_pro->currentText(),ui->lineEdit_Name->text(),ui->lineEdit_ID->text(),dateEdit->text())/10.0);
    if(list.isEmpty())
    {
        warning->setText(QString::fromUtf8("记录不存在"));
        warning->exec();
        ui->pushButton_print->setEnabled(false);
        ui->pushButton_Edit->setEnabled(false);
    }else{
        showResult(list);
        ui->pushButton_down->setVisible(true);
        ui->pushButton_go->setVisible(true);
        ui->pushButton_up->setVisible(true);
        ui->label_page->setVisible(true);
        ui->lineEdit_Page->setVisible(true);
    }
}

void TestShow::showResult(QList<QString> List)
{//翻页，每次显示十条,direction=0上翻；direction=1下翻页
    QStringList lists ;
    int i=0 ;
    int row = 0;

    model->clear();//清空选项

    ui->label_page->setText(QString::number(page+1)+"/"+QString::number(pageTotal));

    for(i=0;i<List.length();i++){
        lists=List[i].split(",");
        initialize(row++,lists);
    }
}

void TestShow::initialize(int row,QStringList message)
{//显示出来结果
    addtop();
    for(int i=0;i<11;i++){
        QStandardItem *item_Result=new QStandardItem();;//仅仅用在显示结果的地方
        item_Result->setTextAlignment(Qt::AlignCenter);//初始化搜索结果的显示列表
        item_Result->setText(message[i]);
        item_Result->setEditable(false);
        model->setItem(row,i,item_Result);
    }
}

void TestShow::PrintItem()
{
    seriport_print =openSerialPort("/dev/ttyO4");
    if (seriport_print < 0) {
        warning->setText(QString::fromUtf8("打开串口失败"));
        warning->exec();
    }
    char *print;
    print=(char *)malloc(500*sizeof(char));

    patientID=model->item(ui->tableView->currentIndex().row(),0)->text();
    Testoption=model->item(ui->tableView->currentIndex().row(),4)->text();
    PatientName=model->item(ui->tableView->currentIndex().row(),1)->text();
    PatientAge=model->item(ui->tableView->currentIndex().row(),2)->text();
    PatientGender=model->item(ui->tableView->currentIndex().row(),3)->text();
    CurrentDate=model->item(ui->tableView->currentIndex().row(),8)->text();
    testtime=model->item(ui->tableView->currentIndex().row(),9)->text();
    inputUser=model->item(ui->tableView->currentIndex().row(),10)->text();
    serialNumber=model->item(ui->tableView->currentIndex().row(),0)->text();
    data=model->item(ui->tableView->currentIndex().row(),6)->text();
    FinalResult=model->item(ui->tableView->currentIndex().row(),7)->text();
    Bach_ID=model->item(ui->tableView->currentIndex().row(),5)->text();

    int printlength;
    printf("Count!\n");
    if(Testoption.isEmpty() && serialNumber.isEmpty() && Bach_ID.isEmpty())
    {
        warning->setText(QString::fromUtf8("没有选中用户！"));
        warning->exec();
    }
    printf("2\n");

    strcpy(print,"\nMagiCare RTR-FS100 打印报告");
    strcat(print,"\n");
    strcat(print,"\n");
    strcat(print,"\n签名：");
    strcat(print,"\n检测结果(a.u.)：");
    strcat(print,FinalResult.toUtf8().data());
    strcat(print,"\n检测项目：");
    strcat(print,Testoption.toUtf8().data());
    strcat(print,"\n年龄：");
    strcat(print,PatientAge.toUtf8().data());
    strcat(print,"\n性别：");
    strcat(print,PatientGender.toUtf8().data());
    strcat(print,"\n姓名：");
    strcat(print,PatientName.toUtf8().data());
    strcat(print,"\n账号：");
    strcat(print,patientID.toUtf8().data());
    strcat(print,"\n批次号：");
    strcat(print,Bach_ID.toUtf8().data());//no need to show Serialnumber
    strcat(print,"\n时间：");
    strcat(print,testtime.toUtf8().data());
    strcat(print,"\n日期：");
    strcat(print,CurrentDate.toUtf8().data());
    strcat(print,"\n");
    strcat(print,"\n");
    strcat(print,"\n");
    strcat(print,"\n");

    printlength=strlen(print);
    printf("%s",print);
    miniPrintWriteUnicode(seriport_print,print);
    ::close(seriport_print);
}

void TestShow::addtop()
{
    QStringList namelables;

    model->setColumnCount(11);
    ui->tableView->setAutoFillBackground(true);
    //in text file its sequence is this

    ui->tableView->setColumnWidth(0,80);
    ui->tableView->setColumnWidth(1,65);
    ui->tableView->setColumnWidth(2,70);
    ui->tableView->setColumnWidth(3,50);
    ui->tableView->setColumnWidth(4,50);
    ui->tableView->setColumnWidth(5,80);
    ui->tableView->setColumnWidth(6,80);
    ui->tableView->setColumnWidth(7,70);
    ui->tableView->setColumnWidth(8,75);
    ui->tableView->setColumnWidth(9,80);
    ui->tableView->setColumnWidth(10,80);


    namelables<<QString::fromUtf8("账号")<<QString::fromUtf8("姓名")
             <<QString::fromUtf8("年龄")<<QString::fromUtf8("性别")
            <<QString::fromUtf8("类型")<<QString::fromUtf8("批次号")
           <<QString::fromUtf8("定量值")<<QString::fromUtf8("结果")
          <<QString::fromUtf8("日期")<<QString::fromUtf8("时间")
         <<QString::fromUtf8("操作员");
    model->setHorizontalHeaderLabels(namelables);
    model->horizontalHeaderItem(0)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(1)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(2)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(3)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(4)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(5)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(6)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(7)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(8)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(9)->setForeground(QBrush(QColor(0,0,0)));
    model->horizontalHeaderItem(10)->setForeground(QBrush(QColor(0,0,0)));

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setShowGrid(true);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}
void TestShow::setPrintTrue(QModelIndex a)
{
    ui->pushButton_print->setEnabled(true);
    ui->pushButton_Edit->setEnabled(true);
}

int TestShow::openSerialPort(const char *Seri_dev)
{
    int s_port = -1;
    s_port = ::open(Seri_dev, O_RDWR|O_NONBLOCK);
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

//print command
void TestShow::miniPrintWriteAscall(int fd,char data)
{
    //know the detail of DWORD and UCHAR
    DWORD dwWrote = 0;
    UCHAR szPrint[128] = {0};
    if(fd==-1) printf("error!\n");

    memset(szPrint, 0, sizeof(szPrint));
    szPrint[0] = 0x1b;
    szPrint[1] = 0x36;
    write(fd,szPrint,2);
    dwWrote = 0;
    write(fd,&data,sizeof(char));
}

void TestShow::view_clicked()
{
    QModelIndexList modellist = ui->tableView->selectionModel()->selectedRows();
    getCurrentData();
    //here set the values
    QString OperatorName=*MyInputUsername;
    label_Data->setText(data);
    label_T->setText(Testoption);
    label_G->setText(PatientGender);
    label_Id->setText(patientID);
    label_Batch->setText(Bach_ID);
    label_Age->setText(PatientAge);
    label_Name->setText(PatientName);
    label_operator->setText(OperatorName);
    label_Time->setText(CurrentDate);

    if(!frame->isVisible())
    {
        ui->tableView->hide();
        frame->show();
    }
    else
    {
        frame->hide();
        ui->tableView->show();
    }
}

void TestShow::initShowResult()
{
    frame = new QFrame(this);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->resize(630,225);
    frame->move((800-frame->width())/2,145);
    frame->setStyleSheet(QString::fromUtf8("QFrame[objectName=\"frame\"]\n"
                                           "{\n"
                                           "	background-image: url(:/ui/view_bg.png);\n"
                                           "background-color:transparent;\n"
                                           "border-radius:10px;\n"
                                           "}"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    CloseButton = new QPushButton(frame);
    CloseButton->setObjectName(QString::fromUtf8("CloseButton"));
    CloseButton->setGeometry(QRect(545, 11, 51, 30));
    CloseButton->setStyleSheet(QString::fromUtf8("background-image: url(:/ui/showDetails/X.png);\n"
                                                 "border:none;"));
    label_Data = new QLabel(frame);
    label_Data->setObjectName(QString::fromUtf8("label_Data"));
    label_Data->setGeometry(QRect(290, 28, 101, 21));
    label_Data->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                "border-radius:10px;"));
    label_T = new QLabel(frame);
    label_T->setObjectName(QString::fromUtf8("label_T"));
    label_T->setGeometry(QRect(180, 70, 111, 21));
    label_T->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                             "border-radius:10px;"));
    label_Id = new QLabel(frame);
    label_Id->setObjectName(QString::fromUtf8("label_Id"));
    label_Id->setGeometry(QRect(180, 104, 111, 21));
    label_Id->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                              "border-radius:10px;"));
    label_Name = new QLabel(frame);
    label_Name->setObjectName(QString::fromUtf8("label_Name"));
    label_Name->setGeometry(QRect(180, 137, 111, 21));
    label_Name->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                "border-radius:10px;"));
    label_Time = new QLabel(frame);
    label_Time->setObjectName(QString::fromUtf8("label_Time"));
    label_Time->setGeometry(QRect(414, 172, 111, 21));
    label_Time->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                "border-radius:10px;"));
    label_G = new QLabel(frame);
    label_G->setObjectName(QString::fromUtf8("label_G"));
    label_G->setGeometry(QRect(413, 71, 111, 21));
    label_G->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                             "border-radius:10px;"));
    label_Batch = new QLabel(frame);
    label_Batch->setObjectName(QString::fromUtf8("label_Batch"));
    label_Batch->setGeometry(QRect(180, 172, 111, 21));
    label_Batch->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                 "border-radius:10px;"));
    label_Age = new QLabel(frame);
    label_Age->setObjectName(QString::fromUtf8("label_Age"));
    label_Age->setGeometry(QRect(415, 102, 111, 21));
    label_Age->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                               "border-radius:10px;"));
    label_operator = new QLabel(frame);
    label_operator->setObjectName(QString::fromUtf8("label_operator"));
    label_operator->setGeometry(QRect(414, 138, 111, 21));
    label_operator->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                    "border-radius:10px;"));
    connect(CloseButton,SIGNAL(clicked()),frame,SLOT(close()));
    connect(CloseButton,SIGNAL(clicked()),ui->tableView,SLOT(show()));
}

void TestShow::getCurrentData()
{
    patientID=model->item(ui->tableView->currentIndex().row(),0)->text();
    Testoption=model->item(ui->tableView->currentIndex().row(),4)->text();
    PatientName=model->item(ui->tableView->currentIndex().row(),1)->text();
    PatientAge=model->item(ui->tableView->currentIndex().row(),2)->text();
    PatientGender=model->item(ui->tableView->currentIndex().row(),3)->text();
    CurrentDate=model->item(ui->tableView->currentIndex().row(),8)->text();
    testtime=model->item(ui->tableView->currentIndex().row(),9)->text();
    inputUser=model->item(ui->tableView->currentIndex().row(),10)->text();
    serialNumber=model->item(ui->tableView->currentIndex().row(),0)->text();
    data=model->item(ui->tableView->currentIndex().row(),6)->text();
    FinalResult=model->item(ui->tableView->currentIndex().row(),7)->text();
    Bach_ID=model->item(ui->tableView->currentIndex().row(),5)->text();
}

void TestShow::setTableViewBlankColor()
{
    int Count=11;
    int i=0;
    QStandardItem *item=new QStandardItem();
    QColor color(56,150,32,80);
    QBrush brush(color,Qt::SolidPattern);
    item->setBackground(brush);
    item->setEditable(false);

    for(i=0;i<Count;i++)
    {
        model->setItem(i,0,item);
    }
}

void TestShow::logout()
{
    //close all windows
    //then create a new main process
    qApp->exit(773);
}

void TestShow::initiatemove()
{
    //set the home button
    //here set the label
    initComboxname();
    warning=new Mywarning(this);
    warning->move((800-warning->width())/2,(480-warning->height())/2);

    ui->pushButton_print->setEnabled(false);
    ui->pushButton_Edit->setEnabled(false);

    timerEvent(0);
    timerEvent_Id=startTimer(100);

    QDateTime CurrentDate=QDateTime::currentDateTime();
    QString YearDate=CurrentDate.toString("yyyy:MM:dd");
    ui->label_date->setText(YearDate);
    ui->Label_UserName->setText(*MyInputUsername);
    this->setObjectName("TestShow");
    initShowResult();

    ui->pushButton_Edit->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_print->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Search->setFocusPolicy(Qt::NoFocus);
}
void TestShow::miniPrintWriteUnicode(int fd,char *pwchello)
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
int TestShow::code_convert(char *from_charset,char *to_charset,char *inbuf,unsigned int inlen,char *outbuf,unsigned int outlen)
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

int TestShow::u2g(char *inbuf,int inlen,char *outbuf,int outlen)                        //UNICODEÂë×ªÎªGB2312Âë
{
    return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}

void TestShow::SetSoftWareVersion(QString Version)
{
    ui->Label_SoftWareVersion->setText(Version);
    ui->Label_SoftWareVersion->show();
}

void TestShow::page_up()
{//上翻页
    if(page>0)
    {
        page-=1;
        list.clear();
        list=SQLITE::sql->Select(ui->comboBox_pro->currentText(),ui->lineEdit_Name->text(),ui->lineEdit_ID->text(),dateEdit->text(),page);
        showResult(list);
    }
}

void TestShow::page_down()
{//下翻页
    if(page<pageTotal-1)
    {
        list.clear();
        page+=1;
        list=SQLITE::sql->Select(ui->comboBox_pro->currentText(),ui->lineEdit_Name->text(),ui->lineEdit_ID->text(),dateEdit->text(),page);
        showResult(list);
    }
}

void TestShow::page_go()
{//前进
    int temp =0;//增加变量不直接对page进行操作
    temp=ui->lineEdit_Page->text().toInt();
    if(temp>pageTotal || temp==0){//输入0或超过总的页码，就不再进去
        warning->setText(QString::fromUtf8("请检查输入"));
        warning->exec();
        ui->lineEdit_Page->setFocus();
    }else{
        ui->lineEdit_Page->setText("");
        list.clear();
        page=temp-1 ;
        list=SQLITE::sql->Select(ui->comboBox_pro->currentText(),ui->lineEdit_Name->text(),ui->lineEdit_ID->text(),dateEdit->text(),page);
        showResult(list);
    }
}

void TestShow::on_pushButton_clicked()
{
    list=SQLITE::sql->Select("","","","",100);
    qDebug()<<list;
}
