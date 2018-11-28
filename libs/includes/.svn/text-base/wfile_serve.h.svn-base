#ifndef __WFILE_SERVE__
#define __WFILE_SERVE__


#include "typedef.h"
#include "msg.h"
//#include "circular_buf.h"
#include "frame_buf.h"
#include "os_api.h"

#define WFS_MAX_FILE   4

typedef struct _wfsv_info
{
    u32 (*fs_write)(void *pFile, void *pBuf, u32 len);
    u32 wfsv_task_prio;
}wfsv_info_t ;

#define WFSV_STATUS_UNINIT     0x00
#define WFSV_STATUS_OPEN       0x01
#define WFSV_STATUS_STOPING    0x02
#define WFSV_STATUS_STOP       0x03
#define WFSV_STATUS_CLOSEING   0x04
#define WFSV_STATUS_CLOSE      0x05
#define WFSV_STATUS_RUNING     0x06

typedef struct _wfsv
{
    void * fp;
    fb_pipe_t *pipe;
    //void * cb_ptr;
    void * sem;
   // msg_t * msg_d;
   // u32 count;
    volatile u32 total;
    volatile u32 status;
    u8 ch ;
   // volatile u32 msg_cnt;
   // volatile u32 err_flag;

}wfsv_t ;

typedef struct _wfsv_close_t
{
    wfsv_t * wfsv_fd[WFS_MAX_FILE];
    u8 is_close[WFS_MAX_FILE];
}wfsv_close_t;

u32 wfsv_init(wfsv_info_t * wfsv_info);
u32 wfsv_uninit();
wfsv_t * wfsv_open(fb_pipe_t *pipe);
u32 wfsv_close(wfsv_t * wfsv);
u32 wfsv_start(wfsv_t * wfsv ,void *fp) ;
u32 wfsv_stop(wfsv_t * wfsv);
u32 wfsv_get_pos(wfsv_t * wfsv);
u32 wfsv_write(wfsv_t * wfsv, fbuf_t *fb);
u32 wfsv_get_wres(wfsv_t * wfsv);
void *wfsv_get_fp(wfsv_t * wfsv);
u32 wfsv_clr_fb(wfsv_t * wfsv);


















#endif





