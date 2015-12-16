#include "warning_two.h"
#include "ui_mywarning.h"
warning_two::warning_two(QWidget *parent):Mywarning(parent)
{
    cancel_button=new QPushButton(this);
    cancel_button->setStyleSheet(QString::fromUtf8("\n"
                                                   "QPushButton {\n"
                                                   "\n"
                                                   "background-image: url(:/ui/ui/Messagebox_cancel.png);\n"
                                                   "\n"
                                                   "border-radius:10px;\n"
                                                   "}\n"
                                                   "\n"
                                                   "\n"
                                                   "QPushButton:pressed\n"
                                                   "{\n"
                                                   "	background-image: url(:/ui/ui/Messagebox_cancel_pressed.png);\n"
                                                   "\n"
                                                   "\n"
                                                   "border-radius:10px;\n"
                                                   "}\n"
                                                   ""));
    ui->pushButton_yes->move(45,100);
    cancel_button->resize(ui->pushButton_yes->width(),ui->pushButton_yes->height());
    cancel_button->move(200,100);
    connect(ui->pushButton_yes,SIGNAL(clicked()),this,SLOT(send_yesSignal()));
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(close()));
}
