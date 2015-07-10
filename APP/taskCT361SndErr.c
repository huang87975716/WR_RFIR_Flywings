#include "includes.h"
void SendTenZero(void)
{
	unsigned char i;
	for(i=0;i<10;i++) USART2_SEND_DATA[i]=0;
	USART2_SEND_DATA[0]=0;
	DMA_Cmd(DMA1_Channel7, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
	DMA1_Channel7->CNDTR=1; //DMA1,����������
	USART2_TX_Finish=0;//DMA���俪ʼ��־��
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
			SendString("waitting for 1.5S\r\n");
			OSTimeDlyHMSM(0,0,2,0);
			SendString("delay ended, sending read version cmd\r\n");
			Send_IRcmd(USART2, (INT8U)(CMD_IRIC_VERSION));
			SendTenZero();
		}
	}
}

