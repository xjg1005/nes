
/*--------------------------------------------------------------------------*/
/**@file     sh60.h
   @brief    芯片资源头文件
   @details
   @author
   @date    2011-3-7
   @note    CD003
*/
/*----------------------------------------------------------------------------*/

#ifndef _DV12_
#define _DV12__

//Note:
//there are 256 words(1024 bytes) in the sfr space
//byte(8bit)       SFR offset-address is:   0x03, 0x07, 0x0b, 0x0f, 0x13 ......
//half-word(16bit) SFR offset-address is:   0x02, 0x06, 0x0a, 0x0e, 0x12 ......
//word(24/32bit)   SFR offset-address is:   0x00, 0x04, 0x08, 0x0c, 0x10 ......

#define hs_base     0x1f70000
#define ls_base     0x1f60000

//===============================================================================//
//
//      high speed sfr address define
//
//===============================================================================//
#define ls_io_base    (ls_base + 0x000*4)
#define ls_uart_base  (ls_base + 0x100*4)
#define ls_spi_base   (ls_base + 0x200*4)
#define ls_sd_base    (ls_base + 0x300*4)
#define ls_tmr_base   (ls_base + 0x400*4)
#define ls_fusb_base  (ls_base + 0x500*4)
#define ls_husb_base  (ls_base + 0x600*4)
#define ls_adda_base  (ls_base + 0x700*4)
#define ls_clk_base   (ls_base + 0x800*4)
#define ls_oth_base   (ls_base + 0x900*4)

#define hs_sdr_base   (hs_base + 0x000*4)
#define hs_isp_base   (hs_base + 0x100*4)
#define hs_cpu_base   (hs_base + 0x200*4)
#define hs_sfc_base   (hs_base + 0x300*4)
#define hs_jpg_base   (hs_base + 0x400*4)
#define hs_oth_base   (hs_base + 0x500*4)
#define hs_dbg_base   (hs_base + 0x600*4)
#define hs_cve_base   (hs_base + 0x700*4)

#define DSP_BF_CON              (*(volatile u32 *)(hs_dbg_base + 0x00*4))       // for debug only
#define DEBUG_WR_EN             (*(volatile u32 *)(hs_dbg_base + 0x01*4))       // for debug only
#define DSP_PC_LIMH             (*(volatile u32 *)(hs_dbg_base + 0x02*4))       // for debug only
#define DSP_PC_LIML             (*(volatile u32 *)(hs_dbg_base + 0x03*4))       // for debug only
#define WR_LIM0H                (*(volatile u32 *)(hs_dbg_base + 0x04*4))       // for debug only
#define WR_LIM0L                (*(volatile u32 *)(hs_dbg_base + 0x05*4))       // for debug only
#define WR_LIM1H                (*(volatile u32 *)(hs_dbg_base + 0x06*4))       // for debug only
#define WR_LIM1L                (*(volatile u32 *)(hs_dbg_base + 0x07*4))       // for debug only
#define WR_LIM2H                (*(volatile u32 *)(hs_dbg_base + 0x08*4))       // for debug only
#define WR_LIM2L                (*(volatile u32 *)(hs_dbg_base + 0x09*4))       // for debug only
#define WR_LIM3H                (*(volatile u32 *)(hs_dbg_base + 0x0a*4))       // for debug only
#define WR_LIM3L                (*(volatile u32 *)(hs_dbg_base + 0x0b*4))       // for debug only
#define WR_LIM4H                (*(volatile u32 *)(hs_dbg_base + 0x0c*4))       // for debug only
#define WR_LIM4L                (*(volatile u32 *)(hs_dbg_base + 0x0d*4))       // for debug only
#define WR_LIM5H                (*(volatile u32 *)(hs_dbg_base + 0x0e*4))       // for debug only
#define WR_LIM5L                (*(volatile u32 *)(hs_dbg_base + 0x0f*4))       // for debug only
#define WR_LIM6H                (*(volatile u32 *)(hs_dbg_base + 0x10*4))       // for debug only
#define WR_LIM6L                (*(volatile u32 *)(hs_dbg_base + 0x11*4))       // for debug only
#define WR_LIM7H                (*(volatile u32 *)(hs_dbg_base + 0x12*4))       // for debug only
#define WR_LIM7L                (*(volatile u32 *)(hs_dbg_base + 0x13*4))       // for debug only

#define PRP_ALLOW_NUM0          (*(volatile u32 *)(hs_dbg_base + 0x18*4))       // for debug only
#define PRP_ALLOW_NUM1          (*(volatile u32 *)(hs_dbg_base + 0x19*4))       // for debug only

#define DEBUG_MSG               (*(volatile u32 *)(hs_dbg_base + 0x20*4))       // for debug only
#define DEBUG_MSG_CLR           (*(volatile u32 *)(hs_dbg_base + 0x21*4))       // for debug only
#define DEBUG_WRNUM             (*(volatile u32 *)(hs_dbg_base + 0x22*4))       // for debug only

#define FPGA_TRI                (*(volatile u16 *)(hs_dbg_base + 0xff*4))

#define PORTA_OUT               (*(volatile u16 *)(ls_io_base + 0x00*4))         //
#define PORTA_IN                (*(volatile u16 *)(ls_io_base + 0x01*4))         //Read Only
#define PORTA_DIR               (*(volatile u16 *)(ls_io_base + 0x02*4))         //
#define PORTA_DIE               (*(volatile u16 *)(ls_io_base + 0x03*4))         //
#define PORTA_PU                (*(volatile u16 *)(ls_io_base + 0x04*4))         //
#define PORTA_PD                (*(volatile u16 *)(ls_io_base + 0x05*4))         //
#define PORTA_HD                (*(volatile u16 *)(ls_io_base + 0x06*4))         //

#define PORTB_OUT               (*(volatile u16 *)(ls_io_base + 0x10*4))         //
#define PORTB_IN                (*(volatile u16 *)(ls_io_base + 0x11*4))         //Read Only
#define PORTB_DIR               (*(volatile u16 *)(ls_io_base + 0x12*4))         //
#define PORTB_DIE               (*(volatile u16 *)(ls_io_base + 0x13*4))         //
#define PORTB_PU                (*(volatile u16 *)(ls_io_base + 0x14*4))         //
#define PORTB_PD                (*(volatile u16 *)(ls_io_base + 0x15*4))         //
#define PORTB_HD                (*(volatile u16 *)(ls_io_base + 0x16*4))         //

#define PORTC_OUT               (*(volatile u16 *)(ls_io_base + 0x20*4))         //
#define PORTC_IN                (*(volatile u16 *)(ls_io_base + 0x21*4))         //Read Only
#define PORTC_DIR               (*(volatile u16 *)(ls_io_base + 0x22*4))         //
#define PORTC_DIE               (*(volatile u16 *)(ls_io_base + 0x23*4))         //
#define PORTC_PU                (*(volatile u16 *)(ls_io_base + 0x24*4))         //
#define PORTC_PD                (*(volatile u16 *)(ls_io_base + 0x25*4))         //
#define PORTC_HD                (*(volatile u16 *)(ls_io_base + 0x26*4))         //

#define PORTD_OUT               (*(volatile u16 *)(ls_io_base + 0x30*4))         //
#define PORTD_IN                (*(volatile u16 *)(ls_io_base + 0x31*4))         //Read Only
#define PORTD_DIR               (*(volatile u16 *)(ls_io_base + 0x32*4))         //
#define PORTD_DIE               (*(volatile u16 *)(ls_io_base + 0x33*4))         //
#define PORTD_PU                (*(volatile u16 *)(ls_io_base + 0x34*4))         //
#define PORTD_PD                (*(volatile u16 *)(ls_io_base + 0x35*4))         //
#define PORTD_HD                (*(volatile u16 *)(ls_io_base + 0x36*4))         //

#define PORTE_OUT               (*(volatile u16 *)(ls_io_base + 0x40*4))         //
#define PORTE_IN                (*(volatile u16 *)(ls_io_base + 0x41*4))         //Read Only
#define PORTE_DIR               (*(volatile u16 *)(ls_io_base + 0x42*4))         //
#define PORTE_DIE               (*(volatile u16 *)(ls_io_base + 0x43*4))         //
#define PORTE_PU                (*(volatile u16 *)(ls_io_base + 0x44*4))         //
#define PORTE_PD                (*(volatile u16 *)(ls_io_base + 0x45*4))         //
#define PORTE_HD                (*(volatile u16 *)(ls_io_base + 0x46*4))         //

#define PORTF_OUT               (*(volatile u16 *)(ls_io_base + 0x50*4))         //
#define PORTF_IN                (*(volatile u16 *)(ls_io_base + 0x51*4))         //Read Only
#define PORTF_DIR               (*(volatile u16 *)(ls_io_base + 0x52*4))         //
#define PORTF_DIE               (*(volatile u16 *)(ls_io_base + 0x53*4))         //
#define PORTF_PU                (*(volatile u16 *)(ls_io_base + 0x54*4))         //
#define PORTF_PD                (*(volatile u16 *)(ls_io_base + 0x55*4))         //
#define PORTF_HD                (*(volatile u16 *)(ls_io_base + 0x56*4))         //

