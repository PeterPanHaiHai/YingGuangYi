#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

enum Mode{Text, Hex};

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);

public:
    QString portName;   // 串口名
    qint8 startSector;    // 起始扇区
    qint8 sectorNum;     // 结束扇区
    bool isCycle;   // 是否循环写卡
    bool isCheck;   // 写卡完成后是否校验
    Mode sendMode;  // 发送模式
    Mode receiveMode;   // 读取模式

signals:

public slots:

};

#endif // CONFIG_H
