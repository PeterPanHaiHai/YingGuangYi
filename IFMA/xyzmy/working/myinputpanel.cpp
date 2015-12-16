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
//DATE:2013-08-05
//UPDATE:使输入法软键盘在鼠标离开当前页面的时候，避免软键盘在输入完成后一直存在
//CODE:QEvent::Leave  when you leave this panel


#include "myinputpanel.h"
//! [0]
int py_number = 0 ;
int py_num=0;
MyInputPanel::MyInputPanel()
    : QWidget(0, Qt::Tool|Qt::WindowStaysOnTopHint)//,
    // lastFocusedWidget(0)
{
    form.setupUi(this);
    QFont f;
    f.setFamily("wenquanyi");
    this->setFont(f);
    CAPS=false;//set as default
    currentPage=0;
    this->installEventFilter(this);
    this->setAttribute(Qt::WA_InputMethodEnabled);
    this->setFocus();
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(saveFocusWidget(QWidget*,QWidget*)));

    QFile file2(":/qss/ButtonPressed.qss");
    file2.open(QFile::ReadOnly);
    QTextStream filetext(&file2);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    form.horizontalLayout_7->setSpacing(0);
    form.horizontalLayout_7->setSpacing(0);
    form.horizontalLayout_Number->setSpacing(0);
    //here set the enter button
    form.panelButton_blank->setText(" ");

    signalMapper.setMapping(form.panelButton_1, form.panelButton_1);
    signalMapper.setMapping(form.panelButton_2, form.panelButton_2);
    signalMapper.setMapping(form.panelButton_3, form.panelButton_3);
    signalMapper.setMapping(form.panelButton_4, form.panelButton_4);
    signalMapper.setMapping(form.panelButton_5, form.panelButton_5);
    signalMapper.setMapping(form.panelButton_6, form.panelButton_6);
    signalMapper.setMapping(form.panelButton_7, form.panelButton_7);
    signalMapper.setMapping(form.panelButton_8, form.panelButton_8);
    signalMapper.setMapping(form.panelButton_9, form.panelButton_9);
    signalMapper.setMapping(form.panelButton_0, form.panelButton_0);
    signalMapper.setMapping(form.panelButton_Decimal, form.panelButton_Decimal);
    //Here Below is to set English words+++++++++++++++++++++++++++++++++++++
    signalMapper.setMapping(form.panelButton_q, form.panelButton_q);
    signalMapper.setMapping(form.panelButton_w, form.panelButton_w);
    signalMapper.setMapping(form.panelButton_e, form.panelButton_e);
    signalMapper.setMapping(form.panelButton_r, form.panelButton_r);
    signalMapper.setMapping(form.panelButton_t, form.panelButton_t);
    signalMapper.setMapping(form.panelButton_y, form.panelButton_y);
    signalMapper.setMapping(form.panelButton_u, form.panelButton_u);
    signalMapper.setMapping(form.panelButton_i, form.panelButton_i);
    signalMapper.setMapping(form.panelButton_o, form.panelButton_o);
    signalMapper.setMapping(form.panelButton_p, form.panelButton_p);
    signalMapper.setMapping(form.panelButton_a, form.panelButton_a);
    signalMapper.setMapping(form.panelButton_s, form.panelButton_s);
    signalMapper.setMapping(form.panelButton_d, form.panelButton_d);
    signalMapper.setMapping(form.panelButton_f, form.panelButton_f);
    signalMapper.setMapping(form.panelButton_g, form.panelButton_g);
    signalMapper.setMapping(form.panelButton_h, form.panelButton_h);
    signalMapper.setMapping(form.panelButton_j, form.panelButton_j);
    signalMapper.setMapping(form.panelButton_k, form.panelButton_k);
    signalMapper.setMapping(form.panelButton_l, form.panelButton_l);
    signalMapper.setMapping(form.panelButton_z, form.panelButton_z);
    signalMapper.setMapping(form.panelButton_x, form.panelButton_x);
    signalMapper.setMapping(form.panelButton_c, form.panelButton_c);
    signalMapper.setMapping(form.panelButton_v, form.panelButton_v);
    signalMapper.setMapping(form.panelButton_b, form.panelButton_b);
    signalMapper.setMapping(form.panelButton_n, form.panelButton_n);
    signalMapper.setMapping(form.panelButton_m, form.panelButton_m);
    signalMapper.setMapping(form.panelButton_blank, form.panelButton_blank);
    //here set the chinese bar
    signalMapper.setMapping(form.font_1, form.font_1);
    signalMapper.setMapping(form.font_2, form.font_2);
    signalMapper.setMapping(form.font_3, form.font_3);
    signalMapper.setMapping(form.font_4, form.font_4);
    signalMapper.setMapping(form.font_5, form.font_5);
    signalMapper.setMapping(form.font_6, form.font_6);
    signalMapper.setMapping(form.font_7, form.font_7);
    signalMapper.setMapping(form.font_8, form.font_8);

    //Here Below is to set English words+++++++++++++++++++++++++++++++++++++
    connect(form.panelButton_1, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.panelButton_2, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.panelButton_3, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.panelButton_4, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.panelButton_5, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.panelButton_6, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.panelButton_7, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.panelButton_8, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.panelButton_9, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    //connect(form.panelButton_star, SIGNAL(clicked()),
    //&signalMapper, SLOT(map()));
    connect(form.panelButton_0, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_Decimal, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    //connect(form.panelButton_hash, SIGNAL(clicked()),
    // &signalMapper, SLOT(map()));
    //Here Below is the English words+++++++++++++++++++++++++++++++++++++++++
    connect(form.panelButton_q, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_w, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_e, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_r, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_t, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_y, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_u, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_i, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_o, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_p, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_a, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_s, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_d, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_f, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_g, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_h, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_j, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_k, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_l, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_z, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_x, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_c, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_v, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_b, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_n, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_m, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(form.panelButton_blank, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    //connect(form.listUp,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    //connect(form.listDown,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    connect(form.font_1,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    connect(form.font_2,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    connect(form.font_3,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    connect(form.font_4,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    connect(form.font_5,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    connect(form.font_6,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    connect(form.font_7,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    connect(form.font_8,SIGNAL(clicked()),&signalMapper,SLOT(map()));
    // connect(form.panelButton_SHIFT, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    //Here Below is the English words+++++++++++++++++++++++++++++++++++++++++
    connect(form.enterButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(form.panelButton_backspace,SIGNAL(clicked()),this,SLOT(backSpace()));//For del
    connect(form.panelButton_CAPS, SIGNAL(clicked()),this, SLOT(CapsShow()));//for Caps
    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(buttonClicked(QWidget*)));

    connect(form.font_1,SIGNAL(clicked()),this,SLOT(chineseSelectFont_1()));
    connect(form.font_2,SIGNAL(clicked()),this,SLOT(chineseSelectFont_2()));
    connect(form.font_3,SIGNAL(clicked()),this,SLOT(chineseSelectFont_3()));
    connect(form.font_4,SIGNAL(clicked()),this,SLOT(chineseSelectFont_4()));
    connect(form.font_5,SIGNAL(clicked()),this,SLOT(chineseSelectFont_5()));
    connect(form.font_6,SIGNAL(clicked()),this,SLOT(chineseSelectFont_6()));
    connect(form.font_7,SIGNAL(clicked()),this,SLOT(chineseSelectFont_7()));
    connect(form.font_8,SIGNAL(clicked()),this,SLOT(chineseSelectFont_8()));
    connect(form.listUp,SIGNAL(clicked()),this,SLOT(pageUp()));
    connect(form.listDown,SIGNAL(clicked()),this,SLOT(pageDown()));

    bar=new chinesebar(this);
    bar->setGeometry(0,0,311,20);
    pageCount = 0;
    //py.txt is the dictionary file
    file.setFileName(":/src/py.txt");

    out.setFileName("serch.txt");

    if( !file.open(QIODevice::ReadOnly) )
        QMessageBox::warning(0,tr("Error"),tr("open pinyin file error"));
    regExp.setPatternSyntax(QRegExp::RegExp);
    regExp.setCaseSensitivity(Qt::CaseSensitive);
    chinese=false;
    connect(form.panelButton_Chinese,SIGNAL(clicked()),this,SLOT(chineseEbglishSwitch()));
    form.frame_Cn->hide();
}

//! [0]

bool MyInputPanel::event(QEvent *e)
{
    switch (e->type()) {
        //! [1]
    case QEvent::WindowActivate:
        if (lastFocusedWidget)
        {
            lastFocusedWidget->activateWindow();
        }
        break;
        //! [1]
    case QEvent::Leave:

        this->hide();
        break;
    default:
        break;
    }

    return QWidget::event(e);
}

//! [2]

void MyInputPanel::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus)) {
        lastFocusedWidget = newFocus;
    }
}

