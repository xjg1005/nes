#ifndef __DISP_FLASH_H__
#define __DISP_FLASH_H__

#include "typedef.h"

//#define VIDEO_TASK_PRIO     20
//#define VIDEO_TASK_N        "video_task"

u8   video_init(u8 *filename,u8 *disp_buf);
void video_wait_play_end();
void video_close();
u8   video_get_audio_sample();
u8   video_get_audio_number();
bool video_task_create(u8 prio,s8 *task_name);

#endif
