/*************************************************
Copyright (C), 2014 -2014, JIELI Tech. Co., Ltd.
File name:	dev_ctrl.h
Author:	Version:	Date:
����	1.0			2014-07-21
Description:
	�豸����ϵͳ
Others:

Function List:
1. ....

History:
1. Date: Author: Modification:
2. ...

*************************************************/
#ifndef _DEV_CTRL_H
#define _DEV_CTRL_H

#include "typedef.h"
#include "drv_manage.h"
#include "mango_dev_spi.h"
#include "mango_dev_spi0flash.h"

                              //5203 JOHNSON 20160509
#define APP_SD_IO_INIT()      //PORTF_DIR |= BIT(10);PORTF_PU |= BIT(10)
#define APP_SD_IO_HIGH()      //PORTF_PU |= BIT(10)
#define APP_SD_IO_READ()      0//( PORTF_IN & BIT(10) )

/*************************************************
Function:	u32 dev_ctrl_sys_init(void *parm)
Description:	�豸����ϵͳ��ʼ��
Input:	*parm	�߳̾��
Output:	��
Return:	0		�ɹ�
		��0		��������Ϣ�б�
Others:	���豸����ϵͳ����ǰ��Ҫ��ʼ���ļ�ϵͳ
*************************************************/
u32 dev_ctrl_sys_init(void *parm);

DEV_HANDLE dev_get_flash_hdl(void);

void  Task_storedevice_ctrl(void *p_arg);
#endif


