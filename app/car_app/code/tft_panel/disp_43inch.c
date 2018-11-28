#include "ILI8960.h"
#include "imb_module.h"
#include "tft.h"


#if (LCD_TYPE == LCD_43INCH)
const L0_PARA_CFG lcd_43_inch_para =
{
    839,//531,//
    1,
    43,//43,
    480,

    272+22+4,
    10,
    12,//12,
    0,
    272,
};


void lcdpanel_4_3_inch_init()
{
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    disp_dev.drive_mode = MODE_RGB_SYNC;
    disp_dev.data_width = PORT_24BITS;
    disp_dev.out_format = FORMAT_RGB888;
    disp_dev.para = (L0_PARA_CFG *)&lcd_43_inch_para;
    disp_dev.hook = NULL;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.ncycle = ONE_CYCLE;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE;
    disp_dev.bkcolor = 0xF08966;//0xE20095;//0x5a8080;

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
    //image->buff_width = 480;
    //image->buff_height = 272;
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
