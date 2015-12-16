#include "sql.h"

SQL::SQL(QObject *parent) :
    QObject(parent)
{
    dbconn = QSqlDatabase::addDatabase("QSQLITE");
    dbconn.setDatabaseName("WANTAI.db3");
}

/*
*函数作用:创建表格
*参数含义：判断是创建User还是Record表格
*返 回 值：创建成功返回true，否则返回false
*/
bool SQL::createTable(QString TName)
{
    QSqlQuery query;
    if(TName == "User")
        if(!query.exec("create table User(id integer primary key autoincrement ,name varchar,password varchar,flag varchar DEFAULT (3))"))
            return false;
    if(TName == "Record")
        if(!query.exec("create table Record(id integer primary key autoincrement,count varchar,name varchar,age varchar,sex varchar,type varchar,batch varchar,quanlity varchar,result varchar,date varchar,time varchar,operator varchar)"))
        {
            return false;
        }
    return true ;
}

/*
*函数作用:删除数据
*参数含义：目前之有用户会有删除功能
*返 回 值：删除成功返回true，否则返回false
*/
bool SQL::Delete(QString Name)
{
    QSqlQuery query ;
    QString sql = "delete from User where name='"+Name+"'";
    if(!query.exec(sql))
        return false;
    return true ;
}

/*
*函数作用:更新数据
*参数含义：目前暂时用不到此功能
*返 回 值：
*/
bool SQL::Update(int id)
{
    QSqlQuery query ;
    QString sql="update User set name='pangzi123' where id>=6";
    if(!query.exec(sql))
        return false;
    return true;
}

/*
*函数作用:Record查询数据
*参数含义：
*返 回 值：
*/
QList<QString> SQL::Select(QString type,QString name,QString count,QString date,int page)
{
    QSqlQuery query;
    QList<QString> result;//保存查询结果
    QString str ;
    QString sql="select * from Record where name like '%"+name+"%'"+
            " and type like '%"+type+"%'"+
            " and count like '%"+count+"%'"+
            " and date like '%"+date+"%' limit "+QString::number(page)
            +"*10,10";
    query.exec(sql);
    result.clear();

    while(query.next())
    {
        str="";
        for(int i=1;i<11;i++)
            str+=query.value(i).toString()+",";
        str+=query.value(11).toString();
        result<<str;//转换成a,b,c…，11的格式，方便分页显示
    }
    return result;
}

/*
*函数作用:User核实用户
*参数含义：type-->0:查询用户名和密码;1-->查询用户名;2-->返回用户名和密码
*返 回 值：0:没有查询到用户,1:超级用户,2:次级用户,3:普通用户
*/
QString SQL::Select(QString name,QString password,int type)
{
    QSqlQuery query;
    QString result="0";//保存查询结果
    QString sql;

    if(type==0)
        sql="select * from User where name='"+name+"' and password='"+password+"'";
    else if(type==1||type==2)
        sql="select * from User where name='"+name+"'";
    qDebug()<<sql ;
    query.exec(sql);
    while(query.next())
    {
        if(type==2)
            result=","+query.value(2).toString();
        else
            result=query.value(3).toString();
    }
    qDebug()<<result ;
    return result;
}


/*
*函数作用:返回所有用户
*参数含义：type=0:查询用户;type=1:查询项目号
*返 回 值：所有普通用户
*/
QStringList SQL::Select(int type)
{
    QSqlQuery query;
    QStringList result;
    int i ;
    if(type==0)
        query.exec("select name from User where flag='3'");
    if(type==1)
        query.exec("select distinct type from Record");

    result.clear();
    i=0;
    while(query.next())
    {
        result<<query.value(0).toString();
    }
    return result;
}
/*
*函数作用:关闭数据库
*参数含义：无
*返 回 值：关闭成功返回true，否则返回false
*/
bool SQL::DBClose()
{
    dbconn.close();
    return true;
}

/*
*函数作用:打开数据库
*参数含义：无
*返 回 值：成功返回true，否则返回false
*/
bool SQL::DBOpen()
{
    if(!dbconn.open())
    {
        qDebug("hahahah");
        return false;
    }
    qDebug()<<"HERE11";
    return true;

}

