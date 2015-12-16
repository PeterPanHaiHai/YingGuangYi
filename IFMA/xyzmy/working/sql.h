#ifndef SQL_H
#define SQL_H

#include "QtSql"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QStringList"
#include <QObject>

class SQL : public QObject
{
    Q_OBJECT
public:
    explicit SQL(QObject *parent = 0);

    QSqlDatabase dbconn ;

    bool createTable(QString TName);
    bool Delete(QString Name);
    bool Update(int id);
    bool Insert(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    bool Insert(QString name,QString password,QString flag);
    bool DBClose();
    bool DBOpen();
    QList<QString> Select(QString type, QString name, QString count, QString date,int page);
    QString Select(QString name, QString password, int type);
    QStringList Select(int type);
    int getRows(QString type, QString name, QString count, QString date);

    void test();

    //wanghai
    QList<QString> selectAllRecord();
    QList<QString> selectTilNowRecord(QString, QString, QString, QString);
    bool IsTableExists(QString);
    bool emptyRecordTable();
    
signals:
    
public slots:
    
};

#endif // SQL_H
