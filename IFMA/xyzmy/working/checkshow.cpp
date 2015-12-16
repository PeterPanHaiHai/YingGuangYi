/*++++++++++++++++++++++++++++++++++++++++++++++++
//Author:Xiong youzheng
//time:2013-3-21
update:
I delete 'OUT' button,and replace with 'Test',it have same function
and press 'in' to let it in



//Update
//Date:2013-4-5
//add a showPicturescreen value,when it finished test,show the curves on screen,next Test,it fades

//to make the curves smoothily,so write function to make it smooth
//date2013-4-9

//USE QPointArray to draw pointsf
//update date:2013-4-12save


//Update:get more data one time
//Date:2013-5-10
//changed algorithm to calcuate the area

//Update:let the curves and scale coordiate automiacatilly
//Date:2013-5-20
//Content:find the peak of the curves

//DATE:2013-05-22
//it often crashed,so i try to find out the reason and solve it
//NEXT
//ALgoritham for area,add more limits

//Add a function,'adjust function',its purpose is let every machine have the same result when you test same standard cassette

//search drdy for 2 0r 3times then you get a data

//add calibrate function,6 points to calcubrate
//if you give me a bach of cards,then you must give me standard cards,so i need to use this bach cards to calibrate
//in case it suddently crashed ,i can blocked exit(1)

//阴性纸条判断方法
//根据张晶的检测结果是，如果是阴性纸条，那么实际上只有一个峰，但是程序会错误的寻找到两个峰，
//算法寻找的俩个峰特征如下：
//算法把自动寻找到的第一个峰作为AT，而此时第一个峰是AC，所以检测到的AT位置在AC范围内
//实际上只有一个峰，而算法为了满足两个峰的条件，则从后面随意找一个非目标峰，此目标峰的面积极小（因为纸条只有一个峰，其余皆背景）
//综合起来，特征：第一个峰在AC位置，面积是AC面积，第二个峰在其后，面积极小
//满足以上两个特征，可作判断，此为阴性纸条
//bool weatherNegative();
//还有一种情况，有两个峰，但是T峰很小，其阈值<3000，AC是正常的，这个也是阴性纸条


对键盘的操作需要多加注意，很多时候用户会忘记消失小键盘
update:2013-09-04
introduction:the first point we get is 0,and i found out the reason,and finished it
later:use recycle way to control the stepper

Update:modify this software to Six point calibrate
两条带和六条带的区别主要是：
1:检测速度PWM不同－－－－－》检测区域不一致
2：finished_test算法不一样－－－－－－－》最后处理算法不一样
所以可以编写一个类，该类含有检测带条数，检测带pwm，检测处理算法

对于不同条带处理，则对该类赋不同值

//DATEL：2013－0305
//UPDATE：在SD卡中导入的公式进行设置，在其中加入“小数为数，显示单位”实现这些信息的动态添加

*/
#include <fstream>
#include "checkshow.h"
#include "ui_checkshow.h"
#include "yiqisuanfa.h"


#include "test.h"

#define TOTAL 1200//Points i got receive form AD7707
#define limit 0.15//threshold to search the start point of the curves
#define endlimit 2.0//threshold to search the end point of the curves
#define Screenlength 640//the size of the screen
int curvesHeight=10;//the threshold to analyze the curves
//here below is the part used to control the linear
bool meetphotodiodeds=false;//default did not meet photodiodeds
bool meetswitch=false;//default did not meet photodiodeds
const int innerphotodioded=0;//内测光电断路器
#define R&D
#define PWM_IOCTL_SET_FREQ_IN   5  //Set motor goes reverse
#define PWM_IOCTL_SET_FREQ_OUT	1 //Set motor goes forward
#define PWM_IOCTL_STOP			0//
#define	ESC_KEY		0x1b

//******************************
//here below is findpeak algorithm

double Curves_HeightDivided;
double First_Height;
double Second_Height;
double First_width;
double Second_width;
double First_Area;
double Second_Area;
double AreaDivided;
double correctAreaDivided;
int dropPoints=0;//drop 50 points
int Search_peaks(double *SmoothSaveData, double *Savedata);
const int RepeatTimes=5;
const int lowerlimitArea=50000;
const int NegativeAreaThreshold=100;
int repeatCount=0;
float repeatTestValues[RepeatTimes];
//************************************************


int direction=PWM_IOCTL_SET_FREQ_OUT;//motor direction

int freq = 1000 ;//the speed of the motor
static int fd = -1;
int minustoshow=20;//Curves is much bigger then screen ,minus the curves to show
int ledopen;

int on=1;

int drawpointgap=12;//draw a point when you received 8 points,First is 3
int LegalLEngth=20;
int LegalHeight=20;
int calcuate=0;
int smoothtemp=0;
int NpointsDrawOnce=1;//
int peaklength=20;
int TestLimit=5;
char c[3];//feedback message from serial port
int saveDataCount=0;

//int rectheight=220;//the hight of the rect(used to contain the curves)
int number=0;
int Ylarge=1;//ampilfy Ylarge to show
int xwidth=460;//the edge of the x axis
int yposition=335;//the edge of the y axis
int WIDTH;
double Savedata[3000]={0};//used to save the data
double AD[2420]={0} ;
double SmoothSaveData[2000]={0};
double paintData[2000]={0};//just used to paint it is savedata/minustoshow
int YCount=10;//Y axis have 7 lines
int XCount=10;//X axis have 12 lines
int Yaxisgap=20;//show one over axisgap
int Xaxisgap=30;//before is 50
int Axis_width=XCount*Xaxisgap;//300
int xdrawpointgap=2;//in xaxis:every points's gap:(is not 1),Frist is 2
//***************************
float reallyDrawpointgap=Axis_width*drawpointgap*1.0/TOTAL;//reallydrawpointgap should be integer
int second_begin;
int second_up;
int second_end;
int begin;
int up;
int T_position;
int C_position;
int j=0;
int end;
int rectheight=Yaxisgap*YCount*Ylarge;

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

CheckShow::CheckShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckShow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    // switchToConsistency_AT(10);
    isDraw=false;//set isdraw false at first;
    SerachDrdyNumber=100;
    //    Savedata=(double*)calloc(2000, sizeof(double));
    //    SmoothSaveData=(double*)calloc(2000, sizeof(double));
    //    paintData=(double*)calloc(2000, sizeof(double));
    saveDataCount=0;
    smoothtemp=0;
    TestFinished=false;
    isCalibrate=false ;
    isSelfCheck=false;
    initiateInputDialog();
    ReadCalibrateFile();
    Savecount=0;
    //directlly output reult
    const int TestStatus=0;
    MAXDrdyRepeat=0;
    saveFileName=0;
    weatherdrawsmooth=false;
    OPtionopen=false;//Default option is unopen

    createdirectory();
    showPixtureScreen=false;//default do not show
    showSmooth=3;
    weatherCalibrate=false;
    connect(this,SIGNAL(finishedDataReceived()),this,SLOT(finished_Test()));
    connect(ui->comboBox_Type,SIGNAL(activated(int)),this,SLOT(Pro_change()));
    NextMoveDirection=PWM_IOCTL_SET_FREQ_OUT;

    ui->comboBox_Stripes->hide();
    ui->label_Stripes->hide();
    TestStrips=new strips;//default it is 2
    TestStrips->SetNumber(2);
    TestStrips->SetPwm(120);
    ui->label_Height1->hide();
    ui->label_Height2->hide();
    ui->label_Height3->hide();
    ui->label_Height4->hide();
    ui->label_Height5->hide();
    ui->label_Height6->hide();
    WeatherLinearMoving=false;
#ifdef R&D
    ui->label_Area1->show();
    ui->label_Area2->show();
#else
    ui->label_Area1->hide();
    ui->label_Area2->hide();
#endif
    connect(ui->pushButton_save,SIGNAL(clicked()),this,SLOT(ResultSave()));
    connect(ui->pushButton_print,SIGNAL(clicked()),this,SLOT(printInfo()));
    ui->pushButton_LogOut->setFocusPolicy(Qt::NoFocus);
    connect(ui->pushButton_LogOut,SIGNAL(clicked()),this,SLOT(LogOut()));


    //test by MarxSu
    ui->pushButton->setVisible(true);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(onTestButtonOnClick()));
    ui->m_saveButton->setVisible(true);
    connect(ui->m_saveButton,SIGNAL(clicked()),this,SLOT(onSaveButtonOnClick()));
    connect(ui->m_deleteButton,SIGNAL(clicked()),this,SLOT(onDeleteButtonOnClick()));
    mTestCount = 1;
    mTestFreq = 235;
     connect(ui->mChangeAppButton,SIGNAL(clicked()),this,SLOT(onChangeAppButtonClick()));
}



CheckShow::~CheckShow()
{
    delete ui;
}

void CheckShow::changeEvent(QEvent *e)
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

void CheckShow::OptionShow()
{
    showPixtureScreen=false;
    update(0,yposition-rectheight,Axis_width+xwidth,rectheight);
    isDraw=false;
}
//back to last interface
void CheckShow::home()
{
    smoothtemp=0;
    saveDataCount=0;
    update(0,yposition-rectheight,Axis_width+xwidth,rectheight);
    delete this;
    //    this->hide();
}

