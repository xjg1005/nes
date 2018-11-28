#include "config.h"

#include "iic.h"

#define iic_pnd             (IIC_CON & BIT(15))
#define clr_iic_pnd         IIC_CON |= BIT(13)//�����ͨpending
#define ack_in              (IIC_CON & BIT(11))
#define pu_ack_out          IIC_CON |= BIT(10)
#define pd_ack_out          IIC_CON &= ~BIT(10)
#define clr_start           IIC_CON |= BIT(8)//�����ʼλ��־

#define IIC_DELAY_COUNT  100

#define clr_stop            IIC_CON |= BIT(12)//��ս���λ��־
#define add_sbit            IIC_CON |= BIT(2)//����ʼλ
#define add_ebit            IIC_CON |= BIT(3)//�ӽ���λ





void iic_tx (u8 dat);
void  iic_ini (void)
{
#if 1
    IOMC1 &= ~(BIT(7)|BIT(6));
    IOMC1 |= BIT(6);
    //00:PD4/PD5 01:PB12/PB14

    PORTB_DIR &= ~BIT(12);//CLK
    PORTB_PU |= (BIT(12) | BIT(14));

    PORTB_HD |= (BIT(12) | BIT(14));
#else

    IOMC1 &= ~(BIT(7)|BIT(6));
    //00:PD4/PD5 01:PB12/PB14

    PORTD_DIR &= ~BIT(4);//CLK
    PORTD_PU |= (BIT(4) | BIT(5));

#endif

 //   IIC_BAUD = 0x20; //old
    IIC_BAUD = 0x40; //����������ػ���ʱ����ģ�� 5203 JOHNSON 20160513
    IIC_CON  = 1;

}

void iic_tx (u8 dat)
{
   // printf("send %x\r\n",dat) ;
 //   printf("%02x ",dat) ;
    pu_ack_out;
    IIC_BUF = dat;
    while(!iic_pnd);
    clr_iic_pnd;
}

void iic_tx_ws (u8 dat)
{
    pu_ack_out;
    //IIC_CON |= BIT(2);
    add_sbit;

    IIC_BUF = dat;
    while(!iic_pnd);
    clr_iic_pnd;
    IIC_CON &= ~BIT(2);

}

void iic_tx_we (u8 dat)
{
  //  printf("%02x ",dat) ;
    pu_ack_out;
    //IIC_CON |= BIT(3);
    add_ebit;

    IIC_BUF = dat;
    while(!iic_pnd);
    clr_iic_pnd;
    clr_start;
   // add_sbit;
    IIC_CON &= ~BIT(3);
}

u8 iic_rx (void)
{
    u8 res;

    //pd_ack_out;
    IIC_BUF = 0xff;
    while(!iic_pnd);
    clr_iic_pnd;
    res = IIC_BUF;
    //pu_ack_out;
    return(res);
}

u8 iic_rx_ws (void)
{
    u8 res;

    //pd_ack_out;
    //IIC_CON |= BIT(2);
    add_sbit;

    IIC_BUF = 0xff;
    while(!iic_pnd);
    clr_iic_pnd;
    IIC_CON &= ~BIT(2);
    res = IIC_BUF;
    //pu_ack_out;
    return(res);
}

u8 iic_rx_we (void)
{
    u8 res;

    //pd_ack_out;
    //IIC_CON |= BIT(3);
    add_ebit;

    IIC_BUF = 0xff;
    while(!iic_pnd);
    clr_iic_pnd;
    clr_start;
    IIC_CON &= ~BIT(3);
    res = IIC_BUF;
    //pu_ack_out;
    return(res);
}

#define IIC_SET_ACK()               (IIC_CON &= ~BIT(10))
void   iic_write_dma(u8 *buf, u16 len)
{

    IIC_SET_ACK();
    IIC_DMA_ADR = (u32)buf;
    IIC_DMA_CNT = len;

}



/**********  ���������IIC���������� ***********/

void startSCCB_A(void)
{
    SCCB_SID_H_A();     //�����߸ߵ�ƽ
    delay(IIC_DELAY_COUNT);

    SCCB_SIC_H_A();	   //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay(IIC_DELAY_COUNT);

    SCCB_SID_L_A();
    delay(IIC_DELAY_COUNT);

    SCCB_SIC_L_A();	 //�����߻ָ��͵�ƽ��������������Ҫ
    delay(IIC_DELAY_COUNT);
}

