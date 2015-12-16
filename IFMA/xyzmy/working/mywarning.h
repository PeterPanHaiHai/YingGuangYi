#ifndef MYWARNING_H
#define MYWARNING_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
namespace Ui {
    class Mywarning;
}

class Mywarning : public QDialog
{
    Q_OBJECT

public:
    explicit Mywarning(QWidget *parent = 0);
    ~Mywarning();
    //Mywarning(int a=2);//create a warning box have 'yes' and 'no'
    QString setText(QString );
    void setRight();
private:
    QFont font ;
protected:
    Ui::Mywarning *ui;
    QPushButton *Button_No;
    //QPushButton *Button_yes;//this 'yes' just for two buttons ui_yes pressed quit,this 'yes' do some function
    QHBoxLayout *H_Layout;
signals:
    void Pressed_Yes();
    //void Pressed_No();
public slots:
    void send_yesSignal();
    //void send_NOSignal();
    //void initiate();

};

#endif // MYWARNING_H
