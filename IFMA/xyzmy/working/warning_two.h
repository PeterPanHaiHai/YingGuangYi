#ifndef WARNING_TWO_H
#define WARNING_TWO_H
#include "mywarning.h"
class warning_two:public Mywarning
{
    Q_OBJECT
public:
    warning_two(QWidget *parent = 0);
private:
    QPushButton *cancel_button;

};

#endif // WARNING_TWO_H
