#include "tft.h"
#include "pap.h"
#include "imb_module.h"

/*
8位接口
16bit PAP8BIT  RGB565 TWO_CYCLE   PORT_8BITS 8bitsx2
18bit PAP8BIT  RGB666 THREE_CYCLE PORT_6BITS 6bitsx3 DB[7:2]

16位接口
16bit PAP16BIT 小端  RGB565 ONE_CYCLE 16bitsx1
16bit PAP16BIT 大端  RGB565 TWO_CYCLE 8bitsx2
18bit PAP16BIT 大端  RGB666 THREE_CYCLE 6bitsx3 DB[7:2]

18位接口
18bit PAP16BIT 小端  RGB666 ONE_CYCLE 18bitsx1
16bit PAP16BIT 大端  RGB565 TWO_CYCLE PORT_8BITS 8bitsx2
16bit PAP16BIT 小端  RGB565 ONE_CYCLE 16bitsx1

9位接口
16bit RGB565 TWO_CYCLE 8bitsx2
18bit RGB666 TWO_CYCLE 9bitsx2

IM0 IM1
 L   L		18bit
 L   H		16bit
 H   L		9bit
 H   H		8bit


reg 00H = 0x0a
reg 01H = 0x04
reg 10H = 0xf0
reg 11H = 0x90


RGB565:       R7|R6|R5|R4|R3|G7|G6|G5|G4|G3|G2|B7|B6|B5|B4|B3
RGB666: R7|R6|R5|R4|R3|R2|G7|G6|G5|G4|G3|G2|B7|B6|B5|B4|B3|B2
*/

#if (LCD_TYPE == LCD_LQ026B3UX01)

//#define PORT_9BIT

void write_cmd(u8 dat,u8 lsb)
{
    TFT_CS_EN();
    TFT_RS_L();
#ifdef PORT_9BIT
    PORTG_DIR &=~ BIT(7);
    delay(2);
    if(lsb == 0)
        PORTG_OUT &=~ BIT(7);
    else
        PORTG_OUT |= BIT(7);
    pap_write(dat);
#else
    dat <<= 1;
    dat += lsb;
    pap_write(dat);
#endif
    TFT_CS_DIS();
}

void write_dat(u8 dat,u8 lsb)
{
    TFT_CS_EN();
    TFT_RS_H();
#ifdef PORT_9BIT
    PORTG_DIR &=~ BIT(7);
    delay(2);
    if(lsb == 0)
        PORTG_OUT &=~ BIT(7);
    else
        PORTG_OUT |= BIT(7);
    pap_write(dat);
#else
    dat <<= 1;
    dat += lsb;
    pap_write(dat);
#endif
    TFT_CS_DIS();
}

u8 read_dat()
{
    u8 val;
    TFT_CS_EN();
    TFT_RS_H();
#ifdef PORT_9BIT
    PORTG_DIR |= BIT(7);
    delay(2);
    val = pap_read();
    val <<= 1;
    if(PORTG_IN&BIT(7))
        val |= BIT(0);
#else
    val = pap_read();
#endif
    TFT_CS_DIS();

    return val;
}

void write_reg(u8 index,u8 dat)
{
//    TFT_CS_EN();
//    TFT_RS_L();
//    pap_write(index);
//    TFT_RS_H();
//    pap_write(dat);
//    TFT_CS_DIS();
    write_cmd(index>>1,index&BIT(0));
    write_dat(dat>>1,dat&BIT(0));
}

u16 read_reg(u8 index)
{
    u16 val;
//    TFT_CS_EN();
//    TFT_RS_L();
//    pap_write(index);
//    TFT_RS_H();
//    val = pap_read();
//    TFT_CS_DIS();

    write_cmd(index>>1,index&BIT(0));
    val = read_dat();
    printf("\nreg %02xH = 0x%02x",index,val);
    return val;
}

void tft_setrange(u16 x0,u16 x1,u16 y0,u16 y1)
{
    write_reg(0x13,x0);
    write_reg(0x16,x1);

    write_reg(0x14,y0);
    write_reg(0x15,y0>>8);

    write_reg(0x17,y1);
    write_reg(0x18,y1>>8);

    write_reg(0x19,0x01);
}

void tft_set_pos(u16 x0,u16 x1,u16 y0,u16 y1)
{
    //putbyte('x');

    tft_setrange(x0,x1,y0,y1);
    write_cmd(0x03>>1,0x03&BIT(0));
    TFT_CS_EN();
    TFT_RS_H();

}

