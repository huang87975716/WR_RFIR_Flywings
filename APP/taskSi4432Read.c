/*
*****************************************************************************
* Copyright (c) 2014, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : tasksi4432Read.c
* Abstract    : si4432  READ
* Author      : wuwei
*
* History
* date          author      notes
* 2014-01-08    wuwei       File Created.
*
*****************************************************************************
*/

#include "includes.h"

u8 Protocolrev_en=0;
void TaskSi4432Read(void* p_arg)
{
//  EXTI_InitTypeDef EXTI_InitStructure;
     INT32U msg;
     INT8U err;
    u8  SI4432_RxLenth,i;//���ճ���
    for (;;)
    {
      msg = (MSG_t)OSQPend(Si4432ReadMBox, 0, &err);

              if(msg==0x01)//  if(flag.Si4432Rx_Status ==1) // //������ڷ��估����ʶ��״̬�ж�,�������
      {
            WatchDog_Feed(); //ι��
                     // if( SI4432_NIRQ == 0 )    //��������ж�
                        {
                            //disable the receiver chain
                             SI4432_WriteReg(0x07, 0x01);                                                   //write 0x01 to the Operating Function Control 1 register
                            ItStatus1 = SI4432_ReadReg(0x03);
                            ItStatus2 = SI4432_ReadReg(0x04);
                              /*CRC Error interrupt occured*/
                              if ( (ItStatus1 & 0x01) == 0x01 )
                              {
                                       //reset the RX FIFO
                                        SI4432_WriteReg(0x08, 0x02);                                                //write 0x02 to the Operating Function Control 2 register
                                        SI4432_WriteReg(0x08, 0x00);                                                //write 0x00 to the Operating Function Control 2 register
                              }

                                if ( (ItStatus1 & 0x02) == 0x02 )
                            {
                                    SI4432_RxLenth = SI4432_ReadReg(0x4B);
                                for (SI4432_RxCount=0;SI4432_RxCount < SI4432_RxLenth;SI4432_RxCount++)
                                {
                                    SI4432_RxBUFF[SI4432_RxCount] =  SI4432_ReadReg(0x7F) ;
                                }

                                 WatchDog_Feed(); //ι��
                                 Protocolrev_en=1;
                            }
                                SI4432_WriteReg(0x08, 0x02);
                                SI4432_WriteReg(0x08, 0x00);
                                SI4432_WriteReg(0x07, 0x05); //�ֶ��򿪽���
                                if (Protocolrev_en==1)
                            {
                                     Protocolrev_en=0;
                                 Data_Decrypt(SI4432_RxBUFF,SI4432_RxLenth);// ����
                                SI4432_Protocol(recieve_data);//Э�����
                                  WatchDog_Feed(); //ι��
                                Empty_Array(recieve_data);// ���BUFF
                                Empty_Array(SI4432_RxBUFF);// ���BUFF
                            }

                        // EXTI_ClearITPendingBit(EXTI_Line13);
                        }
        }


    }
}

