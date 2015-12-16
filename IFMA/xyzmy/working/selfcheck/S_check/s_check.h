#ifndef S_CHECK_H
#define S_CHECK_H

#include <QDialog>

namespace Ui {
    class S_check;
}

class S_check : public QDialog
{
    Q_OBJECT

public:
    explicit S_check(QWidget *parent = 0);
    ~S_check();

private:
    Ui::S_check *ui;
};

#endif // S_CHECK_H
