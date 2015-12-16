#ifndef M3352_H
#define M3352_H
#include "urtclass.h"

class M3352
{
public:
    M3352();
    char * tempchar;
    UrtClass urt_wf;
    int move_flag;
    int dir_flag;
    int led_flag;
    int led_light_hight;
    int led_light_low;
    int spi_flag;
    int speed;

public:
    void LED_open();
    void LED_close();//控制LED灯
    void Motor_out();
    void Motor_in();
    void Motor_open();
    void Motor_close();//控制电机
    void SPI_open();
    void SPI_close();//控制SPI
    void setFreq(int freq);
    void setLedLight(int,int);
};

#endif // M3352_H
