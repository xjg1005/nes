

#ifndef _ISP_COM_H
#define _ISP_COM_H

#include "dv12.h"
#include "typedef.h"


#define WAIT_ISP1_ACK()         (ISP1_CON0 & BIT(8))

//ISP0
#define ISP0_PCLK_EN()                 ISP0_CON0 |= BIT(0)
#define ISP0_PRST_RESET()              ISP0_CON0 &= ~BIT(1)
#define ISP0_PRST_RELEASE()            ISP0_CON0 |= BIT(1)
#define ISP0_PRE_CFG_KSTART()          ISP0_CON0 |= BIT(2)
#define ISP0_PRE_CFG_BUSY()            (ISP0_CON0 & BIT(3))
#define ISP0_STA_KICK0()               ISP0_CON0 |= BIT(4)
#define ISP0_STA_KICK1()               ISP0_CON0 |= BIT(5)
#define ISP0_STA_FLUSH()               ISP0_CON0 |= BIT(6)
#define ISP0_STA_BUSY()               (ISP0_CON0 & BIT(7))
#define ISP0_STA_TEST0()               ISP0_CON0 |= BIT(8)
#define ISP0_STA_TEST1()               ISP0_CON0 |= BIT(9)
#define ISP0_RAW_KICK()                ISP0_CON0 |= BIT(10)


#define ISP0_SEN_F_CLR()             ISP0_CON1 |= BIT(8)
#define ISP0_SEN_F_PND()             (ISP0_CON1 & BIT(9))
#define ISP0_SEN_L_CLR()             ISP0_CON1 |= BIT(10)
#define ISP0_SEN_L_PND()             (ISP0_CON1 & BIT(11))
#define ISP0_OUT_F_CLR()             ISP0_CON1 |= BIT(12)
#define ISP0_OUT_F_PND()             (ISP0_CON1 & BIT(13))
#define ISP0_OUT_L_CLR()             ISP0_CON1 |= BIT(14)
#define ISP0_OUT_L_PND()             (ISP0_CON1 & BIT(15))
#define ISP0_CFG_D_CLR()             ISP0_CON1 |= BIT(16)
#define ISP0_CFG_D_PND()             (ISP0_CON1 & BIT(17))
#define ISP0_RAW_D_CLR()             ISP0_CON1 |= BIT(18)
#define ISP0_RAW_D_PND()             (ISP0_CON1 & BIT(19))


//ISP1
#define ISP1_SEN_F_EN(x)             ISP1_CON2 &= ~BIT(0);ISP1_CON2 |= (x << 0)
#define ISP1_SEN_F_IE(x)             ISP1_CON2 &= ~BIT(1);ISP1_CON2 |= (x << 1)
#define ISP1_SEN_L_EN(x)             ISP1_CON2 &= ~BIT(2);ISP1_CON2 |= (x << 2)
#define ISP1_SEN_L_IE(x)             ISP1_CON2 &= ~BIT(3);ISP1_CON2 |= (x << 3)

#define ISP1_SEN_F_CLR()             ISP1_CON2 |= BIT(8)
#define ISP1_SEN_F_PND()             (ISP1_CON2 & BIT(9))
#define ISP1_SEN_L_CLR()             ISP1_CON2 |= BIT(10)
#define ISP1_SEN_L_PND()             (ISP1_CON2 & BIT(11))


///////////////////////////////////////////////////////////////////////////////////
/*isp device select*/
enum{
	ISP_DEV_0 = 0,
	ISP_DEV_1,

	ISP_DEV_NONE = 0xff,
};

/*set ISP size type*/
enum{
	ISP_SIZE_INPUT = 0,
	ISP_SIZE_OUTPUT,
	ISP_SIZE_NORMAL,
};

/*sensor data width*/
enum{
	SEN_DATA_WIDTH_8BIT = 0,
	SEN_DATA_WIDTH_10BIT,
	SEN_DATA_WIDTH_16BIT,
};


///////////////////////////////////////////////////////////////////////////////////

#define sensor_puts puts_without_mutex


void isp_io_config(u8 isp_dev, u8 data_width);
void isp_sen_set_size(u8 isp_dev, u8 size_type, u16 width, u16 height,u16 hinv);
void isp_sen_config(u8 isp_dev, u32 sen_con);
void isp_pend_config(u8 isp_dev, u32 pend_con);
void isp0_raw_to_yuv(u32 width,u32 heigh);
void isp0_out_config(u8 test_en);


#endif


