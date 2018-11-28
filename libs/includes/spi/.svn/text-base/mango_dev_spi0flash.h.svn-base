#ifndef _MANGO_DEV_SPI0_FLASH_H_
#define _MANGO_DEV_SPI0_FLASH_H_

#include "typedef.h"
#include "ioctl.h"
#include "device_drive.h"

#include "mango_dev_spi.h"

////**********写保护参数**********////
#define PROTECT_NONE		0x00
///////////////512K Flash///////////////
//////////TOP/////////
#define PROTECT_512K_0		0x24
#define PROTECT_512K_0_TO_1	0x28
#define PROTECT_512K_0_TO_3	0x2C
#define PROTECT_512K_0_TO_7	0x30
/////////Bottom///////
#define PROTECT_512K_7		0x04
#define PROTECT_512K_6_TO_7	0x08
#define PROTECT_512K_4_TO_7	0x0C
///////////////////////////////////////

///////////////1M Flash///////////////
//////////TOP/////////
#define PROTECT_1M_0		0x24
#define PROTECT_1M_0_TO_1	0x28
#define PROTECT_1M_0_TO_3	0x2C
#define PROTECT_1M_0_TO_7	0x30
#define PROTECT_1M_0_TO_15	0x34
/////////Bottom///////
#define PROTECT_1M_15		0x04
#define PROTECT_1M_14_TO_15	0x08
#define PROTECT_1M_12_TO_15	0x0C
#define PROTECT_1M_8_TO_15	0x10
///////////////////////////////////////

///////////////2M Flash///////////////
//////////TOP/////////
#define PROTECT_2M_0		0x24
#define PROTECT_2M_0_TO_1	0x28
#define PROTECT_2M_0_TO_3	0x2C
#define PROTECT_2M_0_TO_7	0x30
#define PROTECT_2M_0_TO_15	0x34
#define PROTECT_2M_0_TO_31	0x38
/////////Bottom///////
#define PROTECT_2M_31		0x04
#define PROTECT_2M_30_TO_31	0x08
#define PROTECT_2M_28_TO_31	0x0C
#define PROTECT_2M_24_TO_31	0x10
#define PROTECT_2M_16_TO_31	0x14
///////////////////////////////////////

///////////////4M Flash///////////////
//////////TOP/////////
#define PROTECT_4M_0		0x24
#define PROTECT_4M_0_TO_1	0x28
#define PROTECT_4M_0_TO_3	0x2C
#define PROTECT_4M_0_TO_7	0x30
#define PROTECT_4M_0_TO_15	0x34
#define PROTECT_4M_0_TO_31	0x38
#define PROTECT_4M_0_TO_63	0x3C
/////////Bottom///////
#define PROTECT_4M_63		0x04
#define PROTECT_4M_62_TO_63	0x08
#define PROTECT_4M_60_TO_63	0x0C
#define PROTECT_4M_56_TO_63	0x10
#define PROTECT_4M_48_TO_63	0x14
#define PROTECT_4M_32_TO_63	0x18
///////////////////////////////////////


typedef enum _FLASH_ERASER
{
	CHIP_ERASER,
	BLOCK_ERASER,
	SECTOR_ERASER,
}FLASH_ERASER;

typedef enum FLASH_MODE
{
	FAST_READ_OUTPUT_MODE = 0x0|SPI_PARM_VALID_MASK,
	FAST_READ_IO_MODE = 0x00001000|SPI_PARM_VALID_MASK,
	FAST_READ_IO_CONTINUOUS_READ_MODE = 0x00002000|SPI_PARM_VALID_MASK,
}FLASH_MODE;
#define FLASH_MODE_MASK         (0x00003000|SPI_PARM_VALID_MASK)

extern const struct DEV_IO * dev_reg_flash(void *parm);

#define SPI0FLASH_MAGIC             'F'
#define SPI0FLASH_REMAP_CS          _IOW(SPI0FLASH_MAGIC,2,u32)

#endif //_MANGO_DEV_SPI0_FLASH_H_

