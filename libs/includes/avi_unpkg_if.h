#ifndef __AVI_UNPKG_IF_H__
#define __AVI_UNPKG_IF_H__


#include "typedef.h"
#include "frame_buf.h"
#include "circular_buf.h"

#include "os_api.h"


#ifdef WIN32

#define TEST_SAVE_BITES		1

#endif


#ifdef WIN32
#define AVI_UNPKG_SEEK_CUR    1
#define AVI_UNPKG_SEEK_END    2
#define AVI_UNPKG_SEEK_SET    0
#else
#define AVI_UNPKG_SEEK_SET    1
#define AVI_UNPKG_SEEK_CUR    2
#endif

#define AVI_UNPKG_FF		0x01		//快进
#define AVI_UNPKG_FR		0x02		//快退



typedef struct __if_avi_unpkg_ops
{
	char *name ;
	s32 (*init)();
	s32 (*open)(void *priv);
	s32 (*close)(void *priv);
	s32 (*read)(void *priv ,u8*buf, u32 addr , u32 len);
	s32 (*write)(void *priv ,u8*buf , u32 addr , u32 len);
	s32 (*ioctrl)(void *priv ,void* parm, u32 cmd );
}if_avi_unpkg_ops_t ;



typedef struct _AVI_UNPKG_DEV_T
{
	//与文件操作相关的部分
	void * pf;
	u32 (*fs_write)(void *pFile, void *pBuf, u32 len);
	u32 (*fs_read)(void * pFile ,void *pBuf, u32 btr);
	u32 (*fs_seek)(void * pFile, u32 type, u32 offsize);
	u32 (*fs_tell)(void * pFile);//获取封装文件当前位置
    void (*fs_close)(void  *pFile);

	u8 * fs_buf;
	u32 pos;
	u32 last_read_size;

}AVI_UNPKG_DEV_T;


typedef struct _avi_unpkg_info_t
{
	u32 file_size;
	u32 avih_dwFlags;
	u32 stream_num;
	u32 total_frames;
	u32 vid_fps;
	u32 width;
	u32 heigh;

	u32 real_width;
	u32 real_heigh;

	u32 aud_type;
	u32 channel_num;
	u32 samp_rate;
	u32 bits_per_samp;//每秒/byte = (采样率*通道数*位深/8 )
	u32 aud_support;

	u32 movi_addr;
	u32 movi_len;
	u32 idx1_addr;
	u32 idx1_len;
	u32 offset_from;//相对文件开头还是相对MOVI的偏移

}avi_unpkg_info_t;


typedef struct _aviunpkg_index_entry {
	u32   dwChunkId;   // 表征本数据块的四字符码
	u32   dwFlags;     // 说明本数据块是不是关键帧、是不是‘rec ’列表等信息
	u32   dwOffset;    // 本数据块在文件中的偏移量
	u32   dwSize;      // 本数据块的大小
} aviunpkg_index_entry_t;



typedef struct _avi_unpkg_fd_t
{
	AVI_UNPKG_DEV_T fs_hdl;
	avi_unpkg_info_t avi_info;
	//cbuffer_t *vcb ;
	//cbuffer_t *acb ;

	aviunpkg_index_entry_t * entry_buf;
	u32 * vindex_buf;
	u32 * aindex_buf;
	u32 vdex_sframe;//对应视频索引表的起始帧号
	u32 vdex_eframe;//对应视频索引表的结束帧号
	u32 vdex_saddr;//对应视频索引表的起始地址
	u32 vdex_eaddr;//对应视频索引表的结束地址

	u32 adex_sframe;//对应音频索引表的起始帧号
	u32 adex_eframe;//对应音频索引表的结束帧号
	u32 adex_saddr;//对应音频索引表的起始地址
	u32 adex_eaddr;//对应音频索引表的结束地址

	u32 unpkg_vframe;//当前解析的视频帧号
	u32 unpkg_aframe;//当前解析的音频帧号

	u32 vframe_addr;
	u32 vframe_len;
	u32 aframe_addr;
	u32 aframe_len;
	u32 vframe_end;		//结束标志
	u32 aframe_end;

	u32 last_vmode;
	u32 last_amode;

	OS_SEM unpkg_sem ;

	volatile u32 unpkg_status;

	if_avi_unpkg_ops_t * ops;
	fb_pipe_t *vpipe ;
	fb_pipe_t *apipe ;

    u32 out_self;
    u32 destroy_flag;


}AVI_UNPKG_FD_T;

typedef struct _AVI_UNPKG_SEEK
{
	u8 seek_type ;		//快进快退
	u32 frame_offset;	//需要向前或者向后定位的帧偏移(以1帧为单位)
	u32 frame_base;		//需要偏移的帧号基值,从1开始
}AVI_UNPKG_SEEK;

enum{
	AVI_UNPKG_IDLE = 0,
	AVI_UNPKG_READY,
	AVI_UNPKG_RUN,
	AVI_UNPKG_PAUSE,
	AVI_UNPKG_SET_STOP,
	AVI_UNPKG_STOP,
	AVI_UNPKG_STOPING,
	AVI_UNPKG_ERR,
};

enum{
	AVI_CMD_UNPKG_RUN,
	AVI_CMD_UNPKG_STOP,
	AVI_CMD_UNPKG_SET_STOP,
	AVI_CMD_UNPKG_STATUS,
	AVI_CMD_UNPKG_PAUSE_RUN,
	AVI_CMD_UNPKG_FF_FR,
	AVI_CMD_UNPKG_SET_SEM,
};


enum{
	AVI_UNPKG_ERR_NONE,
	AVI_UNPKG_ERR_FS,			//文件系统的句柄或者函数为空
	AVI_UNPKG_ERR_READ,			//文件系统读失败
	AVI_UNPKG_ERR_NBUF,			//申请索引表或者AVI表头的空间失败
	AVI_UNPKG_ERR_TYPE,			//索引类型出错
	AVI_UNPKG_ERR_STATUS,		//状态出错
	AVI_UNPKG_ERR_STREAM,		//没有MJPEG视频流或者流数为0
	AVI_UNPKG_ERR_FEND,			//当前的视频帧号结尾帧号
	AVI_UNPKG_ERR_INPUT_PARM,	//输入参数有误
	AVI_UNPKG_ERR_IDX,			//索引表出错
	AVI_UNPKG_ERR_SEM,          //信号量出错
	AVI_UNPKG_ERR_NOMEM,         // 没有内存。
	AVI_UNPKG_ERR_IMPERFECT     // 信息不完整
};





if_avi_unpkg_ops_t * get_avi_unpkg_ops();

#ifdef WIN32
void printf_buf(u8 *buf, u32 len );
#endif


#endif

