#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QByteArray>
#include "config.h"
#include "connector.h"

#define START_WRITE "写卡"
#define STOP_WRITE "停止写卡"
#define START_READ "读卡"
#define STOP_READ "停止读卡"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_comboBox_startSector_currentIndexChanged(int index);

    void on_comboBox_num_currentIndexChanged(int index);

    void on_comboBox_port_currentIndexChanged(int index);

    void on_checkBox_cycle_stateChanged(int arg1);

    void on_checkBox_check_stateChanged(int arg1);

    void on_radioButton_sendText_clicked();

    void on_radioButton_sendHex_clicked();

    void on_radioButton_receiveText_clicked();

    void on_radioButton_receiveHex_clicked();

    void on_pushButton_sendClear_clicked();

    void on_pushButton_receiveClear_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_read_clicked();

    void on_pushButton_write_clicked();

    void writeOK();

    void readOK(QByteArray readData);


private:
    Ui::Widget *ui;
    Config config;
    Connector connector;

private:
    void initUi();

};

#endif // WIDGET_H
