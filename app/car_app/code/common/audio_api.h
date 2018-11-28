
#ifndef __AUDIO_API_H__
#define __AUDIO_API_H__

#include "config.h"
#include "audio.h"
#include "avi_unpkg_if.h"
#include "sdfile.h"
#include "circular_buf.h"


#define KEY_TONE_EN
#define TAKE_PHOTO_VOICE
#define PLAY_WAVFILE_VOICE


#define DIFF_SIGNAL_EN      1//是否使能差分输出

typedef struct _dac_api_t
{
    void * fbmng;
    void * dac_pipe;
    fbuf_t *dac_fb;
    u32 offset;
    AVI_UNPKG_FD_T * avi_fd;
    volatile u32 need_play;

    u32 ktone_len;
    u32 ktone_offset;
    u32 ktone_flag;
    u8  ktone_enable;

    u32 ptone_len;
    u32 ptone_offset;
    u32 ptone_flag;
    u8  ptone_enable;

    u32 stone_len;
    u32 stone_offset;
    u32 stone_flag;
    u8  stone_enable;

    sdfile_t ssd_file;
    cbuffer_t ssd_cbuf;
    u32 ssd_offset;

}dac_api_t;



typedef struct _ladc_api_t
{
    void * fbmng;
    void * ladc_pipe;
    void * ladc_fb;
    u32 offset;

}ladc_api_t;


extern dac_api_t g_dac_ctrl;
extern ladc_api_t g_ladc_ctrl;

u32 dac_fbuf_init();
u32 ladc_fbuf_init();

int dac_api_func(void *priv,u8 *buf , u32 len);
int ladc_api_func(void *priv,void *buf , u32 len);

void test_audio(void * parm);
void dac_api_reset_dec(void * fd_hdl);
void dac_api_clr_fb();
void dac_api_set_play(u8 need_play);
u32 dac_api_ktone_len();
void dac_api_play_ktone();
void dac_api_stop_ktone();
void dac_api_enable_ktone(u8 mark);
u32 dac_api_init();
u32 dac_api_open();
u32 dac_api_close();


u32 dac_api_ptone_len();
void dac_api_play_ptone();
void dac_api_stop_ptone();
void dac_api_enable_ptone(u8 mark);

u32 dac_api_stone_len();
void dac_api_play_stone();
void dac_api_play_ctone();
#endif

