#ifndef _CLOCK_
#define _CLOCK_
#include "typedef.h"


///PLL ·ÖÆµÅäÖÃ

#define   PLL_CLOCK_DIV1	0   //1*1
#define   PLL_CLOCK_DIV2	4   //1*2
#define   PLL_CLOCK_DIV3	1   //3*1
#define   PLL_CLOCK_DIV4	8   //1*4
#define   PLL_CLOCK_DIV5	2   //5*1
#define   PLL_CLOCK_DIV6	5   //3*2
#define   PLL_CLOCK_DIV7	3   //7*1
#define   PLL_CLOCK_DIV8	0xc //1*8
#define   PLL_CLOCK_DIV10	6   //5*2
#define   PLL_CLOCK_DIV12	9   //3*4
#define   PLL_CLOCK_DIV14	7   //7*2
#define   PLL_CLOCK_DIV20	0xa //5*4
#define   PLL_CLOCK_DIV24	0xd //3*8
#define   PLL_CLOCK_DIV28	0xb //7*4
#define   PLL_CLOCK_DIV40	0xe //5*8
#define   PLL_CLOCK_DIV56	0xf //7*8



enum
{
    PH13_CLOCK_OUT_XOSC0 = 0,
    PH13_CLOCK_OUT_PLL_24M,
    PH13_CLOCK_OUT_IRTC_CLK,
    PH13_CLOCK_OUT_PLL01_12M,
    PH13_CLOCK_OUT_PLL0,
    PH13_CLOCK_OUT_PLL1,
    PH13_CLOCK_OUT_PLL2,
    PH13_CLOCK_OUT_SYS,
    PH13_CLOCK_OUT_DISABLE,
};

enum
{
    PH14_CLOCK_OUT_XOSC0 = 0,
    PH14_CLOCK_OUT_PLL_48M,
    PH14_CLOCK_OUT_IRTC_CLK,
    PH14_CLOCK_OUT_PLL01_12M,
    PH14_CLOCK_OUT_PLL0,
    PH14_CLOCK_OUT_PLL1,
    PH14_CLOCK_OUT_PLL2,
    PH14_CLOCK_OUT_SYS,
    PH14_CLOCK_OUT_DISABLE,
};

enum
{
    PH15_CLOCK_OUT_XOSC0 = 0,
    PH15_CLOCK_OUT_HTC_CLK,
    PH15_CLOCK_OUT_IRTC_CLK,
    PH15_CLOCK_OUT_PLL01_12M,
    PH15_CLOCK_OUT_PLL0,
    PH15_CLOCK_OUT_PLL1,
    PH15_CLOCK_OUT_PLL2,
    PH15_CLOCK_OUT_SYS,
    PH15_CLOCK_OUT_DISABLE,
};

//pll0|SRC_OSC0|hc_dis|ipin_en


///*************************************


enum
{
    SRC_OSC0 = 0,//=xosc
    SRC_OSC1,//dv12 not support
    SRC_OSC2,//dv12 not support
    SRC_RTC_OSC = 3,
    SRC_HTC,
    SRC_PLL01_12M = 5, //only for pll2

    SRC_PLL_12M,//only for pll2

    SRC_OSC_CLK,
    SRC_XOSC,
    SRC_RTC,
    SRC_PA4,
};


///PLL配置项
#define PLL0                        (0<<5)
#define PLL1                        (1<<5)
#define PLL2                        (2<<5)
///osc 1pin 配置项
#define OSC_1PIN_DISABLE            (0<<4)
#define OSC_1PIN_ENABLE             (1<<4)
///osc hc 配置项
#define OSC_HC_DISABLE              (0<<3)
#define OSC_HC_ENABLE               (1<<3)



///*************************************

#define PLL0_SRC_XOSC   (PLL0|SRC_OSC0)
//#define PLL0_SRC_OSC1   (PLL0|SRC_OSC1)
//#define PLL0_SRC_OSC2   (PLL0|SRC_OSC2)
#define PLL0_SRC_RTC    (PLL0|SRC_RTC_OSC)
#define PLL0_SRC_HTC    (PLL0|SRC_HTC)
//
#define PLL1_SRC_OSC0   (PLL1|SRC_OSC0)
//#define PLL1_SRC_OSC1   (PLL1|SRC_OSC1)
//#define PLL1_SRC_OSC2   (PLL1|SRC_OSC2)
#define PLL1_SRC_RTC    (PLL1|SRC_RTC_OSC)
#define PLL1_SRC_HTC    (PLL1|SRC_HTC)
//
#define PLL2_SRC_OSC0   (PLL2|SRC_OSC0)
//#define PLL2_SRC_OSC1   (PLL2|SRC_OSC1)
//#define PLL2_SRC_OSC2   (PLL2|SRC_OSC2)
#define PLL2_SRC_RTC    (PLL2|SRC_RTC_OSC)
#define PLL2_SRC_PLL0_12M	(PLL2|SRC_PLL0_12M)

//#define PLL0_CLOCK_DIV(x)        CLK_CON0 = ((CLK_CON0 & ~(BIT(24)|BIT(25)|BIT(26)|BIT(27))) | ((x & 0xf)<<24))
//#define PLL1_CLOCK_DIV(x)        CLK_CON0 = ((CLK_CON0 & ~(BIT(20)|BIT(21)|BIT(22)|BIT(23))) | ((x & 0xf)<<20))
//#define PLL2_CLOCK_DIV(x)        CLK_CON2 = ((CLK_CON2 & ~(BIT(24)|BIT(25)|BIT(26)|BIT(27))) | ((x & 0xf)<<24))



//#endif

typedef struct _S_CLK_INFO
{
    u32 s_pll_clk;
    u32 s_osc_frq;
    u32 s_sys_clk;
    u32 s_apb_clk;
    u32 apc_clk_src;
    u32 apc_clk;
    u32 s_pll_src;
    u32 s_pll;
    u32 s_osc_hc_en;
    u32 s_osc_1pin_en;
} S_CLK_INFO;

#ifndef FPGA
u32 sys_clk_init(u32 src, u32 osc, u32 pll_clk);
void init_clk_info(u32 info);
S_CLK_INFO get_sys_clk_info(void);
u32 get_sys_clk();
u32 get_apb_clk();
u32 get_pll_clk(u32 pll);
u32 apb_clk_div(u8 div);
void pll_init(u32 pll_osc, u32 osc_frq, u32 pll_target);
u32 check_pll_is_en(u32 pll);
inline u32 get_pll_clk_p(u32 pll);
void sys_clk_div(u8 div);
u32 pll_clk_div(u32 pll_src, u8 div);
u32 pll_apc_sel(u32 pll_src);
void pll_96m_sel(u32 pll_src);
void PA13_clk_out(u8 src);
void PA14_clk_out(u8 src);
void PA15_clk_out(u8 src);
#endif




#endif
