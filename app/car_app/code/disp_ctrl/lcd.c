#include "lcd.h"
#include "imb_module.h"
#include "tft.h"
#include "sdfile.h"
#include "msg.h"



#define BOTTOM_COLOR     0x008080//0xE20095  //0x5a8080


#define SCR_WIDTH    800
#define SCR_HEIGHT   600

#define HORI_TOTAL_CLK    (1900)
#define HORI_SYNC_CLK     48
#define HORI_START_CLK    88

#define VERT_TOTAL_CLK    (660)
#define VERT_SYNC_CLK     3
#define VERT_START_CLK    39


#if 1 //AVOUT JOHNSON 20160530 ADD

//#define AVOUT_DET_IO_INIT()    PORTF_DIR |= BIT(0);PORTF_PU |= BIT(0)
//#define AVOUT_DET_IO_STATUS()  (((PORTF_IN & BIT(0))>>>3) ? FALSE : TRUE )
#define AVOUT_DET_IO_INIT()    PORTF_DIR |= BIT(0);PORTF_PU |= BIT(0)
#define AVOUT_DET_IO_STATUS()  ((PORTF_IN & BIT(0)) ? FALSE : TRUE )

u16 LCD_W = 320; //JOHNSON 20160621
u16 LCD_H = 240;

u8 avout_det_io()
{
    #if (DOUBLE_UI_C == 1)
    return FALSE;
    #endif
    puts("AVOUT_DET_IO_STATUS=%d*******************\n",AVOUT_DET_IO_STATUS());
    return AVOUT_DET_IO_STATUS();
}
extern bool check_tvout_flag;
void avout_det_scan(void *parm)
{
    static u8 last_status = FALSE;
    u8 this_status = false;

    if(check_tvout_flag && avout_det_io())
    {
        ///如果开机的时候就是tv输出状态，不让检测
    }
    else
    {
        if(check_tvout_flag)
        {
            check_tvout_flag = false;
            last_status = true;
        }
        this_status = avout_det_io();
    }


    if (this_status == FALSE)
    {
        if (last_status == TRUE)
        {
//            puts("\n avout outttttt\n");
            put_msg_fifo(MSG_SCR_DISP_CHANGE ,MSG_TO_PROCESS ,UI_DISP_N);
        }
        last_status = this_status;
    }
    else
    {
        if (last_status == FALSE)
        {
//            puts("\n avout innnnnnnnn\n");
            put_msg_fifo(MSG_SCR_DISP_CHANGE ,MSG_TO_PROCESS ,UI_DISP_N);
        }
        last_status = this_status;
    }

}
void avout_det_io_init()
{
    AVOUT_DET_IO_INIT();
}
void avout_det_reg()
{
    AVOUT_DET_IO_INIT();

    timer_reg(20,avout_det_scan,NULL);
}
#endif


static const L0_PARA_CFG g_mcu_para =
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

extern const L0_PARA_CFG lcd_ili9325_inch_para;





const L0_PARA_CFG lcd_43_inch_para =
{

#if (TFT_LCD == 1)

#elif (NEW_LCD == 1)
//    525,
//    1,
//    40,
//    LCD_W,
//
//    288,
//    1,
//    8,
//    0,
//    LCD_H,
    525*3,  //5203 JOHNSON 20160509
    1,
    40,
    LCD_W,

    288,
    1,
    8,
    0,
    LCD_H,
#else
    839,//531,//
    1,
    43,
    LCD_W,

    LCD_H+22+4,
    10,
    12,
    0,
    LCD_H,
#endif
};

