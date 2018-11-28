#include "config.h"

volatile u32 delay_cnt;



void delay(unsigned int  t)
{
	while(t--)
	{
		t = t ;
		asm("nop");
	}
}

void delay_2ms(u32 cnt)
{
	delay_cnt = cnt;
	while(delay_cnt);
}

/*----------------------------------------------------------------------------*/
/**@brief   ?ú′???±′
   @param   void
   @return  void
   @author  Change.tsai
   @note    void my_memcpy(void * dst, void * src, u16 len)
*/
/*----------------------------------------------------------------------------*/
void my_memcpy(void * dst, const void * src, u16 len)
{
	if((src == dst) || (len == 0))
		return;
	else if((src > dst) || (src + len) < dst)
	{
		while(len)
		{
			*(u8*)dst++ = *(u8*)src++;
			--len;
		}
	}
	else
	{
		while(len)
		{
			--len;
			*(u8*)(dst + len) = *(u8*)(src + len);
		}
	}
}

char *my_strcpy(char *dst, const char *src)
{
//    assert(dst != NULL);
//    assert(src != NULL);
	char *ret = dst;
	my_memcpy(dst, src, strlen(src) + 1);
	return ret;
}

/*----------------------------------------------------------------------------*/
/**@brief   ?ú′?éè??
   @param   void
   @return  void
   @author  Change.tsai
   @note    void my_memset(void * dst, u8 dat, u16 len)
*/
/*----------------------------------------------------------------------------*/
void my_memset(void * dst, u8 dat, u16 len)
{
	while(len)
	{
		*(u8*)dst++ = dat;
		--len;
	}
}

void my_memzero(u32 * dst, u16 len)
{
	while(len)
	{
		*(u32*)dst++ = 0;
		len -= 4;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief   ?ú′?éè??
   @param   void
   @return  void
   @author  Change.tsai
   @note    void my_memcmp(void * dst, u8 dat, u16 len)
*/
/*----------------------------------------------------------------------------*/
bool my_memcmp(void * dst, void *src, u16 len)
{
	while(len)
	{
		if((*(u8*)dst++) != (*(u8*)src++))
		{
			return 0;
		}
		--len;
	}
	return 1;
}


u16 CRC16(void *ptr, u32  len)
{
    u8 *p = (u8 *)ptr;

    CRC_REG = 0 ;

    while(len--)
    {
        CRC_FIFO = *p++;
    }

    return CRC_REG;
}






void com_ASCII_ToUpper(void *buf, u32 len)
{
    while (len--)
    {
        if ((((u8 *)buf)[len] >= 'a') && (((u8 *)buf)[len] <= 'z'))
        {
            ((u8 *)buf)[len] -= 'a' - 'A';
        }
    }
}

u32 com_ASCII_StrCmp(const u8 *src, const u8 *dst, u32 len)
{
    u32 m = len; 			//用于处理文件名中'*'

    while (len--)
    {
        if ((*src == '\0') && (*dst== '\0') )				//同时结束
        {
            return 0;
        }
        if ((*src == '\0') || (*dst== '\0'))	//有一方提前结束
        {
            return m-len;
        }

        if (*src != *dst)							//比较不相等
        {
            if ((*dst != '?') && (*src != '?'))
            {
                return m-len;
            }
        }

        src++;
        dst++;
    }

    return 0;

}

u32 com_ASCII_StrLen(void *str , u32 len)
{
    u32 i;

    for(i=0 ; i< len ; i++)
    {
        if(((u8   *)str)[i] ==0)
        {
            break;
        }
    }

    return i;
}












