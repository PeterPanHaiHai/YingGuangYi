/*************************************************************************************************
* 文件描述：SPI与AD通信，PWM控制电机，IO口控制led灯,串口与ARM通信
* 程序文件：main.c
* 版    本：V0.1
* 日    期：2014.08.22
* 作    者：杨志伟、王海
* 芯    片：STC12C5A60S2
* 时    钟：11.0592MHz
* 开发环境：Keil uVision V4.00a
* 备    注：
*			单片机P34、VCC、P06、P1.3引脚连接M415-64步进驱动器的ENA、OPTO、DIR、PUL引脚
*			ENA:使能信号，禁止或允许驱动器工作，低电平禁止
*			OPTO：驱动电源（+5V）
*			DIR：方向信号，用于改变电机转向，TTL电平驱动
*			PUL：脉冲信号，上升沿有效，每当脉冲由低变高时电机走一步
*
*			单片机P15、P16、P17引脚连接ARM板SPIMOSI1、SPIMISO1、SPICLK1
*
*			单片机P10引脚控制LED
* 修改引脚定义：
* 			time:2014.09.04
*			单片机P1.5、VCC、P1.4、P1.3引脚连接M415-64步进驱动器的ENA、OPTO、DIR、PUL引脚
*			单片机P4.1、P4.2、P4.3引脚连接ARM板SPIMOSI1、SPIMISO1、SPICLK1
*			单片机P1.6引脚控制LED
***************************************************************************************************/


#include "stc12c5a60s2.h"	//包含STC12C5A60S2寄存器定义文件
#include <I2C.H>

#define uchar unsigned char
#define SlaveAddress 0X60

sbit led = P1^6;	//LED控制位
sbit pwm_ena = P1^5;   //PWM时能信号
sbit pwm_dir = P1^4;	//PWM方向信号
sbit spi_cs = P4^0;
sbit nOC1=P3^3;      //外部中断信号
sbit nOC2 = P3^4;      //返回内部光耦信息 
sbit nOC3 = P3^5;      //定义中断

unsigned char D1=0;
unsigned char D2=0;
unsigned char rec_mes_num;	  //串口读取数组变量
unsigned char rec_mes[13];	  //串口读取数组
unsigned char uart_rec_status; //帧数据接收完成标志
unsigned char flag_send_ADC = 0;	//是否要读AD转换后的数据
unsigned char spi_buff[4];	 //SPI数据帧
unsigned char uart_buff;
unsigned char flag=0;	 //SPI是否开始接收数据标志
unsigned long int i;
unsigned char totalnum=0;	//组装SPI数据帧控制位
unsigned int num=0;			//一次SPI返回多少个数据
unsigned char pwmFreL = 230;	//设置PWM频率值	230=133.34 240=211.779hz  250=512Hz 251=600 252=720 253=900.062 254=1.2kHz 255=3.6kHz
unsigned char frelight = 127;
unsigned int flag_noc2=0;
 int adcdata=0;
 int adc[1];
void SPI_write(unsigned char);
void SendUart(unsigned char);

/*
*控制LED灯亮度，共12位
*/
void write_mcp4725(unsigned char high, unsigned char low)
{
		unsigned char temp=0;
		Start_I2c();	//启动总线
		temp=(SlaveAddress << 1) | 0;	//发送器件地址
		SendByte(temp);
		if(ack==0) return;
		SendByte(high);//发送控制字节高位
		if(ack==0) return;
		SendByte(low);//发送控制字节低位
		if(ack==0) return;
		Stop_I2c();	//结束总线
}


/*********************************************************************************************
函数名：SPI初始化程序
调用：SPI_init()
参数：无
返回值：无
备注：无
*********************************************************************************************/
void SPI_init()
{
	AUXR1|=0x20;					//SPI从P1口切换到P4口
    SPCTL=0xd0;                    //详细注释与汇编语言相同
	SPSTAT=0xc0;                   //清0标志位SPIF和WCOL
	IE2=IE2|0x02;                  //ESPI(IE2.1)=1,允许SPIF产生中断

	EA=1;                      //开总中断	
}


/*******************************************************************************************
函数  名：串口1初始化函数
调    用：uart1_init()
入口参数：无
返 回 值：无	
备    注：STC12C5A60S2单片机串口1
		  晶振频率：11.0592M；波特率9600		  
*******************************************************************************************/
void uart1_init(void)
{	
	//初始化串口        //9600bps@11.0592MHz
    PCON &= 0x7f;            //波特率不倍速
    SCON = 0x50;              //8位数据,可变波特率
    BRT = 0xDC;            //设定独立波特率发生器重装值
    AUXR |= 0x04;             //独立波特率发生器时钟为Fosc,即1T
    AUXR |= 0x01;             //串口1选择独立波特率发生器为波特率发生器
    AUXR |= 0x10;             //启动独立波特率发生器

	EA = 1;	//开总中断	
	ES = 1;	//开串口中断
	TR1 = 1;	//启动串口1	
}

