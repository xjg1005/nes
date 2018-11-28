#ifndef _FATFS
#define _FATFS

#include "typedef.h"
#include "dev_Cache.h"
#include "fresultcode.h"
typedef enum
{
    FAT_ERR_NONE,
    FAT_ERR_CLUST_OUT_RANG,
} FAT_ERR;



#define FS_WRITE_EN             /* 是否允许写文件 */

#define DISK_USED_SPACE

#define DIR_RECORD_EN		0


typedef u32	 CLUST;


/* Directory object structure */
typedef struct _DIR
{
    u32 	clust;		        /* Current cluster */
    u32		csect;		        /* Current sector */
    u32		sclust;		        /* Start cluster */
    u16		cindex;		        /* Current index */
} DIR;

typedef u8 (*disk_read)(void   * buf, u16 lba , tu16);



/* DIR status structure */
typedef struct _DIRINFO
{
	DIR dj;				// 存放长文件名入口信息
	u32 psclust;		// 父目录簇号
	u32 sclust;			/* File start cluster */
	u32	fsize;			/* Size */
	u8	fattrib;		/* Attribute */
	u8  fname[15];      /* 为了使exFAT更好计算,长度取15Bytes*/
} DIRINFO;

/* File system API info */
typedef struct _FSAPIMSG
{
    u16     fileCounter;
    u16	    fileNumber;			// 当前文件序号
    u16	    musicDirCounter;    // 包含指定文件的文件夹序号
    u16     dirCounter;			// 文件夹序号
    u16     fileTotalInDir;     // 当前目录的根下有效文件的个数
    u16     fileTotalOutDir;	// 当前目录前的文件总数，目录循环模式下，需要用它来计算文件序号
#if DIR_RECORD_EN
    u16		lastFileNumber;
#endif
    u8	    deepth;				// dir deepth for search
} FSAPIMSG;

typedef struct
{
    u32			fatbase;					/* FAT start sector */
    u32			dirbase;					/* Root directory start sector */
    u32			database;					/* Data start sector */
    u32			max_clust;					/* Maximum cluster# + 1 */
//	u32			Reserved_sector;
#ifdef DISK_USED_SPACE
    u32			used_space; 				/* Disk used space */
#endif
    u32			curDir;
    DEV_CACHE   *pDevCache;

    u32			curFatSect;					//FAT表位置，这两项顺序不可变
    u16			sectOffset;

    u16			n_rootdir;					/* Number of root directory entries */
    u16			fsInfo_sector;
    u8			csize;						/* Number of sectors per cluster,2 power n */
    u8			secotr_512size;				/* size of per sector */
    u8          clustSize;
    FS_TYPE		fs_type;					/* FAT sub type */
//    FAT_ERR		errFlag;
} FAT_LAYER_HDL;


typedef struct
{
    u32 start_clust;
    u32 end_clust;
} CLUST_RANG;


typedef struct _FIL_BACKUP
{
    u32		fptr;				// File R/W pointer
    u32 	curr_clust;			// Current cluster
    u32		curr_sect;			// Current sector
    u8      flag;
} FIL_BACKUP;


typedef enum
{
    FILE0 = 1,
    FILE1,
    FILE2,
    FILE3
} FILES;

#include "fresultcode.h"
/* File function return code (FRESULT) */

//extern u8 g_FatIdn;
extern FAT_LAYER_HDL   *g_pFatHdler;

#define BACK_UP_FAT_HDLER(pFatHdler)	FAT_LAYER_HDL   *old_pFatHdler = g_pFatHdler; g_pFatHdler = pFatHdler

#define RESTORE_FAT_HDLER()				g_pFatHdler = old_pFatHdler


/*-----------------------------------------------------*/
/* Tiny-FatFs module application interface             */

tbool fat_IsValClust(u32 clust);


void fat_u32Copy(void *pDest , void * pSrc , u32 copyLen);


u32 clust2sect (u32 clust);

u32 fat_GetCluster(CLUST_RANG   *pRang , u32 clust);

//tbool  move_window (u32 sector);
FRESULT  next_dir_entry (DIR   *dj , CACHE_INFO   *pCache);

FRESULT f_readnextdir (DIR  * dj, DIRINFO  * dinfo, CACHE_INFO   *pCache);

void  f_opendir( DIR  * dj) ;
FRESULT  f_mount(u32 bootsect) ;
//void f_parse_fat_lfn(DIRINFO  * dinfo);
//FRESULT  f_open(FIL   * fat_file, DIRINFO   * finfo);
FRESULT check_fs (u32 sec);
void f_set_sector_size(u32);
//void f_init(FATFS   *pFatFs, u8   * buf, disk_read io_read, u8 sector_sizeof512);

u32 get_cluster (CLUST_RANG   * pClustRang,  CACHE_INFO   *pCache, u32 clust);