//! [2]

//! [3]
//press the button ,send the text
void MyInputPanel::buttonClicked(QWidget *w)
{
    //QChar chr = qvariant_cast<QChar>(w->property("buttonValue"));
    //at here judge chinese or not

    //print chinese two step
    QPushButton *ButtonValue=qobject_cast<QPushButton *>(w);

    QString chr=ButtonValue->text();
    // QChar *mine=chr.data();
    QChar *mine=chr.data();

    if(!chinese)
        emit characterGenerated(mine);
    else{//it is chinese
        InputChinese(*mine);
        QString pattern("(^[0-9A-Za-z]$)");
        QRegExp rx(pattern);
        bool match=rx.exactMatch(*mine);
        //if it is chinese,show Pingyin in lineEdit

        QRegExp wordregexp("^[a-z]+$");
        form.lineEdit_pingyin->setValidator(new QRegExpValidator(wordregexp,this));
        form.lineEdit_pingyin->insert(*mine);
        if(!match)
        {
            form.lineEdit_pingyin->setText("");
            for(int i=0;i<py_num;i++)
                this->backSpace();
            py_num=0;
        }else
            py_num++;
        emit characterGenerated(mine);
    }
}

//! [3]
void MyInputPanel::ShowInput()
{

}

//delete one wrld in lineedit
void MyInputPanel::backSpace()//here makes change,if form.lineEdit_pinyin is not empty,it will delete the pingyin,else,it will delete chinese in other lineedit
{
    QWidget *w=lastFocusedWidget;//the last focus widget
    QLineEdit *le=qobject_cast<QLineEdit *>(w);
    //here make change
    if(!form.lineEdit_pingyin->text().isEmpty())
    {
        form.lineEdit_pingyin->backspace();
        //reloadChinese(form.lineEdit_pingyin->text());//when pinying is changed e should show the chinese as the latest pinying
        //py="";
        if(!w->inherits("QDateEdit")&&!w->inherits("QTimeEdit"))
            le->backspace();//when you delete dateedit's button,it is error
        py_num--;
        reloadChinese(form.lineEdit_pingyin->text());
    }
    else

        if(!w->inherits("QDateEdit")&&!w->inherits("QTimeEdit"))
            le->backspace();//when you delete dateedit's button,it is error

}

