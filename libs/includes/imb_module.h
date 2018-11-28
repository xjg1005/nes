#ifndef __IMB_MODULE_H__
#define __IMB_MODULE_H__

#include "typedef.h"
#include "dsc_ram.h"

//С��UI��Ϊ1����������Ϊ0
#define DYNAMIC_EFFECT_EN   0//��̬Ч��ʹ��

struct imb_isr_work{
	void * param;
	void (*handler)(void * param);
};

#define REGISTER_IMB_WORK(hdl, p)  const struct imb_isr_work imb_isr_##hdl \
                                        __attribute__((section(".imb_isr_work"))) = {\
										.param = (void *)p,\
										.handler=hdl\
									}

#define SDRAM_ADDR(addr)  ((u32)(0x2000000+addr))
#define SET_REG(val,pos)  ((u32)val << pos)

typedef enum
{
	LAYER_IMAGE0,			//ͼƬ��0(û��͸����)
	LAYER_IMAGE1,			//ͼƬ��1
	LAYER_OSD0_WIN0,		//OSD��0 ����0
	LAYER_OSD0_WIN1,		//OSD��0 ����1
	LAYER_OSD0_WIN2,		//OSD��0 ����2
	LAYER_OSD1_WIN0,		//OSD��1 ����0
	LAYER_OSD1_WIN1,		//OSD��1 ����1
	LAYER_OSD1_WIN2,		//OSD��1 ����2
	LAYER_IMAGE1_MASK,      //MASK��(ͼƬ��1)
}LAYER_TYPE;//ͼ����
//˵����
//1.osd��һ��������,ÿ��osd������������ڣ������������ڴ�ֱ�����ϲ��ܽ���,��ͬ���osd�ڴ�ֱ��������໥����
//2.����ͼƬ��0û��͸���ȣ��������㶼��͸����
//3.��ͼ�㻹��Ҫ��ѭ�����ԭ��
//  ͼƬ��x,y,width,height����Ҫ4����
//  OSD��x,y,width,height����Ҫ2����

typedef struct
{
	u16 x;
	u16 y;
}TEXTPOS;
extern TEXTPOS text_pos;

//------------------------------------------------//
//-------------------AVOUT����--------------------//
//------------------------------------------------//
//<AVOUT��ʽ>
#define CVE_NTSC_27MHZ  0x01   //720*480
#define CVE_NTSC_36MHZ  0x02   //960*480
#define CVE_PAL_27MHZ   0x04   //720*576
#define CVE_PAL_36MHZ   0x08   //960*576
#define CVE_COLOR_BAR   0x10   //����ģʽ


//<OSD���ʶ>
#define OSD0            0x00
#define OSD1            0x01
#define OSD_ALL         0x02
#define OSD1_B2         0x03   //2bitģʽ��ֻ��OSD1��Ч�����ڿ��16 bytes����
#define WINDOW0         0x00
#define WINDOW1         0x10
#define WINDOW2         0x20

//<��ʾ�豸>
enum
{
    DISP_RGB,
    DISP_MCU,
    DISP_HDMI,
};

//<ɨ�跽ʽ>
#define NON_INTERLACED      0x00//�Ǹ���
#define INTERLACED_TIMING   0x01//ʱ�����
#define INTERLACED_DATA     0x02//���ݸ���
#define INTERLACED_ALL      0x03//ʱ�����ݸ���

//<ģʽ>
#define MODE_RGB_SYNC   0x00  //���Դ� ����֡  ��HSYNC VSYNC�ź�
#define MODE_RGB_DE     0x01  //���Դ� ����֡  ��DE�ź�
#define MODE_MCU        0x02  //���Դ� ��֡
#define MODE_HDMI       0x03  //HDMI��ʾ
#define MODE_AVOUT      0x04  //AV OUT��ʾ
#define MODE_CCIR656    0x05  //Ƕ��ͬ���ź�(8BITS+DCLK������ҪHSYNC��VSYNC�ź�)

//<λ��>
#define PORT_1BIT       0x00  //AV OUT
#define PORT_3BITS      0x01
#define PORT_6BITS      0x03
#define PORT_8BITS      0x07
#define PORT_9BITS      0x0f
#define PORT_16BITS     0x1f
#define PORT_18BITS     0x3f
#define PORT_24BITS     0x7f

//<ʱ����>
#define ONE_CYCLE       0x00   //RGB-24BIT��MCU-16BIT
#define TWO_CYCLE       0x01   //MCU-8BIT
#define THREE_CYCLE     0x02   //RGB-8BIT
#define FOUR_CYCLE      0x03   //RGB-8BIT+dummy

