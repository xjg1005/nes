#include "clock.h"
#include "os_api.h"
#include "wdt.h"
#include "app_cfg.h"

/*----------------------------------------------------------------------------*/
/** @brief: 看门狗配置
    @param:time可取：1,2、4、8、16、32 单位s 0为关闭
    @return:
    @author:
    @note:
*/
/*----------------------------------------------------------------------------*/
void wdt_ctf_ctrl(u8 time)
{
#ifdef WDT_EN
    u8 x = 0x0A;
    u8 i;

    for (i = 0; i < 6; i++)
    {
        if(time & BIT(i))
        {
            x +=i;
            break;
        }
    }
    CRC_REG = 0X6EA5;
    WDT_CON = x&0x0f;
    if(time)
    {
        WDT_CON &= ~BIT(5);
        WDT_CON |= BIT(4);
    }
    else
    {
        WDT_CON &= ~BIT(4); //关闭看门狗
        WDT_CON &= ~BIT(0);
    }

    CRC_REG = 0XAAAA;//配置看门狗复位
#endif

}

/*----------------------------------------------------------------------------*/
/** @brief:喂狗
    @param:TRUE 喂狗
    @return:
    @author:
    @note:
*/
/*----------------------------------------------------------------------------*/
u8 wdt_clear_ctrl(u8 flag)
{
#ifdef WDT_EN
    if (flag)
    {
        WDT_CON = ((WDT_CON & ~BIT(6)) | ((flag & 0x01) << 6));
        return TRUE;
    }
    return FALSE;
#endif
}
