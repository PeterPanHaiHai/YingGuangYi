#ifndef TEST_H
#define TEST_H

#include <QDialog>

namespace Ui {
class test;
}

class test : public QDialog
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = 0);
    ~test();
public :
    int getSpiSymbol();
    int getLedSymbol();
    int getPmwEnable();
    int getPmwDirection();
    int getPmwHz();
    int getSwdr();
    int getLedHight();
    int getLedLow();
    int getbindi();
    int getTestCount();

    void setTestCount(int );
    void setLedHight(int );
    void setPwmFreq(int );


signals:

    void okTest(int ,int ,int ,int ,int ,int ,int ,int ,int,int);
public slots:

    void onOkButtonOnClick();

private:
    Ui::test *ui;
};

#endif // TEST_H
