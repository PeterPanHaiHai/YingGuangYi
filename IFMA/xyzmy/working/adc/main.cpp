#include <QtGui/QApplication>
#include "adc.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ADC w;
    w.show();

    return a.exec();
}
