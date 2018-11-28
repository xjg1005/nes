#ifndef _MANGO_DEV_SD_H_
#define _MANGO_DEV_SD_H_

#include "typedef.h"

#ifdef USE_RTOS
#include "ioctl.h"


#include "device_drive.h"
#endif

#define SD_PARM_VALID_MASK	  0x55a00000
#define SD_NWIRE_MODE_MASK    (0x00000f00|SD_PARM_VALID_MASK)
#define SD_CLK_MASK    		  0x000000ff
#define SD_PORTOUT_MASK       0x0000f000
#define SD_DECT_MASK          0x000f0000

typedef enum _sd_nwire_mode
{
    SD_1WIRE_MODE = 0x00000100|SD_PARM_VALID_MASK,
    SD_4WIRE_MODE = 0x00000200|SD_PARM_VALID_MASK,
} SD_NWIRE_MODE;

enum _sd_port_out
{
    SD_DEFAULT_PORT_OUT = 0,
    //sd0
    SD_PA1_4_PH1315 = 0x00001000,
    SD_PH6_11 = 0x00002000,
    SD_PC8_13 = 0x00003000,
    SD_PG8_13 = 0x00004000,
    //sd1
    SD_PF4_9 = 0x00005000,
    SD_PG0_5 = 0x00006000,
    SD_PD6_11 = 0x00007000,
    SD_PA10_15 = 0x00008000,
};

enum _sd_dect_mode
{
    SD_CMD_DECT = 0,
    SD_CLK_DECT = 0x00010000,
    SD_IO_DECT =  0x00020000,
};

typedef enum _SD_CLK
{
    SD_CLK_DIV1 = 0,
    SD_CLK_DIV2,
    SD_CLK_DIV3,
    SD_CLK_DIV4,
    SD_CLK_DIV5,
    SD_CLK_DIV6,
    SD_CLK_DIV7,
    SD_CLK_DIV8,
    SD_CLK_DIV9,
    SD_CLK_DIV10,
    SD_CLK_DIV11,
    SD_CLK_DIV12,
    SD_CLK_DIV13,
    SD_CLK_DIV14,
    SD_CLK_DIV15,
    SD_CLK_DIV16,
        /*
        .
        .
        .
        */
    SD_CLK_DIV256 = 255,
}SD_CLK;


extern const struct DEV_IO * dev_reg_sd0(void *parm);
extern const struct DEV_IO * dev_reg_sd1(void *parm);
extern const struct DEV_IO * dev_reg_sd2(void *parm);

#define SD_MAGIC                'S'
#define SD_SET_CLK              _IOW(SD_MAGIC,1,SD_CLK)
#define SD_SET_DECT_FUNC        _IOW(SD_MAGIC,2,bool (*)(void))
#define GET_VIR_DISK_HDL                    _IOR(SD_MAGIC,3,u32)
#define GET_VIR_DISK_BLOCK_NUM              _IOR(SD_MAGIC,4,u32)

#endif //_MANGO_DEV_SD0_H_

