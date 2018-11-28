#include "config.h"
//#include "usb_inc.h"
#include "clock.h"
//#include "string.h"

#ifndef __UBOOT
#define htc_delay1ms(t) delay((t)*1000)
#endif

static S_CLK_INFO s_clk_info;

typedef struct _PLL_CLK
{
    u32 pll0_target_clk;
    u32 pll0_out_clk;

    u32 pll1_target_clk;
    u32 pll1_out_clk;

    u32 pll2_target_clk;
    u32 pll2_out_clk;
} PLL_CLK_P;

static PLL_CLK_P pll_clk_p_frq;


static const u8 pll_div_tab[][4] =
{
    {1, 3, 5, 7,},
    {1, 2, 4, 8,},
};

static const u32 pll_div_arg[][4] =
{
    //pll_clk, pll_clk_p, div_1, div_2,

    {12, 480, 2, 3,},
    {20, 480, 1, 3,},
    {24, 480, 2, 2,},
    {40, 480, 1, 2,},
    {48, 480, 2, 1,},
    {60, 480, 0, 3,},
    {80, 480, 1, 1,},
    {96, 480, 2, 0,},
    {120, 480, 0, 2,},
    {160, 480, 1, 0,},
    {180, 540, 1, 0,},
    {200, 600, 1, 0,},
};
const u32 clk_div_tab[16]={1,3,5,7,2,6,10,14,4,12,20,28,8,24,40,56};

#define XOSC_HCEN   2
#define XOSC_1PININ     3
#define XOSC_EN     1



static void xosc_init(u32 info)
{
    if(info & OSC_1PIN_ENABLE)
    {
    //osc0 1pin
        PORTH_DIR |= BIT(0);
        PORTH_PD &= ~(BIT(0));
        PORTH_PU &= ~(BIT(0));
        PORTH_DIE &= ~(BIT(0));
        CLK_CON0 |= BIT(XOSC_1PININ);///osc0 1pin en
    }
    else
    {
        ///osc0 2pin
        PORTH_DIR |= BIT(0) | BIT(1);
        PORTH_PD &= ~(BIT(0) | BIT(1));
        PORTH_PU &= ~(BIT(0) | BIT(1));
        PORTH_DIE &= ~(BIT(0) | BIT(1));
        CLK_CON0 &= ~BIT(XOSC_1PININ);
    }

    if(info & OSC_HC_ENABLE)
    {
        ///osc0 hc en
        CLK_CON0 |= BIT(XOSC_HCEN);
    }
    else
    {
        ///osc0 hc sis
        CLK_CON0 &= ~BIT(XOSC_HCEN);
    }

    ///osc0_en
    CLK_CON0 |= BIT(XOSC_EN);
}

static void wr_irtc_reg(u8 con_type, u8 con_dat)
{
    IRTC_CON |= BIT(0);		//IRTC_CS
    IRTC_BUF = con_type;
    asm("ssync");

    while(!(IRTC_CON & BIT(1)));

    IRTC_BUF = con_dat;
    asm("ssync");

    while(!(IRTC_CON & BIT(1)));

    IRTC_CON &= ~BIT(0);		//IRTC_CS
}

static u8 rd_irtc_reg(u8 con_type)
{
    IRTC_CON |= BIT(0);		//IRTC_CS
    IRTC_BUF = con_type;
    asm("ssync");

    while(!(IRTC_CON & BIT(1)));

    IRTC_BUF = 0x00;
    asm("ssync");

    while(!(IRTC_CON & BIT(1)));

    IRTC_CON &= ~BIT(0);		//IRTC_CS

    return(IRTC_BUF);
}

