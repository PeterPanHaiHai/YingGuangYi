#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBuffer>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/select.h>
#include <QObject>
#include <termios.h>
//#include <mythread.h>
//#include <pthread.h>//Create a pthread
#include <QPainterPath>
#include <QDialog>
#include <QPixmap>
#include <QBitmap>
#include <QDateEdit>
//Add a seriport function
#include <qsocketnotifier.h>//用于监听串口的类
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>
#include <termio.h>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <qpainter.h>
//#include <QThread>
#include <QFile>
#include <fcntl.h>
#include <unistd.h>
#include <QTextStream>
#include <iostream>
#include <QColor>
#include <QBrush>
#include <QEvent>
#include <QDateTime>
#include <sys/stat.h>
//here is QScript head file
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QFileDialog>
#include <QDebug>
#include <unistd.h>
#include <sys/ioctl.h>
#include <QRegExp>
#include <QPointF>
#include <signal.h>
#include <sys/types.h>
//#include <pthread.h>
#include <QComboBox>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QComboBox *Project_box;
       QLineEdit *project_lineEdit;
           QLineEdit *LineEdit_Bach;
           void formulaCalcuate();
private slots:

    void add();
    void showProject();
    void loadFormula();
};

#endif // MAINWINDOW_H
