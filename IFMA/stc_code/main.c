/*************************************************************************************************
* �ļ�������SPI��ADͨ�ţ�PWM���Ƶ����IO�ڿ���led��,������ARMͨ��
* �����ļ���main.c
* ��    ����V0.1
* ��    �ڣ�2014.08.22
* ��    �ߣ���־ΰ������
* о    Ƭ��STC12C5A60S2
* ʱ    �ӣ�11.0592MHz
* ����������Keil uVision V4.00a
* ��    ע��
*			��Ƭ��P34��VCC��P06��P1.3��������M415-64������������ENA��OPTO��DIR��PUL����
*			ENA:ʹ���źţ���ֹ�������������������͵�ƽ��ֹ
*			OPTO��������Դ��+5V��
*			DIR�������źţ����ڸı���ת��TTL��ƽ����
*			PUL�������źţ���������Ч��ÿ�������ɵͱ��ʱ�����һ��
*
*			��Ƭ��P15��P16��P17��������ARM��SPIMOSI1��SPIMISO1��SPICLK1
*
*			��Ƭ��P10���ſ���LED
* �޸����Ŷ��壺
* 			time:2014.09.04
*			��Ƭ��P1.5��VCC��P1.4��P1.3��������M415-64������������ENA��OPTO��DIR��PUL����
*			��Ƭ��P4.1��P4.2��P4.3��������ARM��SPIMOSI1��SPIMISO1��SPICLK1
*			��Ƭ��P1.6���ſ���LED
***************************************************************************************************/


#include "stc12c5a60s2.h"	//����STC12C5A60S2�Ĵ��������ļ�
#include <I2C.H>

#define uchar unsigned char
#define SlaveAddress 0X60

sbit led = P1^6;	//LED����λ
sbit pwm_ena = P1^5;   //PWMʱ���ź�
sbit pwm_dir = P1^4;	//PWM�����ź�
sbit spi_cs = P4^0;
sbit nOC1=P3^3;      //�ⲿ�ж��ź�
sbit nOC2 = P3^4;      //�����ڲ�������Ϣ 
sbit nOC3 = P3^5;      //�����ж�

unsigned char D1=0;
unsigned char D2=0;
unsigned char rec_mes_num;	  //���ڶ�ȡ�������
unsigned char rec_mes[13];	  //���ڶ�ȡ����
unsigned char uart_rec_status; //֡���ݽ�����ɱ�־
unsigned char flag_send_ADC = 0;	//�Ƿ�Ҫ��ADת���������
unsigned char spi_buff[4];	 //SPI����֡
unsigned char uart_buff;
unsigned char flag=0;	 //SPI�Ƿ�ʼ�������ݱ�־
unsigned long int i;
unsigned char totalnum=0;	//��װSPI����֡����λ
unsigned int num=0;			//һ��SPI���ض��ٸ�����
unsigned char pwmFreL = 230;	//����PWMƵ��ֵ	230=133.34 240=211.779hz  250=512Hz 251=600 252=720 253=900.062 254=1.2kHz 255=3.6kHz
unsigned char frelight = 127;
unsigned int flag_noc2=0;
 int adcdata=0;
 int adc[1];
void SPI_write(unsigned char);
void SendUart(unsigned char);

/*
*����LED�����ȣ���12λ
*/
void write_mcp4725(unsigned char high, unsigned char low)
{
		unsigned char temp=0;
		Start_I2c();	//��������
		temp=(SlaveAddress << 1) | 0;	//����������ַ
		SendByte(temp);
		if(ack==0) return;
		SendByte(high);//���Ϳ����ֽڸ�λ
		if(ack==0) return;
		SendByte(low);//���Ϳ����ֽڵ�λ
		if(ack==0) return;
		Stop_I2c();	//��������
}


/*********************************************************************************************
��������SPI��ʼ������
���ã�SPI_init()
��������
����ֵ����
��ע����
*********************************************************************************************/
void SPI_init()
{
	AUXR1|=0x20;					//SPI��P1���л���P4��
    SPCTL=0xd0;                    //��ϸע������������ͬ
	SPSTAT=0xc0;                   //��0��־λSPIF��WCOL
	IE2=IE2|0x02;                  //ESPI(IE2.1)=1,����SPIF�����ж�

	EA=1;                      //�����ж�	
}


