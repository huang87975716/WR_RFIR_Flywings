#include "includes.h"
void SendTenZero(void)
{
	unsigned char i;
	for(i=0;i<10;i++) USART2_SEND_DATA[i]=0;
	USART2_SEND_DATA[0]=0;
	DMA_Cmd(DMA1_Channel7, DISABLE); //改变datasize前先要禁止通道工作
	DMA1_Channel7->CNDTR=1; //DMA1,传输数据量
	USART2_TX_Finish=0;//DMA传输开始标志量
	DMA_Cmd(DMA1_Channel7, ENABLE);
}
void TaskCT361SndErr(void *p_arg)
{
	INT8U err;
  INT8U msg;
	p_arg = p_arg;
	
	while(1)
	{
		err = 0;
		SendString("start SemPend\r\n");
		OSSemPend(CT361SndErrSemp, 0, &err);
		SendString("end of SemPend\r\n");
		if(err ==0) 
		{
			SendString("waitting for 500 ms\r\n");
			//OSTimeDlyHMSM(0,0,1,500);
			//SendString("delay ended, sending read version cmd\r\n");
			//Send_IRcmd(USART2, (INT8U)(CMD_IRIC_VERSION));
			//SendTenZero();
			#if 0		//this is waitting for echo from CT361
			SendString("start QPend\r\n");
			msg = (INT8U)OSQPend(CT361SndErrMbox, 10, &err);
			SendString("end QPend\r\n");
			if(err == OS_TIMEOUT)
			{
				//send ERROR info and read version cmd
				Send_IRcmd(USART2, (INT8U)(CMD_IRIC_VERSION));
				SendTenZero();
				SendString("time out\r\n");
			}
			else if(err == 0)
			{
				if((msg)==0xC7) 
				{
					SendString("C7 error\r\n");
				}
				if((msg)==0x87)
				{
					SendString("sent success\r\n");
				}
			}
			#endif
		}
	}
}

unsigned char ChkLenData(unsigned char data[])
{
	unsigned char i = 0;
	unsigned char CountOfZero = 0;
	unsigned char Count = 0;
	unsigned char temp;
	for(i=0;i<208;i++)
	{
		temp = data[i];
		if (data[i]==0)	Count++;
		else 
		{
			
			CountOfZero = (Count>CountOfZero?Count:CountOfZero);
			Count = 0;
		}
	}
	return CountOfZero;
}
