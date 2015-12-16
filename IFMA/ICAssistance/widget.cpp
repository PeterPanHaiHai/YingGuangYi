#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initUi();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initUi()
{
    // 获取所有可用的portName
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBox_port->addItem(info.portName());
    }
    ui->comboBox_port->setCurrentIndex(0);
    //更新设置
    config.portName = ui->comboBox_port->currentText();
    // 起始扇区从0~15，默认1
    for(int i = 0; i < 16; i++) {
        QString str = QString::number(i);
        ui->comboBox_startSector->addItem(str);
    }
    ui->comboBox_startSector->setCurrentIndex(1);
    // 扇区数从1-5，默认1
    for(int i = 1; i <= 5; i++) {
        QString str = QString::number(i);
        ui->comboBox_num->addItem(str);
    }
    //更新设置
    config.startSector = ui->comboBox_startSector->currentText().toInt();
    config.sectorNum = ui->comboBox_num->currentText().toInt();
    // 默认选项
    ui->checkBox_check->setChecked(false);
    ui->checkBox_cycle->setChecked(false);
    ui->radioButton_receiveText->setChecked(true);
    ui->radioButton_receiveHex->setChecked(false);
    ui->radioButton_sendHex->setChecked(false);
    ui->radioButton_sendText->setChecked(true);
    ui->label_status->setText(QStringLiteral("等待写卡"));
    //更新设置
    config.isCheck = false;
    config.isCycle = false;
    config.receiveMode = Text;
    config.sendMode = Text;
}

void Widget::on_comboBox_startSector_currentIndexChanged(int index)
{
    //更新设置
    config.startSector = ui->comboBox_startSector->currentText().toInt();
}

void Widget::on_comboBox_num_currentIndexChanged(int index)
{
    // 更新设置
    config.sectorNum = ui->comboBox_num->currentText().toInt();
}

void Widget::on_comboBox_port_currentIndexChanged(int index)
{
    //更新设置
    config.portName = ui->comboBox_port->currentText();
}

void Widget::on_checkBox_cycle_stateChanged(int arg1)
{
    //更新设置
    config.isCycle = ui->checkBox_cycle->isChecked();
}

void Widget::on_checkBox_check_stateChanged(int arg1)
{
    //更新设置
    config.isCheck = ui->checkBox_check->isChecked();
}

void Widget::on_radioButton_sendText_clicked()
{
    //更新设置
    if (ui->radioButton_sendText->isChecked()) {
        config.sendMode = Text;
    }
}

void Widget::on_radioButton_sendHex_clicked()
{
    //更新设置
    if (ui->radioButton_sendHex->isChecked()) {
        config.sendMode = Hex;
    }
}

void Widget::on_radioButton_receiveText_clicked()
{
    //更新设置
    if (ui->radioButton_receiveText->isChecked()) {
        config.receiveMode = Text;
    }
}

void Widget::on_radioButton_receiveHex_clicked()
{
    //更新设置
    if (ui->radioButton_receiveHex->isChecked()) {
        config.receiveMode = Hex;
    }
}

void Widget::on_pushButton_sendClear_clicked()
{
    //清空发送区
    ui->plainTextEdit_send->clear();
}

void Widget::on_pushButton_receiveClear_clicked()
{
    //清空接收区
    ui->plainTextEdit_receive->clear();
}

void Widget::on_pushButton_open_clicked()
{
    //打开串口
    connector.setPortName(config.portName);
    connector.open(QIODevice::ReadWrite);
    if (!connector.isOpen()) {
        QMessageBox::information(this, QStringLiteral("错误"), QStringLiteral("串口打开失败，请检查串口是否已被占用！"));
        return;
    }
    ui->pushButton_open->setEnabled(false);
    ui->pushButton_close->setEnabled(true);
    ui->pushButton_read->setEnabled(true);
    ui->pushButton_write->setEnabled(true);
    ui->comboBox_port->setEnabled(false);
    ui->label_status->setText(QString(QStringLiteral("连接到%1")).arg(config.portName));
}

void Widget::on_pushButton_close_clicked()
{
    if(ui->pushButton_read->text() == QStringLiteral(STOP_READ) ||
            ui->pushButton_write->text() == QStringLiteral(STOP_WRITE)) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请先停止读/写卡操作！"));
        return;
    }

    //关闭串口
    connector.close();
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_open->setEnabled(true);
    ui->pushButton_read->setEnabled(false);
    ui->pushButton_write->setEnabled(false);
    ui->comboBox_port->setEnabled(true);
    ui->label_status->setText(QStringLiteral("未连接"));
}