#define PORTG_OUT               (*(volatile u16 *)(ls_io_base + 0x60*4))         //
#define PORTG_IN                (*(volatile u16 *)(ls_io_base + 0x61*4))         //Read Only
#define PORTG_DIR               (*(volatile u16 *)(ls_io_base + 0x62*4))         //
#define PORTG_DIE               (*(volatile u16 *)(ls_io_base + 0x63*4))         //
#define PORTG_PU                (*(volatile u16 *)(ls_io_base + 0x64*4))         //
#define PORTG_PD                (*(volatile u16 *)(ls_io_base + 0x65*4))         //
#define PORTG_HD                (*(volatile u16 *)(ls_io_base + 0x66*4))         //

#define PORTH_OUT               (*(volatile u16 *)(ls_io_base + 0x70*4))         //
#define PORTH_IN                (*(volatile u16 *)(ls_io_base + 0x71*4))         //Read Only
#define PORTH_DIR               (*(volatile u16 *)(ls_io_base + 0x72*4))         //
#define PORTH_DIE               (*(volatile u16 *)(ls_io_base + 0x73*4))         //
#define PORTH_PU                (*(volatile u16 *)(ls_io_base + 0x74*4))         //
#define PORTH_PD                (*(volatile u16 *)(ls_io_base + 0x75*4))         //
#define PORTH_HD                (*(volatile u16 *)(ls_io_base + 0x76*4))         //

#define PORTI_IN                (*(volatile u16 *)(ls_io_base + 0x81*4))         //Read Only
#define PORTI_DIR               (*(volatile u16 *)(ls_io_base + 0x82*4))         //
#define PORTI_DIE               (*(volatile u16 *)(ls_io_base + 0x83*4))         //
#define PORTI_PU                (*(volatile u16 *)(ls_io_base + 0x84*4))         //
#define PORTI_PD                (*(volatile u16 *)(ls_io_base + 0x85*4))         //
#define PORTI_HD                (*(volatile u16 *)(ls_io_base + 0x86*4))         //

#define IOMC0                   (*(volatile u32 *)(ls_io_base + 0x90*4))         //
#define IOMC1                   (*(volatile u16 *)(ls_io_base + 0x91*4))         //
#define IOMC2                   (*(volatile u32 *)(ls_io_base + 0x92*4))         //
#define IOMC3                   (*(volatile u16 *)(ls_io_base + 0x93*4))         //
#define WKUP_CON0               (*(volatile u16 *)(ls_io_base + 0x94*4))         //
#define WKUP_CON1               (*(volatile u16 *)(ls_io_base + 0x95*4))         //
#define WKUP_CON2               (*(volatile u16 *)(ls_io_base + 0x96*4))         //write only;
#define WKUP_CON3               (*(volatile u16 *)(ls_io_base + 0x97*4))         //


#define UT0_HRXCNT              (*(volatile u32 *)(ls_uart_base + 0x00*4))
#define UT0_OTCNT               (*(volatile u32 *)(ls_uart_base + 0x01*4))
#define UT0_TXADR               (*(volatile u32 *)(ls_uart_base + 0x02*4))       //26bit write only;
#define UT0_TXCNT               (*(volatile u32 *)(ls_uart_base + 0x03*4))
#define UT0_RXEADR              (*(volatile u32 *)(ls_uart_base + 0x04*4))       //26bit write only;
#define UT0_CON                 (*(volatile u16 *)(ls_uart_base + 0x05*4))
#define UT0_BUF                 (*(volatile u8  *)(ls_uart_base + 0x06*4))
#define UT0_BAUD                (*(volatile u16 *)(ls_uart_base + 0x07*4))       //16bit write only;
#define UT0_RXSADR              (*(volatile u32 *)(ls_uart_base + 0x08*4))       //26bit write only;
#define UT0_RXCNT               (*(volatile u32 *)(ls_uart_base + 0x09*4))

#define UT1_HRXCNT              (*(volatile u32 *)(ls_uart_base + 0x10*4))
#define UT1_OTCNT               (*(volatile u32 *)(ls_uart_base + 0x11*4))
#define UT1_TXADR               (*(volatile u32 *)(ls_uart_base + 0x12*4))       //26bit write only;
#define UT1_TXCNT               (*(volatile u32 *)(ls_uart_base + 0x13*4))
#define UT1_RXEADR              (*(volatile u32 *)(ls_uart_base + 0x14*4))       //26bit write only;
#define UT1_CON                 (*(volatile u16 *)(ls_uart_base + 0x15*4))
#define UT1_BUF                 (*(volatile u8  *)(ls_uart_base + 0x16*4))
#define UT1_BAUD                (*(volatile u16 *)(ls_uart_base + 0x17*4))       //16bit write only;
#define UT1_RXSADR              (*(volatile u32 *)(ls_uart_base + 0x18*4))       //26bit write only;
#define UT1_RXCNT               (*(volatile u32 *)(ls_uart_base + 0x19*4))

#define UT2_CON                 (*(volatile u16 *)(ls_uart_base + 0x20*4))
#define UT2_BUF                 (*(volatile u8  *)(ls_uart_base + 0x21*4))
#define UT2_BAUD                (*(volatile u16 *)(ls_uart_base + 0x22*4))       //write only;

#define UT3_CON                 (*(volatile u16 *)(ls_uart_base + 0x30*4))
#define UT3_BUF                 (*(volatile u8  *)(ls_uart_base + 0x31*4))
#define UT3_BAUD                (*(volatile u16 *)(ls_uart_base + 0x32*4))       //write only;

//SPI
#define SPI0_CON                (*(volatile u16 *)(ls_spi_base + 0x00*4))
#define SPI0_BAUD               (*(volatile u8  *)(ls_spi_base + 0x01*4))
#define SPI0_BUF                (*(volatile u8  *)(ls_spi_base + 0x02*4))
#define SPI0_ADR                (*(volatile u32 *)(ls_spi_base + 0x03*4))       //26bit write only;
#define SPI0_CNT                (*(volatile u16 *)(ls_spi_base + 0x04*4))       //write only;

#define SPI1_CON                (*(volatile u16 *)(ls_spi_base + 0x10*4))
#define SPI1_BAUD               (*(volatile u8  *)(ls_spi_base + 0x11*4))
#define SPI1_BUF                (*(volatile u8  *)(ls_spi_base + 0x12*4))
#define SPI1_ADR                (*(volatile u32 *)(ls_spi_base + 0x13*4))       //26bit write only;
#define SPI1_CNT                (*(volatile u16 *)(ls_spi_base + 0x14*4))       //write only;

//SD
#define SD0_CON0                (*(volatile u16 *)(ls_sd_base + 0x00*4))
#define SD0_CON1                (*(volatile u16 *)(ls_sd_base + 0x01*4))
#define SD0_CON2                (*(volatile u16 *)(ls_sd_base + 0x02*4))
#define SD0_CPTR                (*(volatile u32 *)(ls_sd_base + 0x03*4))        //26bit write only;
#define SD0_DPTR                (*(volatile u32 *)(ls_sd_base + 0x04*4))        //26bit write only;
#define SD0_CTU_CON             (*(volatile u16 *)(ls_sd_base + 0x05*4))
#define SD0_CTU_CNT             (*(volatile u16 *)(ls_sd_base + 0x06*4))

#define SD1_CON0                (*(volatile u16 *)(ls_sd_base + 0x10*4))
#define SD1_CON1                (*(volatile u16 *)(ls_sd_base + 0x11*4))
#define SD1_CON2                (*(volatile u16 *)(ls_sd_base + 0x12*4))
#define SD1_CPTR                (*(volatile u32 *)(ls_sd_base + 0x13*4))        //nbit
#define SD1_DPTR                (*(volatile u32 *)(ls_sd_base + 0x14*4))        //nbit
#define SD1_CTU_CON             (*(volatile u16 *)(ls_sd_base + 0x15*4))
#define SD1_CTU_CNT             (*(volatile u16 *)(ls_sd_base + 0x16*4))

//TIMER

#define T0_CON                  (*(volatile u16 *)(ls_tmr_base + 0x00*4))
#define T0_CNT                  (*(volatile u32 *)(ls_tmr_base + 0x01*4))
#define T0_PRD                  (*(volatile u32 *)(ls_tmr_base + 0x02*4))
#define T0_PWM                  (*(volatile u32 *)(ls_tmr_base + 0x03*4))

#define T1_CON                  (*(volatile u16 *)(ls_tmr_base + 0x10*4))
#define T1_CNT                  (*(volatile u32 *)(ls_tmr_base + 0x11*4))
#define T1_PRD                  (*(volatile u32 *)(ls_tmr_base + 0x12*4))
#define T1_PWM                  (*(volatile u32 *)(ls_tmr_base + 0x13*4))

#define T2_CON                  (*(volatile u16 *)(ls_tmr_base + 0x20*4))
#define T2_CNT                  (*(volatile u32 *)(ls_tmr_base + 0x21*4))
#define T2_PRD                  (*(volatile u32 *)(ls_tmr_base + 0x22*4))
#define T2_PWM                  (*(volatile u32 *)(ls_tmr_base + 0x23*4))

#define T3_CON                  (*(volatile u16 *)(ls_tmr_base + 0x30*4))
#define T3_CNT                  (*(volatile u32 *)(ls_tmr_base + 0x31*4))
#define T3_PRD                  (*(volatile u32 *)(ls_tmr_base + 0x32*4))
#define T3_PWM                  (*(volatile u32 *)(ls_tmr_base + 0x33*4))