//<������ݸ�ʽ>
#define FORMAT_RGB333   0x01
#define FORMAT_RGB666   0x02
#define FORMAT_RGB888   0x04
#define FORMAT_RGB565   0x08
#define FORMAT_RAWPOS   0x10   //���� R->G->B
#define FORMAT_RAWNEG   0x20   //���� B->G->R
#define FORMAT_YUV888   0x40
#define FORMAT_YUV422   0x80
#define FORMAT_ITU656   0x180  //YUV422ͬʱʹ��

//<�źż���>
#define DEFAULT_POSITIVE    0x00
#define DCLK_NEGATIVE       0x01
#define HSYNC_NEGATIVE      0x02
#define VSYNC_NEGATIVE      0x04



typedef struct
{
    u16 h_total_width;  //ˮƽʱ������(Horizontal Line/HSD period)
    u16 h_sync_width;   //ˮƽͬ��ʱ��(HSYNC pulse width/HSD pulse width)
    u16 h_act_start;    //ˮƽ��ʼʱ��(HSYNC blanking/HSD back porch)
    u16 h_act_width;    //ˮƽ����(Horizontal display area/HSD display period)
    u16 v_total_height; //��ֱʱ������(Vertical display area/VSD period time)
    u16 v_sync_height;  //��ֱͬ��ʱ��(VSYNC pulse width)
    u16 v_act_start;    //��ֱ��ʼʱ��(VSYNC Blanking Odd field/VSD back porch Odd filed)
    u16 v_act_start1;   //��ֱ��ʼʱ��(����ģʽ)(VSYNC Blanking Even field/VSD back porch Even filed)
    u16 v_act_width;    //��ֱ����(Vertical display area)
}L0_PARA_CFG;

//OSDͼ�����
typedef struct _OSDWIN
{
	u16 x;
	u16 y;
	u16 width;
	u16 height;
	u8 *buf;
//#if DYNAMIC_EFFECT_EN
	u8 *buf_disp;
	u8 *buf_nodisp;
//#endif
    u8 layer;
	u8 alpha;
	u8 enable;
}OSDWIN;

typedef struct _OSDLAYER
{
	u32 *palette;
	u16 item;
	OSDWIN win[3];
}OSDLAYER;

//IMAGEͼ�����
typedef struct
{
    u8 layer;
    u8 enable;
    u8 alpha;         //ֻ��ͼ��1��Ч
    u8 mask_en;       //ֻ��ͼ��1��Ч
    u8 buf_num;
    u16 x;            //ͼ��x����
    u16 y;            //ͼ��y����
    u16 width;        //ͼ����
    u16 height;       //ͼ��߶�
	u16 buff_x;		  //��ʾbuf��ʼx����
	u16 buff_y;       //��ʾbuf��ʼy����
    u16 buff_width;   //ͼƬ���
    u16 buff_height;  //ͼƬ�߶�
    u8 *buf;
    u8 *mask_buf;     //ֻ��ͼ��1��Ч
    struct dsc_ram *dsc_ram;
}IMGLAYER;

//MASKͼ�����
typedef struct
{
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u8  *buf;
    u8  enable;
}MASKLAYER;

//��ɫУ��
typedef struct
{
    s8 y_offs;
    s8 u_offs;
    s8 v_offs;
    u8 y_gain;
    u8 u_gain;
    u8 v_gain;
    s8 r_offs;
    s8 g_offs;
    s8 b_offs;
    u8 r_gain;
    u8 g_gain;
    u8 b_gain;
    u8 r_coe0;
    s8 r_coe1;
    s8 r_coe2;
    s8 g_coe0;
    u8 g_coe1;
    s8 g_coe2;
    s8 b_coe0;
    s8 b_coe1;
    u8 b_coe2;
    u16 r_gma;
    u16 g_gma;
    u16 b_gma;
}  CGAIN;

//������λ
#define ODD_PHASE_R     0x00
#define ODD_PHASE_G     0x01
#define ODD_PHASE_B     0x02
//ż����λ
#define EVEN_PHASE_R    0x00
#define EVEN_PHASE_G    0x10
#define EVEN_PHASE_B    0x20

//<����Ԥ��Ƶ��>
#define DCLK_6MHz		0x00
#define DCLK_9MHz       0x01
#define DCLK_10MHz      0x02
#define DCLK_12MHz      0x03
#define DCLK_24MHz      0x04
#define DCLK_27MHz      0x05
#define DCLK_36MHz      0x06
#define DCLK_40MHz      0x07

#define IMB_INTERNAL    0x00
#define IMB_EXTERNAL    0x10

