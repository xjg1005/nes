#ifndef _MANGO_DEV_SPI_H_
#define _MANGO_DEV_SPI_H_

#include "typedef.h"
#include "ioctl.h"
#include "device_drive.h"


#define SPI_PARM_VALID_MASK	  0x5a5a0000
#define SPI_MODE_MASK         (0x00000c00|SPI_PARM_VALID_MASK)
#define SPI_CLK_MASK    	  0x000000ff

typedef enum _spi_mode
{
    SPI_2WIRE_MODE = 0x0|SPI_PARM_VALID_MASK,
    SPI_ODD_MODE   = 0x00000400|SPI_PARM_VALID_MASK,
    SPI_DUAL_MODE  = 0x00000800|SPI_PARM_VALID_MASK,
    SPI_QUAD_MODE  = 0x00000c00|SPI_PARM_VALID_MASK,
}spi_mode;

typedef enum _spi_clk
{
    SPI_CLK_DIV1 = 0,
    SPI_CLK_DIV2,
    SPI_CLK_DIV3,
    SPI_CLK_DIV4,
    SPI_CLK_DIV5,
    SPI_CLK_DIV6,
    SPI_CLK_DIV7,
    SPI_CLK_DIV8,
    SPI_CLK_DIV9,
    SPI_CLK_DIV10,
    SPI_CLK_DIV11,
    SPI_CLK_DIV12,
    SPI_CLK_DIV13,
    SPI_CLK_DIV14,
    SPI_CLK_DIV15,
    SPI_CLK_DIV16,
        /*
        .
        .
        .
        */
    SPI_CLK_DIV256 = 255,
}spi_clk;


extern const struct DEV_IO * dev_reg_spi0(void *parm);
extern const struct DEV_IO * dev_reg_spi1(void *parm);

#define SPI_MAGIC              'P'
#define SPI_SET_CS             _IOW(SPI_MAGIC,1,u8)
#define SPI_SET_CLK            _IOW(SPI_MAGIC,2,spi_clk)
#define SPI_WRITE_BYTE         _IOW(SPI_MAGIC,3,u8)
#define SPI_READ_BYTE          _IOR(SPI_MAGIC,4,u8)
#define SPI_REMAP_CS           _IOW(SPI_MAGIC,5,u32)


#endif //_MANGO_DEV_SPI0_H_

