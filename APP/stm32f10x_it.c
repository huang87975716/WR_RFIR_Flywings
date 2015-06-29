/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : stm32f10x_it.c
* Abstract    : 中断函数的实现，所有中断在此文件中实现
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
*
*****************************************************************************
*/

#include "stm32f10x_it.h"
#include "includes.h"
u8 Ir_REV_OK=0;

void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}

/********************************************************************************
 * FunctionName: SysTick_Handler
 *
 * Description : SYSTICK定时器中断
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void SysTick_Handler(void)
{
OS_CPU_SR cpu_sr;
OS_ENTER_CRITICAL();
OSIntNesting++;
OS_EXIT_CRITICAL();
OSTimeTick();
OSIntExit();
}




uint8_t u1RecvCh = 0;               // �洢���յ����ַ�
IDPROTOCOL_t *u1p = &gU1RecvBuff[0];  // ָʾ�����ַ
uint8_t u1BufferSwitch = 0;

uint8_t u2RecvCh = 0;               // �洢���յ����ַ�
PROTOCOL_t *u2p = &gU2RecvBuff[0];  // ָʾ�����ַ
uint8_t u2BufferSwitch = 0;

/********************************************************************************
 * FunctionName: USART1_IRQHandler
 *
 * Description : ����1�Ľ����ж�
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void USART1_IRQHandler(void)
{
    static uint8_t u1StaMach = 0;

     // USART1->DR = u1RecvCh;
    // �����������

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
         // ��ȡUSART1_DR�Ĵ������Զ���ոñ�־λ�
        u1RecvCh = (USART1->DR) & 0xFF;

         // ״̬���ƽ�
        if (u1StaMach == 0)
        {
            if (u1RecvCh == 0xAA)
            {
                u1p->header = u1RecvCh;
                u1StaMach = 1;
                u1p->checksum = u1RecvCh;
            }
        }
        else if (u1StaMach == 1)
        {
            u1p->direction = u1RecvCh;
            u1StaMach = 2;
            u1p->checksum += u1RecvCh;
        }
        else if (u1StaMach == 2)
        {
            u1p->msgref = u1RecvCh;
            u1StaMach = 3;
            u1p->checksum += u1RecvCh;
        }
        else if (u1StaMach == 3)
        {
            u1p->command = u1RecvCh;
            u1StaMach = 4;
            u1p->checksum += u1RecvCh;
        }
        else if (u1StaMach == 4)
        {
            u1p->length = u1RecvCh;
                if (u1p->length > 8)
            {
                u1StaMach = 0;
            }
            else
            {
                if (u1p->length == 0)  //0�ֽ�
                {
                    u1StaMach = 6;
                }
                else                 //1-7�ֽ�
                {
                    u1StaMach = 5;
                }
                u1p->dataIndex = 0;
                u1p->checksum += u1RecvCh;
            }
        }
        else if (u1StaMach == 5)
        {
            u1p->data[u1p->dataIndex++] = u1RecvCh;
            u1p->checksum += u1RecvCh;
            if (u1p->dataIndex == u1p->length)
            {
                u1StaMach = 6;
            }
        }
        else if (u1StaMach == 6)
        {
             // �ж�У���
            if (u1p->checksum == u1RecvCh)
            {
                if (u1BufferSwitch)
                {
                    OSQPost(ProtocolMBox, (void*)UART1_BUFF1_RECV);
                }
                else
                {
                    OSQPost(ProtocolMBox, (void*)UART1_BUFF0_RECV);
                }
            // �ɹ����յ�һ�����ݣ��л���������
                u1BufferSwitch ^= 1;
                u1p = &gU1RecvBuff[u1BufferSwitch];
            }
            u1StaMach = 0;
        }
    }
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}


/********************************************************************************
 * FunctionName: USART2_IRQHandler
 *
 * Description : ����2�Ľ����ж�
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
#if 0
void USART2_IRQHandler(void)
{
    static uint8_t u2StaMach = 0;
//    u8 i;
    if (USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
    {
      u2RecvCh = (USART2->DR) & 0xFF;
            if (u2StaMach == 0)
         {
            if (u2RecvCh == 0x16)  //head
            {
                u2p->header = u2RecvCh;
                u2StaMach = 1;
                u2p->checksum = u2RecvCh;
            }
         }
           else if (u2StaMach == 1)   //cmd
         {
            u2p->command = u2RecvCh;
            u2StaMach = 2;
            u2p->checksum += u2RecvCh;
         }

           else if (u2StaMach ==2)  //length
         {
            u2p->length = u2RecvCh;
                if (u2p->length == 0)  //0�ֽ�  ���ݲ�����ֱ�Ӻ�У��
                {
                    u2StaMach = 4;
                }
                else if (u2p->length == 208)            //208�ֽڻ�����  �������ݽ���
                {
                    u2StaMach = 3;
                }
                else    //�����ֽ�����
                    u2StaMach = 0;
                u2p->dataIndex = 0;
                u2p->checksum += u2RecvCh;
         }

        else if (u2StaMach == 3)  //data recieve
        {
            u2p->data[u2p->dataIndex++] = u2RecvCh;
            u2p->checksum += u2RecvCh;
            if (u2p->dataIndex == u2p->length)
            {
                u2StaMach = 4;
                u2p->dataIndex=0;
            }
        }

        else if (u2StaMach == 4)
        {
             // �ж�У���
            if((u2p->checksum)%256 == u2RecvCh)  //У���MOD 256
            {
                u2StaMach = 5;
            }
            else
            u2StaMach = 0;
        }
        else if (u2StaMach == 5)
        {
             u2p->end = u2RecvCh;
             // �ж�END
             if (learn_cmd==1)  //ѧϰ��ʼ��־λ����ѧϰ
          {
          //   learn_cmd=0;//��տ�ʼѧϰ��־λ
              Timout_Count=TIMOUT_20s;  //  ��ʱ����־��λ
             if(((u2p->command)==0x86)||((u2p->command)==0x8c))  //Э��end  ����ѧϰ�ɹ� 0x86  �յ�ѧϰ�ɹ�0x8c
            {
                    if((learn_DeviceName>=1)&&(learn_DeviceName<=10))//�յ� 1-10�յ�
                    {
                        if((learn_KeyCode>=1)&&(learn_KeyCode<=10))  //ÿ���յ��̶�10��
                        {
                         // Flash_Write(ARC_ADDR,u2p->data,208);//��ȡbuf����FLASH   ����̶�������ַ�ļ�ֵ��ARC1_ADDR+learn_DeviceName*2080+learn_KeyCode*208
                             Learn_State=1;//�յ�
                            OSQPost(TestMBox, (void*)Stu_Success); //ѧϰ�ɹ��ź�
                        }
                    }
                   else if ((learn_DeviceName>=11)&&(learn_DeviceName<=20)) // ����  10-19 ����
                   {
                       if((learn_KeyCode>=1)&&(learn_KeyCode<=30)) //ÿ�����ӹ̶�30��
                      {
                       //  Flash_Write(URC_ADDR,u2p->data,208);//��ȡbuf����FLASH
                           Learn_State=2;//����
                         OSQPost(TestMBox, (void*)Stu_Success); //ѧϰ�ɹ��ź�
                      }
                   }
                   else   //������ֵ��Χ
                   {
                       OSQPost(TestMBox, (void*)Stu_Fail); //ѧϰʧ���ź�
                   }
                u2StaMach = 0;
            }

            else if (((u2p->command)==0xc6)||((u2p->command)==0xcc))  //   ����ѧϰʧ��0xc6  �յ�ѧϰʧ��0xcc
            {
             OSQPost(TestMBox, (void*)Stu_Fail); //ѧϰʧ���ź�
             u2StaMach = 0;
            }
            /*
             else if((u2p->command==0x8d)||(u2p->command==0x87))//���Ϊ 87 8D�������·��ؽ���
            {
                u2StaMach = 0;
            }
            */
            else //��������
            {
             OSQPost(TestMBox, (void*)(u2p->command)); //ѧϰʧ���ź�
             u2StaMach = 0;
            }
          //  learn_cmd=0;//��տ�ʼѧϰ��־λ
          }
              u2StaMach = 0;
        }
       //Send2Char(u2RecvCh);
    }
  USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}