void CheckShow::drawCurves(QPainter *paint)
{

    QPainterPath painter;
    QPen pen(Qt::red,2,Qt::SolidLine);
    paint->setPen(pen);
    //if(saveDataCount==drawpointgap-1)//first time begin to paint
    painter.moveTo(smoothtemp+xwidth-reallyDrawpointgap,yposition-showSmoothNumber[saveDataCount-drawpointgap]);
    painter.lineTo(smoothtemp+xwidth,yposition-showSmoothNumber[saveDataCount]);
    paint->drawPath(painter);
}
//search peak number of the curves
//我设计的用于寻找波峰的算法
double CheckShow::searchPeak(double *Savedata)
{
    //To the second curves,we should make a speciall judgement
    //now we have 1200curves,so curvesheight should be larger
    //and to curves height,to different arrangement set different TestLimit
    int i=0;
    //界定一个阀值，limit,求出这个波峰的起始点
    i=i+50;//drop the first 50 points
    //search the start point of the curves
    while(((Savedata[i+curvesHeight+6]-Savedata[i+6])<(limit*Ylarge))||((Savedata[i+curvesHeight+5]-Savedata[i+5])<(limit*Ylarge))||((Savedata[i+curvesHeight+4]-Savedata[i+4])<(limit*Ylarge))||((Savedata[i+curvesHeight+3]-Savedata[i+3])<(limit*Ylarge))||((Savedata[i+curvesHeight+2]-Savedata[i+2])<(limit*Ylarge))||((Savedata[i+curvesHeight+1]-Savedata[i+1])<(limit*Ylarge))||((Savedata[i+curvesHeight]-Savedata[i])<(limit*Ylarge)))
        i++;
    begin=i;
    //通过对波峰点的左右大约10个像 位置除距离陡壁起自己小，其附近的点也存在同 规则，寻找波峰
    //there area another way to find peak,from fist to half of the area,find the largest
    while((Savedata[i]-Savedata[begin])<(Savedata[begin]/2)||(Savedata[i]<Savedata[i+10])||(Savedata[i]<Savedata[i-10])||(Savedata[i+1]<Savedata[i+11])||(Savedata[i+1]<Savedata[i-9])||(Savedata[i-1]<Savedata[i+9])||(Savedata[i-1]<Savedata[i-11]))
        i++;
    up=i;
    i=up+1;
    while((fabs((Savedata[i+6]-Savedata[i+6+curvesHeight]))>(limit*Ylarge))||(fabs((Savedata[i+5]-Savedata[i+5+curvesHeight]))>(limit*Ylarge))||(fabs((Savedata[i+4]-Savedata[i+4+curvesHeight]))>(limit*Ylarge))||(fabs((Savedata[i+3]-Savedata[i+3+curvesHeight]))>(limit*Ylarge))||(fabs((Savedata[i+2]-Savedata[i+2+curvesHeight]))>(limit*Ylarge))||(fabs((Savedata[i+1]-Savedata[i+1+curvesHeight]))>(limit*Ylarge))||(fabs((Savedata[i]-Savedata[i]))>(limit*Ylarge+curvesHeight)))
        i++;
    end=i;

    i=end+10;
    while(((Savedata[i+curvesHeight+6]-Savedata[i+6])<(limit*Ylarge))||((Savedata[i+curvesHeight+5]-Savedata[i+5])<(limit*Ylarge))||((Savedata[i+curvesHeight+4]-Savedata[i+4])<(limit*Ylarge))||((Savedata[i+curvesHeight+3]-Savedata[i+3])<(limit*Ylarge))||((Savedata[i+curvesHeight+2]-Savedata[i+2])<(limit*Ylarge))||((Savedata[i+curvesHeight+1]-Savedata[i+1])<(limit*Ylarge))||((Savedata[i+curvesHeight]-Savedata[i])<(limit*Ylarge)))
        i++;
    second_begin=i;
    i=second_begin+5;

    while((Savedata[i]<Savedata[i+peaklength])||(Savedata[i]<Savedata[i-peaklength])||(Savedata[i+1]<Savedata[i+peaklength+1])||(Savedata[i+1]<Savedata[i-peaklength+1])||(Savedata[i-1]<Savedata[i+peaklength-1])||(Savedata[i-1]<Savedata[i-peaklength-1]))
        i++;
    second_up=i;
    i=second_up+1;
    if(Savedata[second_up]<500)
        TestLimit=1;
    else
        TestLimit=5;
    while((fabs((Savedata[i+6]-Savedata[i+6+curvesHeight]))>(TestLimit))||(fabs((Savedata[i+5]-Savedata[i+5+curvesHeight]))>(TestLimit))||(fabs((Savedata[i+4]-Savedata[i+4+curvesHeight]))>(TestLimit))||(fabs((Savedata[i+3]-Savedata[i+3+curvesHeight]))>(TestLimit))||(fabs((Savedata[i+2]-Savedata[i+2+curvesHeight]))>(TestLimit))||(fabs((Savedata[i+1]-Savedata[i+1+curvesHeight]))>(TestLimit)))
        i++;
    second_end=i;
    if((second_end-second_up<LegalLEngth)||(Savedata[second_up]-Savedata[second_end])<LegalHeight)
        Second_Curves=false;

    T_position=(begin+end)/2;
    C_position=(second_begin+second_end)/2;

    return 0;
}

//when you find the start,up,end point of the curves ,draw it
void CheckShow::drawpoint(QPainter *painter)
{
    QPen pen(Qt::green,8,Qt::SolidLine);
    painter->setPen(pen);
    //painter->drawPoint((begin*2)+xwidth,yposition-Savedata[begin]);
    painter->drawPoint((up*2)+xwidth,yposition-Savedata[up]);
    int blankplace=(end+second_begin)/2;
    painter->drawPoint((blankplace*2)+xwidth,yposition-Savedata[blankplace]);
}

//求从i-count->(i+count)处像 平均值,drop the max and min
float CheckShow::average(int i,int count)
{
    //int count=30;
    float temp=0.0;
    int j;
    int max=0;
    int min=0;
    if(i-count<0||i+count>TOTAL)
    {
        temp=Savedata[i];
    }
    else//i+count没有超过数值边界
    {
        for(j=i-count;j<i+count;j++)
        {
            temp+=Savedata[j];
            //Drop the max and min
            if(Savedata[j]<min)
                min=Savedata[j];
            if(Savedata[j]>max)
                max=Savedata[j];
        }
        temp=(temp)/(2*count);
    }

    return temp;
}

//将曲线进行平滑滤波一下
//从曲线图中每次取出n个像 点，将n个像 点数据求平均，将其值赋给原来的像 点值
void CheckShow::smooth(int count)
{
    int i=0;
    for(i=0;i<TOTAL;i++)
        SmoothSaveData[i]=average(i,count);//average 函数用来求从i->(i+n)处像 平均值，SmoothSaveData就是平滑以后的数据
    //we get smoothdata so we can paint them
    minustoshow=automaticallyScales()*10;//minus to show should behind smooth(5),after smooth(5),we get smooth number

    for(i=0;i<TOTAL;i++)
        paintData[i]=SmoothSaveData[i]/minustoshow;//we get the data used to paint,it is not used to save and calcuate
}

void CheckShow::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    QPen pen(Qt::black,4,Qt::SolidLine);
    paint.setPen(pen);

    drawaxis(&paint);
    //draw the curves after finished test
    if(showPixtureScreen)
    {
        temppix=pixmap;

        QPainter curvespaint(&temppix);

        drawfinalshow(&curvespaint);
        FinihedTestDraw(&curvespaint,paintData);//paint with paintData

        paint.drawPixmap(xwidth,yposition-rectheight,temppix);
    }
}

//print the information of the patient
void CheckShow::drawaxis(QPainter *painter)
{
    QPainterPath mypainter;
    int i=0;
    mypainter.moveTo(xwidth,yposition);
    //paint the Yaxis
    //here paint the Y
    drawBlack(painter);
    QPen pen(Qt::blue,1,Qt::SolidLine);
    painter->setPen(pen);
    for(i=0;i<YCount+1;i++)
    {
        painter->drawLine(xwidth,yposition-Yaxisgap*i*Ylarge,xwidth+Axis_width,yposition-Yaxisgap*i*Ylarge);
    }
    for(i=0;i<XCount+1;i++)
    {
        painter->drawLine(xwidth+Xaxisgap*i,yposition,xwidth+Xaxisgap*i,yposition-rectheight);
    }
    painter->setPen(pen);

    for(i=0;i<YCount+1;i++)
    {
        painter->drawText(xwidth-42,yposition-Yaxisgap*i*Ylarge+10,QString("%1").arg(Yaxisgap*minustoshow*i,4));
        if(i==YCount)
            painter->drawText(xwidth-42,yposition-Yaxisgap*i*Ylarge-10,"(mv)");
        //Add DataMinus is as our data has minused for 100times(DataMinus)
    }
    rectheight=Yaxisgap*YCount*Ylarge;
    for(i=1;i<XCount+1;i++)//fom 1 on,because y axis has paint 0
    {
        //as we show in x is two by two
        //painter->drawText(xwidth+Xaxisgap*i-20,yposition+12,QString("%1").arg(Xaxisgap*NpointsDrawOnce*i*4*2/xdrawpointgap,5));//NpointsDrawOnce:draw a point for each 3 points,multify 4 is number is form 300->1200  5/3 is before use 50 as xcount now is 30
        painter->drawText(xwidth+Xaxisgap*i-40,yposition+17,QString("%1").arg(i,5));//NpointsDrawOnce:draw a point for each 3 points,multify 4 is number is form 300->1200  5/3 is before use 50 as xcount now is 30
        //minus xdrawpointgap because:(As usual ,we draw point 1by 1,while now,each point's gap is xdrawpointgap)
    }
}