void MyInputPanel::CapsShow()//show the Higher words
{
    if(!CAPS)
    {
        form.panelButton_a->setText("A");
        form.panelButton_b->setText("B");
        form.panelButton_c->setText("C");
        form.panelButton_d->setText("D");
        form.panelButton_e->setText("E");
        form.panelButton_f->setText("F");
        form.panelButton_g->setText("G");
        form.panelButton_h->setText("H");
        form.panelButton_i->setText("I");
        form.panelButton_j->setText("J");
        form.panelButton_k->setText("K");
        form.panelButton_l->setText("L");
        form.panelButton_m->setText("M");
        form.panelButton_n->setText("N");
        form.panelButton_o->setText("O");
        form.panelButton_p->setText("P");
        form.panelButton_q->setText("Q");
        form.panelButton_r->setText("R");
        form.panelButton_s->setText("S");
        form.panelButton_t->setText("T");
        form.panelButton_u->setText("U");
        form.panelButton_v->setText("V");
        form.panelButton_w->setText("W");
        form.panelButton_x->setText("X");
        form.panelButton_y->setText("Y");
        form.panelButton_z->setText("Z");
        CAPS=true;
    }
    else
    {
        form.panelButton_a->setText("a");
        form.panelButton_b->setText("b");
        form.panelButton_c->setText("c");
        form.panelButton_d->setText("d");
        form.panelButton_e->setText("e");
        form.panelButton_f->setText("f");
        form.panelButton_g->setText("g");
        form.panelButton_h->setText("h");
        form.panelButton_i->setText("i");
        form.panelButton_j->setText("j");
        form.panelButton_k->setText("k");
        form.panelButton_l->setText("l");
        form.panelButton_m->setText("m");
        form.panelButton_n->setText("n");
        form.panelButton_o->setText("o");
        form.panelButton_p->setText("p");
        form.panelButton_q->setText("q");
        form.panelButton_r->setText("r");
        form.panelButton_s->setText("s");
        form.panelButton_t->setText("t");
        form.panelButton_u->setText("u");
        form.panelButton_v->setText("v");
        form.panelButton_w->setText("w");
        form.panelButton_x->setText("x");
        form.panelButton_y->setText("y");
        form.panelButton_z->setText("z");
        CAPS=false;
    }
}
//input chinese
void MyInputPanel::InputChinese(QString pyInput)
{
    //pyInput is the word we pressed
    py.append(pyInput);

    out.open(QIODevice::ReadWrite|QIODevice::Truncate);

    int ret = 0;
    int count = 0;
    regExp.setPattern(QString("([^a-z\\s]{2,8})(%1)(\\s)").arg(py));

    while(!file.atEnd())
    {
        QByteArray data = file.readLine();
        ret = regExp.indexIn(QString(data.data()),0,QRegExp::CaretAtZero);

        if( ret > -1)
        {
            findNothing = false;
            out.write(regExp.cap(1).toAscii().data());
            out.write("\n");
            count++;
        }
    }
    file.seek(0);//定位到py.txt的开头
    out.close();

    if(count%8 == 0)
    {
        pageCount = count/8;
    }else{
        pageCount = (count/8)+1;
    }

    if(findNothing)
    {
        form.font_1->setText("");
        form.font_2->setText("");
        form.font_3->setText("");
        form.font_4->setText("");
        form.font_5->setText("");
        form.font_6->setText("");
        form.font_7->setText("");
        form.font_8->setText("");
    }

    findNothing = true;
    showPage(0);
}

