#include "typedef.h"
#include "config.h"
#include "tft.h"
#include "pap.h"
#include "uart.h"
#include "pap.h"
#include "imb_module.h"
#include "tft.h"

#if (LCD_TYPE == LCD_SSD1963)

#define  HDP  799	//Horizontal Display Period
#define  HT   1000	//Horizontal Total
#define  HPS  51	//LLINE Pulse Start Position
#define  LPS  3		//Horizontal Display Period Start Position
#define  HPW  8		//LLINE Pulse Width


#define  VDP  479	//Vertical Display Period
#define  VT   530	//Vertical Total
#define  VPS  24	//LFRAME Pulse Start Position
#define  FPS  23	//Vertical Display Period Start Positio
#define  VPW  3 	//LFRAME Pulse Width


/*Private define-------------------------*/
#define POINT_COLOR RED
#define BACK_COLOR	0xFFFF

/*--------16位颜色值---------------*/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define LGRAY 			 0XC618 //浅灰色

void LCD_Configuration(void);
void LCD_WR_DATA(u16 LCD_Data);
void LCD_WR_REG(u16 LCD_Reg);
void LCD_Init();

u8 sdram_buf[800*480];

void tft_reset(void)
{
    TFT_RST_H();
    delay(50000);//延时
    TFT_RST_L();
    delay(50000);//延时
    TFT_RST_H();
}

void LCD_Configuration(void)
{
    TFT_CS_OUT();
    TFT_RS_OUT();
    TFT_RST_OUT();
}


void LCD_WR_REG(u16 LCD_Reg)
{
    /* Write 16-bit Index, then Write Reg */
    TFT_CS_EN();
	TFT_RS_L();
    pap_write(LCD_Reg);
    //TFT_RS_H();
	TFT_CS_DIS();
}

void LCD_WR_DATA(u16 LCD_Data)
{
	/* Write 16-bit Reg */
    TFT_CS_EN();
	TFT_RS_H();
	pap_write(LCD_Data);
    //TFT_RS_L();
	TFT_CS_DIS();
}

u16 LCD_RD_DATA()
{
    u16 dat;
    TFT_CS_EN();
	TFT_RS_H();
	dat = pap_read();
	TFT_CS_DIS();
	return dat;
}

void get_pixel_data_interface()//获取数据接口
{
    u16 ret;
    LCD_WR_REG(0xf1);
    ret = LCD_RD_DATA();
	printf("\ndata interface:%x",ret);
}

void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WR_REG(0x002A);
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(Xpos&0x00ff);
	LCD_WR_DATA(479>>8);
	LCD_WR_DATA(479&0x00ff);
    LCD_WR_REG(0x002b);
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(Ypos&0x00ff);
	LCD_WR_DATA(271>>8);
	LCD_WR_DATA(271&0x00ff);
}


void lcd_SetRange(u16 xs,u16 xe,u16 ys,u16 ye)
{
	/*  X轴  */
	LCD_WR_REG(0X002A);
	LCD_WR_DATA((xs>>8)&0xff);
	LCD_WR_DATA(xs&0xff);
	LCD_WR_DATA((xe>>8)&0xff);
	LCD_WR_DATA(xe&0XFF);

	/*  Y轴  */
    LCD_WR_REG(0X002B);
	LCD_WR_DATA((ys>>8)&0xff);
	LCD_WR_DATA(ys&0xff);
	LCD_WR_DATA((ye>>8)&0xff);
	LCD_WR_DATA(ye&0xff);

	LCD_WR_REG(0X002C);

//    putchar('x');
    TFT_CS_EN();
	TFT_RS_H();
}

void lcd_SetPos(u16 x,u16 y)
{
    ;
}

void LCD_Clear(u16 Color)
{
    unsigned int l=480,w;
    u16 i;

	/*--------设置刷屏窗口--------*/
	/*  X轴  */
	LCD_WR_REG(0X002A);
	LCD_WR_DATA(0);
	LCD_WR_DATA(0);
	LCD_WR_DATA(HDP>>8);
	LCD_WR_DATA(HDP&0XFF);

	/*  Y轴  */
    LCD_WR_REG(0X002B);
	LCD_WR_DATA(0);
	LCD_WR_DATA(0);
	LCD_WR_DATA(VDP>>8);
	LCD_WR_DATA(VDP&0X00FF);

	LCD_WR_REG(0X002C);

    TFT_CS_EN();
	TFT_RS_H();

	u8 *p;
	p = (u8*)sdram_buf;

    for(i=0;i<HDP+1;i++)
    {
        p[2*i] = (Color >> 8) & 0xff;
        p[2*i+1] = Color & 0xff;
    }

	while(l--)		  /* 480行 */
	{
        pap_dma_write(p,800*2);

	}
	PAP_WAIT_NCLR();
	TFT_CS_DIS();
}


void LCD_Read_DDB()
{
    u8 info[5];
    u8 i;

    TFT_CS_EN();
	TFT_RS_L();
    pap_write(0xA1);
    TFT_RS_H();
    printf("\nddb:");
    for(i=0;i<5;i++)
    {
        info[i] = pap_read();
        printf("%02x ",info[i]);
    }
	TFT_CS_DIS();
}

void LCD_Read_DispMode()
{
    u8 dat;
    LCD_WR_REG(0x0d);
    dat = LCD_RD_DATA();
    printf("\ndispmode:%02x",dat);
}


#define Delay_10ms(s)  delay(s*50000)//1 clock = 20ns

