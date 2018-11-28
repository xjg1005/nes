#include "ILI8960.h"
#include "imb_module.h"
#include "tft.h"
#include "hwi.h"

#if (LCD_TYPE == LCD_JC4301)

const L0_PARA_CFG lcd_43_inch_para =
{
    839,//525,
    1,
    40,
    480,

    288,
    1,
    8,
    0,
    272,
};

///<IMB GAMMA RED表配置
static void imb_gma_rtab(u8 *tab)
{
    u32 *gma_ptr;
    u16 i;

    gma_ptr = &imb_r_gma_tab;
    for(i=0;i<256;i++){
        *gma_ptr++ = tab[i];
    }
}

///<IMB GAMMA GREEN表配置
static void imb_gma_gtab(u8 *tab)
{
    u32 *gma_ptr;
    u16 i;

    gma_ptr = &imb_g_gma_tab;
    for(i=0;i<256;i++){
        *gma_ptr++ = tab[i];
    }
}

///<IMB GAMMA BLUE表配置
static void imb_gma_btab(u8 *tab)
{
    u32 *gma_ptr;
    u16 i;

    gma_ptr = &imb_b_gma_tab;
    for(i=0;i<256;i++){
        *gma_ptr++ = tab[i];
    }
}

u8 r_tab[255],g_tab[255],b_tab[255];
void imb_disp_init()
{
    u32 i,j,k;
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    //io pwm output
    IOMC1 =  (IOMC1 & ~(0xF<<8)) | (0x0b<<8);//sel PWM8_OUT
    //IOMC1 =  (IOMC1 & ~(0xF<<12)) | (0x0b<<12);//sel PWM8_OUT
//    PWM8_CON = (0<<8)|(253);
//    PORTF_DIR &=~ BIT(11);
//    PORTF_PU |= BIT(11);
//    PORTF_PD |= BIT(11);
//    PORTF_DIE &=~ BIT(11);//die

//    PORTA_DIR &=~ BIT(0);
//    PORTA_PU |= BIT(0);
//    PORTA_PD |= BIT(0);
//    PORTA_DIE &=~ BIT(0);//die

//    PORTA_DIR &=~ BIT(0);
//    PORTA_OUT &=~ BIT(0);

    disp_dev.drive_mode = MODE_RGB_DE;
    disp_dev.data_width = PORT_24BITS;
    disp_dev.out_format = FORMAT_RGB888;
    disp_dev.para = (L0_PARA_CFG *)&lcd_43_inch_para;
    disp_dev.hook = NULL;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.ncycle = ONE_CYCLE;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE;
    disp_dev.bkcolor = 0x40709e;//0x008080;//

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

//    gain.r_gain = 0;
//    gain.g_gain = 0;
//    gain.b_gain = 0;
//    gain.r_offs = 0x7f;

    memset(r_tab,/*203*/205   ,256);
    memset(g_tab,/*0*/95   ,256);
    memset(b_tab,/*0*/19   ,256);

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
    image->width = 480;
    image->height = 272;
    image->buf = 0x2800000;

	image->buf_num = 2;
	image->mask_en = FALSE;
	image->dsc_ram = NULL;
	imb_image_cfg(image);

    struct DSC_ADDR *curr_addr;
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);

    image = imb_get_image_hdl(LAYER_IMAGE1);
    image->enable = TRUE;
    image->alpha = 00;
    image->x = 40;
    image->y = 40;
    image->width = 400;
    image->height = 200;
    image->buff_width = 400;
    image->buff_height = 240;
    image->buf = 0x282FD00;

    image->buf_num = 2;
    image->mask_en = FALSE;
    image->mask_buf = malloc(416*240/8);//宽度必须为32的倍数，高度无限制

    memset(image->mask_buf,0x03,416*240/8);
//    for(i=0;i<240;i++)
//    {
//        image->mask_buf[i*416/8+0/8] = 0x08;
//    }

    imb_image_cfg(image);
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);

    OSDWIN *osd = imb_get_osd_hdl(LAYER_OSD0_WIN0);
    osd->enable = TRUE;
    osd->alpha = 0;
    osd->x = 240;
    osd->y = 52;
    osd->width = 240;
    osd->height = 220;
    osd->buf = 0x29209E0;
    imb_osd_win_cfg(osd);

    osd = imb_get_osd_hdl(LAYER_OSD1_WIN0);
    osd->enable = TRUE;
    osd->alpha = 0;
    osd->x = 38;//38;
    osd->y = 4;
    osd->width = 60;//112;
    osd->height = 100;//100;
    osd->buf = 0x29380E0;//0x29087F0;
    imb_osd_win_cfg(osd);

    osd = imb_get_osd_hdl(LAYER_OSD1_WIN1);
    osd->enable = TRUE;
    osd->alpha = 0;
    osd->x = 128;//128;
    osd->y = 104;//104
    osd->width = 60;//112;
    osd->height = 100;//60;
    osd->buf = 0x2939850;//0x29092E0;
    imb_osd_win_cfg(osd);

    osd = imb_get_osd_hdl(LAYER_OSD1_WIN2);
    osd->enable = FALSE;
    osd->alpha = 0;
    osd->x = 350;
    osd->y = 172;
    osd->width = 60;//112;
    osd->height = 100;//100;
    osd->buf = 0x293AFC0;//0x2907D00;
    imb_osd_win_cfg(osd);

    imb_start();
    //config

    PORTF_DIR &=~ BIT(11);
    ENABLE_INT();
    u8 tmp[0x7];
    u8 c = 0;

    memset(tmp,0x00,sizeof(tmp));
    while(1)
    {
/*
        PORTF_OUT |= BIT(11);
        //delay_2ms(1);
        delay(3500);
        PORTF_OUT &=~ BIT(11);
        delay_2ms(1);
        //delay(1000);
*/

        char ch = getchar();
        for(i=0; i<0x7-1; i++)
        {
            tmp[i] = tmp[i+1];
        }
        tmp[0x6] = ch;
        c++;
        //printf("\n%02x %02x %02x %02x %02x %02x %02x c:%02x",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],c);
//        putchar('*');
        if(c == 7)
        {
            //printf("\n%02x %02x %02x %02x %02x %02x %02x",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6]);
            if((tmp[0] == 0xfe)&&(tmp[1] == 0xed)&&(tmp[5] == 0xde)&&(tmp[6] == 0xef))
            {
                imb_close();
                //printf("\nRGB:%x%x%x",tmp[2],tmp[3],tmp[4]);

                memset(r_tab,tmp[2]   ,256);
                memset(g_tab,tmp[3]   ,256);
                memset(b_tab,tmp[4]   ,256);
                imb_gma_rtab(r_tab);
                imb_gma_gtab(g_tab);
                imb_gma_btab(b_tab);
                imb_start();
            }
            c = 0;
        }
        //putbyte(ch);
    }
}

#endif
