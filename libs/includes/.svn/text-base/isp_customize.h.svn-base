
#ifndef _ISP_CUSTOMIZE_H
#define _ISP_CUSTOMIZE_H

#include "typedef.h"


typedef enum {
    WIN_WEIGHT_AVG = 0,  //  平均测光
    WIN_WEIGHT_CENTRE,   //  中央区域测光
    WIN_WEIGHT_SPOT,     //  点测光
    WIN_WEIGHT_MULTI,    //  多区域测光
    WIN_WEIGHT_CARDV,    //  行车记录仪专用
    WIN_WEIGHT_DEFAULT
} ww_type_t;


typedef enum {
    WB_PRESET_DAYLIGHT,  // 晴天5600K
    WB_PRESET_CLOUNDY,       //  多云/阴天 D65 6500K
    WB_PRESET_TUNGSTEN,      // 钨丝灯/白炽灯 TL83/U30 3000K
    WB_PRESET_FLUORESCENT1,    // 荧光灯1 D65 6500K
    WB_PRESET_FLUORESCENT2,    // 荧光灯2 TL84 4000K
    WB_PRESET_DISABLE
} wb_preset_t;


typedef enum {
    DRC_DISABLE = 0,
    DRC_ENABLE = 1,
}drc_en_t;


// 黑电平校正，取值为(-512,511)， OnOff表示该定制功能是否Enable;
void ispt_set_blc(u8 OnOff, s16 r_off, s16 gr_off, s16 gb_off, s16 b_off);

// 曝光补偿，取值 [-3,3]
void ispt_customize_ev(s32 ev);

// 测光模式
void ispt_customize_evMode(ww_type_t mode);

// RGB gamma, 256阶 * 8b, 三个分量相同, 精度是8.0
void ispt_customize_gamma(u8 *tbl);

// 颜色矩阵 DST3x1 = CCM3x4  *  SRC4x1, CCM3x4数据按照 c00,c01,c02,c03,c10,c11,c12,c13,c20...排列，精度是2.7
void ispt_customize_ccm3x4(s16 *ccm);

// 运动侦测设置等级 [0,4]， 越小越灵敏
void ispt_customize_mdLevel(u8 level);

// 运动侦测状态查询，返回前1s (约30fps)是否存在运动；返回1表示存在，0表示不存在
s32 ispt_customize_mdStatus();

// 降噪级别设置 [0,6] 越大越强，需要配合锐化级别，才有较好的效果
void ispt_customize_nrLevel(u8 level);

// 锐化级别设置 [0,4] 越大越强，需要配合降噪级别，才有较好的效果
void ispt_customize_shpnLevel(u8 level);


// 镜头阴影矫正. (oox,ooy)表示光学中心距离图像中心的偏移，范围不超过图像分辨率的一半；
// 三个分量分开设置矫正偏移增益，每个表格 16b x 1024项，不同距离的增益；精度为2.8;
// 图像中每一个分量的每个像素，通过DV12 SOC计算距离后，查找这三表，得到该像素的增益；
void ispt_customize_lsc(u16 oox, u16 ooy, u16 *r, u16 *g, u16 *b);

// 预设白平衡
void ispt_preset_wb(wb_preset_t wb_type);

// 手动填写白平衡增益, gain的精度为3.7, OnOff表示该定制功能是否Enable;
void ispt_mannual_wb(u8 OnOff, u16 r_gain, u16 g_gain, u16 b_gain);

// 饱和度调整, 精度2.8
void ispt_saturation(u16 uv);

// 获取当前外部光线,返回值 0-16
// 其中0-8 夜晚，8-11，傍晚室内，11-16,白天
// 值越大，光线越好。光线越强，清晰越好，码率越高，必须降低q值；
u32 ispt_get_lv();

// 是否打开数字wdr功能
void ispt_customize_drcEnable(u8 en);

// 锐化阀值，取值0-20,其中0表示该自定义功能不打开，由isp内部自动选择锐化阀值和强度
void ispt_customize_shpnThreshold(u8 threshold);

// 锐化强度，精度2.7
void ispt_customize_shpnAmount(u16 amount);

#endif