#endif

//���鶨�壬������������

u8 USART2_SEND_DATA[220];
u8 USART2_RECEIVE_DATA[220];
u8 USART2_TX_Finish=1; // USART2������ɱ�־λ

void USART2_IRQHandler(void)
{
    u32 cnt;
    u16 DATA_LEN;
    u8 USART2_ProBuf_DATA[220];
    u16 i;
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) //���Ϊ���������ж�
    {
        DMA_Cmd(DMA1_Channel6, DISABLE);//�ر�DMA,��ֹ�������������
        //USART_RX_STA = USART1->SR;//�ȶ�SR��Ȼ���DR�������
        //USART_RX_STA = USART1->DR;
        DATA_LEN=512-DMA_GetCurrDataCounter(DMA1_Channel6);
        if(DATA_LEN > 0)
        {
            while(USART2_TX_Finish==0)//�ȴ�������ɲ���һ��
            {
                               cnt++;
                if (cnt>0x70000)
                {
                    cnt=0;break;
                }
            }
                        cnt=0;
            //��������DMA�洢��ַ
            for(i=0;i<DATA_LEN;i++)
            {
               USART2_ProBuf_DATA[i]=USART2_RECEIVE_DATA[i];
            }
           ReV_CH361ReVProtocol(USART2_ProBuf_DATA);//����Э�����
#if 0
            //USART��DMA���������ѯ��ʽ���ͣ��˷��������ȼ��ж϶�������֡����
            DMA_Cmd(DMA1_Channel7, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
            DMA1_Channel7->CNDTR=DATA_LEN; //DMA1,����������
            USART2_TX_Finish=0;//DMA���俪ʼ��־��
            DMA_Cmd(DMA1_Channel7, ENABLE);
#endif
        }
        //DMA_Cmd(DMA1_Channel5, DISABLE);//�ر�DMA,��ֹ�������������
        DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6);//���־
        DMA1_Channel6->CNDTR = 512;//��װ��
        DMA_Cmd(DMA1_Channel6, ENABLE);//������,�ؿ�DMA
        //��SR���DR���Idle
        i = USART2->SR;
        i = USART2->DR;
    }
    if(USART_GetITStatus(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//����
    {
        USART_ClearITPendingBit(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE);
    }
    USART_ClearITPendingBit(USART2, USART_IT_TC);
    USART_ClearITPendingBit(USART2, USART_IT_IDLE);

}



//DMA1_Channel6�жϷ�����
void DMA1_Channel6_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_TC6);
  DMA_ClearITPendingBit(DMA1_IT_TE6);
  DMA_Cmd(DMA1_Channel6, DISABLE);//�ر�DMA,��ֹ�������������
  DMA1_Channel6->CNDTR = 580;//��װ��
  DMA_Cmd(DMA1_Channel6, ENABLE);//������,�ؿ�DMA
}


