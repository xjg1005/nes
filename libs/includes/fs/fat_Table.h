#ifndef	FAT_TABLE_API_H
#define	FAT_TABLE_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "typedef.h"
#include "tff.h"
#include "dev_Cache.h"


    typedef struct
    {
        u32 sector;
        u16 index;
    } FAT_CLUST_POS;





    tbool fatTable_GetClustPos(FAT_CLUST_POS   *pFatSect , u32 clust);

    tbool fatTable_Write(u32 prevClust , u32 newClust);

    tbool fatTable_ClearClustChain(u32 orgClust, tbool isInclude);

    u32 fatTable_AllocClust(u32 prevClust , u32  * fillCnt);




#ifdef __cplusplus
}
#endif

#endif

