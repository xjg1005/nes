#include "typedef.h"
#include "pap.h"
#include "tft.h"
#include "dv12.h"
#include "ILI8960.h"
#include "imb_module.h"
#include "tft.h"

#if (LCD_TYPE == LCD_OTA5182A)

static void delay_50ns(u16 cnt)//380ns
{
    while(cnt--)
    {
        delay(/*8*/80);//50ns
    }
}

/*
void wr_reg(u16 para,u16 dat)
{
    char i;
    TFT_CS_EN();
    TFT_SDA_OUT();
    delay_50ns(1);

    for(i=15;i>=0;i--)  //write addres
    {
        TFT_SCL_L();
        delay_50ns(1);

        if(para&BIT(15))
            TFT_SDA_H();
        else
            TFT_SDA_L();
        para <<= 1;

        TFT_SCL_H();
        delay_50ns(1);
    }
    TFT_CS_DIS();
}
*/


static void wr_reg(u8 reg,u16 dat)
{
    char i;
    u8 addr = 0;

    TFT_CS_EN();
    TFT_SDA_OUT();
    TFT_SCL_OUT();
    delay_50ns(1);

    addr = (reg<<1);//write addr

    for(i=4;i>=0;i--)
    {
        TFT_SCL_L();
        delay_50ns(1);
        //addr <<= 1;

        if(addr&BIT(i))
            TFT_SDA_H();
        else
            TFT_SDA_L();

        TFT_SCL_H();
        delay_50ns(1);
    }

    //dat <<= 5;

    for(i=10;i>=0;i--)
    {
        TFT_SCL_L();
        delay_50ns(1);

        if(dat&BIT(i))
            TFT_SDA_H();
        else
            TFT_SDA_L();
        //dat <<= 1;
        //delay_50ns(1);

        TFT_SCL_H();
        delay_50ns(1);
    }
    TFT_CS_DIS();
    delay_50ns(8);

}


