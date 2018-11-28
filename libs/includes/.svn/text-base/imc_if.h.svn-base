
/*******************************************************************************************
 File name:imc_if.h

 Version: 1.00

 Discription: imc 模块接口用到的头文件


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期四, 07-10-2014  10:51:03上午



 Copyright:(c)JIELI  2014  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef __imc_if_h
#define __imc_if_h

#include "typedef.h"
#include "yuv_ram.h"
#include "dsc_ram.h"
#include "device_drive.h"
#include "irtc.h"


#define IMC_SRC_ISP0        0
#define IMC_SRC_ISP1        1
#define IMC_SRC_REP         2

//分别对应四个固定的通道
#define IMC_ENC_CH0    0
#define IMC_ENC_CH1    1
#define IMC_DISP_CH0   2
#define IMC_DISP_CH1   3
#define IMC_ALL_CH     4


#define IMC_TYPE_NULL           0x00
#define IMC_TYPE_DISP           0x01 //  缩放用于显示
#define IMC_TYPE_ENC            0x02 //  缩放用于编码

#define IMC_DST_DISP_RAM         0x00
#define IMC_DST_VEDIO_RAM        0x01

#define IMC_CMD_CH_EN              0x00
#define IMC_CMD_CH_DIS             0x01
#define IMC_CMD_OSD_EN             0x02
#define IMC_CMD_OSD_DIS            0x03
#define IMC_REP_FRAME_BEGIN        0x04
#define IMC_REP_KSTART_LINES       0x05
#define IMC_REP_IS_END             0x06         //判断一次REP是否结束
#define IMC_CMD_CH_INIT            0x07
#define IMC_CMD_CH_UNINIT          0x08
#define IMC_REP_INIT               0x09
#define IMC_CHECK_ISR              0x0a
#define IMC_SET_RESTART            0x0b
#define IMC_SET_FRAME_FUN          0x0c
#define IMC_CMD_GET_FRAME_BUF	   0x0d
#define IMC_CMD_CLOSE_FRAME		   0x0e
//#define IMC_USER_DFINE_SIZE


#define IMC_STATUS_UNINIT       0x00
#define IMC_STATUS_INIT         0x01
#define IMC_STATUS_OPEN         0x02
#define IMC_STATUS_CLOSE        0x03

#define IMC_FRAME_END          0x00
#define IMC_FRAME_BEGIN        0x01

#define IMC_MAX_OUT_WIDTH 1920
#define IMC_MAX_OUT_HEIGHT 1088


enum{

	IMC_INIT_SUCC = 0,
    IMC_INIT_NO ,
	IMC_INIT_OVF,           //通道号不在合理的范围之内
	IMC_INIT_USED,          //通道已经在用
	IMC_INIT_SIZE_DIF,      //互斥通道间的源大小不匹配
	IMC_INIT_TYPE_ERR,      //互斥通道间的通道类型冲突
	IMC_INIT_SIZE_OVF,      //通道的目标缩放尺寸溢出
	IMC_INIT_REP_ERR,       //使用REP模块时，不能打开编码通道
};

enum
{
	IMC_NO_CUT = 0,
	IMC_CUT_SRC,
	IMC_SECOND_ZOOM,
};

typedef struct __isp_buf
{
u32 addr ;
//u16 ch ;
u8 used ;
u8 index ;
}isp_buf_t ;

typedef struct __isp_buf_mng
{
 //u32 addr[2] ;
 u32 len[2];
 u32 total ;
}
isp_buf_mng_t ;

typedef struct __imc_info
{
    void * enc_hdl;
    struct dsc_ram *dsc ;
    struct video_ram *video;
    pic_size_t src_size ;
    pic_size_t des_size ;

    int (*frame)(void * parm) ;
    int (*line)(void *parm) ;

    isp_buf_t *ibuf ;
    u8 type ;
    u8 src ;
    u8 des ;
    u8 ch ;

    u8 enc_mode;//选择行编码或者帧编码模式 0 : 帧编码 1 : 行编码
    u8 status;//某通道的状态
    u8 line_cnt;//在行编码模式时，用于计算一帧有多少行
    u8 skip_frame_status;//跳帧状态

    u8 cut_frame_status;//0：正常大小不用切割 ,1: 需要切割, 2：切割块二次缩放

    volatile u8 close_status;
    u8 osd_count;

	u8 imc_enter;
}imc_info_t ;


struct rep_cfg
{
    u8 yuv_type;//0:YUV420 1:YUV422
//    u8 ch_sel;//0为ch0 ，1为ch1
//    u16 width;//image width
    u8 * base_addr;
};

typedef struct _IMC_FUN
{
    u8 ch;
    u8 type;
	int (*pfun)(void );
}IMC_FUN;

typedef struct _imc_rep_info
{
    struct rep_cfg imc_rep_cfg;
    u8 line_cnt;
    struct yuv_addr * yuv;

}imc_rep_info;


struct imc_osd_cfg
{
    //hstart :16 aline      hend   :16 aline
    //vstart :1 aline       vend   :1 aline
    //osd窗口设置
    u16 hstart;
    u16 hend;
    u16 vstart;
    u16 vend;
    //osd数据的起始地址,4 aline
    u8 * ptr;
    u8 str[0x60] ;
    u8 ch ; // 编码通道  0 、1或者2
    u8 size ;
    u8 format ;
    volatile u8 status ;// 0 关闭， 1 打开

    u8 user_osd;//标志是否使用用户自定义的OSD
    char * user_osd_str;//用户自定义格式，例如 "yyyy-nn-dd\hh:mm:ss" 或者 "hh:mm:ss"
    u8 user_osd_w;//用户自定义字体大小,必须为8的倍数
    u8 user_osd_h;
    char * user_osd_matrix;//用户自定义字模字符串,例如“abcd....0123..”
    char * user_osd_matrix_addr;//用户自定义字模的起始地址
    u32 user_osd_str_len;//用户自定义字符串长度
    u32 user_osd_matrix_len;//用户自定义总字模数
    u32 total_len;//用户自定义字模数组的长度
    RTC_TIME last_time;

};

typedef struct __imc_osd_info
{
    u16 x ;
    u16 y;
    u32 yuv;
    u8 ch ;
    u8 size ;//字体大小 0 - 小， 1 -中  2 -大
    u8 format;  //  0-3


//注意：下面的字符串地址必须是全局的,然后年是yyyy，月是nn，日是dd，时是hh，分是mm，秒是ss,其他字符
//是英文字母和符号

    char * user_osd_str;//用户自定义格式，例如 "yyyy-nn-dd\hh:mm:ss" 或者 "hh:mm:ss"
    u8 user_osd_w;//用户自定义字体大小,必须为8的倍数
    u8 user_osd_h;
    char * user_osd_matrix;//用户自定义字模字符串,例如“abcd....0123..”
    char * user_osd_matrix_addr;//用户自定义字模的起始地址
    u32 total_len;//用户自定义字模数组的长度

}imc_osd_info_t ;


/** 对外接口函数 **/
s32 imc_open(void *parm);
s32 imc_close();
s32 imc_ioctrl(void *parm ,u32 cmd);

dev_io_t * get_imc_ops(void *parm);

#endif
