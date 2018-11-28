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
	//���ļ�������صĲ���
	void * pf;
	u32 (*fs_write)(void *pFile, void *pBuf, u32 len);
	u32 (*fs_read)(void * pFile ,void *pBuf, u32 btr);
	u32 (*fs_seek)(void * pFile, u32 type, u32 offsize);
	u32 (*fs_tell)(void * pFile);//��ȡ��װ�ļ���ǰλ��
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
	//AVI��װ���������
	u32 aud_sr;//������
	u32 aud_ch;//����ͨ��
	u32 aud_bits;//����λ��

	u32 vid_fps;//֡��
	u32 vid_width;
	u32 vid_heigh;
	u32 vid_real_width;
	u32 vid_real_heigh;

	//u32 vid_cbuf_len;
	//u32 aud_cbuf_len;
	//u8 * vid_cbuf_ptr;
	//u8 * aud_cbuf_ptr;

	int (* is_transf_end)(void * priv);	//�ж��Ƿ���λ������
	void * priv;

	u32 discard_aud;
	u32 need_aud_cnt;

}AVI_PKG_INFO_T;


//#define AVIPKG_IDX_LEN   0x20//(60*60*30*2)
#define AVIPKG_IDX_LEN   (0x10000/8)//(0x8000/8)

typedef struct _avioldindex_entry {
	//u32   dwChunkId;   // ���������ݿ�����ַ���
	//u32   dwFlags;     // ˵�������ݿ��ǲ��ǹؼ�֡���ǲ��ǡ�rec ���б����Ϣ
	u32   dwOffset;    // �����ݿ����ļ��е�ƫ����
	u32   dwSize;      // �����ݿ�Ĵ�С  bit31 ��ʾ dwFlags
} avioldindex_entry_t;

typedef struct _avioldindex_entry_write {
	u32   dwChunkId;   // ���������ݿ�����ַ���
	u32   dwFlags;     // ˵�������ݿ��ǲ��ǹؼ�֡���ǲ��ǡ�rec ���б����Ϣ
	u32   dwOffset;    // �����ݿ����ļ��е�ƫ����
	u32   dwSize;      // �����ݿ�Ĵ�С
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
	AVI_PKG_ERR_FS,			//�ļ�ϵͳ�ľ�����ߺ���Ϊ��
	AVI_PKG_ERR_WRITE,		//�ļ�ϵͳдʧ��
	AVI_PKG_ERR_READ,		//�ļ�ϵͳ��ʧ��

	AVI_PKG_ERR_NBUF,		//�������������AVI��ͷ�Ŀռ�ʧ��
	AVI_PKG_ERR_TYPE,		//�������ͳ���
	AVI_PKG_ERR_STATUS,		//״̬����
	AVI_PKG_ERR_TAB_EXT,	//�����
	AVI_PKG_ERR_SKIP_FRM,	//��֡ʧ��
	AVI_PKG_ERR_CMD,		//������һ��û�е�����
	AVI_PKG_ERR_SEM,        //�ź�������
};

if_avipkg_ops_t * get_avi_pkg_ops();

#endif