/*******************************************************************************************
����  ��������1��ʼ������
��    �ã�uart1_init()
��ڲ�������
�� �� ֵ����	
��    ע��STC12C5A60S2��Ƭ������1
		  ����Ƶ�ʣ�11.0592M��������9600		  
*******************************************************************************************/
void uart1_init(void)
{	
	//��ʼ������        //9600bps@11.0592MHz
    PCON &= 0x7f;            //�����ʲ�����
    SCON = 0x50;              //8λ����,�ɱ䲨����
    BRT = 0xDC;            //�趨���������ʷ�������װֵ
    AUXR |= 0x04;             //���������ʷ�����ʱ��ΪFosc,��1T
    AUXR |= 0x01;             //����1ѡ����������ʷ�����Ϊ�����ʷ�����
    AUXR |= 0x10;             //�������������ʷ�����

	EA = 1;	//�����ж�	
	ES = 1;	//�������ж�
	TR1 = 1;	//��������1	
}

/********************************************************************************************
����������ʱ��0��ʼ������
���ã�timer0_init();
��������
����ֵ����
��ע����
**********************************************************************************************/
void timer0_init(void)
{
//	TMOD = 0x01;	//ʹ��λ�����������������ʱ������Ӱ��
//	TL0 = (65536-pwmFrequency)%256;	//���ó�ֵ
//	TH0 = (65536-pwmFrequency)/256;
//	TL0=64000;
//	TH0=64000>>8;
	TMOD |=0x03;	//ʹ��ģʽ2��8λ�Զ���װģʽ��ʹ��λ�����������������ʱ������Ӱ��
 	TL0 = pwmFreL;
	EA = 1;		//�����ж�
	ET0 = 1; 	//����ʱ��0�ж�
	TR0 = 1;	//������ʱ��0
	TR1 = 0;
}


/*********************************************************************************************
��������PWM��ʼ������
��  �ã�PWM_init();
��  ������
����ֵ����
��  ������PCA��ʼ��ΪPWMģʽ����ʼռ�ձ�Ϊ0
��  ע����Ҫ����·PWM���ֱ�Ӳ���CCAPnH��CCAPnL����
/**********************************************************************************************/
void PWM_init (void)
{
	CMOD=0x04; //����PCA��ʱ��,ͨ��CPS2��CPS1��CPS0λ�ɵ�Ƶ��
    CL=0x00;   
    CH=0x00;
 	CCAPM0=0x42; //PWM0����PCA������ʽΪPWM��ʽ��0100 0010��
    CCAP0L=0x00; //����PWM0��ʼֵ��CCAP0H��ͬ
  	CCAP0H=0x00; // PWM0��ʼʱΪ0

//CCAPM1=0x42; //PWM1����PCA������ʽΪPWM��ʽ��ʹ��ʱɾ��//��
//CCAP1L=0x00; //����PWM1��ʼֵ��CCAP0H��ͬ
//CCAP1H=0x00; // PWM1��ʼʱΪ0

	CR=1; //����PCA��ʱ��
}

/*********************************************************************************************
��������INT1��ʼ������
���ã�INT1_init()
��������
����ֵ����
��ע����
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
��������PWM0ռ�ձ����ú���
��  �ã�PWM0_set();
��  ����0x00~0xFF�������0~255��
����ֵ����
��  ��������PWMģʽռ�ձȣ�Ϊ0ʱȫ���ߵ�ƽ��Ϊ1ʱȫ���͵�ƽ
��  ע�������ҪPWM1�����ú�����ֻҪ��CCAP0L��CCAP0H�е�0��Ϊ1����
/**********************************************************************************************/
void PWM0_set (unsigned char a)
{
	CCAP0L = a; //����ֱֵ��д��CCAP0L
	CCAP0H = a; //����ֱֵ��д��CCAP0H
}


