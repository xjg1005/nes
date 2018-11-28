#ifndef	FF_WRITE_H_
#define	FF_WRITE_H_

#include "typedef.h"
#include "tff.h"
#include "ff_api.h"
#include "dev_Cache.h"
#include "fat_Dir.h"


typedef enum
{
    FERR_NONE,
    FERR_UNWRITEABLE,
    FERR_UNABLE_CREATE,

    FERR_FS_TYPE,
    FERR_READ_END,
    FEERR_WRITE_END,
    FERR_UNKNOW_DEVICE,
    FERR_OPEN_FAILD,
    FERR_CREATE_FAILD,
} FILE_ERR;


typedef struct fat_file
{
    FAT_LAYER_HDL * pFatHdler;					/* f_ChangeDir()函数中传参需要，必须放在首位*/
    u32				fptr;						// File R/W pointer
    u32				fsize;						// File size
    u32				org_clust;					// File start cluster
    u32				curr_clust;					// Current cluster
    u32				curr_sect;					// Current sector
    u32				open_mode;
	u32				rsize;						//文件真正的长度
    CLUST_RANG		clust_rang;					/* 文件连续簇的范围							*/
    DIR				entryDir;
    u8				flag;
	u8				fattrib;					//属性
} FILE_HANDLE;


#define f_InitFileHdler(fileHdl , devHdl)			do{fileHdl.pFatHdler = &devHdl;}while(0)

#define	GOTO_ROOT_DIR(dir)			dir.sclust=0

#define	FAT_SECT_SIZE		512

u32  f_CaculateLen(FILE_HANDLE   *pFile ,u32 btr);

tbool f_UpdataFileSize(FILE_HANDLE   *pFile);

tbool f_IsDevWriteable(FILE_HANDLE   *pFile);

tbool f_PickOutName(u8 **pPath , u8 *rName);

tbool    f_Open(FILE_HANDLE  *pFile, FAT_CREATE   *pFatCreate, u8  *path, u32 openMode);
u32		f_Emu(FILE_HANDLE   *pFile, FAT_CREATE   *pFatCreate, u8* path, void *pram2callback, s32 (*callback)(void *fileparam, void *pram2callback));
u32		fget_free_space(FAT_LAYER_HDL   *pFatHdler);
u32     f_Write(FILE_HANDLE  *pFile, void  *pBuf, u32 len);

void    f_Close(FILE_HANDLE  *pFile);

tbool    f_Seek (FILE_HANDLE   * pFile, u32 type, u32 offsize);

u32     f_Read(FILE_HANDLE   * pFile ,void   *pBuf , u32 btr);



u32     f_GetName(FILE_HANDLE   *pFile , void   *pBuf , u32 bufLen);


tbool    f_Delete(FILE_HANDLE   *pFile);

tbool    f_GetFileInfo(FILE_HANDLE   *pFile, DIRINFO   * pDirInfo);

#endif




























