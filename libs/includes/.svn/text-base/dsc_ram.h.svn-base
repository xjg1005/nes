#ifndef __DSC_RAM_H__
#define __DSC_RAM_H__

#include "config.h"






#define DSC_STATUS      u8

struct DSC_ADDR
{
    u8 *y_addr;
	u8 *u_addr;
	u8 *v_addr;
	u8 state;//1 地址可以拿去显示，0 地址已经显示完
};

typedef void (*suspend_fun)(void *priv, struct DSC_ADDR *dsc_addr);


typedef struct _DSC_YUV_INF
{
    u8 *y_addr;
    u8 dsc_buf_num;//需要分配多少个这样的BUF
    u32 y_len;
    u32 u_len;
    u32 v_len;

    suspend_fun suspending;

}DSC_YUV_INF;


struct dsc_ram
{
    //struct DSC_ADDR dsc_addr[DSC_BUF_NUM];
    struct DSC_ADDR * dsc_addr;
	DSC_STATUS read_status;//读的状态机
	DSC_STATUS write_status;//写的状态机
    suspend_fun suspending;

    suspend_fun global_pending;

    u8 max_dsc_num; //显示buf总数
    u8 is_read_stop_en;//判断读是否可以停止

};


#define ENTER_DSC_RAM_CRITICAL()            OS_ENTER_CRITICAL()//DISABLE_INT()
#define LEAVE_DSC_RAM_CRITICAL()            OS_EXIT_CRITICAL()//ENABLE_INT()

struct dsc_ram * dsc_ram_init( DSC_YUV_INF * dsc_yuv_info);
//int dsc_ram_init(struct dsc_ram *dsc_info, DSC_YUV_INF * dsc_yuv_info);
struct DSC_ADDR * dsc_ram_get_freebuf(struct dsc_ram *dsc_info);
struct DSC_ADDR *  dsc_ram_set_buf_full(struct dsc_ram *dsc_info);
struct DSC_ADDR * dsc_ram_get_full_buf(struct dsc_ram *dsc_info);
struct DSC_ADDR *  dsc_ram_set_free_buf(struct dsc_ram *dsc_info);
void dsc_ram_syn_rw(struct dsc_ram *dsc_info);

void dsc_ram_free(struct dsc_ram * dsc_info);
void dsc_ram_set_rstop(struct dsc_ram * dsc_info);

#endif