static u16 rd_reg(u8 reg)
{
    char i;
    u8 addr;
    u16 dat = 0;
    TFT_CS_EN();
    TFT_SDA_OUT();
    delay_50ns(1);

    addr = (reg<<4)|BIT(3);//read addr

    for(i=4;i>=0;i--)
    {
        TFT_SCL_L();
        delay_50ns(1);

        if(addr&BIT(7))
            TFT_SDA_H();
        else
            TFT_SDA_L();
        addr <<= 1;

        TFT_SCL_H();
        delay_50ns(1);
    }

    TFT_SDA_IN();
    for(i=10;i>=0;i--)
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

void tft_init(void)
{
    u16 dat[16];
    u8 i;
    printf("\nT15P00 init...");

    TFT_CS_OUT();
    TFT_SDA_OUT();
    TFT_SCL_OUT();

    TFT_CS_DIS();
    TFT_SDA_H();
    TFT_SCL_H();

    TFT_RESET_H();
    delay_2ms(50);
    TFT_RESET_L();
    delay_2ms(50);
    TFT_RESET_H();

    wr_reg(0x00,0x0f);
    wr_reg(0x00,0x05);
    delay_2ms(50);

    wr_reg(0x00,0x0f);
    wr_reg(0x00,0x05);
    delay_2ms(50);
    wr_reg(0x00,0x0f);
    //wr_reg(0x0f,0x546);
    //wr_reg(0x09,0x60);
    wr_reg(0x05,0x00);
    delay_2ms(50);
    wr_reg(0x03,0x08);
    wr_reg(0x07,0x40);


#ifdef RAW_MODE
    wr_reg(0x06,0x00);
#endif

#ifdef RGB_MODE
    #if (RGB_FORMAT == RGB_320x288_PAL)
    wr_reg(0x06,0x19);
    #elif (RGB_FORMAT == RGB_360x288_PAL)
    wr_reg(0x06,0x1a);
    #elif (RGB_FORMAT == RGB_320x240_NTSC)
    wr_reg(0x06,0x01);
    #elif (RGB_FORMAT == RGB_360x240_NTSC)
    wr_reg(0x06,0x02);
    #endif
    wr_reg(0x08,0x04);
    wr_reg(0x0a,0x04);
#endif

#ifdef CCIR_MODE
    wr_reg(0x0f,0x546);
    wr_reg(0x09,0x60);

    wr_reg(0x0c,0x05);
    wr_reg(0x0e,0x11);
    //---CCIR656---
    wr_reg(0x06,0x07);
    wr_reg(0x08,0x01);
    wr_reg(0x0a,0x20);
    //-------------
    wr_reg(0x07,0x40);
#endif
    //wr_reg(0x08,0x3);
    //wr_reg(0x07,0x20);

 //   wr_reg(0x05,0x20);
//    wr_reg(0x02,0x3);

//    wr_reg(0x04,0x00);//select scan direction 水平垂直翻转

    for(i=0;i<16;i++)
    {
        printf("\nreg %02d: %02x",i,rd_reg(i));
    }
}

static const u32 palette_yuv[256] =
{
	//stard palette table
	0xFF8080,0xF96684,0xF34D88,0xEE348C,0xE81A91,0xE20095,0xE19195,0xDB779A,
	0xD55E9E,0xD044A2,0xCA2BA6,0xC411AA,0xC3A2AB,0xBD88AF,0xB76FB3,0xB255B7,
	0xAC3CBB,0xA622BF,0xA5B3C0,0x9F99C4,0x9A80C8,0x9466CC,0x8E4DD1,0x8833D5,
	0x87C4D5,0x81AADA,0x7C91DE,0x7677E2,0x705EE6,0x6A44EA,0x69D4EB,0x64BBEF,
	0x5EA1F3,0x5888F7,0x526EFB,0x4C55FF,0xF08966,0xEA6F6B,0xE4566F,0xDE3C73,
	0xD82377,0xD3097B,0xD29A7C,0xCC8080,0xC66684,0xC04D88,0xBB348C,0xB51A91,
	0xB4AA91,0xAE9195,0xA8779A,0xA25E9E,0x9D44A2,0x972BA6,0x96BBA7,0x90A2AB,
	0x8A88AF,0x846FB3,0x7F55B7,0x793CBB,0x78CCBC,0x72B3C0,0x6C99C4,0x6780C8,
	0x6166CC,0x5B4DD1,0x5ADDD1,0x54C4D5,0x4EAADA,0x4991DE,0x4377E2,0x3D5EE6,
	0xE1914D,0xDB7851,0xD55E55,0xCF4559,0xC92B5E,0xC31262,0xC3A262,0xBD8966,
	0xB76F6B,0xB1566F,0xAB3C73,0xA52377,0xA5B378,0x9F9A7C,0x998080,0x936684,
	0x8D4D88,0x88348C,0x87C48D,0x81AA91,0x7B9195,0x75779A,0x6F5E9E,0x6F5E9E,
	0x69D5A2,0x63BBA7,0x5DA2AB,0x5788AF,0x516FB3,0x4C55B7,0x4BE6B8,0x45CCBC,
	0x3FB3C0,0x3999C4,0x3480C8,0x3480C8,0xD19A34,0xCB8038,0xC6673C,0xC04D40,
	0xBA3444,0xB41A48,0xB3AB49,0xAE914D,0xA87851,0xA25E55,0x9C4559,0x962B5E,
	0x95BC5E,0x90A262,0x8A8966,0x846F6B,0x7E566F,0x783C73,0x77CC74,0x72B378,
	0x6C9A7C,0x668080,0x606684,0x5A4D88,0x5ADD89,0x54C48D,0x4EAA91,0x489195,
	0x42779A,0x3C5E9E,0x3CEE9E,0x36D5A2,0x30BBA7,0x2AA2AB,0x2488AF,0x1E6FB3,
	0xC2A21A,0xBC891E,0xB66F22,0xB15626,0xAB3C2B,0xA5232F,0xA4B32F,0x9E9A34,
	0x988038,0x93673C,0x8D4D40,0x873444,0x873444,0x80AB49,0x7B914D,0x757851,
	0x6F5E55,0x694559,0x68D55A,0x62BC5E,0x5DA262,0x578966,0x516F6B,0x4B566F,
	0x4AE66F,0x44CC74,0x3FB378,0x399A7C,0x338080,0x2D6684,0x2CF785,0x27DD89,
	0x21C48D,0x1BAA91,0x159195,0x0F779A,0xB3AB00,0xAD9205,0xA77809,0xA15F0D,
	0x9B4511,0x962C15,0x95BC16,0x8FA21A,0x89891E,0x836F22,0x7E5626,0x783C2B,
	0x77CD2B,0x71B32F,0x6B9A34,0x658038,0x60673C,0x5A4D40,0x59DE41,0x53C445,
	0x4DAB49,0x48914D,0x427851,0x3C5E55,0x3BEF56,0x35D55A,0x2FBC5E,0x2AA262,
	0x248966,0x1E6F6B,0x1DFF6B,0x17E66F,0x11CC74,0x0CB378,0x069A7C,0x4758F7,
	0x425BEE,0x3860DE,0x3363D5,0x2969C4,0x246CBC,0x1972AA,0x1475A2,0x0A7A91,
	0x057D88,0x8C311C,0x823723,0x6E4232,0x644839,0x505347,0x46594E,0x32645C,
	0x286964,0x147572,0x0A7A79,0x1BF76D,0x19EE6E,0x15DE71,0x13D572,0x10C475,
	0x0EBC76,0x0AAA79,0x08A27A,0x04917D,0x02887F,0xEE8080,0xDD8080,0xBB8080,
	0xAA8080,0x888080,0x778080,0x558080,0x448080,0x228080,0x118080,0x008080,
};


#ifdef CCIR_MODE
const L0_PARA_CFG lcd_ota_inch_para =
{
//    1716/2,
//    1,
//    268/2,
//    1440/2,
//
//    273,
//    1,
//    20,
//    20,
//    240,

    1716,///2,
    268,///2,
    272,///2,
    720,

    263,
    42,
    20,
    20,
    240,
};
void lcdpanel_ota5182a_init()
{
    printf("\nota5182a...");
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    disp_dev.drive_mode = MODE_CCIR656;
    disp_dev.data_width = PORT_8BITS;
    disp_dev.out_format = FORMAT_ITU656;//FORMAT_ITU656;
    disp_dev.para = (L0_PARA_CFG *)&lcd_ota_inch_para;
    disp_dev.hook = NULL;
    disp_dev.ncycle = TWO_CYCLE;
    disp_dev.scan_mode = INTERLACED_ALL;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE;

    disp_dev.bkcolor = palette_yuv[235];//0xE20095;//0x5a8080;

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
    image->width = 720;//480;
    image->height = 480;//272;
	image->buf = 0x03000000;

	image->buf_num = 2;
	image->mask_en = FALSE;
	image->dsc_ram = NULL;

	imb_image_cfg(image);
    struct DSC_ADDR *curr_addr;
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);

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

#ifdef RGB_MODE

#if (RGB_FORMAT == RGB_320x240_NTSC)
const L0_PARA_CFG lcd_ota_inch_para =
{
    1560,///4,
    1,
    252,///4,
    320,///4,

    262,
    1,
    13,
    13,
    240
};
#elif (RGB_FORMAT == RGB_320x288_PAL)
const L0_PARA_CFG lcd_ota_inch_para =
{
    1560,///4,
    1,
    252,///4,
    320,///4,

    312,
    1,
    13,
    13,
    288
};
#elif (RGB_FORMAT == RGB_360x240_NTSC)
const L0_PARA_CFG lcd_ota_inch_para =
{
    1716,///4,
    1,
    252,///4,
    360,///4,

    262,
    1,
    13,
    13,
    240
};
#elif (RGB_FORMAT == RGB_360x288_PAL)
const L0_PARA_CFG lcd_ota_inch_para =
{
    1716,///4,
    1,
    252,///4,
    360,///4,

    312,
    1,
    13,
    13,
    288
};
#endif

void lcdpanel_ota5182a_init()
{
    printf("\nota5182a...");
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    disp_dev.drive_mode = MODE_RGB_SYNC;
    disp_dev.data_width = PORT_8BITS;
    disp_dev.out_format = FORMAT_RGB888;//FORMAT_ITU656;
    disp_dev.para = (L0_PARA_CFG *)&lcd_ota_inch_para;
    disp_dev.hook = NULL;
    disp_dev.ncycle = FOUR_CYCLE;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE;

    disp_dev.bkcolor = 0xE20095;//0x5a8080;

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
    image->width = 320;//480;
    image->height = 240;//272;
	image->buf = 0x03000000;

	image->buf_num = 2;
	image->mask_en = FALSE;
	image->dsc_ram = NULL;

	imb_image_cfg(image);
    struct DSC_ADDR *curr_addr;
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);

    imb_start();
}
#endif

