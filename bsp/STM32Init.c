#include "includes.h"
u16 CCR1_Val = 500;
u16 CCR2_Val = 375;
u16 CCR3_Val = 250;
u16 CCR4_Val = 125;
ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;
u8 Speaker_StringSerial[];
/********************************************************************************
 * FunctionName: STM32_Init
 *
 * Description : STM32 �����ʼ��
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void STM32_Init(void)
{
    MyRCCInit();
    MyUsartInit();
    MyExtiNvicInit();
    DMA_Configuration();
}

/********************************************************************************
 * FunctionName: MyRCCInit
 *
 * Description : RCC��ʼ��
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void MyRCCInit(void)
{
#if 0
 /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }

#endif



    #if 0
    /*����ö�����ͱ���HSEStartUpStatus*/
    ErrorStatus HSEStartUpStatus;
    RCC_DeInit();                                               /*��λϵͳʱ������*/
    RCC_HSEConfig(RCC_HSE_ON);                                  /*����HSE*/
    HSEStartUpStatus = RCC_WaitForHSEStartUp();                 /*�ȴ�HSE�����ȶ�*/
    /*�ж�HSE�Ƿ�����ɹ����������if*/
    if(HSEStartUpStatus == SUCCESS)
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);                        /*ѡ��HCLK(AHB)ʱ��ԴΪSYSCLK 1��Ƶ*/
        RCC_PCLK2Config(RCC_HCLK_Div1);                         /*ѡ��PCLK2ʱ��ԴΪHCLK(AHB) 1��Ƶ*/
        RCC_PCLK1Config(RCC_HCLK_Div2);                         /*ѡ��PCLK1ʱ��ԴΪHCLK(AHB) 2��Ƶ*/
        FLASH_SetLatency(FLASH_Latency_2);                      /*����Flash��ʱ������Ϊ2*/
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   /*ʹ��FlashԤȡ����*/
        /*ѡ��PLLʱ��ԴΪHSE 1��Ƶ����Ƶ��Ϊ9����PLL=8MHz*9=72MHz*/
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);                                     /*ʹ��PLL*/
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);     /*�ȴ�PLL����ȶ�*/
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);              /*ѡ��SYSCLKʱ��ԴΪPLL*/
        while(RCC_GetSYSCLKSource() != 0x08);                   /*�ȴ�PLL��ΪSYSCLKʱ��Դ*/
    }
    /*��APB2�����ϵ�PWR��BKPʱ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
#endif
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,   ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,   ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,   ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,   ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,   ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}
/********************************************************************************
 * FunctionName: MyExtiNvicInit
 *
 * Description : �ж����ȼ���ʼ��
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void MyExtiNvicInit(void)
{
    NVIC_InitTypeDef    NVIC_InitStructure;
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //�����ж����ȼ������ֽڷ�ʽ

    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

     // USART1�ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
     // USART2�ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //�ⲿ�ж� SI4432
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;//ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ͨ���ж�ʹ��
    NVIC_Init(&NVIC_InitStructure);       //��ʼ���ж�

         //Enable DMA Channel7 Interrupt
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);

      /*Enable DMA Channel6 Interrupt */
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);

}

/********************************************************************************
 * FunctionName: MyUsartInit
 *
 * Description : USART��ʼ��
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void MyUsartInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //��������ʱ��
    //���ô���TX��Ϊ������������˿�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //���ô���RX��Ϊ��������˿�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 19200;    //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //����λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;     //��żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //ģʽ

    USART_Init(USART1, &USART_InitStructure);
    //USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  // ��������,֡��,����,У����ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // ???��????????
    //ʹ�ܴ���
    USART_Cmd(USART1, ENABLE);
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);  // ��������,֡��,����,У����ж�
    // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  // ???��????????
    USART_Cmd(USART2, ENABLE);
}
/********************************************************************************
 * FunctionName: mySPI_Init
 *
 * Description : SPI��ʼ��
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void mySPI_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_GPIOB,  ENABLE);       //ʹ��SPI2����ʱ��
    //P0/P1/P2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    /* SPI2 ���� */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      //ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                           //��ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       //8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                              //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                            //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               //�������NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;    //�����ʿ��� SYSCLK/256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      //���ݸ�λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;                                //CRC����ʽ�Ĵ�����ʼֵΪ7
    SPI_Init(SPI2, &SPI_InitStructure);
    /* ʹ��SPI2 */
    SPI_Cmd(SPI2, ENABLE);
}
/********************************************************************************
 * FunctionName: myEXIT_Init
 *
 * Description : �ⲿ�ж�����
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void myEXIT_Init(void)
{
  //  GPIO_InitTypeDef GPIO_InitStructure;
    //EXTI_InitTypeDef EXTI_InitStructure;
#if 0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;     //PB11,���ó��ⲿ�ж�ģʽNIRQ �����ж�ʱΪ��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);     //�����ж���

    EXTI_InitStructure.EXTI_Line = EXTI_Line11 ;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
#endif
}
/********************************************************************************
 * FunctionName: MyLedInit
 *
 * Description : ��ʼ�����ָʾ��GPIO��ʼ��
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void MyLedInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


  GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);


}
/********************************************************************************
 * FunctionName: SI4432_GPIOInit
 *
 * Description : SI4432 GPIO ��ʼ��
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void SI4432_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
    /*PB12 = NSEL ����*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    SI4432_NSEL_HIGH;
    /*PB10 =SI_SDN    ����*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    SI4432_SDN_LOW;
}
/*
void GetUniqueID(void)
{
      u8 i;
      u32 Device_Serial0, Device_Serial1, Device_Serial2;

      Device_Serial0 = *(u32*)(0x1FFFF7E8);
      Device_Serial1 = *(u32*)(0x1FFFF7EC);
      Device_Serial2 = *(u32*)(0x1FFFF7F0);

      if (Device_Serial0 != 0)
      {
        Speaker_StringSerial[0] = (u8)(Device_Serial0 & 0x000000FF);
        Speaker_StringSerial[1] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
        Speaker_StringSerial[2] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
        Speaker_StringSerial[3] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

        Speaker_StringSerial[4] = (u8)(Device_Serial1 & 0x000000FF);
        Speaker_StringSerial[5] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
        Speaker_StringSerial[6] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
        Speaker_StringSerial[7] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

        Speaker_StringSerial[8] = (u8)(Device_Serial2 & 0x000000FF);
        Speaker_StringSerial[9] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
        Speaker_StringSerial[10] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
        Speaker_StringSerial[11] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
      }
      for (i=0;i<12;i++)
      {

          learn_ok[i+5] = Speaker_StringSerial[i];
          learn_fail[i+5] = Speaker_StringSerial[i];
          learning[i+5] = Speaker_StringSerial[i];
          SendString("id= ");
          Outint(Speaker_StringSerial[i]);
      }
}
*/

