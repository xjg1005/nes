#ifndef __AVI_PKG_IF_H__
#define __AVI_PKG_IF_H__


#include "typedef.h"
#include "circular_buf.h"
#include "frame_buf.h"


#define AVI_PKG_SEEK_SET        0x00
#define AVI_PKG_SEEK_CUR        0x01
#define AVI_PKG_SEEK_END        0x02


//#ifdef WIN32
#if 1

//#define TEST_JPEG_BITES

typedef struct _pkg_mix_parm
{
    void *priv  ;
    void *data  ;
    u32 len;
}pkg_mix_parm;

#endif


typedef struct __if_avipkg_ops
{
	char *name ;
	s32 (*init)();
	s32 (*open)(void *priv);
	s32 (*close)(void *priv);
	s32 (*read)(void *priv ,u8*buf, u32 addr , u32 len);
	s32 (*write)(void *priv ,u8*buf , u32 addr , u32 len);
	s32 (*ioctrl)(void *priv ,void* parm, u32 cmd );
}if_avipkg_ops_t ;


typedef struct _AVI_PKG_DEV_T
{
	//与文件操作相关的部分
	void * pf;
	u32 (*fs_write)(void *pFile, void *pBuf, u32 len);
	u32 (*fs_read)(void * pFile ,void *pBuf, u32 btr);
	u32 (*fs_seek)(void * pFile, u32 type, u32 offsize);
	u32 (*fs_tell)(void * pFile);//获取封装文件当前位置
    void (*fs_close)(void  *pFile);

    void * (*wfsv_open)(fb_pipe_t *pipe);
    u32 (*wfsv_start)(void * wfsv ,void *fp) ;
    u32 (*wfsv_close)(void * wfsv);
    u32 (*wfsv_stop)(void * wfsv);
    u32 (*wfsv_write)(void  *pFile, fbuf_t *fb);
    u32 (*wfsv_tell)(void * pFile);
    u32 (*wfsv_df)(void * pFile);
    u32 (*wfsv_df_bk)(void * pFile);
    void  * wfsv_pf;
    u32 (*wfsv_clr_fb)(void * wfsv);


}AVI_PKG_DEV_T;

typedef struct _AVI_PKG_INFO_T
{
	//AVI封装器输入参数
	u32 aud_sr;//采样率
	u32 aud_ch;//采样通道
	u32 aud_bits;//采样位深

	u32 vid_fps;//帧率
	u32 vid_width;
	u32 vid_heigh;
	u32 vid_real_width;
	u32 vid_real_heigh;

	//u32 vid_cbuf_len;
	//u32 aud_cbuf_len;
	//u8 * vid_cbuf_ptr;
	//u8 * aud_cbuf_ptr;

	int (* is_transf_end)(void * priv);	//判断是否传输位流结束
	void * priv;

	u32 discard_aud;
	u32 need_aud_cnt;

}AVI_PKG_INFO_T;


//#define AVIPKG_IDX_LEN   0x20//(60*60*30*2)
#define AVIPKG_IDX_LEN   (0x10000/8)//(0x8000/8)

typedef struct _avioldindex_entry {
	//u32   dwChunkId;   // 表征本数据块的四字符码
	//u32   dwFlags;     // 说明本数据块是不是关键帧、是不是‘rec ’列表等信息
	u32   dwOffset;    // 本数据块在文件中的偏移量
	u32   dwSize;      // 本数据块的大小  bit31 表示 dwFlags
} avioldindex_entry_t;

typedef struct _avioldindex_entry_write {
	u32   dwChunkId;   // 表征本数据块的四字符码
	u32   dwFlags;     // 说明本数据块是不是关键帧、是不是‘rec ’列表等信息
	u32   dwOffset;    // 本数据块在文件中的偏移量
	u32   dwSize;      // 本数据块的大小
} avioldindex_entry_write_t;

typedef struct _avi_idx_ctl {
	u32 idx_vnum;
	u32 idx_anum;
	u32 idx_offset;
	u32 idx_soffset;
	u32 idx_toffset;
	avioldindex_entry_t *idx_buf;
	u32 * idx_sbuf;
	u32 * idx_tbuf;
	fbuf_t *fb_index ;
	fbuf_t *fb_sbuf;
	fbuf_t *fb_tbuf ;

} avi_idx_ctl_t;


typedef struct _avi_pkg_fd_t
{
	AVI_PKG_DEV_T fs_hdl;
	AVI_PKG_INFO_T input_parm;

	u32 movi_addr;
	u32 movi_len;
	volatile u32 pkg_status;
	fb_pipe_t *vpipe ;
	fb_pipe_t *apipe ;
	u32 skip_frame_cnt;
	avi_idx_ctl_t idx_ctl;
	if_avipkg_ops_t * ops;
	u32 index ;
	OS_SEM avi_pkg_sem;

	u8 aud_cache[64];
	u8 aud_cache_len;
	u32 aud_offset;
	fbuf_t * aud_fb ;
	u32 stop_cnt;
	u32 real_vid_frame;

}AVI_PKG_FD_T;


enum{
	AVI_PKG_IDLE = 0,
	AVI_PKG_READY,
	AVI_PKG_RUN,
	AVI_PKG_SET_STOP,
	AVI_PKG_STOP,
	AVI_PKG_STOPING,
	AVI_PKG_ERR,
};

enum{
	AVI_CMD_PKG_RUN,
	AVI_CMD_PKG_START,
	AVI_CMD_PKG_STOP,
	AVI_CMD_PKG_SET_STOP,
	AVI_CMD_PKG_STATUS,
	AVI_CMD_PKG_ADD_SKIP_FRM,
	AVI_CMD_PKG_VIN,
	AVI_CMD_PKG_VEND,
	AVI_CMD_PKG_AIN,
	AVI_CMD_PKG_AEND,
	AVI_CMD_PKG_CLR_FB,
};


enum{
	AVI_PKG_ERR_NONE,
	AVI_PKG_ERR_FS,			//文件系统的句柄或者函数为空
	AVI_PKG_ERR_WRITE,		//文件系统写失败
	AVI_PKG_ERR_READ,		//文件系统读失败

	AVI_PKG_ERR_NBUF,		//申请索引表或者AVI表头的空间失败
	AVI_PKG_ERR_TYPE,		//索引类型出错
	AVI_PKG_ERR_STATUS,		//状态出错
	AVI_PKG_ERR_TAB_EXT,	//表溢出
	AVI_PKG_ERR_SKIP_FRM,	//补帧失败
	AVI_PKG_ERR_CMD,		//输入了一个没有的命令
	AVI_PKG_ERR_SEM,        //信号量出错
};

if_avipkg_ops_t * get_avi_pkg_ops();

#endif