/********************************************************************************************
函数名：定时器0初始化函数
调用：timer0_init();
参数：无
返回值：无
备注：无
**********************************************************************************************/
void timer0_init(void)
{
//	TMOD = 0x01;	//使用位操作，避免对其他定时器产生影响
//	TL0 = (65536-pwmFrequency)%256;	//设置初值
//	TH0 = (65536-pwmFrequency)/256;
//	TL0=64000;
//	TH0=64000>>8;
	TMOD |=0x03;	//使用模式2，8位自动重装模式，使用位操作，避免对其他定时器产生影响
 	TL0 = pwmFreL;
	EA = 1;		//开总中断
	ET0 = 1; 	//开定时器0中断
	TR0 = 1;	//启动定时器0
	TR1 = 0;
}


/*********************************************************************************************
函数名：PWM初始化函数
调  用：PWM_init();
参  数：无
返回值：无
结  果：将PCA初始化为PWM模式，初始占空比为0
备  注：需要更多路PWM输出直接插入CCAPnH和CCAPnL即可
/**********************************************************************************************/
void PWM_init (void)
{
	CMOD=0x04; //设置PCA定时器,通过CPS2、CPS1、CPS0位可调频率
    CL=0x00;   
    CH=0x00;
 	CCAPM0=0x42; //PWM0设置PCA工作方式为PWM方式（0100 0010）
    CCAP0L=0x00; //设置PWM0初始值与CCAP0H相同
  	CCAP0H=0x00; // PWM0初始时为0

//CCAPM1=0x42; //PWM1设置PCA工作方式为PWM方式（使用时删除//）
//CCAP1L=0x00; //设置PWM1初始值与CCAP0H相同
//CCAP1H=0x00; // PWM1初始时为0

	CR=1; //启动PCA定时器
}

/*********************************************************************************************
函数名：INT1初始化程序
调用：INT1_init()
参数：无
返回值：无
备注：无
*********************************************************************************************/
void INT1_init()
{
	IT1 = 1;                        //set INT1 int type (1:Falling only 0:Low level)
    EX1 = 1;                        //enable INT1 interrupt
    EA = 1;                         //open global interrupt switch
}

void send()
{
	SendUart(0xff);
	SendUart(0x0f);
	SendUart(0x0f);
	SendUart(0xff);	
}
/*********************************************************************************************
函数名：PWM0占空比设置函数
调  用：PWM0_set();
参  数：0x00~0xFF（亦可用0~255）
返回值：无
结  果：设置PWM模式占空比，为0时全部高电平，为1时全部低电平
备  注：如果需要PWM1的设置函数，只要把CCAP0L和CCAP0H中的0改为1即可
/**********************************************************************************************/
void PWM0_set (unsigned char a)
{
	CCAP0L = a; //设置值直接写入CCAP0L
	CCAP0H = a; //设置值直接写入CCAP0H
}


