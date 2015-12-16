#include "setshow.h"
#include "ui_setshow.h"
//here some special set for admin,ou can change admin's pwd,while you can not delete it

SetShow::SetShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetShow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    superbutton=new QPushButton(this);
    superbutton->setText("");
    superbutton->hide();
    timerEvent(0);
    timerEvent_Id=startTimer(100);
    warning=new Mywarning;
    warning->move((800-warning->width())/2,(480-warning->height())/2);

    ui->pushButton_Calibrate->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Exit->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Project->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Time->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_TouchScreenCalibrate->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_User->setFocusPolicy(Qt::NoFocus);
    ui->label_Calibrate->setParent(ui->pushButton_Calibrate);
    ui->label_Calibrate->move((ui->pushButton_Calibrate->width()-ui->label_Calibrate->width())/2,20);
    ui->label_ProjectSet->setParent(ui->pushButton_Project);
    ui->label_ProjectSet->move((ui->pushButton_Project->width()-ui->label_ProjectSet->width())/2,20);
    ui->label_TimeSet->setParent(ui->pushButton_Time);
    ui->label_TimeSet->move((ui->pushButton_Time->width()-ui->label_TimeSet->width())/2,20);
    ui->label_TScreenSet->setParent(ui->pushButton_TouchScreenCalibrate);
    ui->label_TScreenSet->move((ui->pushButton_TouchScreenCalibrate->width()-ui->label_TScreenSet->width())/2,20);
    ui->label_UserSet->setParent(ui->pushButton_User);
    ui->label_UserSet->move((ui->pushButton_User->width()-ui->label_UserSet->width())/2,20);
    ui->pushButton_Exit->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_LogOut->setFocusPolicy(Qt::NoFocus);

    initiatemove();

    connect(superbutton,SIGNAL(clicked()),this,SLOT(superBehavior()));
    connect(ui->pushButton_Exit,SIGNAL(clicked()),this,SLOT(home()));
    connect(ui->pushButton_User,SIGNAL(clicked()),this,SLOT(initShowUser()));
    connect(ui->pushButton_Time,SIGNAL(clicked()),this,SLOT(showTimeSet()));
    connect(ui->pushButton_Project,SIGNAL(clicked()),this,SLOT(initShowProject()));
    connect(ui->pushButton_TouchScreenCalibrate,SIGNAL(clicked()),this,SLOT(touchscreenCalibrate()));
    connect(ui->pushButton_LogOut,SIGNAL(clicked()),this,SLOT(logout()));
    connect(ui->pushButton_Calibrate,SIGNAL(clicked()),this,SLOT(calibrate()));
    connect(ChangeButton,SIGNAL(clicked()),this,SLOT(SetMachineTime()));
    connect(CancelButton,SIGNAL(clicked()),this,SLOT(TimeSetHide()));
    connect(pushButton_Exit,SIGNAL(clicked()),this,SLOT(quitSetProject()));
    connect(pushButton_add,SIGNAL(clicked()),this,SLOT(addProjectRecord()));
    connect(Choose_box,SIGNAL(activated(int)),this,SLOT(Choose_Add_Del()));
    connect(pushButton_Delete,SIGNAL(clicked()),this,SLOT(deleteProjectRecord()));
    connect(Button_Back,SIGNAL(clicked()),this,SLOT(quitSetProject()));
    connect(comboBox_User,SIGNAL(currentIndexChanged(QString)),lineEdit_CurrentUsername,SLOT(setText(QString)));
    connect(comboBox_User,SIGNAL(currentIndexChanged(QString)),this,SLOT(serachRealtimepwd(QString)));
    connect(pushButton_UserAdd,SIGNAL(clicked()),this,SLOT(addUserRecord()));
    connect(pushButton_UserDelete,SIGNAL(clicked()),this,SLOT(deleteUserRecord()));
    connect(pushButton_UserSave,SIGNAL(clicked()),this,SLOT(ChangeUserRecord()));
    connect(pushButton_UserExit,SIGNAL(clicked()),this,SLOT(quitSetUser()));
}

SetShow::~SetShow()
{
    delete ui;
}

