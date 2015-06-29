#ifndef __base_H
#define __base_H

#include "stm32f10x.h"
/**********************************��־λ����************************************************/
typedef struct
{
    uint8_t Si4432Rx_Status : 1 ;       //SI4432����״̬   1������   0��������
    uint8_t Si4432Tx_Status : 1 ;       //SI4432����״̬   1������   0��������

}FlagType;

extern FlagType Flag;

void SendChar(u8 cha);
void SendString(u8 ch[]);
void Send2Char(u8 cha);
void Send2String(u8 ch[]);

void Outint(long i);
void delay_10us(u16 nTime);
void DelayMs(u16 nTime);
#endif

