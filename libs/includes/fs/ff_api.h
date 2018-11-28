#ifndef _FF_API
#define _FF_API

#include "tff.h"
#include "dev_Cache.h"

#define MAX_DEEPTH		5				/* 0~8 deepth of system */

#if DIR_RECORD_EN
#define		LAST_FILE_NUM		ff_api->fs_msg.lastFileNumber
#else
#define		LAST_FILE_NUM		ff_api->fs_msg.fileNumber
#endif

typedef struct
{
	u8  flname[256];
	u32 flname_len;
	u32	fsize;
	u8	fattrib;
	u8  fname[15];
}FILE_PARAM;

typedef struct _FF_APIS
{
    FSAPIMSG fs_msg;
    DIRINFO  dir_info;
    DIR     path_dj[MAX_DEEPTH];				/* 用于搜索文件时，作为搜索路径的缓存 */
#if DIR_RECORD_EN
    DIR		currDir;
#endif
    FAT_LAYER_HDL   *pFatHdler;
    u32     sclust_id;
    u32     fsize_id;
} FF_APIS;

enum
{
    SEARCH_FILE_BY_NUMBER,  //按文件序号搜索文件
    SEARCH_SPEC_FILE,       //搜索指定的文件
    SEARCH_DISK,            //搜索总有效文件数
    SEARCH_FOLDER           //搜索当前目录根下的有效文件数。
};

#if DIR_RECORD_EN
typedef struct __DIR_RECORD
{
    u32 sector;
    u16 cindex;
    u16 firstFileNums;
} DIR_RECORD;
#endif

typedef struct __DIR_FIND
{
    u32 cur_file_number;
    u32 file_counter;
} DIR_FIND;

extern const char *ext_table;
extern u8  sector_buffer[][512];
extern u32 sector_num[];
extern u32 total_file;
extern DIR_FIND fd_msg;

#if DIR_RECORD_EN
extern u8			g_recordNum;
extern u8			g_recordCnt;
extern DIR_RECORD*	g_dirRecord;
#endif

/* function list */

void fast_scan_files (FF_APIS * ff_api, CACHE_INFO *pCache);

tbool check_ext(u8 *pExtName);

DIR * fs_getdir_byfile(FF_APIS *ff_api, tu16 fileNumber);

tbool scan_files (FF_APIS *ff_api, DIR * dj, u32 search_mode);

void fs_dirRecordAdd(FF_APIS *ff_api);

void fs_ext_setting(const char * str);

tu16 fs_fast_scan_disk(FF_APIS *ff_api, FAT_LAYER_HDL   *pFatHdler);

tu16 fs_scan_disk(FF_APIS *ff_api, FAT_LAYER_HDL *pFatHdler, u32 sclust, u32 fsize);

tbool fs_getfile_bynumber(FF_APIS *f_api, tu16 fileNumber);

tu16 fs_getfile_totalindir(FF_APIS *f_api);

#if DIR_RECORD_EN
void fs_dir_record_init(void *pBuf, u32 len);

u32 fs_getfile_totalOutdir(FF_APIS *ff_api);
#endif




#endif
