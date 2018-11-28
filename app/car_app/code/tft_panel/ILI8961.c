#include "tft.h"
#include "imb_module.h"

#if (LCD_TYPE == LCD_ILI8961)
void lcd_io_init()
{
    TFT_CS_OUT();
    TFT_SDA_OUT();
    TFT_SCL_OUT();

    TFT_CS_DIS();
    TFT_SDA_H();
    TFT_SCL_H();
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
    TFT_CS_EN();
    TFT_SDA_OUT();
    delay_50ns(1);

    for(i=7;i>=0;i--)
    {
        TFT_SCL_L();
        delay_50ns(1);

        if(reg&BIT(7))
            TFT_SDA_H();
        else
            TFT_SDA_L();
        reg <<= 1;

        TFT_SCL_H();
        delay_50ns(1);
    }

    for(i=7;i>=0;i--)
    {
        TFT_SCL_L();
        delay_50ns(1);

        if(dat&BIT(7))
            TFT_SDA_H();
        else
            TFT_SDA_L();
        dat <<= 1;

        TFT_SCL_H();
        delay_50ns(1);
    }
    TFT_CS_DIS();
    delay_50ns(8);
}

u8 rd_reg(u8 reg)
{
    char i;
    u8 dat = 0;
    TFT_CS_EN();
    TFT_SDA_OUT();
    delay_50ns(1);

    for(i=7;i>=0;i--)
    {
        TFT_SCL_L();
        delay_50ns(1);

        if(reg&BIT(7))
            TFT_SDA_H();
        else
            TFT_SDA_L();
        reg <<= 1;

        TFT_SCL_H();
        delay_50ns(1);
    }

    TFT_SDA_IN();
    for(i=7;i>=0;i--)
    {
        TFT_SCL_L();
        delay_50ns(1);

        TFT_SCL_H();
        delay_50ns(1);

        dat <<= 1;
        if(TFT_SDA_R())
            dat |= BIT(0);

        delay_50ns(1);
    }
    TFT_CS_DIS();
    delay_50ns(8);
    return dat;
}

u8 reg_tab[]=
{
    0x00,0x01,0x03,0x04,0x06,0x07,0x08,0x0B,
    0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,
    0x16,0x17,0x18,0x19,0x1A,0x2B,0x2f,0x55,0x5a,0x56,0x57,
    0x61,
};

void ILI8961_initial(void)
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
    wr_reg(0x07,70);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == RGB_8BIT_DUMMY_320x240_NTSC)
    wr_reg(0x00,0x09);
    wr_reg(0x04,0x1f);//8-bit RGB dummy  320*240 NTSC
    wr_reg(0x06,0x15);
    wr_reg(0x07,240);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == RGB_8BIT_DUMMY_320x288_PAL)
    wr_reg(0x00,0x09);
    wr_reg(0x04,0x13);//8-bit RGB dummy  320*288 PAL
    wr_reg(0x06,0x15);
    wr_reg(0x07,240);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == RGB_8BIT_DUMMY_360x240_NTSC)
    wr_reg(0x00,0x09);
    wr_reg(0x04,0x2f);//8-bit RGB dummy  360*240 NTSC
    wr_reg(0x06,0x15);
    wr_reg(0x07,240);
    wr_reg(0x0c,0x46);
#elif (DRIVE_MODE == RGB_8BIT_DUMMY_360x288_PAL)
    wr_reg(0x00,0x09);
    wr_reg(0x04,0x23);//8-bit RGB dummy  360*288 PAL
    wr_reg(0x06,0x15);
    wr_reg(0x07,240);
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


    //////////////
//    wr_reg(0x61,0x01);//FBMODE = 1
//    wr_reg(0x2f,0x21);//VGH Volage
//    wr_reg(0x5a,0x00);//VGL Volage
    //////////////


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
    1716,
    1,
    70,
    320,

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
    1560,
    1,
    240,
    320,

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
    1560,
    1,
    240,
    320,

    313,
    1,
    23,
    24,
    288,
};
#endif

#if  (DRIVE_MODE == RGB_8BIT_DUMMY_360x240_NTSC)
const L0_PARA_CFG lcd_27_inch_para = //8-bit RGB + dummy 360*240 NTSC
{
    1716,
    1,
    240,
    360,

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
    1716,
    1,
    240,
    360,

    313,
    1,
    23,
    24,
    288,
};
#endif


#if  (DRIVE_MODE == YUV640x240_NTSC)
const L0_PARA_CFG lcd_27_inch_para =//YUV640 640x240 two clock
{
    1560,
    1,
    240,
    640,

    263,
    1,
    21,
    22,
    240,
};
#endif