//show the countdown interface
void CheckShow::ShowCountDown()
{
    count=new CountDown;
    count->move(120,165);
    count->exec();
    //when you pressed 'ok' in countdown,stepper goes in
}

//calcuate the Consistency by a formula
float CheckShow::switchToConsistency(float peak)//peak is the highest number in  curves
{
    /*At here switch the data to Consistency
First, log10(AnyData) and the Consistency have also been log10
And the Formula is:Y=1.998*x-3.1187;
Y stands for Consistency;
X stands for log10(data),Y has also been log10

so after we found out y,we get 10^Y,
and it has been dilluted for 100 times,and it's unit is ng/ml,we change it to mg/L
so ,the really Consistency is 10^Y*100/1000=10^Y/10 mg/L
*/

    //read the formula from file coefficient.txt

    //Here Use QScript to calcuate formula
    QString formula;
    //we save lots of formula in flash,and we select the right formula file by Bach_ID
    QString fileName;
    bach_ID_show=ui->comboBox_Batch->currentText();

    Project_Calcuate=ui->comboBox_Type->currentText();
    fileName="formula/"+Project_Calcuate+"_"+bach_ID_show+".txt";//name format:Bach_idformula.txt
    QFile file(fileName);//in arm its name is disk
    if(file.open(QIODevice::ReadOnly)<0)
        QMessageBox::about(NULL,"checkshow","Read error!");
    QTextStream read(&file);
    formula=read.readAll();//read the formula

    QScriptEngine engine;
    QString fun="(function(x){return ";
    formula+="})";
    formula=fun+formula;
    QScriptValue res=engine.evaluate(formula);
    QScriptValueList dataValue;
    //as we should log10peak,so here should change
    peak=log10(peak);
    dataValue<<peak;

    QScriptValue result=res.call(QScriptValue(),dataValue);
    //here change the formula
    float finalresult=result.toNumber();
    finalresult=pow(10,finalresult)/10;
    return finalresult;
}

float CheckShow::switchToConsistency_AT(float AT)//peak is the highest number in  curves
{
    /*At here switch the data to Consistency
      formua is Consistency=(AT+277.33)/31.087
    */

    //read the formula from file coefficient.txt

    //Here Use QScript to calcuate formula
    QString formula;
    QString fileName;
    bach_ID_show=ui->comboBox_Batch->currentText();
    Project_Calcuate=ui->comboBox_Type->currentText();
    fileName="formula/"+Project_Calcuate+"_"+bach_ID_show+".txt";//name format:Bach_idformula.txt

    QFile file(fileName);//in arm its name is disk
    if(file.open(QIODevice::ReadOnly)<0)
        QMessageBox::about(NULL,"checkshow","Read error!");
    QTextStream read(&file);
    formula=read.readAll();//read the formula

    QScriptEngine engine;
    QString fun="(function(x){return ";
    formula+="})";
    formula=fun+formula;
    QScriptValue res=engine.evaluate(formula);
    QScriptValueList dataValue;
    //float peak1=600;
    //as we should log10peak,so here should change

    dataValue<<AT;
    QScriptValue result=res.call(QScriptValue(),dataValue);
    //here change the formula
    float finalresult=result.toNumber();

    return finalresult;
}


void CheckShow::drawBlack(QPainter *paint)
{
    QRect rect(xwidth,yposition-YCount*Yaxisgap,Xaxisgap*XCount,rectheight);
    paint->fillRect(rect,QBrush(QColor(0,0,0)));
}

void CheckShow::SaveAllArea(QString name)
{
    qDebug()<<"CheckShow::SaveAllArea";
    QString fileName;

    int i=0;

    fileName="points/" + name + QString::number(saveFileName) +  ".txt";//create the file

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly)<0)
        QMessageBox::about(NULL,"checkshow","open saveArea error!");
    QTextStream in(&file);
     in<<"firstArea    "<<First_Area<<endl;
     in<<"secodeArea    "<<Second_Area<<endl;
}

void CheckShow::SaveAllPoints(double *data,QString name,int total)
{//暂未修改
    //points file saved as name "BachId+points.txt"
    QString fileName;

    int i=0;

    fileName="points/" + name + QString::number(saveFileName) +  ".txt";//create the file

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly)<0)
        QMessageBox::about(NULL,"checkshow","open points error!");
    QTextStream in(&file);
    for(i=0;i<total;i++)
        in<<(data[i])<<endl;
    if(name == "orgin")
        saveFileName++;
}

//发现用QTestStream类写文件需要的时间太久，所以改用C++的ofstream类写
void CheckShow::SaveAllPointsByCPP(double *data, QString name, int total)
{
    qDebug()<<"CheckShow::SaveAllPointsByCPP";
    std::ofstream fout;
    QString fileName;
    int i=0;
    fileName="points/" + name + QString::number(saveFileName) +  ".txt";//create the file

    fout.open(fileName.toStdString().c_str());

    for(i=0; i<total; i++)
        fout << (data[i]) << '\n';

    if(name == "orgin")
        saveFileName++;
}

void CheckShow::createdirectory()
{
    mkdir("/opt/points",S_IRWXU);//create a new directory
    mkdir("/opt/formula",S_IRWXU);//create a new directory
    mkdir("/opt/database",S_IRWXU);//create a new directory
}
void CheckShow::onTestCountSet()
{
    for(int i = 0; i < mTestCount; i++)
    {
        set_linear_in();
        QCoreApplication::processEvents();
    }
}

void CheckShow::set_linear_in()
{  
    disconnect(ui->pushButton_Out, SIGNAL(clicked()), this, SLOT(Step_in_or_out()));
    disconnect(ui->pushButton_Test,SIGNAL(clicked()),this,SLOT(set_linear_in()));
    disconnect(ui->pushButton_save,SIGNAL(clicked()),this,SLOT(ResultSave()));
    disconnect(ui->pushButton_print,SIGNAL(clicked()),this,SLOT(printInfo()));
    disconnect(ui->pushButton_Exit,SIGNAL(clicked()),this,SLOT(home()));
    disconnect(ui->pushButton_LogOut,SIGNAL(clicked()),this,SLOT(LogOut()));

    if(ui->comboBox_Batch->currentText() == ""
            || ui->comboBox_Type->currentText() == "")
    {
        warningerror=new Mywarning;
        warningerror->move((800-warning->width())/2,(480-warning->height())/2);
        warningerror->setText(QString::fromUtf8("没有项目号或批次号"));
        warningerror->exec();
    }else{
        showPixtureScreen=false;
        update(xwidth-40,yposition-YCount*Yaxisgap,Xaxisgap*XCount+40,rectheight);

        m3352.urt_wf.clearSeral();
        m3352.setFreq(255);
        m3352.Motor_in();

        QTime OcDieTime = QTime::currentTime().addMSecs(2000);  //ms
        while(1)
        {
            if(1==m3352.urt_wf.DataIn(0))//读到内侧光耦就退出
            {
                qDebug() << "read OC successfully";
                break;
            }

            //解决如果没读到串口数据引发死循环问题
            if( QTime::currentTime() >= OcDieTime )
            {
                //rebootWarning = new warning_two(this);
                //rebootWarning->move((800-Backupwarning->geometry().width())/2,(480-Backupwarning->geometry().height())/2);
               // connect(rebootWarning,SIGNAL(Pressed_Yes()),this,SLOT(ReallyReboot()));
                //disconnect(Backupwarning,SIGNAL(Pressed_Yes()));
               // rebootWarning->setText(QString::fromUtf8("读内侧光耦出错，是否重启？"));
               // rebootWarning->exec();
                warning->move((800-warning->width())/2,(480-warning->height())/2);
                warning->setRight();
                warning->setText(QString::fromUtf8("读内侧光耦出错, 请检查"));
                warning->exec();
                break;
            }
        }
        char *data3 ;
        data3=new char[10000];
        int i=0;
        int len=0;

        m3352.LED_open();
        sleep(2) ;
        int tiaodai = TestStrips->GetNumber(ui->comboBox_Type->currentText()+"_"+ui->comboBox_Batch->currentText()) ;
        qDebug()<<"tiaodai="<<tiaodai;

        if(tiaodai==2)
            m3352.setFreq(225);
        else if(tiaodai==6)
            m3352.setFreq(mTestFreq);
        m3352.urt_wf.clearSeral();
        m3352.Motor_out();


//        m3352.urt_wf.DataIn(0);
        m3352.SPI_open();

        QTime DataDieTime = QTime::currentTime().addMSecs(10000);  //ms
        while(1)
        {
            int len2=m3352.urt_wf.DataIn(1);
            for(i=len;i<len+len2;i++)
                data3[i]=m3352.urt_wf.data[i-len];
            len+=len2;
            if(len>=2400)
                break;
            //解决如果没读到串口数据引发死循环问题
            qDebug()<<"QTime::currentTime()"<<QTime::currentTime()<<"DataDieTime"<<DataDieTime;
            if(QTime::currentTime() >= DataDieTime)
            {
                qDebug()<<"hrehrehrhe";
                warning->move((800-warning->width())/2,(480-warning->height())/2);
                warning->setRight();
                warning->setText(QString::fromUtf8("读串口数据出错, 请检查"));
                warning->exec();
                break;
            }
        }

        m3352.SPI_close();

        data3[len]='\0';

        //保存AD数据
        for(i=0;i<2400;i++)
            AD[i] = data3[i] ;

//                SaveAllPoints(AD,"AD",2400) ;
        SaveAllPointsByCPP(AD, "AD", 1200);

        for(i=0;i<1200;i++)
        {//转换
            Savedata[i]=(data3[i*2]*256+data3[i*2+1])/6.5535;
        }

//        SaveAllPoints(Savedata,"orgin",1200) ;
        SaveAllPointsByCPP(Savedata,"orgin",1200) ;
        delete [] data3;
        m3352.LED_close();
        m3352.Motor_close();//电机停止
        NextMoveDirection=PWM_IOCTL_SET_FREQ_IN;

        if(isSelfCheck==true)
        {
            isSelfCheck=false ;
            return ;
        }

        if(isCalibrate==true)
            return ;

        finished_Test();
    }
}

