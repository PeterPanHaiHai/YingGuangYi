#ifndef QWARNING_H
#define QWARNING_H

#include <QDialog>
#include <QMessageBox>
namespace Ui {
    class QWarning;
}

class QWarning : public QMessageBox
{
    Q_OBJECT

public:
    explicit QWarning(QWidget *parent = 0);
    ~QWarning();

private:
    Ui::QWarning *ui;
};

#endif // QWARNING_H