void SetShow::changeEvent(QEvent *e)
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
//back to the last interface
void SetShow::home()
{
    //    close();
    //    delete this;
    this->hide();
}

//show the time,let it move
void SetShow::timerEvent(QTimerEvent *e) //To calcuate the cost Time
{
    QDateTime datatime=QDateTime::currentDateTime();
    QString Stringcurrentime=datatime.time().toString();
    QDateTime CurrentDate=QDateTime::currentDateTime();
    QString YearDate=CurrentDate.toString("yyyy.MM.dd");
    ui->label_date->setText(YearDate);
    ui->label_Time->setText(Stringcurrentime);
}

void SetShow::showTimeSet()
{
    TimeFrame->show();
    ui->frame->hide();
    initTime();//初始化成当前的时间
    ui->pushButton_Exit->setEnabled(false);
}

void SetShow::initTimeSet()
{
    TimeFrame=new QFrame(this);
    TimeFrame->setObjectName(QString::fromUtf8("Time_Frame"));
    TimeFrame->setStyleSheet(QString::fromUtf8("\n"
                                               "QFrame[objectName=\"Time_Frame\"]{\n"
                                               "background:transparent;\n"
                                               "background-image: url(:/ui/icon/TimeSet_Group.png);\n"

                                               "}"));
    TimeFrame->setGeometry(QRect(125,110,546,256));
    dateEdit=new QDateEdit(TimeFrame);
    timeEdit=new QTimeEdit(TimeFrame);
    ChangeButton=new QPushButton(TimeFrame);
    ChangeButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                  "background-image: url(:/ui/icon/TimeSet_up.png);\n"
                                                  "border-radius:20px;\n"
                                                  "}\n"
                                                  "\n"
                                                  "QPushButton:pressed{\n"
                                                  "background-image: url(:/ui/icon/TimeSet_down.png);\n"
                                                  "border-radius:20px;\n"
                                                  "}"));
    ChangeButton->setFocusPolicy(Qt::NoFocus);
    CancelButton=new QPushButton(TimeFrame);
    CancelButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                  "	background-image: url(:/ui/icon/TimeBack_up.png);\n"
                                                  "border-radius:20px;\n"
                                                  "}\n"
                                                  "\n"
                                                  "QPushButton:pressed{\n"
                                                  "	background-image: url(:/ui/icon/TimeBack_down.png);\n"
                                                  "border-radius:20px;\n"
                                                  "}"));
    CancelButton->setFocusPolicy(Qt::NoFocus);
    dateEdit->setGeometry(QRect(70,50,190,60));
    timeEdit->setGeometry(QRect(300,50,190,60));
    ChangeButton->setGeometry(QRect(60,120,141,105));
    CancelButton->setGeometry(QRect(350,120,141,105));
    QFont buttonFont("helvetica",20,QFont::Normal);
    ChangeButton->setFont(buttonFont);
    CancelButton->setFont(buttonFont);

    initTime();//初始化修改时间里的时钟
    TimeFrame->hide();
}

void SetShow::initTime()
{//初始化修改时间里的时钟
    QFont font("helvetica",20);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setDate(QDateTime::currentDateTime().date());
    timeEdit->setDisplayFormat("hh:mm:ss");
    timeEdit->setTime(QDateTime::currentDateTime().time());
    dateEdit->setFont(font);
    timeEdit->setFont(font);
    dateEdit->setFont(QFont("helvetica",24,QFont::Light));
    timeEdit->setFont(QFont("helvetica",24,QFont::Light));
}