int CheckShow::CheckStatusBefore()
{
    int i=0;
    int ret=0;
    char buffer[]={'0','0'};
    char current[]={'0','0'};
    fd_set readfd;
    struct timeval timeout;

    timeout.tv_usec=50;
    while(!meetphotodiodeds)
    {
        FD_ZERO(&readfd);
        FD_SET(photodiodeds,&readfd);
        ret=select(photodiodeds+1,&readfd,NULL,NULL,&timeout);
        printf("selecting..........\n");
        if(FD_ISSET(photodiodeds,&readfd))
        {
            memset(buffer,'0',2);
            read(photodiodeds,buffer,2);
            for(i=0;i<2;i++)
            {
                printf("the %d photodiodeds is %c",i,buffer[i]);
                if(current[1]!=buffer[i])
                {
                    meetphotodiodeds=true;// a interrupt meetphotodiodeds
                    if(0==i)
                        printf("it is inside \n");
                    if(1==i)
                    {
                        printf("it is outer\n");
                    }

                    return i;
                }
            }
        }
    }
}

void CheckShow::finished_Test()
{
    qDebug()<<"CheckShow::finished_Test";
    smooth(10);//smooth the data and save it in smoothSavedata[]
    update(xwidth-20,yposition-YCount*Yaxisgap,Xaxisgap*XCount+20,rectheight);
    isDraw=false;
    weatherdrawsmooth=false;
    saveDataCount=0;
    smoothtemp=0;
    showPixtureScreen=true;//now show one picture on screen

    switch(TestStrips->GetNumber(ui->comboBox_Type->currentText()+"_"+ui->comboBox_Batch->currentText()))
    {
    case 2:
        ui->label_Height1->hide();/*先隐藏6条带的一些参数*/
        ui->label_Height2->hide();
        ui->label_Height3->hide();
        ui->label_Height4->hide();
        ui->label_Height5->hide();
        ui->label_Height6->hide();

        ui->label_Area1->show();
        ui->label_Area2->show();
        Search_peaks(SmoothSaveData,Savedata);

        //SaveAllPoints(Savedata,"orgin",1200) ;
        //SaveAllPoints(Savedata,"smooth",1200);//保存平滑后的数据
        SaveAllArea("area");
        ui->label_Area1->setText(QString::number(First_Area));
        ui->label_Area2->setText(QString::number(Second_Area));

        //deal_paintdata();
        qDebug()<<"new11"<<begin;
        qDebug()<<"12"<<end;
        qDebug()<<"21"<<second_begin;
        qDebug()<<"22"<<second_end;
        showResult();
        break;
    case 6:
        ui->label_Area1->hide();
        ui->label_Area2->hide();
        double *heightpoint=(double *)malloc(sizeof(double)*6);
        heightpoint=TestStrips->Six_Curves_Height(SmoothSaveData);
        ui->label_Height1->show();
        ui->label_Height2->show();
        ui->label_Height3->show();
        ui->label_Height4->show();
        ui->label_Height5->show();
        ui->label_Height6->show();

        ui->label_Height1->setText(QString::number(heightpoint[0]));
        ui->label_Height2->setText(QString::number(heightpoint[1]));
        ui->label_Height3->setText(QString::number(heightpoint[2]));
        ui->label_Height4->setText(QString::number(heightpoint[3]));
        ui->label_Height5->setText(QString::number(heightpoint[4]));
        ui->label_Height6->setText(QString::number(heightpoint[5]));

        Genotype *judgegenotype=new Genotype;
        char *type=judgegenotype->MainFunction(heightpoint);
        ui->label_value->setText(QString::fromLocal8Bit(type));
        break;
    }

    //解决连续点击按钮会响应多次问题
    QTime dieTime = QTime::currentTime().addMSecs(500);  //0.5s
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);  //max 1s

    connect(ui->pushButton_Out, SIGNAL(clicked()), this, SLOT(Step_in_or_out()));
    connect(ui->pushButton_Test,SIGNAL(clicked()),this,SLOT(set_linear_in()));
    connect(ui->pushButton_save,SIGNAL(clicked()),this,SLOT(ResultSave()));
    connect(ui->pushButton_print,SIGNAL(clicked()),this,SLOT(printInfo()));
    connect(ui->pushButton_Exit,SIGNAL(clicked()),this,SLOT(home()));
    connect(ui->pushButton_LogOut,SIGNAL(clicked()),this,SLOT(LogOut()));

    ui->pushButton_Test->setEnabled(true);
    ui->pushButton_Out->setEnabled(true);
    ui->pushButton_save->setEnabled(true);
    ui->pushButton_print->setEnabled(true);
    ui->pushButton_Exit->setEnabled(true);
    ui->pushButton_LogOut->setEnabled(true);
}

int CheckShow::GetCurrentPosition()
{
    return (T_position+C_position)/2;
}

//Change date:2013-4-9
//functioin:Used to show a smooth curves
void CheckShow::showAverage(int point)
{
    float temp;
    if(point<showSmooth-1)
    {
        showSmoothNumber[point]=Savedata[point];
    }
    int i;
    for(i=(point-showSmooth);i<(point-1);i++)
    {
        temp+=Savedata[point];
    }
    showSmoothNumber[point]=temp/showSmooth;
}

//CHANGE
//DATE:when we finished Test,then show the picture
void CheckShow::FinihedTestDraw(QPainter *paint,double *Savedata)
{
    QPainterPath painter;
    QPen pen(Qt::red,2,Qt::SolidLine);
    paint->setPen(pen);

    int i=0;
    int j=0;
    //    int total = 1200 ;

    //    i
    //        total = second_end - begin + 100;
    //        //假如显示980个点，其中实现方法是：截取前后各110个点
    //        //最终前后各加上了20个点
    //        if(total>1200)
    //            total = 1200 ;
    //        reallyDrawpointgap = 3600.0 / total ;
    //        //根据1200的时候是3，来调整为括号中长度时适应度为1200*3/(second_end - begin)
    //    }else {//6条带的时候
    //        total = 1200 ;
    //        reallyDrawpointgap=3.0;
    //    }
    while(j<1200)
    {//并且点数要小于1200个点
        painter.moveTo(i-reallyDrawpointgap,rectheight-Savedata[j-drawpointgap]);
        painter.lineTo(i,rectheight-Savedata[j]);

        paint->drawPath(painter);
        i=i+reallyDrawpointgap;
        j=j+drawpointgap;
    }
}

void CheckShow::SaveFileToSD(float *Savedata)
{
    QString fileName;
    int i=0;
    QString pointname=QDateTime::currentDateTime().toString();

    fileName="sdcard/"+QString::number(saveFileName)+".txt";//create the file
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly)<0)
        printf("open error!\n");
    QTextStream in(&file);
    for(i=0;i<TOTAL;i++)
    {
        in<<(Savedata[i])<<endl;
    }
    saveFileName++;
}

void CheckShow::SaveConsistency(QString ConsistencyName)
{
    QFile ConsistencyFile(ConsistencyName);
    if(ConsistencyFile.open(QIODevice::Append)<0)
        printf("open error!\n");
    QTextStream consis(&ConsistencyFile);

    consis<<(Consistency)<<endl;
}

int CheckShow::automaticallyScales()
{
    //search for the highest number
    int i=0;
    int scales=0;
    float max=0;
    int point=0;
    for(i=0;i<TOTAL;i++)
    {
        if(SmoothSaveData[i]>max)
        {
            max=SmoothSaveData[i];
            point=i;
        }
        //find the max value,and compare it to
    }
    //minustoshow is that we used to show so i minus the number
    scales=(max)/(Yaxisgap*(YCount));//the highest number is Yaxisgap*10*(YCount-1)*minustoshow
    scales=scales/10;
    scales++;
    return scales;
}