#define PWMTMRCON               (*(volatile u16 *)(ls_tmr_base + 0x40*4))       //write only;
#define PWMTMRCNT               (*(volatile u16 *)(ls_tmr_base + 0x41*4))       //write only;
#define PWMTMRPR                (*(volatile u16 *)(ls_tmr_base + 0x42*4))       //write only;
#define PWMCMP1                 (*(volatile u16 *)(ls_tmr_base + 0x43*4))       //write only;
#define PWMCMP2                 (*(volatile u16 *)(ls_tmr_base + 0x44*4))       //write only;
#define PWMCMP3                 (*(volatile u16 *)(ls_tmr_base + 0x45*4))       //write only;
#define PWMCON                  (*(volatile u16 *)(ls_tmr_base + 0x46*4))       //write only;
#define PWMDTCON                (*(volatile u16 *)(ls_tmr_base + 0x47*4))       //write only;
#define PWMFPINCON              (*(volatile u16 *)(ls_tmr_base + 0x48*4))       //write only;

//FUSB
#define FUSB_CON0               (*(volatile u32 *)(ls_fusb_base + 0x00*4))
#define FUSB_CON1               (*(volatile u32 *)(ls_fusb_base + 0x01*4))
#define FUSB_EP0_CNT            (*(volatile u16 *)(ls_fusb_base + 0x02*4))      //write only;
#define FUSB_EP1_CNT            (*(volatile u16 *)(ls_fusb_base + 0x03*4))      //write only;
#define FUSB_EP2_CNT            (*(volatile u16 *)(ls_fusb_base + 0x04*4))      //write only;
#define FUSB_EP3_CNT            (*(volatile u16 *)(ls_fusb_base + 0x05*4))      //write only;
#define FUSB_EP0_ADR            (*(volatile u32 *)(ls_fusb_base + 0x06*4))      //26bit write only;
#define FUSB_EP1_TADR           (*(volatile u32 *)(ls_fusb_base + 0x07*4))      //write only;
#define FUSB_EP1_RADR           (*(volatile u32 *)(ls_fusb_base + 0x08*4))      //write only;
#define FUSB_EP2_TADR           (*(volatile u32 *)(ls_fusb_base + 0x09*4))      //write only;
#define FUSB_EP2_RADR           (*(volatile u32 *)(ls_fusb_base + 0x0a*4))      //write only;
#define FUSB_EP3_TADR           (*(volatile u32 *)(ls_fusb_base + 0x0b*4))      //write only;
#define FUSB_EP3_RADR           (*(volatile u32 *)(ls_fusb_base + 0x0c*4))      //write only;
#define FUSB_IO_CON0            (*(volatile u16 *)(ls_fusb_base + 0x0d*4))      //

#define HUSB_SIE_CON            (*(volatile u32 *)(ls_husb_base + 0x00*4))
#define HUSB_EP0_CNT            (*(volatile u16 *)(ls_husb_base + 0x01*4))      //write only;
#define HUSB_EP1_CNT            (*(volatile u16 *)(ls_husb_base + 0x02*4))      //write only;
#define HUSB_EP2_CNT            (*(volatile u16 *)(ls_husb_base + 0x03*4))      //write only;
#define HUSB_EP3_CNT            (*(volatile u16 *)(ls_husb_base + 0x04*4))      //write only;
#define HUSB_EP4_CNT            (*(volatile u16 *)(ls_husb_base + 0x05*4))      //write only;
#define HUSB_EP0_ADR            (*(volatile u32 *)(ls_husb_base + 0x06*4))      //26bit write only;
#define HUSB_EP1_TADR           (*(volatile u32 *)(ls_husb_base + 0x07*4))      //26bit write only;
#define HUSB_EP1_RADR           (*(volatile u32 *)(ls_husb_base + 0x08*4))      //26bit write only;
#define HUSB_EP2_TADR           (*(volatile u32 *)(ls_husb_base + 0x09*4))      //26bit write only;
#define HUSB_EP2_RADR           (*(volatile u32 *)(ls_husb_base + 0x0a*4))      //26bit write only;
#define HUSB_EP3_TADR           (*(volatile u32 *)(ls_husb_base + 0x0b*4))      //26bit write only;
#define HUSB_EP3_RADR           (*(volatile u32 *)(ls_husb_base + 0x0c*4))      //26bit write only;
#define HUSB_EP4_TADR           (*(volatile u32 *)(ls_husb_base + 0x0d*4))      //26bit write only;
#define HUSB_EP4_RADR           (*(volatile u32 *)(ls_husb_base + 0x0e*4))      //26bit write only;

#define HUSB_COM_CON            (*(volatile u32 *)(ls_husb_base + 0x10*4))
#define HUSB_PHY_CON0           (*(volatile u32 *)(ls_husb_base + 0x11*4))
#define HUSB_PHY_CON1           (*(volatile u32 *)(ls_husb_base + 0x12*4))
#define HUSB_PHY_CON2           (*(volatile u32 *)(ls_husb_base + 0x13*4))

#define ALNK_CON0               (*(volatile u16 *)(ls_adda_base + 0x00*4))
#define ALNK_CON1               (*(volatile u16 *)(ls_adda_base + 0x01*4))
#define ALNK_ADR0               (*(volatile u32 *)(ls_adda_base + 0x02*4))      //26bit write only;
#define ALNK_ADR1               (*(volatile u32 *)(ls_adda_base + 0x03*4))      //26bit write only;
#define ALNK_ADR2               (*(volatile u32 *)(ls_adda_base + 0x04*4))      //26bit write only;
#define ALNK_ADR3               (*(volatile u32 *)(ls_adda_base + 0x05*4))      //26bit write only;
#define ALNK_CON2               (*(volatile u16 *)(ls_adda_base + 0x06*4))
#define ALNK_LEN                (*(volatile u16 *)(ls_adda_base + 0x07*4))      //16bit write only;

#define LADC_CON                (*(volatile u16 *)(ls_adda_base + 0x10*4))
#define LADC_ADR                (*(volatile u32 *)(ls_adda_base + 0x11*4))      //26bit write only;
#define LADC_CON1               (*(volatile u16 *)(ls_adda_base + 0x12*4))
#define LADC_CON2               (*(volatile u16 *)(ls_adda_base + 0x13*4))
#define LADC_RES12              (*(volatile u16 *)(ls_adda_base + 0x14*4))      //12bit read only;
#define LADC_LEN                (*(volatile u16 *)(ls_adda_base + 0x15*4))      //16bit write only;

#define ADC_CON                 (*(volatile u16 *)(ls_adda_base + 0x20*4))
#define ADC_RES                 (*(volatile u16 *)(ls_adda_base + 0x21*4))      //10bit read only;

#define DAC_CON                 (*(volatile u16 *)(ls_adda_base + 0x30*4))
#define DAC_TRML                (*(volatile u8  *)(ls_adda_base + 0x31*4))      //8bit write only;
#define DAC_TRMR                (*(volatile u8  *)(ls_adda_base + 0x32*4))      //8bit write only;
#define DAC_ADR                 (*(volatile u32 *)(ls_adda_base + 0x33*4))      //26bit write only;
#define DAA_CON0                (*(volatile u16 *)(ls_adda_base + 0x34*4))
#define DAA_CON1                (*(volatile u16 *)(ls_adda_base + 0x35*4))
#define DAA_CON2                (*(volatile u16 *)(ls_adda_base + 0x36*4))
#define DAA_CON3                (*(volatile u16 *)(ls_adda_base + 0x37*4))      //15bit;
#define DAC_LEN                 (*(volatile u16 *)(ls_adda_base + 0x38*4))      //16bit write only;

#define PWR_CON                 (*(volatile u8  *)(ls_clk_base + 0x00*4))
#define CLK_CON0                (*(volatile u32 *)(ls_clk_base + 0x01*4))
#define CLK_CON1                (*(volatile u16 *)(ls_clk_base + 0x02*4))
#define CLK_CON2                (*(volatile u16 *)(ls_clk_base + 0x03*4))
#define LCLK_GAT                (*(volatile u32 *)(ls_clk_base + 0x04*4))
#define HCLK_GAT                (*(volatile u32 *)(ls_clk_base + 0x05*4))
#define ACLK_GAT                (*(volatile u32 *)(ls_clk_base + 0x06*4))
#define PLL0_NF                 (*(volatile u16 *)(ls_clk_base + 0x07*4))
#define PLL0_NR                 (*(volatile u16 *)(ls_clk_base + 0x08*4))
#define PLL1_NF                 (*(volatile u16 *)(ls_clk_base + 0x09*4))
#define PLL1_NR                 (*(volatile u16 *)(ls_clk_base + 0x0a*4))
#define OSA_CON                 (*(volatile u16 *)(ls_clk_base + 0x0b*4))
#define PLL_CON0                (*(volatile u32 *)(ls_clk_base + 0x0c*4))
#define PLL_CON1                (*(volatile u16 *)(ls_clk_base + 0x0d*4))