void Widget::on_pushButton_read_clicked()
{
    if (config.startSector + config.sectorNum >= 16) {
        QMessageBox::information(this, QStringLiteral("警告"), QStringLiteral("超出16个扇区！"));
        return;
    }
    if (ui->pushButton_read->text() == QStringLiteral(START_READ)) {
        ui->comboBox_startSector->setEnabled(false);
        ui->comboBox_num->setEnabled(false);
        ui->pushButton_read->setText(QStringLiteral(STOP_READ));
        ui->pushButton_write->setEnabled(false);
        connect(&connector, SIGNAL(readOK(QByteArray)), this, SLOT(readOK(QByteArray)));
        connector.startRead(config.startSector, config.sectorNum);
    } else if (ui->pushButton_read->text() == QStringLiteral(STOP_READ)) {
        ui->comboBox_startSector->setEnabled(true);
        ui->comboBox_num->setEnabled(true);
        ui->pushButton_read->setText(QStringLiteral(START_READ));
        ui->pushButton_write->setEnabled(true);
        disconnect(&connector, SIGNAL(readOK(QByteArray)), this, SLOT(readOK(QByteArray)));
    }
}

void Widget::on_pushButton_write_clicked()
{
    if (config.startSector + config.sectorNum >= 16) {
        QMessageBox::information(this, QStringLiteral("警告"), QStringLiteral("超出16个扇区！"));
        return;
    }
    if (ui->pushButton_write->text() == QStringLiteral(START_WRITE)) {
        ui->comboBox_startSector->setEnabled(false);
        ui->comboBox_num->setEnabled(false);
        ui->pushButton_write->setText(QStringLiteral(STOP_WRITE));
        ui->pushButton_read->setEnabled(false);
        QByteArray writeData = ui->plainTextEdit_send->toPlainText().toUtf8();
        if(writeData.size() > 48 * config.sectorNum ||
                writeData.size() > 120) {
            QMessageBox::information(this, QStringLiteral("警告"), QStringLiteral("数据长度过长！"));
            ui->comboBox_startSector->setEnabled(true);
            ui->comboBox_num->setEnabled(true);
            ui->pushButton_write->setText(QStringLiteral(START_WRITE));
            ui->pushButton_read->setEnabled(true);
            return;
        }
        connect(&connector, SIGNAL(writeOK()), this, SLOT(writeOK()));
        connector.startWrite(config.startSector, config.sectorNum, writeData);
    } else if (ui->pushButton_write->text() == QStringLiteral(STOP_WRITE)) {
        ui->comboBox_startSector->setEnabled(true);
        ui->comboBox_num->setEnabled(true);
        ui->pushButton_write->setText(QStringLiteral(START_WRITE));
        ui->pushButton_read->setEnabled(true);
        disconnect(&connector, SIGNAL(writeOK()), this, SLOT(writeOK()));
    }
}

void Widget::writeOK()
{
    disconnect(&connector, SIGNAL(writeOK()), this, SLOT(writeOK()));
    QMessageBox::information(this, QStringLiteral("写卡结束"), QStringLiteral("写卡结束！"));

    if (config.isCycle) {
        if (QMessageBox::question(this, QStringLiteral("提示"), QStringLiteral("按确定继续写卡"), QMessageBox::Ok, QMessageBox::Cancel) ==
                QMessageBox::Ok) {
            ui->pushButton_write->setText(QStringLiteral(START_WRITE));
            on_pushButton_write_clicked();
        } else {
            ui->pushButton_write->setText(QStringLiteral(STOP_WRITE));
            on_pushButton_write_clicked();
        }
    } else {
        ui->pushButton_write->setText(QStringLiteral(STOP_WRITE));
        on_pushButton_write_clicked();
    }
}

void Widget::readOK(QByteArray readData)
{
    disconnect(&connector, SIGNAL(readOK(QByteArray)), this, SLOT(readOK(QByteArray)));
    ui->plainTextEdit_receive->appendPlainText(QString(readData));
    QMessageBox::information(this, QStringLiteral("读卡结束"), QStringLiteral("读卡结束！"));
    ui->pushButton_read->setText(QStringLiteral(STOP_READ));
    on_pushButton_read_clicked();
}
