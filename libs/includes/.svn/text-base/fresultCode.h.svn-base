#ifndef _FRESULT_CODE_
#define _FRESULT_CODE_

/* File function return code (FRESULT) */
typedef enum
{
    FR_OK = 0,
    FR_FIND_DIR,
    FR_FIND_FILE,
    FR_DIR_END,
    FR_NO_FILE,
    FR_NO_PATH,
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

	FR_DISK_ERR,
    FR_FILE_END,
	FR_MOUNT_FAIL,
	FR_DEV_ERROR,
	FR_ERROR_PARTTION,
	FR_NOBUFFER,
	FR_DRV_NOT_EXIST,
	FR_FMT_MOUNTEDPARTITION,
	FR_NO_FUNCTION,
	FR_ERROR_PATH,

	FR_MKDIR_FAIL,
	FR_INT_ERR,
	FR_NOT_ENABLED,
	FR_INVALID_OBJECT,
	FR_EXIST,
	FR_NOT_ENOUGH_CORE,
} FRESULT;

#define N_ROOTDIR	512		/* Number of root dir entries for FAT12/16 */
#define N_FATS		2		/* Number of FAT copies (1 or 2) */
#define	SZ_DIR				32		/* Size of a directory entry */
#define BPB_BytsPerSec		11
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

#define	FSI_LeadSig			0
#define	FSI_StrucSig		484
#define	FSI_Free_Count		488
#define	FSI_Nxt_Free		492


#define	openByDirInfo				0x80
#define	openCreate					0x01
#define	openWrite					0x02
#define	openReadOnly				0x04
#define	openSetToCurDir				0x08


#define	FA_READ				openReadOnly
#define	FA_OPEN_EXISTING	0x00

#define	FA_WRITE			openWrite
#define	FA_CREATE_NEW		openCreate
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define FA__WRITTEN			0x20
#define FA__DIRTY			0x40



#ifdef SEEK_CUR
#undef SEEK_CUR
#undef SEEK_SET 
#undef SEEK_END
#endif
enum SEEKTYPE
{
    SEEK_SET = 0x01,
    SEEK_CUR = 0x02,
	SEEK_END = 0x03
};


/* FAT sub type */
typedef enum
{
    FS_FAT12	=	0x01,
    FS_FAT16	=	0x02,
    FS_FAT32	=	0x04,
    FS_EXFAT	=	0x14,
} FS_TYPE;

#define MIN_FAT16	4086	/* Minimum number of clusters for FAT16 */
#define	MIN_FAT32	65526	/* Minimum number of clusters for FAT32 */

#endif