#define HTC_CON                 (*(volatile u16 *)(ls_oth_base + 0x00*4))
#define LDO_CON                 (*(volatile u16 *)(ls_oth_base + 0x01*4))
#define LVD_CON                 (*(volatile u16 *)(ls_oth_base + 0x02*4))
#define IRTC_CON                (*(volatile u16 *)(ls_oth_base + 0x03*4))
#define IRTC_BUF                (*(volatile u8  *)(ls_oth_base + 0x04*4))
#define MODE_CON                (*(volatile u8  *)(ls_oth_base + 0x05*4))
#define CRC_FIFO                (*(volatile u8  *)(ls_oth_base + 0x06*4))       //write only;
#define CRC_REG                 (*(volatile u16 *)(ls_oth_base + 0x07*4))
#define WDT_CON                 (*(volatile u8  *)(ls_oth_base + 0x08*4))
#define CHIP_ID                 (*(volatile u8  *)(ls_oth_base + 0x09*4))       //read only;
#define IRFLT_CON               (*(volatile u8  *)(ls_oth_base + 0x0a*4))
#define IIC_CON                 (*(volatile u16 *)(ls_oth_base + 0x0b*4))
#define IIC_BUF                 (*(volatile u8  *)(ls_oth_base + 0x0c*4))
#define IIC_BAUD                (*(volatile u8  *)(ls_oth_base + 0x0d*4))
#define IIC_DMA_ADR             (*(volatile u32 *)(ls_oth_base + 0x0e*4))       //write only
#define IIC_DMA_CNT             (*(volatile u16 *)(ls_oth_base + 0x0f*4))       //write only
#define IIC_DMA_NRATE           (*(volatile u8  *)(ls_oth_base + 0x10*4))       //write only
#define PWM8_CON                (*(volatile u16 *)(ls_oth_base + 0x11*4))
#define PAP_CON                 (*(volatile u32 *)(ls_oth_base + 0x12*4))
#define PAP_BUF                 (*(volatile u16 *)(ls_oth_base + 0x13*4))
#define PAP_ADR                 (*(volatile u32 *)(ls_oth_base + 0x14*4))       //26bit write only;
#define PAP_CNT                 (*(volatile u16 *)(ls_oth_base + 0x15*4))       //write only;
#define PAP_DAT0                (*(volatile u16 *)(ls_oth_base + 0x16*4))       //write only;
#define PAP_DAT1                (*(volatile u16 *)(ls_oth_base + 0x17*4))       //write only;
#define LCDC_CON0               (*(volatile u16 *)(ls_oth_base + 0x18*4))
#define LCDC_CON1               (*(volatile u16 *)(ls_oth_base + 0x19*4))
#define SEG_IOEN                (*(volatile u32 *)(ls_oth_base + 0x1a*4))       //22bit;
#define EFUSE_CON               (*(volatile u16 *)(ls_oth_base + 0x1b*4))
#define MPUCON                  (*(volatile u32 *)(ls_oth_base + 0x1c*4))
#define MPUSTART                (*(volatile u32 *)(ls_oth_base + 0x1d*4))
#define MPUEND                  (*(volatile u32 *)(ls_oth_base + 0x1e*4))
#define MPUCATCH0               (*(volatile u32 *)(ls_oth_base + 0x1f*4))
#define MPUCATCH1               (*(volatile u32 *)(ls_oth_base + 0x20*4))
#define PLCNTCON                (*(volatile u8  *)(ls_oth_base + 0x21*4))
#define PLCNTVL                 (*(volatile u16 *)(ls_oth_base + 0x22*4))       //read only;
#define CS_CON                  (*(volatile u16 *)(ls_oth_base + 0x23*4))
#define CS_REG                  (*(volatile u32 *)(ls_oth_base + 0x24*4))
#define CS_FIFO                 (*(volatile u32 *)(ls_oth_base + 0x25*4))       //write only
#define CS_RADR                 (*(volatile u32 *)(ls_oth_base + 0x26*4))       //write only
#define CS_RCNT                 (*(volatile u32 *)(ls_oth_base + 0x27*4))       //write only
#define RAND64L                 (*(volatile u32 *)(ls_oth_base + 0x28*4))       //read only
#define RAND64H                 (*(volatile u32 *)(ls_oth_base + 0x29*4))       //read only


//===============================================================================//
//
//      high speed sfr address define
//
//===============================================================================//
#define SDRCON0                 (*(volatile u32 *)(hs_sdr_base + 0x00*4))
//#define SDRSPTR                 (*(volatile u32 *)(hs_sdr_base + 0x01*4))
//#define SDRQPTR                 (*(volatile u32 *)(hs_sdr_base + 0x02*4))
#define SDRREFREG               (*(volatile u32 *)(hs_sdr_base + 0x03*4))       //write only
//#define SDRDMACNT               (*(volatile u32 *)(hs_sdr_base + 0x04*4))
#define SDRCON1                 (*(volatile u32 *)(hs_sdr_base + 0x05*4))       //write only
#define SDRREFSUM               (*(volatile u32 *)(hs_sdr_base + 0x06*4))       //13bit
#define SDRDBG                  (*(volatile u32 *)(hs_sdr_base + 0x07*4))       //13bit
#define SDRCON2                 (*(volatile u32 *)(hs_sdr_base + 0x08*4))       //write only

#define DSP_ILAT                (*(volatile u32 *)(hs_cpu_base + 0x00*4))
#define DSP_CLR_ILAT            (*(volatile u32 *)(hs_cpu_base + 0x01*4))
#define DSP_SET_ILAT            (*(volatile u32 *)(hs_cpu_base + 0x02*4))
#define IPCON0                  (*(volatile u32 *)(hs_cpu_base + 0x03*4))       //30bit
#define IPCON1                  (*(volatile u32 *)(hs_cpu_base + 0x04*4))       //30bit
#define IPCON2                  (*(volatile u32 *)(hs_cpu_base + 0x05*4))       //30bit
#define DSPCON                  (*(volatile u32 *)(hs_cpu_base + 0x06*4))       //32bit
#define CPASS_CON               (*(volatile u32 *)(hs_cpu_base + 0x08*4))       //32bit
#define CPASS_ADRH              (*(volatile u32 *)(hs_cpu_base + 0x09*4))       //32bit
#define CPASS_ADRL              (*(volatile u32 *)(hs_cpu_base + 0x0a*4))       //32bit
#define CPASS_BUF_LAST          (*(volatile u32 *)(hs_cpu_base + 0x0b*4))       //32bit

//graph
#define ISP0_CON0               (*(volatile u32 *)(hs_isp_base + 0x00*4))
#define ISP0_CON1               (*(volatile u32 *)(hs_isp_base + 0x01*4))
#define ISP0_CON2               (*(volatile u32 *)(hs_isp_base + 0x02*4))

#define ISP1_CON0               (*(volatile u32 *)(hs_isp_base + 0x10*4))
#define ISP1_CON1               (*(volatile u32 *)(hs_isp_base + 0x11*4))
#define ISP1_CON2               (*(volatile u32 *)(hs_isp_base + 0x12*4))

#define IMC_CON0                (*(volatile u32 *)(hs_isp_base + 0x20*4))
#define IMC_CON1                (*(volatile u32 *)(hs_isp_base + 0x21*4))
#define IMC_CON2                (*(volatile u32 *)(hs_isp_base + 0x22*4))

#define IMB_CON0                (*(volatile u32 *)(hs_isp_base + 0x30*4))
#define IMB_CON1                (*(volatile u32 *)(hs_isp_base + 0x31*4))
#define IMB_CON2                (*(volatile u32 *)(hs_isp_base + 0x32*4))

//CVE
#define AVO_CON0                (*(volatile u32 *)(hs_cve_base + 0x00*4))
#define AVO_CON1                (*(volatile u32 *)(hs_cve_base + 0x01*4))
#define AVO_CON2                (*(volatile u32 *)(hs_cve_base + 0x02*4))
#define AVO_CON3                (*(volatile u32 *)(hs_cve_base + 0x03*4))
#define AVO_CON4                (*(volatile u32 *)(hs_cve_base + 0x04*4))

//SFC
#define SFC_CON                 (*(volatile u32 *)(hs_sfc_base + 0x00*4))
#define SFC_BAUD                (*(volatile u16 *)(hs_sfc_base + 0x01*4))

#define SFC_BASE_ADR            (*(volatile u32 *)(hs_sfc_base + 0x03*4))

#define JPG0_CON0                (*(volatile u32 *)(hs_jpg_base + 0x00*4))
#define JPG0_CON1                (*(volatile u32 *)(hs_jpg_base + 0x01*4))
#define JPG0_CON2                (*(volatile u32 *)(hs_jpg_base + 0x02*4))
#define JPG0_YDCVAL              (*(volatile u32 *)(hs_jpg_base + 0x03*4))
#define JPG0_UDCVAL              (*(volatile u32 *)(hs_jpg_base + 0x04*4))
#define JPG0_VDCVAL              (*(volatile u32 *)(hs_jpg_base + 0x05*4))
#define JPG0_YPTR0               (*(volatile u32 *)(hs_jpg_base + 0x06*4))
#define JPG0_UPTR0               (*(volatile u32 *)(hs_jpg_base + 0x07*4))
#define JPG0_VPTR0               (*(volatile u32 *)(hs_jpg_base + 0x08*4))
#define JPG0_YPTR1               (*(volatile u32 *)(hs_jpg_base + 0x09*4))
#define JPG0_UPTR1               (*(volatile u32 *)(hs_jpg_base + 0x0a*4))
#define JPG0_VPTR1               (*(volatile u32 *)(hs_jpg_base + 0x0b*4))
#define JPG0_BADDR               (*(volatile u32 *)(hs_jpg_base + 0x0c*4))
#define JPG0_BCNT                (*(volatile u32 *)(hs_jpg_base + 0x0d*4))
#define JPG0_MCUCNT              (*(volatile u32 *)(hs_jpg_base + 0x0e*4))
#define JPG0_PRECNT              (*(volatile u32 *)(hs_jpg_base + 0x0f*4))
#define JPG0_YUVLINE             (*(volatile u32 *)(hs_jpg_base + 0x10*4))
#define JPG0_CFGRAMADDR          (*(volatile u32 *)(hs_jpg_base + 0x11*4))
#define JPG0_CFGRAMVAL           (*(volatile u32 *)(hs_jpg_base + 0x12*4))
#define JPG0_TIME           (*(volatile u32 *)(hs_jpg_base + 0x13*4))

