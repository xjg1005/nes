#include "tft.h"
#include "imb_module.h"
#include "sdfile.h"
#include "my_ui.h"

//#if (LCD_TYPE == LCD_AVOUT)

/*
    864,
    138,
    138,
    720,

    313,
    21,
    21,
    22,
    288,
    */

/*
    864,
    144,
    144,
    720,

    313,
    21,
    23,
    24,
    288,
*/

const L0_PARA_CFG avout_ntsc_27MHz =
{
    //NTSC 720x480
    858*2,
    138*2,
    138*2,
    720,

    263,
    21,
    21,//cve2_wr(51,21);
    22,
    240,
};

const L0_PARA_CFG avout_ntsc_36MHz =
{
    //NTSC 960x480
    1144*2,
    182*2,
    182*2,
    960,

    263,
    21,
    21,//cve2_wr(51,21);
    22,
    240,
};

const L0_PARA_CFG avout_pal_27MHz =
{
    //PAL 720x576
    864*2,
    138*2,
    138*2,
    720,

    313,
    21,
    23,
    24,
    288,
};

const L0_PARA_CFG avout_pal_36MHz =
{
    //PAL 960x576
//    1144*2,
//    182*2,
//    182*2,
//    962,

//    1144*2,
//    184*2,
//    184*2,
//    960,

    1152*2,
    192*2,
    192*2,
    960,

    313,
    21,
    23,
    24,
    288,
};

extern char getchar();
void avout_init()
{
    printf("\navout init");
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

//    PORTH_DIR |= (BIT(1)|BIT(0));
//    PORTH_PU &=~ (BIT(1)|BIT(0));
//    PORTH_PD &=~ (BIT(1)|BIT(0));

    disp_dev.drive_mode = MODE_AVOUT;
    disp_dev.data_width = PORT_1BIT;
    disp_dev.out_format = FORMAT_YUV422;
    disp_dev.hook = NULL;
    disp_dev.scan_mode = INTERLACED_ALL;
    disp_dev.ncycle = ONE_CYCLE;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE;
    disp_dev.bkcolor = 0xE20095;//0x5a8080;
    //disp_dev.avout_spec = CVE_NTSC_36MHZ/*|CVE_COLOR_BAR*/;
    disp_dev.avout_spec = CVE_PAL_27MHZ;  //JOHNSON 20160612

    if(disp_dev.avout_spec & CVE_NTSC_27MHZ)
        disp_dev.para = (L0_PARA_CFG *)&avout_ntsc_27MHz;
    if(disp_dev.avout_spec & CVE_NTSC_36MHZ)
        disp_dev.para = (L0_PARA_CFG *)&avout_ntsc_36MHz;
    if(disp_dev.avout_spec & CVE_PAL_27MHZ)
        disp_dev.para = (L0_PARA_CFG *)&avout_pal_27MHz;
    if(disp_dev.avout_spec & CVE_PAL_36MHZ)
        disp_dev.para = (L0_PARA_CFG *)&avout_pal_36MHz;

    memset(&gain,0x00,sizeof(gain));
    gain.y_offs = 0x00;//-20;//0x10;//0x00;
    gain.u_offs = 0x00;
    gain.v_offs = 0x00;
    gain.y_gain = 0x80;
    gain.u_gain = 0x80;//0x80;
    gain.v_gain = 0x80;//0x80;
    gain.r_gain = 0x80;
    gain.g_gain = 0x80;
    gain.b_gain = 0x80;
    gain.r_coe0 = 0x40;
    gain.g_coe1 = 0x40;
    gain.b_coe2 = 0x40;

    printf("\ngain.y_offs = %x",gain.y_offs);

    disp_dev.gain = &gain;
    disp_dev.isr_prior = 3;
    disp_dev.r_gma_tab = NULL;
    disp_dev.g_gma_tab = NULL;
    disp_dev.b_gma_tab = NULL;

    //////////////////
//    disp_dev.drive_mode = MODE_RGB_SYNC;
//    disp_dev.dclk_freq = IMB_INTERNAL|DCLK_40MHz;
//    disp_dev.dclk_divider = 0;
//    printf("\nimb init");
//    imb_init(&disp_dev);
//    printf("\nimb init ok");
//    imb_start();
//    printf("\nimb start");
//    while(1);
    //////////////////////////

    imb_init(&disp_dev);

#if 0//1
    IMGLAYER *image= imb_get_image_hdl(LAYER_IMAGE0);

	image->enable = TRUE;
    image->alpha = 0;
    image->x = 0;
    image->y = 0;

    if(disp_dev.avout_spec & CVE_NTSC_27MHZ)
    {
        image->width = 720;
        image->height = 480;
//      image->buff_width = 720;
//      image->buff_height = 480;
    }
    else if(disp_dev.avout_spec & CVE_NTSC_36MHZ)
    {
        image->width = 960;
        image->height = 480;
    }
    else if(disp_dev.avout_spec & CVE_PAL_27MHZ)
    {
        image->width = 720;
        image->height = 576;
    }
    else if(disp_dev.avout_spec & CVE_PAL_36MHZ)
    {
        image->width = 960;
        image->height = 576;
    }

//	image->buf = 0x02800000;
    u8 *buf;
    if(disp_dev.avout_spec & CVE_NTSC_27MHZ)	//720*480
    {
        buf = malloc(720*480*3/2);
        sdfile_t file;
        if(sd_fopen("720x480.bin",&file))
        {
            sd_fread(&file,buf,720*480*3/2);
            flush_dcache(buf,720*480*3/2);
        }
        else
        {
            printf("\nopen file fail");
            while(1);
        }
    }
    else if(disp_dev.avout_spec & CVE_NTSC_36MHZ)//960*480
    {
        buf = malloc(960*480*3/2);
        sdfile_t file;
        if(sd_fopen("960x480.bin",&file))
        {
            sd_fread(&file,buf,960*480*3/2);
            flush_dcache(buf,960*480*3/2);
        }
        else
        {
            printf("\nopen file fail");
            while(1);
        }
    }
    else if(disp_dev.avout_spec & CVE_PAL_27MHZ)//720*576
    {
        buf = malloc(720*576*3/2);
        sdfile_t file;
        if(sd_fopen("720x576.bin",&file))
        {
            sd_fread(&file,buf,720*576*3/2);
            flush_dcache(buf,720*576*3/2);
        }
        else
        {
            printf("\nopen file fail");
            while(1);
        }
    }
    else if(disp_dev.avout_spec & CVE_PAL_36MHZ)//960*576
    {
        buf = malloc(960*576*3/2);
        sdfile_t file;
        if(sd_fopen("960x576.bin",&file))
        {
            sd_fread(&file,buf,960*576*3/2);
            flush_dcache(buf,960*576*3/2);
        }
        else
        {
            printf("\nopen file fail");
            while(1);
        }
    }
    image->buf = buf;

	image->buf_num = 2;
	image->mask_en = FALSE;
	image->dsc_ram = NULL;
	imb_image_cfg(image);

    struct DSC_ADDR *curr_addr;
	curr_addr = dsc_ram_set_buf_full(image->dsc_ram);

    imb_start();
    printf("\nimb_start 000");
    while(1);
#endif
}

//#endif
