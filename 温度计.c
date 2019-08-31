#include "reg52.h"  
#include "onewire.h" 
#include "absacc.h"

unsigned char dspbuf[8] = {10,10,10,10,10,10,0,0};//��ʾ������
unsigned char dspcom = 0;
unsigned char intr;
bit temper_flag = 0;//�¶ȶ�ȡ��־
code unsigned char tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                            0xff};

void display(void);

//������
void main(void)
{ 
    unsigned char temperature;
    
    TMOD |= 0x01;  //���ö�ʱ������ģʽ
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    EA = 1;
    ET0 = 1;  //�򿪶�ʱ���ж�
    TR0 = 1;  //������ʱ��
	
    while(1)
    {
        if(temper_flag)
		{
			temper_flag = 0;
			temperature = rd_temperature();  //���¶�         
        }
		
		//��ʾ���ݸ���    
		dspbuf[6] = temperature/10;       
		dspbuf[7] = temperature%10; 
    }
}

//��ʱ���жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //��ʱ������  
    display();
	if(++intr == 100)  //2msִ��һ��
	{
        intr = 0;
		temper_flag = 1;  //200ms�¶ȶ�ȡ��־λ��1
    }
}

//��ʾ����
void display(void)
{   
	P2 = ((P2&0x1f)|0xE0); 
	P0 = 0xff;
	P2 &= 0x1f;

	P2 = ((P2&0x1f)|0xC0); 
	P0 = 1<<dspcom;
	P2 &= 0x1f;
	
    P2 = ((P2&0x1f)|0xE0); 
	P0 = tab[dspbuf[dspcom]];
	P2 &= 0x1f;
  
    if(++dspcom == 8){
        dspcom = 0;
    }    
}