#define JPG1_CON0                (*(volatile u32 *)(hs_jpg_base + 0x40*4))
#define JPG1_CON1                (*(volatile u32 *)(hs_jpg_base + 0x41*4))
#define JPG1_CON2                (*(volatile u32 *)(hs_jpg_base + 0x42*4))
#define JPG1_YDCVAL              (*(volatile u32 *)(hs_jpg_base + 0x43*4))
#define JPG1_UDCVAL              (*(volatile u32 *)(hs_jpg_base + 0x44*4))
#define JPG1_VDCVAL              (*(volatile u32 *)(hs_jpg_base + 0x45*4))
#define JPG1_YPTR0               (*(volatile u32 *)(hs_jpg_base + 0x46*4))
#define JPG1_UPTR0               (*(volatile u32 *)(hs_jpg_base + 0x47*4))
#define JPG1_VPTR0               (*(volatile u32 *)(hs_jpg_base + 0x48*4))
#define JPG1_YPTR1               (*(volatile u32 *)(hs_jpg_base + 0x49*4))
#define JPG1_UPTR1               (*(volatile u32 *)(hs_jpg_base + 0x4a*4))
#define JPG1_VPTR1               (*(volatile u32 *)(hs_jpg_base + 0x4b*4))
#define JPG1_BADDR               (*(volatile u32 *)(hs_jpg_base + 0x4c*4))
#define JPG1_BCNT                (*(volatile u32 *)(hs_jpg_base + 0x4d*4))
#define JPG1_MCUCNT              (*(volatile u32 *)(hs_jpg_base + 0x4e*4))
#define JPG1_PRECNT              (*(volatile u32 *)(hs_jpg_base + 0x4f*4))
#define JPG1_YUVLINE             (*(volatile u32 *)(hs_jpg_base + 0x50*4))
#define JPG1_CFGRAMADDR          (*(volatile u32 *)(hs_jpg_base + 0x51*4))
#define JPG1_CFGRAMVAL           (*(volatile u32 *)(hs_jpg_base + 0x52*4))
//#define JPG1_CFGRAMVAL           (*(volatile u32 *)(hs_jpg_base + 0x2c00*4))


#define ENC_CON                 (*(volatile u32 *)(hs_oth_base + 0x00*4))
#define ENC_KEY                 (*(volatile u32 *)(hs_oth_base + 0x01*4))
#define ENC_ADR                 (*(volatile u32 *)(hs_oth_base + 0x02*4))
#define SFC_UNENC_ADRH          (*(volatile u32 *)(hs_oth_base + 0x03*4))
#define SFC_UNENC_ADRL          (*(volatile u32 *)(hs_oth_base + 0x04*4))
#define DMA_CON                 (*(volatile u32 *)(hs_oth_base + 0x05*4))
#define DMA_CNT                 (*(volatile u32 *)(hs_oth_base + 0x06*4))   //write only
#define DMA_RADR                (*(volatile u32 *)(hs_oth_base + 0x07*4))   //write only
#define DMA_WADR                (*(volatile u32 *)(hs_oth_base + 0x08*4))   //write only
//...........  Full Speed USB .....................
#define FADDR       0x00
#define POWER       0x01
#define INTRTX1     0x02
#define INTRTX2     0x03
#define INTRRX1     0x04
#define INTRRX2     0x05
#define INTRUSB     0x06
#define INTRTX1E    0x07
#define INTRTX2E    0x08
#define INTRRX1E    0x09
#define INTRRX2E    0x0a
#define INTRUSBE    0x0b
#define FRAME1      0x0c
#define FRAME2      0x0d
#define INDEX       0x0e
#define DEVCTL      0x0f
#define TXMAXP      0x10
#define CSR0        0x11
#define TXCSR1      0x11
#define TXCSR2      0x12
#define RXMAXP      0x13
#define RXCSR1      0x14
#define RXCSR2      0x15
#define COUNT0      0x16
#define RXCOUNT1    0x16
#define RXCOUNT2    0x17
#define TXTYPE      0x18
#define TXINTERVAL  0x19
#define RXTYPE      0x1a
#define RXINTERVAL  0x1b

//...........  High Speed USB .....................
#define husb_base /*0x7e000*/ ls_base + 0x7e00
#define H_FADDR         (*(volatile u8  *)(husb_base + 0x000))
#define H_POWER         (*(volatile u8  *)(husb_base + 0x001))
#define H_INTRTX        (*(volatile u16 *)(husb_base + 0x002))
#define H_INTRRX        (*(volatile u16 *)(husb_base + 0x004))
#define H_INTRTXE       (*(volatile u16 *)(husb_base + 0x006))
#define H_INTRRXE       (*(volatile u16 *)(husb_base + 0x008))
#define H_INTRUSB       (*(volatile u8  *)(husb_base + 0x00a))
#define H_INTRUSBE      (*(volatile u8  *)(husb_base + 0x00b))
#define H_FRAME         (*(volatile u16 *)(husb_base + 0x00c))
#define H_INDEX         (*(volatile u8  *)(husb_base + 0x00e))
#define H_TESTMODE      (*(volatile u8  *)(husb_base + 0x00f))

#define H_FIFO0         (*(volatile u8  *)(husb_base + 0x020))
#define H_FIFO1         (*(volatile u8  *)(husb_base + 0x024))
#define H_FIFO2         (*(volatile u8  *)(husb_base + 0x028))
#define H_FIFO3         (*(volatile u8  *)(husb_base + 0x02c))
#define H_FIFO4         (*(volatile u8  *)(husb_base + 0x030))
#define H_DEVCTL        (*(volatile u8  *)(husb_base + 0x060))

#define H_CSR0          (*(volatile u16 *)(husb_base + 0x102))
#define H_COUNT0        (*(volatile u16 *)(husb_base + 0x108))
#define H_NAKLIMIT0     (*(volatile u8  *)(husb_base + 0x10b))
#define H_CFGDATA       (*(volatile u8  *)(husb_base + 0x10f))

#define H_EP1TXMAXP     (*(volatile u16 *)(husb_base + 0x110))
#define H_EP1TXCSR      (*(volatile u16 *)(husb_base + 0x112))
#define H_EP1RXMAXP     (*(volatile u16 *)(husb_base + 0x114))
#define H_EP1RXCSR      (*(volatile u16 *)(husb_base + 0x116))
#define H_EP1RXCOUNT    (*(volatile u16 *)(husb_base + 0x118))
#define H_EP1TXTYPE     (*(volatile u8  *)(husb_base + 0x11a))
#define H_EP1TXINTERVAL (*(volatile u8  *)(husb_base + 0x11b))
#define H_EP1RXTYPE     (*(volatile u8  *)(husb_base + 0x11c))
#define H_EP1RXINTERVAL (*(volatile u8  *)(husb_base + 0x11d))
#define H_EP1FIFOSIZE   (*(volatile u8  *)(husb_base + 0x11f))

#define H_EP2TXMAXP     (*(volatile u16 *)(husb_base + 0x120))
#define H_EP2TXCSR      (*(volatile u16 *)(husb_base + 0x122))
#define H_EP2RXMAXP     (*(volatile u16 *)(husb_base + 0x124))
#define H_EP2RXCSR      (*(volatile u16 *)(husb_base + 0x126))
#define H_EP2RXCOUNT    (*(volatile u16 *)(husb_base + 0x128))
#define H_EP2TXTYPE     (*(volatile u8  *)(husb_base + 0x12a))
#define H_EP2TXINTERVAL (*(volatile u8  *)(husb_base + 0x12b))
#define H_EP2RXTYPE     (*(volatile u8  *)(husb_base + 0x12c))
#define H_EP2RXINTERVAL (*(volatile u8  *)(husb_base + 0x12d))
#define H_EP2FIFOSIZE   (*(volatile u8  *)(husb_base + 0x12f))

#define H_EP3TXMAXP     (*(volatile u16 *)(husb_base + 0x130))
#define H_EP3TXCSR      (*(volatile u16 *)(husb_base + 0x132))
#define H_EP3RXMAXP     (*(volatile u16 *)(husb_base + 0x134))
#define H_EP3RXCSR      (*(volatile u16 *)(husb_base + 0x136))
#define H_EP3RXCOUNT    (*(volatile u16 *)(husb_base + 0x138))
#define H_EP3TXTYPE     (*(volatile u8  *)(husb_base + 0x13a))
#define H_EP3TXINTERVAL (*(volatile u8  *)(husb_base + 0x13b))
#define H_EP3RXTYPE     (*(volatile u8  *)(husb_base + 0x13c))
#define H_EP3RXINTERVAL (*(volatile u8  *)(husb_base + 0x13d))
#define H_EP3FIFOSIZE   (*(volatile u8  *)(husb_base + 0x13f))

