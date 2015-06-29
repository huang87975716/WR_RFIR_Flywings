
/******************************************************************************/
/*                        --��ɽ��̩�Ƽ����޹�˾ --                        */
/*
//  �ļ�����encrypt.c
//  ˵����  ���߼���
//  ��д��Ա��  WuWei
//  ��д���ڣ�  2013.10.25
//  ����ά����
//  ά����¼��
//
//
*/
// �����������ó��������ѧϰ�뽻��
// (c) Robotechn Corporation. All rights reserved.
/******************************************************************************/
#include "includes.h"
/******������*******/
u8 const Roll_key[16][16]={
                                    {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f},
                                    {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
                                    {0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f},
                                    {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f},
                                    {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f},
                                    {0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f},
                                    {0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f},
                                    {0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f},
                                    {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f},
                                    {0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f},
                                    {0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf},
                                    {0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf},
                                    {0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf},
                                    {0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf},
                                    {0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef},
                                    {0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff},
                            };


//�����÷�������(����ǰ)
 u8     test_charger[18]={0xaa,0x12,0x34,0x56,0x78, 0,0,0,0,0x8f,0x00,0x41,0x00, 0,0,0,0, 0};
 u8     si4432cmd_senddata[18]={0xaa,0x12,0x34,0x56,0x78, 0,0,0,0  ,0,0,0,0, 0,0,0,0,  0};

//��Flash��ȡ��ID
 //u8 adress_buf[4];

//������ͬ����
uint64_t Master_TBDATA =0x12345678;
//�����ù��������� ��Ӧ������0x66

//����������
u8   test_roll_list;
//������ɵĹ�������
//u8   suiji_roll_list;
//���ID����
u8   checkID_data[4];
//���ܺ�����
u8   recieve_data[18];
//���ܺ�����
u8   send_data[18];
//���������ֽڳ���
u8 Send_Legth=0;



//��־λ
u8 Learn_4432_Cmd=0;//4432ѧϰID��־λ
/********************************************************************************
 * FunctionName: Data_Encrypt
 *
 * Description : ���ݼ���
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/
void Data_Encrypt(u8 *data_buf,u8 data_length)
{  u8 roll_row,roll_col,roll_key,i,Encrypt_legth,j=0;//data_qty=0,
   u32 my_roll_list;
       Encrypt_legth= data_length;// sizeof(need_charger)/sizeof(need_charger[0]);                            //���������ֽ� �� ���ݳ�������Ӧ
       my_roll_list=test_roll_list;//ȡֵ������
       Empty_Array(send_data);      //����ϴη�������
      roll_col=my_roll_list&0x0F;                              //�к�
      roll_row=my_roll_list>>4;                                //�к�
      roll_key= Roll_key[roll_row][ roll_col];         //���ɹ�����

      send_data[j]= (*(data_buf+j))^0X55;j++;               //HEAD

      send_data[j]= (*(data_buf+j))^0x55;j++;              //����ͬ����
      send_data[j]= (*(data_buf+j))^0x55;j++;
      send_data[j]= (*(data_buf+j))^0x55;j++;
      send_data[j]= (*(data_buf+j))^0x55;j++;

      send_data[j]= (*(data_buf+j))^roll_key ;j++;          //���ܴӻ�ID
      send_data[j]= (*(data_buf+j))^roll_key ;j++;
      send_data[j]= (*(data_buf+j))^roll_key ;j++;
      send_data[j]= (*(data_buf+j))^roll_key ;j++;


      send_data[j]=(*(data_buf+j))^roll_key;  j++;                         //���ݹ���������������   buf[1]
      send_data[j]=(*(data_buf+j))^roll_key;  j++;                         //���ݹ��������ο���Ϣ   buf[1]
      send_data[j]=(*(data_buf+j))^roll_key;  j++;                         //���ݹ�����������ֵ     buf[1]
      send_data[j]=(*(data_buf+j))^roll_key;                               //���ݹ����������ݳ���   buf[1]

       for (i = 1; i <= (*(data_buf + j)); i++)
        {
            send_data[i + j] = (*(data_buf + i + j)) ^ roll_key;    //��������
        }

     send_data[data_length-1]=my_roll_list^(*(data_buf+1))^(*(data_buf+2))^(*(data_buf+3))^(*(data_buf+4));//���ܹ���������

     Send_Legth=Encrypt_legth;

}





/********************************************************************************
 * FunctionName: Data_Encrypt
 *
 * Description : �ӵ�һ֡���ݽ��н���
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/
FlagCrypt falgrx;
void Data_Decrypt(u8 *buf ,u8 data_legth)
{
u32 Recieve_TBDATA0, Recieve_TBDATA1 ,Recieve_TBDATA2,Recieve_TBDATA3  ;//Recieve_TBDATA=0,
 u8 i;//,data_qty=0,j=0;
 u8 redata_length,roll_row,roll_col, roll_key,roll_list;
 // data_length= strlen(buf);                                                     //���������ֽ� �� ���ݳ�������Ӧ
 // data_length= sizeof( SI4432_RxBUFF)/sizeof( SI4432_RxBUFF[0]);
     redata_length=data_legth;//�������������ֽ�

    // falgrx.Decryptok=0;  //δ����
             //�ж�����ͷ�����ݳ���
             if((*(buf+0))^0X55==0xaa)//&& redata_length==23)
            {
                //��ս�������
                 Empty_Array(recieve_data);
               //��һ��:����ͬ����
                Recieve_TBDATA0   =(*(buf+1))^0X55;
                Recieve_TBDATA1   =(*(buf+2))^0X55;
                Recieve_TBDATA2   =(*(buf+3))^0X55;
                Recieve_TBDATA3   =(*(buf+4))^0X55;

#ifdef Tongbu_Test    //�˶�ͬ����  ��δʹ��
               Recieve_TBDATA=Recieve_TBDATA|(Recieve_TBDATA0<<24);
               Recieve_TBDATA=Recieve_TBDATA|(Recieve_TBDATA1<<16);
               Recieve_TBDATA=Recieve_TBDATA|(Recieve_TBDATA2<<8);
               Recieve_TBDATA=Recieve_TBDATA|(Recieve_TBDATA3);

              if((Recieve_TBDATA<=Master_TBDATA) || ((Recieve_TBDATA-Master_TBDATA)>100)                       // 0<������ܵ�ͬ���룭�ϴμ�¼��ͬ����<100
              return;//��������
#endif

                //head
                recieve_data[0]=(*(buf+0))^0X55;
                //ͬ����
                recieve_data[1]=Recieve_TBDATA0;
                recieve_data[2]=Recieve_TBDATA1;
                recieve_data[3]=Recieve_TBDATA2;
                recieve_data[4]=Recieve_TBDATA3;
                //�ڶ���:�������������=��ȡ���յ�֡���ĵ����һ���ֽ�^ͬ����  ��4ΪΪ�к�,����λΪ�к�   exp:  0xa5    Roll_key[10][5]=0x5a

                roll_list=(*(buf + (redata_length-1)))^Recieve_TBDATA0^Recieve_TBDATA1^Recieve_TBDATA2^Recieve_TBDATA3;
                roll_col=roll_list&0x0F;                     //�к�
                roll_row=roll_list>>4;                       //�к�
                roll_key= Roll_key[roll_row][ roll_col];     //���ɹ�����
                //������:���ݹ���������ID  Ŀǰ4λ
             //   for(i=i;i<=ID_Legth;i++)
              //  {
              //  recieve_data[i+j]=(*(buf+i+j))^roll_key;
              //  }
                recieve_data[5]=(*(buf+5))^roll_key;
                recieve_data[6]=(*(buf+6))^roll_key;
                recieve_data[7]=(*(buf+7))^roll_key;
                recieve_data[8]=(*(buf+8))^roll_key;
               //���Ĳ�:���ݹ������������
                 recieve_data[9]=(*(buf+9))^roll_key;                                //���ݹ����������� ����
                 recieve_data[10]=(*(buf+10))^roll_key;                                //���ݹ��������ο���Ϣ   buf[1]
                 recieve_data[11]=(*(buf+11))^roll_key;                            //���ݹ�����������ֵ   buf[1]
                 recieve_data[12]=(*(buf+12))^roll_key;                              //���ݹ����������ݳ���   buf[1]

                 if(recieve_data[12]<5)                                                 // ���ݳ���С�ڵ���4�ֽ�
                 {
                   for (i = 1; i <= recieve_data[12]; i++)
                    {
                        recieve_data[i + 12] = (*(buf + i + 12)) ^ roll_key;    //��������
                    }
                   //   falgrx.Decryptok=1;  //����ɹ�
                 }

                 recieve_data[redata_length-1]=roll_list;     //�������з����������һλ  //

#if 0  // ��ӡ���ܺ�����
            //     outstring("recieve_data=");
                         for(i=0;i<redata_length;i++)//
                    {
                        SendChar( recieve_data[i]);
                        //outstring(";");
                    }
#endif


            }
}

/********************************************************************************
 * FunctionName:Empty_Array
 *
 * Description :���һ������
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/

void Empty_Array(u8 *buf)
{
    u8 i,data_length;
    data_length= sizeof( buf)/sizeof( buf[0]);
    for(i=0;i<data_length;i++)
    *(buf+i)=0;
}

/********************************************************************************
 * FunctionName:Check_Id
 *
 * Description :ID���
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/
u8 Check_Id(void)
{
        u8 i;
            for (i=0;i<4;i++)                //����ID 4�ֽ�
        {
            checkID_data[i]=recieve_data[i+5];
        }

           if(!memcmp (MY_ID,checkID_data,4))       //�Ƚ�2����ǰ12λ  ��ͬ����0
        {
           // SendString(" MY_ID");
            return 1;
        }

        Empty_Array(checkID_data); //���
           return 0;

}

/********************************************************************************
 * FunctionName:    Set4432buf
 *
 * Description : ���÷�������BUF
 *
 * Parameters  :
 *
 *
 * Returns     : None.
 *******************************************************************************/
/*

void Send4432buf(u8 direction, u8 cmd)
{

test_send_data_buf[0]=0xaa;

if(direction
test_send_data_buf[0]=0xaa;

}

*/


/********************************************************************************
 * FunctionName: SI4432_Protocol
 *
 * Description : 4432Э�����
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/

SI4432PROTOCOL_t *SI4432p = &SI4432RecvBuff[0];  // ָʾ�����ַ
u8 si4432_buff1[3];
u8 si4432_buff2[3];
u8 si4432_buff3[3];
void SI4432_Protocol(u8 *receive_buf )
{

static u8 si4432_cnt=0;
 u8 i,HEAD,CMD;


   //  id_check=Check_Id();//ID����

         if( Check_Id()==0x01) //�Ƿ�������
     {
                     for(i=0;i<ID_Legth;i++)                   //4�ֽ�ID
                     {
                     SI4432p->rfid[i] = *(receive_buf + i+5);
                     }
                      SI4432p->rfdirection=*(receive_buf+9);//recieve_data[17];   //����
                      SI4432p->rfmsgref=*(receive_buf+10);//recieve_data[18];    //�ο���Ϣ
                      SI4432p->rfcommand=*(receive_buf+11);//recieve_data[19];  //����ֵ
                      SI4432p->rflength=*(receive_buf+12);//recieve_data[20];        //���ݳ���
                          if(SI4432p->rflength<5)          //С��5�ֽ�
                      {
                              for(i=1;i<=(SI4432p->rflength);i++)              //���ݽ���
                              {
                                  SI4432p->rfdata[i-1]=*(receive_buf+12+i);//recieve_data[20+i];
                              }
                      }
                      switch( SI4432p->rfdirection)   //�жϷ��򣬽���Э�����
                      {
                      case 0x28:       //�����ܼҾ�ת����ָ��
                                       if (SI4432p->rfcommand==0x32)//���ܼҾ�ת����ָ��
                                       {
                                         //  if(SI4432p->rfdata[0]==0x01)//��������ѧϰ
                                              learn_DeviceName =SI4432p->rfdata[0];     //��ȡ�豸����  0x00-0x0a �յ�  0x0b-0x14  ����
                                              learn_KeyCode    =SI4432p->rfdata[1];     //��ȡҪѧϰ�ļ�ֵ  �յ�0-10   ����0-30
                 //                             HEAD=0xaa;
                                              CMD=0x32;


                                              if(si4432_cnt==0)
                                              {
                                              si4432_buff1[0]=CMD;
                                              si4432_buff1[1]=learn_DeviceName;
                                              si4432_buff1[2]=learn_KeyCode;
                                                 OSQPost(ProtoSendMBox,&si4432_buff1);
                                                 si4432_cnt++;
                                              }
                                              else   if(si4432_cnt==1)
                                              {
                                              si4432_buff2[0]=CMD;
                                              si4432_buff2[1]=learn_DeviceName;
                                              si4432_buff2[2]=learn_KeyCode;
                                                 OSQPost(ProtoSendMBox,&si4432_buff2);
                                                  si4432_cnt++;
                                              }
                                              else   if(si4432_cnt==2)
                                              {
                                              si4432_buff3[0]=CMD;
                                              si4432_buff3[1]=learn_DeviceName;
                                              si4432_buff3[2]=learn_KeyCode;
                                                 OSQPost(ProtoSendMBox,&si4432_buff3);
                                                  si4432_cnt=0;
                                              }
                                       }

                                       else if(SI4432p->rfcommand==0x33)//�����ܼҾ�ת����ѧϰָ��
                                       {
                                           if(SI4432p->rfdata[0]==0x01)//��������ѧϰ
                                           {
                                                /*�յ�   ���������10    ÿ���յ���ѧϰ�������10
                                                  ����  ������� ��10    ÿ�����ӿ�ѧϰ����:30
                                                */
                                                learn_cmd=1;//����ѧϰ
                                              // LED_Learn(1); //����ѧϰLED��
                                               learn_switch = SI4432p->rfdata[0]; //ѧϰ����״̬
                                               learn_DeviceName =SI4432p->rfdata[1];     //��ȡ�豸����  0x00-0x0a �յ�  0x0b-0x14  ����
                                               learn_KeyCode    =SI4432p->rfdata[2];     //��ȡҪѧϰ�ļ�ֵ  �յ�0-10   ����0-30
                                                CMD=0x33;

                                              if(si4432_cnt==0)
                                              {
                                              si4432_buff1[0]=CMD;
                                              si4432_buff1[1]=learn_DeviceName;
                                              si4432_buff1[2]=learn_KeyCode;
                                              OSQPost(ProtoSendMBox,&si4432_buff1);
                                              si4432_cnt++;
                                              }
                                              else   if(si4432_cnt==1)
                                              {
                                              si4432_buff2[0]=CMD;
                                              si4432_buff2[1]=learn_DeviceName;
                                              si4432_buff2[2]=learn_KeyCode;
                                              OSQPost(ProtoSendMBox,&si4432_buff2);
                                              si4432_cnt++;
                                              }
                                              else   if(si4432_cnt==2)
                                              {
                                              si4432_buff3[0]=CMD;
                                              si4432_buff3[1]=learn_DeviceName;
                                              si4432_buff3[2]=learn_KeyCode;
                                              OSQPost(ProtoSendMBox,&si4432_buff3);
                                              si4432_cnt=0;
                                              }


                                           }
                                           else if(SI4432p->rfdata[0]==0x02)//�رպ���ѧϰ
                                           {
                                                learn_cmd=0;
                                                Timout_Count=TIMOUT_20s;//����������ֵ
                                                //send_sig=0;  //
                                                SendString("�رտ�ʼѧϰ��");
                                           }

                                       }
                                        break;
                      case 0xf8:        //�����ź�
                                        // SendString("test");
                                        if(SI4432p->rfcommand==0x40)
                                        {
                                           DelayMs(200);
                                          SendString("test");
                                          for(i=0;i<4;i++)  //����ID
                                          {
                                              test_charger[5+i] = MY_ID[i];
                                          }
                                          Data_Encrypt(test_charger,18);   //???????��????????
                                          sending(send_data,Send_Legth);
                                        }

                                       // SendFallingSignal(USART1,SI4432p->rfdata[0],SI4432p->rfid);
                                        break;
          //            case 0x29:        //�����������ź�   ת����8126
                                     //   SendString("kongqi ");
                                     //   break;

                      default:
                                        break;
                      }

     }
}


#if 0

/********************************************************************************
 * FunctionName: SI4432_ID_Learn
 *
 * Description : 4432 �ӻ�IDѧϰ���� ��flash��
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/
void SI4432_ID_Learn(uint32_t save_addr, u8 *receive_buf,uint32_t save_size)
{

          Flash_Write(save_addr,receive_buf,save_size);//��ȡbuf����FLASH
//    Flash_Read(CHARGER_ID_ADDR,adress_buf,12);

}



/********************************************************************************
 * FunctionName: SI4432_ID_Pickup
 *
 * Description : ��ָ��FLASH��ַ��ȡID
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/
void SI4432_ID_Pickup(uint32_t save_addr, u8 *receive_buf,uint32_t save_size)
{
//  Flash_Write(save_addr,receive_buf,save_size);
      Flash_Read(save_addr,receive_buf,save_size);    //��ȡFALSH ID����BUF


}

#endif