//<ʱ��ѡ���Լ���Ƶ:ʹ��IMB_EXTERNAL��Ч>
#define USE_PLL_CLK   (0<<8)
#define USE_SYS_CLK   (1<<8)
#define USE_OTHER_CLK (2<<8)

#define PLL0_CLK  (0<<6)
#define PLL1_CLK  (1<<6)
#define PLL2_CLK  (2<<6)

#define DIVA_1    (0<<4)
#define DIVA_3    (1<<4)
#define DIVA_5    (2<<4)
#define DIVA_7    (3<<4)

#define DIVB_1    (0<<2)
#define DIVB_2    (1<<2)
#define DIVB_4    (2<<2)
#define DIVB_8    (3<<2)

#define DIVC_1    (0<<0)
#define DIVC_2    (1<<0)
#define DIVC_3    (2<<0)
#define DIVC_4    (3<<0)
/*
e.g

ʹ���ⲿƵ������
dclk_freq = IMB_EXTERNAL;
dclk_divider = PLL_CLK|PLL0_CLK|DIVA_3|DIVB_1|DIVC_2;

dclk_freq = IMB_EXTERNAL;
dclk_divider = SYS_CLK|DIVA_1|DIVB_4|DIVC_3;

ʹ���ڲ�Ƶ������
dclk_freq = IMB_INTERNAL|DCLK_27MHz;
dclk_divider = 0;

ע:AVOUTģʽ��Ƶ�����ڲ��̶����ã�����dclk_freq��dclk_divider��Ч
*/

typedef struct
{
    u16 out_format;        //������ݸ�ʽ
    u8 drive_mode;         //��������
    u8 avout_spec;         //AVOUT���(PAL/NTSC)
    u8 data_width;         //���ݽӿ�λ��
    u8 isr_prior;          //�ж����ȼ�
    u8 ncycle;             //���ʱ����(1~4 clk)
    u8 signal_polarity;    //�źż���
    u8 scan_mode;          //ɨ�跽ʽ �Ǹ���/ʱ�����/���ݸ���
    u8 dclk_freq;          //DCLKƵ��ѡ��
    u16 dclk_divider;       //DCLK��Ƶϵ��
    u32 bkcolor;           //������ɫ
    u8 *r_gma_tab;         //red gamma��
    u8 *g_gma_tab;         //green gamma��
    u8 *b_gma_tab;         //blue gamma��
    u8 raw_phase;          //��ż�г�ʼ��λ ֻ��RAWģʽ��Ч
    L0_PARA_CFG *para;     //ʱ������
    CGAIN *gain;           //��ɫ����
    void (*hook) (u16 x,u16 y,u16 w,u16 h);   //���ÿ�������ֻ���MCU�ӿڵ���
}DISP_DEV_CFG;

extern IMGLAYER image[2];
extern OSDLAYER osdlayer[2];
extern MASKLAYER mask;

extern u32 tempaddr;
extern u16 tempwidth,tempheight;

#define SET_A(addr,width,height)  tempaddr = addr; tempwidth = width; tempheight = height;
#define GET_AY()  (tempaddr)
#define GET_AU()  (tempaddr+tempwidth*tempheight)
#define GET_AV()  (tempaddr+tempwidth*tempheight*5/4)

//OSDͼ����ɫ����
//#define COLOR_LUCENCY   0      //͸��
//#define COLOR_YELLOW    5      //��ɫ
//#define COLOR_RED       35     //��ɫ
//#define COLOR_GREEN     225    //��ɫ
//#define COLOR_BLUE		235    //��ɫ
//#define COLOR_WHITE		245    //��ɫ
//#define COLOR_GRAY      250    //��ɫ
//#define COLOR_BLACK		255    //��ɫ

//�Դ�
#define BUF_DISP 	0
#define BUF_NODISP  1


//READ_IMB
#define GET_ALL         0x01
//ADJUST_IMB
#define SET_Y_OFFS      0x01
#define SET_U_OFFS      0x02
#define SET_V_OFFS      0x03
#define SET_Y_GAIN      0x04
#define SET_U_GAIN      0x05
#define SET_V_GAIN      0x06
#define SET_R_OFFS      0x07
#define SET_G_OFFS      0x08
#define SET_B_OFFS      0x09
#define SET_R_GAIN      0x0a
#define SET_G_GAIN      0x0b
#define SET_B_GAIN      0x0c
#define SET_R_COE0      0x0d
#define SET_R_COE1      0x0e
#define SET_R_COE2      0x0f
#define SET_G_COE0      0x10
#define SET_G_COE1      0x11
#define SET_G_COE2      0x12
#define SET_B_COE0      0x13
#define SET_B_COE1      0x14
#define SET_B_COE2      0x15
#define SET_R_GMA       0x16
#define SET_G_GMA       0x17
#define SET_B_GMA       0x18
#define ADJUST_DIS      0x19//��ֹ����
#define ADJUST_EN       0x1a//ʹ�ܵ���

