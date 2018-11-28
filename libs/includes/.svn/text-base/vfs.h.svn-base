
/*************************************************************
File: vfs.h
Author:xuhuajun
Discriptor:
    vfs函数及数据结构定义
Version:1.0
Date：2014.5
*************************************************************/

#ifndef _VFS_
#define _VFS_
#include "typedef.h"
#include "fresultcode.h"
#ifdef WIN32
#include "stdio.h"
#else
#include "drv_manage.h"
#endif

#define MAXFILENO file_no_num
#define MAXFSADAPTER 2
#define MAXDRIVER 5
#define MAXSTOREDEVICE 2
#define MAXPARTITIONPERDEVICE 4
#define DEFAULT_PARTITION_BUF_SIZE 4096

typedef struct
{
	u8 drv;
	u8 free;				//0:未使用，1使用
    u8 temp[2];
	u8 real_file_handle[64];//FS使用的文件句柄
} file_t;

typedef struct
{
	u8 *dev_name;
	u8 free;
	void * devHandle;
	u32 relative_sectors[MAXPARTITIONPERDEVICE];
	u32 total_sectors[MAXPARTITIONPERDEVICE];
	u8* buffer[MAXPARTITIONPERDEVICE];
	u32 buffer_len[MAXPARTITIONPERDEVICE];
}device_info;

struct DRIVER_INFO;
typedef struct DRIVER_INFO driver_info;
typedef struct
{
	u8 *dev_name;	//通用文件系统时为空，专用文件系统时指定设备名
	void (*init_cur_fat)();
	u32 (*get_gartiotion_nums)();
	u32 (*mount_partition)(u32 partition_index , device_info * the_device);
	u32 (*fopen)(void *filehandle , u8* path ,u32 opt, driver_info *the_driver);
	u32 (*fread)(void *fp,u8 *ptr,u32 len, driver_info *the_driver);
	u32 (*fwrite)(void *fp,u8* ptr, u32 len, driver_info *the_driver) ;
	u32 (*fseek)(void *fp, u32 len, u32 opt, driver_info *the_driver);
	u32 (*ftell)(void *fp, driver_info *the_driver) ;
	void (*fclose)(void *fp, driver_info *the_driver) ;
	u32 (*fdelete)(u8* path, driver_info *the_driver);
	u32 (*get_file_name)(void *fp ,u8 * ptr ,u32 len ,u32 opt, driver_info *the_driver);
	u32 (*fopen_byindex)(u32 index,driver_info *the_driver, void *fp);
	//u32 (*get_file_num)(u8* ext, u8* buf, u32 buf_len, driver_info *the_driver);
	u32 (*f_emu)(u8* filter, void *pram2callback, driver_info *the_driver , s32 (*callback)(void *fileparam, void *pram2callback));
	u32 (*f_chdir)(u8* path, driver_info *the_driver);
	u32 (*f_mkdir)(u8* path, u32 opt, driver_info *the_driver);
	u32 (*get_free_space)(driver_info *the_driver);
	u32 (*get_file_length)(u8* path, driver_info *the_driver);
	u8  (*get_file_attr)(u8* path, driver_info *the_driver);
	u32 (*f_rename)(u8* oldname, u8* newname, driver_info *the_driver);
	u32 (*fsize)(void *fp, driver_info *the_driver);
	u32 (*get_cluster_size)(driver_info *the_driver);
	u32 (*f_findfirst)(void* dp , void* fno , const char* path , const char* pattern , driver_info *the_driver);
	u32 (*f_findnext)(void* dp , void* fno , driver_info *the_driver);
	u32 (*f_closedir)(void *dp , driver_info *the_driver);
	WORD (*get_file_time)(u8* path, driver_info *the_driver);
	WORD (*get_file_date)(u8* path, driver_info *the_driver);

}fs_adapter;



typedef struct DRIVER_INFO
{
	u8 drv;
	u8 free;				//0:未使用，1使用
	fs_adapter	*thefs;		//此盘所用的FS
	device_info *the_device;
	int	partition_index;
	void *reg_dev_io;
};

/* File status structure (FILINFO) */

typedef struct {
	DWORD	fsize;			/* File size */
	WORD	fdate;			/* Last modified date */
	WORD	ftime;			/* Last modified time */
	BYTE	fattrib;		/* Attribute */
	char	fname[13];		/* Short file name (8.3 format) */
#if _USE_LFN
	TCHAR*	lfname;			/* Pointer to the LFN buffer */
	UINT 	lfsize;			/* Size of LFN buffer in TCHAR */
#endif
} F_FILINFO;


typedef struct {
	void*	fs;				/* Pointer to the owner file system object (**do not change order**) */
	WORD	id;				/* Owner file system mount ID (**do not change order**) */
	WORD	index;			/* Current read/write index number */
	DWORD	sclust;			/* Table start cluster (0:Root dir) */
	DWORD	clust;			/* Current cluster */
	DWORD	sect;			/* Current sector */
	BYTE*	dir;			/* Pointer to the current SFN entry in the win[] */
	BYTE*	fn;				/* Pointer to the SFN (in/out) {file[8],ext[3],status[1]} */
	BYTE*	pat;
#if _FS_LOCK
	UINT	lockid;			/* File lock ID (index of file semaphore table Files[]) */
#endif
#if _USE_LFN
	WCHAR*	lfn;			/* Pointer to the LFN working buffer */
	WORD	lfn_idx;		/* Last matched LFN index number (0xFFFF:No LFN) */
#endif
} SRCDIR;

typedef struct
{
	u8 drv;
	SRCDIR dir;
}F_DIR;

