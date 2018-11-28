#include "typedef.h"
#include "pap.h"
#include "tft.h"
#include "dv12.h"
#include "ILI8960.h"
#include "imb_module.h"
#include "sdfile.h"



#if (LCD_TYPE == LCD_ILI9325)

/*
CDN3

RESET       PA10
CS          PA6
RS          PA7
RD          PC1
WR          PC2
DB[10..17]  PB[8..15]

DV10 FPGA
RESET                  PA0(P00)
CS                     PA1(P01)
RS                     PA2(P02)
EMI_WR(LCDHSYNC)       PG4(PC4)
EMI_RD(LCDVSYNC)       PG5(PC5)
EMI_D0~EMI_D15         PF0~PF15(PA0-PA7,PB0-PB7)
*/


//ILI9325命令
#define WINDOW_XADDR_START	0x0212 // 水平的开始地址组
#define WINDOW_XADDR_END	0x0213 // 水平的结束地址组
#define WINDOW_YADDR_START	0x0210 // 垂直的开始地址组
#define WINDOW_YADDR_END	0x0211 // 垂直的结束地址组
#define GRAM_XADDR		    0x0200 // GRAM 水平的地址组
#define GRAM_YADDR		    0x0201 // GRAM 垂直的地址组
#define GRAMWR 			    0x0202 // 读写寄存器

u8 LCDBuff[8][128];
//TFT复位
void tft_reset(void)
{
    TFT_RST_H();
    //delay(50000);
    delay_2ms(5);
    TFT_RST_L();
    //delay(50000);
    delay_2ms(5);
    TFT_RST_H();
}

void tft_SetForRW()
{
	TFT_CS_EN();
	TFT_RS_L();
    pap_write(GRAMWR);
	TFT_CS_DIS();
}
//写命令
void Trans_Com_16(u16 d)
{
	TFT_CS_EN();
	TFT_RS_L();
	pap_write16(d);
	TFT_CS_DIS();
}
//写数据
void Trans_Dat_16(u16 d)
{
	TFT_CS_EN();
	TFT_RS_H();
	pap_write16(d);
	TFT_CS_DIS();
}

//写命令
void tft_wrReg(u16 regInd, u16 v)
{
	TFT_CS_EN();
	TFT_RS_L();
	pap_write16(regInd);
	TFT_RS_H();
	pap_write16(v);
	TFT_CS_DIS();
}


u16 tft_rdReg(u8 regInd)
{
	u16 reg_val;

	TFT_CS_EN();
	TFT_RS_L();
	pap_write(regInd);
	TFT_RS_H();
	reg_val = pap_read();
	TFT_CS_DIS();

	return reg_val;
}

//通过DMA写指定长度的数据到GRAM
void tft_WriteByDMA(u8 *pBuf, u16 len)
{
	u16 data, i;
	u8 *p = pBuf;

	TFT_CS_EN();
	TFT_RS_H();
#if 1
	pap_dma_write(pBuf, len);
#else
	for(i = 0; i < len; i += 2)
	{
		data = (u16)((p[i] << 8) | p[i + 1]);
		pap_write(data);
	}
#endif
}

//设置坐标
void tft_SetPos(u16 x, u16 y)
{
    //printf("\nSetPos:%x %x",x,y);
#ifdef SCREEN_ROTATE //横屏显示
    tft_wrReg(GRAM_XADDR, y);
	tft_wrReg(GRAM_YADDR, x);
	tft_SetForRW();
#else
	tft_wrReg(GRAM_XADDR, x);
	tft_wrReg(GRAM_YADDR, y);
	tft_SetForRW();
#endif
}


//设置窗口区域
void tft_SetRange(u16 x0, u16 x1, u16 y0, u16 y1)
{
    if(x1 > TFT_WIDTH - 1)  x1 = TFT_WIDTH - 1;
    if(y1 > TFT_HIGHT - 1)  y1 = TFT_HIGHT - 1;

#ifdef SCREEN_ROTATE //横屏显示
    tft_wrReg(WINDOW_XADDR_START, y0);
	tft_wrReg(WINDOW_XADDR_END, y1);
	tft_wrReg(WINDOW_YADDR_START, x0);
	tft_wrReg(WINDOW_YADDR_END, x1);
#else
	tft_wrReg(WINDOW_XADDR_START, x0);
	tft_wrReg(WINDOW_XADDR_END, x1);
	tft_wrReg(WINDOW_YADDR_START, y0);
	tft_wrReg(WINDOW_YADDR_END, y1);
#endif
//
    tft_SetPos(0,0);
    TFT_CS_EN();
    TFT_RS_H();

//    putchar('x');
}


void tft_ClearArea(u16 color,u16 x0,u16 x1,u16 y0,u16 y1)
{
    tu16 i;
    tu16 width,height;
    u8 *p = (u8 *)LCDBuff;

    width = x1 - x0 + 1;
    height = y1 - y0 + 1;

    tft_SetRange(x0,x1,y0,y1);
    tft_SetPos(x0,y0);

    for(i=0;i<width;i++)
    {
        p[2*i] = (u8)(color >> 8);
        p[2*i+1] = (u8)color;
    }

    for(i=0;i<height;i++)
    {
        tft_WriteByDMA(p,width*2);
    }

    PAP_WAIT_NCLR();
    TFT_CS_DIS();
}




//读取指定坐标数据
u16 tft_ReadOnePixel(u16 x, u16 y)
{
	u16 dat;
	tft_SetPos(x, y); //定义坐标
	TFT_CS_EN();
	TFT_RS_H();
	pap_read();//Dummy Read
	dat = pap_read();
	TFT_CS_DIS();
	return dat;
}


//读取指定区域数据
void tft_ReadMultiPixel(void *pBuf, u16 xs, u16 xe, u16 ys, u16 ye, u16 align)
{
	u32 i,j;
	u32 ret;
    u32 fr = 0;
    u16 *p = (u16 *)pBuf;

	tft_SetRange(xs, xe, ys, ye); //Set HAS/HEA,VSA/VEA
	for(j = ys; j <= ye; j++)
	{
	    tft_wrReg(GRAM_YADDR, j);
		fr = (j - ys) * align;
		for(i = xs; i <= xe; i++)
		{
			tft_wrReg(GRAM_XADDR, i);
			TFT_CS_EN();
			TFT_RS_L();//cmd
			pap_write(GRAMWR);
			TFT_RS_H();//data

			pap_read();//Dummy Read
			p[fr++] = pap_read();
			p[fr++] <= 8;
			p[fr++] |= pap_read();
			TFT_CS_DIS();
		}
	}
}

