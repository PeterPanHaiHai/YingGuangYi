#ifndef BACKUPANDDELETE_H
#define BACKUPANDDELETE_H

#include <sys/stat.h>
#include <sys/types.h>
#include <QDialog>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/vfs.h>
#include <stdio.h>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDateEdit>
#include <QDateTime>
#include <QProcess>
#include "common.h"
#include "warning_two.h"
#include "mywarning.h"
namespace Ui {
    class BackUpandDelete;
}

class BackUpandDelete : public QDialog
{
    Q_OBJECT

public:
    explicit BackUpandDelete(QWidget *parent = 0);
    ~BackUpandDelete();
    QString inputUserName;
    void SetSoftWareVersion(QString,QString);

private:
    Ui::BackUpandDelete *ui;
    QPushButton *Pushbutton_exit;
    warning_two *Backupwarning;
    warning_two *Deleteupwarning;
    Mywarning *warning;
    void timerEvent(QTimerEvent *);
    int timerEvent_Id;

private slots:
    bool BackUPandDelete();
    void DeleteData();
    void BackUpData();
    void LogOut();
    void unlinkdata();
    void ReallyBackup();
    void home();
};

#endif // BACKUPANDDELETE_H