//show the next page
void MyInputPanel::showPage(int index)
{
    int n =0;
    QString chinese[8];

    out.open(QIODevice::ReadWrite);
    while(!out.atEnd())
    {
        for(int j=0;j<8;j++)
        {
            QByteArray data = out.readLine();

            data.replace("\n","\0");
            n++;
            if((index*8)< n && n <((index+1)*8)+1)
            {
                chinese[j].append(data);
            }
        }
    }
    form.font_1->setText(tr(chinese[0].toLatin1().data()));
    form.font_2->setText(tr(chinese[1].toAscii()));
    form.font_3->setText(tr(chinese[2].toAscii()));
    form.font_4->setText(tr(chinese[3].toAscii()));
    form.font_5->setText(tr(chinese[4].toAscii()));
    form.font_6->setText(tr(chinese[5].toAscii()));
    form.font_7->setText(tr(chinese[6].toAscii()));
    form.font_8->setText(tr(chinese[7].toAscii()));
    out.close();
}

//switch english and chinese
void MyInputPanel::chineseEbglishSwitch()
{
    //print chinese first step
    if(chinese)//if it is chinese,switch to english
    {
        py_num = 0 ;
        py.clear();
        form.panelButton_CAPS->setEnabled(true);
        chinese=false;
        form.panelButton_Chinese->setText("EN");
        form.frame_Cn->hide();
        form.frame_Cn->setVisible(false);

        form.frame_Number->show();
    }else{
        chinese=true;
        CAPS=true ;
        CapsShow();
        form.panelButton_CAPS->setEnabled(false);
        form.panelButton_Chinese->setText("CN");
        QRect NewGermotry=form.frame_Number->geometry();
        form.frame_Cn->setGeometry(NewGermotry);
        form.frame_Cn->show();
        form.frame_Number->hide();//hide the number frame
        form.frame_Number->setVisible(false);
    }
}


void MyInputPanel::chineseSelectFont_1()
{
    //keyRev->insert(bar->m_ui->font_1->text());
    py.clear();
    //form.lineEdit->clear();
    form.font_1->setText("");
    form.font_2->setText("");
    form.font_3->setText("");
    form.font_4->setText("");
    form.font_5->setText("");
    form.font_6->setText("");
    form.font_7->setText("");
    form.font_8->setText("");
}

void MyInputPanel::chineseSelectFont_2()
{
    //keyRev->insert(bar->m_ui->font_2->text());
    py.clear();
    //form.lineEdit->clear();
    //form.lineEdit->clear();
    form.font_1->setText("");
    form.font_2->setText("");
    form.font_3->setText("");
    form.font_4->setText("");
    form.font_5->setText("");
    form.font_6->setText("");
    form.font_7->setText("");
    form.font_8->setText("");
}

