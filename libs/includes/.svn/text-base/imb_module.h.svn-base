#ifndef __IMB_MODULE_H__
#define __IMB_MODULE_H__

#include "typedef.h"
#include "dsc_ram.h"

//小屏UI设为1，大屏的设为0
#define DYNAMIC_EFFECT_EN   0//动态效果使能

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
	LAYER_IMAGE0,			//图片层0(没有透明度)
	LAYER_IMAGE1,			//图片层1
	LAYER_OSD0_WIN0,		//OSD层0 窗口0
	LAYER_OSD0_WIN1,		//OSD层0 窗口1
	LAYER_OSD0_WIN2,		//OSD层0 窗口2
	LAYER_OSD1_WIN0,		//OSD层1 窗口0
	LAYER_OSD1_WIN1,		//OSD层1 窗口1
	LAYER_OSD1_WIN2,		//OSD层1 窗口2
	LAYER_IMAGE1_MASK,      //MASK层(图片层1)
}LAYER_TYPE;//图层标号
//说明：
//1.osd层一共有两层,每层osd最多有三个窗口，这三个窗口在垂直方向上不能交叉,不同层的osd在垂直方向可以相互交叉
//2.除了图片层0没有透明度，其它各层都有透明度
//3.各图层还需要遵循对齐的原则
//  图片层x,y,width,height都需要4对齐
//  OSD层x,y,width,height都需要2对齐

typedef struct
{
	u16 x;
	u16 y;
}TEXTPOS;
extern TEXTPOS text_pos;

//------------------------------------------------//
//-------------------AVOUT配置--------------------//
//------------------------------------------------//
//<AVOUT制式>
#define CVE_NTSC_27MHZ  0x01   //720*480
#define CVE_NTSC_36MHZ  0x02   //960*480
#define CVE_PAL_27MHZ   0x04   //720*576
#define CVE_PAL_36MHZ   0x08   //960*576
#define CVE_COLOR_BAR   0x10   //彩条模式


//<OSD层标识>
#define OSD0            0x00
#define OSD1            0x01
#define OSD_ALL         0x02
#define OSD1_B2         0x03   //2bit模式，只对OSD1有效，窗口宽度16 bytes对齐
#define WINDOW0         0x00
#define WINDOW1         0x10
#define WINDOW2         0x20

//<显示设备>
enum
{
    DISP_RGB,
    DISP_MCU,
    DISP_HDMI,
};

//<扫描方式>
#define NON_INTERLACED      0x00//非隔行
#define INTERLACED_TIMING   0x01//时序隔行
#define INTERLACED_DATA     0x02//数据隔行
#define INTERLACED_ALL      0x03//时序数据隔行

//<模式>
#define MODE_RGB_SYNC   0x00  //无显存 连续帧  接HSYNC VSYNC信号
#define MODE_RGB_DE     0x01  //无显存 连续帧  接DE信号
#define MODE_MCU        0x02  //带显存 单帧
#define MODE_HDMI       0x03  //HDMI显示
#define MODE_AVOUT      0x04  //AV OUT显示
#define MODE_CCIR656    0x05  //嵌入同步信号(8BITS+DCLK、不需要HSYNC、VSYNC信号)

//<位宽>
#define PORT_1BIT       0x00  //AV OUT
#define PORT_3BITS      0x01
#define PORT_6BITS      0x03
#define PORT_8BITS      0x07
#define PORT_9BITS      0x0f
#define PORT_16BITS     0x1f
#define PORT_18BITS     0x3f
#define PORT_24BITS     0x7f

//<时钟数>
#define ONE_CYCLE       0x00   //RGB-24BIT、MCU-16BIT
#define TWO_CYCLE       0x01   //MCU-8BIT
#define THREE_CYCLE     0x02   //RGB-8BIT
#define FOUR_CYCLE      0x03   //RGB-8BIT+dummy

//<输出数据格式>
#define FORMAT_RGB333   0x01
#define FORMAT_RGB666   0x02
#define FORMAT_RGB888   0x04
#define FORMAT_RGB565   0x08
#define FORMAT_RAWPOS   0x10   //正向 R->G->B
#define FORMAT_RAWNEG   0x20   //逆向 B->G->R
#define FORMAT_YUV888   0x40
#define FORMAT_YUV422   0x80
#define FORMAT_ITU656   0x180  //YUV422同时使能

