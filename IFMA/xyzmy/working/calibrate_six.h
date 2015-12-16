#ifndef CALIBRATE_SIX_H
#define CALIBRATE_SIX_H

#include <QDialog>
#include "checkshow.h"
#include "mywarning.h"
#include "ui_calibrate_six.h"
namespace Ui {
    class Calibrate_Six;
}

class Calibrate_Six : public CheckShow
{
    Q_OBJECT

public:
    explicit Calibrate_Six(QWidget *parent = 0);
    ~Calibrate_Six();
    void initiatemove();
private:
    Ui::Calibrate_Six *ui;
    //CheckShow *ShowCalibrate;
    QStringList cassetteList;
    //void saveCalibrateResult;
    void initComboxname();
Mywarning *warning;

private slots:
    //float calibrate();
    void saveCalibrateResult();
    void showfinished(QString);
    void quitshow();
    void disabledOut();
    void weatherSavefile();
protected:
    void paintEvent(QPaintEvent *event);

signals:
    void calibrateFinished();
};

#endif // CALIBRATE_SIX_H