void SetShow::initUserSet()
{
    Frame_User = new QFrame(this);
    Frame_User->setObjectName(QString::fromUtf8("Frame_User"));
    Frame_User->setGeometry(QRect(25, 60, 751, 350));
    Frame_User->setStyleSheet(QString::fromUtf8("\n"
                                                "QFrame[objectName=\"Frame_User\"]{\n"
                                                "background:transparent;\n"
                                                "border:none;\n"
                                                "}"));
    Frame_User->setFrameShape(QFrame::StyledPanel);
    Frame_User->setFrameShadow(QFrame::Raised);
    label_Group = new QLabel(Frame_User);
    label_Group->setObjectName(QString::fromUtf8("label_Group"));
    label_Group->setGeometry(QRect(60, 30, 639, 195));
    label_Group->setMinimumSize(QSize(639, 195));
    label_Group->setMaximumSize(QSize(639, 195));
    label_Group->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                 "background-image: url(:/ui/icon/UserSet_Group.png);\n"
                                                 ""));
    label_CurrentPasswd = new QLabel(Frame_User);
    label_CurrentPasswd->setObjectName(QString::fromUtf8("label_CurrentPasswd"));
    label_CurrentPasswd->setGeometry(QRect(370, 120, 263, 48));
    label_CurrentPasswd->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                         " background-image: url(:/ui/ui_new/pwd.png);\n"
                                                         "font-size:20px bold;\n"

                                                         "color:black;\n"
                                                         ));
    pushButton_UserExit = new QPushButton(Frame_User);
    pushButton_UserExit->setObjectName(QString::fromUtf8("pushButton_UserExit"));
    pushButton_UserExit->setGeometry(QRect(529, 241, 152, 102));
    pushButton_UserExit->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                         "	background-image: url(:/ui/icon/UserSet_Back_up.png);\n"
                                                         "border-radius:20px;\n"
                                                         "}\n"
                                                         "\n"
                                                         "QPushButton:pressed{\n"
                                                         "	background-image: url(:/ui/icon/UserSet_Back_down.png);\n"
                                                         "border-radius:20px;\n"
                                                         "}"));
    pushButton_UserExit->setFocusPolicy(Qt::NoFocus);
    pushButton_UserDelete = new QPushButton(Frame_User);
    pushButton_UserDelete->setObjectName(QString::fromUtf8("pushButton_UserDelete"));
    pushButton_UserDelete->setGeometry(QRect(382, 243, 145,102));
    pushButton_UserDelete->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                           "	background-image: url(:/ui/icon/UserSet_Delete_up.png);\n"
                                                           "border-radius:20px;\n"
                                                           "}\n"
                                                           "\n"
                                                           "QPushButton:pressed{\n"
                                                           "	background-image: url(:/ui/icon/UserSet_Delete_down.png);\n"
                                                           "border-radius:20px;\n"
                                                           "}"));
    pushButton_UserDelete->setFocusPolicy(Qt::NoFocus);
    label_idFrame = new QLabel(Frame_User);
    label_idFrame->setObjectName(QString::fromUtf8("label_idFrame"));
    label_idFrame->setGeometry(QRect(470, 70, 200, 31));
    label_idFrame->setMinimumSize(QSize(200, 31));
    label_idFrame->setMaximumSize(QSize(200, 31));
    label_idFrame->setHidden(true);
    label_idFrame->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                   "background-image: url(:/ui/icon/UsetSet_itemback.png);"));
    label_User = new QLabel(Frame_User);
    label_User->setObjectName(QString::fromUtf8("label_User"));
    label_User->setGeometry(QRect(150, 90, 79, 50));
    label_User->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                " background-image: url(:/ui/ui_new/UserA.png);\n"
                                                "color: rgb(255, 255, 255);\n"
                                                "font-size:20px bold;\n"
                                                ""));
    label_CurrentUsernamet = new QLabel(Frame_User);
    label_CurrentUsernamet->setObjectName(QString::fromUtf8("label_CurrentUsernamet"));
    label_CurrentUsernamet->setGeometry(QRect(370, 70, 263, 48));
    label_CurrentUsernamet->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                            "background-image: url(:/ui/ui_new/User.png);\n"
                                                            "font-size:20px bold;\n"

                                                            "color:black;\n"
                                                            ));
    pushButton_UserAdd = new QPushButton(Frame_User);
    pushButton_UserAdd->setObjectName(QString::fromUtf8("pushButton_UserAdd"));
    pushButton_UserAdd->setGeometry(QRect(75, 240, 152, 103));
    pushButton_UserAdd->setMinimumSize(QSize(152, 99));
    pushButton_UserAdd->setMaximumSize(QSize(152, 110));
    pushButton_UserAdd->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                        "	background-image: url(:/ui/icon/UserSet_Add_up.png);\n"
                                                        "border-radius:20px;\n"
                                                        "}\n"
                                                        "\n"
                                                        "QPushButton:pressed{\n"
                                                        "	background-image: url(:/ui/icon/UserSet_Add_down.png);\n"
                                                        "border-radius:20px;\n"
                                                        "}"));
    pushButton_UserAdd->setFocusPolicy(Qt::NoFocus);
    pushButton_UserSave = new QPushButton(Frame_User);
    pushButton_UserSave->setObjectName(QString::fromUtf8("pushButton_UserSave"));
    pushButton_UserSave->setGeometry(QRect(230, 243, 132, 104));
    pushButton_UserSave->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                         "	background-image: url(:/ui/icon/UserSet_Change_up.png);\n"
                                                         "border-radius:20px;\n"
                                                         "}\n"
                                                         "\n"
                                                         "QPushButton:pressed{\n"
                                                         "	background-image: url(:/ui/icon/UserSet_Change_down.png);\n"
                                                         "border-radius:20px;\n"
                                                         "}"));
    pushButton_UserSave->setFocusPolicy(Qt::NoFocus);
    label_pwdframe = new QLabel(Frame_User);
    label_pwdframe->setObjectName(QString::fromUtf8("label_pwdframe"));
    label_pwdframe->setGeometry(QRect(470, 120, 200, 31));
    label_pwdframe->setMinimumSize(QSize(200, 31));
    label_pwdframe->setMaximumSize(QSize(200, 31));
    label_pwdframe->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                    "background-image: url(:/ui/icon/UsetSet_itemback.png);"));
    label_pwdframe->setHidden(true);
    comboBox_User = new QComboBox(Frame_User);
    comboBox_User->setObjectName(QString::fromUtf8("comboBox_User"));
    comboBox_User->setGeometry(QRect(226, 98, 81, 34));

    lineEdit_CurrenttPwd = new QLineEdit(Frame_User);
    lineEdit_CurrenttPwd->setObjectName(QString::fromUtf8("lineEdit_CurrenttPwd"));
    lineEdit_CurrenttPwd->setGeometry(QRect(480, 118, 120, 46));
    lineEdit_CurrenttPwd->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                          "\n"
                                                          "border-radius:10px;"));

    lineEdit_CurrentUsername = new QLineEdit(Frame_User);
    lineEdit_CurrentUsername->setObjectName(QString::fromUtf8("lineEdit_CurrentUsername"));
    lineEdit_CurrentUsername->setGeometry(QRect(480, 70, 113, 46));
    lineEdit_CurrentUsername->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
                                                              "background:transparent;"));

    label_Group->raise();
    pushButton_UserExit->raise();
    pushButton_UserDelete->raise();
    label_idFrame->raise();
    label_User->raise();
    label_CurrentUsernamet->raise();
    pushButton_UserAdd->raise();
    pushButton_UserSave->raise();
    label_pwdframe->raise();
    label_CurrentPasswd->raise();
    comboBox_User->raise();
    lineEdit_CurrenttPwd->raise();
    lineEdit_CurrentUsername->raise();

    addUserrecordCommobox();
    Frame_User->hide();
}

