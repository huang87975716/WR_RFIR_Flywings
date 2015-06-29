/*
*****************************************************************************
* Copyright (c) 2014, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : si4432.c
* Abstract    : SI4432  ����ģ��
* Author      : wuwei
*
* History
* date          author      notes
* 2014-01-07    wuwei      File Created.
*
*****************************************************************************
*/


#include "includes.h"
FlagType  aa;
u8 SI4432_RxBUFF[20];
u8 SI4432_RxCount;
u8 ItStatus1,ItStatus2,res;
u8 si4432_rev_en;
//u8 hombot_buf[12]= {"hombot1"} ;
// ��ȡ�Ĵ���

u8  SI4432_ReadReg(u8  addr)   //��ȡ�Ĵ�����ֵ
{
    uint8_t temp = 0;
    SI4432_NSEL_LOW;
    SPI2_ReadWriteByte(addr);    //���Ͷ�ȡ�Ĵ�����ַ
    temp = SPI2_ReadWriteByte(0Xff);
    SI4432_NSEL_HIGH;
    return temp;
}
//д�Ĵ���
void SI4432_WriteReg(u8 addr, u8 value)  //д�Ĵ���
{
    SI4432_NSEL_LOW;
    spi_send_byte(addr | 0x80);
    spi_send_byte(value);
    SI4432_NSEL_HIGH;
}

void SI4432_Configuration(void)
{
    si4432_rev_en = 0;
    SI4432_GPIOSet();
    SI4432_SDN_HIGH;
    DelayMs(100);
    SI4432_SDN_LOW;
    SI4432_Init();
    si4432_rev_en = 1;
}

void SI4432_GPIOSet(void)
{   SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //A,Cʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // Pb12 = NSEL ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  // PB12 =SI_SDN    ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_10);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //���ó��ⲿ�ж�ģʽNIRQ �����ж�ʱΪ��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
    EXTI_ClearITPendingBit(EXTI_Line11);

    SI4432_NSEL_HIGH;
    SI4432_SDN_LOW;
    DelayMs(10);


    //spi�˿�����
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);       //ʹ��SPI2����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);       //ʹ��SPI2����ʱ��
                                                                                      //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    //SCK/MISO/MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi��������
    SPI_Cmd(SPI2, DISABLE);
    /* SPI2���� */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                          //��ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                      //8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                           //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                              //�������NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;   //�����ʿ��� SYSCLK/128
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //���ݸ�λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;                               //CRC����ʽ�Ĵ�����ʼֵΪ7
    SPI_Init(SPI2, &SPI_InitStructure);
    /* ʹ��SPI2 */
    SPI_Cmd(SPI2, ENABLE);

}




void SI4432_Init(void)  //����
{  u32 i = 0;
    SI4432_NSEL_LOW;
    DelayMs(20);
//��ȡ�ж�״̬   ����ж�  �ͷ�  SI4432_NIRQ
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);
//�����λ
    SI4432_WriteReg(0x07, 0x80); //��0X07��ַ  д��0X80  �����λ
                                 // SendString("init1");
                                 //while ( SI4432_NIRQ == 1);
    while(SI4432_NIRQ == 1)
    {
        //  i=i;
        i++;
        if(i > 0x70000)
        {
            i = 0;
            break;
        }
    }
    i = 0;
    // SendString("init2");
    //��ȡ�ж�״̬   ����ж�  �ͷ�  SI4432_NIRQ
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);
/////////////////////////��ʼ����
// Ƶ������ 434
    SI4432_WriteReg(0x75, 0x53);//53
    SI4432_WriteReg(0x76, 0x64);  //
    SI4432_WriteReg(0x77, 0x00);
// 1.2K bps ��������
    SI4432_WriteReg(0x2a, 0x14);
    SI4432_WriteReg(0x6e, 0x09);
    SI4432_WriteReg(0x6f, 0xd5);
    SI4432_WriteReg(0x70, 0x2c); //

//SpiWriteRegister(0x72, 0x48); //(9.6kbps)
    SI4432_WriteReg(0x72, 0x38);    //Ƶ��ƫ��(45KHZ)
// ��������ø���Silabs ��Excel (1.2 kbps, deviation: 45 kHz, channel filter BW: 102.2 kHz
    SI4432_WriteReg(0x1C, 0x1b);  //1b                                                          //write 0x1E to the IF Filter Bandwidth register
    SI4432_WriteReg(0x20, 0x83);                                                            //write 0xD0 to the Clock Recovery Oversampling Ratio register
    SI4432_WriteReg(0x21, 0xc0);                   	                                         //write 0x00 to the Clock Recovery Offset 2 register
    SI4432_WriteReg(0x22, 0x13);                                                            //write 0x9D to the Clock Recovery Offset 1 register
    SI4432_WriteReg(0x23, 0xa9);                                                            //write 0x49 to the Clock Recovery Offset 0 register
    SI4432_WriteReg(0x24, 0x00);                                                            //write 0x00 to the Clock Recovery Timing Loop Gain 1 register
    SI4432_WriteReg(0x25, 0x03);                                                            //write 0x24 to the Clock Recovery Timing Loop Gain 0 register
    SI4432_WriteReg(0x1D, 0x40);                                                            //write 0x40 to the AFC Loop Gearshift Override register
    SI4432_WriteReg(0x1E, 0x0A);                                                            //write 0x0A to the AFC Timing Control register
    SI4432_WriteReg(0x2A, 0x14);//14                    	                                        //write 0x20 to the AFC Limiter register
