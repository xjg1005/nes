#ifndef _UPGRADE_H_
#define _UPGRADE_H_
#include "mango_dev_sd.h"
#define MAX_FILE_NAME_LEN   12

typedef struct _update_info
{
    u32 dev_type;
//  [24-31]:0
//  [16-23]:��ʾ�豸�ţ�0ѭ������������衢sd--1��usb--2��flash--3��pc--4
//  [12-15]:��ʾ�豸����
//  [8-11]:��ʾ��ģʽ
//  [0-7] ���豸�ķ�Ƶֵ

    u32 dest_addr;  //�����������ʼ��ַ
    u32 change_head_addr;//�ļ�ͷ�޸ĵ�ַ
    u32 src_addr;   //�����ļ���ƫ�Ƶ�ַ
    char updata_file_name[MAX_FILE_NAME_LEN];//���豸����ʱ���ļ���
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