void tft_drv_init(void)
{
#if 0//9326-120
//************* Start Initial Sequence **********//
tft_wrReg(0x0702, 0x3008);// Set internal timing, don’t change this value
tft_wrReg(0x0705, 0x0036);// Set internal timing, don’t change this value
tft_wrReg(0x070B, 0x1213);// Set internal timing, don’t change this value
tft_wrReg(0x0001, 0x0000);// set SS and SM bit
tft_wrReg(0x0002, 0x0000);// set 1 line inversion
tft_wrReg(0x0003, 0x1038);//0x1000// set GRAM write direction and BGR=1.
tft_wrReg(0x0008, 0x0202);// set the back porch and front porch
tft_wrReg(0x0009, 0x0000);// set non-display area refresh cycle ISC[3:0]
tft_wrReg(0x000C, 0x0000);// RGB interface setting
tft_wrReg(0x000F, 0x0000);// RGB interface polarity
//*************Power On sequence ****************//
tft_wrReg(0x0100, 0x0000);// SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0102, 0x0000);// VREG1OUT voltage
tft_wrReg(0x0103, 0x0000); // VDV[4:0] for VCOM amplitude
delay_2ms(200); // Dis-charge capacitor power voltage
tft_wrReg(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0101, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
delay_2ms(50);   // Delay 50ms
tft_wrReg(0x0102, 0x01BD); // VREG1OUT voltage
delay_2ms(50);   // Delay 50ms
tft_wrReg(0x0103, 0x5400); // VDV[4:0] for VCOM amplitude
tft_wrReg(0x0281, 0x1000); // VCM[5:0] for VCOMH
tft_wrReg(0x020b, 0x0000);
delay_2ms(50);
tft_wrReg(0x0200, 0x0000); // GRAM horizontal Address
tft_wrReg(0x0201, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma   Curve ----------//  tft_wrReg(0x0300, 0x0000);   tft_wrReg(0x0301, 0x0707);
tft_wrReg(0x0300, 0x0000);
tft_wrReg(0x0301, 0x0202);
tft_wrReg(0x0302, 0x0202);
tft_wrReg(0x0305, 0x0202);
tft_wrReg(0x0306, 0x0202);
tft_wrReg(0x0307, 0x0202);
tft_wrReg(0x0308, 0x0202);
tft_wrReg(0x0309, 0x0202);
tft_wrReg(0x030C, 0x0202);
tft_wrReg(0x030D, 0x0202);
//------------------ Set GRAM area --------------//
tft_wrReg(0x0210, 0x0000);// Horizontal GRAM Start Address
tft_wrReg(0x0211, 0x00EF);// Horizontal GRAM End Address
tft_wrReg(0x0212, 0x0000);// Vertical GRAM Start Address
tft_wrReg(0x0213, 0x018F);// Vertical GRAM Start Address
tft_wrReg(0x0400, 0x3100);// Gate Scan Line 400 lines
tft_wrReg(0x0401, 0x0001);// NDL,VLE, REV
tft_wrReg(0x0404, 0x0000);// set scrolling line
//-------------- Partial Display Control ---------//
tft_wrReg(0x0500, 0x0000);//Partial Image 1 Display Position
tft_wrReg(0x0501, 0x0000);//Partial Image 1 RAM Start/End Address
tft_wrReg(0x0502, 0x0000);//Partial Image 1 RAM Start/End Address
tft_wrReg(0x0503, 0x0000);//Partial Image 2 Display Position
tft_wrReg(0x0504, 0x0000);//Partial Image 2 RAM Start/End Address
tft_wrReg(0x0505, 0x0000);//Partial Image 2 RAM Start/End Address
//-------------- Panel Control -------------------//
tft_wrReg(0x0010, 0x0010);//DIVI[1:0];RTNI[4:0]
tft_wrReg(0x0011, 0x0600);//NOWI[2:0];SDTI[2:0]
tft_wrReg(0x0020, 0x0002);//DIVE[1:0];RTNE[5:0]
tft_wrReg(0x0007, 0x0173);// 262K color and display ON
Trans_Com_16(0x0202);

#endif




#if 0        //5420   4001把这个打开   下面9326也打开
				//TW28-U19 SPFD5420 凌阳S303主板调试OK 20151215 解决暗线

					tft_wrReg(0x0606,0x0000);
					delay_2ms(10);
					tft_wrReg(0x0007,0x0001);
					delay_2ms(10);
					tft_wrReg(0x0110,0x0001);
					delay_2ms(10);
					tft_wrReg(0x0100,0x17B0);
					tft_wrReg(0x0101,0x0147);
					tft_wrReg(0x0102,0x019D);
					tft_wrReg(0x0103,0x3600);
					tft_wrReg(0x0281,0x0010);
					delay_2ms(10);
					tft_wrReg(0x0102,0x01BD);
					delay_2ms(10);
					tft_wrReg(0x0000,0x0000);			// ID read register
					tft_wrReg(0x0001,0x0000);			  // Driver output control register 0x0000
					tft_wrReg(0x0002,0x0000);			  // LCD driving waveform control //帧翻转
					tft_wrReg(0x0003,0x1030); // entry mode – 8 bit  0x10A0
					tft_wrReg(0x0008,0x0808);			// Display control 2
					tft_wrReg(0x0009,0x0001);			// Display control 3
					tft_wrReg(0x000B,0x0010);			// Low power control
					tft_wrReg(0x000C,0x0001);	// External display interface control 1
					tft_wrReg(0x000F,0x0000);			// External display interface control 2
					tft_wrReg(0x0007,0x0001);			// Display control 1
					tft_wrReg(0x0010,0x0010); //帧频
					tft_wrReg(0x0011,0x0202);
					tft_wrReg(0x0012,0x0300);
					tft_wrReg(0x0020,0x021E);
					tft_wrReg(0x0021,0x0202);
					tft_wrReg(0x0022,0x0100);
					tft_wrReg(0x0090,0x8000);		   // Frame marker control
					tft_wrReg(0x0092,0x0000);		   // Frame marker control

					//	tft_wrReger control 1~6 ---------------//
					tft_wrReg(0x0100,0x16B0);

					tft_wrReg(0x0101,0x0147);

					tft_wrReg(0x0102,0x01Bd);

					tft_wrReg(0x0103,0x1000); //调对比度0x2600
					tft_wrReg(0x020b,0x0000);


					tft_wrReg(0x0107,0x0000);
					tft_wrReg(0x0110,0x0001);
					tft_wrReg(0x0210,0x0000); // Window horizontal RAM address start
					tft_wrReg(0x0211,0x00EF); // Window horizontal RAM address end
					tft_wrReg(0x0212,0x0000); // Window vertical RAM address start
					tft_wrReg(0x0213,0x018F); // Window vertical RAM address end
					tft_wrReg(0x0280,0x0000); // NVM write / read

					tft_wrReg(0x0281,0x0008); // Vcom high voltage 1 调水波纹[DB4-DB0]VCM1 0x0001	vcom  000a

				//	tft_wrReg(0x0281,0x000c); // Vcom high voltage 1 调水波纹[DB4-DB0]VCM1 0x0001

					tft_wrReg(0x0282,0x0001); // Vcom high voltage 1 调水波纹[DB4-DB0]VCM2 0x0001
					tft_wrReg(0x0200,0x0000); // GRAM horizontal Address
					tft_wrReg(0x0201,0x0000); //0x018f// GRAM Vertical Address
					 // tft_wrRegma 2.2 control ---------------//
#if 0
					tft_wrReg(0x0300,0x0101);
					tft_wrReg(0x0301,0x0b26);
					tft_wrReg(0x0302,0x1329);
					tft_wrReg(0x0303,0x2913);
					tft_wrReg(0x0304,0x260b);
					tft_wrReg(0x0305,0x0101);
					tft_wrReg(0x0306,0x1805);
					tft_wrReg(0x0307,0x0418);
					tft_wrReg(0x0308,0x0205);
					tft_wrReg(0x0309,0x0303);
					tft_wrReg(0x030A,0x0e05);
					tft_wrReg(0x030B,0x0d01);
					tft_wrReg(0x030C,0x010d);
					tft_wrReg(0x030D,0x050e);
					tft_wrReg(0x030E,0x0303);
					tft_wrReg(0x030F,0x0502);
#endif

#if 0
							tft_wrReg(0x0300,0x0101);
							tft_wrReg(0x0301,0x0b27);
							tft_wrReg(0x0302,0x132a);
							tft_wrReg(0x0303,0x2a13);
							tft_wrReg(0x0304,0x270b);
							tft_wrReg(0x0305,0x0101);
							tft_wrReg(0x0306,0x1205);
							tft_wrReg(0x0307,0x0512);
							tft_wrReg(0x0308,0x0005);
							tft_wrReg(0x0309,0x0003);
							tft_wrReg(0x030A,0x0f04);
							tft_wrReg(0x030B,0x0f00);
							tft_wrReg(0x030C,0x000f);
							tft_wrReg(0x030D,0x040f);
							tft_wrReg(0x030E,0x0300);
							tft_wrReg(0x030F,0x0500);
#endif
					tft_wrReg(0x0400,0x3100);
					tft_wrReg(0x0401,0x0007);
					tft_wrReg(0x0404,0x0000);
					tft_wrReg(0x0500,0x0000);
					tft_wrReg(0x0501,0x0000);
					tft_wrReg(0x0502,0x0000);
					tft_wrReg(0x0503,0x0000);
					tft_wrReg(0x0504,0x0000);
					tft_wrReg(0x0505,0x0000);
					tft_wrReg(0x0600,0x0000); //Software reset
					tft_wrReg(0x0606,0x0000); //i80-i/f Endian control
					tft_wrReg(0x06F0,0x0000); // NVA access control
					tft_wrReg(0x07F0,0x5420);
					tft_wrReg(0x07F2,0x00DF);
					tft_wrReg(0x07F3,0x288C);
					tft_wrReg(0x07F4,0x0022);
					tft_wrReg(0x07F5,0x0061);
					tft_wrReg(0x07F0,0x0000);
					tft_wrReg(0x0007,0x0173); // Display on
					Trans_Com_16(0x0202);
#endif



#if 0//9326-117  ********OK********
//************* Start Initial Sequence ***********//
tft_wrReg(0x0702, 0x3008);// Set internal timing, don’t change this value
tft_wrReg(0x0705, 0x0036);// Set internal timing, don’t change this value
tft_wrReg(0x070B, 0x1213);// Set internal timing, don’t change this value
tft_wrReg(0x0001, 0x0100);// set SS and SM bit
tft_wrReg(0x0002, 0x0000);// set 1 line inversion
tft_wrReg(0x0003, 0x1008);// set GRAM write direction and BGR=1.
tft_wrReg(0x0008, 0x0202);// set the back porch and front porch
tft_wrReg(0x0009, 0x0000);// set non-display area refresh cycle ISC[3:0]
tft_wrReg(0x000C, 0x0000);// RGB interface setting
tft_wrReg(0x000F, 0x0000);// RGB interface polarity


//*************Power On sequence ****************//
tft_wrReg(0x0100, 0x0000);// SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0102, 0x0000);// VREG1OUT voltage
tft_wrReg(0x0103, 0x2600); // VDV[4:0] for VCOM amplitude   2600
delay_2ms(200); // Dis-charge capacitor power voltage
tft_wrReg(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0101, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
delay_2ms(50);   // Delay 50ms
tft_wrReg(0x0102, 0x01BD); // VREG1OUT voltage
delay_2ms(50);   // Delay 50ms
tft_wrReg(0x0103, 0x2600); // VDV[4:0] for VCOM amplitude
tft_wrReg(0x0281, 0x0002); // VCM[5:0] for VCOMH
tft_wrReg(0x020b, 0x0000);
delay_2ms(50);
tft_wrReg(0x0200, 0x00EF); //OO// GRAM horizontal Address
tft_wrReg(0x0201, 0x018F); // GRAM Vertical Address
// ----------- Adjust the Gamma   Curve ----------//  tft_wrReg(0x0300, 0x0000);   tft_wrReg(0x0301, 0x0707);
//------------------ Set GRAM area ---------------//
	tft_wrReg(0x0210, 0x0000);// Horizontal GRAM Start Address
	tft_wrReg(0x0211, 0x00EF);// Horizontal GRAM End Address
	tft_wrReg(0x0212, 0x0000);// Vertical GRAM Start Address
	tft_wrReg(0x0213, 0x018F);// Vertical GRAM Start Address
	tft_wrReg(0x0400, 0xb104);// Gate Scan Line 400 lines
	tft_wrReg(0x0401, 0x0001);// NDL,VLE, REV
	tft_wrReg(0x0404, 0x0000);// set scrolling line
	//-------------- Partial Display Control ---------//
	tft_wrReg(0x0500, 0x0000);//Partial Image 1 Display Position
	tft_wrReg(0x0501, 0x0000);//Partial Image 1 RAM Start/End Address
	tft_wrReg(0x0502, 0x0000);//Partial Image 1 RAM Start/End Address
	tft_wrReg(0x0503, 0x0000);//Partial Image 2 Display Position
	tft_wrReg(0x0504, 0x0000);//Partial Image 2 RAM Start/End Address
	tft_wrReg(0x0505, 0x0000);//Partial Image 2 RAM Start/End Address
	//-------------- Panel Control -------------------//
	tft_wrReg(0x0010, 0x0010);//DIVI[1:0];RTNI[4:0]
	tft_wrReg(0x0011, 0x0600);//NOWI[2:0];SDTI[2:0]
	tft_wrReg(0x0020, 0x0002);//DIVE[1:0];RTNE[5:0]
	tft_wrReg(0x0007, 0x0173);// 262K color and display ON K color and display ON
	Trans_Com_16(0x0202);
#endif



#if  0  //LG4551
	tft_wrReg(0x0000,0x0001);
	//*************Power On sequence ****************//
	tft_wrReg(0x0015,0x7050);
	tft_wrReg(0x0011,0x0112);
	tft_wrReg(0x0010,0x3668);
	tft_wrReg(0x0012,0x0002);
	tft_wrReg(0x0013,0x0D2b);
	delay_2ms(40);
	tft_wrReg(0x0012,0x0012);
	delay_2ms(40);
	tft_wrReg(0x0010,0x3660);
	tft_wrReg(0x0013,0x2d2b); //--2d26//3538//3232
	delay_2ms(20);
	// ----------- Display Mode & Gamma settings ----------//
	tft_wrReg(0x0030,0x0007);
	tft_wrReg(0x0031,0x0502);
	tft_wrReg(0x0032,0x0307);
	tft_wrReg(0x0033,0x0305);//0303
	tft_wrReg(0x0034,0x0004);
	tft_wrReg(0x0035,0x0401);
	tft_wrReg(0x0036,0x0007);
	tft_wrReg(0x0037,0x0603);//0403
	tft_wrReg(0x0038,0x0E1E);
	tft_wrReg(0x0039,0x0f1f);
	tft_wrReg(0x0001,0x0100);
	tft_wrReg(0x0002,0x0300);
	tft_wrReg(0x0003,0x1010);
	tft_wrReg(0x0016,0x0001);//--加的
	tft_wrReg(0x0008,0x0206);//0808
	tft_wrReg(0x000A,0x0008);
	tft_wrReg(0x0060,0x3100);
	tft_wrReg(0x0061,0x0001);
	//tft_wrReg(0x006a,0x0031);
	tft_wrReg(0x0090,0x0046);
	tft_wrReg(0x0092,0x010F);
	tft_wrReg(0x0093,0x0701);
	tft_wrReg(0x009A,0x0007);
	tft_wrReg(0x00A3,0x0010);

	tft_wrReg(0x0050, 0x0000);
	tft_wrReg(0x0051, 0x00ef);
	tft_wrReg(0x0052, 0x0000);
	tft_wrReg(0x0053, 0X018f);


	//-------------- Display ON sequence -------------------//
	tft_wrReg(0x0007,0x0001);
	tft_wrReg(0x0007,0x0021);
	tft_wrReg(0x0007,0x0023);
	tft_wrReg(0x0007,0x0033);
	tft_wrReg(0x0007,0x0133);

	tft_wrReg(0x0020,0x0000);
	tft_wrReg(0x0021,0x018f);
    Trans_Com_16(0x0022);
#endif
#if  0   //LG4551
	tft_wrReg(0x0000,0x0001);
	//*************Power On sequence ****************//
	tft_wrReg(0x0015,0x7050);
	tft_wrReg(0x0011,0x0112);
	tft_wrReg(0x0010,0x3668);
	tft_wrReg(0x0012,0x0002);
	tft_wrReg(0x0013,0x0D2b);
	delay_2ms(40);
	tft_wrReg(0x0012,0x0012);
	delay_2ms(40);
	tft_wrReg(0x0010,0x3660);
	tft_wrReg(0x0013,0x2d2b); //--2d26//3538//3232
	delay_2ms(20);
	// ----------- Display Mode & Gamma settings ----------//
	tft_wrReg(0x0030,0x0007);
	tft_wrReg(0x0031,0x0502);
	tft_wrReg(0x0032,0x0307);
	tft_wrReg(0x0033,0x0305);//0303
	tft_wrReg(0x0034,0x0004);
	tft_wrReg(0x0035,0x0401);
	tft_wrReg(0x0036,0x0007);
	tft_wrReg(0x0037,0x0603);//0403
	tft_wrReg(0x0038,0x0E1E);
	tft_wrReg(0x0039,0x0f1f);
	tft_wrReg(0x0001,0x0100);
	tft_wrReg(0x0002,0x0300);
	tft_wrReg(0x0003,0x1010);
	tft_wrReg(0x0016,0x0001);//--加的
	tft_wrReg(0x0008,0x0206);//0808
	tft_wrReg(0x000A,0x0008);
	tft_wrReg(0x0060,0x3100);
	tft_wrReg(0x0061,0x0001);
	//tft_wrReg(0x006a,0x0031);
	tft_wrReg(0x0090,0x0046);
	tft_wrReg(0x0092,0x010F);
	tft_wrReg(0x0093,0x0701);
	tft_wrReg(0x009A,0x0007);
	tft_wrReg(0x00A3,0x0010);

	tft_wrReg(0x0050, 0x0000);
	tft_wrReg(0x0051, 0x00ef);
	tft_wrReg(0x0052, 0x0000);
	tft_wrReg(0x0053, 0X018f);


	//-------------- Display ON sequence -------------------//
	tft_wrReg(0x0007,0x0001);
	tft_wrReg(0x0007,0x0021);
	tft_wrReg(0x0007,0x0023);
	tft_wrReg(0x0007,0x0033);
	tft_wrReg(0x0007,0x0133);

	tft_wrReg(0x0020,0x0000);
	tft_wrReg(0x0021,0x018f);
    Trans_Com_16(0x00022);
#endif


#if 0//4551
	tft_wrReg(0x0000,0x0001);
	//*************Power On sequence ****************//
	tft_wrReg(0x0015,0x7040);
	tft_wrReg(0x0011,0x0110);
	tft_wrReg(0x0010,0x3628);
	tft_wrReg(0x0012,0x0002);
	tft_wrReg(0x0013,0x0D26);	//	di 8 vcm
	delay_2ms(40);
	tft_wrReg(0x0012,0x0012);
	delay_2ms(40);
	tft_wrReg(0x0010,0x3620);
	tft_wrReg(0x0013,0x3A32); //--2d26//3538//3232
	delay_2ms(20);
	// ----------- Display Mode & Gamma settings ----------//
	tft_wrReg(0x0030,0x0007);
	tft_wrReg(0x0031,0x0502);
	tft_wrReg(0x0032,0x0307);
	tft_wrReg(0x0033,0x0305);//0303
	tft_wrReg(0x0034,0x0004);
	tft_wrReg(0x0035,0x0401);
	tft_wrReg(0x0036,0x0007);
	tft_wrReg(0x0037,0x0603);//0403
	tft_wrReg(0x0038,0x0E1E);
	tft_wrReg(0x0039,0x0E1E);
	tft_wrReg(0x0001,0x0100);
	tft_wrReg(0x0002,0x0300);
	tft_wrReg(0x0003,0x1010);
	tft_wrReg(0x0016,0x0001);//--加的
	tft_wrReg(0x0008,0x0206);//0808
	tft_wrReg(0x000A,0x0008);
	tft_wrReg(0x0060,0x3100);
	tft_wrReg(0x0061,0x0001);
	//tft_wrReg(0x006a,0x0031);
	tft_wrReg(0x0090,0x0046);
	tft_wrReg(0x0092,0x010F);
	tft_wrReg(0x0093,0x0701);
	tft_wrReg(0x009A,0x0007);
	tft_wrReg(0x00A3,0x0010);

	//-------------- Display ON sequence -------------------//
	tft_wrReg(0x0007,0x0001);
	tft_wrReg(0x0007,0x0021);
	tft_wrReg(0x0007,0x0023);
	tft_wrReg(0x0007,0x0033);
	tft_wrReg(0x0007,0x0133);

	//tft_wrReg(0x0020,0x01AF);//431
	//tft_wrReg(0x0021,0x00EF); //239
	tft_wrReg(0x0020,0x0000);
	tft_wrReg(0x0021,0x018F);
	Trans_Com_16(0x0022);
#endif


#if 0//4001-120线 lidan 20151109
	//************* Start Initial Sequence **********//
	tft_wrReg(0x0702, 0x3008);// Set internal timing, don’t change this value
	tft_wrReg(0x0705, 0x0036);// Set internal timing, don’t change this value
	tft_wrReg(0x070B, 0x1213);// Set internal timing, don’t change this value
	tft_wrReg(0x0001, 0x0100);// set SS and SM bit
	tft_wrReg(0x0002, 0x0000);// set 1 line inversion
	tft_wrReg(0x0003, 0x1008);// set GRAM write direction and BGR=1.
	tft_wrReg(0x0008, 0x0202);// set the back porch and front porch
	tft_wrReg(0x0009, 0x0000);// set non-display area refresh cycle ISC[3:0]
	tft_wrReg(0x000C, 0x0000);// RGB interface setting
	tft_wrReg(0x000F, 0x0000);// RGB interface polarity
	//*************Power On sequence ****************//
	tft_wrReg(0x0100, 0x0000);// SAP, BT[3:0], AP, DSTB, SLP, STB
	tft_wrReg(0x0102, 0x0000);// VREG1OUT voltage
	tft_wrReg(0x0103, 0x2600); // VDV[4:0] for VCOM amplitude
	delay_2ms(200); // Dis-charge capacitor power voltage
	tft_wrReg(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
	tft_wrReg(0x0101, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
	delay_2ms(50);	 // Delay 50ms
	tft_wrReg(0x0102, 0x010F); // VREG1OUT voltage
	delay_2ms(50);	 // Delay 50ms
	tft_wrReg(0x0103, 0x5400); // VDV[4:0] for VCOM amplitude
	tft_wrReg(0x0281, 0x1000); // VCM[5:0] for VCOMH
	tft_wrReg(0x020b, 0x0000);
	delay_2ms(50);
	tft_wrReg(0x0200, 0x00ef); // GRAM horizontal Address
	tft_wrReg(0x0201, 0x018f); // GRAM Vertical Address
	// ----------- Adjust the Gamma   Curve ----------//  tft_wrReg(0x0300, 0x0000);	tft_wrReg(0x0301, 0x0707);
	//------------------ Set GRAM area ---------------//
		tft_wrReg(0x0210, 0x0000);// Horizontal GRAM Start Address
		tft_wrReg(0x0211, 0x00EF);// Horizontal GRAM End Address
		tft_wrReg(0x0212, 0x0000);// Vertical GRAM Start Address
		tft_wrReg(0x0213, 0x018F);// Vertical GRAM Start Address
		tft_wrReg(0x0400, 0xb104);// Gate Scan Line 400 lines
		tft_wrReg(0x0401, 0x0001);// NDL,VLE, REV
		tft_wrReg(0x0404, 0x0000);// set scrolling line
		//-------------- Partial Display Control ---------//
		tft_wrReg(0x0500, 0x0000);//Partial Image 1 Display Position
		tft_wrReg(0x0501, 0x0000);//Partial Image 1 RAM Start/End Address
		tft_wrReg(0x0502, 0x0000);//Partial Image 1 RAM Start/End Address
		tft_wrReg(0x0503, 0x0000);//Partial Image 2 Display Position
		tft_wrReg(0x0504, 0x0000);//Partial Image 2 RAM Start/End Address
		tft_wrReg(0x0505, 0x0000);//Partial Image 2 RAM Start/End Address
		//-------------- Panel Control -------------------//
		tft_wrReg(0x0010, 0x0010);//DIVI[1:0];RTNI[4:0]
		tft_wrReg(0x0011, 0x0600);//WI[2:0];SDTI[2:0]
		tft_wrReg(0x0020, 0x0002);//DIVE[1:0];RTNE[5:0]
		tft_wrReg(0x0007, 0x0173);// 262K color and display ON K color and display ON

		Trans_Com_16(0x0202);

#endif

#if 0
	 //BODEKANG9326
//************* Start Initial Sequence **********//
tft_wrReg(0x0702, 0x3008);// Set internal timing, don’t change this value
tft_wrReg(0x0705, 0x0036);// Set internal timing, don’t change this value
tft_wrReg(0x070B, 0x1213);// Set internal timing, don’t change this value
tft_wrReg(0x0001, 0x0100); // set SS and SM bit
tft_wrReg(0x0002, 0x0000); // set 1 line inversion
tft_wrReg(0x0003, 0x1028); // set GRAM write direction and BGR=1. //0x1000//
tft_wrReg(0x0008, 0x0202); // set the back porch and front porch
tft_wrReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
tft_wrReg(0x000C, 0x0000); // RGB interface setting
tft_wrReg(0x000F, 0x0000); // RGB interface polarity
//*************Power On sequence ****************//
tft_wrReg(0x0100, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0102, 0x0000); // VREG1OUT voltage
tft_wrReg(0x0103, 0x0000); // VDV[4:0] for VCOM amplitude
delay_2ms(200); // Dis-charge capacitor power voltage
tft_wrReg(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0101, 0x0001); // DC1[2:0], DC0[2:0], VC[2:0]
delay_2ms(50); // Delay 50ms
tft_wrReg(0x0102, 0x001D); // VREG1OUT voltage
delay_2ms(50); // Delay 50ms
tft_wrReg(0x0103, 0x2a00); // VDV[4:0] for VCOM amplitude //2a00
tft_wrReg(0x0281, 0x003f); // VCM[5:0] for VCOMH
tft_wrReg(0x020b, 0x0001);
delay_2ms(50);
tft_wrReg(0x0200, 0x0000); // GRAM horizontal Address
tft_wrReg(0x0201, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//
tft_wrReg(0x0300, 0x0000);
tft_wrReg(0x0301, 0x0507);
tft_wrReg(0x0302, 0x0203);
tft_wrReg(0x0305, 0x0105);
tft_wrReg(0x0306, 0x000F);
tft_wrReg(0x0307, 0x0605);
tft_wrReg(0x0308, 0x0305);
tft_wrReg(0x0309, 0x0707);
tft_wrReg(0x030C, 0x0502);
tft_wrReg(0x030D, 0x040E);
//------------------ Set GRAM area ---------------//
tft_wrReg(0x0210, 0x0000); // Horizontal GRAM Start Address
tft_wrReg(0x0211, 0x00EF); // Horizontal GRAM End Address
tft_wrReg(0x0212, 0x0000); // Vertical GRAM Start Address
tft_wrReg(0x0213, 0x018F); // Vertical GRAM Start Address
tft_wrReg(0x0400, 0xb104); // Gate Scan Line
tft_wrReg(0x0401, 0x0001); // NDL,VLE, REV
tft_wrReg(0x0404, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
tft_wrReg(0x0500, 0x0000);//Partial Image 1 Display Position
tft_wrReg(0x0501, 0x0000); //Partial Image 1 RAM Start/End Address
tft_wrReg(0x0502, 0x0000); //Partial Image 1 RAM Start/End Address
tft_wrReg(0x0503, 0x0000); //Partial Image 2 Display Position
tft_wrReg(0x0504, 0x0000);//Partial Image 2 RAM Start/End Address
tft_wrReg(0x0505, 0x0000); //Partial Image 2 RAM Start/End Address
//-------------- Panel Control -------------------//
tft_wrReg(0x0010, 0x0010); //DIVI[1:0];RTNI[4:0]
tft_wrReg(0x0011, 0x0600); //NOWI[2:0];SDTI[2:0]
tft_wrReg(0x0020, 0x0002); //DIVE[1:0];RTNE[5:0]
tft_wrReg(0x0007, 0x0173); // 262K color and display ON
Trans_Com_16(0x0202);
#endif
#if 0    //9327-3.0-ipv
//************* Start Initial Sequence **********//
tft_wrReg(0x0702, 0x3008);// Set internal timing, don’t change this value
tft_wrReg(0x0705, 0x0036);// Set internal timing, don’t change this value
tft_wrReg(0x070B, 0x1213);// Set internal timing, don’t change this value
tft_wrReg(0x0001, 0x0100); // set SS and SM bit
tft_wrReg(0x0002, 0x0100); // set 1 line inversion
tft_wrReg(0x0003, 0x1028); // set GRAM write direction and BGR=1.
tft_wrReg(0x0008, 0x0202); // set the back porch and front porch
tft_wrReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
tft_wrReg(0x000C, 0x0000); // RGB interface setting
tft_wrReg(0x000F, 0x0000); // RGB interface polarity
//*************Power On sequence ****************//
tft_wrReg(0x0100, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0102, 0x0000); // VREG1OUT voltage
tft_wrReg(0x0103, 0x2600); // VDV[4:0] for VCOM amplitude
delay_2ms(200); // Dis-charge capacitor power voltage
tft_wrReg(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0101, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
delay_2ms(50); // Delay 50ms
tft_wrReg(0x0102, 0x01BD); // VREG1OUT voltage
delay_2ms(50); // Delay 50ms
tft_wrReg(0x0103, 0x3400); // VDV[4:0] for VCOM amplitude
tft_wrReg(0x0281, 0x0002); // VCM[5:0] for VCOMH
delay_2ms(50);
tft_wrReg(0x0200, 0x00ef); // GRAM horizontal Address
tft_wrReg(0x0201, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//
//tft_wrReg(0x0300, 0x0000);
//tft_wrReg(0x0301, 0x0303);
//tft_wrReg(0x0302, 0x0203);
//tft_wrReg(0x0305, 0x0205);
//tft_wrReg(0x0306, 0x040C);
//tft_wrReg(0x0307, 0x0405);
//tft_wrReg(0x0308, 0x0404);
//tft_wrReg(0x0309, 0x0707);
//tft_wrReg(0x030C, 0x0502);
//tft_wrReg(0x030D, 0x1006);
//------------------ Set GRAM area ---------------//
tft_wrReg(0x0210, 0x0000); // Horizontal GRAM Start Address
tft_wrReg(0x0211, 0x00EF); // Horizontal GRAM End Address
tft_wrReg(0x0212, 0x0000); // Vertical GRAM Start Address
tft_wrReg(0x0213, 0x018F); // Vertical GRAM Start Address
tft_wrReg(0x0400, 0x3500); // Gate Scan Line
tft_wrReg(0x0401, 0x0001); // NDL,VLE, REV
tft_wrReg(0x0404, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
tft_wrReg(0x0500, 0x0000);//Partial Image 1 Display Position
tft_wrReg(0x0501, 0x0000); //Partial Image 1 RAM Start/End Address
tft_wrReg(0x0502, 0x0000); //Partial Image 1 RAM Start/End Address
tft_wrReg(0x0503, 0x0000); //Partial Image 2 Display Position
tft_wrReg(0x0504, 0x0000);//Partial Image 2 RAM Start/End Address
tft_wrReg(0x0505, 0x0000); //Partial Image 2 RAM Start/End Address
//-------------- Panel Control -------------------//
tft_wrReg(0x0010, 0x0010); //DIVI[1:0];RTNI[4:0]
tft_wrReg(0x0011, 0x0600); //NOWI[2:0];SDTI[2:0]
tft_wrReg(0x0020, 0x0002); //DIVE[1:0];RTNE[5:0]
tft_wrReg(0x0007, 0x0173); // 262K color and display ON
Trans_Com_16(0x0202);
#endif
#if 0 //9326-2.8
tft_wrReg(0x0702, 0x3008);// Set internal timing, don’t change this value
tft_wrReg(0x0705, 0x0036);// Set internal timing, don’t change this value
tft_wrReg(0x070B, 0x1213);// Set internal timing, don’t change this value
tft_wrReg(0x0001, 0x0100); // set SS and SM bit
tft_wrReg(0x0002, 0x0100); // set 1 line inversion
tft_wrReg(0x0003, 0x1028); // set GRAM write direction and BGR=1.
tft_wrReg(0x0008, 0x0202); // set the back porch and front porch
tft_wrReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
tft_wrReg(0x000C, 0x0000); // RGB interface setting
tft_wrReg(0x000F, 0x0000); // RGB interface polarity
//*************Power On sequence ****************//
tft_wrReg(0x0100, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0102, 0x0000); // VREG1OUT voltage
tft_wrReg(0x0103, 0x0000); // VDV[4:0] for VCOM amplitude
delay_2ms(200); // Dis-charge capacitor power voltage
tft_wrReg(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0101, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
delay_2ms(50); // Delay 50ms
tft_wrReg(0x0102, 0x01BD); // VREG1OUT voltage
delay_2ms(50); // Delay 50ms
tft_wrReg(0x0103, 0x2F00); // VDV[4:0] for VCOM amplitude
tft_wrReg(0x0281, 0x0008); // VCM[5:0] for VCOMH
delay_2ms(50);
tft_wrReg(0x0200, 0x0000); // GRAM horizontal Address
tft_wrReg(0x0201, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//
tft_wrReg(0x0300, 0x0000);
tft_wrReg(0x0301, 0x0406);
tft_wrReg(0x0302, 0x0303);
tft_wrReg(0x0305, 0x0005);
tft_wrReg(0x0306, 0x1604);
tft_wrReg(0x0307, 0x0404);
tft_wrReg(0x0308, 0x0103);
tft_wrReg(0x0309, 0x0707);
tft_wrReg(0x030C, 0x0500);
tft_wrReg(0x030D, 0x000F);
//------------------ Set GRAM area ---------------//
tft_wrReg(0x0210, 0x0000); // Horizontal GRAM Start Address
tft_wrReg(0x0211, 0x00EF); // Horizontal GRAM End Address
tft_wrReg(0x0212, 0x0000); // Vertical GRAM Start Address
tft_wrReg(0x0213, 0x018F); // Vertical GRAM Start Address
tft_wrReg(0x0400, 0x3500); // Gate Scan Line
tft_wrReg(0x0401, 0x0001); // NDL,VLE, REV
tft_wrReg(0x0404, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
tft_wrReg(0x0500, 0x0000);//Partial Image 1 Display Position
tft_wrReg(0x0501, 0x0000); //Partial Image 1 RAM Start/End Address
tft_wrReg(0x0502, 0x0000); //Partial Image 1 RAM Start/End Address
tft_wrReg(0x0503, 0x0000); //Partial Image 2 Display Position
tft_wrReg(0x0504, 0x0000);//Partial Image 2 RAM Start/End Address
tft_wrReg(0x0505, 0x0000); //Partial Image 2 RAM Start/End Address
//-------------- Panel Control -------------------//
tft_wrReg(0x0010, 0x0010); //DIVI[1:0];RTNI[4:0]
tft_wrReg(0x0011, 0x0600); //NOWI[2:0];SDTI[2:0]
tft_wrReg(0x0020, 0x0002); //DIVE[1:0];RTNE[5:0]
tft_wrReg(0x0007, 0x0173); // 262K color and display ON
Trans_Com_16(0x0202);
#endif
#if 1//9326-4001-3.0
//************* Start Initial Sequence **********//
tft_wrReg(0x0702, 0x3008);// Set internal timing, don’t change this value
tft_wrReg(0x0705, 0x0036);// Set internal timing, don’t change this value
tft_wrReg(0x070B, 0x1213);// Set internal timing, don’t change this value
tft_wrReg(0x0001, 0x0100); // set SS and SM bit
tft_wrReg(0x0002, 0x0100); // set 1 line inversion
tft_wrReg(0x0003, 0x1028); // set GRAM write direction and BGR=1.
tft_wrReg(0x0008, 0x0202); // set the back porch and front porch
tft_wrReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
tft_wrReg(0x000C, 0x0000); // RGB interface setting
tft_wrReg(0x000F, 0x0000); // RGB interface polarity
//*************Power On sequence ****************//
tft_wrReg(0x0100, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0102, 0x0000); // VREG1OUT voltage
tft_wrReg(0x0103, 0x0000); // VDV[4:0] for VCOM amplitude
delay_2ms(200); // Dis-charge capacitor power voltage
tft_wrReg(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
tft_wrReg(0x0101, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
delay_2ms(50); // Delay 50ms
tft_wrReg(0x0102, 0x01BD); // VREG1OUT voltage
delay_2ms(50); // Delay 50ms
tft_wrReg(0x0103, 0x2000); // VDV[4:0] for VCOM amplitude
tft_wrReg(0x0281, 0x0008); // VCM[5:0] for VCOMH
delay_2ms(50);
tft_wrReg(0x0200, 0x0000); // GRAM horizontal Address
tft_wrReg(0x0201, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//
//tft_wrReg(0x0300, 0x0000);
//tft_wrReg(0x0301, 0x0406);
//tft_wrReg(0x0302, 0x0303);
//tft_wrReg(0x0305, 0x0005);
//tft_wrReg(0x0306, 0x1604);
//tft_wrReg(0x0307, 0x0404);
//tft_wrReg(0x0308, 0x0103);
//tft_wrReg(0x0309, 0x0707);
//tft_wrReg(0x030C, 0x0500);
//tft_wrReg(0x030D, 0x000F);
//------------------ Set GRAM area ---------------//
tft_wrReg(0x0210, 0x0000); // Horizontal GRAM Start Address
tft_wrReg(0x0211, 0x00EF); // Horizontal GRAM End Address
tft_wrReg(0x0212, 0x0000); // Vertical GRAM Start Address
tft_wrReg(0x0213, 0x018F); // Vertical GRAM Start Address
tft_wrReg(0x0400, 0x3500); // Gate Scan Line
tft_wrReg(0x0401, 0x0001); // NDL,VLE, REV
tft_wrReg(0x0404, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
tft_wrReg(0x0500, 0x0000);//Partial Image 1 Display Position
tft_wrReg(0x0501, 0x0000); //Partial Image 1 RAM Start/End Address
tft_wrReg(0x0502, 0x0000); //Partial Image 1 RAM Start/End Address
tft_wrReg(0x0503, 0x0000); //Partial Image 2 Display Position
tft_wrReg(0x0504, 0x0000);//Partial Image 2 RAM Start/End Address
tft_wrReg(0x0505, 0x0000); //Partial Image 2 RAM Start/End Address
//-------------- Panel Control -------------------//
tft_wrReg(0x0010, 0x0010); //DIVI[1:0];RTNI[4:0]
tft_wrReg(0x0011, 0x0600); //NOWI[2:0];SDTI[2:0]
tft_wrReg(0x0020, 0x0002); //DIVE[1:0];RTNE[5:0]
tft_wrReg(0x0007, 0x0173); // 262K color and display ON
Trans_Com_16(0x0202);
#endif
}


void tft_ClearScreen(u16 color)
{
	unsigned int i, j;
	u8 *p = (u8 *)LCDBuff;

//    tft_SetRange(0, 319,0, 239);
//    tft_SetRange(0,239 ,0,  319);
//	tft_SetPos(0, 0);
	for(i = 0; i < (TFT_WIDTH*2); i += 2)
	{
		p[i] = color >> 8;
		p[i + 1] = color;
	}

    for(i = 0; i < TFT_HIGHT; i++)
    {
        tft_WriteByDMA(p, (TFT_WIDTH*2));
    }
    PAP_WAIT_NCLR();
    TFT_CS_DIS();
}


void tft_SetColor(u16 BackColor,u16 ForeColor)
{
    PAP_DAT0 = BackColor;
    PAP_DAT1 = ForeColor;
}


void tft_init_9325(void)
{
    //u16 id;

    puts("\ntft init");

    TFT_CS_DIS();
    tft_reset(); //由于引脚和sensor复用的原因注释掉
    delay_2ms(20);

    //os_time_dly(40);

	pap_init();

//    while(1)
//    {
//        pap_write(id++);
//        delay(100);
//    }

	tft_drv_init();

    //tft_SetRange(0, 319, 0, 239);

//	id = tft_rdReg(0);
//	puts("\nlcd id:");
//	put_u16hex(id);
//
//
//	id = tft_rdReg(0X03);
//	puts("\n0X03:");
//	put_u16hex(id);
//
//	id = tft_rdReg(0X07);
//	puts("\n0X07:");
//	put_u16hex(id);
//    while(1)
//    {
//
        tft_ClearScreen(COLOUR_RED);
        delay_2ms(500);
//        tft_ClearScreen(COLOUR_BLUE);
//        delay_2ms(500);
//        tft_ClearScreen(COLOUR_GREEN);
//        delay_2ms(500);
//    }

//
//    u16 dat = tft_ReadOnePixel(0x01,0x01);
//    printf("\ndat:%x",dat);
//    puts("\ntft out");
}


const L0_PARA_CFG lcd_ili9325_inch_para =
{
    400*2+20+20,
    20,
    20,
    400,

    240+20+20,
    20,
    20,
    0,
    240,
};

void tft_SetRange(u16 x0, u16 x1, u16 y0, u16 y1);
void imb_disp_init()
{
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    printf("\nili9325 init...");
    tft_init_9325();
    TFT_CS_EN();
    disp_dev.drive_mode = MODE_MCU;
    disp_dev.data_width = PORT_8BITS;
    disp_dev.out_format = FORMAT_RGB565;
    disp_dev.ncycle = TWO_CYCLE;
    disp_dev.para = (L0_PARA_CFG *)&lcd_ili9325_inch_para;
//    disp_dev.hook = NULL;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.bkcolor = 0xE28080;//0xE20095;//

    disp_dev.signal_polarity = DEFAULT_POSITIVE;
    disp_dev.dclk_freq = IMB_EXTERNAL;
    disp_dev.dclk_divider = USE_PLL_CLK|PLL0_CLK|DIVA_1|DIVB_8|DIVC_2;
//    disp_dev.lcd_ctrl = LCD_CTRL;

    memset(&gain,0x00,sizeof(gain));

    sdfile_t imbfile;
    if(sd_fopen("JL_IMB.BIN",&imbfile))
    {
        u8 *buf = malloc(imbfile.len);
        sd_fread(&imbfile,buf,imbfile.len);
        gain.y_offs = buf[0];
        gain.u_offs = buf[1];
        gain.v_offs = buf[2];
        gain.y_gain = buf[3];
        gain.u_gain = buf[4];
        gain.v_gain = buf[5];
        gain.r_offs = buf[6];
        gain.g_offs = buf[7];
        gain.b_offs = buf[8];
        gain.r_gain = buf[9];
        gain.g_gain = buf[10];
        gain.b_gain = buf[11];
        gain.r_coe0 = buf[12];
        gain.r_coe1 = buf[13];
    	gain.r_coe2 = buf[14];
        gain.g_coe0 = buf[15];
        gain.g_coe1 = buf[16];
        gain.g_coe2 = buf[17];
        gain.b_coe0 = buf[18];
        gain.b_coe1 = buf[19];
        gain.b_coe2 = buf[20];
        gain.r_gma  = buf[22]<<8|buf[21];
        gain.g_gma  = buf[24]<<8|buf[23];
        gain.b_gma  = buf[26]<<8|buf[25];
        free(buf);
    }
//    else
//    {
//        gain.y_offs = 0;
//        gain.y_gain = 0x80;
//        gain.u_gain = 0x80;
//        gain.v_gain = 0x80;
//        gain.r_gain = 0x80;
//        gain.g_gain = 0x80;
//        gain.b_gain = 0x80;
//        gain.r_coe0 = 0x40;
//        gain.g_coe1 = 0x40;
//        gain.b_coe2 = 0x40;
//    }
//
//    disp_dev.gain = &gain;
//    disp_dev.isr_prior = 3;

//    u16 r_clut,g_clut,b_clut;
//    sdfile_t lut_file;
//    if(!sd_fopen("lut.bin",&lut_file))
//            return FALSE;
//
//    u8 *r_tab = malloc(256);
//    u8 *g_tab = malloc(256);
//    u8 *b_tab = malloc(256);
//	r_clut = 150;
//	g_clut = 150;
//	b_clut = 150;
//    sd_fseek(&lut_file,SD_SEEK_SET,(r_clut-75)*256);
//    sd_fread(&lut_file,r_tab,256);
//    sd_fseek(&lut_file,SD_SEEK_SET,(g_clut-75)*256);
//    sd_fread(&lut_file,g_tab,256);
//    sd_fseek(&lut_file,SD_SEEK_SET,(b_clut-75)*256);
//    sd_fread(&lut_file,b_tab,256);


//    disp_dev.r_gma_tab = r_tab;
//    disp_dev.g_gma_tab = g_tab;
//    disp_dev.b_gma_tab = b_tab;

//    imb_init(&disp_dev);

//    free(r_tab);
//    free(g_tab);
//    free(b_tab);

//    memset(&gain,0x00,sizeof(gain));
//    gain.y_offs = -40;
//    gain.u_offs = 0x00;
//    gain.v_offs = 0x00;
//    gain.y_gain = 138;
//    gain.u_gain = 160;
//    gain.v_gain = 160;
//    gain.r_offs = 0x00;
//    gain.g_offs = -3;
//    gain.b_offs = -2;
//    gain.r_gain = 134;
//    gain.g_gain = 166;
//    gain.b_gain = 160;
//    gain.r_coe0 = 64;
//    gain.r_coe1 = 0x00;
//    gain.r_coe2 = 0x00;
//    gain.g_coe0 = 0x00;
//    gain.g_coe1 = 0x40;
//    gain.g_coe2 = 0x00;
//    gain.b_coe0 = 5;
//    gain.b_coe1 = 0x00;
//    gain.b_coe2 = 0x40;
//    gain.r_gma = 100;
//    gain.g_gma = 100;
//    gain.b_gma = 104;

    disp_dev.gain = &gain;
    disp_dev.isr_prior = 3;
    disp_dev.r_gma_tab = NULL;
    disp_dev.g_gma_tab = NULL;
    disp_dev.b_gma_tab = NULL;

    imb_init(&disp_dev);
    puts("\n 1111111111111111111111111111111111111111 \n");
//    imb_start();
//    while(1);
    return;

}

#endif
