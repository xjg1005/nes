#include "ILI8960.h"
#include "imb_module.h"
#include "tft.h"

#if (LCD_TYPE == LCD_ILI8960)

void lcd_io_init()
{
    LCD_CS_OUT();
    LCD_SDA_OUT();
    LCD_SCL_OUT();

    LCD_CS_DIS();
    LCD_SDA_H();
    LCD_SCL_H();
}

void delay_50ns(u16 cnt)//380ns
{
    while(cnt--)
    {
        delay(8);//50ns
    }
}

void wr_reg(u8 reg,u8 dat)
{
    char i;
    LCD_CS_EN();
    LCD_SDA_OUT();
    delay_50ns(1);

    for(i=7;i>=0;i--)
    {
        LCD_SCL_L();
        delay_50ns(1);

        if(reg&BIT(7))
            LCD_SDA_H();
        else
            LCD_SDA_L();
        reg <<= 1;

        LCD_SCL_H();
        delay_50ns(1);
    }

    for(i=7;i>=0;i--)
    {
        LCD_SCL_L();
        delay_50ns(1);

        if(dat&BIT(7))
            LCD_SDA_H();
        else
            LCD_SDA_L();
        dat <<= 1;

        LCD_SCL_H();
        delay_50ns(1);
    }
    LCD_CS_DIS();
    delay_50ns(8);
}

u8 rd_reg(u8 reg)
{
    char i;
    u8 dat = 0;
    LCD_CS_EN();
    LCD_SDA_OUT();
    delay_50ns(1);

    for(i=7;i>=0;i--)
    {
        LCD_SCL_L();
        delay_50ns(1);

        if(reg&BIT(7))
            LCD_SDA_H();
        else
            LCD_SDA_L();
        reg <<= 1;

        LCD_SCL_H();
        delay_50ns(1);
    }

    LCD_SDA_IN();
    for(i=7;i>=0;i--)
    {
        LCD_SCL_L();
        delay_50ns(1);

        LCD_SCL_H();
        delay_50ns(1);

        dat <<= 1;
        if(LCD_SDA_R())
            dat |= BIT(0);

        delay_50ns(1);
    }
    LCD_CS_DIS();
    delay_50ns(8);
    return dat;
}

u8 reg_tab[]=
{
    0x00,0x01,0x03,0x04,0x06,0x07,0x08,0x0B,
    0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,
    0x16,0x17,0x18,0x19,0x1A,0x2B,0x55,0x56,0x57,
    0x61,
};