void SetShow::initProSet()
{
    Frame_Project = new QFrame(this);
    Frame_Project->setObjectName(QString::fromUtf8("Frame_Project"));
    Frame_Project->setGeometry(QRect(15, 60, 751, 350));
    Frame_Project->setFrameShape(QFrame::StyledPanel);
    Frame_Project->setFrameShadow(QFrame::Raised);
    Frame_Project->setStyleSheet(QString::fromUtf8("\n"
                                                   "QFrame[objectName=\"Frame_Project\"]{\n"
                                                   "background:transparent;\n"
                                                   "border:none;\n"
                                                   "}"));
    Button_Back = new QPushButton(Frame_Project);
    Button_Back->setObjectName(QString::fromUtf8("Button_Back"));
    Button_Back->setGeometry(QRect(440, 270, 141, 61));
    Button_Back->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                 "	background-image: url(:/ui/icon/ProSet_Back_up.png);\n"
                                                 "border-radius:10px;\n"
                                                 "}\n"
                                                 "\n"
                                                 "QPushButton:pressed{\n"
                                                 "	background-image: url(:/ui/icon/ProSet_Back_down.png);\n"
                                                 "border-radius:10px;\n"
                                                 "}"));
    Button_Back->setFocusPolicy(Qt::NoFocus);
    label_Group = new QLabel(Frame_Project);
    label_Group->setObjectName(QString::fromUtf8("label_Group"));
    label_Group->setGeometry(QRect(70, 40, 638, 228));
    label_Group->setMinimumSize(QSize(638, 228));
    label_Group->setMaximumSize(QSize(638, 228));
    label_Group->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                 "background-image: url(:/ui/icon/ProSet_Group.png);\n"
                                                 ""));
    pushButton_add = new QPushButton(Frame_Project);
    pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
    pushButton_add->setGeometry(QRect(142, 100, 152, 99));
    pushButton_add->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                    "	background-image: url(:/ui/icon/ProSet_Add_up.png);\n"
                                                    "border-radius:10px;\n"
                                                    "}\n"
                                                    "\n"
                                                    "QPushButton:pressed{\n"
                                                    "	background-image: url(:/ui/icon/ProSet_Add_down.png);\n"
                                                    "border-radius:10px;\n"
                                                    "}"));
    pushButton_add->setFocusPolicy(Qt::NoFocus);

    pushButton_Delete = new QPushButton(Frame_Project);
    pushButton_Delete->setObjectName(QString::fromUtf8("pushButton_Delete"));
    pushButton_Delete->setGeometry(QRect(500, 100, 152, 99));
    pushButton_Delete->setMinimumSize(QSize(152, 99));
    pushButton_Delete->setMaximumSize(QSize(152, 99));
    pushButton_Delete->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                       "	\n"
                                                       "	background-image: url(:/ui/icon/ProSet_Delete_up.png);\n"
                                                       "border-radius:10px;\n"
                                                       "}\n"
                                                       "\n"
                                                       "QPushButton:pressed{\n"
                                                       "	background-image: url(:/ui/icon/ProSet_Delete_down.png);\n"
                                                       "border-radius:10px;\n"
                                                       "}"));
    pushButton_Delete->setFocusPolicy(Qt::NoFocus);
    pushButton_Exit = new QPushButton(Frame_Project);
    pushButton_Exit->setObjectName(QString::fromUtf8("pushButton_Exit"));
    pushButton_Exit->setGeometry(QRect(160, 270, 141, 61));
    pushButton_Exit->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                     "	background-image: url(:/ui/icon/ProSet_Ok_up.png);\n"
                                                     "border-radius:10px;\n"
                                                     "}\n"
                                                     "\n"
                                                     "QPushButton:pressed{\n"
                                                     "	background-image: url(:/ui/icon/ProSet_Ok_down.png);\n"
                                                     "border-radius:10px;\n"
                                                     "}"));
    pushButton_Exit->setFocusPolicy(Qt::NoFocus);
    Project_box = new QComboBox(Frame_Project);
    Project_box->setObjectName(QString::fromUtf8("Project_box"));
    Project_box->setGeometry(QRect(180, 58, 121, 28));

    label_Project = new QLabel(Frame_Project);
    label_Project->setObjectName(QString::fromUtf8("label_Project"));
    label_Project->setGeometry(QRect(103, 56, 79, 31));
    label_Project->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                   "background-image: url(:/ui/ui_new/ProSet_ChoosePro.png);\n"
                                                   "font-size:20px bold;\n"

                                                   "color:black;\n"
                                                   ));
    //label_Project->setText(QString::fromUtf8("Choose"));
    label_input = new QLabel(Frame_Project);
    label_input->setObjectName(QString::fromUtf8("label_Project"));
    label_input->setGeometry(QRect(465, 57, 79, 32));
    label_input->setStyleSheet(QString::fromUtf8("background:transparent;\n"
                                                 "background-image: url(:/ui/inputproject.png);\n"
                                                 "font-size:20px bold;\n"

                                                 "color:black;\n"
                                                 ));
    //选择是添加还是删除按钮
    Choose_box = new QComboBox(Frame_Project);
    Choose_box->setObjectName(QString::fromUtf8("Choose_box"));
    Choose_box->setGeometry(QRect(540, 61, 121, 28));

    Choose_box->addItem(QString::fromUtf8("添加项目"));
    Choose_box->addItem(QString::fromUtf8("删除项目"));

    Button_Back->raise();
    label_Group->raise();
    pushButton_add->raise();
    pushButton_Delete->raise();
    pushButton_Exit->raise();
    Project_box->raise();
    label_Project->raise();
    Choose_box->raise();
    label_input->raise();

    Choose_Add_Del();

    Frame_Project->hide();
}

