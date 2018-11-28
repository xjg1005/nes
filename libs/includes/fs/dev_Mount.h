#ifndef DEV_MOUNT_H
#define	DEV_MOUNT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "typedef.h"
#include "dev_Cache.h"


#define MAX_PART_NUM		5



    typedef struct
    {
        u32 bootSect[MAX_PART_NUM];		//分区的起始sector列表
        u8  sectCnt;						//有效的分区个数
    } DEV_PART;




    /*-------------------------挂载设备--------------------------
    	pDevCache		:	已初始化过的设备缓存地址
    	pDevPart		：	设备挂载后的分区表信息
    	sector_512size	：	扇区大小/512
    *-----------------------------------------------------------*/
//tbool  dev_Mount(DEV_CACHE   *pDevCache, DEV_PART   *pDevPart , u8 sector_512size);

    u32 dev_Mount(DEV_CACHE *pDevCache, u32 bootSect,u32 partNum , tu8 sector_512size);

    u32 get_first_one(u32 n);
    tu16 ld_word(u8 *p);
    u32 ld_dword(u8 *p);

#ifdef __cplusplus
}
#endif


#endif

