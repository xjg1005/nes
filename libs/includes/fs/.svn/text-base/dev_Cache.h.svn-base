#ifndef FATCACHE_DRIVER_H
#define	FATCACHE_DRIVER_H
//#include "config.h"
#include "typedef.h"
#include "fs_err.h"


#ifdef __cplusplus
extern "C" {
#endif


#define	DEV_CACHE_DIRTY		0x01
#define	DEV_CACHE_LOCK		0x02


    typedef u32 ( *pEnterDevRWHook)(void);
    typedef void( *pLeaveDevRWHook)(u32 );
    typedef u32 ( * PFUN_DEV_RW)(void   * , void * , u32 , u32 );

    typedef struct
    {
        u8  buf[512];
        u32 sector;
        u8  states;
        u8 rec[32-5];
    } CACHE_INFO  ;


    typedef struct
    {
        CACHE_INFO   *pCaches;
        PFUN_DEV_RW  pFun_DevRead;
        PFUN_DEV_RW  pFun_DevWrite;
        u8			cacheNum;
		void * devHandle;
		u32		segment;
		u8 alig[8];
    } DEV_CACHE;


    typedef enum
    {
        ALLOC_RW,				/*读写方式分配*/
        ALLOC_RO,				/*只读方式分配*/
        ALLOC_NRW,				/*非读写方式分配*/
    } CACHE_ALLOC_METHOD;

extern pEnterDevRWHook   g_pFunEnterRW;
extern pLeaveDevRWHook   g_pFunLeaveRW;

#define ASSERT(a)				if(!(a)){return FALSE;}


#define dev_CacheDirty(pCache)				(pCache)->states |= DEV_CACHE_DIRTY

#define dev_CacheSect(pCache , sect)		(pCache)->sector = sect



    /*-------------------释放申请的缓存-----------------------------
    	*pCache	:	dev_CacheAlloc()的返回值
    *---------------------------------------------------------------*/
#define dev_CacheFree(pCache)				(pCache)->states &= ~DEV_CACHE_LOCK


    /*----------------------------初始化设备缓存----------------------------

    	*pFun_DevR	:	读设备函数指针
    	*pFun_DevW	:	写设备函数指针
    	*pCaches	:	cache起始地址
    	*num		:	cache个数
    *------------------------------------------------------------------------*/
    void dev_CacheInit(DEV_CACHE   *pDevCache, PFUN_DEV_RW pFun_DevR ,  PFUN_DEV_RW pFun_DevW ,CACHE_INFO   *pCaches , u32 num);



    void dev_HookInit(pEnterDevRWHook pEnterRW, pLeaveDevRWHook pLeaveRW);

    /*--------------------------------申请缓存-----------------------------

    	*method:
    			ALLOC_RW:	读写方式申请，cache的DIRTY标志默认被置位
    			ALLOC_RO:	只读方式申请
    			ALLOC_NRW:	非读写方式分配,只分配一块缓存

    	sector：需要读取的sector

    	return:	成功：返回512byte的cache地址，此地址可强制转换为(CACHE_INFO *)
    			失败：返回NULL

    	other:	申请的缓存用完后请及时通过dev_CacheFree()释放
    *--------------------------------------------------------------------------*/
    CACHE_INFO   *dev_CacheAlloc(DEV_CACHE  *pDevCache, CACHE_ALLOC_METHOD method , u32 sector);


    /*
    	*刷新缓存中的内容到设备，pCacheInfo等于0时，刷新所有缓存
    */
    tbool dev_CacheFlush(DEV_CACHE   *pDevCache, CACHE_INFO *pCacheInfo);


    /*
    	*写buf中的内容到地址addr，大小为512
    */

    tbool dev_CacheWDev(DEV_CACHE   *pDevCache, u32 addr , void   *buf,u32 count);


    /*
    	*读地址addr中的内容到buf，大小为512
    */
    tbool dev_CacheRDev(DEV_CACHE   *pDevCache, u32 addr , CACHE_INFO *pCache);


    tbool dev_CacheRDevOnly(DEV_CACHE *pDevCache, u32 addr , void *buf);




#ifdef __cplusplus
}
#endif


#endif