float CheckShow::correct(float fbt)
{
    float temp1=0,temp2=0,correctValue=0;

    float D1=RealValue[0];
    float D2=RealValue[1];
    float D3=RealValue[2];
    float D4=RealValue[3];
    float D5=RealValue[4];
    float D6=RealValue[5];
    float AV_D1=StandardValue[0];
    float AV_D2=StandardValue[1];
    float AV_D3=StandardValue[2];
    float AV_D4=StandardValue[3];
    float AV_D5=StandardValue[4];
    float AV_D6=StandardValue[5];
    int i=0;
    if (( D1>0)&&( D2>0)&&( D3>0)&&( D4>0)&&( D5>0)&&( D6>0))
    {
        if (fbt< D1)
        {
            temp1=fabs(fbt-0)/(fabs(fbt- D1)+fabs(fbt-0))*( D1- AV_D1);
            correctValue=fbt-temp1;
        }
        if ((fbt>= D1)&&(fbt< D2))
        {
            temp1=fabs(fbt- D1)/(fabs(fbt- D2)+fabs(fbt- D1))*( D2- AV_D2);
            temp1=temp1+fabs(fbt- D2)/(fabs(fbt- D2)+fabs(fbt- D1))*( D1- AV_D1);
            correctValue=fbt-temp1;
        }
        if ((fbt>= D2)&&(fbt<= D3))
        {
            temp1=fabs(fbt- D3)/(fabs(fbt- D2)+fabs(fbt- D3))*(( D2- AV_D2));
            temp1=temp1+fabs(fbt- D2)/(fabs(fbt- D2)+fabs(fbt- D3))*(( D3- AV_D3));
            correctValue=fbt-temp1;
        }
        if ((fbt>= D3)&&(fbt<= D4))
        {
            temp1=fabs(fbt- D4)/(fabs(fbt- D3)+fabs(fbt- D4))*(( D3- AV_D3));
            temp1=temp1+fabs(fbt- D3)/(fabs(fbt- D3)+fabs(fbt- D4))*(( D4- AV_D4));
            correctValue=fbt-temp1;
        }

        if ((fbt>= D4)&&(fbt<= D5))
        {
            temp1=fabs(fbt- D5)/(fabs(fbt- D4)+fabs(fbt- D5))*(( D4- AV_D4));
            temp1=temp1+fabs(fbt- D4)/(fabs(fbt- D4)+fabs(fbt- D5))*(( D5- AV_D5));
            correctValue=fbt-temp1;
        }

        if ((fbt>= D5)&&(fbt<= D6))
        {
            temp1=fabs(fbt- D6)/(fabs(fbt- D5)+fabs(fbt- D6))*(( D5- AV_D5));
            temp1=temp1+fabs(fbt- D5)/(fabs(fbt- D5)+fabs(fbt- D6))*(( D6- AV_D6));
            correctValue=fbt-temp1;
        }

        if ((fbt>= D6))
        {
            temp1=(( D6- AV_D6));
            correctValue=fbt-temp1;
        }
        return correctValue;
    }
    else
        return fbt;
}

void CheckShow::ReadCalibrateFile()//when your input optioin is pressed_ok,then you do this function
{
    //QString testType=showOption->getProjectName();
    QString name="calibrateFile.txt";
    QFile CalibrateFile(name);
    int i=0;
    if(CalibrateFile.open(QIODevice::ReadOnly)<0)
        perror("open file error");
    QFileInfo info(name);
    if(!info.exists())
    {
        for(i=0;i<6;i++)
        {
            StandardValue[i]=0;
            RealValue[i]=0;
        }
    }
    QTextStream stream(&CalibrateFile);

    QString line;
    int count=0;
    while(!stream.atEnd())
    {
        line=stream.readLine();//read the txt file line by line
        if(!line.isEmpty())
        {
            StandardValue[count]=line.section(",",1,1).toFloat();
            RealValue[count]=line.section(",",2,2).toFloat();
        }
        else
        {
            StandardValue[i]=0;
            RealValue[i]=0;
        }
        count++;
    }
    CalibrateFile.close();
}

void CheckShow::sendFinished()
{
    emit(CheckShowFinished());
}
void NewAlgorithm()
{
}

QString CheckShow::GetDelayTime()
{
    QString fileName="DelayTime.txt";
    QString DelayTime=NULL;
    QFile file(fileName);//in arm its name is disk
    if(file.open(QIODevice::ReadOnly)<0)
        QMessageBox::about(NULL,"checkshow","Read DelayTime.txt error!");
    QTextStream read(&file);
    DelayTime=read.readAll();
    //偏移这些距离需要多少个点
    //采集一个点大概需要3955us，采集1200个点大概4.75s
    //那么总共偏移length个位置，对应需要的时间为Ys
    // 1200.t/1200=Y/length;Y=length*t;
    //Y=length*3955us;

    file.close();
    if(DelayTime.isEmpty())
        return 0;

    return DelayTime;
}

void CheckShow::delay(unsigned char second)
{
    int i,j=0;
    while(second--)
    {
        for(i=0;i<33;i++)
        {
            j=1;
        }
    }
}

//here below is mr tang's algorithm

void findpeak(int n, int npeak0, int *npeak, real_T *x0, real_T peaks[600], real_T newpeaks[600])
{
    real_T resolution=10;  /// 根据需要设置分辨率
    double npeak1;
    emxArray_real_T *x;
    emxInit_real_T(&x, 2);
    x->size[0] = n ;
    x->size[1] = 1;
    x->data=x0;
    findpeakcore(npeak0,resolution,x, &npeak1, peaks, newpeaks);
    *npeak=npeak1;
    x->data=0;
    emxFree_real_T(&x);
}
void  lyq_pinghua(int data[],int windowsize,int i)
{
    for(int j=windowsize/2;j<i-windowsize/2;j++)
    {
        int avg = 0,sum = 0;
        for(int k=j-windowsize/2;k<windowsize+j-windowsize/2+1;k++)
        {
            sum += data[k];
        }
        avg = sum/(10.0);
        data[j]=avg;
    }
}

void  lyq_qiudao(int data[],int i,double daoshudata[])
{

    for(int j=0;j<i;j++)
    {
        if(j==0)
        {
            daoshudata[j]=data[j+1]-data[j];
        }
        else if(j==i-1)
        {
            daoshudata[j]=data[j]-data[j-1];
        }
        else
        {
            daoshudata[j]=(data[j+1]-data[j-1])/2.0;

        }
    }
}

void lyq_test(int data[],int i,double daoshudata[],int result[])
{
    int testwindowsize=10;

    double avg=0.0;
    bool flag1 = false;
    bool flag2 = false;
    int feng[4]={0};
    int c = 0;
    char output[10];
    for(int j=0;j<i-testwindowsize+1;j++)
    {
        if(c==4) break;
        double sum = 0.0;
        for(int k=j;k<testwindowsize+j;k++)
        {
            sum += daoshudata[k];
        }
        avg=sum/testwindowsize;
        if(avg>=0.4 && j>=200 && j<=1100 && !flag1)
        {
            flag1 = true;
            feng[c] = j;
            c++;
        }
        else if(flag1 && avg<-0.5 && !flag2)
        {
            flag2 = true;
        }
        else if(flag1 && flag2 && (avg>=-0.5||j==1120))
        {
            flag1 = false;
            flag2 = false;
            feng[c]=j;
            c++;
        }
    }
    int bendi[2]={(data[feng[0]]+data[feng[1]])/2,(data[feng[2]]+data[feng[3]])/2};
    for(int j=0;j<c;j++)
    {
        if(j<2)
        {
            result[j]=feng[j];
        }
        else
        {
            result[j+1]=feng[j];
        }
    }
    for(int j=0;j<c;j+=2)
    {
        int mianji=0;
        for(int x=feng[j];x<feng[j+1];x++)
        {
            mianji+=(data[x]-bendi[j/2]);
        }
        if(j==0)
        {
            result[2]=mianji;
        }
        else if(j==2)
        {
            result[5]=mianji;
        }
    }
}