void ILI8960_initial(void)
{
    u8 i;

    lcd_io_init();

	wr_reg(0x05,0x5F);//GRB STB
	delay_2ms(5);
	wr_reg(0x05,0x1F);//GRB STB  Reset All Regesiter
	delay_2ms(25);
	wr_reg(0x05,0x5F);//GRB STB
	delay_2ms(50);

	wr_reg(0x00,0x09);//VCOMAC  00   04
	wr_reg(0x01,0x92);//VCOMDC  95   92
	wr_reg(0x03,0x40);//brightness setting
	wr_reg(0x04,0x0F);//8-bit RGB interface,VDIR/HDIR   0B
	wr_reg(0x06,0x15);
	wr_reg(0x07,0x46);
	wr_reg(0x08,0x00);
	wr_reg(0x0B,0x83);
	wr_reg(0x0C,0x06);

	wr_reg(0x0D,0x40);//contrast setting
	wr_reg(0x0E,0x40);
	wr_reg(0x0F,0x40);
	wr_reg(0x10,0x40);
	wr_reg(0x11,0x40);

	wr_reg(0x12,0x00);
	wr_reg(0x13,0x01);//Delta color filter   01

	wr_reg(0x16,0x04);//Gamma setting  2.2
	wr_reg(0x17,0x54);
	wr_reg(0x18,0x54);
	wr_reg(0x19,0x43);
	wr_reg(0x1A,0x54);

	wr_reg(0x95,0x00);
	wr_reg(0x96,0x03);
	wr_reg(0x97,0x02);
	wr_reg(0xa1,0x00);

	//////////////////////////
#if (DRIVE_MODE == RGB_8BIT)
    wr_reg(0x00,0x09);
    wr_reg(0x04,0x0f);//8-bit RGB
    wr_reg(0x06,0x15);
    wr_reg(0x07,24*3);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == RGB_8BIT_DUMMY_320x240_NTSC)
    wr_reg(0x00,0x09);
    wr_reg(0x04,0x1f);//8-bit RGB dummy  320*240 NTSC
    wr_reg(0x06,0x15);
    wr_reg(0x07,60*4);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == RGB_8BIT_DUMMY_320x288_PAL)
    wr_reg(0x00,0x09);
    wr_reg(0x04,0x13);//8-bit RGB dummy  320*288 PAL
    wr_reg(0x06,0x15);
    wr_reg(0x07,60*4);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == RGB_8BIT_DUMMY_360x240_NTSC)
    wr_reg(0x00,0x09);
    wr_reg(0x04,0x2f);//8-bit RGB dummy  360*240 NTSC
    wr_reg(0x06,0x15);
    wr_reg(0x07,60*4);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == RGB_8BIT_DUMMY_360x288_PAL)
    wr_reg(0x00,0x09);
    wr_reg(0x04,0x23);//8-bit RGB dummy  360*288 PAL
    wr_reg(0x06,0x15);
    wr_reg(0x07,60*4);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == YUV720x240_NTSC)
	wr_reg(0x00,0x49);//CCIR601
	wr_reg(0x04,0x6f);//YUV720
	wr_reg(0x06,0x15);
	wr_reg(0x07,240);
	wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == YUV720x288_PAL)
 	wr_reg(0x00,0x49);//CCIR601
	wr_reg(0x04,0x63);//YUV720
	wr_reg(0x06,0x15);
	wr_reg(0x07,240);
	wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == YUV640x240_NTSC)
    wr_reg(0x00,0x49);//CCIR601
	wr_reg(0x04,0x4f);//YUV640
	wr_reg(0x06,0x15);
	wr_reg(0x07,240);
	wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == YUV640x288_PAL)
    wr_reg(0x00,0x49);//CCIR601
	wr_reg(0x04,0x43);//YUV640
	wr_reg(0x06,0x15);
	wr_reg(0x07,240);
	wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == CCIR656_PAL)
    wr_reg(0x00,0x09);//C601_EN = 0
    wr_reg(0x04,0x43);//CCIR656 PAL
    wr_reg(0x06,0x15);
    wr_reg(0x07,240);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == CCIR656_NTSC)
    wr_reg(0x00,0x09);//C601_EN = 0
    wr_reg(0x04,0x47);//CCIR656 NTSC
    wr_reg(0x06,0x15);
    wr_reg(0x07,240);
    wr_reg(0x0c,0x46);
#endif

	wr_reg(0x2B,0x01);//Exit STB mode and Auto display ON
	//wr_reg(0x2B,0x00);//Enter STB mode

    for(i=0;i<sizeof(reg_tab)/sizeof(reg_tab[0]);i++)
    {
        u8 reg = reg_tab[i];
        reg &=~BIT(7);
        reg |= (reg&BIT(6))<<1;
        reg |= BIT(6);
        printf("\nreg:%02x val:%02x %02x",reg_tab[i],rd_reg(reg),reg);
    }
}



#if (DRIVE_MODE == RGB_8BIT)
const L0_PARA_CFG lcd_27_inch_para = //8-bit RGB
{
    1716,//1716/3
    1,
    72,//72/3
    320,//960/3

    263,
    1,
    21,
    21,
    240,
};
#endif

#if (DRIVE_MODE == RGB_8BIT_DUMMY_320x240_NTSC)
const L0_PARA_CFG lcd_27_inch_para = //8-bit RGB + dummy 320*240 NTSC
{
    1560,//1560/4,
    1,
    240,//240/4,
    320,//1280/4,

    263,
    1,
    21,
    21,
    240,
};
#endif

#if  (DRIVE_MODE == RGB_8BIT_DUMMY_320x288_PAL)
const L0_PARA_CFG lcd_27_inch_para = //8-bit RGB + dummy 320*240 PAL
{
    1560,//1560/4,
    1,
    240,//240/4,
    320,//1280/4,

    313,//263,
    1,
    23,//21,
    24,//21,
    288,//240,
};
#endif

#if  (DRIVE_MODE == RGB_8BIT_DUMMY_360x240_NTSC)
const L0_PARA_CFG lcd_27_inch_para = //8-bit RGB + dummy 360*240 NTSC
{
    1716,//1716/4,
    1,
    240,//240/4,
    360,//1440/4,

    263,
    1,
    21,
    21,
    240,
};
#endif

#if  (DRIVE_MODE == RGB_8BIT_DUMMY_360x288_PAL)
const L0_PARA_CFG lcd_27_inch_para = //8-bit RGB + dummy 360*240 PAL
{
    1716,//1716/4,
    1,
    240,//240/4,
    360,//1440/4,

    313,//263,
    1,
    23,//21,
    24,//21,
    288,//240,
};
#endif

