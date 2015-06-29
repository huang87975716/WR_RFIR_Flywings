/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : protocol.h
* Abstract    : ͨѶЭ�����ض���
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
* 2014-04-02    wuwei       File Modify.
*
*****************************************************************************
*/


#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef  GLOBALS_RFIR
#define PROTOCOL_EXT
#else
#define PROTOCOL_EXT extern
#endif

// ��·��ID�Ŷ���
//#define ID_GM8126           1
//#define ID_LEDPANEL         2
//#define ID_DRVBOARD         3
#define ID_RFIRBOARD         8
#define ID_DESTMASK         0x0F
#define ID_SOURCEMASK       0xF0


#define CMD_URC_INDEX                        0X01      //URC�ڲ��뷢������(��ͨ�豸����ң����)
#define CMD_ARC_INDEX                        0X02      //ARC�ڲ��뷢������յ��豸����ң������
#define CMD_URCLEN_START                     0X06      //����URCѧϰ����  ����
#define CMD_ARCLEN_START                     0X0C      //����ARCѧϰ����  �յ�
#define CMD_IRIC_VERSION                     0X08      //��ȡ�汾��Ϣ

// ָ���б�
#define CMD_SETID                            0x42    // д�ӻ�ID




#define CMD_CHT                              0x60    // ��ʪ�ȶ�ȡָ��
#define CMD_ASR                              0x64   // ����ʶ��ָ��


/***********������*****************/


#define CMD_CHARGERCTR                       0x8F    // ������ָ��
//#define CMD_LEARN4432ID                      0x90   // 4432IDѧϰָ��

/***********����ת��4432���**********/

#define CMD_SMARTHOME                        0x32    // ���ܼҾӺ���ת��ָ��
#define CMD_CHARGER_4432CMD                  0X83     // ���������4432ָ��
#define CMD_SMARTLEARN                       0x33    // ���ܼҾӺ���ת��ѧϰ

/***********4432���ת����**********/
#define CMD_FALLING                          0x30    // ����������ָ��
#define CMD_CHARGER_4432FB                   0X82     // �����4432ָ���
#define CMD_SMARTLEARN_FB                    0x34    // ���ܼҾӺ���ת��ѧϰ״̬����


typedef struct tagProtocolID
{
    uint8_t header;
    uint8_t direction;
    uint8_t msgref;
    uint8_t command;
    uint8_t length;
    uint8_t data[8];
    uint8_t checksum;
    uint8_t dataIndex;
    uint8_t buffswitch; // ????????
}IDPROTOCOL_t;

typedef struct tagProtocol
{
    uint8_t header;
    uint8_t command;
    uint8_t length;
    uint8_t data[208];
    uint8_t checksum;
    uint8_t end;
    uint8_t dataIndex;
    uint8_t buffswitch; // �����л�
}PROTOCOL_t;

typedef struct si44432tagProtocol
{

   //  uint64_t  rfslaveid;

    uint8_t rf_header;
    uint8_t rfdirection;
    uint8_t rfmsgref;
    uint8_t rfcommand;
    uint8_t rflength;
    uint8_t rfid[4];   //4�ֽ�ID
    uint8_t rfdata[4];
  //  uint8_t checksum;
    uint8_t rfdataIndex;
   // uint8_t buffswitch; // �����л�
}SI4432PROTOCOL_t;





// �����Ƿ���ҪACK
#define NEED_ACK_EN     0

typedef struct ack
{
    INT16U needACK;    // �����õ���Ӧ��
    INT16U recvACK;    // ʵ�ʵõ���Ӧ��
}ACK_t;

// �鿴�Ƿ���ҪӦ����Ϣ����ΪCMD����ҪӦ��
#define IS_PROTO_NEED_ACK(pProto) ((pProto->msgref & 0xC0) == 0xC0)

// Uart1��Uart2�Ľ��ջ������ֿ������Է�ֹ��������ͬʱ���յ����ݶ�����
PROTOCOL_EXT IDPROTOCOL_t gU1RecvBuff[2];
PROTOCOL_EXT PROTOCOL_t gU2RecvBuff[2];
//SI4432
PROTOCOL_EXT SI4432PROTOCOL_t SI4432RecvBuff[2];

// ���Ӧ�𻺳���
PROTOCOL_EXT ACK_t gACK[2];
//
extern PROTOCOL_t *u2p;

// Ӧ��
void SendAck(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto);
void SendIDAck(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto);
// ��������
void SendProtocol(USART_TypeDef *USARTx, PROTOCOL_t *pProto);
void SendIDProtocol(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto);

//���Ϳ�ʼѧϰ
void Send_IRcmd(USART_TypeDef *USARTx, INT8U CMD);
//����ѧϰ����
void Send_IRLenData(USART_TypeDef *USARTx, INT8U CMD,u8 *IRlen_buf);
//����ֹͣ����
void Send_Stop(USART_TypeDef *USARTx);
//����ѧϰֹͣ����
void Send_LenStop(USART_TypeDef *USARTx);
//Э�����
void ReV_CH361ReVProtocol(INT8U *buf);
#endif
