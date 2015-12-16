#include "onekeyimport.h"
//use this file to import all file in SD card，将SD卡中所有信息文件导入到 ARM 板的nand flash中
//here below is the content of the formula file
//2 (x+0.0181)/0.001 3 ug 2012-10-30 1 2 3 4 5 6
//其中第三个是小数点显示位数，地四个是单位
//再望其中添加一些变量，放在日期和校准参数之前，修改为如下
//2 (x+0.0181)/0.001 3 1 100 ug 2012-10-30 1 2 3 4 5 6
OneKeyImport::OneKeyImport()
{
}

bool OneKeyImport::WeatherHaveSD()
{
    if(-1==access("/sdcard",0))//sdcard 不存在
        return false;
    else
        return true;
}

QStringList OneKeyImport::FindFormulaFileInSD(char *path)
{
    struct dirent *ent=NULL;
    DIR *pDir;
    QStringList ProList;//保存项目的信息将起存在List上,最后保存在一个文件中
    count=0;
    if((pDir=opendir(path))==NULL)
    {
        return ProList;
    }

    while((ent=readdir(pDir))!=NULL)
    {
        if((0==strcmp(ent->d_name,"."))||(0==strcmp(ent->d_name,"..")))
            continue;

        QString D_Name=QString::fromLocal8Bit(ent->d_name);

        QString Suffix=D_Name.section(".",1,1);
        if(Suffix=="txt" && D_Name.contains("_"))//判断是否是txt且文件中是否包含“_”
        {
            QString ProjectName=D_Name.section(".",0,0);//获得项目名和批次批次号
            if(!ProList.contains(ProjectName))
            {
                ProList<<ProjectName;
            }
            FormulaPath[count]=QString::fromLocal8Bit(path);
            FormulaPath[count].append(ent->d_name);
            count++;
        }
        //strcat(FormulaPath[count],ent->d_name);
    }
    closedir(pDir);
    return ProList ;
}

bool OneKeyImport::ImportFormula(QString SourceFile)
{
    QString DestFile="/formula/";
    //QFile file(&SourceFile);

    QFileInfo info(SourceFile);
    bool ret;
    QFile copyFile;
    QString filename=info.fileName();
    DestFile=DestFile.append(filename);
    QFileInfo Destinfo(DestFile);
    //如果目标文件开始已经存在，那么则删除掉Flash中目标文件，从新复制，保证最新公式文件得到使用，
    //否则直接复制
    if(Destinfo.exists())//如果目标文件已存在,则删除，重新复制，不存在则直接复制
        unlink(DestFile.toUtf8().data());
    ret=copyFile.copy(SourceFile,DestFile);

    return ret;
}

bool OneKeyImport::WriteFiletoFlash()
{
    bool back;
    for(int i=0;i<count;i++)
    {
        back=ImportFormula(FormulaPath[i]);//write the count one in file
    }

    //调用shell来执行文件复制，即从SD卡将公式文件复制到Flash中，可以保证只有当SD卡中公式更新时，才替换，否则不替换
    //system("cp -r -u -v /sdcard/formula /");
    return back;
}

float OneKeyImport::DetermineFormula(QString Project, QString Batch,float AT)
{
    //利用项目号和批次号来确定要使用的检测公式,根据面积比等将AT（检测线）／AC（控制线），返回物质的浓度
    //如果公式从SD卡导入到Flash的过程中，失败了，falsh中没有这些公式文件，那么直接判断SD卡是否存在，若存在
    //若存在，其中有没有该公式，若有则直接从中读取，否则提错
    QString formula=showContent(Project,Batch,1);//取出公式

    QScriptEngine engine;
    QString fun="(function(x){return ";
    formula+="})";
    formula=fun+formula;
    QScriptValue res=engine.evaluate(formula);
    QScriptValueList dataValue;
    //as we should log10peak,so here should change

    dataValue<<AT;
    QScriptValue result=res.call(QScriptValue(),dataValue);
    //here change the formula
    float finalresult=result.toNumber();

    return finalresult;
}

QString OneKeyImport::showContent(QString Project, QString Batch,int i)
{
    QString formulaPath;
    //如果FLASH中无这些公式，那么去查看SD卡是否存在，如果SD卡存在，看其中是否有这些公式
    formulaPath="formula/";
    formulaPath.append(Project);
    formulaPath.append("_");
    formulaPath.append(Batch);
    formulaPath.append(".txt");

    QFile file(formulaPath);//使用对应参数文件
    if(file.open(QIODevice::ReadOnly)<0)
        QMessageBox::about(NULL,"onekeyimport","Read formula error!");
    QTextStream read(&file);
    QString content=read.readAll().section(' ',i,i);//read the formula
    file.close();
    if(!content.isEmpty())
        return content;
    else
        return 0;
}

int OneKeyImport::showstrips(QString Project, QString Batch)
{
    QString strips=showContent(Project,Batch,0);
    return strips.toInt();
}

QString OneKeyImport::Show_decimal_Unit(QString Project, QString Batch)
{
    QString data=NULL;//save dcimal and unit
    data=showContent(Project,Batch,2);//decimal
    data.append(" ");
    data.append(showContent(Project,Batch,5));//unit

    return data;
}

QString OneKeyImport::ShowLimit(QString Project, QString Batch)
{
    QString data=NULL;//save dcimal and unit
    data=showContent(Project,Batch,3);//最低范围
    data.append(" ");
    data.append(showContent(Project,Batch,4));//最高范围

    return data;
}

float *OneKeyImport::showCalibrateparameters(QString Project, QString Batch)
{
    float *Parameters=(float *)malloc(sizeof(float)*6);
    for(int i=3;i<9;i++)
    {
        QString content=showContent(Project,Batch,i);
        Parameters[i-3]=content.toFloat();
    }
    return Parameters;
}