/*----------------------------------------------------------------------------*/
/*                            IMB Module Function List                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*�������̣�imb_init->imb_image_cfg/imb_osd_win_cfg->imb_start                  */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**@brief   ��ʼ��IMBģ��
   @param   dev:�豸����
   @return  ��
   @note    void imb_init(DISP_DEV_CFG* dev);
*/
/*----------------------------------------------------------------------------*/
extern void imb_init(DISP_DEV_CFG* dev);
/*----------------------------------------------------------------------------*/
/**@brief   ��ȡIMB���
   @param   ��
   @return  DISP_DEV_CFG�ṹ
   @note    DISP_DEV_CFG imb_get_dev_hdl();
*/
/*----------------------------------------------------------------------------*/
extern DISP_DEV_CFG *imb_get_dev_hdl();
/*----------------------------------------------------------------------------*/
/**@brief   ����OSD��ɫ��
   @param   osd��osd������ osd_tab��osd����ɫ�� num����ɫ������
   @return  ��
   @note    void imb_osd_pal_cfg(u8 osd, const u32 *osd_tab,u16 num);
*/
/*----------------------------------------------------------------------------*/
extern void imb_osd_pal_cfg(u8 osd, const u32 *osd_tab,u16 num);
/*----------------------------------------------------------------------------*/
/**@brief   ��ȡͼƬ����
   @param   layer:ͼ�����
   @return  ͼƬ����
   @note    IMGLAYER *imb_get_image_hdl(u8 layer);
*/
/*----------------------------------------------------------------------------*/
extern IMGLAYER *imb_get_image_hdl(u8 layer);
/*----------------------------------------------------------------------------*/
/**@brief   ͼƬ������
   @param   img:ͼƬ����
   @return  ��
   @note    void imb_image_cfg(IMGLAYER *img);
*/
/*----------------------------------------------------------------------------*/
extern void imb_image_cfg(IMGLAYER *img);
/*----------------------------------------------------------------------------*/
/**@brief   ��ȡOSD����
   @param   layer:ͼ�����
   @return  OSD����
   @note    OSDWIN *imb_get_osd_hdl(u8 layer);
*/
/*----------------------------------------------------------------------------*/
extern OSDWIN *imb_get_osd_hdl(u8 layer);
/*----------------------------------------------------------------------------*/
/**@brief   OSD������
   @param   oshdl:OSD����
   @return  ��
   @note    void imb_osd_win_cfg(OSDWIN *osdhdl);
*/
/*----------------------------------------------------------------------------*/
extern void imb_osd_win_cfg(OSDWIN *osdhdl);
/*----------------------------------------------------------------------------*/
/**@brief   ����IMBģ��
   @param   ��
   @return  ��
   @note    void imb_start(void);
*/
/*----------------------------------------------------------------------------*/
extern void imb_start(void);
/*----------------------------------------------------------------------------*/
/**@brief   ��IMBģ��
   @param   ��
   @return  ��
   @note    void imb_open(void);
*/
/*----------------------------------------------------------------------------*/
extern void imb_open(void);
/*----------------------------------------------------------------------------*/
/**@brief   �ر�IMBģ��
   @param   ��
   @return  ��
   @note    void imb_close(void);
*/
/*----------------------------------------------------------------------------*/
extern void imb_close(void);
/*----------------------------------------------------------------------------*/
/**@brief   ��ʾbuf�л�
   @param   ��
   @return  ��
   @note    void imb_osd_buf_switch(u8 layer,u8 buf_index);
*/
/*----------------------------------------------------------------------------*/
extern void imb_osd_buf_switch(u8 layer,u8 buf_index);
/*----------------------------------------------------------------------------*/
/**@brief   ��ɫ����
   @param   cmd:���� val:����
   @return  ��
   @note    void imb_color_cfg(u8 cmd,u32 val);
*/
/*----------------------------------------------------------------------------*/
extern void imb_color_cfg(u8 cmd,u32 val);
/*----------------------------------------------------------------------------*/
/**@brief   ��ȡ��ɫ���ڲ���
   @param   ��
   @return  CGAIN�ṹ
   @note    CGAIN *imb_get_gain();
*/
/*----------------------------------------------------------------------------*/
extern CGAIN *imb_get_gain();

#endif