#define H_EP4TXMAXP     (*(volatile u16 *)(husb_base + 0x140))
#define H_EP4TXCSR      (*(volatile u16 *)(husb_base + 0x142))
#define H_EP4RXMAXP     (*(volatile u16 *)(husb_base + 0x144))
#define H_EP4RXCSR      (*(volatile u16 *)(husb_base + 0x146))
#define H_EP4RXCOUNT    (*(volatile u16 *)(husb_base + 0x148))
#define H_EP4TXTYPE     (*(volatile u8  *)(husb_base + 0x14a))
#define H_EP4TXINTERVAL (*(volatile u8  *)(husb_base + 0x14b))
#define H_EP4RXTYPE     (*(volatile u8  *)(husb_base + 0x14c))
#define H_EP4RXINTERVAL (*(volatile u8  *)(husb_base + 0x14d))
#define H_EP4FIFOSIZE   (*(volatile u8  *)(husb_base + 0x14f))

//...........  isp0 config .....................
#define isp0_base 0x1f74000
#define isp0_sen_con       (*(volatile u32 *)(isp0_base + 0x0000*4))
#define isp0_sen_vblk      (*(volatile u32 *)(isp0_base + 0x0001*4))
#define isp0_sen_vact      (*(volatile u32 *)(isp0_base + 0x0002*4))
#define isp0_sen_hblk      (*(volatile u32 *)(isp0_base + 0x0003*4))
#define isp0_sen_hact      (*(volatile u32 *)(isp0_base + 0x0004*4))
#define isp0_sen_hinv      (*(volatile u32 *)(isp0_base + 0x0005*4))

#define isp0_sta_win_width (*(volatile u32 *)(isp0_base + 0x0006*4))
#define isp0_sta_win_high  (*(volatile u32 *)(isp0_base + 0x0007*4))
#define isp0_sta_base_h    (*(volatile u32 *)(isp0_base + 0x0008*4))
#define isp0_sta_base_l    (*(volatile u32 *)(isp0_base + 0x0009*4))

#define isp0_blc_off_r     (*(volatile u32 *)(isp0_base + 0x000a*4))
#define isp0_blc_off_g     (*(volatile u32 *)(isp0_base + 0x000b*4))
#define isp0_blc_off_b     (*(volatile u32 *)(isp0_base + 0x000c*4))

#define isp0_dpc_max_th    (*(volatile u32 *)(isp0_base + 0x000d*4))
#define isp0_dpc_min_th    (*(volatile u32 *)(isp0_base + 0x000e*4))

#define isp0_lsc_cen_x     (*(volatile u32 *)(isp0_base + 0x000f*4))
#define isp0_lsc_cen_y     (*(volatile u32 *)(isp0_base + 0x0010*4))
#define isp0_lsc_lut       (*(volatile u32 *)(isp0_base + 0x0011*4))

#define isp0_awb_gain_r    (*(volatile u32 *)(isp0_base + 0x0411*4))
#define isp0_awb_gain_g    (*(volatile u32 *)(isp0_base + 0x0412*4))
#define isp0_awb_gain_b    (*(volatile u32 *)(isp0_base + 0x0413*4))

#define isp0_drc_con       (*(volatile u32 *)(isp0_base + 0x0414*4))
#define isp0_drc_lut       (*(volatile u32 *)(isp0_base + 0x0415*4))

#define isp0_sta_ae_con    (*(volatile u32 *)(isp0_base + 0x0815*4))
#define isp0_sta_ae_lvl1   (*(volatile u32 *)(isp0_base + 0x0816*4))
#define isp0_sta_ae_lvl2   (*(volatile u32 *)(isp0_base + 0x0817*4))
#define isp0_sta_ae_lvl3   (*(volatile u32 *)(isp0_base + 0x0818*4))
#define isp0_sta_ae_lvl4   (*(volatile u32 *)(isp0_base + 0x0819*4))
#define isp0_sta_ae_lvl5   (*(volatile u32 *)(isp0_base + 0x081a*4))
#define isp0_sta_ae_lvl6   (*(volatile u32 *)(isp0_base + 0x081b*4))
#define isp0_sta_ae_lvl7   (*(volatile u32 *)(isp0_base + 0x081c*4))

#define isp0_sta_wb_con    (*(volatile u32 *)(isp0_base + 0x081d*4))
#define isp0_sta_wb_w_th   (*(volatile u32 *)(isp0_base + 0x081e*4))
#define isp0_sta_wb_r_th   (*(volatile u32 *)(isp0_base + 0x081f*4))
#define isp0_sta_wb_g_th   (*(volatile u32 *)(isp0_base + 0x0820*4))
#define isp0_sta_wb_b_th   (*(volatile u32 *)(isp0_base + 0x0821*4))

#define isp0_ccm_r_coe0    (*(volatile u32 *)(isp0_base + 0x0822*4))
#define isp0_ccm_r_coe1    (*(volatile u32 *)(isp0_base + 0x0823*4))
#define isp0_ccm_r_coe2    (*(volatile u32 *)(isp0_base + 0x0824*4))
#define isp0_ccm_g_coe0    (*(volatile u32 *)(isp0_base + 0x0825*4))
#define isp0_ccm_g_coe1    (*(volatile u32 *)(isp0_base + 0x0826*4))
#define isp0_ccm_g_coe2    (*(volatile u32 *)(isp0_base + 0x0827*4))
#define isp0_ccm_b_coe0    (*(volatile u32 *)(isp0_base + 0x0828*4))
#define isp0_ccm_b_coe1    (*(volatile u32 *)(isp0_base + 0x0829*4))
#define isp0_ccm_b_coe2    (*(volatile u32 *)(isp0_base + 0x082a*4))
#define isp0_ccm_r_off     (*(volatile u32 *)(isp0_base + 0x082b*4))
#define isp0_ccm_g_off     (*(volatile u32 *)(isp0_base + 0x082c*4))
#define isp0_ccm_b_off     (*(volatile u32 *)(isp0_base + 0x082d*4))

#define isp0_gma0_con      (*(volatile u32 *)(isp0_base + 0x082e*4))
#define isp0_gma_r_lut     (*(volatile u32 *)(isp0_base + 0x082f*4))
#define isp0_gma_g_lut     (*(volatile u32 *)(isp0_base + 0x092f*4))
#define isp0_gma_b_lut     (*(volatile u32 *)(isp0_base + 0x0a2f*4))
#define isp0_gma1_con      (*(volatile u32 *)(isp0_base + 0x0b2f*4))
#define isp0_gma_y_lut     (*(volatile u32 *)(isp0_base + 0x0b30*4))

#define isp0_shp_amount    (*(volatile u32 *)(isp0_base + 0x0c30*4))
#define isp0_shp_lone_th   (*(volatile u32 *)(isp0_base + 0x0c31*4))
#define isp0_shp_en_th     (*(volatile u32 *)(isp0_base + 0x0c32*4))
#define isp0_shp_max_th    (*(volatile u32 *)(isp0_base + 0x0c33*4))
#define isp0_shp_min_th    (*(volatile u32 *)(isp0_base + 0x0c34*4))
#define isp0_shp_coe0      (*(volatile u32 *)(isp0_base + 0x0c35*4))
#define isp0_shp_coe1      (*(volatile u32 *)(isp0_base + 0x0c36*4))
#define isp0_shp_coe2      (*(volatile u32 *)(isp0_base + 0x0c37*4))
#define isp0_shp_coe3      (*(volatile u32 *)(isp0_base + 0x0c38*4))
#define isp0_shp_coe4      (*(volatile u32 *)(isp0_base + 0x0c39*4))
#define isp0_shp_coe5      (*(volatile u32 *)(isp0_base + 0x0c3a*4))
#define isp0_shp_shft      (*(volatile u32 *)(isp0_base + 0x0c3b*4))

#define isp0_smt_en_th     (*(volatile u32 *)(isp0_base + 0x0c3c*4))
#define isp0_smt_coe0      (*(volatile u32 *)(isp0_base + 0x0c3d*4))
#define isp0_smt_coe1      (*(volatile u32 *)(isp0_base + 0x0c3e*4))
#define isp0_smt_coe2      (*(volatile u32 *)(isp0_base + 0x0c3f*4))
#define isp0_smt_coe3      (*(volatile u32 *)(isp0_base + 0x0c40*4))
#define isp0_smt_coe4      (*(volatile u32 *)(isp0_base + 0x0c41*4))
#define isp0_smt_coe5      (*(volatile u32 *)(isp0_base + 0x0c42*4))
#define isp0_smt_shft      (*(volatile u32 *)(isp0_base + 0x0c43*4))

#define isp0_dnr_mod       (*(volatile u32 *)(isp0_base + 0x0c44*4))
#define isp0_dnr_th        (*(volatile u32 *)(isp0_base + 0x0c45*4))

