#include <QtGui/QApplication>
#include "s_check.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    S_check w;
    w.show();

    return a.exec();
}