void stopSCCB_A(void)
{
    SCCB_SIC_L_A();
    delay(IIC_DELAY_COUNT);

    SCCB_SID_L_A();
    delay(IIC_DELAY_COUNT);

    SCCB_SIC_H_A();
    delay(IIC_DELAY_COUNT);

    SCCB_SID_H_A();
    delay(IIC_DELAY_COUNT);
}

u8 Wait_Ack_A(void)
{
    u8 ucErrTime=0;

	SCCB_SID_IN_A; delay(IIC_DELAY_COUNT);//SDA����Ϊ����
	SET_SID_H_A();delay(IIC_DELAY_COUNT);
	SCCB_SIC_H_A();delay(IIC_DELAY_COUNT);
	while(SCCB_SID_STATE_A)
	{
		ucErrTime++;
		if(ucErrTime> 250)
		{
			stopSCCB_A();
			return 1;
		}
	}
	SCCB_SIC_L_A();//ʱ�����0
	delay(IIC_DELAY_COUNT);
	return 0;

}

void Ack_A(void)
{
    SCCB_SIC_L_A();
	delay(IIC_DELAY_COUNT);

	SCCB_SID_L_A();
    delay(IIC_DELAY_COUNT);

    SCCB_SIC_H_A();
	delay(IIC_DELAY_COUNT);

	SCCB_SIC_L_A();
	delay(IIC_DELAY_COUNT);

}

void noAck_A(void)
{
	SCCB_SIC_L_A();
	delay(IIC_DELAY_COUNT);

	SCCB_SID_H_A();
	delay(IIC_DELAY_COUNT);

	SCCB_SIC_H_A();
	delay(IIC_DELAY_COUNT);

	SCCB_SIC_L_A();
	delay(IIC_DELAY_COUNT);
}

unsigned char SCCBwriteByte_A(unsigned char m_data)
{
	unsigned char j,tem;

	SCCB_SIC_L_A();
    delay(IIC_DELAY_COUNT);

	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if((m_data<<j)&0x80)
		{
			SCCB_SID_H_A();
		}
		else
		{
			SCCB_SID_L_A();
		}
		delay(IIC_DELAY_COUNT);
		SCCB_SIC_H_A();
		delay(IIC_DELAY_COUNT);
		SCCB_SIC_L_A();
		delay(IIC_DELAY_COUNT);

	}
	delay(IIC_DELAY_COUNT);

//	SCCB_SID_IN_A;/*����SDAΪ����*/
//	delay(500);
//	SCCB_SIC_H_A();
//	delay(1000);
//	if(SCCB_SID_STATE_A){tem=0;}   //SDA=1����ʧ�ܣ�����0}
//	else {tem=1;}   //SDA=0���ͳɹ�������1
//	SCCB_SIC_L_A();
//	delay(500);

    tem = !Wait_Ack_A();

        SCCB_SID_OUT_A;/*����SDAΪ���*/

	return (tem);
}

unsigned char SCCBreadByte_A(void)
{
	unsigned char read,j;
	read=0x00;

	SCCB_SID_IN_A;/*����SDAΪ����*/
	SCCB_SID_PU_A;//����
	delay(IIC_DELAY_COUNT);
	for(j=8;j>0;j--) //ѭ��8�ν�������
	{
		SCCB_SIC_L_A();
		delay(IIC_DELAY_COUNT);
		SCCB_SIC_H_A();
		delay(IIC_DELAY_COUNT);
		read=read<<1;
		if(SCCB_SID_STATE_A)
		{
			read=read+1;
		}
		delay(IIC_DELAY_COUNT);
	}

	return(read);
}

static void saa7113_delay(unsigned int t)
{

    while ( t--)
    {
       asm volatile (" nop  ;");
       //asm volatile (" nop  ;");
    }
}

//�ڶ����ӿڵ����IIC����
void startSCCB_B(void)
{
    SCCB_SID_H_B();     //�����߸ߵ�ƽ
    saa7113_delay(IIC_DELAY_COUNT);

    SCCB_SIC_H_B();	   //��ʱ���߸ߵ�ʱ���������ɸ�����
    saa7113_delay(IIC_DELAY_COUNT);

    SCCB_SID_L_B();
    saa7113_delay(IIC_DELAY_COUNT);

    SCCB_SIC_L_B();	 //�����߻ָ��͵�ƽ��������������Ҫ
    saa7113_delay(IIC_DELAY_COUNT);
}