u8 currentDrv;
u8 current_path[512];
int file_no_num;
file_t		*vfs_file_handle;//[MAXFILENO];
fs_adapter	*vfs_fs_adapter[MAXFSADAPTER];
driver_info vfs_driver_info[MAXDRIVER];
device_info vfs_device_info[MAXSTOREDEVICE];

void (*callback_drv_onoff)(char drv, bool on_off);
u32 device_read(void * hDev , void *buf,u32 LBAAddr , u32 segment);
u32 device_write(void * hDev , void *buf,u32 LBAAddr , u32 segment);


/*管理接口*/
void init_VFS(int max_file_no);
/*
Dev_name为设备名，比如sd0、spi0，另外，对于自定义文件系统，该设备名可以自定义;
Opt用以指定是否用固定文件系统去扫描，0为自动用通用的文件系统去扫描，非0则用指定文件系统扫描。
注0：由opt指定文件系统，暂定指定文件系统ID;
注1：此函数不会去初始化物理存储设备，因此在调用之前，必须已完成设备的防问准备，比如，SD卡已签定完成。
*/

#ifdef WIN32
u32 scan_fs( const char *dev_name,u32 opt);
#else
u32 scan_fs( DEV_HANDLE dev_io,u32 opt);
#endif
/*
挂载磁盘某分区。
partition_index为存储设备上的分区序号，由0开始。

挂载成功返回0，成功后drv指向挂载后的盘符。
注0:挂载成功后，会当默认路径切换至此盘根目录。
*/
#ifdef WIN32
FRESULT mount_partition( const char *dev_name , u32 partition_index, char* drv,u32 buffersize);
#else
FRESULT mount_partition( DEV_HANDLE dev_io , u32 partition_index, char* drv,u32 buffersize);
#endif
FRESULT unmount_partition(const char drv);

/*格式化指定的分区*/
#ifdef WIN32
FRESULT mkfs_partition(const char *dev_name, u32 partition_index);
#else
FRESULT mkfs_partition(DEV_HANDLE dev_io, u32 partition_index);
#endif
/*格式化指定的盘符*/
FRESULT mkfs_drv(char drv);//B
FRESULT remount(char drv);
/*当有存储设备插入或拔出时调用，此函数由设备管理器调用
dev_name为设备发生变化的设备名
on_off: true:插入 false:拔出
*/
#ifdef WIN32
void device_on_off(const char *dev_name, bool on_off);
#else
void device_on_off(DEV_HANDLE dev_io, bool on_off);
#endif
u32 get_logic_drvs(char * drvs,int count);
/*管理接口---end*/

#ifdef WIN32
/*应用接口*/
file_t *vfs_fopen(u8* path ,u32 opt);
u32 vfs_frename(u8* oldpath ,u8* newpath);
u32 vfs_fread(file_t *fp,u8 *ptr,u32 len);
u32 vfs_fwrite(file_t *fp,u8* ptr, u32 len) ;
u32 vfs_fseek(file_t *fp, u32 opt , u32 len);
u32 vfs_ftell(file_t *fp) ;
u32 vfs_fsize(file_t *fp);
void vfs_fclose(file_t *fp) ;
u32 vfs_fdelete(u8* path);
u32 vfs_get_file_length(u8* path);//获取文件长度
u8  vfs_get_file_attr(u8* path);//获取文件属性
u32 vfs_get_file_name(file_t *fp ,u8 * ptr ,u32 len ,u32 opt);
file_t * vfs_fopen_byindex(char drv , u32 index); //根据据文件序号打开文件
//u32 vfs_get_file_num(char drv , u8* ext, u8* dirbuf, u32 buf_len);//扫描指定盘符中指定扩展名的文件数量
u32 vfs_f_emu(u8* path, void *pram2callback, s32 (*callback)(void *fileparam, void *pram2callback));
u32 vfs_f_chdir(u8* path);
u32 vfs_f_mkdir(u8* path, u32 opt);
u32 vfs_f_getdir(u8* buf, u32 len);
u32 vfs_get_free_space(u8 drv);
/*应用接口--end*/
#else
file_t *fopen(u8* path ,u32 opt);
u32 frename(u8* oldpath ,u8* newpath);
u32 fread(file_t *fp,u8 *ptr,u32 len);
u32 fwrite(file_t *fp,u8* ptr, u32 len);
u32 fseek(file_t *fp, u32 opt , u32 len);
u32 ftell(file_t *fp);
u32 fsize(file_t *fp);
void fclose(file_t *fp) ;
u32 fdelete(u8* path);
u32 get_file_length(u8* path);//获取文件长度
u8  get_file_attr(u8* path);//获取文件属性
u32 get_file_name(file_t *fp ,u8 * ptr ,u32 len ,u32 opt);
file_t * fopen_byindex(char drv , u32 index);
//u32 get_file_num(char drv , u8* ext, u8* dirbuf, u32 buf_len);
u32 f_emu(u8* path, void *pram2callback, s32 (*callback)(void *fileparam, void *pram2callback));
u32 f_chdir(u8* path);
u32 f_mkdir(u8* path, u32 opt);
u32 f_getdir(u8* buf, u32 len);
u32 get_free_space(u8 drv);
u32 get_clust_size(u8 drv);
#endif
/*文件系统相当接口*/
u32 reg_fs(fs_adapter* the_fs_adapter); //注册一个文件系统，不成功返回-1，否则返回为该FS的ID
u32 get_fs_index(fs_adapter* the_fs_adapter); //查询文件系统ID，没有则返回-1，否则返回为该FS的ID
/*文件系统相当接口*/
#endif
