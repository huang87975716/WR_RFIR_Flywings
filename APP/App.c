/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : App.c
* Abstract    : Ӧ�ó��������ļ���main�������.
* Author      : wuwei
*
* History
* date          author      notes
* 2014-04-14    wuwei       File Created.
*
*****************************************************************************
*/
#define GLOBALS_RFIR
#include "includes.h"


/********************************************************************************
 * FunctionName: main
 *
 * Description :  main ��ں���
 *
 * Parameters  : none.
 *
 * Returns     : none.
 *******************************************************************************/

int main(void)
{

    OSInit();

    //������������
    OSTaskCreate(TaskSet,
                 (void*)0,
                 &TaskSetStk[TASK_SET_STKSIZE-1],
                 TASK_SET_PRIO);
    OSStart();
    return 0;
}