static void rtc_osc_init(u32 info, u32 osc_frq)
{
    if(info & OSC_1PIN_ENABLE)
    {
        wr_irtc_reg(0X26, (rd_irtc_reg(0xa6) & ~BIT(7)));  //2pin select 1pin
        wr_irtc_reg(0X26, (rd_irtc_reg(0xa6) | BIT(6)));  //1pin mode en

        wr_irtc_reg(0X20, (rd_irtc_reg(0xa0) & ~BIT(0))); //osc_dxen 1pin en

        htc_delay1ms(1000);//delay 800ms

    }
    else
    {
        wr_irtc_reg(0X26, (rd_irtc_reg(0xa6) | BIT(7)));  //SSCCK 2pin select 2pin
        wr_irtc_reg(0X26, (rd_irtc_reg(0xa6) & ~BIT(6)));  //1pin mode disable

        if(osc_frq == 12000000L)
        {
            wr_irtc_reg(0X26, (rd_irtc_reg(0xa6)& ~BIT(5)));  //SD32K = 0 12M
        }
        else if(osc_frq == 32768L)
        {
            wr_irtc_reg(0X26, (rd_irtc_reg(0xa6) | BIT(5))); //SD32K
            //wr_irtc_reg(0x26,0xa5); //osc configure 2pinmode 32K mode
        }

        wr_irtc_reg(0X20, (rd_irtc_reg(0xa0) | BIT(0))); //osc_dxen double pin en
    }

    htc_delay1ms(1000);//delay 800ms

}

