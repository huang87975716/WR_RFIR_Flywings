/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : taskset.c
* Abstract    : ���񴴽�ϵͳ��ʼ��  ��LED��ʾ
* Author      : wuwei
*
* History
* date          author      notes
* 2014-07-07    wuwei       File Created.
*
*****************************************************************************
*/

#include "includes.h"
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private define ------------------------------------------------------------*/
#define  FLASH_WriteAddress     0x700000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
#define  M25P64_FLASH_ID        0x202017
#define  BufferSize (countof(Tx_Buffer)-1)



FlagType flag;

 //INT8U ID[20];
u8 MY_ID[4] ={0};
/* Private variables ---------------------------------------------------------*/
//u8 Tx_Buffer[] = "STM32F10x SPI Firmware Library Example: communication with an M25P64 SPI FLASH";
//u8 Index;
u16 Timout_Count=TIMOUT_20s;//ѧϰ��ʱ���  30��
u16 Timout_Count2=200;//ѧϰ��ʱ���  30��
//u8 Rx_Buffer[BufferSize];
//volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;
//vu32 FLASH_ID = 0;
/********************************************************************************
 * FunctionName: TaskSet
 *
 * Description : ��������������ɾ������
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void TaskSet(void *p_arg)
{
   // INT8U i;
	//unsigned char k, data_flash_temp[208]={ 0xAE, 0x35, 0x24, 0x70, 0x80, 0xB7, 0x80, 0xBA, 0x1F, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x1D, 0x5C, 0x1B, 0x5E, 0x02, 0x77, 0x02, 0xED, 0xEF, 0x80, 0xB8, 0x80, 0xB9, 0x1D, 0x5C, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1F, 0x80, 0x9B, 0x1D, 0x5C, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x1D, 0x80, 0x9A, 0x1E, 0x80, 0xF5, 0x1E, 0x8F, 0x70, 0x02, 0xEA, 0x8C, 0x80, 0xB8, 0x80, 0xB9, 0x1D, 0x5C, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9A, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x1D, 0x80, 0x9A, 0x1E, 0x80, 0xF5, 0x1E, 0x85, 0x88, 0x80, 0xB8, 0x80, 0xB8, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9A, 0x1E, 0x5C, 0x1E, 0x5B, 0x1F, 0x5B, 0x1E, 0x5B, 0x1F, 0x5B, 0x1F, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x15, 0x80, 0xA4, 0x02, 0x86, 0xB7, 0x04, 0x81, 0x6D, 0x02, 0x77, 0x02, 0x77, 0x02, 0x77, 0x02, 0x77, 0x02, 0x77, 0x02, 0x77, 0x02, 0xAF, 0xFB, 0x02, 0x77, 0x1F, 0x5B, 0x1F, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x1D, 0x5C, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x00, 0x17};
	unsigned char k, data_flash_temp[208]={0xAE, 0x35, 0x24, 0x70, 0x80, 0xB7, 0x80, 0xBA, 0x1F, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x1D, 0x5C, 0x1B, 0x5E, 0x02, 0x77, 0x02, 0xED, 0xEF, 0x80, 0xB8, 0x80, 0xB9, 0x1D, 0x5C, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1F, 0x80, 0x9B, 0x1D, 0x5C, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x1D, 0x80, 0x9A, 0x1E, 0x80, 0xF5, 0x1E, 0x8F, 0x70, 0x02, 0xEA, 0x8C, 0x80, 0xB8, 0x80, 0xB9, 0x1D, 0x5C, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9A, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x1D, 0x80, 0x9A, 0x1E, 0x80, 0xF5, 0x1E, 0x85, 0x88, 0x80, 0xB8, 0x80, 0xB8, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9A, 0x1E, 0x5C, 0x1E, 0x5B, 0x1F, 0x5B, 0x1E, 0x5B, 0x1F, 0x5B, 0x1F, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x15, 0x80, 0xA4, 0x02, 0x86, 0xB7, 0x04, 0x81, 0x6D, 0x02, 0x77, 0x02, 0x77, 0x02, 0x77, 0x02, 0x77, 0x02, 0x77, 0x02, 0x77, 0x02, 0xAF, 0xFB, 0x02, 0x77, 0x1F, 0x5B, 0x1F, 0x5B, 0x1E, 0x5B, 0x1E, 0x80, 0x9B, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD7};	
    u32 seed;
    u8 random_data,i;
   //INT32U msg = 0;
    //INT8U err = 0;
    OS_CPU_SR  cpu_sr = 0;
    p_arg = p_arg;
    DelayMs(30);
    OS_ENTER_CRITICAL();
	
    BSP_Init();
    Flash_Read(RFIR_ID_ADDR, MY_ID, 4);    //��ȡFLASH ID����BUF
    #if 0
    SendString("\n\rMY ID:");
    for(i=0;i<4;i++)//sizeof(ID)
    {
      SendString(";");
       Outint(MY_ID[i]);
    }
#endif

/********************************************************************************
 * Description : ������   ��9������
 *******************************************************************************/

		CT361SndErrSemp = OSSemCreate(0);//creat sem
		
		//����Э���������
    OSTaskCreate(TaskProtocol,
                 (void*)0,
                 &TaskProtocolStk[TASK_PROTOCOL_STKSIZE-1],
                 TASK_PROTOCOL_PRIO);

    //�������ڷ�������
    OSTaskCreate(TaskProtoSend,
                 (void*)0,
                 &TaskProtoSendStk[TASK_PROTO_SEND_STKSIZE-1],
              TASK_PROTO_SEND_PRIO );

    //����������������
    OSTaskCreate(TaskTest,
                 (void*)0,
                 &TaskTestStk[TASK_TEST_STKSIZE-1],
                 TASK_TEST_PRIO );

    //����4432��������
    OSTaskCreate(TaskSi4432Read,
                 (void*)0,
                 &TaskSi4432ReadStk[TASK_SET_STKSIZE-1],
                 TASK_Si4432_READ_PRIO );

	//����CT361�������������
	OSTaskCreate(TaskCT361SndErr,\
					(void*)0,\
					&TaskCT361SndErrStk[TASK_CT361_SND_ERR_STKSIZE-1],\
					TASK_CT361_SND_ERR_PRIO);