//DMA1_Channel7�жϷ�����
//USART2ʹ��DMA�������жϷ������
void DMA1_Channel7_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_TC7);
  DMA_ClearITPendingBit(DMA1_IT_TE7);
  DMA_Cmd(DMA1_Channel7, DISABLE);//�ر�DMA
  USART2_TX_Finish=1;//��DMA�������
}


/********************************************************************************
 * FunctionName: TIM4_IRQHandler
 *
 * Description : ��ʱ��4��PWM�����жϣ�PWM����ģʽ����������Ӧ
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void TIM4_IRQHandler(void)
{

}

/********************************************************************************
 * FunctionName: TIM2_IRQHandler
 *
 * Description : ��ʱ��2��PWM�����жϣ�PWM����ģʽ��PT2262���롣��������Ӧ
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void TIM2_IRQHandler(void)
{

}


/********************************************************************************
 * FunctionName: TIM1_CC_IRQHandler
 *
 * Description : TIM1�ıȽ�ƥ���ж�
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
    if (TIM_GetFlagStatus(TIM1,TIM_FLAG_CC1))
    {
        TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
    }
}

/********************************************************************************
 * FunctionName: EXTI15_10_IRQHandler
 *
 * Description : SI4432  SIGNAL
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void EXTI15_10_IRQHandler(void)
{

 //  SendString("EXTI_IRQ1\n");
    if(EXTI_GetITStatus(EXTI_Line11) != RESET )
    {
        if (si4432_rev_en==1)
        {
            OSQPost(Si4432ReadMBox, (void *)0x01); //4432����һ֡
        }

            EXTI_ClearITPendingBit(EXTI_Line11);
    }
}

void EXTI2_IRQHandler(void)
{

}