//<信号极性>
#define DEFAULT_POSITIVE    0x00
#define DCLK_NEGATIVE       0x01
#define HSYNC_NEGATIVE      0x02
#define VSYNC_NEGATIVE      0x04



typedef struct
{
    u16 h_total_width;  //水平时钟总数(Horizontal Line/HSD period)
    u16 h_sync_width;   //水平同步时钟(HSYNC pulse width/HSD pulse width)
    u16 h_act_start;    //水平起始时钟(HSYNC blanking/HSD back porch)
    u16 h_act_width;    //水平像素(Horizontal display area/HSD display period)
    u16 v_total_height; //垂直时钟总数(Vertical display area/VSD period time)
    u16 v_sync_height;  //垂直同步时钟(VSYNC pulse width)
    u16 v_act_start;    //垂直起始时钟(VSYNC Blanking Odd field/VSD back porch Odd filed)
    u16 v_act_start1;   //垂直起始时钟(隔行模式)(VSYNC Blanking Even field/VSD back porch Even filed)
    u16 v_act_width;    //垂直像素(Vertical display area)
}L0_PARA_CFG;

//OSD图层参数
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

//IMAGE图层参数
typedef struct
{
    u8 layer;
    u8 enable;
    u8 alpha;         //只对图层1有效
    u8 mask_en;       //只对图层1有效
    u8 buf_num;
    u16 x;            //图层x坐标
    u16 y;            //图层y坐标
    u16 width;        //图层宽度
    u16 height;       //图层高度
	u16 buff_x;		  //显示buf起始x坐标
	u16 buff_y;       //显示buf起始y坐标
    u16 buff_width;   //图片宽度
    u16 buff_height;  //图片高度
    u8 *buf;
    u8 *mask_buf;     //只对图层1有效
    struct dsc_ram *dsc_ram;
}IMGLAYER;

//MASK图层参数
typedef struct
{
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u8  *buf;
    u8  enable;
}MASKLAYER;

//颜色校正
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

//奇行相位
#define ODD_PHASE_R     0x00
#define ODD_PHASE_G     0x01
#define ODD_PHASE_B     0x02
//偶行相位
#define EVEN_PHASE_R    0x00
#define EVEN_PHASE_G    0x10
#define EVEN_PHASE_B    0x20

//<常用预设频率>
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

//<时钟选择以及分频:使用IMB_EXTERNAL有效>
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

使用外部频率配置
dclk_freq = IMB_EXTERNAL;
dclk_divider = PLL_CLK|PLL0_CLK|DIVA_3|DIVB_1|DIVC_2;

dclk_freq = IMB_EXTERNAL;
dclk_divider = SYS_CLK|DIVA_1|DIVB_4|DIVC_3;

使用内部频率配置
dclk_freq = IMB_INTERNAL|DCLK_27MHz;
dclk_divider = 0;

注:AVOUT模式的频率在内部固定配置，设置dclk_freq、dclk_divider无效
*/

