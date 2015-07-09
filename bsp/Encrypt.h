
#ifndef __encrypt_H
#define __encrypt_H

// #define  Tongbu_Test
#if !defined  Tongbu_Test

/**
 * @brief Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will
   be based on direct access to peripherals registers
   */
 // #define Tongbu_Test
#endif
//extern u8 test_buf[];


 //�����ź� ״̬ 1��ѧϰ�ɹ� 2��ѧϰʧ�� 3������ѧϰ��

#define  Stu_Success 0x01
#define  Stu_Fail    0x02
#define  Stu_On 0x03

#define   Charger_ID Charger_ID1

//��������ID����
#define ID_Legth 4

//���屣��ID����
//#define SlaveID_Qty 4

//���屣�����ݳ���
//#define  SI4432_ID_SIZE   12


//�����ڲ�FLASH�������ݵ�ַ
#define  RFIR_ID_ADDR       0x08008400    //ID�洢��ַ  4�ֽ�    ����ռ䣺0x08000000-0x080078ff        �洢�ռ䣺0x08007900-0x0800ffff   33K�ֽ�
#define  DEVICE_REC_ADDR    RFIR_ID_ADDR+4    //��¼�ѱ�����豸 2�ֽ�   ��1�ֽڼ�¼��ѧϰ�Ŀյ��豸����  ��2�ֽڼ�¼��ѧϰ�ĵ����豸����

#define  ARC1_ADDR   DEVICE_REC_ADDR+4  //�յ�1��ʼ��ַ ÿ���յ���ַ 2080�ֽڿɴ� 10����ֵ ��10*208��  7908
#define  ARC2_ADDR   ARC1_ADDR+2080     //�յ�2��ʼ��ַ
#define  ARC3_ADDR   ARC2_ADDR+2080     //�յ�3��ʼ��ַ
#define  ARC4_ADDR   ARC3_ADDR+2080     //�յ�4��ʼ��ַ
#define  ARC5_ADDR   ARC4_ADDR+2080     //�յ�5��ʼ��ַ
#define  ARC6_ADDR   ARC5_ADDR+2080     //�յ�6��ʼ��ַ
#define  ARC7_ADDR   ARC6_ADDR+2080     //�յ�7��ʼ��ַ
#define  ARC8_ADDR   ARC7_ADDR+2080     //�յ�8��ʼ��ַ
#define  ARC9_ADDR   ARC8_ADDR+2080     //�յ�9��ʼ��ַ
#define  ARC10_ADDR  ARC9_ADDR+2080    //�յ�10��ʼ��ַ  c228

#define  URC1_ADDR   ARC10_ADDR+2080  //����1��ʼ��ַ  ÿ�����ӵ�ַ 6240�ֽڿɴ� 30����ֵ ��30*208��   ca48   // da88
#define  URC2_ADDR   URC1_ADDR+6240   //����2��ʼ��ַ
#define  URC3_ADDR   URC2_ADDR+6240   //����3��ʼ��ַ
#define  URC4_ADDR   URC3_ADDR+6240   //����4��ʼ��ַ
#define  URC5_ADDR   URC4_ADDR+6240   //����4��ʼ��ַ
//#define  CHARGER_ID_ADDR   0x08012000

#define  ARC_ADDR ARC1_ADDR+(learn_DeviceName-1)*2080+(learn_KeyCode-1)*208   //��10���豸 ��ÿ���յ��豸ռ��2080���ֽ� ���յ���ռ�� 20800�ֽ�
#define  URC_ADDR URC1_ADDR+(learn_DeviceName-11)*6240+(learn_KeyCode-1)*208  //��5���豸 ��ÿ�������豸ռ��6240���ֽ� �����ӹ�ռ�� 31200�ֽ�

//ID
#define Send_4432ID0      si4432cmd_senddata[5]
#define Send_4432ID1      si4432cmd_senddata[6]
#define Send_4432ID2      si4432cmd_senddata[7]
#define Send_4432ID3      si4432cmd_senddata[8]
//CMD
#define Send_4432DIR      si4432cmd_senddata[9]
#define Send_4432CMD      si4432cmd_senddata[11]
#define Send_4432LENGTH   si4432cmd_senddata[12]
//data
#define Send_4432DATA0    si4432cmd_senddata[13]
#define Send_4432DATA1    si4432cmd_senddata[14]
#define Send_4432DATA2    si4432cmd_senddata[15]
#define Send_4432DATA3    si4432cmd_senddata[16]


#define   TIMOUT_20s  320//160


/***********************************************************
* ��    �ƣ�typedef struct
* ��    �ܣ��ṹ���־λ�����ڽ��ܱ�־
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
typedef struct
{
    u32  Decryptok            : 1;   //����ʶ���־λ
}   FlagCrypt;



extern u16 Timout_Count;
extern u8 learn_switch;
extern u8 learn_KeyCode;
extern u8 learn_DeviceName;
extern  u8 learn_cmd;
extern  u8 Learn_State;
//extern u8 len_ok;
//extern u8 send_sig;
extern unsigned char ct361_Buffer[];//��CT361���仺��



extern FlagCrypt falgrx;
extern u8 si4432cmd_senddata[];
extern u8 MY_ID[];
extern u8 send_data[];
extern u8 recieve_data[];
extern u8 Learn_4432_Cmd;//4432ѧϰ��־λ
//extern u8 test_send_data_buf[];
extern u8 Send_Legth;
extern u8 test_roll_list;
//extern u8 suiji_roll_list;
void Data_Decrypt(u8 *buf ,u8 data_legth) ;
void Data_Encrypt(u8 *data_buf,u8 data_legth);
void SI4432_Protocol(u8 *receive_buf );
//void SI4432_ID_Learn(uint32_t save_addr, u8 *receive_buf,uint32_t save_size);
//void SI4432_ID_Pickup(uint32_t save_addr, u8 *receive_buf,uint32_t save_size);
//u8  Check_Id(void);
void Empty_Array(u8 *buf);
#endif