/*
*函数作用:Record数据库插入数据
*参数含义：11个参数
*返 回 值：插入成功返回true，否则返回false
*/
bool SQL::Insert(QString count, QString name, QString age, QString sex,
                 QString type, QString batch, QString quanlity, QString result,
                 QString date, QString time, QString Operator)
{
    QSqlQuery query;
    QString sql;
    sql="insert into Record values(null,'"+count+"','"+name+"','"+age+"','"
            +sex+"','"+type+"','"+batch+"','"+quanlity+"','"+result+"','"
            +date+"','"+time+"','"+Operator+"'"
            +")";
    if(!query.exec(sql))
        return false;
    return true;
}

/*
*函数作用:User数据库插入数据
*参数含义：3个参数-->姓名，密码，标志（1：管理员，2：次级管理员，3：普通用户）
*返 回 值：插入成功返回true，否则返回false
*/
bool SQL::Insert(QString name, QString password, QString flag)
{
    QSqlQuery query;
    QString sql;
    sql="insert into User values(null,'"+name+"','"+password+"','"+flag+"'"
            +")";
    if(!query.exec(sql))
        return false;
    return true;
}

/*
*函数作用: 返回有多少行
*参数含义：无
*返 回 值：行数
*/
int SQL::getRows(QString type,QString name,QString count,QString date)
{
    QSqlQuery query;
    QString sql="select count(*) from Record where name like '%"+name+"%'"+
            " and type like '%"+type+"%'"+
            " and count like '%"+count+"%'"+
            " and date like '%"+date+"%'";
    query.exec(sql);

    query.next();

    return query.value(0).toInt();
}

/*
*函数作用:在开发过程中进行测试，没有别的功能
*参数含义：无
*返 回 值：无
*/
void SQL::test()
{
    QSqlQuery query;
    query.exec(QString::fromUtf8("select * from User where name like'%%'"));
    qDebug()<<"-------------------------1";
    while(query.next())//query.next()指向查找到的第一条记录，然后每次后移一条记录
    {
        int ele0=query.value(0).toInt();//query.value(0)是id的值，将其转换为int型
        QString ele1=query.value(1).toString();
        QString ele2=query.value(2).toString();
        qDebug()<<ele0<<","<<ele1<<","<<ele2;//输出两个值
    }
    qDebug()<<"-------------------------2";
    query.exec(QString::fromUtf8("select * from User where password like'%long%'"));
    while(query.next())//query.next()指向查找到的第一条记录，然后每次后移一条记录
    {
        int ele0=query.value(0).toInt();//query.value(0)是id的值，将其转换为int型
        QString ele1=query.value(1).toString();
        QString ele2=query.value(2).toString();
        qDebug()<<ele0<<","<<ele1<<","<<ele2;//输出两个值
    }
    qDebug()<<"-------------------------3";
}



/*************************
 * name: wanghai
 * time: 20150421
 * fuction: select all data from Record Table
 * parameter: null
 * return: all data
***************************/
QList<QString> SQL::selectAllRecord()
{
    QList<QString> result;
    QSqlQuery query;
    QString sql = "select * from Record";
    QString str = "";
    result.clear();
    query.exec(sql);
    while (query.next()) {
        str.clear();
        for(int i=0; i<11; i++)
            str += query.value(i).toString() + ",";
        str += query.value(11).toString();
        result.append(str);
    }
    return result;
}

/**************************************************
 * 查询两个时间点之间所有的数据
 * ***************************************************/
QList<QString> SQL::selectTilNowRecord(QString fromDate, QString fromTime, QString toDate, QString toTime)
{
    QList<QString> result;
    QSqlQuery query;
    QString sql = "select * from Record where date>'"+fromDate
            +"' and date<'"+toDate+"' or (date='"
            +fromDate+"' and time>='"+fromTime+"') or (date='"
            +toDate+"' and time<='"+toTime+"')";
    QString str = "";
    result.clear();
    query.exec(sql);
    while(query.next()){
        str.clear();
        for(int i=0; i<11; i++)
            str += query.value(i).toString() + ",";
        str += query.value(11).toString();
        result.append(str);
    }
    return result;
}

/***********************
 * judge  table  existes or not
 * *********************/
bool SQL::IsTableExists(QString tableName)
{
    QSqlQuery query;
    bool exists = query.exec(
                QString("select count(*) from sqlite_master where type='table' and name='%1'")
                .arg(tableName));
    if(exists)
        return true;
    return false;
}

/**************************************
 * empty Record table data and delete table
 * ************************************/
bool SQL::emptyRecordTable()
{
    QSqlQuery query;
    QString sql = "drop table Record";
    return query.exec(sql);
}

