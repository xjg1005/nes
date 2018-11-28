#include "pap.h"
#include "config.h"

#define PAP_TS      14//(2bit) ���ݽ���ʱ��
#define PAP_TH      12//(2bit) ���ݱ���ʱ��
#define PAP_TW      8 //(4bit) ��/дʹ���źſ��  ϵͳʱ�� 0:16�� 1:1�� 2:2�� ��������
#define PAP_PND     7 //�ж������־
#define PAP_CPND    6 //��λд'1'���PND�ж������־
#define PAP_DW16ED  5 //16bitģʽ���ݴ�С��ѡ��(8bitģʽ,������Ϊ0)   0:�͵�ַ�������˿ڵ�λ 1:�͵�ַ�������˿ڸ�λ
#define PAP_DW16EN  4 //0:8bitģʽ 1:16bitģʽ
#define PAP_PRE     3 //���źż���ѡ�� 0:����ʱΪ�͵�ƽ 1:����ʱΪ�ߵ�ƽ
#define PAP_PWE     2 //д�źż���ѡ�� 0:����ʱΪ�͵�ƽ 1:����ʱΪ�ߵ�ƽ
#define PAP_DIR     1 //0:��������       1:��������
#define PAP_EN      0 //PAP�ӿ�ʹ��

/*----------------------------------------------------------------------------*/
/**@brief   PAP�ӿڳ�ʼ��
   @param	��
   @return	��
   @note
*/
/*----------------------------------------------------------------------------*/
void pap_init()
{
	PAP_PORT_EN();//ʹ��PAP�ӿ�
	PAP_RD_OUT();
	PAP_WR_OUT();

#if (PAP_8BIT_MODE_EN == 1)
	//��DV12���ԣ���ʹ��8bit���ݿ��ģʽʱ����ʹ�õ�����PORT�ڵĸ�8bit������뽫DW16ED����Ϊ"1"
	PAP_CON = (2<<PAP_TS) | (2<<PAP_TH) | (5<<PAP_TW) | (1<<PAP_DW16ED) | (1<<PAP_PRE) | (1<<PAP_PWE) | (1<<PAP_EN);
#else
	PAP_CON = (2<<PAP_TS) | (2<<PAP_TH) | (5<<PAP_TW) | (PAP_16BIT_ENDIAN<<PAP_DW16ED) | (1 <<PAP_DW16EN) | (1<<PAP_PRE) | (1<<PAP_PWE) | (1<<PAP_EN);
#endif
}


/*----------------------------------------------------------------------------*/
/**@brief   дһ���������(16bit)
   @param	16λ����
   @return	��
   @note
*/
/*----------------------------------------------------------------------------*/
void pap_write16(u16 w) //9325 JOHNSON 20160526 add
{
    PAP_WAIT();
	PAP_DIR_O();
//#if (PAP_8BIT_MODE_EN==1)
	PAP_BUF = (w >> 8)&0xff;

	PAP_WAIT();
	PAP_BUF = w&0xff;

//#endif
}

void pap_write(u16 w)
{
    PAP_WAIT();
	PAP_DIR_O();
#if (PAP_8BIT_MODE_EN==1)&&(DW_16BIT==1)
	PAP_BUF = (w >> 8)&0xff;

	PAP_WAIT();
	PAP_BUF = w&0xff;
#else
    PAP_BUF = w;
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief   ��һ���������(16bit)
   @param	��
   @return	16λ����
   @note
*/
/*----------------------------------------------------------------------------*/

u16 pap_read()
{
	u16 r;

    PAP_WAIT();
	PAP_DIR_I();
//#if (PAP_8BIT_MODE_EN==1)&&(DW_16BIT==1)
//#if (PAP_8BIT_MODE_EN==1)//&&(DW_16BIT==1) //9325 JOHNSON 20160526
	PAP_BUF = 0xff;
	delay(1000);
	r = (PAP_BUF&0xff)<<8;

	PAP_WAIT();
	PAP_BUF = 0xff;
	delay(1000);
	r |= PAP_BUF & 0xff;
//#else //9325 JOHNSON 20160526
//    PAP_BUF = 0xff;
//    delay(2000);
//    r = PAP_BUF;
//#endif

	return r;
}

/*----------------------------------------------------------------------------*/
/**@brief   ͨ��DMA������
   @param	pBuf��buf��ַ  len������
   @return	��
   @note
*/
/*----------------------------------------------------------------------------*/
void pap_dma_read(void *pBuf, u16 len)
{
	PAP_WAIT();
	PAP_DIR_I();
	PAP_ADR = (u16)(u32)pBuf;
	PAP_CNT = len;
}

/*----------------------------------------------------------------------------*/
/**@brief   ͨ��DMAд����
   @param	pBuf��buf��ַ  len������
   @return	��
   @note
*/
/*----------------------------------------------------------------------------*/
void pap_dma_write(void *pBuf, u16 len)
{
#if 1
    PAP_WAIT();
    PAP_DIR_O();
    PAP_ADR = (u32)pBuf;
    PAP_CNT = len;
#else
    u16 i;
    u8 *p = (u8*)pBuf;
    for(i=0;i<len;i++)
    {
        pap_write(p[i]);
    }
#endif
}
