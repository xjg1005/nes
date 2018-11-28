#ifndef __USB_SLAVE_API_H__
#define __USB_SLAVE_API_H__
#include "drv_manage.h"
#include "ioctl.h"
#include "errno-base.h"
#include "device_drive.h"
//video和hid不可以同时使能

#ifndef     USB_MSD_EN
#define     USB_MSD_EN           1
#define     USB_MIC_EN           1
#define     USB_SPEAKER_EN       1
#define     USB_HID_EN           0
#define     USB_VIDEO_EN         1
#define 	USB_VIDEO_PRIVATE_EN 1
#endif

#define USB_SLAVE_MASKE 0xf000
#define MASSSTORAGE_CLASS   (USB_SLAVE_MASKE|BIT(0))
#define SPEAKER_CLASS       (USB_SLAVE_MASKE|BIT(1))
#define MIC_CLASS           (USB_SLAVE_MASKE|BIT(2))
#define AUDIO_CLASS         (USB_SLAVE_MASKE|(BIT(1)|BIT(2)))
#define HID_CLASS           (USB_SLAVE_MASKE|BIT(3))
#define VIDEO_CLASS         (USB_SLAVE_MASKE|BIT(4))
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _usb_slv_str_desc
{
    u32 vid_pid;
    const u8 *manufacture;
    const u8 *product_desc;
    const u8 *chipserial;
    const u8 *msd_desc;
    const u8 *spk_desc;
    const u8 *mic_desc;
    const u8 *video_desc;
}USB_SLV_STR_DESC;

struct usb_slv_arg
{
    u16 isr_priority;
    const USB_SLV_STR_DESC *pstr_desc;
    void *parm;

};
typedef struct _hid_cmd
{
    void *addr;
    u32 len;
}HID_CMD;
struct _freame_info
{
    u32 width;
    u32 height;
};
typedef struct _camera_info
{
    struct _freame_info frame[5];
}CAMERA_INFO;

typedef enum _usb_audio_ctl_type
{
    USB_MIC_VOLUME=0x10,
    USB_MIC_LEFT_VOLUME,
    USB_MIC_RIGHT_VOLUME,
    USB_MIC_MUTE,

    USB_SPK_VOLUME=0x20,
    USB_SPK_LEFT_VOLUME,
    USB_SPK_RIGHT_VOLUME,
    USB_SPK_MUTE,
}USB_AUDIO_CTL_TYPE;

typedef u16(* USB_MIC_GET_DATA)(void *ptr,u16 len);
typedef void(* USB_SPK_WRITE_DATA)(void *ptr,u16 len);
typedef u32(* USB_AUDIO_VOLUME_CTL)(USB_AUDIO_CTL_TYPE type,u32 volume);//

typedef void (*OPEN_CAMERA)(u32 frame_index);
typedef void (*CLOSE_CAMERA)();
typedef void* (*CONFIG_CMAERA)(u32 cmd,void *parm);
typedef u32 (*USB_VIDEO_INDATA)(void *ptr,u32 len,BOOL *is_frame_end);



typedef struct _uvc_ctrl_arg
{
    u32 camera_magic;
    OPEN_CAMERA open_camera;
    CLOSE_CAMERA close_camera;
    CONFIG_CMAERA config_camera;
    USB_VIDEO_INDATA video_indata;
    CAMERA_INFO *info;
}UVC_CTRL_ARG;

typedef struct _usb_audio_arg
{
	u32 uac_magic;
    USB_SPK_WRITE_DATA usb_spk_write_data;
    USB_MIC_GET_DATA usb_mic_get_data;
    USB_AUDIO_VOLUME_CTL usb_audio_volume_ctl;
}USB_AUDIO_ARG;


#define LANGUAGE_STR_INDEX  0
#define MANUFACTURE_STR_INDEX  1
#define PRODUCT_STR_INDEX  2
#define SERIAL_STR_INDEX  3
#define MIC_STR_INDEX   4
#define SPEAKER_STR_INDEX   5
#define VIDEO_STR_INDEX   6


const struct DEV_IO * dev_reg_usb0slv(void *parm);
const struct DEV_IO * dev_reg_usb1slv(void *parm);
typedef u16(* HOOK_SCSI_FUN)(void *ptr, void *);

u16 HUSB0_msd_run();
u16 HUSB1_msd_run();


#define USB_SLAVE_MAGIC  'u'//usb slave device
#define USB_SLAVE_MSD_ADD_DEV       _IOW(USB_SLAVE_MAGIC,3,u32)
#define USB_SLAVE_SET_SCSIHOOK      _IOW(USB_SLAVE_MAGIC,4,u32)
#define USB_SLAVE_INIT_UPDATE       _IOW(USB_SLAVE_MAGIC,5,u32)
#define USB_SLAVE_SET_UAC_INFO      _IOW(USB_SLAVE_MAGIC,6,USB_AUDIO_ARG)
#define USB_SLAVE_SET_UVC_INFO      _IOW(USB_SLAVE_MAGIC,7,UVC_CTRL_ARG)
#define USB_SLAVE_UVC_SHUTTER       _IOW(USB_SLAVE_MAGIC,8,u32)
#define USB_SLAVE_HID_TX            _IOW(USB_SLAVE_MAGIC,9,u32)
#define USB_SLAVE_CONNECT_CTL       _IOW(USB_SLAVE_MAGIC,0xa,u32)
#define USB_SLAVE_VIDEO_SHUTTER     _IOW(USB_SLAVE_MAGIC,0xb,u32)
#define USB_SLAVE_SET_HOOK_INFO		_IOW(USB_SLAVE_MAGIC,0xc,u32)
#ifdef __cplusplus
}
#endif
#endif
