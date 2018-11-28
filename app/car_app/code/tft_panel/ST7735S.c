#include "typedef.h"
#include "pap.h"
#include "tft.h"
#include "dv12.h"
#include "imb_module.h"

#if (LCD_TYPE == LCD_ST7735S)

//ST735S控制器命令
#define MEMORY_READ     0X2E
#define MEMORY_WRITE    0X2C
#define ROW_ADDR_SET    0X2B
#define COLUMN_ADDR_SET 0X2A
#define MADCTL          0x36

#define HORIZONTAL  	0
#define VERTICAL    	1

u8 Tft_Dir;
u8 LCDBuff[8][128];

void tft_reset(void)
{
    TFT_RST_H();
    TFT_DELAY(5000); //延时
    TFT_RST_L();
    TFT_DELAY(5000); //延时
    TFT_RST_H();
    TFT_DELAY(5000); //延时
}

void tft_io_cfg()
{
    TFT_CS_OUT();
    TFT_RS_OUT();
    TFT_RST_OUT();
}

void tft_write_cmd(u8 cmd)
{
    TFT_CS_L();
    TFT_RS_L();
    pap_write(cmd);
    TFT_CS_H();
}

void tft_write_data(u16 data)
{
    TFT_CS_L();
    TFT_RS_H();
    pap_write(data);
    TFT_CS_H();
}

u8 tft_read_data()
{
    u8 dat;
    TFT_CS_L();
    TFT_RS_H();
    dat = pap_read();
    TFT_CS_H();
    return dat;
}

void tft_set_dir(u8 dir)
{
    Tft_Dir = dir;

    tft_write_cmd(MADCTL);
    if (dir == VERTICAL)
    {
        tft_write_data(0xc8);
        //tft_write_data(0x08);
    }
    else
    {
        tft_write_data(0xa8);
        //tft_write_data(0x28);
    }
}

u8 tft_get_dir(void)
{
    return (Tft_Dir);
}

//通过DMA写指定长度的数据到GRAM
void tft_WriteByDMA(void *pBuf, u16 len)
{
	TFT_CS_L();
	TFT_RS_H();
	pap_dma_write(pBuf, len);
}


//设置坐标
void tft_SetPos(u16 x, u16 y)
{
    x= x;
    y = y;
}

//设置窗口区域
void tft_SetRange(u16 x0, u16 x1, u16 y0, u16 y1)
{
    x0 = x0>0x7F?0x7F:x0;
    x1 = x1>0x7F?0x7F:x1;
    y0 = y0>0x9F?0x9F:y0;
    y1 = y1>0x9F?0x9F:y1;

    tft_write_cmd(COLUMN_ADDR_SET);
	tft_write_data(x0>>8);	        //起始位置的高8位
	tft_write_data(x0&0xff);        //起始位置的低8位
	tft_write_data(x1>>8);	        //结束位置的高8位
	tft_write_data(x1&0xff);        //结束位置的低8位

    tft_write_cmd(ROW_ADDR_SET);
	tft_write_data(y0>>8);	        //起始位置的高8位
	tft_write_data(y0&0xff);        //起始位置的低8位
	tft_write_data(y1>>8);	        //结束位置的高8位
	tft_write_data(y1&0xff);        //结束位置的低8位

    tft_write_cmd(MEMORY_WRITE);
    TFT_CS_EN();
    TFT_RS_H();

    putbyte('*');
}

u32 tft_Readid(u16 index)
{
    u32 ret = 0;
    tft_write_cmd(index);
    tft_read_data();
    ret = tft_read_data();
    ret |= tft_read_data()<<8;
    ret |= tft_read_data()<<16;
    return ret;
}

void tft_ClearArea(u16 color,u16 x0,u16 x1,u16 y0,u16 y1)
{
    tu16 i;
    tu16 width,height;
    u8 *p = (u8 *)LCDBuff;

    if((x1 < x0) || (y1 < y0))
       return;
    width = x1 - x0 + 1;
    height = y1 - y0 + 1;
    tft_SetRange(x0,x1,y0,y1);
    //tft_write_cmd(MEMORY_WRITE);

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
    TFT_CS_H();
}