int Search_peaks(double *SmoothSaveData, double *Savedata)
{
    double result[2]={0.0,0.0} ;
    checkTest(SmoothSaveData,1200,result,Savedata);
//    qCheckTest(Savedata,1200,result);
    First_Area = result[0] ;
    Second_Area = result[1] ;
    qDebug()<<"yiqisuanfa"<<"firstArea"<<First_Area<<"secondArea"<<Second_Area;
    if(Second_Area>=1.0)
        AreaDivided=First_Area/Second_Area;//增加一个判断second_area是否为零的判断
    //    int n=1200;
    //    int npeak0=6, npeak;
    //    int i ;
    //    double peaks[600], newpeaks[600];  // 设置最大100个峰，如果10个，则定义为real_T peaks[60], newpeaks[60];

    //    findpeak(n,npeak0, &npeak, SmoothSaveData, peaks, newpeaks);
    //    ////////////////////////////////////////////////////
    //    //at first ,some points are useless,we should drop it,drop point
    //    //here show the finalResult fo findpeak
    //    int max_pos,submax_pos;
    //    max_pos=500;
    //    submax_pos=500;
    //    int tmp=-1;

    //    for(i=500;i<600;i++)
    //    {//寻找最大值
    //        if(tmp<peaks[i])
    //        {
    //            tmp=peaks[i];
    //            max_pos=i;
    //        }
    //    }
    //    tmp=-1;
    //    for(i=500;i<600;i++)
    //    {//寻找第二个最大值
    //        if(tmp<peaks[i]&&i!=max_pos)
    //        {
    //            tmp=peaks[i];
    //            submax_pos=i;
    //        }
    //    }
    //    int first_pos;
    //    int second_pos;
    //    if(peaks[max_pos-500]<peaks[submax_pos-500])
    //    {//比较两个峰的位置,这种情况是大峰在前面
    //        first_pos=max_pos;
    //        second_pos=submax_pos;
    //    }else {//大面积峰在后面
    //        first_pos=submax_pos;
    //        second_pos=max_pos;
    //    }

    //    T_position=peaks[0];//T线位置
    //    C_position=peaks[second_pos-500];//C线位置
    //    begin=peaks[first_pos-400];//第一个波峰起点
    //    second_begin=peaks[second_pos-400];//第二个波峰起点
    //    end=peaks[first_pos-300];//第一个波峰终止点
    //    second_end=peaks[second_pos-300];//第二个波峰终止点

    //    First_width=peaks[first_pos-200];
    //    Second_width=peaks[second_pos-200];
    //    if(peaks[second_pos] == 0 && end>800)//如果只有一个值，且结束点在800以后，则是C线的值.因为地一个波峰的终点不可能在800
    //    {
    //        First_Area=0;
    //        Second_Area=peaks[first_pos];
    //    }else{
    //        First_Area=peaks[first_pos];
    //        Second_Area=peaks[second_pos];
    //    }
    //    AreaDivided=First_Area/Second_Area;//增加一个判断second_area是否为零的判断
    //    qDebug()<<"Savedata[3]="<<Savedata[3];
    //    qDebug()<<QString::fromUtf8("          第一个      第二个");
    //    qDebug()<<QString::fromUtf8("峰坐标：")<< T_position<<"\t"<<C_position;
    //    qDebug()<<QString::fromUtf8("峰起点：")<< begin<<"\t"<<second_begin;
    //    qDebug()<<QString::fromUtf8("峰终点：")<< end<<"\t"<<second_end;
    //    qDebug()<<QString::fromUtf8("峰宽度：")<< First_width<<"\t"<<Second_width;
    //    qDebug()<<QString::fromUtf8("峰面积：")<< First_Area<<"\t"<<Second_Area;
    //    qDebug()<<QString::fromUtf8("AT／AC：")<< AreaDivided;
    return 0;
}

void CheckShow::LogOut()
{
    delete this;
    qApp->exit(773);
}

void CheckShow::timerEvent(QTimerEvent *e)
{
    QDateTime datatime=QDateTime::currentDateTime();
    QString Stringcurrentime=datatime.time().toString();
    QDateTime CurrentDate=QDateTime::currentDateTime();
    QString YearDate=CurrentDate.toString("yyyy.MM.dd");
    ui->label_Time->setText(YearDate);
    ui->label_date->setText(Stringcurrentime);
}


void CheckShow::BeginRepeatTest()
{
    isCalibrate=true;
    for(int i=0;i<RepeatTimes;i++)
    {
        sleep(3);
        set_linear_in();//获得数据
        smooth(10);//平滑
        Search_peaks(SmoothSaveData,Savedata);//找波峰
        repeatTestValues[i]=AreaDivided;//get areadivided
    }
    gettheaverage();
    isCalibrate=false;
}

float CheckShow::gettheaverage()
{
    float sum=0.0;
    float max=0;
    float min=repeatTestValues[0];
    float average;
    int i=0;
    while(i<RepeatTimes)
    {
        if(repeatTestValues[i]>max)
            max=repeatTestValues[i];
        if(repeatTestValues[i]<min)
            min=repeatTestValues[i];
        sum+=repeatTestValues[i];
        i++;
    }
    average=(sum-max-min)/(RepeatTimes-2);
    QString QStringAverage=QString::number(average,'f',4);
    emit RepeattestValue(QStringAverage);
    return average;
}
void CheckShow::drawfinalshow(QPainter *painter)
{
    int i=0;
    QPen pen(Qt::blue,1,Qt::SolidLine);
    painter->setPen(pen);
    for(i=0;i<YCount+1;i++)
    {
        painter->drawLine(0,rectheight-Yaxisgap*i*Ylarge,Axis_width,rectheight-Yaxisgap*i*Ylarge);
    }
    for(i=0;i<XCount+1;i++)
    {
        painter->drawLine(0+Xaxisgap*i,rectheight,Xaxisgap*i,rectheight-rectheight);
    }
}

void CheckShow::deleteCurves()
{
    showPixtureScreen=false;
    update();
}

void CheckShow::exceptionjudgement(float AC,float AT)
{
    if(AT<1.0 || AC <1.0)
    {
        warningerror=new Mywarning;
        warningerror->move((800-warning->width())/2,(480-warning->height())/2);
        warningerror->setText(QString::fromUtf8("请核实测试结果"));
        warningerror->exec();
    }
}

bool CheckShow::whetherNegative(int T,int C)
{
    //阴性纸条判断方法
    //根据张晶的结果是，如果是阴性纸条，那么实际上只有一个峰，但是程序会错误的寻找到两个峰，
    //算法寻找的俩个峰特征如下：
    //算法把自动寻找到的第一个峰作为AT，而此时第一个峰是AC，所以检测到的AT位置在AC范围内
    //实际上只有一个峰，而算法为了满足两个峰的条件，则从后面随意找一个非目标峰，此目标峰的面积极小（因为纸条只有一个峰，其余皆背景）
    //综合起来，特征：第一个峰在AC位置，面积是AC面积，第二个峰在其后，面积极小
    //满足以上两个特征，可作判断，此为阴性纸条
    //AT代表第一个波峰的最高点位置，AC代表第二个波峰的最高点位置
    //还有一种情况，波峰寻找位置没有错误，有两个峰，但是T峰很小，其阈值<3000，C峰正常，AC是正常的，这个也是阴性纸条
    //波峰寻找错误情况
    bool Negative=false;
    if(650<T<1150&&(C>T||C==T))//T落在C范围，C在T后面
        if(First_Area/Second_Area>NegativeAreaThreshold)//AT此时就是AC面积，AC非目标峰面积极小，所以两者比值要大于一个阈值
            Negative=true;
    //波峰寻找正确情况，此时波峰比值应该很小
    if(100<T<650&&AreaDivided<0.005)
        Negative=true;
    return Negative;
}

void CheckShow::initiateInputDialog()
{
    ui->lineEdit_ID->setText("");
    ui->lineEdit_ID->setEnabled(false);//帐号是不能输入的
    ui->lineEdit_ID->setFocus();//get the focus
    ShowID();//显示检测帐号，每次都增加，但是只有保存之后才会将这个ID写入到文件中

    ui->comboBox_Gender->addItem("M");
    ui->comboBox_Gender->addItem("F");
    ShowProjectName("");
    Pro_change();//加载第一次的批次号
    ui->label_operator->setText(inputUserName);
    //you can push 'out' when you come in first
}

void CheckShow::ShowID()
{
    QFile file("/opt/Patient_ID.txt") ;//text.txt放在可执行文件夹下
    //若要在text/text.txt文件夹下，就要用file("text/text.txt")
    file.open(QIODevice::ReadOnly);
    QString line ;
    QTextStream stream(&file);//此处的file有'＆',勿忘记
    line = stream.readLine();//读取当前的帐号
    ui->lineEdit_ID->setText(line);
    file.close();
}

void CheckShow::ShowProjectName(QString pro)
{//pro="";表示当前只是加载项目号；不为空的时候表示要加载项目对应的批次号
    QStringList list ;//list保存的是形如：{"crp_111","tcp_222"}之类的
    QStringList list_Pro;//最终显示的项目号
    QStringList temp ;
    QString str ;
    list.clear();
    list = Test.FindFormulaFileInSD("formula/");

    ui->comboBox_Batch->clear();

    for(int i=0;i<list.length();i++)
    {
        str = list[i];//str形如：“crp_111”
        temp = str.split('_');
        if(pro == ""){//当pro为空的时候才加载项目，也就是第一次
            list_Pro<<temp[0];
            for(int i=0;i<list_Pro.length()-1;i++)
                if(list_Pro[i] == temp[0])
                    list_Pro.removeLast();
        }//if
        if(pro == temp[0])
            ui->comboBox_Batch->addItem(temp[1]);
    }//for

    if(list_Pro.length()>0)
        ui->comboBox_Type->addItems(list_Pro);
}

void CheckShow::Pro_change()
{//根据当前项目，动态加载该项目所对应的批次号
    QString Pro = ui->comboBox_Type->currentText();
    ShowProjectName(Pro);

    ui->label_data->setText("");
    ui->label_value->setText("");//将两个值清空
}

