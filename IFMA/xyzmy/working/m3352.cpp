#include "m3352.h"

M3352::M3352()
{
    tempchar=new char[14];
    urt_wf.Init_Dev();
    move_flag=0x10;
    dir_flag=0x10;
    spi_flag=0x10;
    led_flag=0x10;
    led_light_hight = 0xd7;
    led_light_low = 0x0f;
    speed=255;
}

void M3352::LED_close()
{
    led_flag=0x10;
    tempchar[0]=0xff;//start
    tempchar[1]=0xff;//start
    tempchar[2]=spi_flag;//spi
    tempchar[3]=led_flag;//LED
    tempchar[4]=move_flag;//enable
    tempchar[5]=dir_flag;//dir
    tempchar[6]=speed; //fre
    tempchar[7]=127;   //null
    tempchar[8]=0;   //null
    tempchar[9]=led_light_low;   //null
    tempchar[10]=led_light_hight;   //null
    tempchar[11]=0xef;//end
    tempchar[12]=0xef;//end
    urt_wf.Send_Dev(tempchar);
}

void M3352::LED_open()
{
    led_flag=0x11;
    tempchar[0]=0xff;//start
    tempchar[1]=0xff;//start
    tempchar[2]=spi_flag;//spi
    tempchar[3]=led_flag;//LED
    tempchar[4]=move_flag;//enable
    tempchar[5]=dir_flag;//dir
    tempchar[6]=speed; //fre
    tempchar[7]=127;   //null
    tempchar[8]=0;   //null
    tempchar[9]=led_light_low;   //null
    tempchar[10]=led_light_hight;   //null
    tempchar[11]=0xef;//end
    tempchar[12]=0xef;//end
    urt_wf.Send_Dev(tempchar);
}

void M3352::Motor_close()
{
    move_flag=0x10;
    tempchar[0]=0xff;//start
    tempchar[1]=0xff;//start
    tempchar[2]=spi_flag;//spi
    tempchar[3]=led_flag;//LED
    tempchar[4]=move_flag;//enable
    tempchar[5]=dir_flag;//dir
    tempchar[6]=speed; //fre
    tempchar[7]=127;   //null
    tempchar[8]=0;   //null
    tempchar[9]=led_light_low;   //null
    tempchar[10]=led_light_hight;   //null
    tempchar[11]=0xef;//end
    tempchar[12]=0xef;//end
    urt_wf.Send_Dev(tempchar);
}

void M3352::Motor_open()
{//暂时未用到
    move_flag=0x11;
    tempchar[0]=0xff;//start
    tempchar[1]=0xff;//start
    tempchar[2]=spi_flag;//spi
    tempchar[3]=led_flag;//LED
    tempchar[4]=move_flag;//enable
    tempchar[5]=dir_flag;//dir
    tempchar[6]=speed; //fre
    tempchar[7]=127;   //null
    tempchar[8]=0;   //null
    tempchar[9]=led_light_low;   //null
    tempchar[10]=led_light_hight;   //null
    tempchar[11]=0xef;//end
    tempchar[12]=0xef;//end
    urt_wf.Send_Dev(tempchar);
}

void M3352::Motor_in()
{
    dir_flag=0x10;
    move_flag=0x11;
    tempchar[0]=0xff;//start
    tempchar[1]=0xff;//start
    tempchar[2]=spi_flag;//spi
    tempchar[3]=led_flag;//LED
    tempchar[4]=move_flag;//enable
    tempchar[5]=dir_flag;//dir
    tempchar[6]=speed; //fre
    tempchar[7]=127;   //null
    tempchar[8]=0;   //null
    tempchar[9]=led_light_low;   //null
    tempchar[10]=led_light_hight;   //null
    tempchar[11]=0xef;//end
    tempchar[12]=0xef;//end
    urt_wf.Send_Dev(tempchar);
}

void M3352::Motor_out()
{
    dir_flag=0x11;
    move_flag=0x11;
    tempchar[0]=0xff;//start
    tempchar[1]=0xff;//start
    tempchar[2]=spi_flag;//spi
    tempchar[3]=led_flag;//LED
    tempchar[4]=move_flag;//enable
    tempchar[5]=dir_flag;//dir
    tempchar[6]=speed; //fre
    tempchar[7]=127;   //null
    tempchar[8]=0;   //null
    tempchar[9]=led_light_low;   //null
    tempchar[10]=led_light_hight;   //null
    tempchar[11]=0xef;//end
    tempchar[12]=0xef;//end
    urt_wf.Send_Dev(tempchar);
}

void M3352::SPI_close()
{
    spi_flag=0x10;
    move_flag=0x10;
    tempchar[0]=0xff;//start
    tempchar[1]=0xff;//start
    tempchar[2]=spi_flag;//spi
    tempchar[3]=led_flag;//LED
    tempchar[4]=move_flag;//enable
    tempchar[5]=dir_flag;//dir
    tempchar[6]=speed; //fre
    tempchar[7]=127;   //null
    tempchar[8]=0;   //null
    tempchar[9]=led_light_low;   //null
    tempchar[10]=led_light_hight;   //null
    tempchar[11]=0xef;//end
    tempchar[12]=0xef;//end
    urt_wf.Send_Dev(tempchar);
}

void M3352::SPI_open()
{
    spi_flag=0x11;
    move_flag=0x11;
    tempchar[0]=0xff;//start
    tempchar[1]=0xff;//start
    tempchar[2]=spi_flag;//spi
    tempchar[3]=led_flag;//LED
    tempchar[4]=move_flag;//enable
    tempchar[5]=dir_flag;//dir
    tempchar[6]=speed; //fre
    tempchar[7]=127;   //null
    tempchar[8]=0;   //null
    tempchar[9]=led_light_low;   //null
    tempchar[10]=led_light_hight;   //null
    tempchar[11]=0xef;//end
    tempchar[12]=0xef;//end
    urt_wf.Send_Dev(tempchar);
}

void M3352::setFreq(int freq)
{
    this->speed = freq;
}
void M3352::setLedLight(int tmpHight, int tmpLow)
{
    this->led_light_hight = tmpHight;
    this->led_light_low = tmpLow;
}
