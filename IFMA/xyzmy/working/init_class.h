#ifndef INIT_CLASS_H
#define INIT_CLASS_H
/*
    主要初始化了如下界面：登录后的四个按钮分别对应的界面
    检测功能
    记录搜索
    系统设置
    记录备份及删除

    注：不能够初始化Dialog类，因为这个头文件要在Dialog中引用，所以不行
*/
#include "testshow.h"
#include "common.h"
#include "setshow.h"
#include "backupanddelete.h"

extern BackUpandDelete *backupDelete;
extern SetShow *setshow ;
extern TestShow *testshow ;

#endif // INIT_CLASS_H
