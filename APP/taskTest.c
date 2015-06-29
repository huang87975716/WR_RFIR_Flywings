/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : taskProtoSend.c
* Abstract    : ����ѧϰ���� ר��4432Э�鷢�������ʵ��
* Author      : wuwei
*
* History
* date          author      notes
* 2014-07-07    wuwei       File Created.
*
*****************************************************************************
*/

#include "includes.h"
extern u8 Ir_REV_OK;
extern u8 test_charger[];
extern PROTOCOL_t *u2p;

u8 learn_switch=0;  //ѧϰ״̬
u8 learn_KeyCode=0; // ѧϰ�ļ�ֵ ( �յ�<10  ����<30)
u8 learn_DeviceName=0;   // �������ԣ��յ�������OR������  0x00-0x1f �յ�  0x20-3f ����
//u8 len_ok=0;   //ѧϰ�ɹ���־λ  1:�ɹ�  0�����ɹ�
u8 learn_cmd=0; //ѧϰ��ʼ��־λ  1:�ɹ�  0�����ɹ�
u8 Learn_State=0;//  1�յ�ѧϰ  2����ѧϰ
//u8 send_sig=0;  //�����ź� ��־λ 1��ѧϰ�ɹ� 2��ѧϰʧ�� 3������ѧϰ��
 //#define Operate_Page (32*256-1)//�������һ��ҳ
 //unsigned char M25P16_Buffer[M25P16_Page_Size]={0};//����һ���������ڴ�Ŷ�ȡ�����ݣ���������鶨�嵽main�ڣ���������ȷִ�У�ԭ���д���֤
 unsigned char ct361_Buffer[208]={0};//��CT361���仺��
/********************************************************************************
 * FunctionName: Tasktest
 *
 * Description : Э�鷢������
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/
void TaskTest(void *p_arg)
{
    INT8U err ;
//    u16 i;
  //  unsigned char buf[]="0123456789abcdefghijklmnopqrstuvwxyz";
    INT32U msg;
    u8 send_sig=0;  //�����ź� ��־λ 1��ѧϰ�ɹ� 2��ѧϰʧ�� 3������ѧϰ��
    u8  SI4432_RxLenth;// ���ճ���
    p_arg = p_arg;

    for (;;)
    {
    /********����4432����********/
    msg = (INT32U)OSQPend(TestMBox, 0, &err);
    send_sig= msg;
       if(send_sig!=0)
      {
           WatchDog_Feed(); //ι��
          if(send_sig==1)
          {
              if (Learn_State==1)//�յ�ѧϰ�ɹ�
              {
                   WatchDog_Feed(); //ι��
                 Send_LenStop(USART2);//ֹͣѧϰ
                  WatchDog_Feed(); //ι��
                 Flash_Write(ARC_ADDR,u2p->data,208);//��ȡbuf����FLASH   ����̶�������ַ�ļ�ֵ��ARC1_ADDR+learn_DeviceName*2080+learn_KeyCode*208
                 learn_cmd=0;//��տ�ʼѧϰ��־?
                 Learn_State=0;

                // SendString(" �յ�ѧϰ�ɹ���");
              }
              else if(Learn_State==2)//����ѧϰ�ɹ�
              {
                   WatchDog_Feed(); //ι��
                 Send_LenStop(USART2);//ֹͣѧϰ
                  WatchDog_Feed(); //ι��
                 Flash_Write(URC_ADDR,u2p->data,208);//��ȡbuf����FLASH
                 learn_cmd=0;//��տ�ʼѧϰ��־?
                 Learn_State=0;

                // SendString(" ����ѧϰ�ɹ���");
              }
             Send_4432ID0 =  MY_ID[0];// my id
             Send_4432ID1 =  MY_ID[1];
             Send_4432ID2 =  MY_ID[2];
             Send_4432ID3 =  MY_ID[3];
             Send_4432DIR = 0x82 ;//����
             Send_4432CMD = 0x34 ;//����ָ��
             Send_4432LENGTH= 3; //���ݳ���
             Send_4432DATA0 = send_sig;//ѧϰ�ɹ�
             Send_4432DATA1 = learn_DeviceName;//
             Send_4432DATA2 = learn_KeyCode;//
             Data_Encrypt(si4432cmd_senddata,18);   //
              WatchDog_Feed(); //ι��
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //ι��
               OSTimeDly(8);
                WatchDog_Feed(); //ι��
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //ι��
          }
          else if(send_sig==2)
          {
              learn_cmd=0;//��տ�ʼѧϰ��־?
              SendString(" ѧϰʧ�ܣ�");
             Send_4432ID0 =  MY_ID[0];// my id
             Send_4432ID1 =  MY_ID[1];
             Send_4432ID2 =  MY_ID[2];
             Send_4432ID3 =  MY_ID[3];
             Send_4432DIR = 0x82 ;//����
             Send_4432CMD = 0x34 ;//����ָ��
             Send_4432LENGTH= 3; //���ݳ���
             Send_4432DATA0 = send_sig;//ѧϰ�ɹ�
             Send_4432DATA1 = learn_DeviceName;//
             Send_4432DATA2 = learn_KeyCode;//
             Data_Encrypt(si4432cmd_senddata,18);   //
              WatchDog_Feed(); //ι��
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //ι��
               OSTimeDly(5);
                WatchDog_Feed(); //ι��
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //ι��
          }
          else if(send_sig==3)
              SendString(" ����ѧϰ��");
          else
          {
               learn_cmd=0;//��տ�ʼѧϰ��־?
               SendString(" ��������= ");
             Send_4432ID0 =  MY_ID[0];// my id
             Send_4432ID1 =  MY_ID[1];
             Send_4432ID2 =  MY_ID[2];
             Send_4432ID3 =  MY_ID[3];
             Send_4432DIR = 0x82 ;//����
             Send_4432CMD = 0x34 ;//����ָ��
             Send_4432LENGTH= 3; //���ݳ���
             Send_4432DATA0 = send_sig;//ѧϰ�ɹ�
             Send_4432DATA1 = learn_DeviceName;//
             Send_4432DATA2 = learn_KeyCode;//
             Data_Encrypt(si4432cmd_senddata,18);   //
              WatchDog_Feed(); //ι��
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //ι��
               OSTimeDly(5);
                WatchDog_Feed(); //ι��
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //ι��
             //  Outint(send_sig);
          }


         //if((send_sig==1)||(send_sig==2))//���ѧϰ�ɹ���ʧ�� ����豸�ź� ��ֵ ����ѧϰֹͣ�ź�
            if (send_sig!=3)
          {
           //Send_LenStop(USART2);
           learn_DeviceName = 0;
           learn_KeyCode = 0;
           learn_cmd=0;//��տ�ʼѧϰ��־λ
          }
         send_sig=0;
      }
//else  //����4432�ź�send_sig==0

    }
}
