#include "parking_guard.h"
#include "irtc.h"


/*----------------------------------------------------------------------------*/
/** @brief:     停车守卫PR2 IO的pnd检测
    @param:
    @return:
    @author:
    @note:
*/
/*----------------------------------------------------------------------------*/
void gsensor_pr2Pnd_api(u8 flag)
{//读取是否是由于PR2引起开机，是就开始录像

    if (read_RTC_reg(0xa5) & BIT(2))//读取wkup_pnd
	{
        puts("\n 5555555 clear wkup_pnd\n");
        write_RTC_reg(0x25,0x40);//清除pnd
//        if (flag == TRUE)
//            mode_change_ctrl.parking_guard = TRUE;
	}

    if (read_RTC_reg(0xa1) & BIT(2))//读取pr2高低电平变化
	{
        puts("\ 3333333 io pr2 high \n");
	}
}



