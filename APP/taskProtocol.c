/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : taskProtocol.c
* Abstract    : Э��ľ���ʵ��.
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
 * FunctionName: TaskProtocol
 *
 * Description : Э�鴦������
 *
 * Parameters  : p_arg-�������
 *
 * Returns     : None.
 *******************************************************************************/
void TaskProtocol(void *p_arg)
{
    MSG_t msg;
     INT8U err , i;
     IDPROTOCOL_t *pProto;
  //   INT8U *p;

 // SendString("TaskProtocol \n");
    for (;;)
    {
        msg = (MSG_t)OSQPend(ProtocolMBox, 0, &err);
      //  OSTimeDly(3000);
         //SendString("taskprotocol");

        switch (msg)
        {
            // ����1���յ�Э������
            case UART1_BUFF0_RECV:
            case UART1_BUFF1_RECV:
                pProto = &gU1RecvBuff[msg - UART1_BUFF0_RECV];
             //   p = (INT8U *)pProto;

                  if ((pProto->direction & ID_DESTMASK) == ID_RFIRBOARD)
                {
                        if (pProto->command == CMD_SETID)
                    {
                            Flash_Write(RFIR_ID_ADDR,pProto->data,4);//��ȡbuf����FLASH
                                SendString("\n\rID setok:");
                              Flash_Read(RFIR_ID_ADDR, MY_ID, 4);    //��ȡFLASH ID����BUF
                                for(i=0;i<4;i++)//sizeof(ID)
                                {
                                  SendString(";");
                                   Outint(MY_ID[i]);
                                }
                    }
                }
                    // �鿴�Ƿ���ҪӦ��
                   if (IS_PROTO_NEED_ACK(pProto))
                 {
                     // ����Ӧ������
                     SendIDAck(USART1, pProto);
                 }
                break;

            default:
                break;
        }

    }
}

