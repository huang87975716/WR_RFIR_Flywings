/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : task.h
* Abstract    : ��������ص����������嶼�ڴ��ļ���ʵ��.
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
* 2014-04-02    wuwei       File Modify.
*
*****************************************************************************
*/

#ifndef __TASK_H
#define __TASK_H

#include "os_cpu.h"

#ifdef GLOBALS_RFIR
#define EXTERN
#else
#define EXTERN extern
#endif

/******************************************************************************
 ����1 * Э�����
 *****************************************************************************/
#define TASK_PROTOCOL_PRIO              7
#define TASK_PROTOCOL_STKSIZE           300
EXTERN OS_STK TaskProtocolStk[TASK_PROTOCOL_STKSIZE];
#define TASK_PROTOCOL_QSIZE             20u
EXTERN OS_EVENT* ProtocolMBox;
EXTERN void* ProtocolQBuff[TASK_PROTOCOL_QSIZE];
void TaskProtocol(void *p_arg);

/******************************************************************************
 ����2* Э�鷢��  ���ⷢ������
 *****************************************************************************/
#define TASK_PROTO_SEND_PRIO            6
#define TASK_PROTO_SEND_STKSIZE         512
EXTERN OS_STK TaskProtoSendStk[TASK_PROTO_SEND_STKSIZE];
#define TASK_PROTOSEND_QSIZE            20u
EXTERN OS_EVENT* ProtoSendMBox;
EXTERN void* ProtoSendQBuff[TASK_PROTOSEND_QSIZE];
void TaskProtoSend(void* p_arg);

/******************************************************************************
 ����3* ��������       ����ѧϰ����
 *****************************************************************************/
#define TASK_TEST_PRIO            5
#define TASK_TEST_STKSIZE         512
EXTERN OS_STK TaskTestStk[TASK_TEST_STKSIZE];
#define TASK_TEST_QSIZE                 10u
EXTERN OS_EVENT* TestMBox;
EXTERN void* TestQBuff[TASK_TEST_QSIZE];
void TaskTest(void* p_arg);


/******************************************************************************
 ����4* ��������   ָʾ�Ʒ���
 *****************************************************************************/
#define TASK_SET_PRIO            9
#define TASK_SET_STKSIZE         512
EXTERN OS_STK TaskSetStk[TASK_SET_STKSIZE];

/******************************************************************************
 ����5 * SI4432����
 *****************************************************************************/
#define TASK_Si4432_READ_PRIO           8
#define TASK_Si4432_READ_STKSIZE        512
EXTERN OS_STK TaskSi4432ReadStk[TASK_Si4432_READ_STKSIZE];
void TaskSi4432Read(void *p_arg);
#define TASK_Si4432READ_QSIZE           30u
EXTERN OS_EVENT* Si4432ReadMBox;
EXTERN void* Si4432ReadQBuff[TASK_Si4432READ_QSIZE];


/*
#define TASK_SET_QSIZE                 20u
EXTERN OS_EVENT* SetMBox;
EXTERN void* SetQBuff[TASK_SET_QSIZE];
*/
void TaskSet(void* p_arg);




#endif