/*
//通过DMA读TFT GRAM数据
static void tft_ReadByDMA(void *pBuf, u16 len)
{
    tft_write_cmd(MEMORY_READ);
    TFT_CS_L();
    TFT_RS_H();
    pap_read();   //dummy read
	pap_ReadByDMA(pBuf, len);
}

//读取指定坐标数据
static u16 tft_ReadOnePixel(u16 x, u16 y)
{
    u16 val = 0XFFFF;

    tft_SetRange(x, x,y,y);
    tft_write_cmd(MEMORY_READ);
    TFT_CS_L();
    TFT_RS_H();
    pap_read();   //dummy read
    val = pap_read();
    TFT_CS_H();

    return val;
}


static void tft_ReadMultiPixel(void *pBuf, u16 xs, u16 xe, u16 ys, u16 ye, u16 align)
{
    u32 i,j;
	u32 ret;
    u32 fr = 0;
    u16 *p = (u16 *)pBuf;

	tft_SetRange(xs, xe, ys, ye); //Set HAS/HEA,VSA/VEA

	tft_write_cmd(MEMORY_READ);
    TFT_CS_L();
    TFT_RS_H();
    pap_read();   //dummy read

	for(j = ys; j <= ye; j++)
	{
		fr = (j - ys) * align;
		for(i = xs; i <= xe; i++)
		{
			p[fr++] = pap_read();
		}
	}

	TFT_CS_H();
}
*/


/*
void tft_SetColor(u16 BackColor,u16 ForeColor)
{
    PAP_DAT0 = BackColor;
    PAP_DAT1 = ForeColor;
}

//PAP扩展模式 显示单个汉字字符
void tft_DispCharacter(void *buf,u16 x0,u16 x1,u16 y0,u16 y1)
{
    tft_SetRange(x0, x1, y0, y1);
    tft_write_cmd(MEMORY_WRITE);

    PAP_EXT_EN();//Enable EXTE
    tft_WriteByDMA(buf,32);
    PAP_WAIT();
    PAP_EXT_DIS();//Disable EXTE
}
*/


void tft_drv_init(void)
{
    u32 i;
    tft_write_cmd(0x11);//sleep out
    for(i=0;i<120;i++)
        TFT_DELAY(2500);//延时120ms,系统时钟48M

    tft_write_cmd(0xB1);//frame_rate=850KHZ/((RTNA*2+40) * (LINE+FPA+BPA+2) )
    tft_write_data(0x01);//RTNA
    tft_write_data(0x2C);//FPA5
    tft_write_data(0x2D);//BPA5

    tft_write_cmd(0xB2);//frame_rate=850KHZ/((RTNA*2+40) * (LINE+FPA+BPA+2) )
    tft_write_data(0x01);
    tft_write_data(0x2C);
    tft_write_data(0x2D);

    tft_write_cmd(0xB3);//frame_rate=850KHZ/((RTNA*2+40) * (LINE+FPA+BPA+2) )
    tft_write_data(0x01);
    tft_write_data(0x2C);
    tft_write_data(0x2D);
    tft_write_data(0x01);
    tft_write_data(0x2C);
    tft_write_data(0x2D);

    tft_write_cmd(0xB4);//display inversion control
    tft_write_data(0x07);//column inversion

    tft_write_cmd(0xC0);//power control
    tft_write_data(0xA2);//5V
    tft_write_data(0x02);
    tft_write_data(0x84);

    tft_write_cmd(0xC1);//power control
    tft_write_data(0xC5);

    tft_write_cmd(0xC2);//power control
    tft_write_data(0x0A);
    tft_write_data(0x00);

    tft_write_cmd(0xC3);//power control
    tft_write_data(0x8A);
    tft_write_data(0x2A);

    tft_write_cmd(0xC4);//power control
    tft_write_data(0x8A);
    tft_write_data(0xEE);

    tft_write_cmd(0xC5);//vcom control
    tft_write_data(0x0E);

/*
    tft_write_cmd(MADCTL);//memory data access control
    if (Tft_Dir == VERTICAL)
        //tft_write_data(0xE8);
        tft_write_data(0x28);
    else
        //tft_write_data(0x88);
        tft_write_data(0x08);
*/

    tft_write_cmd(0x20);//display inversion off
    tft_write_cmd(0xe0);
    tft_write_data(0x02);
    tft_write_data(0x1c);
    tft_write_data(0x07);
    tft_write_data(0x12);
    tft_write_data(0x37);
    tft_write_data(0x32);
    tft_write_data(0x29);
    tft_write_data(0x2d);
    tft_write_data(0x29);
    tft_write_data(0x25);
    tft_write_data(0x2b);
    tft_write_data(0x39);
    tft_write_data(0x00);
    tft_write_data(0x01);
    tft_write_data(0x03);
    tft_write_data(0x10);

    tft_write_cmd(0xe1);
    tft_write_data(0x03);
    tft_write_data(0x1d);
    tft_write_data(0x07);
    tft_write_data(0x06);
    tft_write_data(0x2e);
    tft_write_data(0x2c);
    tft_write_data(0x29);
    tft_write_data(0x2d);
    tft_write_data(0x2e);
    tft_write_data(0x2e);
    tft_write_data(0x37);
    tft_write_data(0x3f);
    tft_write_data(0x00);
    tft_write_data(0x00);
    tft_write_data(0x02);
    tft_write_data(0x10);
    tft_write_cmd(0x3a);//界面像素
    tft_write_data(0x05);//16-bit/pixel
    tft_write_cmd(0x29);//开显示

    u8 id[3];
    tft_write_cmd(0x04);//RDDID
    tft_read_data();
    id[0] = tft_read_data();
    id[1] = tft_read_data();
    id[2] = tft_read_data();
    printf("\nRDDID:");
    printf_buf(id,3);
}