/*
void tft_ClearScreen(u16 color)
{
    u8 *buf;
    u32 i;

    tft_set_pos(0,239,0,399);
    buf = malloc(240*400*2);

    for(i=0;i<240*2;i+=2)
    {
        buf[i] = color>>8;
        buf[i+1] = color;
    }

    for(i=0;i<400;i++)
    {
        pap_dma_write(buf,240*2);
    }
    PAP_WAIT_NCLR();
    TFT_CS_DIS();
    free(buf);
}*/

void tft_ClearScreen(u16 color)
{
    u8 *buf;
    u32 i;

    tft_set_pos(0,239,0,399);
    buf = malloc(240*400*3);

    for(i=0;i<240*3;i+=3)
    {
        buf[i] = 0x00;
        buf[i+1] = 0xff;
        buf[i+2] = 0xff;
    }

    for(i=0;i<400;i++)
    {
        pap_dma_write(buf,240*3);
    }
    PAP_WAIT_NCLR();
    TFT_CS_DIS();
    free(buf);
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


void tft_init(void)
{
    u8 id;
    u32 i,j;
    u8 *buf;

    printf("\nLQ026B3UX01 init ...");
    TFT_CS_OUT();
    TFT_RS_OUT();
    TFT_RST_OUT();

    TFT_RST_H();
    delay_2ms(100);
    TFT_RST_L();
    delay_2ms(100);
    TFT_RST_H();

    pap_init();
    printf("\nPAP_CON:%x",PAP_CON);

    write_reg(0x68,0x07);
    write_reg(0x69,0x80);

    //8bit
    //write_reg(0x01,0x02);//8bit+8bit 2 times
    //write_reg(0x01,0x04);//6bit+6bit+6bit 3 times DB[7..2]

    //9bit
    //write_reg(0x01,0x02);//8bit+8bit 2 times
    //write_reg(0x01,0x00);//9bit+9bit 2 times
    //write_reg(0x01,0x03);//9bit 1 times

    //16bit
    //write_reg(0x01,0x02);//16bit 1 times
    //write_reg(0x01,0x03);//8bit+8bit 2 times
    //write_reg(0x01,0x05);//6bit+6bit+6bit 3 times DB[7..2]

    //18bit
    //write_reg(0x01,0x02);//8bit+8bit 2times
    write_reg(0x01,0x00);//18bit 1 times
    //write_reg(0x01,0x03);//16bit 1 times

    write_reg(0x08,0x00);
    write_reg(0x10,0xF0);
    write_reg(0x11,0x90);
    write_reg(0x12,0x01);
    write_reg(0x1A,0xef);
    write_reg(0x1B,0x00);
    write_reg(0x1C,0x00);

    //write_reg(0x1D,/*0x03*/0x04);//write_reg(0x1D,0x07);
    write_reg(0x1D,0x03);

    write_reg(0x1E,0x50);
    write_reg(0x30,0x81);//write_reg(0x30,0x80);
    write_reg(0x33,0x02);
    write_reg(0x42,0x07);
    write_reg(0x43,0x01);
    write_reg(0x44,0x04);
    write_reg(0x45,0x08);
    write_reg(0x46,0x06);
    write_reg(0x48,0x01);
    write_reg(0x4A,0x01);
    write_reg(0x4B,0xA8);
    write_reg(0x4C,0x07);
    write_reg(0x5A,0x00);
    write_reg(0x62,0x21);
    write_reg(0x65,0x02);
    write_reg(0x66,0x00);
    write_reg(0x6A,0x02);
    write_reg(0x81,0x44);
    write_reg(0x82,0x15);
    write_reg(0x83,0x25);
    write_reg(0x84,0x10);
    write_reg(0x85,0x25);
    write_reg(0x86,0x17);
    write_reg(0x90,0x25);
    write_reg(0x91,0x04);
    write_reg(0x92,0x15);
    write_reg(0x31,0x01);
    write_reg(0x3E,0x01);

    tft_set_pos(0,239,0,399);
    /*
    buf = 0x302EE00;
    for(i=0;i<240;i++)
    {
        pap_dma_write(buf,400*2);
        buf += 400*2;
    }*/


    buf = 0x302EE00;
    for(i=0;i<400;i++)
    {
        pap_dma_write(buf,240*2);
        buf += 240*2;
    }
    PAP_WAIT_NCLR();
    TFT_CS_DIS();

  //  while(1)
//    {
        //delay_2ms(500);
        //tft_ClearScreen(0x00F8);
//        delay_2ms(500);
//        tft_ClearScreen(0xE007);
//        delay_2ms(500);
//        tft_ClearScreen(0x3F00);
        delay_2ms(500);
//    }


    printf("\ninit out ...");
    read_reg(0x00);
    read_reg(0x01);
    read_reg(0x10);
    read_reg(0x11);
}


const L0_PARA_CFG lcd_lq026b3ux01_para =
{
    400*1+20,
    20,
    20,
    400,

    240+20,
    20,
    20,
    0,
    240,
};

void test(u16 x0,u16 x1,u16 y0,u16 y1)
{
    putbyte('x');
}

u8 r_tab[255],g_tab[255],b_tab[255];
void imb_disp_init()
{
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    tft_init();

    disp_dev.drive_mode = MODE_MCU;
    disp_dev.data_width = PORT_18BITS;
    disp_dev.out_format = FORMAT_RGB666;
    disp_dev.ncycle = ONE_CYCLE;
    disp_dev.signal_polarity = DEFAULT_POSITIVE;//VSYNC_NEGATIVE|HSYNC_NEGATIVE|DCLK_NEGATIVE;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.para = (L0_PARA_CFG *)&lcd_lq026b3ux01_para;
    disp_dev.hook = test;

    disp_dev.bkcolor = palette_yuv[5];

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

//    gain.r_gain = 0x00;
//    gain.g_gain = 0x00;
//    gain.b_gain = 0x80;
//    gain.r_offs = 0x00;
//    gain.g_offs = 0x00;
//    gain.b_offs = 0x00;

    disp_dev.gain = &gain;
    disp_dev.isr_prior = 2;
    memset(r_tab,140,256);
    memset(g_tab,253,256);
    memset(b_tab,153,256);
    disp_dev.r_gma_tab = NULL;
    disp_dev.g_gma_tab = NULL;
    disp_dev.b_gma_tab = NULL;

    imb_init(&disp_dev);

    IMGLAYER *image= imb_get_image_hdl(LAYER_IMAGE0);
	image->enable = TRUE;
    image->alpha = 0;
    image->x = 0;
    image->y = 0;
    image->width = 400;
    image->height = 240;
//    image->buff_width = 400;
//    image->buff_height = 240;
	image->buf = 0x02800000;

	image->buf_num = 2;
	image->mask_en = FALSE;
	image->dsc_ram = NULL;
	imb_image_cfg(image);

    struct DSC_ADDR *curr_addr;
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);

    printf("\nimb_l6_fmt:%x",imb_l6_fmt);
    printf("\nimb_emi_con:%x",imb_emi_con);
    printf("\nimb_io_con:%x",imb_io_con);

    OSDWIN *osd = imb_get_osd_hdl(LAYER_OSD0_WIN0);
    osd->enable = FALSE;
    osd->alpha = 0;
    osd->x = 0;
    osd->y = 0;
    osd->width = 240;
    osd->height = 400;
    osd->buf = 0x3037AA0;
    imb_osd_win_cfg(osd);


/*
    OSDWIN *osd = imb_get_osd_hdl(LAYER_OSD0_WIN0);
    osd->enable = TRUE;
    osd->alpha = 0;
    osd->x = 0;
    osd->y = 0;
    osd->width = 60;
    osd->height = 100;
    osd->buf = 0x302EE00;
    imb_osd_win_cfg(osd);

    osd = imb_get_osd_hdl(LAYER_OSD0_WIN1);
    osd->enable = TRUE;
    osd->alpha = 0;
    osd->x = 0;
    osd->y = 100;
    osd->width = 60;
    osd->height = 100;
    osd->buf = 0x3030570;
    imb_osd_win_cfg(osd);

    osd = imb_get_osd_hdl(LAYER_OSD0_WIN2);
    osd->enable = TRUE;
    osd->alpha = 0;
    osd->x = 0;
    osd->y = 200;
    osd->width = 60;
    osd->height = 100;
    osd->buf = 0x3031CE0;
    imb_osd_win_cfg(osd);

    osd = imb_get_osd_hdl(LAYER_OSD1_WIN0);
    osd->enable = TRUE;
    osd->alpha = 0;
    osd->x = 60;
    osd->y = 0;
    osd->width = 60;
    osd->height = 100;
    osd->buf = 0x3033450;
    imb_osd_win_cfg(osd);

    osd = imb_get_osd_hdl(LAYER_OSD1_WIN1);
    osd->enable = TRUE;
    osd->alpha = 0;
    osd->x = 60;
    osd->y = 100;
    osd->width = 60;
    osd->height = 100;
    osd->buf = 0x3034BC0;
    imb_osd_win_cfg(osd);

    osd = imb_get_osd_hdl(LAYER_OSD1_WIN2);
    osd->enable = TRUE;
    osd->alpha = 0;
    osd->x = 60;
    osd->y = 200;
    osd->width = 60;
    osd->height = 100;
    osd->buf = 0x3036330;
    imb_osd_win_cfg(osd);
*/
    printf("\nimb_l3_con:%x",imb_l3_con);
    printf("\nimb_l4_con:%x",imb_l4_con);

    tft_set_pos(0,239,0,399);

	imb_start();
}
#endif
