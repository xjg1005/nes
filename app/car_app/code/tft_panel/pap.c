#include "pap.h"
#include "config.h"

#define PAP_TS      14//(2bit) 数据建立时间
#define PAP_TH      12//(2bit) 数据保持时间
#define PAP_TW      8 //(4bit) 读/写使能信号宽度  系统时钟 0:16个 1:1个 2:2个 依此类推
#define PAP_PND     7 //中断请求标志
#define PAP_CPND    6 //此位写'1'清除PND中断请求标志
#define PAP_DW16ED  5 //16bit模式数据大小端选择(8bit模式,需设置为0)   0:低地址数据至端口低位 1:低地址数据至端口高位
#define PAP_DW16EN  4 //0:8bit模式 1:16bit模式
#define PAP_PRE     3 //读信号极性选择 0:空闲时为低电平 1:空闲时为高电平
#define PAP_PWE     2 //写信号极性选择 0:空闲时为低电平 1:空闲时为高电平
#define PAP_DIR     1 //0:发送数据       1:接收数据
#define PAP_EN      0 //PAP接口使能

/*----------------------------------------------------------------------------*/
/**@brief   PAP接口初始化
   @param	无
   @return	无
   @note
*/
/*----------------------------------------------------------------------------*/
void pap_init()
{
	PAP_PORT_EN();//使能PAP接口
	PAP_RD_OUT();
	PAP_WR_OUT();

#if (PAP_8BIT_MODE_EN == 1)
	//对DV12而言，当使用8bit数据宽度模式时，是使用的整组PORT口的高8bit，因此请将DW16ED设置为"1"
	PAP_CON = (2<<PAP_TS) | (2<<PAP_TH) | (5<<PAP_TW) | (1<<PAP_DW16ED) | (1<<PAP_PRE) | (1<<PAP_PWE) | (1<<PAP_EN);
#else
	PAP_CON = (2<<PAP_TS) | (2<<PAP_TH) | (5<<PAP_TW) | (PAP_16BIT_ENDIAN<<PAP_DW16ED) | (1 <<PAP_DW16EN) | (1<<PAP_PRE) | (1<<PAP_PWE) | (1<<PAP_EN);
#endif
}


/*----------------------------------------------------------------------------*/
/**@brief   写一个点的数据(16bit)
   @param	16位数据
   @return	无
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
/**@brief   读一个点的数据(16bit)
   @param	无
   @return	16位数据
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
/**@brief   通过DMA读数据
   @param	pBuf：buf地址  len：长度
   @return	无
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
/**@brief   通过DMA写数据
   @param	pBuf：buf地址  len：长度
   @return	无
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