typedef struct
{
    u16 out_format;        //输出数据格式
    u8 drive_mode;         //驱动类型
    u8 avout_spec;         //AVOUT规格(PAL/NTSC)
    u8 data_width;         //数据接口位宽
    u8 isr_prior;          //中断优先级
    u8 ncycle;             //输出时钟数(1~4 clk)
    u8 signal_polarity;    //信号极性
    u8 scan_mode;          //扫描方式 非隔行/时序隔行/数据隔行
    u8 dclk_freq;          //DCLK频率选择
    u16 dclk_divider;       //DCLK分频系数
    u32 bkcolor;           //背景颜色
    u8 *r_gma_tab;         //red gamma表
    u8 *g_gma_tab;         //green gamma表
    u8 *b_gma_tab;         //blue gamma表
    u8 raw_phase;          //奇偶行初始相位 只对RAW模式有效
    L0_PARA_CFG *para;     //时钟配置
    CGAIN *gain;           //颜色增益
    void (*hook) (u16 x,u16 y,u16 w,u16 h);   //配置控制器，只针对MCU接口的屏
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

//OSD图层颜色定义
//#define COLOR_LUCENCY   0      //透明
//#define COLOR_YELLOW    5      //黄色
//#define COLOR_RED       35     //红色
//#define COLOR_GREEN     225    //绿色
//#define COLOR_BLUE		235    //蓝色
//#define COLOR_WHITE		245    //白色
//#define COLOR_GRAY      250    //灰色
//#define COLOR_BLACK		255    //黑色

//显存
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
#define ADJUST_DIS      0x19//禁止调试
#define ADJUST_EN       0x1a//使能调试

/*----------------------------------------------------------------------------*/
/*                            IMB Module Function List                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*调用流程：imb_init->imb_image_cfg/imb_osd_win_cfg->imb_start                  */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**@brief   初始化IMB模块
   @param   dev:设备参数
   @return  无
   @note    void imb_init(DISP_DEV_CFG* dev);
*/
/*----------------------------------------------------------------------------*/
extern void imb_init(DISP_DEV_CFG* dev);
/*----------------------------------------------------------------------------*/
/**@brief   获取IMB句柄
   @param   无
   @return  DISP_DEV_CFG结构
   @note    DISP_DEV_CFG imb_get_dev_hdl();
*/
/*----------------------------------------------------------------------------*/
extern DISP_DEV_CFG *imb_get_dev_hdl();
/*----------------------------------------------------------------------------*/
/**@brief   配置OSD颜色表
   @param   osd：osd层索引 osd_tab：osd的颜色表 num：颜色的数量
   @return  无
   @note    void imb_osd_pal_cfg(u8 osd, const u32 *osd_tab,u16 num);
*/
/*----------------------------------------------------------------------------*/
extern void imb_osd_pal_cfg(u8 osd, const u32 *osd_tab,u16 num);
/*----------------------------------------------------------------------------*/
/**@brief   获取图片层句柄
   @param   layer:图层序号
   @return  图片层句柄
   @note    IMGLAYER *imb_get_image_hdl(u8 layer);
*/
/*----------------------------------------------------------------------------*/
extern IMGLAYER *imb_get_image_hdl(u8 layer);
/*----------------------------------------------------------------------------*/
/**@brief   图片层配置
   @param   img:图片层句柄
   @return  无
   @note    void imb_image_cfg(IMGLAYER *img);
*/
/*----------------------------------------------------------------------------*/
extern void imb_image_cfg(IMGLAYER *img);
/*----------------------------------------------------------------------------*/
/**@brief   获取OSD层句柄
   @param   layer:图层序号
   @return  OSD层句柄
   @note    OSDWIN *imb_get_osd_hdl(u8 layer);
*/
/*----------------------------------------------------------------------------*/
extern OSDWIN *imb_get_osd_hdl(u8 layer);
/*----------------------------------------------------------------------------*/
/**@brief   OSD层配置
   @param   oshdl:OSD层句柄
   @return  无
   @note    void imb_osd_win_cfg(OSDWIN *osdhdl);
*/
/*----------------------------------------------------------------------------*/
extern void imb_osd_win_cfg(OSDWIN *osdhdl);
/*----------------------------------------------------------------------------*/
/**@brief   启动IMB模块
   @param   无
   @return  无
   @note    void imb_start(void);
*/
/*----------------------------------------------------------------------------*/
extern void imb_start(void);
/*----------------------------------------------------------------------------*/
/**@brief   打开IMB模块
   @param   无
   @return  无
   @note    void imb_open(void);
*/
/*----------------------------------------------------------------------------*/
extern void imb_open(void);
/*----------------------------------------------------------------------------*/
/**@brief   关闭IMB模块
   @param   无
   @return  无
   @note    void imb_close(void);
*/
/*----------------------------------------------------------------------------*/
extern void imb_close(void);
/*----------------------------------------------------------------------------*/
/**@brief   显示buf切换
   @param   无
   @return  无
   @note    void imb_osd_buf_switch(u8 layer,u8 buf_index);
*/
/*----------------------------------------------------------------------------*/
extern void imb_osd_buf_switch(u8 layer,u8 buf_index);
/*----------------------------------------------------------------------------*/
/**@brief   颜色调节
   @param   cmd:命令 val:参数
   @return  无
   @note    void imb_color_cfg(u8 cmd,u32 val);
*/
/*----------------------------------------------------------------------------*/
extern void imb_color_cfg(u8 cmd,u32 val);
/*----------------------------------------------------------------------------*/
/**@brief   获取颜色调节参数
   @param   无
   @return  CGAIN结构
   @note    CGAIN *imb_get_gain();
*/
/*----------------------------------------------------------------------------*/
extern CGAIN *imb_get_gain();

#endif
