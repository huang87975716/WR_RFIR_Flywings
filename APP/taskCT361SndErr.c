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
  INT8U *msg;
	p_arg = p_arg;
	
	while(1)
	{
		OSSemPend(CT361SndErrSemp, 0, &err);
		if(err ==0) 
		{
			msg = OSQPend(CT361SndErrMbox, 10, &err);
			
			if(err == OS_TIMEOUT)
			{
				//send ERROR info and read version cmd
				Send_IRcmd(USART2, (INT8U)(CMD_IRIC_VERSION));
				SendTenZero();
				
			}
			else if(err == 0)
			{
				if((*msg)==0xC7) {};				//send error info 
			}
		}
	}
}

