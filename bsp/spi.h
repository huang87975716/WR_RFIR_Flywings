#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"
//#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//#define SPI_ReadByte(SPIx)  SPI_WriteByte(SPIx,0)
unsigned char SPI_WriteByte(SPI_TypeDef* SPIx,unsigned char Byte);
void SPI1_Init(void);            //��ʼ��SPI��
void SPI1_SetSpeed(unsigned char SpeedSet); //����SPI�ٶ�
unsigned char SPI2_ReadWriteByte(unsigned char TxData);//SPI���߶�дһ���ֽ�
unsigned char spi_send_byte(unsigned char byte);
#endif

