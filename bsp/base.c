#include "base.h"
#include "si4432.h"

//uint16_t TimingDelay;
FlagType Flag;

/********************************************************************************
 * FunctionName: SendChar
 *
 * Description : �������һ���ַ�
 *
 * Parameters  : signed char ch[]>Ҫ��ʾ���ַ�
 *
 * Returns     : ��
 *******************************************************************************/
void SendChar(u8 cha)
{
    #if 1
    USART_SendData(USART1,cha);
     delay_10us(1);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {}
 #endif
}

/********************************************************************************
 * FunctionName: SendString
 *
 * Description : �����һ���ַ���
 *
 * Parameters  : signed char ch[]>Ҫ��ʾ���ַ���
 *
 * Returns     : ��
 *******************************************************************************/
void SendString(u8 ch[])
{
    #if 1
    int i=0;
    while(ch[i]!='\0')
    {
        SendChar(ch[i]);
        i++;
    }
     #endif
}

/********************************************************************************
 * FunctionName: SendChar
 *
 * Description : �������һ���ַ�
 *
 * Parameters  : signed char ch[]>Ҫ��ʾ���ַ�
 *
 * Returns     : ��
 *******************************************************************************/
void Send2Char(u8 cha)
{
    USART_SendData(USART2,cha);
     delay_10us(30);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {}
}

/********************************************************************************
 * FunctionName: SendString
 *
 * Description : �����һ���ַ���
 *
 * Parameters  : signed char ch[]>Ҫ��ʾ���ַ���
 *
 * Returns     : ��
 *******************************************************************************/
void Send2String(u8 ch[])
{
    int i=0;
    while(ch[i]!='\0')
    {
        Send2Char(ch[i]);
        i++;

    }
}


/********************************************************************************
 * FunctionName: Outint
 *
 * Description : ת��Ϊ10�������
 *
 * Parameters  : Ҫ���������
 *
 * Returns     : ��
 *******************************************************************************/
void Outint(long i)
{
     int a;
     char b[10];
     u8 k=0;
     if(i==0)
            SendChar('0');
     else
     {
         while(i)
         {
             b[k]=i%10;
             k++;
             i=i/10;
         }
    for(a=k-1;a>=0;a--)
       {
        switch (b[a])
        {
            case 0:SendChar('0');break;
            case 1:SendChar('1');break;
            case 2:SendChar('2');break;
            case 3:SendChar('3');break;
            case 4:SendChar('4');break;
            case 5:SendChar('5');break;
            case 6:SendChar('6');break;
            case 7:SendChar('7');break;
            case 8:SendChar('8');break;
            case 9:SendChar('9');break;
        }
       }
    }
}

/********************************************************************************
 * FunctionName: delay_10us
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void delay_10us(u16 nTime)  //25us
{
    int i, j;
    for (i = 0; i < nTime; i++)
        for (j = 0; j < 72; j++) ;
}

/********************************************************************************
 * FunctionName: DelayMs
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void DelayMs(u16 nTime)
{
    int i;
    for (i = 0; i < nTime; i++)
        delay_10us(100);
}


