#include "mywarning.h"
#include "ui_mywarning.h"

Mywarning::Mywarning(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Mywarning)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->pushButton_yes->move((this->width()-ui->pushButton_yes->width())/2,100);
    ui->label_warning->setWordWrap(true);
    connect(ui->pushButton_yes,SIGNAL(clicked()),this,SLOT(close()));
    ui->pushButton_yes->setFocusPolicy(Qt::NoFocus);
}
Mywarning::~Mywarning()
{
    delete ui;
}

QString Mywarning::setText(QString text)
{
    font.setFamily("wenquanyi");
    font.setPointSize(160);
    font.setBold(true);
    ui->label_warning->setFont(font);
    ui->label_warning->setAlignment(Qt::AlignCenter);
    ui->label_warning->setText(text);
    return ui->label_warning->text();
}

void Mywarning::send_yesSignal()
{
    emit Pressed_Yes();
    this->close();
}

void Mywarning::setRight()
{//配合QSS显示正确的标志
    ui->label->setEnabled(false);
}