//ǰ���� ͬ����
    SI4432_WriteReg(0x34, 0X0A);  // ����5�ֽڵ�Preamble
    SI4432_WriteReg(0x35, 0x2A);  // ��Ҫ��� Preamble
#if 0
    SI4432_WriteReg(0x33, 0x02); // ͬ����3,2 ��ͬ����
    SI4432_WriteReg(0x36, 0x44);  // ͬ����Ϊ 0x2dd4
    SI4432_WriteReg(0x37, 0x55);
#endif
#if 1
    SI4432_WriteReg(0x33, 0x04); //  3�ֽ�
    SI4432_WriteReg(0x36, 0x72);  //ͬ���� 0x726684
    SI4432_WriteReg(0x37, 0x66);
    SI4432_WriteReg(0x38, 0x84);
#endif
    SI4432_WriteReg(0x30, 0x8D);   // ʹ��PH+ FIFOģʽ����λ��ǰ�棬ʹ��CRCУ�� CCITTT
    SI4432_WriteReg(0x32, 0x00); // ��ֹ֡ͷ
    SI4432_WriteReg(0x71, 0x63); // ���䲻��Ҫ CLK��FiFo �� FSKģʽ

//GPIO
    SI4432_WriteReg(0x0b, 0x12); //
    SI4432_WriteReg(0x0c, 0x15); //
//��������
    SI4432_WriteReg(0x09, 0xD7);  //  ���ص���
    SI4432_WriteReg(0x69, 0x60);  //AGC����
//���书��
    SI4432_WriteReg(0x6d, 0x1e);
//�ֶ��򿪽���
    SI4432_WriteReg(0x07, 0x05);
//�� �����ж�
    SI4432_WriteReg(0x05, 0x03);
    SI4432_WriteReg(0x06, 0x00);
//���ж�
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);
    SI4432_WriteReg(0x07, 0x05); //�򿪽���
    SI4432_WriteReg(0x0d, 0xf4); //GPIO2��������

}


void sending(u8 *buf, u8 data_legth)  //���ݣ����ȣ���ַ
{

    //  u8 Length=0;
    u32 i = 0;

    SI4432_WriteReg(0x07, 0x01);    // rf ģ�����Ready ģʽ
    DelayMs(10);

    SI4432_WriteReg(0x3e, data_legth); //Send_Legth);  // ��ȡ�ֽ��� ���ܹ�����Length���ֽڵ�����
    for(i = 0; i < data_legth; i++)
    {
        SI4432_WriteReg(0x7F,  *(buf + i));
    }

    SI4432_WriteReg(0x05, 0x04);    // �������ݷ�����󣬲����ж�
    SI4432_WriteReg(0x06, 0x00);
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);
    SI4432_WriteReg(0x07, 0x09); //�򿪷���
                                 // while ( SI4432_NIRQ == 1);
                                 //  DelayMs(120);
    while(SI4432_NIRQ == 1)
    {
        //  i=i;
        i++;
        if(i > 0x70000)
        {
            i = 0;
            break;
        }
    }
    i = 0;
    //read interrupt status registers to release the interrupt flags
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);

    //after packet transmission set the interrupt enable bits according receiving mode
    //Enable two interrupts:
    // a) one which shows that a valid packet received: 'ipkval'
    // b) second shows if the packet received with incorrect CRC: 'icrcerror'
    SI4432_WriteReg(0x05, 0x03);                                                //write 0x03 to the Interrupt Enable 1 register
    SI4432_WriteReg(0x06, 0x00);                                                //write 0x00 to the Interrupt Enable 2 register
                                                                                //read interrupt status registers to release all pending interrupts
    ItStatus1 = SI4432_ReadReg(0x03);                                           //read the Interrupt Status1 register
    ItStatus2 = SI4432_ReadReg(0x04);                                           //read the Interrupt Status2 register
    /*enable receiver chain again*/
    SI4432_WriteReg(0x07, 0x05);

    //reset the RX FIFO
    SI4432_WriteReg(0x08, 0x02);                                                    //write 0x02 to the Operating Function Control 2 register
    SI4432_WriteReg(0x08, 0x00);                                                    //write 0x00 to the Operating Function Control 2 register
                                                                                    //enable the receiver chain again
    SI4432_WriteReg(0x07, 0x05);


}


void Enable_4432EXIT(void)
{   //GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line13);
}

void Disable_4432EXIT(void)
{  //  GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line13);
}
