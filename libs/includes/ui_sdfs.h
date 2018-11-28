#ifndef __UI_SDFS_H__
#define __UI_SDFS_H__

#ifdef __cplusplus
extern "C" {
#endif
	
#include "typedef.h"
#include "sdfile.h"
	
tbool ui_sd_fopen(const u8 *filename,sdfile_t *file);
u32   ui_sd_fread(sdfile_t *fp,void *buf,u32 len);
u32   ui_sd_fseek(sdfile_t *fp,u8 seek_mode,u32 offset);

#ifdef __cplusplus
}
#endif

#endif