void tft_init(void)
{
    pap_init();

    printf("\nlcd init ");
	LCD_Configuration();
    tft_reset();

	//LCD_FSMCConfiguration();
	delay_2ms(25); /* delay 50 ms */
	delay_2ms(25); /* delay 50 ms */

	/* 命令E2H Set PLL MN*/
	LCD_WR_REG(0x00E2);
	/* 参数1 设置VCO频率
	   SSD1963外部晶振频率为10MHz，VCO要求大有250MHz
	   36倍频，最后得360MHz*/
	LCD_WR_DATA(0x0023);
	/* 参数2 设置PLL频率
	   M=2, PLL=360/(M+1)=120MHz*/
	LCD_WR_DATA(0x0002);
	/* 参数3 使设置生效*/
	LCD_WR_DATA(0x0004);

	LCD_WR_REG(0x00E0);  	/* Set PLL */
	LCD_WR_DATA(0x0001);   /* PLL enable  */
	Delay_10ms(10);	       /* 延时100ms   */
	LCD_WR_REG(0x00E0);
	LCD_WR_DATA(0x0003);
	Delay_10ms(10);

	LCD_WR_REG(0x0001);   /*  软件复位命令，没有参数 */
	Delay_10ms(10);

	LCD_WR_REG(0x00E6);	  /*  Set LSHIFT Frequency */
	LCD_WR_DATA(0x0004);
	LCD_WR_DATA(0x0093);
	LCD_WR_DATA(0x00e0);

	LCD_WR_REG(0x00B0);	      	   /*命令Set LCD Mode */
	LCD_WR_DATA(0x0000);
	LCD_WR_DATA(0x0000);
	LCD_WR_DATA((HDP>>8)&0X00FF);  /*Set the horizontal panel size*/
	LCD_WR_DATA(HDP&0X00FF);
    LCD_WR_DATA((VDP>>8)&0X00FF);  /*Set the horizontal panel size*/
	LCD_WR_DATA(VDP&0X00FF);
    LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00B4);	           /*Set Horizontal Period */
	LCD_WR_DATA((HT>>8)&0X00FF);  //Set HT
	LCD_WR_DATA(HT&0X00FF);
	LCD_WR_DATA((HPS>>8)&0X00FF);  //Set HPS
	LCD_WR_DATA(HPS&0X00FF);
	LCD_WR_DATA(HPW);			   //Set HPW
	LCD_WR_DATA((LPS>>8)&0X00FF);  //Set HPS
	LCD_WR_DATA(LPS&0X00FF);
	LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00B6);	           //VSYNC
	LCD_WR_DATA((VT>>8)&0X00FF);   //Set VT
	LCD_WR_DATA(VT&0X00FF);
	LCD_WR_DATA((VPS>>8)&0X00FF);  //Set VPS
	LCD_WR_DATA(VPS&0X00FF);
	LCD_WR_DATA(VPW);			   //Set VPW
	LCD_WR_DATA((FPS>>8)&0X00FF);  //Set FPS
	LCD_WR_DATA(FPS&0X00FF);

	LCD_WR_REG(0x00BA);
	LCD_WR_DATA(0x0005);           //GPIO[3:0] out 1

	LCD_WR_REG(0x00B8);
	LCD_WR_DATA(0x0007);    //GPIO3=input, GPIO[2:0]=output
	LCD_WR_DATA(0x0001);    //GPIO0 normal

	LCD_WR_REG(0x0036);       //rotation
	LCD_WR_DATA(0x0008);

	LCD_WR_REG(0x00F0);     //pixel data interface
	LCD_WR_DATA(0x0003);


	//Delay_10ms(50);
	LCD_WR_REG(0x0029);        //display on

	LCD_WR_REG(0x00BE);        //set PWM for B/L
	LCD_WR_DATA(0x0006);
	LCD_WR_DATA(0x0080);

	LCD_WR_DATA(0x0001);
	LCD_WR_DATA(0x00f0);
	LCD_WR_DATA(0x0000);
	LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00d0);//设置动态背光控制配置
	LCD_WR_DATA(0x000d);


	get_pixel_data_interface();
    //////////////////
    u8 info[7];
    u8 i;
    LCD_WR_REG(0x00B1);
    for(i=0;i<7;i++)
        info[i] = LCD_RD_DATA();
    printf("\nreg b1:");
    printf_buf(info,7);

    LCD_Read_DDB();
    LCD_Read_DispMode();
    //while(1)
    {
        LCD_Clear(0xF800);
        delay_2ms(500);
//        LCD_Clear(0x07E0);
//        delay_2ms(500);
//        LCD_Clear(0x001F);
//        delay_2ms(500);
    }
    printf("\ntft init out");
}



const L0_PARA_CFG lcd_7_inch_para =
{
    800+20,
    20,
    20,
    800,

    480+20,
    20,
    20,
    0,
    480,
};

void lcd_SetRange(u16 xs,u16 xe,u16 ys,u16 ye);

u8 r_tab[256],g_tab[256],b_tab[256];
void imb_disp_init()
{
    DISP_DEV_CFG disp_dev;
    CGAIN gain;

    printf("\nlcdpanel 7 inch init");
    tft_init();
    disp_dev.drive_mode = MODE_MCU;
    disp_dev.data_width = PORT_16BITS;
    disp_dev.out_format = FORMAT_RGB565;
    disp_dev.ncycle = ONE_CYCLE;
    disp_dev.para = (L0_PARA_CFG *)&lcd_7_inch_para;
    disp_dev.hook = lcd_SetRange;
    disp_dev.scan_mode = NON_INTERLACED;

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

    memset(r_tab,0x00,256);
    memset(g_tab,0xff,256);
    memset(b_tab,0xff,256);

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
    image->height = 480;
//    image->buff_width = 800;
//    image->buff_height = 480;
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