#if (DRIVE_MODE == YUV640x288_PAL)
const L0_PARA_CFG lcd_27_inch_para =//YUV640 640x288 two clock
{
    1560,
    1,
    240,
    640,

    313,
    1,
    24,
    25,
    288,
};
#endif


#if  (DRIVE_MODE == YUV720x240_NTSC)
const L0_PARA_CFG lcd_27_inch_para =//YUV720 720x240 two clock
{
    1716,
    1,
    240,
    720,

    263,
    1,
    21,
    22,
    240,
};
#endif

#if (DRIVE_MODE == YUV720x288_PAL)
const L0_PARA_CFG lcd_27_inch_para =//YUV720 720x288 two clock
{
    1728,
    1,
    240,
    720,

    313,
    1,
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


void imb_disp_init()
{
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    ILI8961_initial();

    disp_dev.drive_mode = MODE_RGB_SYNC;
    disp_dev.data_width = PORT_8BITS;
#if (DRIVE_MODE == YUV720x240_NTSC)||(DRIVE_MODE == YUV640x240_NTSC)||(DRIVE_MODE == YUV720x288_PAL)||(DRIVE_MODE == YUV640x288_PAL)
    disp_dev.out_format = FORMAT_YUV422;
    disp_dev.ncycle = TWO_CYCLE;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE|DCLK_NEGATIVE;
    disp_dev.scan_mode = NON_INTERLACED;
#elif (DRIVE_MODE == RGB_8BIT)
    disp_dev.out_format = FORMAT_RGB888;
    disp_dev.ncycle = THREE_CYCLE;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE|DCLK_NEGATIVE;
    disp_dev.scan_mode = NON_INTERLACED;
#elif (DRIVE_MODE == RGB_8BIT_DUMMY_320x240_NTSC)||(DRIVE_MODE == RGB_8BIT_DUMMY_320x288_PAL)||(DRIVE_MODE == RGB_8BIT_DUMMY_360x240_NTSC)||(DRIVE_MODE == RGB_8BIT_DUMMY_360x288_PAL)
    disp_dev.out_format = FORMAT_RGB888;
    disp_dev.ncycle = FOUR_CYCLE;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE|DCLK_NEGATIVE;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.dclk_freq = IMB_INTERNAL|DCLK_27MHz;
    disp_dev.dclk_divider = 0;//PLL_CLK|PLL0_CLK|DIVA_5|DIVB_8|DIVC_1;
#else//CCIR656
    disp_dev.drive_mode = MODE_CCIR656;
    disp_dev.out_format = FORMAT_ITU656;
    disp_dev.ncycle = TWO_CYCLE;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE|DCLK_NEGATIVE;
    disp_dev.scan_mode = INTERLACED_TIMING;
#endif
    disp_dev.para = (L0_PARA_CFG *)&lcd_27_inch_para;
    disp_dev.hook = NULL;


    disp_dev.bkcolor = 0xE20095;

    memset(&gain,0x00,sizeof(gain));
    gain.y_offs = -40;
    gain.u_offs = 0x00;
    gain.v_offs = 0x00;
    gain.y_gain = 138;
    gain.u_gain = 160;
    gain.v_gain = 160;
    gain.r_offs = 0x00;
    gain.g_offs = -3;
    gain.b_offs = -2;
    gain.r_gain = 134;
    gain.g_gain = 166;
    gain.b_gain = 160;
    gain.r_coe0 = 64;
    gain.r_coe1 = 0x00;
    gain.r_coe2 = 0x00;
    gain.g_coe0 = 0x00;
    gain.g_coe1 = 0x40;
    gain.g_coe2 = 0x00;
    gain.b_coe0 = 5;
    gain.b_coe1 = 0x00;
    gain.b_coe2 = 0x40;
    gain.r_gma = 100;
    gain.g_gma = 100;
    gain.b_gma = 104;

    disp_dev.gain = &gain;
    disp_dev.isr_prior = 3;
    disp_dev.r_gma_tab = NULL;
    disp_dev.g_gma_tab = NULL;
    disp_dev.b_gma_tab = NULL;

    imb_init(&disp_dev);
/*
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
	memset(image->mask_buf,0xf0,320*100/8);
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
	osdwin->buf = malloc(224*80);
	memset(osdwin->buf,0x55,224*80);
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
	memset(osdwin->buf,0xaa,160*40);
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
	memset(osdwin->buf,0x00,160*60);
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
    */
}
#endif