void CheckShow::acitveButton(QString)
{
    if(!ui->lineEdit_ID->text().isEmpty()&&!ui->lineEdit_Name->text().isEmpty())
        ui->pushButton_Out->setEnabled(true);
    else
        //        ui->pushButton_Out->setEnabled(false);
        ;
}

void CheckShow::ResultSave()
{
    repaint();
    showPixtureScreen=false;
    weatherSaved=true;

    if(CurrentDate.isNull()||YearDate.isNull())
    {
        CurrentDate=QDateTime::currentDateTime();
        YearDate=CurrentDate.toString("yyyy-MM-dd");
    }
    Bach_ID=ui->comboBox_Batch->currentText();

    patientID=ui->lineEdit_ID->text();
    TestType=ui->comboBox_Type->currentText();
    PatientName=ui->lineEdit_Name->text();
    PatientAge=ui->lineEdit_Age->text();
    PatientGender=ui->comboBox_Gender->currentText();
    GenderItem=ui->comboBox_Gender->currentIndex();
    inputUserName=ui->Label_UserName->text();

    warning->move((800-warning->width())/2,(480-warning->height())/2);

    SQLITE::sql->Insert(patientID,PatientName,PatientAge,PatientGender,
                        TestType,Bach_ID,ui->label_data->text(),ui->label_value->text(),
                        YearDate,CurrentDate.time().toString(),ui->lineEdit_operator->text());
    warning->setRight();
    warning->setText(QString::fromUtf8("信息保存成功"));
    warning->exec();
    //--------------------将ID加1，重新写入文件中-------------------------
    QFile file("/opt/Patient_ID.txt") ;//text.txt放在可执行文件夹下
    file.open(QIODevice::WriteOnly);
    QString line ;
    const QChar fillChar='0';
    QTextStream stream_id(&file);
    long num = ui->lineEdit_ID->text().toFloat();
    num ++ ;
    line = QString("%1").arg(num,7,10,fillChar);
    stream_id<<line;
    file.close();
    ShowID();//及时更新
}

void CheckShow::printInfo()
{
    showPixtureScreen=false;
    seriport_print=openSerialPort("/dev/ttyO4");
    if (seriport_print < 0) {
        warning->setText(QString::fromUtf8("打开串口失败!"));
        warning->show();
    }
    char *print;
    print=(char *)malloc(500*sizeof(char));
    int printlength;
    //just print English
    //below for english print**************************
    CurrentDate=QDateTime::currentDateTime();

    YearDate=CurrentDate.toString("yyyy:MM:dd");
    strcpy(print,"\nMagiCare RTR-FS100 打印报告");
    strcat(print,"\n");
    strcat(print,"\n");
    strcat(print,"\n签名：");
    strcat(print,"\n检测结果(a.u.)：");
    strcat(print,ui->label_value->text().toLatin1().data());
    strcat(print,"\n检测项目：");
    strcat(print,ui->comboBox_Type->currentText().toUtf8().data());
    strcat(print,"\n年龄：");
    strcat(print,ui->lineEdit_Age->text().toUtf8().data());
    strcat(print,"\n性别：");
    strcat(print,ui->comboBox_Gender->currentText().toUtf8().data());
    strcat(print,"\n姓名：");
    strcat(print,ui->lineEdit_Name->text().toUtf8().data());
    strcat(print,"\n账号：");
    strcat(print,ui->lineEdit_ID->text().toUtf8().data());
    strcat(print,"\n批次号：");
    strcat(print,ui->comboBox_Batch->currentText().toUtf8().data());//no need to show Serialnumber
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
    ::close(seriport_print);
}

int CheckShow::openSerialPort(const char *Seri_dev)
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

//用来打印中文
void CheckShow::miniPrintWriteUnicode(int fd,char *pwchello)
{
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
    //**********************************************************************
    int i=1;
    i=u2g(pwchello,500,szTmp,sizeof(szTmp));//Here DID not work
    sprintf(szPrint, "%s", szTmp);
    write(fd,szPrint,sizeof(szPrint));
    memset(szPrint, 0, sizeof(szPrint));
    szPrint[0] = 0x1c;
    szPrint[1] = 0x2e;

    write(fd,szPrint,2);
}

bool CheckShow::judgeweatherSaved(QString ID)
{
    //when you decide to save a list of message,you should judge weather it have saved before
    //if it have saved before ,replace it,while it maybe cost lots of time
    QFile ResultFile("Result/Result.txt");
    if(ResultFile.open(QIODevice::ReadOnly)<0)
    {
        warning->setText(QString::fromUtf8("保存文件打开失败！"));
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
            //there stays a record with same id
            break;
        }
    }
    ResultFile.close();
    return weathersaved;
}
int CheckShow::u2g(char *inbuf,int inlen,char *outbuf,int outlen)                        //UNICODEÂë×ªÎªGB2312Âë
{
    return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}
int CheckShow::code_convert(char *from_charset,char *to_charset,char *inbuf,unsigned int inlen,char *outbuf,unsigned int outlen)
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

void CheckShow::initshow()
{
    setWindowFlags(Qt::FramelessWindowHint);
    //    ui->pushButton_save->setEnabled(false);
    ui->label_data->setAlignment(Qt::AlignCenter);
    ui->label_value->setAlignment(Qt::AlignCenter);
    ui->label_data->setFont(QFont("helvetica",24,QFont::Light));
    ui->label_value->setFont(QFont("helvetica",24,QFont::Light));
    ui->lineEdit_operator->setText(inputUserName);
    ui->Label_UserName->setText(inputUserName);
    ui->lineEdit_operator->setEnabled(false);//帐号是不能输入的
    pixmap= QPixmap(Axis_width,rectheight);
    pixmap.fill(Qt::transparent);
    temppix.fill(Qt::transparent);

    warning=new Mywarning;
    warning->move((800-warning->width())/2,(480-warning->height())/2);
    timerEvent_Id=startTimer(1000);
    timerEvent(0);
    //test by marxsu
//    connect(ui->pushButton_Test,SIGNAL(clicked()),this,SLOT(set_linear_in()));
    connect(ui->pushButton_Test,SIGNAL(clicked()),this,SLOT(onTestCountSet()));

    //when you presised out,then a box full of message you need to input
    connect(ui->pushButton_Out, SIGNAL(clicked()), this, SLOT(Step_in_or_out()));
    connect(ui->pushButton_Exit,SIGNAL(clicked()),this,SLOT(home()));
    //    ui->pushButton_print->setEnabled(false);
    ui->pushButton_Exit->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Out->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_Test->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_print->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_save->setFocusPolicy(Qt::NoFocus);
}

void CheckShow::hideui()
{
    ui->frame->hide();
    ui->label_data->hide();
    ui->label_value->hide();
    ui->pushButton_Out->hide();
    ui->pushButton_print->hide();
    ui->pushButton_save->hide();
    ui->pushButton_Test->hide();
    ui->label->hide();
    ui->pushButton_LogOut->hide();
    ui->Label_UserName->hide();
    ui->pushButton_Exit->hide();
    ui->Label_SoftWareVersioin->hide();
    ui->label_6->hide();
    ui->label_2->hide();
    ui->label_data->hide();
    ui->label_3->hide();
    ui->label_value->hide();
}

void CheckShow::deal_paintdata()
{//平滑方法：取第一个峰起点，往前50个的平均值+3（-3）为当前平滑值
    //        取第二个峰终点，往后50个的平均值+3（-3）为当前平滑值
    /*int i = 0 ;
    float sum = 0.0 ;
    //求出前50个点的平均值
    for(i=begin-30;i>begin-130&&i>0;i--)
        sum += paintData[i];
    sum /= 50.0 ;
    //进行平滑
    int count = 0 ;
    for(i=begin;i>=0;i--)
    {
        //if(paintData[i]>sum+3.8||paintData[i]<sum-3.8)
            paintData[i] = paintData[begin]-0.005*count ;
            count++;
    }

    //---------------对尾部进行平滑----------------
    sum = 0.0 ;
    //求出后50个点的平均值
    for(i=second_end+30;i<second_end+130&&i<1200;i++)
        sum += paintData[i];
    sum /= 50.0 ;
    //进行平滑
    count = 0 ;
    for(i=second_end+30;i<1200;i++)
    {
        //if(paintData[i]>sum+3.8||paintData[i]<sum-3.8)
            paintData[i] = paintData[second_end] - 0.005*count ;
    }*/
    int i ;
    int total = second_end-begin+100;
    if(total > 1200)
        total = 1200 ;
    for(i=0;i<total;i++)
    {
        if((second_end+50)>=1200)//如果超出范围了则退出
            break ;
        paintData[i] = paintData[i+begin-50];
    }
}

QString OneKeyImport::Show_Qualitative(QString Project, QString Batch)
{
    QString data=NULL;//save dcimal and unit
    data=showContent(Project,Batch,6);//decimal

    return data;
}