#ifdef RAW_MODE

const L0_PARA_CFG lcd_ota_inch_para =
{
    617,
    1,
    100,
    480,

    262,
    1,
    13,
    14,
    240,
};
void lcdpanel_ota5182a_init()
{
    printf("\nota5182a...");
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    disp_dev.drive_mode = MODE_RGB_SYNC;
    disp_dev.data_width = PORT_8BITS;
    disp_dev.out_format = FORMAT_RAWPOS;
    disp_dev.raw_phase = EVEN_PHASE_R|ODD_PHASE_G;
//    disp_dev.out_format = FORMAT_RAWNEG;
//    disp_dev.raw_phase = EVEN_PHASE_R|ODD_PHASE_B;

    disp_dev.para = (L0_PARA_CFG *)&lcd_ota_inch_para;
    disp_dev.hook = NULL;
    disp_dev.ncycle = ONE_CYCLE;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE|DCLK_NEGATIVE;

    disp_dev.bkcolor = palette_yuv[5];//0xE20095;//0x5a8080;

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

//    gain.r_offs = 0x00;
//    gain.g_offs = 0x7f;
//    gain.b_offs = 0x00;

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
    image->width = 480;//480;
    image->height = 240;//272;
	image->buf = 0x03000000;

	image->buf_num = 2;
	image->mask_en = FALSE;
	image->dsc_ram = NULL;

	imb_image_cfg(image);
    struct DSC_ADDR *curr_addr;
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);


    imb_start();
}
#endif

void imb_disp_init()
{
    tft_init();
    lcdpanel_ota5182a_init();
}

#endif