#if  (DRIVE_MODE == YUV640x240_NTSC)
const L0_PARA_CFG lcd_27_inch_para =//YUV640 640x240 two clock
{
    1560,//1560/2,
    1,
    240,//240/2,
    640,//1280/2,

    263,
    1,//42,
    21,
    22,
    240,
};
#endif

#if (DRIVE_MODE == YUV640x288_PAL)
const L0_PARA_CFG lcd_27_inch_para =//YUV640 640x288 two clock
{
    1560,//1560/2,
    1,
    240,//240/2,
    640,//1280/2,

    313,
    1,//42,
    24,
    25,
    288,
};
#endif


#if  (DRIVE_MODE == YUV720x240_NTSC)
const L0_PARA_CFG lcd_27_inch_para =//YUV720 720x240 two clock
{
    1716,//1716/2,
    1,
    240,//240/2,
    720,//1440/2,

    263,
    1,//42,
    21,
    22,
    240,
};
#endif

#if (DRIVE_MODE == YUV720x288_PAL)
const L0_PARA_CFG lcd_27_inch_para =//YUV720 720x288 two clock
{
    1728,//1728/2,
    1,
    240,//240/2,
    720,//1440/2,

    313,
    1,//42,
    24,
    25,
    288,
};
#endif

#if (DRIVE_MODE == CCIR656_PAL)
const L0_PARA_CFG lcd_27_inch_para =//YUV720 720x288 two clock
{
    1728,
    138*2,
    140*2,
    720,

    313,
    1,
    23,
    24,
    288,
};
#endif

#if (DRIVE_MODE == CCIR656_NTSC)
const L0_PARA_CFG lcd_27_inch_para =
{
    1716,
    268-4,
    268,
    720,

    263,
    1,
    21,
    22,
    240,
};
#endif

/*
e.g
void video_disp(void *p)
{
}
REGISTER_IMB_WORK(video_disp,NULL);
*/


/******************************************
ILI8960接线说明
CS	-> PF12
SDA	-> PF13
SCL	-> PF14
HSYNC  -> PH4
VSYNC  -> PG4
DCLK   -> PH5
DB7	-> R5(PE7)
DB6	-> R4(PF7)
DB5	-> R3(PC0)
DB4	-> R2(PD0)
DB3	-> G7(PE2)
DB2	-> G6(PF2)
DB1	-> G5(PE3)
DB0	-> G4(PF3)
*******************************************/