void MyInputPanel::chineseSelectFont_3()
{
    // keyRev->insert(bar->m_ui->font_3->text());
    py.clear();
    //form.lineEdit->clear();
    // form.lineEdit->clear();
    form.font_1->setText("");
    form.font_2->setText("");
    form.font_3->setText("");
    form.font_4->setText("");
    form.font_5->setText("");
    form.font_6->setText("");
    form.font_7->setText("");
    form.font_8->setText("");
}

void MyInputPanel::chineseSelectFont_4()
{
    // keyRev->insert(bar->m_ui->font_4->text());
    py.clear();
    //form.lineEdit->clear();
    //form.lineEdit->clear();
    form.font_1->setText("");
    form.font_2->setText("");
    form.font_3->setText("");
    form.font_4->setText("");
    form.font_5->setText("");
    form.font_6->setText("");
    form.font_7->setText("");
    form.font_8->setText("");
}

void MyInputPanel::chineseSelectFont_5()
{
    //keyRev->insert(form.font_5->text());
    py.clear();
    // form.lineEdit->clear();
    //form.lineEdit->clear();
    form.font_1->setText("");
    form.font_2->setText("");
    form.font_3->setText("");
    form.font_4->setText("");
    form.font_5->setText("");
    form.font_6->setText("");
    form.font_7->setText("");
    form.font_8->setText("");
}

void MyInputPanel::chineseSelectFont_6()
{
    //keyRev->insert(form.font_6->text());
    py.clear();
    //form.lineEdit->clear();
    //form.lineEdit->clear();
    form.font_1->setText("");
    form.font_2->setText("");
    form.font_3->setText("");
    form.font_4->setText("");
    form.font_5->setText("");
    form.font_6->setText("");
    form.font_7->setText("");
    form.font_8->setText("");
}

void MyInputPanel::chineseSelectFont_7()
{
    //keyRev->insert(form.font_7->text());
    py.clear();
    //form.lineEdit->clear();
    // form.lineEdit->clear();
    form.font_1->setText("");
    form.font_2->setText("");
    form.font_3->setText("");
    form.font_4->setText("");
    form.font_5->setText("");
    form.font_6->setText("");
    form.font_7->setText("");
    form.font_8->setText("");
}

void MyInputPanel::chineseSelectFont_8()
{
    //keyRev->insert(form.font_8->text());
    py.clear();
    // form.lineEdit->clear();
    //form.lineEdit->clear();
    form.font_1->setText("");
    form.font_2->setText("");
    form.font_3->setText("");
    form.font_4->setText("");
    form.font_5->setText("");
    form.font_6->setText("");
    form.font_7->setText("");
    form.font_8->setText("");
}
//switch to last page
void MyInputPanel::pageUp()
{
    currentPage--;
    if(currentPage > -1)
    {
        form.listDown->setEnabled(true);
        showPage(currentPage);
    }else
        currentPage++;
}

//switch to next page
void MyInputPanel::pageDown()
{
    currentPage++;
    if(currentPage < pageCount)
    {
        form.listUp->setEnabled(true);
        showPage(currentPage);
    }else{
        currentPage--;
    }
}

void MyInputPanel::reloadChinese(QString Pinying)
{
    //pyInput is the word we pressed
    py=Pinying;

    out.open(QIODevice::ReadWrite|QIODevice::Truncate);

    int ret = 0;
    int count = 0;
    regExp.setPattern(QString("([^a-z\\s]{2,8})(%1)(\\s)").arg(py));
    while(!file.atEnd())
    {
        QByteArray data = file.readLine();
        ret = regExp.indexIn(QString(data.data()),0,QRegExp::CaretAtZero);
        if( ret > -1)
        {
            findNothing = false;
            out.write(regExp.cap(1).toAscii().data());
            out.write("\n");
            count++;
        }
    }
    file.seek(0);
    out.close();

    if(count%8 == 0)
    {
        pageCount = count/8;
    }else{
        pageCount = (count/8)+1;
    }

    if(findNothing)
    {
        form.font_1->setText("");
        form.font_2->setText("");
        form.font_3->setText("");
        form.font_4->setText("");
        form.font_5->setText("");
        form.font_6->setText("");
        form.font_7->setText("");
        form.font_8->setText("");
    }

    findNothing = true;
    if(!form.lineEdit_pingyin->text().isEmpty())
        showPage(0);
    else
    {
        form.font_1->setText("");
        form.font_2->setText("");
        form.font_3->setText("");
        form.font_4->setText("");
        form.font_5->setText("");
        form.font_6->setText("");
        form.font_7->setText("");
        form.font_8->setText("");
    }
}


