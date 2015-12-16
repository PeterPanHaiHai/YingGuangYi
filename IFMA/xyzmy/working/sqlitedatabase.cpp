#include "sqlitedatabase.h"

sqlitedatabase::sqlitedatabase()
{
}
void sqlitedatabase::opensqlite3(char dbpath[100])
{


     //memset(dbpath,'\0',100);
    if(SQLITE_OK!=sqlite3_open(dbpath,&db))
        qDebug()<<"open database fails";

}

void sqlitedatabase::closesqlite3()
{
sqlite3_close(db);
}


void sqlitedatabase::execusql(char *sql)
{


    memset(sql,'\0',500);
    char *err_message;
    //sprintf(sql,"insert into user values('%s','%s');",lineEdit_CurrentUsername->text().toLatin1().data(),lineEdit_CurrenttPwd->text().toLatin1().data());
    if(SQLITE_OK!=sqlite3_exec(db,sql,NULL,NULL,&err_message))
        printf("Success\n");
    else
        printf("error\n");

}

char ** sqlitedatabase::selectsql(char *sql)
{


    char *err_message;

    qDebug()<<"sql is"<<sql;
  char **value;
  int nrow=0;
  int ncolum=0;
   sqlite3_get_table(db,sql,&value,&nrow,&ncolum,&err_message);
   int i=0;
   qDebug()<<"row"<<nrow<<"coloum"<<ncolum;

    sqlite3_free_table(value);
return value;

}


int *sqlitedatabase::rowandcolum(char *sql)
{

    int randc[2];


    char *err_message;

    qDebug()<<"sql is"<<sql;
  char **value;
  int nrow=0;
  int ncolum=0;
   sqlite3_get_table(db,sql,&value,&nrow,&ncolum,&err_message);
   int i=0;
   qDebug()<<"row"<<nrow<<"coloum"<<ncolum;

   randc[0]=nrow;
   randc[1]=ncolum;


   sqlite3_free_table(value);
   //return value;
}