#define isp0_cbs_y_gain    (*(volatile u32 *)(isp0_base + 0x0c46*4))
#define isp0_cbs_u_gain    (*(volatile u32 *)(isp0_base + 0x0c47*4))
#define isp0_cbs_v_gain    (*(volatile u32 *)(isp0_base + 0x0c48*4))
#define isp0_cbs_y_offs    (*(volatile u32 *)(isp0_base + 0x0c49*4))
#define isp0_cbs_u_offs    (*(volatile u32 *)(isp0_base + 0x0c4a*4))
#define isp0_cbs_v_offs    (*(volatile u32 *)(isp0_base + 0x0c4b*4))

#define isp0_out_hst       (*(volatile u32 *)(isp0_base + 0x0c4c*4))
#define isp0_out_hed       (*(volatile u32 *)(isp0_base + 0x0c4d*4))
#define isp0_out_vst       (*(volatile u32 *)(isp0_base + 0x0c4e*4))
#define isp0_out_ved       (*(volatile u32 *)(isp0_base + 0x0c4f*4))
#define isp0_out_base_h    (*(volatile u32 *)(isp0_base + 0x0c50*4))
#define isp0_out_base_l    (*(volatile u32 *)(isp0_base + 0x0c51*4))

//...........  imc config .....................
#define imc_base 0x1f78000
#define imc_pnd_con         (*(volatile u32 *)(imc_base + 0x00*4))
#define imc_src0_con        (*(volatile u32 *)(imc_base + 0x01*4))
#define imc_src1_con        (*(volatile u32 *)(imc_base + 0x02*4))
#define imc_src2_con        (*(volatile u32 *)(imc_base + 0x03*4))
#define imc_src2_h_cfg      (*(volatile u32 *)(imc_base + 0x04*4))
#define imc_src2_v_cfg      (*(volatile u32 *)(imc_base + 0x05*4))
#define imc_src2_y_base     (*(volatile u32 *)(imc_base + 0x06*4))
#define imc_src2_u_base     (*(volatile u32 *)(imc_base + 0x07*4))
#define imc_src2_v_base     (*(volatile u32 *)(imc_base + 0x08*4))

#define imc_ch0_sca0_con    (*(volatile u32 *)(imc_base + 0x10*4))
#define imc_ch0_sca0_step   (*(volatile u32 *)(imc_base + 0x11*4))
#define imc_ch0_sca0_phase  (*(volatile u32 *)(imc_base + 0x12*4))
#define imc_ch0_sca0_width  (*(volatile u32 *)(imc_base + 0x13*4))
#define imc_ch0_sca1_con    (*(volatile u32 *)(imc_base + 0x14*4))
#define imc_ch0_sca1_step   (*(volatile u32 *)(imc_base + 0x15*4))
#define imc_ch0_sca1_phase  (*(volatile u32 *)(imc_base + 0x16*4))
#define imc_ch0_sca1_high   (*(volatile u32 *)(imc_base + 0x17*4))
#define imc_ch0_enc_con     (*(volatile u32 *)(imc_base + 0x18*4))
#define imc_ch0_enc_y_base0 (*(volatile u32 *)(imc_base + 0x19*4))
#define imc_ch0_enc_u_base0 (*(volatile u32 *)(imc_base + 0x1a*4))
#define imc_ch0_enc_v_base0 (*(volatile u32 *)(imc_base + 0x1b*4))
#define imc_ch0_enc_y_base1 (*(volatile u32 *)(imc_base + 0x1c*4))
#define imc_ch0_enc_u_base1 (*(volatile u32 *)(imc_base + 0x1d*4))
#define imc_ch0_enc_v_base1 (*(volatile u32 *)(imc_base + 0x1e*4))
#define imc_ch0_enc_o_base  (*(volatile u32 *)(imc_base + 0x1f*4))
#define imc_ch0_osd_color   (*(volatile u32 *)(imc_base + 0x20*4))
#define imc_ch0_osd_h_cfg   (*(volatile u32 *)(imc_base + 0x21*4))
#define imc_ch0_osd_v_cfg   (*(volatile u32 *)(imc_base + 0x22*4))

#define imc_ch1_sca0_con    (*(volatile u32 *)(imc_base + 0x30*4))
#define imc_ch1_sca0_step   (*(volatile u32 *)(imc_base + 0x31*4))
#define imc_ch1_sca0_phase  (*(volatile u32 *)(imc_base + 0x32*4))
#define imc_ch1_sca0_width  (*(volatile u32 *)(imc_base + 0x33*4))
#define imc_ch1_sca1_con    (*(volatile u32 *)(imc_base + 0x34*4))
#define imc_ch1_sca1_step   (*(volatile u32 *)(imc_base + 0x35*4))
#define imc_ch1_sca1_phase  (*(volatile u32 *)(imc_base + 0x36*4))
#define imc_ch1_sca1_high   (*(volatile u32 *)(imc_base + 0x37*4))
#define imc_ch1_enc_con     (*(volatile u32 *)(imc_base + 0x38*4))
#define imc_ch1_enc_y_base0 (*(volatile u32 *)(imc_base + 0x39*4))
#define imc_ch1_enc_u_base0 (*(volatile u32 *)(imc_base + 0x3a*4))
#define imc_ch1_enc_v_base0 (*(volatile u32 *)(imc_base + 0x3b*4))
#define imc_ch1_enc_y_base1 (*(volatile u32 *)(imc_base + 0x3c*4))
#define imc_ch1_enc_u_base1 (*(volatile u32 *)(imc_base + 0x3d*4))
#define imc_ch1_enc_v_base1 (*(volatile u32 *)(imc_base + 0x3e*4))
#define imc_ch1_enc_o_base  (*(volatile u32 *)(imc_base + 0x3f*4))
#define imc_ch1_osd_color   (*(volatile u32 *)(imc_base + 0x40*4))
#define imc_ch1_osd_h_cfg   (*(volatile u32 *)(imc_base + 0x41*4))
#define imc_ch1_osd_v_cfg   (*(volatile u32 *)(imc_base + 0x42*4))

#define imc_ch2_sca0_con    (*(volatile u32 *)(imc_base + 0x50*4))
#define imc_ch2_sca0_step   (*(volatile u32 *)(imc_base + 0x51*4))
#define imc_ch2_sca0_phase  (*(volatile u32 *)(imc_base + 0x52*4))
#define imc_ch2_sca0_width  (*(volatile u32 *)(imc_base + 0x53*4))
#define imc_ch2_sca1_con    (*(volatile u32 *)(imc_base + 0x54*4))
#define imc_ch2_sca1_step   (*(volatile u32 *)(imc_base + 0x55*4))
#define imc_ch2_sca1_phase  (*(volatile u32 *)(imc_base + 0x56*4))
#define imc_ch2_sca1_high   (*(volatile u32 *)(imc_base + 0x57*4))
#define imc_ch2_dis_con     (*(volatile u32 *)(imc_base + 0x58*4))
#define imc_ch2_dis_y_base  (*(volatile u32 *)(imc_base + 0x59*4))
#define imc_ch2_dis_u_base  (*(volatile u32 *)(imc_base + 0x5a*4))
#define imc_ch2_dis_v_base  (*(volatile u32 *)(imc_base + 0x5b*4))

#define imc_ch3_sca0_con    (*(volatile u32 *)(imc_base + 0x60*4))
#define imc_ch3_sca0_step   (*(volatile u32 *)(imc_base + 0x61*4))
#define imc_ch3_sca0_phase  (*(volatile u32 *)(imc_base + 0x62*4))
#define imc_ch3_sca0_width  (*(volatile u32 *)(imc_base + 0x63*4))
#define imc_ch3_sca1_con    (*(volatile u32 *)(imc_base + 0x64*4))
#define imc_ch3_sca1_step   (*(volatile u32 *)(imc_base + 0x65*4))
#define imc_ch3_sca1_phase  (*(volatile u32 *)(imc_base + 0x66*4))
#define imc_ch3_sca1_high   (*(volatile u32 *)(imc_base + 0x67*4))
#define imc_ch3_dis_con     (*(volatile u32 *)(imc_base + 0x68*4))
#define imc_ch3_dis_y_base  (*(volatile u32 *)(imc_base + 0x69*4))
#define imc_ch3_dis_u_base  (*(volatile u32 *)(imc_base + 0x6a*4))
#define imc_ch3_dis_v_base  (*(volatile u32 *)(imc_base + 0x6b*4))
//...........  imb config .....................
#define imb_base 0x1f7c000
#define imb_clk_con     (*(volatile u32 *)(imb_base + 0x000*4))
#define imb_io_con      (*(volatile u32 *)(imb_base + 0x001*4))

#define imb_img0_yb     (*(volatile u32 *)(imb_base + 0x002*4))
#define imb_img0_ub     (*(volatile u32 *)(imb_base + 0x003*4))
#define imb_img0_vb     (*(volatile u32 *)(imb_base + 0x004*4))
#define imb_img0_aw     (*(volatile u32 *)(imb_base + 0x005*4))
#define imb_img1_yb     (*(volatile u32 *)(imb_base + 0x006*4))
#define imb_img1_ub     (*(volatile u32 *)(imb_base + 0x007*4))
#define imb_img1_vb     (*(volatile u32 *)(imb_base + 0x008*4))
#define imb_img1_aw     (*(volatile u32 *)(imb_base + 0x009*4))
#define imb_mask_mb     (*(volatile u32 *)(imb_base + 0x00a*4))
#define imb_mask_aw     (*(volatile u32 *)(imb_base + 0x00b*4))