void SetShow::TimeSetHide()
{
    TimeFrame->hide();
    ui->frame->show();
    ui->pushButton_Exit->setEnabled(true);
}

void SetShow::initShowProject()
{
    Frame_Project->show();
    ui->frame->hide();
    Choose_Add_Del();
    ui->pushButton_Exit->setEnabled(false);
}

void SetShow::realAddPro()
{
    char *copy ;
    QString path = "cp /media/mmcblk0p1/formula/" + Project_box->currentText()
            + ".txt formula" ;//整体路径，即在SD中的路径
    QString path1 = "formula/" + Project_box->currentText() + ".txt" ;//
    //cp /sdcard/formula/xxx.txt /fomula
    QByteArray ba = path1.toLatin1();
    copy = ba.data();
    ba = path.toLatin1();//整体路径
    copy = ba.data();
    ::system(copy);
    ba = path1.toLatin1();
    copy = ba.data();
    warning=new Mywarning(this);
    warning->move((800-warning->width())/2,(480-warning->height())/2);
    if(-1==access(copy,0))//不存在
    {
        warning->setText(QString::fromUtf8("添加失败，请重新添加"));
        warning->exec();
    }else{
        warning->setRight();
        warning->setText(QString::fromUtf8("添加成功"));
        warning->exec();
    }
}

