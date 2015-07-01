/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : taskProtoSend.c
* Abstract    : ר����Э�鷢�͵�����ʵ��
* Author      : wenwu
*
* History
* date          author      notes
* 2012-09-01    wenwu       File Created.
*
*****************************************************************************
*/

#include "includes.h"
u8 protosend_buf[3]={0};
/********************************************************************************
 * FunctionName: TaskProtoSend
 *
 * Description : Э�鷢������
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/
void TaskProtoSend(void *p_arg)
{
    INT8U err;
    INT8U *msg;

   static uint8_t IRSendStaMach = 0;
   static u8 cmd=0,DeviceName=0,DeviceCode=0;
   // SendString("TaskProtoSend \n");
    p_arg = p_arg;

    for (;;)
    {
          // �ȴ�200ms����û���յ���Ϣ�ͳ�ʱ
        msg =  OSQPend(ProtoSendMBox, 0, &err);

           memcpy(protosend_buf,msg,3);
           cmd=protosend_buf[0];
           DeviceName=protosend_buf[1];
           DeviceCode=protosend_buf[2];
           Timout_Count2=200;//���LEDcnt
           LED_Send(0);//
#if 1
                learn_DeviceName=DeviceName;
                learn_KeyCode= DeviceCode;
              //  IRSendStaMach =0;//״̬����λ
                if(cmd==0x32)//���ⷢ��
                {
                    if((learn_DeviceName >=1)&&(learn_DeviceName <=10)) //�յ�ң��
                        {
                              LED_Learn(1);
                               WatchDog_Feed(); //ι��
                              Flash_Read(ARC_ADDR, ct361_Buffer, 208);    //��ȡFLASH ID����BUF
                               WatchDog_Feed(); //ι��
                              Send_IRLenData(USART2,0x0d,ct361_Buffer);   //�յ�
															OSSemPost(CT361SndErrSemp);//
                             // SendString("�յ�����");
                               LED_Learn(0);
                               WatchDog_Feed(); //ι��
                              DelayMs(150);  //  250=625ms     //50 =125MS  25=62MS
                              WatchDog_Feed(); //ι��
													    DelayMs(150);
													    WatchDog_Feed(); //ι��
                            //  OSTimeDly(100);
                              Send_Stop(USART2);
                        }
                       else if ((learn_DeviceName>=11)&&(learn_DeviceName<=20))//����ң��
                        {
                              LED_Learn(1);
                               WatchDog_Feed(); //ι��
                              Flash_Read(URC_ADDR, ct361_Buffer, 208);    //��ȡFLASH ID����BUF
                               WatchDog_Feed(); //ι��
                          OSSemPost(CT361SndErrSemp);//    
													Send_IRLenData(USART2,0x07,ct361_Buffer);   //����
															
                                 LED_Learn(0);
                           //    SendString("���ӷ���");
                               WatchDog_Feed(); //ι��
                               DelayMs(30);
                               WatchDog_Feed(); //ι��
                              //OSTimeDly(100);
                              Send_Stop(USART2);

                          }
                DeviceName=0,DeviceCode=0;
                learn_DeviceName =0;//��� �豸��  ��ֵ ct361_buffer
                learn_KeyCode =0;
                }

                else if(cmd==0x33)//����ѧϰ
                {
                           if((learn_DeviceName>=1)&&(learn_DeviceName<=10))//�յ�
                        {
                            Send_IRcmd(USART2, (INT8U)(CMD_ARCLEN_START)); //(CMD_URCLEN_START  CMD_IRIC_VERSION
                           // learn_cmd=1;  //����ѧϰ
                             WatchDog_Feed(); //ι��
                            OSQPost(TestMBox, (void*)Stu_On); //����ѧϰ�ź�
                        }
                       else if ((learn_DeviceName>=11)&&(learn_DeviceName<=20))//����
                        {
                            Send_IRcmd(USART2, (INT8U)(CMD_URCLEN_START)); //(CMD_URCLEN_START  CMD_IRIC_VERSION
                            //learn_cmd=1; //����ѧϰ
                             WatchDog_Feed(); //ι��
                            OSQPost(TestMBox, (void*)Stu_On); //����ѧϰ�ź�
                        }
                     DeviceName=0,DeviceCode=0;
                }
                Empty_Array(ct361_Buffer);//��շ��仺��


#endif
        //SendString("sendproto");
      //  OSTimeDly(200);
        #if 0
       // ��ʱû���յ���Ϣ�Ĵ���
        if (err == OS_TIMEOUT)
        {
            // ��һ��ָ�����Դ�����ָ��
            if ((lastIRMsg>=IR_HEAD_UP)&&(lastIRMsg<=IR_HEAD_RIGHT))
            {
                 // ����ֹͣ�ź�
                SendIRCtrl(USART2, (INT8U)(IR_HEAD_STOP-IR_HEAD_UP));
            }
            // ��һ��ָ�����������ָ��
            else if ((lastIRMsg >= IR_BODY_RIGHT) && (lastIRMsg <= IR_BODY_LEFT))
            {
               // ����ֹͣ�ź�
                SendIRCtrl(USART2, (INT8U)(IR_BODY_STOP-IR_HEAD_UP));
            }
            lastIRMsg = 0;
            continue;
        }
         // ������Ϣ�ж�
        if ((msg >=IR_HEAD_UP) && (msg <= IR_NET_CFG))
        {
            // ���ϴ���Ϣһ���������κδ���
            if (msg == lastIRMsg)
            {
                continue;
            }
            // �µ���Ϣ
            else
            {
                // ��һ��ָ�����Դ�����ָ��
                if ((lastIRMsg>=IR_HEAD_UP)&&(lastIRMsg<=IR_HEAD_RIGHT))
                {
                   // ����ֹͣ�ź�
                    SendIRCtrl(USART2, (INT8U)(IR_HEAD_STOP-IR_HEAD_UP));
                }
                // ��һ��ָ�����������ָ��
                else if ((lastIRMsg >= IR_BODY_RIGHT) && (lastIRMsg <= IR_BODY_LEFT))
                {
                     // ����ֹͣ�ź�
                    SendIRCtrl(USART2, (INT8U)(IR_BODY_STOP-IR_HEAD_UP));
                }
                  // ��¼�ϴ���Ϣ
                lastIRMsg = msg;
            }
        }

        switch (msg)
        {
        // ����ң����ָ���
        case IR_HEAD_UP:
        case IR_HEAD_DOWN:
        case IR_HEAD_LEFT:
        case IR_HEAD_RIGHT:
        case IR_BODY_RIGHT:
        case IR_BODY_AHEAD:
        case IR_BODY_BACK:
        case IR_BODY_LEFT:
        case IR_HEAD_STOP:
        case IR_BODY_STOP:
            SendIRCtrl(USART2, (INT8U)(msg-IR_HEAD_UP));
            break;
        case IR_VOLUME_ADD:
        case IR_VOLUME_SUB:
        case IR_SECURITY:
        case IR_CHARGING:
        case IR_SLEEP:
        case IR_AUDIO_PLAY:
        case IR_AUDIO_PREV:
        case IR_AUDIO_NEXT:
        case IR_NET_CFG:
            SendIRCtrl(USART1, (INT8U)(msg-IR_HEAD_UP));
            break;
           // 315M�ź�
        case RF_315M_SIGNAL:
            SendRF315MSignal(USART1, gSysState.rfSignal);
            break;
            //�����������  ���͸�������
        case  FIR_SIGNAL :
            if(gSysState.firSignal)
            SendFIRSignal(USART2, gSysState.firSignal);
            break;
            //���״ָ̬��  ���͸�������
         case  CHARGERSTATUES_SIGNAL :
        // SendChargerSignal(USART2, );
            break;



           // RFM31B�ź�
       // case RFM31B_SIGNAL:
       // SendRFM31BSignal(USART1, gSysState.rfm31bSignal);
       //   break;
        default:
            break;
        }
       #endif
    }
}
