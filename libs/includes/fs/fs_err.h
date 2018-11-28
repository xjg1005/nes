#ifndef FS_ERR_H
#define	FS_ERR_H

#include "typedef.h"

typedef enum
{
    FS_ERR_NONE,
    FS_ERR_DEV_READ,
    FS_ERR_DEV_WRITE,
    FS_ERR_DISK_FULL,
    FS_ERR_DIR_FULL,
    FS_ERR_FILE_FULL,
    FS_ERR_UNWRITEABLE,

    FS_ERR_CACHE_ALLOC,
    FS_ERR_UNAVAILABLE_CLUST,

} FS_ERR;


extern u8	  gFsErrFlag;


tu8 fs_GetLastErr();













#endif



