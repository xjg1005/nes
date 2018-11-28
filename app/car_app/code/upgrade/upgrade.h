#ifndef _UPGRADE_H_
#define _UPGRADE_H_
#include "mango_dev_sd.h"
#define MAX_FILE_NAME_LEN   12

typedef struct _update_info
{
    u32 dev_type;
//  [24-31]:0
//  [16-23]:表示设备号：0循环检测所有外设、sd--1、usb--2、flash--3、pc--4
//  [12-15]:表示设备出口
//  [8-11]:表示线模式
//  [0-7] ：设备的分频值

    u32 dest_addr;  //更新区域的起始地址
    u32 change_head_addr;//文件头修改地址
    u32 src_addr;   //升级文件的偏移地址
    char updata_file_name[MAX_FILE_NAME_LEN];//从设备升级时的文件名
    u16 res;
    u16 crc;
}UPDATE_INFO;

enum
{
    UPGRADE_DEV_SD = 0x10000,
    UPGRADE_DEV_USB = 0x20000,
    UPGRADE_DEV_FLASH = 0x30000,
    UPGRADE_DEV_PC = 0x40000,
};

enum
{
    UPGRADE_SD_TIMEOUT = 1,
    UPGRADE_NO_VALID_FILE,
    UPGRADE_LOW_VERSION,
};

u8 upgrade_check(void);
#endif
