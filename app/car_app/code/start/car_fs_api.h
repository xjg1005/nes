
#ifndef __CAR_FS_API_H__
#define __CAR_FS_API_H__

//#include "sd2_host_api.h"
//#include "dev_Cache.h"
//#include "fat_Mount.h"
//#include "ff_api.h"
//#include "fs_Api.h"
#include "mango_dev_sd.h"
#include "app_cfg.h"

#include "dev_ctrl.h"
#include "FileSelecter.h"



#define FS_NEXT_FILE    0x01
#define FS_PREV_FILE    0x02
#define FS_FIRST_FILE   0x03


u16 fs_wait_data_ok();
u16 fs_data_ok();


#define FS_ENTER_CRITICAL()     fs_wait_data_ok()
#define FS_EXIT_CRITICAL()      fs_data_ok()

//#define VFS_ENTER_CRITICAL()     FS_ENTER_CRITICAL()
//#define VFS_EXIT_CRITICAL()      FS_EXIT_CRITICAL()

#define VFS_ENTER_MUTEX()     FS_ENTER_CRITICAL()
#define VFS_EXIT_MUTEX()      FS_EXIT_CRITICAL()


#define FILE_ENTER_MUTEX()     VFS_ENTER_MUTEX()
#define FILE_EXIT_MUTEX()      VFS_EXIT_MUTEX()




//tbool mutex_f_Open(void  *pFile, void   *pFatCreate, u8  *path, u32 openMode);
file_t * mutex_f_Open(u8  *path, u32 openMode);
u32 mutex_f_Write(void  *pFile, void  *pBuf, u32 len);
u32 mutex_f_Read(void   * pFile ,void   *pBuf , u32 btr);
tbool mutex_f_Seek (void   * pFile, u32 type, u32 offsize);
u32 mutex_f_Tell(void   * pFile);
void mutex_f_Close(void  *pFile);
u32 mutex_f_size(char  *pfile_path);

void * read_file(u8 search_mode, u8 * file_type);

#endif




