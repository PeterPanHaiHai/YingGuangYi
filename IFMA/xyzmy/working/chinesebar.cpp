#include "chinesebar.h"
#include "ui_chinesebar.h"

chinesebar::chinesebar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chinesebar)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    //setWindowFlags(Qt::FramelessWindowHint);
}

chinesebar::~chinesebar()
{
    delete ui;
}
