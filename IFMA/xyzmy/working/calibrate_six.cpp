//we use area divided to calibrate,not consistency
#include "calibrate_six.h"
#include "ui_calibrate_six.h"
float finalresult[6]={0};//detect 6 cassette,and get its value
float Standresult[6]={0};
Calibrate_Six::Calibrate_Six(QWidget *parent) :
        CheckShow(parent),
        ui(new Ui::Calibrate_Six)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    warning=new Mywarning(this);
    warning->move((800-warning->width())/2,(480-warning->height())/2);
    warning->hide();
    ui->pushButton_calibrate->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_exit->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_out->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Save->setFocusPolicy(Qt::NoFocus);
    weatherCalibrate=true;
    connect(ui->pushButton_calibrate,SIGNAL(clicked()),this,SLOT(BeginRepeatTest()));
    connect(ui->pushButton_Save,SIGNAL(clicked()),this,SLOT(weatherSavefile()));
    connect(this,SIGNAL(RepeattestValue(QString)),this,SLOT(showfinished(QString)));
    connect(ui->pushButton_exit,SIGNAL(clicked()),this,SLOT(quitshow()));
    connect(ui->pushButton_out,SIGNAL(clicked()),this,SLOT(Step_in_or_out()));
}

Calibrate_Six::~Calibrate_Six()
{
    delete ui;
}



void Calibrate_Six::saveCalibrateResult()
{
    char *pathname;
    bool AllCalibrated=true;//weather six points calibrated
    QString FileName;
    QByteArray array;
    int i=0;
    FileName.append("calibrateFile.txt");
    //delete the file firsts
    array=FileName.toLatin1();
    pathname=array.data();
    unlink(pathname);
    QFile calibrateFile(FileName);
    if(calibrateFile.open(QIODevice::WriteOnly|QIODevice::Append)<0)
    {
        perror("open file error");
        QMessageBox::information(this,tr("error"),tr("open calibrate file error"));
    }
    QTextStream in(&calibrateFile);
    for(i=0;i<6;i++)
    {
        if(finalresult[i]==0)
        {
            QString error_mes;
            error_mes.append(ui->comboBox_Cassette->itemText(i));
            error_mes.append(QString::fromUtf8("未校正"));
            warning->setText(error_mes);
            warning->move(150,100);
            AllCalibrated=false;
        }
    }
    for(i=0;i<6;i++)
    {
        if(AllCalibrated)
            in<<ui->comboBox_Cassette->itemText(i)<<","<<Standresult[i]<<","<<finalresult[i]<<endl;
    }
}

void Calibrate_Six::showfinished(QString value)
{
    int index=ui->comboBox_Cassette->currentIndex();
    finalresult[index]=value.toFloat();//get the value
    Standresult[index]=ui->lineEdit_StandardValue->text().toFloat();
    ui->lineEdit_RealValue->setText(value);
    QString error_mes;
    error_mes.append(ui->comboBox_Cassette->currentText());
    error_mes.append("finished calibrated");
    ui->pushButton_out->setEnabled(true);
}
void Calibrate_Six::initComboxname()
{
    ui->comboBox_project->clear();
    QStringList list ;//list保存的是形如：{"crp_111","tcp_222"}之类的
    QStringList list_Pro;//最终显示的项目号
    QStringList temp ;
    QString str ;
    list = Test.FindFormulaFileInSD("formula/");

    for(int i=0;i<list.length();i++)
    {
        str = list[i];//str形如：“crp_111”
        temp = str.split('_');
        list_Pro<<temp[0];
        for(int i=0;i<list_Pro.length()-1;i++)
            if(list_Pro[i] == temp[0])
                list_Pro.removeLast();
    }//for

    if(list_Pro.length()>0)
        ui->comboBox_project->addItems(list_Pro);
}

void Calibrate_Six::quitshow()
{
    delete warning;
    close();
    emit calibrateFinished();
}
void Calibrate_Six::disabledOut()
{
    ui->pushButton_out->setEnabled(false);
}
void Calibrate_Six::initiatemove()
{
    hideui();
    initComboxname();
    HAS=true;
    saveFileName=0;
}
void Calibrate_Six::weatherSavefile()
{
    QMessageBox *warning=new QMessageBox;
    warning->setToolTip("Save");
    warning->setText(QString::fromUtf8("是否覆盖校正文件？"));
    warning->setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    warning->setButtonText(QMessageBox::Yes,QString::fromUtf8("是"));
    warning->setButtonText(QMessageBox::No,QString::fromUtf8("否"));

    if(warning->exec()==QMessageBox::Yes)
    {
        saveCalibrateResult();
    }
}

void Calibrate_Six::paintEvent(QPaintEvent *event)
{
}
