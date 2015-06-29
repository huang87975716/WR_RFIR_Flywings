/******************************************************************************
* Copyright (c) 2014, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : main.c
* Abstract    :
* Author      :
*
* History
* date          author      notes
* 2014-03-18                File Created.
*
******************************************************************************/

#include "includes.h"


void SystemInit(void)
{
}

void RCCInit(void)
{
        RCC_DeInit ();                        /* RCC system reset(for debug purpose)*/
        RCC_HSEConfig (RCC_HSE_ON);           /* Enable HSE                         */
        /* Wait till HSE is ready                                                   */
        while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
        RCC_HCLKConfig   (RCC_SYSCLK_Div1);   /* HCLK   = SYSCLK                    */
        RCC_PCLK2Config  (RCC_HCLK_Div1);     /* PCLK2  = HCLK                      */
        RCC_PCLK1Config  (RCC_HCLK_Div2);     /* PCLK1  = HCLK/2                    */
        RCC_ADCCLKConfig (RCC_PCLK2_Div4);    /* ADCCLK = PCLK2/4                   */
        *(vu32 *)0x40022000 = 0x01;           /* Flash 2 wait state                 */
        /* PLLCLK = 8MHz * 9 = 72 MHz                                               */
        RCC_PLLConfig (RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        RCC_PLLCmd (ENABLE);                  /* Enable PLL                         */
        /* Wait till PLL is ready                                                   */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        /* Select PLL as system clock source                                        */
        RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK);
        /* Wait till PLL is used as system clock source                             */
        while (RCC_GetSYSCLKSource() != 0x08);
        /* Enable USART1 and GPIOA clock                                            */
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
}

void BSP_Init(void)
{    // RCCInit();
      STM32_Init();
    /*SI4432��ʼ��*/
      SI4432_Configuration();
      MyLedInit();
      LED_Learn(0);
      LED_Send(0);
    /*��־λ��ʼ��*/
   // MyFlagInit();
      SendString("initok\n");
      WatchDog_Init(7,2000); //���Ź���ʼ��
}

void MyFlagInit(void)
{
    Flag.Si4432Rx_Status = 1;    //4432���ձ�־λ��1������ 0�����ɽ���
    Flag.Si4432Tx_Status = 1;    //4432���ͱ�־λ��1���ɷ���  0�����ɷ���
}


/********************************************************************************
 * FunctionName: MyTim3Init
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void MyTim3PWMInit(u16 Value1)
{
    TIM_OCInitTypeDef TIMOCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_InternalClockConfig(TIM3);          //8M
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );

    /*PB0 = MOD_S--38k�ز�--����*/
    //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    //TIM_TimeBaseStructure.TIM_Prescaler = 0;   //72M
    TIM_TimeBaseStructure.TIM_Period = 206;   //ԼΪ38K
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

    TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWMģʽ1���
    TIMOCInitStructure.TIM_Pulse = Value1;     //����ֵ ����CCR�Ĵ���//ռ�ձ�=(CCRx/ARR)*100%   TIM_Pulse/(TIM_Period+1)    50%:64
    TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM����Ƚϼ��Ը�
    TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//ʹ�����״̬
    //TIM_OC3Init(TIM2, &TIMOCInitStructure);//TIM2��CH2���
    TIM_OC3Init(TIM3, &TIMOCInitStructure);
    TIM_ARRPreloadConfig(TIM3,ENABLE);
    TIM_CtrlPWMOutputs(TIM3,ENABLE);//����TIM2��PWM���Ϊʹ��
    TIM_Cmd(TIM3, ENABLE); //����ʱ��
}

/********************************************************************************
 * FunctionName: MyTIM1InterruptNvicInit
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/


void MyTIM1InterruptNvicInit(void)
{
    NVIC_InitTypeDef    NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // Enable the TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


/********************************************************************************
 * FunctionName: MyTIM1CaptureInit
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void MyTIM1CaptureInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    TIM_InternalClockConfig(TIM1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /**
     * ����0-0xFFFF����Ƶ71��������Ϊ1S�����һ��ʱ��Ϊ1uS*0xFFFF=65ms
     */
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;                  // ��������
    TIM_TimeBaseInitStructure.TIM_Prescaler = 71;                   // 72MHz / (71+1) = 1000KHz = 1M -->1uS
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // ��������
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);                     // ��ֹ����ж�
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                // ͨ��1���벶��
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;     // �½��ش���
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           // ÿ�μ�⵽������һ�β���
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // TI1�����ӵ�IC1
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_PWMIConfig(TIM1, &TIM_ICInitStructure);

    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);                    // ѡ�񴥷��ź�
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);                 // ѡ�д����źŵ��½������³�ʼ���������������Ĵ�������
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);    // ʹ������ģʽ
    TIM_ITConfig(TIM1, TIM_IT_CC1, DISABLE);
    TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
    TIM_Cmd(TIM1, ENABLE);
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
}

//DMA���ã�
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  /* DMA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1
  /* DMA1 Channel7 (triggered by USART2 Tx event) Config */
  DMA_DeInit(DMA1_Channel7);
  DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40004404;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_SEND_DATA;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 512;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel7, &DMA_InitStructure);
  DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
  DMA_ITConfig(DMA1_Channel7, DMA_IT_TE, ENABLE);
  /* Enable USART2 DMA TX request */
  USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
  DMA_Cmd(DMA1_Channel7, DISABLE);



/* DMA1 Channel6 (triggered by USART1 Rx event) Config */
  DMA_DeInit(DMA1_Channel6);
  DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40004404;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_RECEIVE_DATA;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 512;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel6, &DMA_InitStructure);
  DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
  DMA_ITConfig(DMA1_Channel6, DMA_IT_TE, ENABLE);
  /* Enable USART2 DMA RX request */
  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
  DMA_Cmd(DMA1_Channel6, ENABLE);

}



/********************************************************************************
 * FunctionName: SysTickInit
 *
 * Description : ϵͳ�δ�ʱ�ӳ�ʼ��
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void SysTickInit(void)
{
    RCC_ClocksTypeDef rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
    SysTick_Config(rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC);
}

