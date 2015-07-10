/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : protocol.c
* Abstract    : ͨѶЭ��ľ���ʵ��
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
*
*****************************************************************************
*/



#include "includes.h"
/********************************************************************************
 * FunctionName: SendAck
 *
 * Description : ����Ӧ���ź�
 *
 * Parameters  : USARTx-����ͨ����pProto-��Ӧ���Э��.
 *
 * Returns     : None.
 *******************************************************************************/

void SendIDAck(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto)
{
    IDPROTOCOL_t proto;
    proto.header = 0xAA;
    proto.direction = pProto->direction >> 4;
    proto.direction &= 0x0F;
    proto.direction |= (pProto->direction << 4);
    proto.msgref = pProto->msgref & 0x3F;
    proto.command = pProto->command;
    proto.length = 0;

    proto.checksum = proto.header;
    proto.checksum += proto.direction;
    proto.checksum += proto.msgref;
    proto.checksum += proto.command;
    proto.checksum += proto.length;

    SendIDProtocol(USARTx, &proto);
}

#if  0





/********************************************************************************
 * FunctionName: SendAck
 *
 * Description : ����Ӧ���ź�
 *
 * Parameters  : USARTx-����ͨ����pProto-��Ӧ���Э��.
 *
 * Returns     : None.
 *******************************************************************************/
void SendAck(USART_TypeDef *USARTx, PROTOCOL_t *pProto)
{
    PROTOCOL_t proto;
    proto.header = 0xAA;
    proto.direction = pProto->direction >> 4;
    proto.direction &= 0x0F;
    proto.direction |= (pProto->direction << 4);
    proto.msgref = pProto->msgref & 0x3F;
    proto.command = pProto->command;
    proto.length = 0;

    proto.checksum = proto.header;
    proto.checksum += proto.direction;
    proto.checksum += proto.msgref;
    proto.checksum += proto.command;
    proto.checksum += proto.length;

    SendProtocol(USARTx, &proto);
}
#endif

/********************************************************************************
 * FunctionName: SendProtocol
 *
 * Description : ����Э������
 *
 * Parameters  : USARTx-���͵�ͨ����pProto-�����͵�Э��.
 *
 * Returns     : None.
 *******************************************************************************/
void SendIDProtocol(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto)
{
      INT32U i;
    INT8U *p = (INT8U *)pProto;
#if (NEED_ACK_EN)
    ACK_t *pAck;
#endif

#if (NEED_ACK_EN)
    if ((INT32U)USARTx == (INT32U)USART1)
    {
        pAck = gACK[0];
    }
    else
    {
        pAck = gACK[1];
    }
#endif

    // ͨ������2ת��Э��
    for (i=0; i<5; i++)
    {
        USARTx->DR = *p++;
        while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    }
    for (i=0; i<pProto->length; i++)
    {
        USARTx->DR = *p++;
        while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    }
    USARTx->DR = pProto->checksum;
    while ((USARTx->SR & USART_FLAG_TXE) == RESET);
}


/********************************************************************************
 * FunctionName: SendProtocol
 *
 * Description : ����Э������
 *
 * Parameters  : USARTx-���͵�ͨ����pProto-�����͵�Э��.
 *
 * Returns     : None.
 *******************************************************************************/
