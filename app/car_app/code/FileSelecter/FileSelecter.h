

/*************************************************************
File: FileSelecter.h
Author:xuhuajun
Discriptor:
    文件选择器函数及数据结构定义
Version:1.0
Date：2014.8
*************************************************************/

#ifndef _FILE_SELECTER_
#define _FILE_SELECTER_
#include "vfs.h"
#ifdef WIN32
#include "stdio.h"
#endif

typedef struct
{
	u32	fsize;
	u8	fattrib;//属性
	u8  fname[15];
	u32 flname_len;
	u8  flname[256];
}FILE_PARAM;


typedef struct
{
    s32 foladerA_endFile_index;
    s32 foladerA_startFile_index;
    s32 foladerB_endFile_index;
    s32 foladerB_startFile_index;
    s32 cur_findex_A;
    s32 cur_findex_B;
    u16 foladerB_file_num;
    u16 foladerA_file_num;

    u16 fldA_pro_fnum;
    u16 fldB_pro_fnum;

    u8 current_folder;
    u8 current_flie_err;
    u8 no_file;
    u8 file_protect;
    u8 ftype;
    u8 cur_dec_folder;        //指示当前正在解码的是A文件夹的视频还是B文件夹的视频
    u8 search_flag;
} pb_fctrl_t;



extern u8 video_path[125];
extern char root_dir[13];
extern char root_dir1[13];
extern s32 current_index;
extern s32 current_index1;
extern pb_fctrl_t pb_fctrl;
extern void * last_file_hdl ;
extern u32 bdisk_ready;
extern u32 g_need_format;

u32 Load_vfs(int max_open_file);
void set_file_format_path(char *rootdir,char*rec_format,char *sos_format,char *pic_format,char *rootdir1, char *rec_format1,char *sos_format1);
BOOL file_sel_msg_ctrl(u32 msg, void *parm);
BOOL init_record_dir();
file_t *create_video_file();
file_t *create_pic_file();
BOOL protect_current_file(u8 type);
u32 lock_current_file( void );
u32 unlock_current_file( void );
void dec_video_index_to_name(u32 index, char *video_format_to_decode,const u8 *video_dir);

BOOL freeSpace(u32 need_space, u32 flag);
u8 check_all_files(void);//检查是否有文件
s32 dec_femu_cb_for_A(void *fileparam, void *pram2callback);
u32 is_sd_online();
u32 is_phy_sd_online();

u8 wait_sd_ok(void);

u16 file_mutex_init();


void get_folader_status(void);
void * select_file(u8 search_mode);
s32 dec_del_file();
u32 dec_file_pro();


extern file_t *create_video_file_mov();//mov
extern file_t *create_pic_file_data_camera();
extern file_t *create_pic_file_analog_camera();
extern file_t *create_video_file_avi();

s32 enc_femu_cb_for_A(void *fileparam, void *pram2callback);
s32 enc_femu_cb_for_B(void *fileparam, void *pram2callback);

#endif