/********************************************************************************
 * Description : ������Ϣ����
 *******************************************************************************/
  //  SetMBox  = OSQCreate(&SetQBuff[0], TASK_SET_QSIZE);
    ProtocolMBox  = OSQCreate(&ProtocolQBuff[0], TASK_PROTOCOL_QSIZE);
    ProtoSendMBox = OSQCreate(&ProtoSendQBuff[0], TASK_PROTOSEND_QSIZE);
    TestMBox      = OSQCreate(&TestQBuff[0], TASK_TEST_QSIZE);
    Si4432ReadMBox   = OSQCreate(&Si4432ReadQBuff[0], TASK_Si4432READ_QSIZE);
		CT361SndErrMbox = OSQCreate(&CT361SndErrQBuff[0],TASK_CT361_SND_ERR_QSIZE);
		

   OS_EXIT_CRITICAL();
   SysTickInit();

    //����ɾ��
   //  OSTaskDel(OS_PRIO_SELF);
    /*����ɾ���������Զ���������ĵ��ȣ���˴˴�����Ҫ�����ʱ*/

	Flash_Write(0x800E178,data_flash_temp,208);
  for (;;)
    {
         /**********ѧϰ״ָ̬ʾ��********/
            if(learn_cmd==1)  //ѧϰ��ʼ��־λ
            {
                                    LED_Send(0);// �̵ƹر�
                                    LED_Learn(1);//�����˸
                                     WatchDog_Feed(); //ι��
                                    OSTimeDly(4);
                                     WatchDog_Feed(); //ι��
                                    LED_Learn(0);
                                     WatchDog_Feed(); //ι��
                                    OSTimeDly(4);
                                     WatchDog_Feed(); //ι��
                                    Timout_Count--;
                                    if(Timout_Count==0)
                                {
                                    Timout_Count=TIMOUT_20s;
                                    OSQPost(TestMBox, (void*)Stu_Fail); //ѧϰʧ���ź�
                                    learn_cmd=0; //���ѧϰ�ɹ���־λ
                                }
            }
             if(learn_cmd==0)  //ѧϰ�ɹ���־λ
           {
                                    LED_Learn(0);//��ƹر�
                                    LED_Send(1); //�̵ƿ���
           }
       /**********���������������*********/
           seed+=23;
           if(seed>65000)
           seed=0;
           srand(seed);
           random_data=rand();
           test_roll_list=random_data^0x29;
                     if(learn_cmd==0)
                     {
                      Timout_Count2--;
                        if((Timout_Count2>=40)&&(Timout_Count2<=200))
                    {
                            LED_Send(0);
                    }
                    else if((Timout_Count2<40)&&(Timout_Count2>=1))
                    {
                            LED_Send(1);
                    //  Timout_Count2=200;
                    }
                    else if(Timout_Count2<=0)
                            Timout_Count2=200;
                        }
             WatchDog_Feed(); //ι��
           OSTimeDly(1);

    }
  }