static void htc_init()
{
    if(HTC_CON & BIT(8))
    {
        return;
    }

    //turn on htc
    HTC_CON |= BIT(8);
    delay(100 * 5);
//    delay(2);          //large than 50us
    HTC_CON |= BIT(9);
    delay(100 * 5);
//    delay(2);          //large than 50us --~240us
}
static void osc_clk_init(u32 osc_src)
{
    u32 osc;
    osc = osc_src & 0x7;
    CLK_CON0 &= ~(3 << 4);

    switch(osc)
    {
        case SRC_OSC0:  //=xosc
            xosc_init(osc_src);
            break;

        case SRC_RTC:
            CLK_CON0 |= BIT(4);
            break;

        case SRC_PA4:
            CLK_CON0 |= BIT(4) | BIT(5);
            break;
    }
    htc_delay1ms(10);//delay 800ms
}
/*
*************************************************************
*Description: PLL0初始化函数
*Arguments  : pll_osc：时钟源及哪一路PLL，
              osc_frq：外部输入的时钟频率
               pll_target：pll配置的频率目标值
*Returns    : NULL
*Note       :
*By         :
*************************************************************
*/
static void pll0_init(u32 osc_src, u32 osc_frq, u32 pll_target)
{
    u16 nr, nf;

    u32 osc;
    osc = osc_src & 0x7;

    nr = osc_frq / 100000; //front scaler 100K

    if(nr == 0)
    {
        nr = 1;
    }

    pll_target /= 2;
    osc_frq /= nr;  //PLL reference clock
    nf = pll_target / osc_frq;

    if((pll_target - (osc_frq * nf)) <= (osc_frq / 2))    //rounding
    {
        nf -= 1;
    }

    nr -= 1;
    PLL_CON0 &= ~BIT(0);//关闭pll

    PLL_CON0 &= ~(0x3 << 2);

    PLL_CON1 &= ~(0xf << 0);

    switch(osc)
    {

        case SRC_RTC_OSC:
            PLL_CON0 |= BIT(3);
            break;

        case SRC_HTC:
            PLL_CON0 |= BIT(2) | BIT(3); // htc_clk
            break;

        case SRC_OSC0:
        case SRC_RTC:
        case SRC_PA4:
        case SRC_OSC_CLK:
            PLL_CON0 |= BIT(2);
            break;


            break;
    }

    PLL0_NR = nf;
    PLL0_NF = nr;

    PLL_CON0 |= BIT(0); //enable PLL0
}
/*
*************************************************************
*Description: PLL1初始化函数
*Arguments  : pll_osc：时钟源及哪一路PLL，
              osc_frq：外部输入的时钟频率
               pll_target：pll配置的频率目标值
*Returns    : NULL
*Note       :
*By         :
*************************************************************
*/
static void pll1_init(u32 osc_src, u32 osc_frq, u32 pll_target)
{
    u16 nr, nf;

    u32 osc;
    osc = osc_src & 0x7;

    nr = osc_frq / 100000; //front scaler 100K

    if(nr == 0)
    {
        nr = 1;
    }

    pll_target /= 2;
    osc_frq /= nr;  //PLL reference clock
    nf = pll_target / osc_frq;

    if((pll_target - (osc_frq * nf)) <= (osc_frq / 2))    //rounding
    {
        nf -= 1;
    }

    nr -= 1;
    PLL_CON0 &= ~BIT(1);//关闭pll1
    PLL_CON0 &= ~(0x3 << 4);
    PLL_CON1 &= ~(0xf << 4);

    switch(osc)
    {

        case SRC_RTC_OSC:
            PLL_CON0 |= BIT(5);
            break;

        case SRC_HTC:
            PLL_CON0 |= BIT(4) | BIT(5); // htc_clk
            break;

        case SRC_OSC0:
        case SRC_RTC:
        case SRC_PA4:
        case SRC_OSC_CLK:
            PLL_CON0 |= BIT(4);
            break;

    }

    PLL1_NR = nf;
    PLL1_NF = nr;

    PLL_CON0 |= BIT(1); //enable PLL1
}
/*
*************************************************************
*Description: PLL2初始化函数
*Arguments  : pll_osc：时钟源及哪一路PLL，
              osc_frq：外部输入的时钟频率

*Returns    : NULL
*Note       : PLL的目标值为固定的480M
*By         :
*************************************************************
*/
static void pll2_init(u32 osc_src, u32 osc_frq)
{
    u32 osc;
    osc = osc_src & 0x7;


    PLL_CON0 &= ~(0x3 << 16);

    PLL_CON1 &= ~(0xf << 8);

    switch(osc)
    {
        case SRC_PLL_12M:
            PLL_CON0 |= BIT(17);
            break;

        case SRC_PLL01_12M:
            PLL_CON0 |= BIT(16) | BIT(17);
            break;

        case SRC_OSC0:
        case SRC_RTC:
        case SRC_PA4:
        case SRC_OSC_CLK:
            PLL_CON0 |= BIT(16);
            break;


    }

    //pll2 init
    HUSB_COM_CON |= BIT(0);
    HUSB_COM_CON |= BIT(3);   //pll_icp
    HUSB_COM_CON |= BIT(4);   //pll_rlf
    HUSB_COM_CON &= ~BIT(5);  //pll_refdiven
    HUSB_COM_CON |= (18 << 11); //NR-2

    HUSB_COM_CON |= BIT(1);   //pll_en
    delay(60);                //large than 10us
    HUSB_COM_CON |= BIT(2);   //pll_rst
    delay(60);                //large than 10us
    HUSB_COM_CON &= ~BIT(2);  //pll_rst release
    delay(5000);              //large than 1ms

    HUSB_PHY_CON0 |= BIT(0);  //cpu_clk_en
    delay(60);
    HUSB_PHY_CON0 |= BIT(1);  //cpu_utm_en
}

/*
*************************************************************
*Description: PLL初始化函数
*Arguments  : pll_osc：时钟源及哪一路PLL，
              osc_frq：外部输入的时钟频率
              pll_target：pll配置的频率目标值
*Returns    : NULL
*Note       :
*By         :
*************************************************************
*/
void pll_init(u32 pll_info, u32 osc_frq, u32 pll_target)
{
    u8 pll;
    pll = pll_info & (3 << 5); //pll_osc >> 5;

    switch(pll)
    {
        case PLL0:
            pll_clk_p_frq.pll0_target_clk = pll_target;
            pll0_init(pll_info, osc_frq, pll_target);
            break;

        case PLL1:
            pll_clk_p_frq.pll1_target_clk = pll_target;
            pll1_init(pll_info, osc_frq, pll_target);
            break;

        case PLL2:
            pll_clk_p_frq.pll2_target_clk = 480000000L;
            pll2_init(pll_info, osc_frq);
            break;

        default:
            break;
    }

    delay(10000);
}

