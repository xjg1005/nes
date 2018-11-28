
/*************************************************************
File: vfs.h
Author:xuhuajun
Discriptor:
    vfs���������ݽṹ����
Version:1.0
Date��2014.5
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
	u8 free;				//0:δʹ�ã�1ʹ��
    u8 temp[2];
	u8 real_file_handle[64];//FSʹ�õ��ļ����
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
	u8 *dev_name;	//ͨ���ļ�ϵͳʱΪ�գ�ר���ļ�ϵͳʱָ���豸��
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
	u8 free;				//0:δʹ�ã�1ʹ��
	fs_adapter	*thefs;		//�������õ�FS
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


/*����ӿ�*/
void init_VFS(int max_file_no);
/*
Dev_nameΪ�豸��������sd0��spi0�����⣬�����Զ����ļ�ϵͳ�����豸�������Զ���;
Opt����ָ���Ƿ��ù̶��ļ�ϵͳȥɨ�裬0Ϊ�Զ���ͨ�õ��ļ�ϵͳȥɨ�裬��0����ָ���ļ�ϵͳɨ�衣
ע0����optָ���ļ�ϵͳ���ݶ�ָ���ļ�ϵͳID;
ע1���˺�������ȥ��ʼ������洢�豸������ڵ���֮ǰ������������豸�ķ���׼�������磬SD����ǩ����ɡ�
*/

#ifdef WIN32
u32 scan_fs( const char *dev_name,u32 opt);
#else
u32 scan_fs( DEV_HANDLE dev_io,u32 opt);
#endif
/*
���ش���ĳ������
partition_indexΪ�洢�豸�ϵķ�����ţ���0��ʼ��

���سɹ�����0���ɹ���drvָ����غ���̷���
ע0:���سɹ��󣬻ᵱĬ��·���л������̸�Ŀ¼��
*/
#ifdef WIN32
FRESULT mount_partition( const char *dev_name , u32 partition_index, char* drv,u32 buffersize);
#else
FRESULT mount_partition( DEV_HANDLE dev_io , u32 partition_index, char* drv,u32 buffersize);
#endif
FRESULT unmount_partition(const char drv);

/*��ʽ��ָ���ķ���*/
#ifdef WIN32
FRESULT mkfs_partition(const char *dev_name, u32 partition_index);
#else
FRESULT mkfs_partition(DEV_HANDLE dev_io, u32 partition_index);
#endif
/*��ʽ��ָ�����̷�*/
FRESULT mkfs_drv(char drv);//B
FRESULT remount(char drv);
/*���д洢�豸�����γ�ʱ���ã��˺������豸����������
dev_nameΪ�豸�����仯���豸��
on_off: true:���� false:�γ�
*/
#ifdef WIN32
void device_on_off(const char *dev_name, bool on_off);
#else
void device_on_off(DEV_HANDLE dev_io, bool on_off);
#endif
u32 get_logic_drvs(char * drvs,int count);
/*����ӿ�---end*/

#ifdef WIN32
/*Ӧ�ýӿ�*/
file_t *vfs_fopen(u8* path ,u32 opt);
u32 vfs_frename(u8* oldpath ,u8* newpath);
u32 vfs_fread(file_t *fp,u8 *ptr,u32 len);
u32 vfs_fwrite(file_t *fp,u8* ptr, u32 len) ;
u32 vfs_fseek(file_t *fp, u32 opt , u32 len);
u32 vfs_ftell(file_t *fp) ;
u32 vfs_fsize(file_t *fp);
void vfs_fclose(file_t *fp) ;
u32 vfs_fdelete(u8* path);
u32 vfs_get_file_length(u8* path);//��ȡ�ļ�����
u8  vfs_get_file_attr(u8* path);//��ȡ�ļ�����
u32 vfs_get_file_name(file_t *fp ,u8 * ptr ,u32 len ,u32 opt);
file_t * vfs_fopen_byindex(char drv , u32 index); //���ݾ��ļ���Ŵ��ļ�
//u32 vfs_get_file_num(char drv , u8* ext, u8* dirbuf, u32 buf_len);//ɨ��ָ���̷���ָ����չ�����ļ�����
u32 vfs_f_emu(u8* path, void *pram2callback, s32 (*callback)(void *fileparam, void *pram2callback));
u32 vfs_f_chdir(u8* path);
u32 vfs_f_mkdir(u8* path, u32 opt);
u32 vfs_f_getdir(u8* buf, u32 len);
u32 vfs_get_free_space(u8 drv);
/*Ӧ�ýӿ�--end*/
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
u32 get_file_length(u8* path);//��ȡ�ļ�����
u8  get_file_attr(u8* path);//��ȡ�ļ�����
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
/*�ļ�ϵͳ�൱�ӿ�*/
u32 reg_fs(fs_adapter* the_fs_adapter); //ע��һ���ļ�ϵͳ�����ɹ�����-1�����򷵻�Ϊ��FS��ID
u32 get_fs_index(fs_adapter* the_fs_adapter); //��ѯ�ļ�ϵͳID��û���򷵻�-1�����򷵻�Ϊ��FS��ID
/*�ļ�ϵͳ�൱�ӿ�*/
#endif
