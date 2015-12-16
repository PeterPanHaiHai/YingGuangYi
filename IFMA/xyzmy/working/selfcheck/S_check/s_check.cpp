#include "s_check.h"
#include "ui_s_check.h"

S_check::S_check(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::S_check)
{
    ui->setupUi(this);
}

S_check::~S_check()
{
    delete ui;
}
