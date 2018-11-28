#ifndef FAT_SYS_H
#define FAT_SYS_H

#include "typedef.h"
#include "dev_Cache.h"


struct if_fat_dev_ops {
	u32 (*read)(void *buf, u32 addr);
	u32 (*write)(void *buf, u32 addr);
};

struct fat_dev_caches {
	DEV_CACHE 	*cache;
	CACHE_INFO	*cache_info;
	u8 cache_info_num;
};











#endif

