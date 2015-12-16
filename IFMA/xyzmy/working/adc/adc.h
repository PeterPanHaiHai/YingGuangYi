#ifndef ADC_H
#define ADC_H

#include <QMainWindow>

namespace Ui {
    class ADC;
}

class ADC : public QMainWindow
{
    Q_OBJECT

public:
    explicit ADC(QWidget *parent = 0);
    ~ADC();

private:
    Ui::ADC *ui;
private slots:
    void Test();
};

#endif // ADC_H