/*********************************************************************************************
��������������
��  �ã���
��  ������
����ֵ����
��  ��������ʼ��������ѭ��
��  ע��
/**********************************************************************************************/
void main (void)
{
	
	led = 0;	//Ĭ��LED����	
	timer0_init();	//��ʱ��0��ʼ��


	PWM_init(); //PWM��ʼ��
	pwm_ena = 0;
	pwm_dir = 0;   //����
	PWM0_set(255); //����PWMռ�ձ�,�ܹ�255
	
	uart1_init();	//����1��ʼ��	

    SPI_init();	//SPI��ʼ��

	INT1_init();
	uart_buff=0;

	for(i=0;i<4;i++)
		spi_buff[i]=0;

	spi_cs=0;
	SPI_write(0x23);		 //AD��ʼ��
	SPI_write(0x1b);
	SPI_write(0x13);
	SPI_write(0x4e);

	uart_rec_status  = 0;

	while(1)
	{
		if(uart_rec_status)	   //֡������ɴ���
		{
			
			uart_rec_status = 0;	//���㣬������һ֡
			if(rec_mes[0]==0xFF && rec_mes[1]==0xFF && rec_mes[11]==0xEF && rec_mes[12]==0xEF)
			{
				/*֡����*/
				if(rec_mes[2] == 0x11)
				{	
					//��ADת���������
					flag_send_ADC = 1;					     
					rec_mes[2] = 0x10;//��ʱ�ر�SPI����Ҫ���´�while�ٷ�������	
				}
				else
					flag_send_ADC = 0;

				if(rec_mes[3] == 0x11)
				{
					//LED���� 
					led = 1;
					//����LED������
					write_mcp4725(rec_mes[9],rec_mes[10]);					
				}
				else
				{
					led = 0;
				}
				
				if(rec_mes[4] == 0x11)	 //���ʹ��
				{
															
					if(rec_mes[5] == 0x11)
					{
						//����
						pwm_dir = 0;					    
					}
					else if(rec_mes[5] == 0x10)
					{
						//���
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
			//��Ҫ��������
			num = 1200;	//һ�η���1200������
			flag_send_ADC = 0;
			while(num>0)
			{
				SPI_write(0x0b);
				SPI_write(0xff);
				if(SPDAT==0x0b)
				{
					totalnum=0;
					flag=1;			 //����ȷ��������,��ʼ��װSPI����֡
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
		{ //�������ڲ����pwm_dir=1;���룬֮��ת����
		  //�����ܶ�δ������ڲ�����
	
			if(pwm_dir==0)	 //��ֹ��ζ�ȡ�ڲ������ź�
				continue;
			pwm_dir = 0;
			pwm_ena = 0;
			SendUart(0xff);//����f0��ʾ�����ڲ�����
			SendUart(0xf0);//����f0��ʾ�����ڲ�����
			SendUart(0xf0);//����f0��ʾ�����ڲ�����
			SendUart(0xff);//����f0��ʾ�����ڲ�����
		//	break;
		}  
	
	}//end while
}


/***************************************************************
			    ����1�жϷ������
****************************************************************/
void UART1_SER(void) interrupt 4 using 1
{
	if(RI)                        //�ж��ǽ����жϲ���
	{	
		RI=0;                      //��־λ����
	  	rec_mes[rec_mes_num]=SBUF;    //���뻺������ֵ
		//SendUart(SBUF);

	  	if(rec_mes[0] == 0xFF)	   //�ж��Ǵ�����������
			rec_mes_num++;
		else
			rec_mes_num = 0;
		if(rec_mes_num >= 13)	   //����13���ֽڣ���ʾһ������������
		{
			uart_rec_status = 1; //֡�������
			rec_mes_num = 0;
		}
		else
			uart_rec_status = 0; //֡��δ���
	}
} 

/***************************************************************
			    INT1�ⲿ�жϷ������
****************************************************************/
void exint1()  interrupt 2           //(location at 0013H)
{   		  //�ⲿ����ж�ʵ��
	
	if(pwm_dir==1)	  //��ֹһֱ�����ⲿ
		return;
	//while(nOC1==0); //�жϴ�����Ϊ0		
	pwm_ena = 0;
	pwm_dir=1;
	send();
} 

/***************************************************************
					��ʱ��0�жϷ������
****************************************************************/
void timer0_isr(void) interrupt 1  using 2
{
	TL0 = pwmFreL;
//	TL0 = 64000;	//���ó�ֵ
//	TH0 = 64000>>8;
//	SendUart(0x12);
//	pwm_fre=~pwm_fre;
//	TF0=0;
}

/***************************************************************
					SPI�жϷ������
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
//	SendUart(SPDAT);		 //ͨ�����ڷ���ADת�������ݸ�ARM
//	spi_buff=0;
//	IE2|=0x02;
}

/*****************************************************************
��������ͨ��SPI��ADCд������
���ã�SPI_write()
������data1����ADCд������
����ֵ����
��ע����
*****************************************************************/
void SPI_write(unsigned char data1)
{
	IE2&=0xfd;
	SPDAT=data1;
	while((SPSTAT&0x80)==0);  //�ȴ�SPI���ݴ������
	IE2|=0x02;             //ESPI(IE2.1)=1,����SPIF�����ж�
}		 

/*******************************************************************
�����������ڷ��ͺ���
���ã� SendUart()
������dat��Ҫͨ�����ڷ��͵�����
����ֵ����
��ע����
*******************************************************************/
void SendUart(unsigned char dat)
{
	SBUF = dat; //send current data
	while (!TI); //wait pre-data sent
	TI = 0; //clear TI flag,���Ϳ�����
}
