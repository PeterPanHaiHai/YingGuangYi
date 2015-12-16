/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QComboBox *comboBox_port;
    QLabel *label_port;
    QLabel *label_start;
    QComboBox *comboBox_startSector;
    QLabel *label_num;
    QComboBox *comboBox_num;
    QCheckBox *checkBox_cycle;
    QCheckBox *checkBox_check;
    QPushButton *pushButton_write;
    QGroupBox *groupBox_send;
    QPlainTextEdit *plainTextEdit_send;
    QRadioButton *radioButton_sendText;
    QRadioButton *radioButton_sendHex;
    QPushButton *pushButton_sendClear;
    QGroupBox *groupBox_receive;
    QPlainTextEdit *plainTextEdit_receive;
    QPushButton *pushButton_receiveClear;
    QRadioButton *radioButton_receiveText;
    QRadioButton *radioButton_receiveHex;
    QLabel *label_status;
    QPushButton *pushButton_read;
    QPushButton *pushButton_open;
    QPushButton *pushButton_close;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(569, 338);
        comboBox_port = new QComboBox(Widget);
        comboBox_port->setObjectName(QStringLiteral("comboBox_port"));
        comboBox_port->setGeometry(QRect(100, 60, 71, 31));
        label_port = new QLabel(Widget);
        label_port->setObjectName(QStringLiteral("label_port"));
        label_port->setGeometry(QRect(50, 59, 41, 31));
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setPointSize(12);
        label_port->setFont(font);
        label_port->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_start = new QLabel(Widget);
        label_start->setObjectName(QStringLiteral("label_start"));
        label_start->setGeometry(QRect(20, 19, 71, 31));
        label_start->setFont(font);
        label_start->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        comboBox_startSector = new QComboBox(Widget);
        comboBox_startSector->setObjectName(QStringLiteral("comboBox_startSector"));
        comboBox_startSector->setGeometry(QRect(100, 20, 71, 31));
        label_num = new QLabel(Widget);
        label_num->setObjectName(QStringLiteral("label_num"));
        label_num->setGeometry(QRect(170, 19, 71, 31));
        label_num->setFont(font);
        label_num->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        comboBox_num = new QComboBox(Widget);
        comboBox_num->setObjectName(QStringLiteral("comboBox_num"));
        comboBox_num->setGeometry(QRect(250, 20, 71, 31));
        checkBox_cycle = new QCheckBox(Widget);
        checkBox_cycle->setObjectName(QStringLiteral("checkBox_cycle"));
        checkBox_cycle->setEnabled(true);
        checkBox_cycle->setGeometry(QRect(340, 30, 87, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\226\260\345\256\213\344\275\223"));
        font1.setPointSize(12);
        checkBox_cycle->setFont(font1);
        checkBox_check = new QCheckBox(Widget);
        checkBox_check->setObjectName(QStringLiteral("checkBox_check"));
        checkBox_check->setEnabled(false);
        checkBox_check->setGeometry(QRect(340, 70, 103, 20));
        checkBox_check->setFont(font1);
        pushButton_write = new QPushButton(Widget);
        pushButton_write->setObjectName(QStringLiteral("pushButton_write"));
        pushButton_write->setEnabled(false);
        pushButton_write->setGeometry(QRect(450, 20, 81, 31));
        pushButton_write->setFont(font1);
        groupBox_send = new QGroupBox(Widget);
        groupBox_send->setObjectName(QStringLiteral("groupBox_send"));
        groupBox_send->setGeometry(QRect(30, 100, 251, 201));
        plainTextEdit_send = new QPlainTextEdit(groupBox_send);
        plainTextEdit_send->setObjectName(QStringLiteral("plainTextEdit_send"));
        plainTextEdit_send->setGeometry(QRect(10, 20, 231, 151));
        radioButton_sendText = new QRadioButton(groupBox_send);
        radioButton_sendText->setObjectName(QStringLiteral("radioButton_sendText"));
        radioButton_sendText->setGeometry(QRect(10, 180, 71, 16));
        radioButton_sendText->setChecked(true);
        radioButton_sendHex = new QRadioButton(groupBox_send);
        radioButton_sendHex->setObjectName(QStringLiteral("radioButton_sendHex"));
        radioButton_sendHex->setEnabled(false);
        radioButton_sendHex->setGeometry(QRect(90, 180, 71, 16));
        pushButton_sendClear = new QPushButton(groupBox_send);
        pushButton_sendClear->setObjectName(QStringLiteral("pushButton_sendClear"));
        pushButton_sendClear->setGeometry(QRect(160, 176, 71, 20));
        groupBox_receive = new QGroupBox(Widget);
        groupBox_receive->setObjectName(QStringLiteral("groupBox_receive"));
        groupBox_receive->setGeometry(QRect(290, 100, 251, 201));
        plainTextEdit_receive = new QPlainTextEdit(groupBox_receive);
        plainTextEdit_receive->setObjectName(QStringLiteral("plainTextEdit_receive"));
        plainTextEdit_receive->setGeometry(QRect(10, 20, 231, 151));
        pushButton_receiveClear = new QPushButton(groupBox_receive);
        pushButton_receiveClear->setObjectName(QStringLiteral("pushButton_receiveClear"));
        pushButton_receiveClear->setGeometry(QRect(160, 176, 71, 20));
        radioButton_receiveText = new QRadioButton(groupBox_receive);
        radioButton_receiveText->setObjectName(QStringLiteral("radioButton_receiveText"));
        radioButton_receiveText->setGeometry(QRect(10, 180, 71, 16));
        radioButton_receiveText->setChecked(true);
        radioButton_receiveHex = new QRadioButton(groupBox_receive);
        radioButton_receiveHex->setObjectName(QStringLiteral("radioButton_receiveHex"));
        radioButton_receiveHex->setEnabled(false);
        radioButton_receiveHex->setGeometry(QRect(90, 180, 71, 16));
        label_status = new QLabel(Widget);
        label_status->setObjectName(QStringLiteral("label_status"));
        label_status->setEnabled(true);
        label_status->setGeometry(QRect(30, 300, 511, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font2.setPointSize(12);
        label_status->setFont(font2);
        pushButton_read = new QPushButton(Widget);
        pushButton_read->setObjectName(QStringLiteral("pushButton_read"));
        pushButton_read->setEnabled(false);
        pushButton_read->setGeometry(QRect(450, 60, 81, 31));
        pushButton_read->setFont(font1);
        pushButton_open = new QPushButton(Widget);
        pushButton_open->setObjectName(QStringLiteral("pushButton_open"));
        pushButton_open->setGeometry(QRect(180, 60, 61, 31));
        pushButton_open->setFont(font1);
        pushButton_close = new QPushButton(Widget);
        pushButton_close->setObjectName(QStringLiteral("pushButton_close"));
        pushButton_close->setEnabled(false);
        pushButton_close->setGeometry(QRect(260, 60, 61, 31));
        pushButton_close->setFont(font1);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "IC\345\215\241\345\206\231\345\215\241\347\250\213\345\272\217", 0));
        label_port->setText(QApplication::translate("Widget", "\344\270\262\345\217\243", 0));
        label_start->setText(QApplication::translate("Widget", "\350\265\267\345\247\213\346\211\207\345\214\272", 0));
        label_num->setText(QApplication::translate("Widget", "\346\211\207\345\214\272\346\225\260", 0));
        checkBox_cycle->setText(QApplication::translate("Widget", "\345\276\252\347\216\257\345\206\231\345\215\241", 0));
        checkBox_check->setText(QApplication::translate("Widget", "\345\206\231\345\215\241\345\220\216\346\240\241\351\252\214", 0));
        pushButton_write->setText(QApplication::translate("Widget", "\345\206\231\345\215\241", 0));
        groupBox_send->setTitle(QApplication::translate("Widget", "\345\217\221\351\200\201\347\274\223\345\206\262\345\214\272", 0));
        radioButton_sendText->setText(QApplication::translate("Widget", "\346\226\207\346\234\254\346\250\241\345\274\217", 0));
        radioButton_sendHex->setText(QApplication::translate("Widget", "Hex\346\250\241\345\274\217", 0));
        pushButton_sendClear->setText(QApplication::translate("Widget", "\346\270\205\347\251\272\347\274\223\345\206\262\345\214\272", 0));
        groupBox_receive->setTitle(QApplication::translate("Widget", "\346\216\245\346\224\266\347\274\223\345\206\262\345\214\272", 0));
        pushButton_receiveClear->setText(QApplication::translate("Widget", "\346\270\205\347\251\272\347\274\223\345\206\262\345\214\272", 0));
        radioButton_receiveText->setText(QApplication::translate("Widget", "\346\226\207\346\234\254\346\250\241\345\274\217", 0));
        radioButton_receiveHex->setText(QApplication::translate("Widget", "Hex\346\250\241\345\274\217", 0));
        label_status->setText(QApplication::translate("Widget", "\346\234\252\350\277\236\346\216\245", 0));
        pushButton_read->setText(QApplication::translate("Widget", "\350\257\273\345\215\241", 0));
        pushButton_open->setText(QApplication::translate("Widget", "\346\211\223\345\274\200", 0));
        pushButton_close->setText(QApplication::translate("Widget", "\345\205\263\351\227\255", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