void SetShow::addProjectRecord()
{
    char *copy ;
    QString path1 = "formula/" + Project_box->currentText() + ".txt" ;//
    //cp /sdcard/formula/xxx.txt /fomula
    QByteArray ba = path1.toLatin1();
    copy = ba.data();
    if(Project_box->currentText() == "")
    {//没有SD卡
        warning->setText(QString::fromUtf8("请确认SD中公式格式正确"));
        warning->exec();
    }else if(-1==access(copy,0))//内存中不存在可以复制---提示是否覆盖
    {
        realAddPro();//复制
    }else{
        DelProwarning = new warning_two(this);
        DelProwarning->move((800-DelProwarning->geometry().width())/2,(480-DelProwarning->geometry().height())/2);

        connect(DelProwarning,SIGNAL(Pressed_Yes()),this,SLOT(realAddPro()));
        disconnect(DelProwarning,SIGNAL(Pressed_Yes()));
        DelProwarning->setText(QString::fromUtf8("已存在，是否覆盖？"));
        DelProwarning->exec();
    }
}

void SetShow::realDelPro()
{
    char *del ;
    QString path = "rm formula/" + Project_box->currentText()
            + ".txt" ;//整体路径，即在SD中的路径
    QByteArray ba = path.toLatin1();
    del = ba.data();
    QString path1 = "formula/" + Project_box->currentText() + ".txt" ;

    ::system(del);//删除Flash中的文件

    ba = path1.toLatin1();
    del = ba.data();

    warning=new Mywarning(this);
    warning->move((800-warning->width())/2,(480-warning->height())/2);
    if(-1==access(del,0))
    {
        warning->setRight();
        warning->setText(QString::fromUtf8("项目删除成功"));
        warning->exec();
        Choose_Add_Del();
    }else{
        warning->setRight();
        warning->setText(QString::fromUtf8("项目删除失败"));
        warning->exec();
    }
}
void SetShow::deleteProjectRecord()
{
    DelProwarning = new warning_two(this);
    DelProwarning->move((800-DelProwarning->geometry().width())/2,(480-DelProwarning->geometry().height())/2);

    connect(DelProwarning,SIGNAL(Pressed_Yes()),this,SLOT(realDelPro()));
    disconnect(DelProwarning,SIGNAL(Pressed_Yes()));
    DelProwarning->setText(QString::fromUtf8("确定删除项目?"));
    DelProwarning->exec();
}

