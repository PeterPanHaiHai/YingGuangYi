//login is the class used to judge weather the user legitimate
#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFile>
#include <sys/stat.h>
#include <sys/types.h>
#include "dialog.h"
#include "sqlite.h"
#include "common.h"
#include "mywarning.h"
#include "login.h"//used to connect to sqlite
#include <QSqlQuery>
#include <QTableView>
#include <QSqlQueryModel>
//#include "softwareversioin.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    QString inputUserName;

    void SetVersion(QString);
private:
    Ui::Login *ui;
    Dialog *dialog;
    QString SoftwareVersion;

    void DefaultUser();
    void UpdateDefaultUser();
private slots:
    void loginin();//login in
    void judgeText();
};

#endif // LOGIN_H
