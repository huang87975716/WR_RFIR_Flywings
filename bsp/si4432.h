/*
*****************************************************************************
* Copyright (c) 2014, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : SI4432.h
* Abstract    : SI4432  ����ģ��
* Author      : wuwei
*
* History
* date          author      notes
* 2014-01-07    wuwei       File Created.
*
*****************************************************************************
*/

#ifndef __SI_4432
#define __SI_4432

#ifdef GLOBALS_RFIR
#define SPI_EXTERN
#else
#define SPI_EXTERN extern
#endif

// 硬件SPI接口
#define HARDWARE_SPI        0
// 采用软件GPIO口模拟SPI接口
#define SOFTWARE_SPI        1

#define SPI_TYPE            HARDWARE_SPI

#define SI4432_NSEL_HIGH    GPIO_SetBits(GPIOB, GPIO_Pin_12)  //SI4432_NSEL  ��ֹ
#define SI4432_NSEL_LOW     GPIO_ResetBits(GPIOB, GPIO_Pin_12)  //SI4432_NSEL  ʹ��

#define SI4432_SDN_HIGH   GPIO_SetBits(GPIOB, GPIO_Pin_10)   //SI4432_SDN   ʹ��
#define SI4432_SDN_LOW    GPIO_ResetBits(GPIOB, GPIO_Pin_10)   //SI4432_SDN   ʹ��

#define SI4432_NIRQ       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)  //SI4432_NIRQ  �ж�״̬
extern u8 si4432_rev_en;
extern u8 ItStatus1,ItStatus2,res;
extern u8 SI4432_RxBUFF[];
extern u8 SI4432_RxCount;
extern u8  Si4432Read_Status ;   //SI4432��ʶ���־λ
extern u8  Si4432Write_Status ;   //SI4432дʶ���־λ
extern u8 hombot_buf[12];
extern u8 SI4432_RxBUFF[];

u8   SI4432_ReadReg(u8  addr) ;  //��ȡ�Ĵ�����ֵ
void SI4432_WriteReg(u8 addr, u8 value) ; //д�Ĵ���
void SI4432_GPIOSet(void);
void SI4432_Configuration(void );
void SI4432_Init(void)  ;//����


//void sending(u8  buf[]);
void sending(u8 *buf ,u8 data_legth); //���͵����ݣ�����

//unsigned short SpiReadWriteWord(unsigned short spi_in);

#endif
