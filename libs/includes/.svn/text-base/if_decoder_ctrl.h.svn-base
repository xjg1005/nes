
/*******************************************************************************************

Copyright (C), 2014 -2014, JIELI Tech. Co., Ltd.
File name:if_decoder_ctrl.h
Author:	Version:	Date: // 作者、版本及完成日期
邓玉林  1.0             2014-06-19   09:02:00上午
Description:

Others:	// 其它内容的说明
Function List:


1. ....
History:


1. Date: Author: Modification:
2. ...

*************************************************/
#ifndef __if_decoder_ctrl_h
#define __if_decoder_ctrl_h






#include "typedef.h"
#define PLAY_MOD_NORMAL   0x00
#define PLAY_MOD_FF   0x01
#define PLAY_MOD_FB   0x02


struct if_decoder_io{
    void *priv ;
	int (*input)(void *priv ,u32 addr , void *buf, int len,u8 type);
	/*
	priv -- 私有结构体，由初始化函数提供。
	addr -- 文件位置
	buf  -- 读入地址
	len  -- 读入长度 512 的整数倍
	type -- 0 --同步读（等到数据读回来，函数才返回） ，1 -- 异步读（不等数据读回来，函数就放回）

	*/
	int (*check_buf)(void *priv ,u32 addr , void *buf);
	void (*output)(void *priv  ,void *data, int len);
};

typedef struct break_point_inf
{
u32 revlen ; // 对应frame 帧的时候解码器中剩余数据长度
u32 frame ;  // 帧索引号

}bp_inf_t ;

typedef struct decoder_inf
{
	u16 sr ; // sample rate
	u16 br ; // bit rate
	u32 nch ;
}dec_inf_t ;


typedef struct if_decoder_ops {
	char *name ;               // 解码器名称
	int (*open)(const struct if_decoder_io *decoder_io);
	int (*run)(u32 type);  //  type 参数 参考 PLAY_MOD_X
	int (*stop)();
	u32 (*type_check)() ;// 类型检查
	bp_inf_t* (* get_bp_inf)();
	dec_inf_t* (*get_dec_inf)() ;
	u32 (*get_time)() ;
	u32 (*need_buf_size)() ;
	void (*set_buf)(void* ptr);
	void (*set_step)(u32 step); // 设置快进快进步长。
}decoder_ops_t;



#endif