//清屏
void tft_ClearScreen(u16 color)
{
    tu16 i;
    u8 *p = (u8 *)LCDBuff;

    printf("color:%x",color);
    tft_SetRange(0,TFT_WIDTH-1,0,TFT_HIGHT-1);
    tft_write_cmd(MEMORY_WRITE);
    for(i=0;i<TFT_WIDTH;i++)
    {
        p[2*i] = (u8)(color >> 8);
        p[2*i+1] = (u8)color;
    }

    for(i=0;i<TFT_HIGHT;i++)
    {
        tft_WriteByDMA(p,TFT_WIDTH*2);
    }
    PAP_WAIT_NCLR();
    TFT_CS_L();
}

void tft_init(void)
{
    u16 i;
    u32 id;

    tft_io_cfg();
    pap_init();

    puts("\ntft_init ");
    tft_reset();
    TFT_DELAY(80000); //延时
    tft_drv_init();
    tft_set_dir(VERTICAL);


    id = tft_Readid(4);
    puts("\nid:");
    put_u32hex(id);

    tft_write_cmd(0xDA);
    tft_read_data();
    i = tft_read_data();
    printf("\nDAH = %02x\n7",i);

    tft_ClearScreen(COLOUR_RED);
}

const L0_PARA_CFG lcd_128x160_para =
{
    128*2+20,
    20,
    20,
    128,

    160+20,
    20,
    20,
    0,
    160,
};

void imb_disp_init()
{
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    tft_init();

    disp_dev.drive_mode = MODE_MCU;
    disp_dev.data_width = PORT_8BITS;
    disp_dev.out_format = FORMAT_RGB565;
    disp_dev.para = (L0_PARA_CFG *)&lcd_128x160_para;
    disp_dev.hook = tft_SetRange;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.ncycle = TWO_CYCLE;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE;
    disp_dev.bkcolor = 0xE20095;

    memset(&gain,0x00,sizeof(gain));
    gain.y_offs = 0;
    gain.y_gain = 0x80;
    gain.u_gain = 0x80;
    gain.v_gain = 0x80;
    gain.r_gain = 0x80;
    gain.g_gain = 0x80;
    gain.b_gain = 0x80;
    gain.r_coe0 = 0x40;
    gain.g_coe1 = 0x40;
    gain.b_coe2 = 0x40;

    disp_dev.gain = &gain;
    disp_dev.isr_prior = 3;
    disp_dev.r_gma_tab = NULL;
    disp_dev.g_gma_tab = NULL;
    disp_dev.b_gma_tab = NULL;

    imb_init(&disp_dev);

    IMGLAYER *image= imb_get_image_hdl(LAYER_IMAGE0);
	image->enable = TRUE;
    image->alpha = 0;
    image->x = 0;
    image->y = 0;
    image->width = 128;
    image->height = 160;
	image->buf = 0x02800000;

	image->buf_num = 2;
	image->mask_en = FALSE;
	image->dsc_ram = NULL;
	imb_image_cfg(image);

    struct DSC_ADDR *curr_addr;
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);

    imb_start();
}

#endif