/*********************************************************************************************
函数名：主函数
调  用：无
参  数：无
返回值：无
结  果：程序开始处，无限循环
备  注：
/**********************************************************************************************/
void main (void)
{
	
	led = 0;	//默认LED灯灭	
	timer0_init();	//定时器0初始化


	PWM_init(); //PWM初始化
	pwm_ena = 0;
	pwm_dir = 0;   //出仓
	PWM0_set(255); //设置PWM占空比,总共255
	
	uart1_init();	//串口1初始化	

    SPI_init();	//SPI初始化

	INT1_init();
	uart_buff=0;

	for(i=0;i<4;i++)
		spi_buff[i]=0;

	spi_cs=0;
	SPI_write(0x23);		 //AD初始化
	SPI_write(0x1b);
	SPI_write(0x13);
	SPI_write(0x4e);

	uart_rec_status  = 0;

	while(1)
	{
		if(uart_rec_status)	   //帧接收完成处理
		{
			
			uart_rec_status = 0;	//清零，接收下一帧
			if(rec_mes[0]==0xFF && rec_mes[1]==0xFF && rec_mes[11]==0xEF && rec_mes[12]==0xEF)
			{
				/*帧处理*/
				if(rec_mes[2] == 0x11)
				{	
					//读AD转换后的数据
					flag_send_ADC = 1;					     
					rec_mes[2] = 0x10;//及时关闭SPI，不要在下次while再发送数据	
				}
				else
					flag_send_ADC = 0;

				if(rec_mes[3] == 0x11)
				{
					//LED灯亮 
					led = 1;
					//控制LED灯亮度
					write_mcp4725(rec_mes[9],rec_mes[10]);					
				}
				else
				{
					led = 0;
				}
				
				if(rec_mes[4] == 0x11)	 //电机使能
				{
															
					if(rec_mes[5] == 0x11)
					{
						//出仓
						pwm_dir = 0;					    
					}
					else if(rec_mes[5] == 0x10)
					{
						//入仓
						pwm_dir = 1;
					}
					
					pwmFreL = rec_mes[6];
					frelight= rec_mes[7];
					PWM0_set(frelight);
					pwm_ena = 1;
				}
				else
				{
					pwmFreL = rec_mes[6];
					frelight= rec_mes[7];
					PWM0_set(frelight);
					pwm_ena = 0;
				}
			}

		}//end if

		if(flag_send_ADC == 1)
		{
			//需要反馈数据
			num = 1200;	//一次发送1200个数据
			flag_send_ADC = 0;
			while(num>0)
			{
				SPI_write(0x0b);
				SPI_write(0xff);
				if(SPDAT==0x0b)
				{
					totalnum=0;
					flag=1;			 //是正确的数据了,开始组装SPI数据帧
					SPI_write(0x3b);
					SPI_write(0xff);
					SPI_write(0xff);
					spi_buff[3]=0xef;
					for(i=1;i<3;i++)
					SendUart(spi_buff[i]);	
					num--;
				} //if
				flag=0;
				//num--;
			} //while
		} //if

		 if(nOC2==0)
		{ //碰到了内部光耦，pwm_dir=1;进入，之后反转方向
		  //但可能多次触碰到内部光耦
	
			if(pwm_dir==0)	 //防止多次读取内部光耦信号
				continue;
			pwm_dir = 0;
			pwm_ena = 0;
			SendUart(0xff);//发送f0表示碰到内部光耦
			SendUart(0xf0);//发送f0表示碰到内部光耦
			SendUart(0xf0);//发送f0表示碰到内部光耦
			SendUart(0xff);//发送f0表示碰到内部光耦
		//	break;
		}  
	
	}//end while
}


/***************************************************************
			    串口1中断服务程序
****************************************************************/
void UART1_SER(void) interrupt 4 using 1
{
	if(RI)                        //判断是接收中断产生
	{	
		RI=0;                      //标志位清零
	  	rec_mes[rec_mes_num]=SBUF;    //读入缓冲区的值
		//SendUart(SBUF);

	  	if(rec_mes[0] == 0xFF)	   //判断是传过来的命令
			rec_mes_num++;
		else
			rec_mes_num = 0;
		if(rec_mes_num >= 13)	   //接受13个字节，表示一个命令接收完毕
		{
			uart_rec_status = 1; //帧接收完成
			rec_mes_num = 0;
		}
		else
			uart_rec_status = 0; //帧接未完成
	}
} 

/***************************************************************
			    INT1外部中断服务程序
****************************************************************/
void exint1()  interrupt 2           //(location at 0013H)
{   		  //外部光耦，中断实现
	
	if(pwm_dir==1)	  //防止一直卡在外部
		return;
	//while(nOC1==0); //中断触发后为0		
	pwm_ena = 0;
	pwm_dir=1;
	send();
} 

/***************************************************************
					定时器0中断服务程序
****************************************************************/
void timer0_isr(void) interrupt 1  using 2
{
	TL0 = pwmFreL;
//	TL0 = 64000;	//设置初值
//	TH0 = 64000>>8;
//	SendUart(0x12);
//	pwm_fre=~pwm_fre;
//	TF0=0;
}

/***************************************************************
					SPI中断服务程序
****************************************************************/
void SPI_ISR(void) interrupt 9 using 3
{
//    IE2&=0xfd;
	SPSTAT|=0x80;
//	spi_buff=SPDAT;
	if(flag==1)
	{
		if(totalnum==0)
			spi_buff[totalnum]=0xff;
		else
			spi_buff[totalnum]=SPDAT;
		totalnum++;
	}
//	SendUart(SPDAT);		 //通过串口发送AD转换的数据给ARM
//	spi_buff=0;
//	IE2|=0x02;
}

/*****************************************************************
函数名：通过SPI向ADC写入数据
调用：SPI_write()
参数：data1：给ADC写得数据
返回值：无
备注：无
*****************************************************************/
void SPI_write(unsigned char data1)
{
	IE2&=0xfd;
	SPDAT=data1;
	while((SPSTAT&0x80)==0);  //等待SPI数据传输完成
	IE2|=0x02;             //ESPI(IE2.1)=1,允许SPIF产生中断
}		 

/*******************************************************************
函数名：串口发送函数
调用： SendUart()
参数：dat：要通过串口发送的数据
返回值：无
备注：无
*******************************************************************/
void SendUart(unsigned char dat)
{
	SBUF = dat; //send current data
	while (!TI); //wait pre-data sent
	TI = 0; //clear TI flag,发送控制器
}