#define imb_osd0_w0b    (*(volatile u32 *)(imb_base + 0x00c*4))
#define imb_osd0_w1b    (*(volatile u32 *)(imb_base + 0x00d*4))
#define imb_osd0_w2b    (*(volatile u32 *)(imb_base + 0x00e*4))
#define imb_osd1_w0b    (*(volatile u32 *)(imb_base + 0x00f*4))
#define imb_osd1_w1b    (*(volatile u32 *)(imb_base + 0x010*4))
#define imb_osd1_w2b    (*(volatile u32 *)(imb_base + 0x011*4))

#define imb_l0_dat      (*(volatile u32 *)(imb_base + 0x012*4))
#define imb_l0_h_cfg0   (*(volatile u32 *)(imb_base + 0x013*4))
#define imb_l0_h_cfg1   (*(volatile u32 *)(imb_base + 0x014*4))
#define imb_l0_h_cfg2   (*(volatile u32 *)(imb_base + 0x015*4))
#define imb_l0_h_cfg3   (*(volatile u32 *)(imb_base + 0x016*4))
#define imb_l0_v_cfg0   (*(volatile u32 *)(imb_base + 0x017*4))
#define imb_l0_v_cfg1   (*(volatile u32 *)(imb_base + 0x018*4))
#define imb_l0_v_cfg2   (*(volatile u32 *)(imb_base + 0x019*4))
#define imb_l0_v_cfg3   (*(volatile u32 *)(imb_base + 0x01a*4))
#define imb_l0_v_cfg4   (*(volatile u32 *)(imb_base + 0x01b*4))

#define imb_l1_con      (*(volatile u32 *)(imb_base + 0x01c*4))
#define imb_l1_h_cfg0   (*(volatile u32 *)(imb_base + 0x01d*4))
#define imb_l1_h_cfg1   (*(volatile u32 *)(imb_base + 0x01e*4))
#define imb_l1_v_cfg0   (*(volatile u32 *)(imb_base + 0x01f*4))
#define imb_l1_v_cfg1   (*(volatile u32 *)(imb_base + 0x020*4))

#define imb_l2_con      (*(volatile u32 *)(imb_base + 0x021*4))
#define imb_l2_aph      (*(volatile u32 *)(imb_base + 0x022*4))
#define imb_l2_h_cfg0   (*(volatile u32 *)(imb_base + 0x023*4))
#define imb_l2_h_cfg1   (*(volatile u32 *)(imb_base + 0x024*4))
#define imb_l2_v_cfg0   (*(volatile u32 *)(imb_base + 0x025*4))
#define imb_l2_v_cfg1   (*(volatile u32 *)(imb_base + 0x026*4))

#define imb_l3_con      (*(volatile u32 *)(imb_base + 0x027*4))
#define imb_l3_w0_aph   (*(volatile u32 *)(imb_base + 0x028*4))
#define imb_l3_w1_aph   (*(volatile u32 *)(imb_base + 0x029*4))
#define imb_l3_w2_aph   (*(volatile u32 *)(imb_base + 0x02a*4))
#define imb_l3_w0_cfg0  (*(volatile u32 *)(imb_base + 0x02b*4))
#define imb_l3_w0_cfg1  (*(volatile u32 *)(imb_base + 0x02c*4))
#define imb_l3_w0_cfg2  (*(volatile u32 *)(imb_base + 0x02d*4))
#define imb_l3_w0_cfg3  (*(volatile u32 *)(imb_base + 0x02e*4))
#define imb_l3_w1_cfg0  (*(volatile u32 *)(imb_base + 0x02f*4))
#define imb_l3_w1_cfg1  (*(volatile u32 *)(imb_base + 0x030*4))
#define imb_l3_w1_cfg2  (*(volatile u32 *)(imb_base + 0x031*4))
#define imb_l3_w1_cfg3  (*(volatile u32 *)(imb_base + 0x032*4))
#define imb_l3_w2_cfg0  (*(volatile u32 *)(imb_base + 0x033*4))
#define imb_l3_w2_cfg1  (*(volatile u32 *)(imb_base + 0x034*4))
#define imb_l3_w2_cfg2  (*(volatile u32 *)(imb_base + 0x035*4))
#define imb_l3_w2_cfg3  (*(volatile u32 *)(imb_base + 0x036*4))

#define imb_l4_con      (*(volatile u32 *)(imb_base + 0x037*4))
#define imb_l4_w0_aph   (*(volatile u32 *)(imb_base + 0x038*4))
#define imb_l4_w1_aph   (*(volatile u32 *)(imb_base + 0x039*4))
#define imb_l4_w2_aph   (*(volatile u32 *)(imb_base + 0x03a*4))
#define imb_l4_w0_cfg0  (*(volatile u32 *)(imb_base + 0x03b*4))
#define imb_l4_w0_cfg1  (*(volatile u32 *)(imb_base + 0x03c*4))
#define imb_l4_w0_cfg2  (*(volatile u32 *)(imb_base + 0x03d*4))
#define imb_l4_w0_cfg3  (*(volatile u32 *)(imb_base + 0x03e*4))
#define imb_l4_w1_cfg0  (*(volatile u32 *)(imb_base + 0x03f*4))
#define imb_l4_w1_cfg1  (*(volatile u32 *)(imb_base + 0x040*4))
#define imb_l4_w1_cfg2  (*(volatile u32 *)(imb_base + 0x041*4))
#define imb_l4_w1_cfg3  (*(volatile u32 *)(imb_base + 0x042*4))
#define imb_l4_w2_cfg0  (*(volatile u32 *)(imb_base + 0x043*4))
#define imb_l4_w2_cfg1  (*(volatile u32 *)(imb_base + 0x044*4))
#define imb_l4_w2_cfg2  (*(volatile u32 *)(imb_base + 0x045*4))
#define imb_l4_w2_cfg3  (*(volatile u32 *)(imb_base + 0x046*4))

#define imb_l5_y_gain   (*(volatile u32 *)(imb_base + 0x047*4))
#define imb_l5_u_gain   (*(volatile u32 *)(imb_base + 0x048*4))
#define imb_l5_v_gain   (*(volatile u32 *)(imb_base + 0x049*4))
#define imb_l5_y_offs   (*(volatile u32 *)(imb_base + 0x04a*4))
#define imb_l5_u_offs   (*(volatile u32 *)(imb_base + 0x04b*4))
#define imb_l5_v_offs   (*(volatile u32 *)(imb_base + 0x04c*4))
#define imb_l5_r_gain   (*(volatile u32 *)(imb_base + 0x04d*4))
#define imb_l5_g_gain   (*(volatile u32 *)(imb_base + 0x04e*4))
#define imb_l5_b_gain   (*(volatile u32 *)(imb_base + 0x04f*4))
#define imb_l5_r_coe0   (*(volatile u32 *)(imb_base + 0x050*4))
#define imb_l5_r_coe1   (*(volatile u32 *)(imb_base + 0x051*4))
#define imb_l5_r_coe2   (*(volatile u32 *)(imb_base + 0x052*4))
#define imb_l5_g_coe0   (*(volatile u32 *)(imb_base + 0x053*4))
#define imb_l5_g_coe1   (*(volatile u32 *)(imb_base + 0x054*4))
#define imb_l5_g_coe2   (*(volatile u32 *)(imb_base + 0x055*4))
#define imb_l5_b_coe0   (*(volatile u32 *)(imb_base + 0x056*4))
#define imb_l5_b_coe1   (*(volatile u32 *)(imb_base + 0x057*4))
#define imb_l5_b_coe2   (*(volatile u32 *)(imb_base + 0x058*4))
#define imb_l5_r_offs   (*(volatile u32 *)(imb_base + 0x059*4))
#define imb_l5_g_offs   (*(volatile u32 *)(imb_base + 0x05a*4))
#define imb_l5_b_offs   (*(volatile u32 *)(imb_base + 0x05b*4))

#define imb_l6_con      (*(volatile u32 *)(imb_base + 0x05c*4))
#define imb_l6_fmt      (*(volatile u32 *)(imb_base + 0x05d*4))
#define imb_emi_con     (*(volatile u32 *)(imb_base + 0x05e*4))

#define imb_osd0_tab    (*(volatile u32 *)(imb_base + 0x100*4))
#define imb_osd1_tab    (*(volatile u32 *)(imb_base + 0x200*4))
#define imb_r_gma_tab   (*(volatile u32 *)(imb_base + 0x300*4))
#define imb_g_gma_tab   (*(volatile u32 *)(imb_base + 0x400*4))
#define imb_b_gma_tab   (*(volatile u32 *)(imb_base + 0x500*4))

//=============== ISP1 REG ADDR ==================//
#define isp1_sen_con        0x00
#define isp1_sen_hst        0x01
#define isp1_sen_hed        0x02
#define isp1_sen_vst_o      0x03
#define isp1_sen_ved_o      0x04
#define isp1_sen_vst_e      0x05
#define isp1_sen_ved_e      0x06

#define isp1_dit_con        0x07
#define isp1_dit_haw        0x08
#define isp1_dit_hbw        0x09
#define isp1_dit_vaw        0x0a

#endif