void imb_disp_init()
{
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    ILI8960_initial();

    disp_dev.drive_mode = MODE_RGB_SYNC;
    disp_dev.data_width = PORT_8BITS;
#if (DRIVE_MODE == YUV720x240_NTSC)||(DRIVE_MODE == YUV640x240_NTSC)||(DRIVE_MODE == YUV720x288_PAL)||(DRIVE_MODE == YUV640x288_PAL)
    disp_dev.out_format = FORMAT_YUV422;
    disp_dev.ncycle = TWO_CYCLE;
    disp_dev.signal_polarity = HSYNC_NEGATIVE|VSYNC_NEGATIVE|DCLK_NEGATIVE;
#elif (DRIVE_MODE == RGB_8BIT)
    disp_dev.out_format = FORMAT_RGB888;
    disp_dev.ncycle = THREE_CYCLE;
    disp_dev.signal_polarity = HSYNC_NEGATIVE|VSYNC_NEGATIVE|DCLK_NEGATIVE;
#elif (DRIVE_MODE == CCIR656_PAL) || (DRIVE_MODE == CCIR656_NTSC)
    disp_dev.out_format = FORMAT_ITU656;
    disp_dev.ncycle = TWO_CYCLE;
    disp_dev.signal_polarity = HSYNC_NEGATIVE|VSYNC_NEGATIVE|DCLK_NEGATIVE;
#else
    disp_dev.out_format = FORMAT_RGB888;
    disp_dev.ncycle = FOUR_CYCLE;
    disp_dev.signal_polarity = HSYNC_NEGATIVE|VSYNC_NEGATIVE|DCLK_NEGATIVE;
#endif
    disp_dev.para = (L0_PARA_CFG *)&lcd_27_inch_para;
    disp_dev.hook = NULL;
    disp_dev.scan_mode = NON_INTERLACED;

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
    image->width = 320;
    image->height = 240;
	image->buf = 0x02800000;

	image->buf_num = 2;
	image->mask_en = FALSE;
	image->dsc_ram = NULL;
	imb_image_cfg(image);

    struct DSC_ADDR *curr_addr;
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);
	//----------------------------------------------
    image= imb_get_image_hdl(LAYER_IMAGE1);
	image->enable = TRUE;
    image->alpha = 0;
    image->x = 0;
    image->y = 0;
    image->width = 320;
    image->height = 240;
	image->buf = 0x0281C200;

	image->buf_num = 2;
	image->mask_en = FALSE;
	image->mask_buf = malloc(320*240/8);
	memset(image->mask_buf,0xf0,320*120/8);
	memset(image->mask_buf+320*120/8,0x03,320*120/8);
	image->dsc_ram = NULL;
	imb_image_cfg(image);

    //struct DSC_ADDR *curr_addr;
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);
    //----------------------------------------------
	OSDWIN *osdwin = imb_get_osd_hdl(LAYER_OSD0_WIN0);
	osdwin->enable = FALSE;
	osdwin->alpha = 30;
	osdwin->x = 0;
	osdwin->y = 0;
	osdwin->width = 120;
	osdwin->height = 80;
	osdwin->buf = malloc(120*80);
	memset(osdwin->buf,0x05,120*80);
	imb_osd_win_cfg(osdwin);
    //----------------------------------------------
	osdwin = imb_get_osd_hdl(LAYER_OSD0_WIN1);
	osdwin->enable = FALSE;
	osdwin->alpha = 35;
	osdwin->x = 20;
	osdwin->y = 80;
	osdwin->width = 120;
	osdwin->height = 40;
	osdwin->buf = malloc(120*40);
	memset(osdwin->buf,240,120*40);
	imb_osd_win_cfg(osdwin);
    //----------------------------------------------
	osdwin = imb_get_osd_hdl(LAYER_OSD0_WIN2);
	osdwin->enable = FALSE;
	osdwin->alpha = 25;
	osdwin->x = 20;
	osdwin->y = 120;
	osdwin->width = 220;
	osdwin->height = 60;
	osdwin->buf = malloc(220*60);
	memset(osdwin->buf,31,220*60);
	imb_osd_win_cfg(osdwin);
    //----------------------------------------------
	osdwin = imb_get_osd_hdl(LAYER_OSD1_WIN0);
	osdwin->enable = FALSE;
	osdwin->alpha = 20;
	osdwin->x = 20;
	osdwin->y = 20;
	osdwin->width = 224;//224/32 = 7 32align
	osdwin->height = 80;
	osdwin->buf = malloc(224*80/4);
	memset(osdwin->buf,0x55,224*80/4);
	imb_osd_win_cfg(osdwin);
    //----------------------------------------------
	osdwin = imb_get_osd_hdl(LAYER_OSD1_WIN1);
	osdwin->enable = FALSE;
	osdwin->alpha = 30;
	osdwin->x = 80;
	osdwin->y = 100;
	osdwin->width =160;//160/32 = 5
	osdwin->height = 40;
	osdwin->buf = malloc(160*40);
	memset(osdwin->buf,0xaa,160*40/4);
	imb_osd_win_cfg(osdwin);
    //----------------------------------------------
	osdwin = imb_get_osd_hdl(LAYER_OSD1_WIN2);
	osdwin->enable = FALSE;
	osdwin->alpha = 30;
	osdwin->x = 60;
	osdwin->y = 140;
	osdwin->width = 160;//160/32 = 5
	osdwin->height = 60;
	osdwin->buf = malloc(160*60);
	memset(osdwin->buf,0x00,160*60/4);
	imb_osd_win_cfg(osdwin);
    //----------------------------------------------

    printf("\nimb_clk_con:%08x",imb_clk_con);
    printf("\nimb_l6_con:%08x",imb_l6_con);
    printf("\nimb_l6_fmt:%08x",imb_l6_fmt);
    printf("\nimb_emi_con:%08x",imb_emi_con);
    printf("\nimb_l1_con:%08x",imb_l1_con);
    printf("\nimb_l2_con:%08x",imb_l2_con);
    printf("\nimb_l3_con:%08x",imb_l3_con);
    printf("\nimb_l4_con:%08x",imb_l4_con);
    printf("\nimb_io_con:%08x",imb_io_con);
    printf("\nPORTG_DIR:%08x",PORTG_DIR);
    printf("\nPORTH_DIR:%08x",PORTH_DIR);

    imb_start();
}

#endif
