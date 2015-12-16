#ifndef CHINESEBAR_H
#define CHINESEBAR_H

#include <QDialog>

namespace Ui {
    class chinesebar;
}

class chinesebar : public QDialog
{
    Q_OBJECT

public:
    explicit chinesebar(QWidget *parent = 0);
    ~chinesebar();
    Ui::chinesebar *ui;


};

#endif // CHINESEBAR_H
