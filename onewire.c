#include "reg52.h"

sbit DQ = P1^4;

//单总线延时函数

void Delay_OneWire(unsigned int t)  //STC89C52RC
{
	while(t--);
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ=1;
		Delay_OneWire(2);
		DQ = 0;
		Delay_OneWire(5);
		DQ = dat&0x01;
		Delay_OneWire(66);
		dat >>= 1;
		
	}
		DQ = 1;
		Delay_OneWire(11);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat=0;
  
	for(i=0;i<8;i++)
	{
		DQ=1;
		Delay_OneWire(2);
		dat >>= 1;
		DQ = 0;
		Delay_OneWire(4);
		DQ = 1;
		Delay_OneWire(4);
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(66);
	}
	DQ = 1;
	return dat;
}

//DS18B20初始化
bit init_ds18b20(void)
{
  	bit initflag = 1;
  	while(initflag)
		{
			DQ=1;
			Delay_OneWire(2);
			DQ = 0;
			Delay_OneWire(550); // 延时大于480us
			DQ = 1;
			Delay_OneWire(66);  
			initflag = DQ;     // initflag等于1初始化失败
		}
		Delay_OneWire(550);	
		DQ=1;
		
  	return initflag;
}

//DS18B20温度采集程序：整数
unsigned char rd_temperature(void)
{
    unsigned char low,high;
  	char temp;
    
  	init_ds18b20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0x44); //启动温度转换
  	Delay_OneWire(200);

  	init_ds18b20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0xBE); //读取寄存器

  	low = Read_DS18B20(); //低字节
  	high = Read_DS18B20(); //高字节
  
  	temp = high<<4;
  	temp |= (low>>4);
	
  	return temp;
		
}