#ifndef FAT_MOUNT_H
#define FAT_MOUNT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "typedef.h"
#include "dev_Cache.h"
#include "tff.h"


#define	REPAIR_FAT32_FSINFO_EN				1


    void fat_InitFatHdler(FAT_LAYER_HDL   *pFatHdler, DEV_CACHE   *pDevCache, u32 sector_sizeof512);


    tbool fat_Mount(FAT_LAYER_HDL   *pFatHdler, u32 bootsect);






#ifdef __cplusplus
}
#endif



#endif