/*
*************************************************************
*Description: 检查pll是否被使用函数
*Arguments  : pll：检查的pll
*Returns    : 0：被使用  1：未使用
*Note       :
*By         :
*************************************************************
*/
u32 check_pll_is_en(u32 pll)
{
    if(pll == PLL0)
    {
        return PLL_CON0 & BIT(0) ? 1 : 0;
    }
    else if(pll == PLL1)
    {
        return PLL_CON0 & BIT(1) ? 1 : 0;
    }
    else if(pll == PLL2)
    {
        return HUSB_COM_CON & BIT(1) ? 1 : 0;
    }

    return 0;
}

/*
*************************************************************
*Description: 系统时钟初始化函数
*Arguments  : pll_osc：时钟源，osc_frq：外部输入时钟的频率
              clk：系统时钟目标值
*Returns    : NULL
*Note       :
*By         :
*************************************************************
*/
u32 sys_clk_init(u32 pll_info, u32 osc_frq, u32 clk)
{
    u16 i;//, j;
    u32 pll_target = 0;
    u32 div1 = 0;
    u32 div2 = 0;
    u8 pll, osc;
    u32 reg;


    for(i = 0; i < (sizeof(pll_div_arg) / sizeof(pll_div_arg[0])); i++)
    {
        if(clk == pll_div_arg[i][0])
        {
            pll_target = pll_div_arg[i][1] * 1000000;
            div1 = pll_div_arg[i][2];
            div2 = pll_div_arg[i][3];
            break;
        }
    }

    printf("clk = %d div=%d--%d\n",pll_target,div1,div2);
    if(pll_target == 0)
    {
        return 1;
    }

    pll = pll_info & (0x3 << 5);
    osc = pll_info & 0x7;

    printf("pll =%x,osc=%x\n",pll,osc);

    if((CLK_CON0 & (7 << 6)) == 6 << 6)//检测是否运行在htc模式
    {
    }
    else//非htc模式
    {
        reg = CLK_CON0;

        if(HTC_CON & BIT(8))
        {
        }
        else
        {
            htc_init();
        }

        reg &= ~(7 << 6);
        reg |= 2 << 6;

        CLK_CON0 &= ~BIT(8); //switch to rc_clk
        delay(2);
        CLK_CON0 = reg;
        delay(2);
        CLK_CON0 |= BIT(8);//switc htc clk
        delay(10);
    }



    switch(osc)
    {
        case SRC_RTC_OSC:
            rtc_osc_init(pll_info, osc_frq);
            break;

        case SRC_HTC:
            htc_init();
            break;

        case SRC_OSC0:
        case SRC_RTC:
        case SRC_PA4:
        case SRC_OSC_CLK:
            osc_clk_init(pll_info);
            break;
    }



    if((pll == PLL2) && (osc_frq != 12000000)) //使用PLL0提供12M的参考时钟给PLL2
    {
        pll0_init(pll_info, osc_frq, pll_target);
        pll_info = SRC_PLL01_12M;

        htc_delay1ms(10);
    }


    CLK_CON2 &= ~3;

    switch(pll)
    {
        case PLL0:
            pll0_init(pll_info, osc_frq, pll_target);
            CLK_CON2 |= BIT(0);
            break;

        case PLL1:
            pll1_init(pll_info, osc_frq, pll_target);
            CLK_CON2 |= BIT(1);
            break;

        case PLL2:
            pll2_init(pll_info, osc_frq);
            CLK_CON2 |= BIT(0) | BIT(1);
            break;

        default:
            return 2;
    }


    PLL_CON0 &= ~(0xf << 8);
    PLL_CON0 |= div1 << 8;
    PLL_CON0 |= div2 << 10;

    if(clk > 160)
    {
        apb_clk_div(3);
    }
    else if(clk > 80)
    {
        apb_clk_div(1);
    }

    htc_delay1ms(10);//pll stability


    CLK_CON0 &= ~BIT(8); //switch to rc_clk

    delay(5);

    CLK_CON0 |= 3 << 6;

    delay(5);

    CLK_CON0 |= BIT(8); //switch to pll_clk

    delay(5000000);//wait pll stability

    s_clk_info.s_pll_clk = clk * 1000000;
    s_clk_info.s_sys_clk = s_clk_info.s_pll_clk;
    s_clk_info.s_apb_clk = get_apb_clk();

    pll_96m_sel(pll);
    pll_apc_sel(pll);
    return 0;
}
u32 pll_apc_sel(u32 pll_src)
{
    PLL_CON0 &=~(3<<18);
    if(pll_src == PLL0)
    {
        PLL_CON0 |= BIT(18);
    }
    else if(pll_src == PLL1)
    {
        PLL_CON0 |= BIT(19);
    }
    else if(pll_src == PLL2)
    {
        PLL_CON0 |= BIT(19)|BIT(18);
    }

    s_clk_info.apc_clk_src = pll_src;
    s_clk_info.apc_clk = get_pll_clk(pll_src);
    return s_clk_info.apc_clk;
}