void SendProtocol(USART_TypeDef *USARTx, PROTOCOL_t *pProto)
{
    INT32U i;
    INT8U *p = (INT8U *)pProto;
    INT8U cnt=0;
#if (NEED_ACK_EN)
    ACK_t *pAck;
#endif

#if (NEED_ACK_EN)
    if ((INT32U)USARTx == (INT32U)USART1)
    {
        pAck = gACK[0];
    }
    else
    {
        pAck = gACK[1];
    }
#endif
/*
    USARTx->DR = 0x00;
      while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    USARTx->DR = 0x00;
      while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    USARTx->DR = 0x00;
    while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    */
          USART2_SEND_DATA[0]=0;
          DMA_Cmd(DMA1_Channel7, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
          DMA1_Channel7->CNDTR=1; //DMA1,����������
          USART2_TX_Finish=0;//DMA���俪ʼ��־��
          DMA_Cmd(DMA1_Channel7, ENABLE);
    DelayMs(30);

    // ͨ������2ת��Э��
    for (i=0; i<3; i++)
    {
      //  USARTx->DR = *p++;
       //delay_10us(20);
      //  while ((USARTx->SR & USART_FLAG_TXE) == RESET);
      USART2_SEND_DATA[cnt]=*p++;
      cnt++;
    }
    for (i=0; i<pProto->length; i++)
    {
       // USARTx->DR = *p++;
       // delay_10us(20);
       // while ((USARTx->SR & USART_FLAG_TXE) == RESET);
      USART2_SEND_DATA[cnt]=*p++;
      cnt++;
    }
    //USARTx->DR = pProto->checksum;
    //      delay_10us(20);
    //while ((USARTx->SR & USART_FLAG_TXE) == RESET);
      USART2_SEND_DATA[cnt]=pProto->checksum;//*p++;
      cnt++;

    //USARTx->DR = pProto->end;
        //  delay_10us(20);
   // while ((USARTx->SR & USART_FLAG_TXE) == RESET);
      USART2_SEND_DATA[cnt]=pProto->end;//*p++;
      cnt++;
      DMA_Cmd(DMA1_Channel7, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
      DMA1_Channel7->CNDTR=cnt; //DMA1,����������
      USART2_TX_Finish=0;//DMA���俪ʼ��־��
      DMA_Cmd(DMA1_Channel7, ENABLE);


      cnt=0;
}


/********************************************************************************
 * FunctionName: Send_LRC
 *
 * Description : Sending the LRC  to study protocol.
 *
 * Parameters  : USARTx-Sending Port. irSignal-infrad signal in byte.
 *
 * Returns     : None.
 *******************************************************************************/
void Send_IRcmd(USART_TypeDef *USARTx, INT8U CMD)
{
    PROTOCOL_t IRCtrl;

    IRCtrl.header = 0x16;

    IRCtrl.command = CMD;

    IRCtrl.length = 0;


    IRCtrl.checksum =  IRCtrl.header;
    IRCtrl.checksum += IRCtrl.command;
    IRCtrl.checksum += IRCtrl.length;
    IRCtrl.checksum =  IRCtrl.checksum%256;
    IRCtrl.end      =   0x08;
    SendProtocol(USARTx, &IRCtrl);
}

void Send_Stop(USART_TypeDef *USARTx )
{   u16 i;
    for (i=0;i<400;i++)
    {
   //    USARTx->DR=(u8)0x00;
           USART2_SEND_DATA[0]=0;
         DMA_Cmd(DMA1_Channel7, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
         DMA1_Channel7->CNDTR=1; //DMA1,����������
         USART2_TX_Finish=0;//DMA���俪ʼ��־��
         DMA_Cmd(DMA1_Channel7, ENABLE);
       delay_10us(15);//   1MS
    }
}


void Send_LenStop(USART_TypeDef *USARTx )
{   u16 i;
    for (i=0;i<400;i++)
    {
      // USARTx->DR=(u8)0x00;
        USART2_SEND_DATA[0]=0;
         DMA_Cmd(DMA1_Channel7, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
        DMA1_Channel7->CNDTR=1; //DMA1,����������
          USART2_TX_Finish=0;//DMA���俪ʼ��־��
         DMA_Cmd(DMA1_Channel7, ENABLE);
       delay_10us(25);// 3515
    }
}

void Send_IRLenData(USART_TypeDef *USARTx, INT8U CMD,u8 *IRlen_buf)
{  u8 i;
    PROTOCOL_t IRCtrl;

    IRCtrl.header = 0x16;

    IRCtrl.command = CMD;

    IRCtrl.length = 208;
    for (i=0;i<IRCtrl.length;i++)  //4λID
    {
      IRCtrl.data[i] =*(IRlen_buf+i);
    }

    IRCtrl.checksum =  IRCtrl.header;
    IRCtrl.checksum += IRCtrl.command;
    IRCtrl.checksum += IRCtrl.length;
    for (i=0;i<IRCtrl.length;i++)  //4λID
    {
     IRCtrl.checksum+= IRCtrl.data[i];
    }

    IRCtrl.checksum =  IRCtrl.checksum%256;
    IRCtrl.end      =   0x08;
    SendProtocol(USARTx, &IRCtrl);
}

void ReV_CH361ReVProtocol(INT8U *buf)
{
     u8 u2StaMach=0,i=0,j=0;

         if (u2StaMach == 0)
         {
            if (buf[i] == 0x16)  //head
            {
                u2p->header = buf[i];
                u2StaMach = 1;
                u2p->checksum = buf[i];
                i++;
            }
            else u2StaMach = 0;
         }
            if (u2StaMach == 1)   //cmd
         {
            u2p->command = buf[i];
            u2StaMach = 2;
            u2p->checksum += buf[i];
            i++;
         }

           if (u2StaMach ==2)  //length
         {
                u2p->length = buf[i];
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
                u2p->checksum += buf[i];
                i++;
         }

          if (u2StaMach == 3)  //data recieve
        {
            for (j=0;j<208;j++)
            {
                u2p->data[u2p->dataIndex++] = buf[i];
                u2p->checksum += buf[i];
                i++;
            }
            //u2p->data[u2p->dataIndex++] = buf[i];
            // u2p->checksum += u2RecvCh;
            if (u2p->dataIndex == u2p->length)
            {
                u2StaMach = 4;
                u2p->dataIndex=0;
            }
        }
          if (u2StaMach == 4)
        {
             // �ж�У���
            if((u2p->checksum)%256 == buf[i])  //У���MOD 256
            {
                u2StaMach = 5;
                i++;
            }
            else
            u2StaMach = 0;
        }
          if (u2StaMach == 5)
        {
             u2p->end = buf[i];
             i++;
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
              Empty_Array(buf);//���buf
        }
       //Send2Char(u2RecvCh);
}

