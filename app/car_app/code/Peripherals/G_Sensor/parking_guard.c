#include "parking_guard.h"
#include "irtc.h"


/*----------------------------------------------------------------------------*/
/** @brief:     ͣ������PR2 IO��pnd���
    @param:
    @return:
    @author:
    @note:
*/
/*----------------------------------------------------------------------------*/
void gsensor_pr2Pnd_api(u8 flag)
{//��ȡ�Ƿ�������PR2���𿪻����ǾͿ�ʼ¼��

    if (read_RTC_reg(0xa5) & BIT(2))//��ȡwkup_pnd
	{
        puts("\n 5555555 clear wkup_pnd\n");
        write_RTC_reg(0x25,0x40);//���pnd
//        if (flag == TRUE)
//            mode_change_ctrl.parking_guard = TRUE;
	}

    if (read_RTC_reg(0xa1) & BIT(2))//��ȡpr2�ߵ͵�ƽ�仯
	{
        puts("\ 3333333 io pr2 high \n");
	}
}