u32 pll_apc_div(u8 div)
{
    PLL_CON0 &=~(0xf<<12);
    PLL_CON0 |= div<<12;
    s_clk_info.apc_clk = get_pll_clk(s_clk_info.apc_clk_src)/clk_div_tab[div];
    return s_clk_info.apc_clk;
}

/*
*************************************************************
*Description: PLL96M PLL选择选择
*Arguments  : pll_src：PLL源，只支持PLL0, PLL2
*Returns    : NULL
*Note       :
*By         :
*************************************************************
*/
void pll_96m_sel(u32 pll_src)
{
    if(pll_src == PLL0)
    {
        PLL_CON0 &= ~BIT(21);
    }
    else if(pll_src == PLL2)
    {
        PLL_CON0 |= BIT(21);
    }
    else
    {
        return;
    }
}


/*
*************************************************************
*Description: 系统启动获取时钟信息函数
*Arguments  : info：
*Returns    : pll_clk
*Note       :
*By         :
*************************************************************
*/
void init_clk_info(u32 info)
{
    u32 pll_clk_tmp;
    u32 osc_frq_tmp;
    u32 pll_tmp;


    memset(&s_clk_info, 0, sizeof(S_CLK_INFO));

    ///pll_clk [8~16] 8bit
    pll_clk_tmp = (info >> 8) & 0xFF;
    s_clk_info.s_pll_clk = pll_clk_tmp * 1000000L;

    ///osc_frq [16~25] 9bit 0:32.768/  45: 4.5M
    osc_frq_tmp = (info >> (8 + 8)) & 0x1FF;

    if(osc_frq_tmp == 0)
    {
        s_clk_info.s_osc_frq = 32768L;
    }
    else
    {
        s_clk_info.s_osc_frq = osc_frq_tmp * 100000L;
    }

    ///pll [5~6]PLL0 PLL1 PLL2
    pll_tmp = (info >> 5) & 0x3;

    if(pll_tmp == 0)
    {
        s_clk_info.s_pll = PLL0;
    }
    else if(pll_tmp == 1)
    {
        s_clk_info.s_pll = PLL1;
    }
    else if(pll_tmp == 2)
    {
        s_clk_info.s_pll = PLL2;
    }

    ///pll_src [0~3] SRC_OSC0~2, RTC_OSC, HTC, SRC_PLL0_12M
    s_clk_info.s_pll_src = info & 0x7;

    ///osc hc enable [4]
    s_clk_info.s_osc_hc_en = (info & OSC_HC_ENABLE) ? 1 : 0;

    ///osc 1pin enable [3]
    s_clk_info.s_osc_1pin_en = (info & OSC_1PIN_ENABLE) ? 1 : 0;

    ///sys_clk
    s_clk_info.s_sys_clk = get_sys_clk();

    ///apb_clk
    s_clk_info.s_apb_clk = get_apb_clk();
}

