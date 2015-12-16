//countdown is the class that used to countdown for 10 seconds
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QDialog>
#include <QTimer>
#include <QFont>
#include <QPropertyAnimation>
namespace Ui {
class CountDown;
}

class CountDown : public QDialog
{
    Q_OBJECT

public:
    explicit CountDown(QWidget *parent = 0);
    ~CountDown();

private:
    Ui::CountDown *ui;
    int CountDownTime;
    int timerEvent_Id;

    void timerEvent(QTimerEvent *e);
private slots:
    void send_signals();
signals:
    void insert_ok();
    void timeOutOK();

};

#endif // COUNTDOWN_H
