#ifndef _UBOOT_FS_
#define _UBOOT_FS_
#include "typedef.h"

#define _xdata

#define MAX_DEEPTH		1				/* 0~9 deepth of system */

#define LFN_MAX_SIZE    512   //不能超过512
typedef u32	 CLUST;

#define FS_WRITE_EN             /* 是否允许写文件 */
#ifdef  FS_WRITE_EN
#define FAT12_WRITE_EN
//#define EXFAT_WRITE_EN
#endif
#define WIN_DATA_DIRTY  0x08
/* File system API info */
typedef struct _FSAPIMSG
{
    u16	    musicdir_counter;    // 包含指定文件的文件夹序号
    u16     dir_counter;			// 文件夹序号
    u16     file_total_indir;     // 当前目录的根下有效文件的个数
    u16     file_total_outdir;	// 当前目录前的文件总数，目录循环模式下，需要用它来计算文件序号
    u16	    file_number;			// 当前文件序号
    u16     file_counter;		//用于搜索文件计数
    u8	    deepth;				// dir deepth for search
} FSAPIMSG;

typedef struct _SWIN_BUF
{
    u8  start[512];
    u32  sector;
    struct _FATFS  *fs;
    u8   flag;
} SWIN_BUF;
/*
#define FS_WIN_START  fs->win->start
#define FS_WIN_SECTOR fs->win->sector
#define FS_WIN_FLAG   fs->win->flag
*/
#define FS_WIN_START  fs->win.start
#define FS_WIN_SECTOR fs->win.sector
#define FS_WIN_FLAG   fs->win.flag

/* File system object structure */
struct _FATFS
{
    //SWIN_BUF *win;		        /* Disk access window for Directory/FAT/File */
    u32		fatbase;		/* FAT start sector */
    u32		dirbase;		/* Root directory start sector */
    u32		database;		/* Data start sector */
    u32 	n_fatent;		/* Maximum cluster# + 1 */
#ifdef FS_WRITE_EN
    u8      write_en;       //文件系统是否允许写操作：只允许FAT16，FAT32并且sector size只能是512bytes
    u8	    fsi_flag;		/* fsinfo dirty flag (1:must be written back) */
    u32	    last_clust;		/* Last allocated cluster */
    u32	    fsi_sector;		/* fsinfo sector (FAT32) */
#endif
    u32     boot_sect;
    u16		n_rootdir;		/* Number of root directory entries */
    u16     total_file;		/* 当前设备的匹配文件总数*/
    u8		s_size;			/* sector size, 2 power n*/
    u8		csize;			/* Number of sectors per cluster,2 power n */
    u8		fs_type;		/* FAT sub type */
    u8		secotr_512size;	/* size of per sector */
    u32 (*disk_read)(u8 _xdata * buf, u32 lba);		/* device read function */
    u32 (*disk_write)(u8 _xdata * buf, u32 lba);		/* device write function */
    u32     fat_time;
//    u8      *lfn;           //长文件名buffer
//	u16     lfn_cnt;
//    char    *tpath;       //路径名称buffer
    void *hdev;
    SWIN_BUF win;		        /* Disk access window for Directory/FAT/File */
    u8 drive_cnt;
    //FSAPIMSG *fs_msg;
};
typedef struct _FATFS FATFS;


struct _FS_NAME
{
    char    tpath[128];       //路径名称buffer
    char    *lfn;             //长文件名buffer
    u16     lfn_cnt;

};
typedef struct _FS_NAME FS_NAME;


/* Directory object structure */
typedef struct _DIR
{
    u32 	clust;		        /* Current cluster */
    u32		csect;		        /* Current sector */
    u32		sclust;		        /* Start cluster */
    u16		cindex;		        /* Current index */
    //u16     lfn_index;		/* 长文件名index号 */
} DIR;

/* DIR status structure */
typedef struct _DIRINFO
{
    DIR     dj;
    u32 	sclust;			/* File start cluster */
    u32		fsize;			/* Size */
    u8		fattrib;		/* Attribute */
    char    fname[12];          /* */
} DIRINFO;



typedef struct _FIL0
{
    u32		fptr;				// File R/W pointer
    //u32		fsize;				// File size
    u8      flag;
    SWIN_BUF data_win;
    u32		csect;			// Current sector
    u32 	clust;			// Current cluster
    //u32 	sclust;			// File start cluster
#ifdef FS_WRITE_EN
    SWIN_BUF dir_win;
    u8       dir_duty;
#endif
    u32     start_clust;
    u32     end_clust;
    FATFS   *fs;
} FIL0;

typedef struct _FIL
{
    FIL0 fil;
    DIRINFO dir_info;             //文件的目录项信息
    FSAPIMSG fs_msg;
    //u8 filename[512];///<[1][512];
    //u8 path_name[128];///<128
    DIR f_dj[MAX_DEEPTH];
    FS_NAME fs_n;
} FIL;

typedef struct _LDIR_INFO
{
    u8 ldir_ord;
    u8 ldir_name1[10];             //文件的目录项信息
    u8 ldir_attr0;
    u8 ldir_type0;
    u8 ldir_chksum0;
    u8 ldir_name2[12];
    u16 ldir_fst_clus_lo;
    u8 ldir_name3[4];
} LDIR_INFO;


enum
{
    SEEK_SET = 0x01,
    SEEK_CUR = 0x02
};

#define	openCreate					0x01
#define	openWrite					0x02
#define	openReadOnly				0x04

typedef enum
{
    FR_OK = 0,
    FR_FIND_DIR = 0x80,
    FR_FIND_FILE,
    FR_DIR_END,         //前面几个位置不能变
    FR_NO_FILE,
    FR_NO_PATH,
    FR_EXIST,
    FR_INVALID_NAME,
    FR_INVALID_DRIVE,
    FR_DENIED,
    FR_RW_ERROR,
    FR_WRITE_PROTECTED,
    FR_NO_FILESYSTEM,
    FR_DEEP_LIMIT,
    FR_END_PATH,
    FR_FILE_LIMIT,
    FR_END_FILE,
    FR_LFN_ERR,
    FR_MKFS_ABORTED,
    FR_DIR_DELETE,
    FR_DISK_ERROR,
    FR_FILE_END,
    FR_FILE_ERR,
    FR_NO_WINBUF,
    FR_INT_ERR,				/* (2) Assertion failed */
    FR_NO_SEL_DRIVE,
} FRESULT;

u32 fs_dev_mount(void *arg);
FIL *fopen(u8 *file_path, u8 mode);
u16 fseek(FIL *fp, u8 type, u32 offset);
u32 fread(FIL *fp, u8 *buf, u32 len);
#endif /* _FATFS */
