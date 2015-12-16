#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFileInfo>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QFrame *Frame_Project = new QFrame(this);
    Frame_Project->setObjectName(QString::fromUtf8("Frame_Project"));
    Frame_Project->setGeometry(QRect(90, 80, 601, 361));
    Frame_Project->setFrameShape(QFrame::StyledPanel);
    Frame_Project->setFrameShadow(QFrame::Raised);
    Frame_Project->show();
    //add a add button
    QHBoxLayout *HLayout=new QHBoxLayout();
    QHBoxLayout *SLayout=new QHBoxLayout();
    QVBoxLayout *VLayout=new QVBoxLayout();
    QPushButton *pushButton_add=new QPushButton(Frame_Project);
    pushButton_add->setText("ADD Project");
    // pushButton_add->show();
    QPushButton *pushButton_LoadFormula=new QPushButton(Frame_Project);
    pushButton_LoadFormula->setText("Load Formula");
    Project_box=new QComboBox(Frame_Project);
    showProject();//read Project file and show it
  LineEdit_Bach=new QLineEdit(Frame_Project);


   project_lineEdit=new QLineEdit(Frame_Project);
    project_lineEdit->setFixedWidth(40);

    HLayout->addWidget(project_lineEdit);
    HLayout->addWidget(pushButton_add);
    SLayout->addWidget(Project_box);
    SLayout->addWidget(LineEdit_Bach);
    SLayout->addWidget(pushButton_LoadFormula);
    VLayout->addLayout(HLayout);
    VLayout->addLayout(SLayout);
    //HLayout->addWidget(pushButton_LoadFormula);
    Frame_Project->setLayout(VLayout);
    //addrecordCommobox();
    //connect(pushButton_Exit,SIGNAL(clicked()),this,SLOT(quitSetProject()));
  connect(pushButton_add,SIGNAL(clicked()),this,SLOT(add()));
    //connect(pushButton_LoadFormula,SIGNAL(clicked()),this,SLOT(loadFormula()));
    //connect(pushButton_Save,SIGNAL(clicked()),this,SLOT(ChangeProjectRecord()));
    //connect(pushButton_Delete,SIGNAL(clicked()),this,SLOT(deleteProjectRecord()));
    //connect(comboBox_project,SIGNAL(currentIndexChanged(QString)),lineEdit_CurrnetProject,SLOT(setText(QString)));
   connect(pushButton_LoadFormula,SIGNAL(clicked()),this,SLOT(loadFormula()));//load formula
   formulaCalcuate();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::add()
{
    QFile file("Project.txt");
    QString Content;
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        Content=stream.readAll();
        qDebug()<<"content"<<Content;

    }
    if(Content.contains(project_lineEdit->text()))//it did not exits
    { QMessageBox::warning(this,tr("warning"),tr("it already exist"));
        //break;
        qDebug()<<project_lineEdit->text();
        file.close();
    }
    else
    {
        QFile file("Project.txt");
        file.open(QIODevice::WriteOnly|QIODevice::Append);

        QTextStream writein(&file);

        writein<<project_lineEdit->text()<<endl;
        qDebug()<<project_lineEdit->text();
        QMessageBox::warning(this,tr("warning"),tr("writing success"));
        file.close();
    }

}

void MainWindow::showProject()
{


    QFile ProjectFile("Project.txt");
    if(ProjectFile.open(QIODevice::ReadOnly)<0)
        QMessageBox::warning(this,tr("error"),tr("No Project file"));
    QTextStream stream(&ProjectFile);
    QString line;
    while(!stream.atEnd())
    {
        line=stream.readLine();//read the txt file line by line
       Project_box->addItem(line);
}}

void MainWindow::loadFormula()
{

    //save the formula from SD
    //then get the formula from SD,when we first Use a kind of casset
    //consisder the formula is y=ax+b;
    //so initial we should check if there ars stays SD card
    //set the regexp as y=(*)x+(*);
//Foramt of the formula is 'CRP_BACHID.txt'
    QString formula;
    QString FilePath;
    float coefficient[3];
    //set the formula file as formula.txt in SD
    FilePath="/sdcard/"+Project_box->currentText()+"_"+LineEdit_Bach->text()+".txt";
    qDebug()<<"path is"<<FilePath;

        QFile file(FilePath);//in arm its name is disk
          QFileInfo info(FilePath);
          if(info.exists())
          {
        if(file.open(QIODevice::ReadOnly)<0)
            QMessageBox::warning(this,tr("error"),tr("open file error"));
        QTextStream read(&file);
        formula=read.readAll();
        qDebug()<<formula;
        //QString SaveFormulapath;
        QString SaveFormulapath="formula/"+info.fileName();
        QFile fileSave(SaveFormulapath);
        if(fileSave.open(QIODevice::WriteOnly)<0)
        {
            printf("open error!\n");
            QMessageBox::warning(this,tr("Error"),tr("load formula is error,repeat"));
        }


        QTextStream write(&fileSave);
        write<<formula<<endl;
        qDebug()<<"formula is"<<formula;
        qDebug()<<coefficient[0]<<" "<<coefficient[1];
        QMessageBox::warning(this,tr("Informationi"),tr("Loading formula finished"));
}
          else
             QMessageBox::warning(this,tr("error"),tr("No Such file"));



}


void MainWindow::formulaCalcuate()

{
    QString formula;
    //we save lots of formula in flash,and we select the right formula file by Bach_ID
    QString fileName;
    QString bach_ID_show="CRP";
    qDebug()<<"Bach id"<<bach_ID_show;
    QString Project_Calcuate="111";
    qDebug()<<"project is"<<Project_Calcuate;
    fileName="formula/"+Project_Calcuate+"_"+bach_ID_show+".txt";//name format:Bach_idformula.txt
    qDebug()<<"fileName"<<fileName;
    QFile file(fileName);//in arm its name is disk
    if(file.open(QIODevice::ReadOnly)<0)
        qDebug()<<"Read error!"<<endl;
    QTextStream read(&file);
    formula=read.readAll();//read the formula
    //Use QScrit to finish the formula
    //let it be a stable



    //formula="1.998*x-3.1167";
    QScriptEngine engine;
    QString fun="(function(x){return ";
    formula+="})";
    formula=fun+formula;
    qDebug()<<"formula"<<formula;
    QScriptValue res=engine.evaluate(formula);
    QScriptValueList dataValue;
    //float peak1=600;
    //as we should log10peak,so here should change
    int peak=1000;
    dataValue<<peak;

    QScriptValue result=res.call(QScriptValue(),dataValue);
    //here change the formula
    qDebug()<<result.toString();
    float finalresult=result.toNumber();
    finalresult=pow(10,finalresult)/10;
    qDebug()<<finalresult;
    //return finalresult;
    //Here Use QScript to calcuate formula


    /* QString formula;
    float coefficient[2];
    float TempConsistency=0;
    float Consistency=0;
    QFile file("coefficient.txt");//in arm its name is disk
    if(file.open(QIODevice::ReadOnly)<0)
        qDebug()<<"Read error!"<<endl;
    QTextStream read(&file);
    formula=read.readAll();

    coefficient[0]=formula.section(" ",0,0).toFloat();

    coefficient[1]=formula.section(" ",1,1).toFloat();
    qDebug()<<"in write file"<<endl;
    qDebug()<<coefficient[0]<<" "<<coefficient[1]<<endl;

    peak=log10(peak);//log10 the highest point
    TempConsistency=coefficient[0]*peak+coefficient[1];
    Consistency=pow(10,TempConsistency)/10;
    qDebug()<<Consistency;
    return Consistency;*/

}