/*
*************************************************************
*Description: 获取当前系统的时钟信息
*Arguments  : NULL
*Returns    : s_clk_info
*Note       :
*By         :
*************************************************************
*/
S_CLK_INFO get_sys_clk_info(void)
{
    return s_clk_info;
}



/*
*************************************************************
*Description: 获取分频前的pll_clk_p函数
*Arguments  : pll: 哪一个pll
*Returns    : pll_clk
*Note       :
*By         :
*************************************************************
*/
u32 get_pll_clk(u32 pll)
{
    u32 frq_tmp = 0;
    u32 reg = 0;
    u32 div1 = 0;
    u32 div2 = 0;

    switch(pll)
    {
        case PLL0:
            if(s_clk_info.s_pll == PLL0)
            {
                frq_tmp = s_clk_info.s_pll_clk;
            }
            else
            {
                reg = PLL_CON1;
                div1 = (reg >> 0) & 0x3;
                div2 = (reg >> 2) & 0x3;
                div1 = pll_div_tab[0][div1];
                div2 = pll_div_tab[1][div2];
                printf("div1: %d div2: %d \n", div1, div2);
                frq_tmp = pll_clk_p_frq.pll0_target_clk / (div1 * div2);
            }

            break;

        case PLL1:
            if((s_clk_info.s_pll == PLL1))
            {
                frq_tmp = s_clk_info.s_pll_clk;
            }
            else
            {
                reg = PLL_CON1;
                div1 = (reg >> 4) & 0x3;
                div2 = (reg >> 6) & 0x3;
                div1 = pll_div_tab[0][div1];
                div2 = pll_div_tab[1][div2];
                printf("div1: %d div2: %d \n", div1, div2);
                frq_tmp = pll_clk_p_frq.pll1_target_clk / (div1 * div2);
            }

            break;

        case PLL2:
            if(s_clk_info.s_pll == PLL2)
            {
                frq_tmp = s_clk_info.s_pll_clk;
            }
            else
            {
                reg = PLL_CON1;
                div1 = (reg >> 8) & 0x3;
                div2 = (reg >> 10) & 0x3;
                div1 = pll_div_tab[0][div1];
                div2 = pll_div_tab[1][div2];
                printf("div1: %d div2: %d \n", div1, div2);
                frq_tmp = pll_clk_p_frq.pll2_target_clk / (div1 * div2);
            }

            break;

        default:
            break;
    }

    return frq_tmp;
}
/*
*************************************************************
*Description: 系统时钟获取函数
*Arguments  : NULL
*Returns    : sys_clk
*Note       :
*By         :
*************************************************************
*/
u32 get_sys_clk()
{
    s_clk_info.s_sys_clk = s_clk_info.s_pll_clk;//(SYS_DIV + 1);
    return s_clk_info.s_sys_clk;
}


/*
*************************************************************
*Description: apb_clk获取函数
*Arguments  : NULL
*Returns    : apb_clk
*Note       :
*By         :
*************************************************************
*/

u32 get_apb_clk()
{
    u32 apb_div = 0;
    u32 reg = CLK_CON2;
    apb_div = 0x7 & (reg >> 8);
    s_clk_info.s_apb_clk = get_sys_clk() / (apb_div + 1);
    return s_clk_info.s_apb_clk;
}

/*
*************************************************************
*Description: apb_clk分频函数
*Arguments  : 分频系数
*Returns    :
*Note       :
*By         :
*************************************************************
*/
u32 apb_clk_div(u8 div)
{
    u32 reg;

    if(div > 0x7)
    {
        return -1;
    }

    reg = CLK_CON2;
    reg &= ~(0x7 << 8);
    reg |= (div << 8);
    CLK_CON2 = reg;

    s_clk_info.s_apb_clk = get_sys_clk() / (1 + div);

    return s_clk_info.s_apb_clk;
}


