#include "ILI8960.h"
#include "imb_module.h"
#include "tft.h"

#if (LCD_TYPE == LCD_8INCH)

#define SCR_WIDTH    800
#define SCR_HEIGHT   600

#define HORI_TOTAL_CLK    (1900)
#define HORI_SYNC_CLK     48
#define HORI_START_CLK    88

#define VERT_TOTAL_CLK    (660)
#define VERT_SYNC_CLK     3
#define VERT_START_CLK    39

const L0_PARA_CFG rgb_para0 =
{
    HORI_TOTAL_CLK,
    HORI_SYNC_CLK,
    HORI_START_CLK,
    SCR_WIDTH,

    VERT_TOTAL_CLK,
    VERT_SYNC_CLK,
    VERT_START_CLK,
	0,
    SCR_HEIGHT,
};

const L0_PARA_CFG rgb_para =
{
    1300,//1900,//
    48,
    88,
    800,//width

    660,
    3,
    39,
    0,
    600,//height
};

void lcdpanel_7_inch_init()
{
	DISP_DEV_CFG disp_dev;
    CGAIN gain;

    disp_dev.drive_mode = MODE_RGB_SYNC;
    disp_dev.data_width = PORT_24BITS;
    disp_dev.out_format = FORMAT_RGB888;
    disp_dev.para = &rgb_para;
    disp_dev.hook = NULL;
	disp_dev.ncycle = ONE_CYCLE;
    disp_dev.scan_mode = NON_INTERLACED;

    disp_dev.bkcolor = 0xE20095 ;//palette_yuv[32];
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
    image->width = 800;
    image->height = 600;
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