void CheckShow::showResult()
{
    //correctAreaDivided=correct_3(AreaDivided);  //calibrate Areadivided
    exceptionjudgement(First_Area,Second_Area);
    //直接调用OneKeyImport类来实现结果计算，而不需使用这个函数switchToConsistency_AT
    //Test=new OneKeyImport;
    //判断读值是否超过了这个当前可以显示的范围

    QString decimalunit=NULL;
    QString Qualitative=NULL;

    Qualitative=Test.Show_Qualitative(ui->comboBox_Type->currentText(),ui->comboBox_Batch->currentText());

    if(Qualitative == "0")
    {//标志为0表示定性
        if(First_Area>=8000.0)
            ui->label_value->setText(QString::fromUtf8("阳性"));
        else
            ui->label_value->setText(QString::fromUtf8("阴性"));
        return ;
    }
    decimalunit=Test.Show_decimal_Unit(ui->comboBox_Type->currentText(),ui->comboBox_Batch->currentText());
    int decimal=decimalunit.section(' ',0,0).toInt();
    QString unit=decimalunit.section(' ',1,1);

    QString  Limit=Test.ShowLimit(ui->comboBox_Type->currentText(),ui->comboBox_Batch->currentText());
    float LowLimit=Limit.section(' ',0,0).toFloat();
    float  HighLimit=Limit.section(' ',1,1).toFloat();
    float tempConsistency=Test.DetermineFormula(ui->comboBox_Type->currentText(),ui->comboBox_Batch->currentText(),AreaDivided);

    if(tempConsistency<LowLimit)
    {//小于下限
        ui->label_data->setText(QString::number(AreaDivided,'f',4));
        ui->label_value->setText("<"+QString::number(LowLimit)+unit);
        return ;
    }
    if(tempConsistency>HighLimit)
    {//大于上限
        ui->label_data->setText(QString::number(AreaDivided,'f',4));
        ui->label_value->setText(">"+QString::number(HighLimit)+unit);
        return ;
    }
    bool negative=false;
    negative=whetherNegative(T_position,C_position);
    if(negative||QString::number(tempConsistency)<0)//是阴性纸条，那么赋值为0
    {
        tempConsistency=0.000;
    }

    Consistency=QString::number(tempConsistency,'f',decimal)+unit;
    ui->label_data->setText(QString::number(AreaDivided,'f',4));
    ui->label_value->setText(Consistency);
    ui->pushButton_Out->setEnabled(true);
}

float CheckShow::correct_3(float fbt)//used for 3 strips calibrate
{
    float temp1=0,temp2=0,correctValue=0;

    float D1=RealValue[0];
    float D2=RealValue[1];
    float D3=RealValue[2];

    float AV_D1=StandardValue[0];
    float AV_D2=StandardValue[1];
    float AV_D3=StandardValue[2];

    int i=0;

    if (( D1>0)&&( D2>0)&&( D3>0))
    {
        if (fbt< D1)
        {
            temp1=fabs(fbt-0)/(fabs(fbt- D1)+fabs(fbt-0))*( D1- AV_D1);
            correctValue=fbt-temp1;
        }
        if ((fbt>= D1)&&(fbt< D2))
        {
            temp1=fabs(fbt- D1)/(fabs(fbt- D2)+fabs(fbt- D1))*( D2- AV_D2);
            temp1=temp1+fabs(fbt- D2)/(fabs(fbt- D2)+fabs(fbt- D1))*( D1- AV_D1);
            correctValue=fbt-temp1;
        }
        if ((fbt>= D2)&&(fbt<= D3))
        {
            temp1=fabs(fbt- D3)/(fabs(fbt- D2)+fabs(fbt- D3))*(( D2- AV_D2));
            temp1=temp1+fabs(fbt- D2)/(fabs(fbt- D2)+fabs(fbt- D3))*(( D3- AV_D3));
            correctValue=fbt-temp1;
        }
        if ((fbt>= D3))
        {
            temp1=(( D3- AV_D3));
            correctValue=fbt-temp1;
        }
        return correctValue;
    }
    else
        return fbt;
}


void CheckShow::Step_in_or_out()
{//进出控制
    if(NextMoveDirection==PWM_IOCTL_SET_FREQ_IN)//last direction is forward('out')
    {
        NextMoveDirection=PWM_IOCTL_SET_FREQ_OUT;
        qDebug()<<"11";
        m3352.setFreq(255);
        m3352.Motor_in();
        m3352.urt_wf.DataIn(0);//将碰到光耦的数据取出来
    }
    else if(NextMoveDirection==PWM_IOCTL_SET_FREQ_OUT)//last direction is forward('out')
    {
        qDebug()<<"12";
        NextMoveDirection=PWM_IOCTL_SET_FREQ_IN;
        m3352.setFreq(255);
        m3352.Motor_out();
    }
}

void CheckShow::SetSoftWareVersion(QString Version,QString username)
{
    inputUserName = username;
    ui->Label_SoftWareVersioin->setText(Version);
    ui->Label_SoftWareVersioin->show();
    qDebug()<<Version ;
}

void CheckShow::initshowStrips()
{
    switch(Test.showstrips(ui->comboBox_Type->currentText(),ui->comboBox_Batch->currentText()))
    {
    case 2:
        TestStrips->SetNumber(2);
        TestStrips->SetPwm(120);
        break;
    case 6:
        TestStrips->SetNumber(6);
        TestStrips->SetPwm(245);
        break;

    default:
        TestStrips->SetNumber(2);
        TestStrips->SetPwm(Test.showstrips(ui->comboBox_Type->currentText(),ui->comboBox_Batch->currentText()));
        return;
    }
}

void CheckShow::onTestButtonOnClick()
{
    test *tmpTest = new test();
    tmpTest->setTestCount(mTestCount);
    tmpTest->setLedHight(m3352.led_light_hight);
    tmpTest->setPwmFreq(mTestFreq);
    connect(tmpTest,SIGNAL(okTest(int,int,int,int,int,int,int,int,int,int)),this,SLOT(onTestOkButtonOnClick(int,int,int,int,int,int,int,int,int,int)));
    tmpTest->show();
}
void CheckShow::onTestOkButtonOnClick(int tmpLedHight,int tmpLedLow,int tmpLedSymbol,int tmpPmwDirection,int tmpPmwEnable,int tmpPmwHz,int tmpSpiSymbol,int tmpSwdr,int tmpBenDi,int tmpTestCount)
{
    qDebug()<<tmpLedHight<<tmpLedLow<<tmpLedSymbol<<tmpPmwDirection<<tmpPmwDirection<<tmpPmwEnable<<tmpPmwHz<<tmpSpiSymbol<<tmpSwdr<<tmpBenDi;
    m3352.setLedLight(tmpLedHight,tmpLedLow);
    mTestCount = tmpTestCount;
    mTestFreq = tmpPmwHz;
    m3352.setFreq(tmpPmwHz);
}
void CheckShow::onChangeAppButtonClick()
{
    QFileInfo sdcard("/media/sda1");
    if(!sdcard.exists())
    {
        warning->setText(QString::fromUtf8("请插入SD卡"));
        warning->show();
        return ;
    }
    QString removeFilePath = "/opt/xyz";
    QFile::remove(removeFilePath);
    QFile::copy("/media/sda1/xyz","/opt/xyz");
     system("sync");
     warning->setText(QString::fromUtf8("文件拷贝完毕"));
     warning->show();
}

void CheckShow::onDeleteButtonOnClick()
{
    removefilesindir("/opt/points");
    warning->setText(QString::fromUtf8("文件删除完毕"));
    warning->show();
}
void CheckShow:: removefilesindir(const QString& path)
{
  QDir dir(path);
  QFileInfoList info_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::AllDirs);
  foreach(QFileInfo file_info, info_list)
  {
    if (file_info.isDir())
    {
      removefilesindir(file_info.absoluteFilePath());
    }
    else if (file_info.isFile())
    {
      QFile file(file_info.absoluteFilePath());
      qDebug() << "remove file  : " << file_info.absoluteFilePath();
      file.remove();
    }
  }

}
void CheckShow::onSaveButtonOnClick()
{
    QFileInfo sdcard("/media/sda1");
    if(!sdcard.exists())
    {
        warning->setText(QString::fromUtf8("请插入SD卡"));
        warning->show();
        return ;
    }
    QString fromDir = "/opt/points";
    QString toDir = "/media/sda1/points";
    if(copyDirectoryFiles(fromDir,toDir,true))
    {
        system("sync");
        int fromDirSize =  dirSize(fromDir);
        int toDirSize = 0;
        while(fromDirSize != toDirSize)
        {
            toDirSize = dirSize(toDir);
        }
        qDebug()<<"fromDirSize"<<fromDirSize;
        qDebug()<<"toDirSize"<<toDirSize;
        warning->setText(QString::fromUtf8("文件拷贝完毕"));
        warning->show();
    }else
    {
        warning->setText(QString::fromUtf8("错误"));
        warning->show();
    }
}
qint64 CheckShow::dirSize(QString tmpDir)
{
    QDir dir(tmpDir);
    qint64 size = 0;
    // dir.entryInfoList(QDir::Files)返回文件信息
    foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    //计算文件大小
    size += fileInfo.size();
    // dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
    foreach (QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    //若存在子目录，则递归调用du函数
    size += dirSize(tmpDir + QDir::separator() + subDir);
    return size;
}

bool CheckShow::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);

    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()),
                coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;
}