/*
*************************************************************
*Description: 系统时钟分频函数
*Arguments  : div：分频数
*Returns    : NULL
*Note       :
*By         :
*************************************************************
*/
void sys_clk_div(u8 div)
{
    //s_clk_info.s_sys_clk = s_clk_info.s_pll_clk/(div+1);
}


/*
*************************************************************
*Description: PLL分频函数
*Arguments  : pll_src：pll源，div pll分频数
*Returns    : pll out clk
*Note       :
*By         :
*************************************************************
*/
u32 pll_clk_div(u32 pll_src, u8 div)
{
    u32 pll_out_clk;
    u32 reg;
    if(div>PLL_CLOCK_DIV56)
    {
        return -1;
    }
    reg = PLL_CON1;
    switch(pll_src)
    {
        case PLL0:
            reg &= ~0xf;
            reg |= div;

            pll_clk_p_frq.pll0_out_clk = pll_clk_p_frq.pll0_target_clk / clk_div_tab[div];
            pll_out_clk = pll_clk_p_frq.pll0_out_clk;
            break;

        case PLL1:
            reg &= ~(0xf << 4);
            reg |= div << 4;

            pll_clk_p_frq.pll1_out_clk = pll_clk_p_frq.pll1_target_clk / clk_div_tab[div];
            pll_out_clk = pll_clk_p_frq.pll1_out_clk;
            break;

        case PLL2:
            reg &= ~(0xf << 8);
            reg |= (div << 8);

            pll_clk_p_frq.pll2_out_clk = pll_clk_p_frq.pll2_target_clk / clk_div_tab[div];
            pll_out_clk = pll_clk_p_frq.pll2_out_clk;
            break;

        default:
            break;
    }

    PLL_CON1 = reg;

    return pll_out_clk;
}


/*
*************************************************************
*Description: PA13时钟输出函数
*Arguments  : src：要输出的时钟
*Returns    : NULL
*Note       :
*By         :
*************************************************************
*/
void PH13_clk_out(u8 src)
{
    if(src == PH13_CLOCK_OUT_DISABLE)
    {
        CLK_CON0 &= ~BIT(13);
        PORTH_DIR |= BIT(13);
    }

    CLK_CON0 &= ~(0xF << 10);
    CLK_CON0 |= (BIT(3) | (src)) << 10;
    PORTH_DIR &= ~BIT(13);
    PORTH_HD |= BIT(13);
}
/*
*************************************************************
*Description: PA14时钟输出函数
*Arguments  : src：要输出的时钟
*Returns    : NULL
*Note       : 输出HTC_CLK，不输出SYS_CLK
*By         :
*************************************************************
*/
void PH14_clk_out(u8 src)
{
    if(src == PH14_CLOCK_OUT_DISABLE)
    {
        CLK_CON0 &= ~BIT(17);
        PORTH_DIR |= BIT(14);
    }

    CLK_CON0 &= ~(0xF << 14);
    CLK_CON0 |= (BIT(3) | (src)) << 14;
    PORTH_DIR &= ~BIT(14);
    PORTH_HD |= BIT(14);
}

/*
*************************************************************
*Description: PA15时钟输出函数
*Arguments  : src：要输出的时钟
*Returns    : NULL
*Note       :
*By         :
*************************************************************
*/
void PH15_clk_out(u8 src)
{
    if(src == PH15_CLOCK_OUT_DISABLE)
    {
        CLK_CON0 &= ~BIT(21);
        PORTH_DIR |= BIT(15);
    }

    CLK_CON0 &= ~(0xF << 18);
    CLK_CON0 |= (BIT(3) | (src)) << 18;
    PORTH_DIR &= ~BIT(15);
    PORTH_HD |= BIT(15);
}