extern void tft_SetRange(u16 x0, u16 x1, u16 y0, u16 y1);
//void lcd_init()  //old
void lcd_init(u8 mode)  //AVOUT JOHNSON 20160530
{
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

//**************AVOUT JOHNSON 20160530
#if (TFT_LCD == 1)
    if (mode == LCD_ILI9325)
    {
        puts("imb_ili9325_disp_init\n");
  #if (LCD_TYPE != LCD_AVOUT)
    imb_disp_init();
  #endif
	}
	else if (mode == LCD_AVOUT)
    {
        avout_init();
    }
//************************

#elif (NEW_LCD == 1)
    disp_dev.drive_mode = MODE_RGB_DE;
    //disp_dev.data_width = PORT_24BITS;
    disp_dev.data_width = PORT_8BITS; //5203 JOHNSON 20160509
    disp_dev.out_format = FORMAT_RGB888;
    disp_dev.para = (L0_PARA_CFG *)&lcd_43_inch_para;
    disp_dev.hook = NULL;
    disp_dev.scan_mode = NON_INTERLACED;
    //disp_dev.ncycle = ONE_CYCLE;
    disp_dev.ncycle = THREE_CYCLE; //5203 JOHNSON 20160509
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE;
    disp_dev.bkcolor = BOTTOM_COLOR;
    disp_dev.dclk_freq = IMB_EXTERNAL;
    //disp_dev.dclk_divider = USE_PLL_CLK|PLL0_CLK|DIVA_5|DIVB_8|DIVC_1;
    disp_dev.dclk_divider = USE_PLL_CLK|PLL0_CLK|DIVA_1|DIVB_4|DIVC_4;//5203 JOHNSON 20160509


    memset(&gain,0x00,sizeof(gain));

    sdfile_t imbfile;
    if(sd_fopen("JL_IMB.BIN",&imbfile))
    {
        u8 *buf = malloc(imbfile.len);
        sd_fread(&imbfile,buf,imbfile.len);
        gain.y_offs = buf[0];
        gain.u_offs = buf[1];
        gain.v_offs = buf[2];
        gain.y_gain = buf[3];
        gain.u_gain = buf[4];
        gain.v_gain = buf[5];
        gain.r_offs = buf[6];
        gain.g_offs = buf[7];
        gain.b_offs = buf[8];
        gain.r_gain = buf[9];
        gain.g_gain = buf[10];
        gain.b_gain = buf[11];
        gain.r_coe0 = buf[12];
        gain.r_coe1 = buf[13];
    	gain.r_coe2 = buf[14];
        gain.g_coe0 = buf[15];
        gain.g_coe1 = buf[16];
        gain.g_coe2 = buf[17];
        gain.b_coe0 = buf[18];
        gain.b_coe1 = buf[19];
        gain.b_coe2 = buf[20];
        gain.r_gma  = buf[22]<<8|buf[21];
        gain.g_gma  = buf[24]<<8|buf[23];
        gain.b_gma  = buf[26]<<8|buf[25];
        free(buf);
    }
    else
    {
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
    }

    disp_dev.gain = &gain;
    disp_dev.isr_prior = 3;

    u16 r_clut,g_clut,b_clut;
    sdfile_t lut_file;
    if(!sd_fopen("lut.bin",&lut_file))
            return FALSE;

    u8 *r_tab = malloc(256);
    u8 *g_tab = malloc(256);
    u8 *b_tab = malloc(256);
	r_clut = 150;
	g_clut = 150;
	b_clut = 150;
    sd_fseek(&lut_file,SD_SEEK_SET,(r_clut-75)*256);
    sd_fread(&lut_file,r_tab,256);
    sd_fseek(&lut_file,SD_SEEK_SET,(g_clut-75)*256);
    sd_fread(&lut_file,g_tab,256);
    sd_fseek(&lut_file,SD_SEEK_SET,(b_clut-75)*256);
    sd_fread(&lut_file,b_tab,256);


    disp_dev.r_gma_tab = r_tab;
    disp_dev.g_gma_tab = g_tab;
    disp_dev.b_gma_tab = b_tab;

    imb_init(&disp_dev);

    free(r_tab);
    free(g_tab);
    free(b_tab);

#else
    disp_dev.drive_mode = MODE_RGB_SYNC;
    disp_dev.data_width = PORT_18BITS;
    disp_dev.out_format = FORMAT_RGB888;
    disp_dev.para = (L0_PARA_CFG *)&lcd_43_inch_para;
    disp_dev.hook = NULL;
    disp_dev.scan_mode = NON_INTERLACED;
    disp_dev.ncycle = ONE_CYCLE;
    disp_dev.signal_polarity = VSYNC_NEGATIVE|HSYNC_NEGATIVE;
    disp_dev.bkcolor = BOTTOM_COLOR;

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
#endif

}

void lcd_size_init(u32 weight, u32 height)  //AVOUT JOHNSON 20160530 ADD
{
    LCD_W = weight;
    LCD_H = height;
}

u32 lcd_get_size(u16 * width, u16 * heigh)
{
    *width = LCD_W;
    *heigh = LCD_H;
}



u32 lcd_get_bottom_color()
{
    return BOTTOM_COLOR;
}


/*----------------------------------------------------------------------------*/
/** @brief: 初始化屏幕背光调节功能
    @param:
    @return:
    @author:
    @note:
*/
/*----------------------------------------------------------------------------*/

void lcd_pwm_init(void)
{
    IOMC1 &= ~(0xF<<8);  // output channel 0 (注意是否与clockout冲突) 0110 TMR2
    IOMC1 |= (0x6<<8);
    PORTF_DIR &= ~BIT(14);
    PORTF_PU |= BIT(14);
    PORTF_PD |= BIT(14);
    PORTF_DIE &=~BIT(14);

    T2_CON = 0;
//    T2_CON |= BIT(5) | BIT(4);//160M/64分频
    T2_CON |= BIT(4);//4分频

//    T2_CON |= BIT(3);//OSC时钟源

    T2_CNT = 0;
    T2_PRD = 3000;

    lcd_brightness_control(0);

    T2_CON |= BIT(0);//开始计数
    T2_CON |= BIT(8);//PWM使能

}

void lcd_brightness_control(u8 level)
{
    if(level == 1)
    {
//        led_level = level;
        T2_PWM = 600;//20%
    }
    else if(level == 2)
    {
//        led_level = level;
        T2_PWM = 750;//25%
    }
    else
    {
//        led_level = level;
        T2_PWM = 300 * level;
    }
}