void stopSCCB_B(void)
{
    SCCB_SIC_L_B();
    saa7113_delay(IIC_DELAY_COUNT);

    SCCB_SID_L_B();
    saa7113_delay(IIC_DELAY_COUNT);

    SCCB_SIC_H_B();
    saa7113_delay(IIC_DELAY_COUNT);

    SCCB_SID_H_B();
    saa7113_delay(IIC_DELAY_COUNT);
}

u8 Wait_Ack_B(void)
{
    u8 ucErrTime=0;

	SCCB_SID_IN_B; saa7113_delay(IIC_DELAY_COUNT);//SDA����Ϊ����
	SET_SID_H_B();saa7113_delay(IIC_DELAY_COUNT);
	SCCB_SIC_H_B();saa7113_delay(IIC_DELAY_COUNT);
	while(SCCB_SID_STATE_B)
	{
		ucErrTime++;
		if(ucErrTime> 250)
		{
			stopSCCB_B();
			return 1;
		}
	}
	SCCB_SIC_L_B();//ʱ�����0
	saa7113_delay(IIC_DELAY_COUNT);
	return 0;

}

void Ack_B(void)
{
    SCCB_SIC_L_B();
	saa7113_delay(IIC_DELAY_COUNT);

	SCCB_SID_L_B();
    saa7113_delay(IIC_DELAY_COUNT);

    SCCB_SIC_H_B();
	saa7113_delay(IIC_DELAY_COUNT);

	SCCB_SIC_L_B();
	saa7113_delay(IIC_DELAY_COUNT);

}

void noAck_B(void)
{
	SCCB_SIC_L_B();
	saa7113_delay(IIC_DELAY_COUNT);

	SCCB_SID_H_B();
	saa7113_delay(IIC_DELAY_COUNT);

	SCCB_SIC_H_B();
	saa7113_delay(IIC_DELAY_COUNT);

	SCCB_SIC_L_B();
	saa7113_delay(IIC_DELAY_COUNT);
}

unsigned char SCCBwriteByte_B(unsigned char m_data)
{
	unsigned char j,tem;

	SCCB_SIC_L_B();
    saa7113_delay(IIC_DELAY_COUNT);

	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if((m_data<<j)&0x80)
		{
			SCCB_SID_H_B();
		}
		else
		{
			SCCB_SID_L_B();
		}
		saa7113_delay(IIC_DELAY_COUNT);
		SCCB_SIC_H_B();
		saa7113_delay(IIC_DELAY_COUNT);
		SCCB_SIC_L_B();
		saa7113_delay(IIC_DELAY_COUNT);

	}
	saa7113_delay(IIC_DELAY_COUNT);

    tem = !Wait_Ack_B();

    SCCB_SID_OUT_B;/*����SDAΪ���*/

	return (tem);
}

unsigned char SCCBreadByte_B(void)
{
	unsigned char read,j;
	read=0x00;

	SCCB_SID_IN_B;/*����SDAΪ����*/
	saa7113_delay(IIC_DELAY_COUNT);
	for(j=8;j>0;j--) //ѭ��8�ν�������
	{
		SCCB_SIC_L_B();
		saa7113_delay(IIC_DELAY_COUNT);
		SCCB_SIC_H_B();
		saa7113_delay(IIC_DELAY_COUNT);
		read=read<<1;
		if(SCCB_SID_STATE_B)
		{
			read=read+1;
		}
		saa7113_delay(IIC_DELAY_COUNT);
	}

	return(read);
}



////////////////////////////////////////////////////////////
const S_IIC_FUN iic_fun[2] =
{
    {
        startSCCB_A,
        stopSCCB_A,
        Wait_Ack_A,
        Ack_A,
        noAck_A,
        SCCBwriteByte_A,
        SCCBreadByte_A,
    },
    {
        startSCCB_B,
        stopSCCB_B,
        Wait_Ack_B,
        Ack_B,
        noAck_B,
        SCCBwriteByte_B,
        SCCBreadByte_B,
    },
};

s32 iic_select(S_IIC_FUN * iic, u8 sel)
{
	if (sel >= 2)
	{
		return -1;
	}

	memcpy(iic, &iic_fun[sel], sizeof(S_IIC_FUN));

	return 0;
}

