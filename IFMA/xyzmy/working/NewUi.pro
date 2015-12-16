# -------------------------------------------------
# Project created by QtCreator 2012-04-09T20:33:51
# -------------------------------------------------
QT += core \
    gui
QT +=sql
QT +=script

CONFIG   += serialport   #to use serialport class

TARGET = xyz
TEMPLATE = app
#QMAKE_CXXFLAGS+=-fopenmp

LIBS += -L /lib -lhello_arm\
         /lib/libiconv_ARM.so

#LIBS += -L /lib -lhello_x86\
#         /lib/libiconv_x86.so

SOURCES += main.cpp \
    dialog.cpp \
    testshow.cpp \
    checkshow.cpp \
    setshow.cpp \
    myinputpanelcontext.cpp \
    myinputpanel.cpp \
    checkoption.cpp \
    login.cpp \
    chinesebar.cpp \
    countdown.cpp \
    selfcheck.cpp \
       calibrate_six.cpp \
    backupanddelete.cpp \
    mywarning.cpp \
    warning_two.cpp \
    strips.cpp \
    gradient.cpp \
    genotype.cpp \
    onekeyimport.cpp \
    positioncalibrate.cpp \
    m3352.cpp \
    urtclass.cpp \
    sql.cpp \
    sqlite.cpp \
    commserial.cpp \
    connector.cpp \
    test.cpp \
    FLOUR_v1.0/findpeakcore.cpp \
    FLOUR_v1.0/findpeakcore_initialize.cpp \
    FLOUR_v1.0/findpeakcore_terminate.cpp \
    FLOUR_v1.0/fpeak.cpp \
    FLOUR_v1.0/rtGetInf.cpp \
    FLOUR_v1.0/rtGetNaN.cpp \
    FLOUR_v1.0/rt_nonfinite.cpp \
    FLOUR_v1.0/qtest.cpp

HEADERS += dialog.h \
    testshow.h \
    checkshow.h \
    setshow.h \
    myinputpanelcontext.h \
    myinputpanel.h \
    checkoption.h \
    login.h \
    chinesebar.h \
    countdown.h \
    common.h \
    selfcheck.h \
       calibrate_six.h \
    initiate.h \
    backupanddelete.h \
    peak/sortrows.h \
    peak/rtwtypes.h \
    peak/rt_nonfinite.h \
    peak/rtGetNaN.h \
    peak/rtGetInf.h \
    peak/fpeak.h \
    peak/findpeakcore_types.h \
    peak/findpeakcore_terminate.h \
    peak/findpeakcore_initialize.h \
    peak/findpeakcore_emxutil.h \
    peak/findpeakcore_emxAPI.h \
    peak/findpeakcore.h \
    peak/diff.h \
    mywarning.h \
    warning_two.h \
    strips.h \
    gradient.h \
    genotype.h \
    onekeyimport.h \
    positioncalibrate.h \
    init_class.h \
    m3352.h \
    urtclass.h \
    sql.h \
    sqlite.h \
    yiqisuanfa.h \
    commserial.h \
    connector.h \
    test.h \
    FLOUR_v1.0/findpeakcore.h \
    FLOUR_v1.0/findpeakcore_initialize.h \
    FLOUR_v1.0/findpeakcore_terminate.h \
    FLOUR_v1.0/findpeakcore_types.h \
    FLOUR_v1.0/fpeak.h \
    FLOUR_v1.0/rtGetInf.h \
    FLOUR_v1.0/rtGetNaN.h \
    FLOUR_v1.0/rt_nonfinite.h \
    FLOUR_v1.0/rtwtypes.h \
    FLOUR_v1.0/qtest.h
FORMS += dialog.ui \
    testshow.ui \
    checkshow.ui \
    setshow.ui \
    myinputpanelform.ui \
    checkoption.ui \
    login.ui \
    chinesebar.ui \
    countdown.ui \
    InputFormula.ui \
    ShowResult.ui \
    calibrate_six.ui \
    backupanddelete.ui \
    temp.ui \
    xyz.ui \
    selfcheck.ui \
    mywarning.ui \
    test.ui
RESOURCES += \
    resource.qrc

OTHER_FILES += \
    qss/xyzQss.qss \
    qss/ButtonPressed.qss \
    warning.txt