void get_dir_info (u8   *dir,DIRINFO   *dinfo);


u32 get_one_count(u32 tt);


/* FAT sub-type boundaries */
/* Note that the FAT spec by Microsoft says 4085 but Windows works with 4087! */
#define MIN_FAT16	4086	/* Minimum number of clusters for FAT16 */
#define	MIN_FAT32	65526	/* Minimum number of clusters for FAT32 */




//#define FS_FAT12	0x01
//#define FS_FAT16	0x02
//#define FS_FAT32	0x04
//#define FS_EXFAT	0x14


/* File attribute bits for directory entry */

#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define	AM_VOL	0x08	/* Volume label */
#define AM_LFN	0x0F	/* LFN entry */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */
#define AM_FCH	0x80    /* exFAT下,文件簇连续标志 */



/* Offset of FAT structure members */

#define BS_jmpBoot			0
#define BS_OEMName			3
#define BPB_BytsPerSec_l	11
#define BPB_BytsPerSec_h	12
#define BPB_SecPerClus		13
#define BPB_RsvdSecCnt		14
#define BPB_NumFATs			16
#define BPB_RootEntCnt		17
#define BPB_TotSec16		19
#define BPB_Media			21
#define BPB_FATSz16			22
#define BPB_SecPerTrk		24
#define BPB_NumHeads		26
#define BPB_HiddSec			28
#define BPB_TotSec32		32
#define BS_55AA				510

#define BS_DrvNum			36
#define BS_BootSig			38
#define BS_VolID			39
#define BS_VolLab			43
#define BS_FilSysType		54

#define BPB_FATSz32			36
#define BPB_ExtFlags		40
#define BPB_FSVer			42
#define BPB_RootClus		44
#define BPB_FSInfo			48
#define BPB_BkBootSec		50
#define BS_DrvNum32			64
#define BS_BootSig32		66
#define BS_VolID32			67
#define BS_VolLab32			71
#define BS_FilSysType32		82
#define BS_FileSysTypeexFAT				5
#define BPB_FatOffset					80
#define BPB_FatLength					84
#define BPB_ClusterHeapOffset			88
#define BPB_ClusterCount				92
#define BPB_FirstClusterOfRootDirectory	96
#define BPB_VolumeFlags					106
#define BPB_BytesPerSectorShift			108
#define BPB_SectorsPerClusterShift		109
#define BPB_NumberOfFats				110
#define MBR_Table			446

///for FAT12/FAT16/FAT32
#define	DIR_Name			0
#define	DIR_Attr			11
#define	DIR_NTres			12
#define DIR_Checksum		13
#define	DIR_CrtTime			14
#define	DIR_CrtDate			16
#define	DIR_FstClusHI		20
#define	DIR_WrtTime			22
#define	DIR_WrtDate			24
#define	DIR_FstClusLO		26
#define	DIR_FileSize		28
////for exFAT
#define DIR_FileChanFlags	1
#define DIR_NameLen		    3
#define DIR_AttrexFAT		4
#define DIR_FileSizeexFAT   8
#define DIR_FstClustexFAT	20


#endif /* _FATFS */

/*
16进制值	最大容量	说明
00h		未用
01h	2 Mb	FAT12
02h		XENIX Root
03h		XENIX User
04h	32 Mb	FAT16 （CHS 寻址模式，分区最大为 32 Mb）
05h	2 Gb	扩展分区（CHS 寻址模式）
06h	2 Gb	FAT16 （CHS 寻址模式，分区大小大于 32 Mb)
07h	16 Eb	NTFS
08h		AIX
09h		AIX Bootable
0Ah		Bootmanager
0Bh	2 Gb	FAT32 （CHS 寻址模式）
0Ch	2 Tb	FAT32 （LBA 寻址模式）
0Eh	2 Gb	FAT16 （LBA address mode）
0Fh	2 Tb	扩展分区 （LBA 寻址模式）
10h		OPUS
12h		CPQ Diagnostics
14h	2^89 bytes	Omega FS （用于 Maverick OS）
15h		Swap 分区
16h		Hidden big FAT16 （非正式）
24h		NEC MS-DOS 3.x
40h		VENIX 80286
42h		SFS
50h		Disk Manager
51h		Novell
52h		CP/M
56h		GoldenBow
61h		SpeedStor
63h		Unix SysV386
64h		Novell Netware
65h		Novell Netware
75h		PC/IX
80h		Old MINIX
81h		Linux / MINIX
82h		Linux Swap
83h		Linux Nature
93h		Amoeba
94h		Amoeba BBT
A5h		BSD/386
B7h		BSDI file system
B8h		BSDI swap
C7h		Cyrnix
DBh		CP/M
E1h		DOS Access
E3h		DOS R/O
F2h		DOS Secondary
F4h		SpeedStor
FEh		LANstep
FFh		BBT

*/