void SetShow::quitSetProject()
{
    Frame_Project->hide();
    ui->frame->show();
    ui->pushButton_Exit->setEnabled(true);
}

void SetShow::initShowUser()
{
    Frame_User->show();
    ui->frame->hide();
    ui->pushButton_Exit->setEnabled(false);

    warning=new Mywarning(this);
    warning->move((800-warning->width())/2,(480-warning->height())/2);
}

void SetShow::Choose_Add_Del()
{//当选择“添加项目”的时候就会筛选出SD卡中的公式，否则当选择的是“删除项目”则筛选出Flash中的公式
    QStringList list ;
    OneKeyImport formula;
    Project_box->clear();
    if(Choose_box->currentText() == QString::fromUtf8("添加项目"))
    {
        list.clear();
        pushButton_add->setEnabled(true);
        pushButton_Delete->setEnabled(false);//设置添加按钮可点击
        //if(formula.WeatherHaveSD())//SD卡是否存在
        list = formula.FindFormulaFileInSD("/media/mmcblk0p1/formula/");//在固定目录下面查找所有公式
        Project_box->addItems(list);
    }else if(Choose_box->currentText() == QString::fromUtf8("删除项目"))
    {
        pushButton_add->setEnabled(false);
        pushButton_Delete->setEnabled(true);//设置删除按钮可点击
        list.clear();
        list = formula.FindFormulaFileInSD("formula/");
        Project_box->addItems(list);
    }
}



void SetShow::addUserRecord()
{//添加用户
    //you can not add admin
    QString name  = lineEdit_CurrentUsername->text();
    QString password = lineEdit_CurrenttPwd->text() ;
    if(name != "" && password != "")
    {
        if(SQLITE::sql->Select(name,"",1)!="0")
        {//用户名已经存在
            warning->setText(QString::fromUtf8("用户已存在"));
            warning->exec();
        }else{//查找到最后了，也没有发现相同的，则添加用户
            SQLITE::sql->Insert(name,password,"3");
            warning->setRight();
            warning->setText(QString::fromUtf8("用户添加成功"));
            warning->exec();
        }
    }else{
        warning->setText(QString::fromUtf8("请输入用户名或密码"));
        warning->exec();
    }
    addUserrecordCommobox();
}

void SetShow::deleteUserRecord()
{//删除用户
    if(lineEdit_CurrentUsername->text() == comboBox_User->currentText())
    {
        SQLITE::sql->Delete(lineEdit_CurrentUsername->text());

        warning->setRight();
        warning->setText(QString::fromUtf8("用户删除成功"));
        warning->exec();
    }
    addUserrecordCommobox();
}

void SetShow::ChangeUserRecord()
{
    QString LoginName="user/user.txt";
    QFile LoginFile(LoginName);
    if(LoginFile.open(QIODevice::ReadOnly)<0)
        perror("open file error");
    QTextStream LoginStream(&LoginFile);
    QString lines;
    QString finallines;
    while(!LoginStream.atEnd())
    {
        QString line=LoginStream.readLine();
        QString section=line.section(",",0,0);
        if((section==comboBox_User->currentText()))//use combox to change the file
        {
            if(comboBox_User->currentText()=="admin")//you can only change admin's pwd
                line=comboBox_User->currentText()+","+lineEdit_CurrenttPwd->text();//if it did not contains your input
            else
                line=lineEdit_CurrentUsername->text()+","+lineEdit_CurrenttPwd->text();//if it did not contains your input
        }
        line.append("\n");
        finallines+=line;
    }
    LoginFile.close();

    QFile InputFile(LoginName);
    if(InputFile.open(QIODevice::WriteOnly)<0)
        perror("open file error");
    QTextStream InputStream(&InputFile);
    InputStream<<finallines;

    InputFile.close();
    addUserrecordCommobox();
}

void SetShow::quitSetUser()
{
    Frame_User->hide();
    ui->frame->show();
    ui->pushButton_Exit->setEnabled(true);
}

void SetShow::addUserrecordCommobox()
{
    comboBox_User->clear();
    QStringList list;
    list=SQLITE::sql->Select(0);
    comboBox_User->addItems(list);
}

