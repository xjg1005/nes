#include "i2c.h"
#include "iic.h"
#include "GMA301.H"
#include "mode_switch.h"
#include "enc_mode.h"

volatile u8 Gm301_threhold = 70;

u8 gma301_sensor_command(u8 register_address, u8 function_command)
{
    gravity_sensor_command(WRITE_COMMAND_FOR_GMA301, register_address, function_command);
}

u8 gma301_sensor_get_data(u8 register_address)
{
    return gravity_sensor_get_data(WRITE_COMMAND_FOR_GMA301, READ_COMMAND_FOR_GMA301, register_address);
}


void ReadXYZ(void *pram);
void gma301_init(void)
{
//    puts("\n gma301_init\n");
    gma301_sensor_command(0x21,0x52);//RESET
    gma301_sensor_command(0x00,0x02);//This will download OTP data into internal parameter registers and
    gma301_sensor_command(0x00,0x12);//enable internal clock then clear the data path.
    gma301_sensor_command(0x00,0x02);//
    gma301_sensor_command(0x00,0x82);//
    gma301_sensor_command(0x00,0x02);//
    gma301_sensor_command(0x1F,0x28);//Select the calibrated data as output
    //gma301_sensor_command(0x1F,0x08);//Select the decimation filter as output 2014-12-29 alter by zhuo
    gma301_sensor_command(0x0F,0x00); // set tap number register 2014-12-29 add by zhuo
    gma301_sensor_command(0x11,0x07); //
    gma301_sensor_command(0x0C,0x8f);// 0x8f to enable enz eny enx ent  / 0x8d  to enable z y ent / 0x89 to enable z ent
    gma301_sensor_command(0x00,0x06);//enable sensor  and output

    #ifdef PULLING_GMA301
        timer_reg(100, ReadXYZ, NULL);
    #endif

//interrupt init
//    gma301_sensor_command(0x11,0x07);//IIC 0X07 for no pull up //0x06 High active  0x04 low active
#if 0
//    gma301_sensor_command(0x38,0X53);//用来配置加速度值的大小//10 1g 20 2g 30 3g 40 4g 50 5g 60 6g 最大60 ，在gma301_resolution_range中配置
//    gma301_sensor_command(0x39,0X60);//10 1g 20 2g 30 3g 40 4g 50 5g 60 6g 最大60

    delay_2ms(1);

    gma301_sensor_command(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt

    delay_2ms(1); //2014_0819 added 1ms delay for micro motion setup itself.

    gma301_sensor_command(0x0E,0x1C);//To enable interrupt.
    gma301_sensor_get_data(0x1C);//clear INT status
    gma301_sensor_get_data(0x1D);
#endif
    puts("\n gma301_init end\n");

}



s8 gma301_id_check(void)
{
    u8 i;
    u8 chipid = 0x00;
    puts("\nCHIPID_GMA301 : ");
    for(i=0;i<3;i++)
    {
        chipid = gma301_sensor_get_data(0x12);//chip id

        put_u16hex((u16)chipid);
        if (chipid == 0x55)
            break;
    }
    if (chipid != 0x55)
    {
        puts("\n not gma301");
        return -1;
    }

    return 0;
}


s8 gma301_check(void)
{
    init_i2c_io();

//    gma301_iic_set(1);

//    gma301_sensor_command(0x21,0x52);//RESET

    if (0 != gma301_id_check())
    {
        return -1;
    }

    return 0;
}


//工作模式选着
void gma301_work_mode(u8 work_mode)
{
    switch(work_mode)
    {
    case G_NORMAL_MODE:
        gma301_sensor_command(0x00, 0X06);//normal mode
        break;
    case G_SUSPEND_MODE:
        gma301_sensor_command(0x00, 0X00);
        break;
    case G_LOW_POWER_MODE:
//            gma301_sensor_command(0x11, 0X07);//INT  active H
        gma301_sensor_command(0x00, 0X06);//low_power mode
        gma301_sensor_command(0x0C, 0x8F);
        gma301_sensor_command(0x0D, 0X50);//感度

        break;
    }
}


//工作分辨率选着
void gma301_resolution_range(u8 range)
{
#ifndef PULLING_GMA301
        //gma301_sensor_command(0x00, 0X02);
        gma301_sensor_command(0x11,0x07);
        gma301_sensor_command(0x0F, 0X00);
//        gma301_sensor_command(0x0D, 0X50);
        gma301_sensor_command(0x0E, 0X00);
        gma301_sensor_command(0x1F, 0X28);
    switch(range)
    {
    case G_SENSITY_HIGH:
//        gma301_sensor_command(0x38, 0x3F);// 高灵敏度 2g
        puts("\n G_SENSITY_HIGH \n");
        gma301_sensor_command(0x0D, 0X50);//感度 70 60 50 44 34 24 14
        gma301_sensor_command(0x39, 0x18);// 高灵敏度 1.5g
        break;
    case G_SENSITY_MEDIUM:
//        gma301_sensor_command(0x38, 0x7F);// 中  4g
        puts("\n G_SENSITY_MEDIUM \n");
        gma301_sensor_command(0x0D, 0X44);//感度 34
        gma301_sensor_command(0x39, 0x40);// 中  3.5g
        break;
    case G_SENSITY_LOW:
        puts("\n G_SENSITY_LOW \n");
//        gma301_sensor_command(0x38, 0xFF);// 低 8g
        gma301_sensor_command(0x0D, 0X34);//感度 34
        gma301_sensor_command(0x39, 0x58);// 低 5.5g
        break;
    }
//        puts("\n gma301_sensor_get_data 1 \n");
        gma301_sensor_get_data(0x12);//
        gma301_sensor_get_data(0x13);
        gma301_sensor_get_data(0x14);//X
        gma301_sensor_get_data(0x15);
        gma301_sensor_get_data(0x16);//Y
        gma301_sensor_get_data(0x17);
        gma301_sensor_get_data(0x18);//Z
        gma301_sensor_get_data(0x19);
//        puts("\n gma301_sensor_get_data 2 \n");

//        delay_2ms(2); //2014_0819 added 1ms delay for micro motion setup itself.
        delay(1000);
//        puts("\n gma301_sensor_get_data 3 \n");
        gma301_sensor_command(0x1F,0x38);
//        gma301_sensor_command(0x11,0x07);
//        puts("\n gma301_sensor_get_data 4 \n");
//        delay_2ms(1);
//        delay(1000);
//        puts("\n gma301_sensor_get_data 5 \n");
        gma301_sensor_get_data(0x1C);//clear INT status
        gma301_sensor_get_data(0x1D);
        gma301_sensor_command(0x0E,0x1C);
        //gma301_sensor_command(0x00, 0X06);
//        puts("\n gma301_resolution_range \n");
#else
        //gma301_sensor_command(0x00, 0X02);
        gma301_sensor_command(0x11,0x07);
        gma301_sensor_command(0x0F, 0X00);
//        gma301_sensor_command(0x0D, 0X50);
        gma301_sensor_command(0x0E, 0X00);
        gma301_sensor_command(0x1F, 0X28);

    switch(range)
    {
    case G_SENSITY_HIGH:
        Gm301_threhold = 20;
		gma301_sensor_command(0x0D,0x70);
        break;
    case G_SENSITY_MEDIUM:
        Gm301_threhold = 50;
        gma301_sensor_command(0x0D,0x60);

        break;
    case G_SENSITY_LOW:
        Gm301_threhold = 90;
		gma301_sensor_command(0x0D,0x70);
        break;
    }

//        puts("\n gma301_sensor_get_data 1 \n");
        gma301_sensor_get_data(0x12);//
        gma301_sensor_get_data(0x13);
        gma301_sensor_get_data(0x14);//X
        gma301_sensor_get_data(0x15);
        gma301_sensor_get_data(0x16);//Y
        gma301_sensor_get_data(0x17);
        gma301_sensor_get_data(0x18);//Z
        gma301_sensor_get_data(0x19);
//        puts("\n gma301_sensor_get_data 2 \n");

//        delay_2ms(2); //2014_0819 added 1ms delay for micro motion setup itself.
        delay(1000);
//        puts("\n gma301_sensor_get_data 3 \n");
        gma301_sensor_command(0x1F,0x38);
//        gma301_sensor_command(0x11,0x07);
//        puts("\n gma301_sensor_get_data 4 \n");
//        delay_2ms(1);
//        delay(1000);
//        puts("\n gma301_sensor_get_data 5 \n");
        gma301_sensor_get_data(0x1C);//clear INT status
        gma301_sensor_get_data(0x1D);
        gma301_sensor_command(0x0E,0x1C);
        //gma301_sensor_command(0x00, 0X06);
//        puts("\n gma301_resolution_range \n");

#endif

}

void gma301_gravity_sensity(u8 gsid,u8 sensity)
{
    //高中低
    if (gsid == G_SENSOR_HIGH)  //高
    {
        gma301_work_mode(G_NORMAL_MODE);
        gma301_resolution_range(G_SENSITY_HIGH);
    }

    if (gsid == G_SENSOR_MEDIUM)  //中
    {
        gma301_work_mode(G_NORMAL_MODE);
        gma301_resolution_range(G_SENSITY_MEDIUM);
    }

    if (gsid == G_SENSOR_LOW)  //低
    {
        gma301_work_mode(G_NORMAL_MODE);
        gma301_resolution_range(G_SENSITY_LOW);
    }

    if (gsid == G_SENSOR_CLOSE)  //
    {
        gma301_sensor_command(0x38,0XFF);//关闭
        gma301_work_mode(G_SUSPEND_MODE);//暂停

    }

    if (gsid == G_SENSOR_LOW_POWER_MODE)//低功耗
    {
        gma301_work_mode(G_LOW_POWER_MODE);
//        if(sensity == 0xFF)//默认值
//        {
//            gma301_resolution_range(G_SENSITY_MEDIUM);
//        }
//        else
//            gma301_resolution_range(sensity);
        gma301_sensor_command(0x11,0x07);
        gma301_sensor_command(0x0F, 0X00);
//        gma301_sensor_command(0x0D, 0X50);
        gma301_sensor_command(0x0E, 0X00);
        gma301_sensor_command(0x1F, 0X28);

        switch(sensity)
        {
        case G_SENSITY_HIGH:
    //        gma301_sensor_command(0x38, 0x3F);// 高灵敏度 2g
            puts("\n G_SENSITY_HIGH \n");
            gma301_sensor_command(0x0D, 0X50);//感度 70 60 50 44 34 24 14
            gma301_sensor_command(0x39, 0x18);// 高灵敏度 1.5g
            break;
        case G_SENSITY_MEDIUM:
    //        gma301_sensor_command(0x38, 0x7F);// 中  4g
            puts("\n G_SENSITY_MEDIUM \n");
            gma301_sensor_command(0x0D, 0X44);//感度 34
            gma301_sensor_command(0x39, 0x38);// 中  3.5g
            break;
        case G_SENSITY_LOW:
            puts("\n G_SENSITY_LOW \n");
    //        gma301_sensor_command(0x38, 0xFF);// 低 8g
            gma301_sensor_command(0x0D, 0X34);//感度 34
            gma301_sensor_command(0x39, 0x58);// 低 5.5g
            break;

        case 0xFF:
            gma301_sensor_command(0x0D, 0X50);//感度 34
            gma301_sensor_command(0x39, 0x38);// 中  3.5g
        break;
        }


        gma301_sensor_get_data(0x12);//
        gma301_sensor_get_data(0x13);
        gma301_sensor_get_data(0x14);//X
        gma301_sensor_get_data(0x15);
        gma301_sensor_get_data(0x16);//Y
        gma301_sensor_get_data(0x17);
        gma301_sensor_get_data(0x18);//Z
        gma301_sensor_get_data(0x19);

        gma301_sensor_command(0x1F,0x38);

        gma301_sensor_get_data(0x1C);//clear INT status
        gma301_sensor_get_data(0x1D);

        gma301_sensor_command(0x0E,0x1C);
    }

}

//u8 lock_current_file;

volatile static u8 gsensor_usb = 0;//0 没动，插入和拔出usb，gsensor解锁 1插入 2拔出

void usb_charging_status (void)
{
    static u8 charging_status = 0; //0没充电 1充电
    if (charging_status == 0)
    {
        if(usb_is_chargeing() == 1)
        {
            charging_status = 1;
            gsensor_usb = 1;
            OSTimeDly(10);
        }
    }
    else
    {
        if (usb_is_chargeing() == 0)
        {
            charging_status = 0;
            gsensor_usb = 2;
            OSTimeDly(10);
        }
    }
}

void gma301_pin_int_interrupt(void *pr)
{
    return;
#ifndef PULLING_GMA301
    u8 temp;
	temp = gma301_sensor_get_data(0x12);
    temp = gma301_sensor_get_data(0x13);
    if (temp & BIT(5))//INT
    {
//        if(!gma301_sensor_get_data(0x1c))
//        {
//            gma301_init();
//            return;
//        }
//        if(!gma301_sensor_get_data(0x1D))
//        {
//            gma301_init();
//            return;
//        }
        gma301_sensor_get_data(0x1c);
        gma301_sensor_get_data(0x1D);
//        gsensor_pr2Pnd_api(FALSE);
        if( (get_gravity_sensor() != 0)
             &&  (enc_ctrl.enc0_status == ENC_APP_RUNNING)
             &&  (mode_ctrl.cur_mode & ENC_MODE)
             &&  (enc_ctrl.lock_current_file  == 0)
        )
        {
            //printf("\n gma301 lock the file#######\n");
            puts("\ngm\n");
            if (gsensor_usb !=0)
            {
                gsensor_usb = 0;
//                enc_ctrl.lock_current_file = 0;
            }
            else
            enc_ctrl.lock_current_file = 1;//锁当前文件
        }
    }
#else
        u8 temp;
	temp = gma301_sensor_get_data(0x12);
    temp = gma301_sensor_get_data(0x13);
    if (temp & BIT(5))//INT
    {

        gma301_sensor_get_data(0x1c);
        gma301_sensor_get_data(0x1D);

    }
#endif
}

#ifdef PULLING_GMA301
void ReadXYZ(void *pram)
{
 s32 tmp_data[6];
 s32 Xdata,Ydata,Zdata,tmp;
 static s32 Xdatab=0,Ydatab=0,Zdatab=0;
 s32 value;

 gma301_sensor_get_data(0x12);
 gma301_sensor_get_data(0x13);

 tmp_data[0] = gma301_sensor_get_data(0x14);
 tmp_data[1] = gma301_sensor_get_data(0x15);
 tmp_data[2] = gma301_sensor_get_data(0x16);
 tmp_data[3] = gma301_sensor_get_data(0x17);
 tmp_data[4] = gma301_sensor_get_data(0x18);
 tmp_data[5] = gma301_sensor_get_data(0x19);

 //printf("TmpX :%x,%x\r\n",tmp_data[0],tmp_data[1]);
 //printf("TmpY :%x,%x\r\n",tmp_data[2],tmp_data[3]);
 //printf("TmpZ :%x,%x\r\n",tmp_data[4],tmp_data[5]);


 if((tmp_data[1] & 0xf0 == 0xf0) )
 {
  Xdata = (short)(tmp_data[1] << 8 |tmp_data[0]);
  Xdata = (0xffffffff) - Xdata + 1;
  Xdata = -Xdata;
 }
 else
 {
  Xdata = (short)(tmp_data[1] << 8 |tmp_data[0]);
   }

 if((tmp_data[3] & 0xf0 == 0xf0) )
 {
  Ydata = (short)(tmp_data[3] <<8 |tmp_data[2]);
  Ydata = (0xffffffff) - Ydata + 1;
  Ydata = -Ydata;
 }
 else
 {
  Ydata = (short)(tmp_data[3] << 8|tmp_data[2]);
  }

 if((tmp_data[5] & 0xf0 == 0xf0) )
 {
  Zdata = (short)(tmp_data[5] << 8|tmp_data[4]);
  Zdata = (0xffffffff) - Zdata + 1;
  Zdata = -Zdata;
 }
 else
 {
  Zdata = (short)(tmp_data[5] <<8 |tmp_data[4]);
 }

#define moving_step 4//感度調整 設8、16也可以，越大越不靈敏
static int pointer = -1;
static s32 bufferave[3][moving_step]; // 8 buffer
s32 sum[3];  //  sum of moving_step
s32 avg[3];  // moving average result
int i,j,k,G_INT_COUNT=0;

if ( pointer == -1 )
{
 for(k = 0; k < moving_step; ++k)
 {
  bufferave[0][k] = Xdata;
  bufferave[1][k] = Ydata;
  bufferave[2][k] = Zdata;
 }
}
pointer++;
pointer %= moving_step;

bufferave[0][pointer] = Xdata;
bufferave[1][pointer] = Ydata;
bufferave[2][pointer] = Zdata;

for(i = 0; i < 3; ++i)
 {
 sum[i] = 0;

 for(j = 0; j < moving_step; ++j)
  {
  sum[i] += bufferave[i][j];
  }

 avg[i] = sum[i] / moving_step;
 }

 Xdata= avg[0];
 Ydata= avg[1];
 Zdata= avg[2];
//moving average end
//printf("Gm301_threhold:%d\r\n",Gm301_threhold);

if(Xdatab!=0 & Ydatab!=0 & Zdatab !=0)
  {
  if(abs(Xdatab-Xdata)>(Gm301_threhold*1))
  	 {

	  	G_INT_COUNT++;
	    if( G_INT_COUNT>=1)
		{
			G_INT_COUNT==1;
			printf("GM301 X-----:%d\r\n",abs(Xdatab-Xdata));
	    }
		else
			return FALSE;
	}
  if(abs(Ydatab-Ydata)>Gm301_threhold)
  	{
	  	G_INT_COUNT++;
	    if( G_INT_COUNT>=1)
		{
			G_INT_COUNT==1;
			printf("GM301 Y-----:%d\r\n",abs(Ydatab-Ydata));
	    }
		else
			return FALSE;
	}
  if(abs(Zdatab-Zdata)>(Gm301_threhold*1))
  	{
	  	G_INT_COUNT++;
	    if( G_INT_COUNT>=3)
		{
			G_INT_COUNT==1;
			printf("GM301 Z-----:%d\r\n",abs(Zdatab-Zdata));
	    }
		else
			return FALSE;
	}
 	   Xdatab = Xdata;
	   Ydatab = Ydata;
	   Zdatab = Zdata;

  if(G_INT_COUNT>=1)
		{

        u8 temp;
        temp = gma301_sensor_get_data(0x12);
        temp = gma301_sensor_get_data(0x13);
        if (temp & BIT(5))//INT
        {

            gma301_sensor_get_data(0x1c);
            gma301_sensor_get_data(0x1D);

        }

        if( (get_gravity_sensor() != 0)
         &&  (enc_ctrl.enc0_status == ENC_APP_RUNNING)
         &&  (mode_ctrl.cur_mode & ENC_MODE)
         &&  (enc_ctrl.lock_current_file  == 0))
        {
            if (gsensor_usb !=0)
            {
                gsensor_usb = 0;
//                enc_ctrl.lock_current_file = 0;
            }
            else
            enc_ctrl.lock_current_file = 1;//锁当前文件
//            tmp = gma301_sensor_get_data(0x0d);
//            printf("GM301 0d :%x\r\n",tmp);
//            printf("gsensor_usb=:%d\r\n",gsensor_usb) ;
//            printf("GM301 LOCKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK\r\n");
//            printf("GM301 X:%d\r\n",Xdata);
//            printf("GM301 Y:%d\r\n",Ydata);
//            printf("GM301 Z:%d\r\n",Zdata);
        }
//   return 1;
  }
  else
  return FALSE;

  }
  else
  {
   Xdatab = Xdata;
   Ydatab = Ydata;
   Zdatab = Zdata;
  }
}
#endif
