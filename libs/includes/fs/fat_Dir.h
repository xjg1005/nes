#ifndef FAT_FS_LAYER_H
#define FAT_FS_LAYER_H


#ifdef __cplusplus
extern "C" {
#endif

#include "typedef.h"
#include "tff.h"
#include "dev_Cache.h"
#include "fat_Table.h"


#ifndef BIG_ENDIAN
//#error  "no define IS_BIG_ENDIAN"
#endif



typedef struct
{
    u16 year;
    u8 month;
    u8 day;
} FAT_DATE;

typedef struct
{
    u8 hour;
    u8 minute;
    u8 second;
} FAT_TIME;

typedef struct
{
    DIRINFO		dirInfo;
    DIR			dir;
    u8	        *pTmpBuf;			//�Զ������ļ���ʱ�Ļ���buf��
    u16			len;				//���沿�ֳ��ȣ��ܹ��Զ����ɵ�����ļ��� = len*8 (e.g len=512ʱ��һ��Ŀ¼�����ɴ���512*8���ļ�)
    u8			name[11];
    u8			upCaseName[11];
    u32         cFileNum;
} FAT_CREATE;

typedef  void (*PFUN_GET_SYS_TIME)(FAT_DATE   *pDate , FAT_TIME   *pTime);
typedef tbool (* PFUN_CREATE_LFN)(FAT_CREATE   *pCreate);



extern PFUN_CREATE_LFN   g_pFun_CreateLfn;
extern PFUN_GET_SYS_TIME    g_pFun_GetSysTime;

tbool fatDir_ClearDirEntry(u32 startSect);

u32 fatDir_GetTimeDate();

void fatDir_InitHook(PFUN_CREATE_LFN pFunCreate,  PFUN_GET_SYS_TIME pFunGetSysTime);

FRESULT fatDir_FindDir(FAT_CREATE   *pCreate, u32 find);

tbool fatDir_CreateDirEntry(FAT_CREATE   *pFatCreate, u32 attr);

tbool fatDir_ExtendDirEntry(DIR   *pDir);

tbool fatDir_CreateDirOrFile(FAT_CREATE   *pFatCreate, u32 attr);

u32 fatDir_GetEntryName(DIR   *pDir , void   *pBuf , u32 bufLen);


#ifdef __cplusplus
}
#endif



#endif

