/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
//MyInputPanel is the class of the soft keyboard
#ifndef MYINPUTPANEL_H
#define MYINPUTPANEL_H

#include <QtGui>
#include <QtCore>
#include <QPoint>
#include <QRect>
#include <QRegExp>
#include "ui_myinputpanelform.h"
#include "chinesebar.h"
#include "ui_chinesebar.h"
//! [0]

class MyInputPanel : public QWidget
{
    Q_OBJECT

public:
    MyInputPanel();
    QWidget *lastFocusedWidget;
    bool chinese;
    chinesebar *bar;
signals:
    void characterGenerated(QChar *character);

protected:
    bool event(QEvent *e);

private slots:
    void saveFocusWidget(QWidget *oldFocus, QWidget *newFocs);
    void buttonClicked(QWidget *w);
    void ShowInput();
    void backSpace();
    void CapsShow();
    void InputChinese(QString );
    void showPage(int );
    void chineseEbglishSwitch();
    void chineseSelectFont_1();
    void chineseSelectFont_2();
    void chineseSelectFont_3();
    void chineseSelectFont_4();
    void chineseSelectFont_5();
    void chineseSelectFont_6();
    void chineseSelectFont_7();
    void chineseSelectFont_8();
    void pageUp();
    void pageDown();
    void reloadChinese(QString );
    //void focusOutEvent(QFocusEvent *);
   // bool eventFilter(QObject *, QEvent *);
private:
    Ui::MyInputPanelForm form;
    QSignalMapper signalMapper;
    bool CAPS;//judge if it was CAPS,default:false
    int currentPage;
    QFile out;
    QRegExp regExp;
    QString py;
    QFile file;
    bool findNothing;
    int pageCount;
    QRect TrueGermotry;
    QRect HideGermotry;

};

//! [0]

#endif // MYINPUTPANEL_H