void SetShow::serachRealtimepwd(QString Firstusername)
{
    QString pwd;
    qDebug()<<"Firstusername"<<Firstusername;
    pwd=SQLITE::sql->Select(Firstusername,"",2);
    qDebug()<<"pwd"<<pwd;
    lineEdit_CurrenttPwd->setText(pwd.section(',',1,1));
}

void SetShow::logout()
{
    //close all windows
    //then create a new main process
    delete this;
    qApp->exit(773);
}

void SetShow::calibrate()
{
    ui->frame->hide();
    ui->pushButton_Exit->setEnabled(false);
    Calibrate_Six *calibrateResult=new Calibrate_Six(this);
    calibrateResult->move((800-calibrateResult->width())/2,100);
    calibrateResult->initiatemove();
    calibrateResult->show();

    connect(calibrateResult,SIGNAL(calibrateFinished()),ui->frame,SLOT(show()));
    connect(calibrateResult,SIGNAL(calibrateFinished()),this,SLOT(show_exit()));
}

void SetShow::show_exit()
{//在系统校正界面退出的时候显示“返回首页”按钮
    ui->pushButton_Exit->setEnabled(true);
}

void SetShow::touchscreenCalibrate()
{
    DelProwarning = new warning_two(this);
    DelProwarning->move((800-DelProwarning->geometry().width())/2,(480-DelProwarning->geometry().height())/2);

    connect(DelProwarning,SIGNAL(Pressed_Yes()),this,SLOT(realCalibrate()));
    disconnect(DelProwarning,SIGNAL(Pressed_Yes()));
    DelProwarning->setText(QString::fromUtf8("重启并校准？"));
    DelProwarning->exec();
}

void SetShow::realCalibrate()
{
    QFileInfo ts_Calibrate("/etc/pointercal");
    if(ts_Calibrate.exists())
    {
        ::system("wr rm /etc/pointercal");
        ::system("wr reboot");
    }
}

void SetShow::superBehavior()
{
    QFile File;
    QString DestFile="/xyz";

    QString SourceFile="/media/mmcblk0p1/xyz";

    QFileInfo xyzfile(SourceFile);

    QMessageBox warning;
    warning.setToolTip("warning");
    warning.setText("Are you sure");
    warning.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    if(warning.exec()==QMessageBox::Yes)
    {
        if(xyzfile.exists())
        {
            int back=unlink(DestFile.toLatin1().data());//delete it First

            int ret=File.copy(SourceFile,DestFile);
            if(ret>0)
                QMessageBox::warning(this,tr("success"),tr("recover finished"));
        }
    }
    else
        return ;
}

bool SetShow::judge_subadmin()
{
    if(SQLITE::sql->Select(Current_user,"",1) == "2")
    {//若为次级管理员
        ui->pushButton_Calibrate->setEnabled(false);
    }else{
        ui->pushButton_Calibrate->setEnabled(true);
    }
    return true;
}

void SetShow::initiatemove()
{
    initTimeSet();
    initUserSet();
    initProSet();
}

void SetShow::SetSoftWareVersion(QString Version,QString username)
{
    Current_user = username ;
    judge_subadmin();
    ui->Label_UserName->setText(username);
    ui->Label_SoftWareVersion->setText(Version);
    ui->Label_SoftWareVersion->show();
}

void SetShow::SetMachineTime()
{
    int year,month,date,hour,minute,second;
    year=dateEdit->date().year();
    month=dateEdit->date().month();
    date=dateEdit->date().day();
    hour=timeEdit->time().hour();
    minute=timeEdit->time().minute();
    second=timeEdit->time().second();

    time_t t;
    struct tm nowtime;
    nowtime.tm_sec=second;
    nowtime.tm_min=minute;
    nowtime.tm_hour=hour;
    nowtime.tm_mday=date;
    nowtime.tm_mon=month-1;
    nowtime.tm_year=year-1900;
    nowtime.tm_isdst=-1;
    t=mktime(&nowtime);
    stime(&t);
    system("hwclock -w");
    warning=new Mywarning(this);
    warning->move((800-warning->width())/2,(480-warning->height())/2);
    warning->setRight();
    warning->setText(QString::fromUtf8("时间设置成功"));
    warning->exec();
}
