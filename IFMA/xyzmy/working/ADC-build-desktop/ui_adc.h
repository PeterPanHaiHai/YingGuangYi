/********************************************************************************
** Form generated from reading UI file 'adc.ui'
**
** Created: Sat Mar 16 13:17:45 2013
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADC_H
#define UI_ADC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ADC
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLineEdit *lineEdit_Point;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ADC)
    {
        if (ADC->objectName().isEmpty())
            ADC->setObjectName(QString::fromUtf8("ADC"));
        ADC->resize(400, 300);
        centralWidget = new QWidget(ADC);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 40, 98, 27));
        lineEdit_Point = new QLineEdit(centralWidget);
        lineEdit_Point->setObjectName(QString::fromUtf8("lineEdit_Point"));
        lineEdit_Point->setGeometry(QRect(150, 40, 141, 27));
        ADC->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ADC);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
        ADC->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ADC);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ADC->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ADC);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ADC->setStatusBar(statusBar);

        retranslateUi(ADC);

        QMetaObject::connectSlotsByName(ADC);
    } // setupUi

    void retranslateUi(QMainWindow *ADC)
    {
        ADC->setWindowTitle(QApplication::translate("ADC", "ADC", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ADC", "Begin", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ADC: public Ui_ADC {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADC_H
