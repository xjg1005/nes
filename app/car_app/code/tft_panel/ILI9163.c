#include "typedef.h"
#include "pap.h"
#include "tft.h"
#include "dv12.h"
#include "imb_module.h"

#if (LCD_TYPE == LCD_ILI9163)

void WriteCOM(u8 cmd)
{
    TFT_CS_EN();
    TFT_RS_L();//cmd
    pap_write(cmd);
    TFT_CS_DIS();
}

void WriteDAT_8(u8 dat)
{
    TFT_CS_EN();
    TFT_RS_H();//dat
    pap_write(dat);
    TFT_CS_DIS();
}

u8 ReadDAT_8()
{
    u8 dat;
    TFT_CS_EN();
    TFT_RS_H();//dat
    dat = pap_read();
    TFT_CS_DIS();
}

void tft_SetRange(u16 xs,u16 xe,u16 ys,u16 ye)
{
	WriteCOM(0x2A);
	WriteDAT_8(xs>>8);
	WriteDAT_8(xs);
	WriteDAT_8(xe>>8);
	WriteDAT_8(xe);
	WriteCOM(0x2B);
	WriteDAT_8(ys>>8);
	WriteDAT_8(ys);
	WriteDAT_8(ye>>8);
	WriteDAT_8(ye);
	WriteCOM(0x2C); //write to RAM

	TFT_CS_EN();
	TFT_RS_H();
//	putchar('x');
}

void tft_io_init()
{
    TFT_RST_OUT();
    TFT_RS_OUT();
    TFT_CS_OUT();
    TFT_RST_H();
    TFT_CS_H();
    TFT_RS_H();
}

void ILI9163_init(void)
{
    u32 id;
    u16 i,j;

    printf("\nILI9163 init...");
    tft_io_init();
    pap_init();

	TFT_RST_H();
	delay_2ms(5);
	TFT_RST_L();
	delay_2ms(5);
	TFT_RST_H();
	delay_2ms(5);

	WriteCOM(0x01);//software reset

//	WriteCOM(0xDA);
//	ReadDAT_8();
//	id = ReadDAT_8();//54h
//	printf("\nid = %x",id);
//
//	WriteCOM(0x04);//Read Display Identification Information
//	ReadDAT_8();
//	id = ReadDAT_8();//54h
//	id <<= 8;
//	id |= ReadDAT_8();//80h
//	id <<= 8;
//	id |= ReadDAT_8();//66h
//	printf("\nid = %06x",id);

	WriteCOM(0x11);//退出睡眠模式
	delay_2ms(50);

	WriteCOM(0x26);    //Gammut format 伽马曲线设置
	WriteDAT_8(0x04);  /*04h GC3 Gammut 2.2，01h GC1 Gammut 1.0 , 02h GC2 gammut 1.8, 08h GC3 Gammut 2.5 */
	delay_2ms(5);

	WriteCOM(0xB1);   //帧频控制
	WriteDAT_8(0x08);
	WriteDAT_8(0x0B);

	WriteCOM(0xC0);   //电源控制
	WriteDAT_8(0x10);
	WriteDAT_8(0x00);

	WriteCOM(0xC1);   //电源控制
	WriteDAT_8(0x03);

	WriteCOM(0xC5);   //偏压控制
	WriteDAT_8(0x42);  //vcom control 0x48,0x24 34 //2b
	WriteDAT_8(0x45);  //vcom control 0x48,0x24 50 //47

	WriteCOM(0xC7);    //偏压控制
	WriteDAT_8(0xC1);  //C4

	WriteCOM(0xEC);
	WriteDAT_8(0x0C);
	delay_2ms(5);

	WriteCOM(0x3A);    // Interface Pixel mode
	WriteDAT_8(0x05);  /*0x05 16 bit/Pixel RGB 53-35 Twice per pixel for 80-8bits parallel  0x06 18 bit/Pixel RGB 6-6-6 Three times per pixel for 80-8bits parallel */

	WriteCOM(0x36);    //扫描方向控制
	WriteDAT_8(/*0xA8*/0x18);//BIT(4)垂直翻转 0x08

	WriteCOM(0xB7);
	WriteDAT_8(0x00);

	WriteCOM(0xF2);
	WriteDAT_8(0x01);
	delay_2ms(5);

    //Gamma,伽马曲线设置
	WriteCOM(0xE0);  //Positive 05
	WriteDAT_8(0x3F);//p1
	WriteDAT_8(0x29);//p2
	WriteDAT_8(0x27);//p3
	WriteDAT_8(0x2C);//p4
	WriteDAT_8(0x27);//p5
	WriteDAT_8(0x0C);//p6
	WriteDAT_8(0x54);//p7
	WriteDAT_8(0xC7);//p8
	WriteDAT_8(0x40);//p9
	WriteDAT_8(0x19);//p10
	WriteDAT_8(0x17);//p11
	WriteDAT_8(0x1E);//p12
	WriteDAT_8(0x02);//p13
	WriteDAT_8(0x01);//p14
	WriteDAT_8(0x00);//p15

	WriteCOM(0xE1);  //Negative
	WriteDAT_8(0x00);//p1
	WriteDAT_8(0x16);//p2
	WriteDAT_8(0x18);//p3
	WriteDAT_8(0x13);//p4
	WriteDAT_8(0x18);//p5
	WriteDAT_8(0x13);//p6
	WriteDAT_8(0x2B);//p7
	WriteDAT_8(0x38);//p8
	WriteDAT_8(0x3F);//p9
	WriteDAT_8(0x06);//p10
	WriteDAT_8(0x18);//p11
	WriteDAT_8(0x21);//p12
	WriteDAT_8(0x3D);//p13
	WriteDAT_8(0x3E);//p14
	WriteDAT_8(0x3F);//p15
	WriteCOM(0x29);  //Display on

/*
    tft_SetRange(0,128-1,0,160-1);

	for(i=0;i<128;i++)
        for(j=0;j<160;j++)
        {
             WriteDAT_8(0xf8);
             WriteDAT_8(0x00);
        }
    delay_2ms(500);
	for(i=0;i<128;i++)
        for(j=0;j<160;j++)
        {
             WriteDAT_8(0x07);
             WriteDAT_8(0xe0);
        }
    delay_2ms(500);
	for(i=0;i<128;i++)
        for(j=0;j<160;j++)
        {
             WriteDAT_8(0x00);
             WriteDAT_8(0x3f);
        }
    delay_2ms(500);
    */
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

    ILI9163_init();

    disp_dev.drive_mode = MODE_MCU;
    disp_dev.data_width = PORT_8BITS;
    disp_dev.out_format = FORMAT_RGB565;
    disp_dev.para = (L0_PARA_CFG *)&lcd_128x160_para;
    disp_dev.hook = tft_SetRange;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.ncycle = TWO_CYCLE;
    disp_dev.signal_polarity = DEFAULT_POSITIVE;
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
