#include "config.h"

Config::Config(QObject *parent) :
    QObject(parent)
{
    portName = "COM1";
    startSector = 1;
    sectorNum = 1;
    isCycle = false;
    isCheck = false;
    sendMode = Text;
    receiveMode = Text;
}
