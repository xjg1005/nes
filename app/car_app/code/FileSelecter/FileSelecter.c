

/*************************************************************
File: FileSelecter.c
Author:xuhuajun
Discriptor:
    文件选择器函数实现
Version:1.0
Date：2014.8
*************************************************************/

#include "vfs.h"
#include "FileSelecter.h"
#include "car_fs_api.h"
#include "menu.h"
#include "mode_switch.h"
#include "my_ui.h"


char root_dir[13];// = "B:/DCIMA\0";
char root_dir1[13];// = "B:/DCIMB\0";
char rec_file_format[13];
char rec_file_format1[13];
char pro_file_format[13];
char pro_file_format1[13];
char pic_file_format[13];
u8 video_path[125];
u8 cur_video_name[2][64];

char today_path[9];
s32 current_index;
s32 current_index1;
s32 name_fix_len;

int cycle_time = 120 ;

u32 bdisk_ready;
u32 g_need_format;//格式化标志


u32 is_sd_online()
{
    if(g_need_format == TRUE){
        return FALSE;
    }else{
        return bdisk_ready;
    }
}

u32 is_phy_sd_online()
{
    return bdisk_ready;
}

u8 wait_sd_ok(void)
{
    u32 bdisk_cnt = 0;

//    while ((is_sd_online() == FALSE))
    while ((is_phy_sd_online() == FALSE))
    {
        os_time_dly(10);

        bdisk_cnt++;
        if (bdisk_cnt > 40){//四秒等待tf上线，解决TF卡有时不升级,如tf卡上线特别慢，可增加到 50\60\70
            bdisk_cnt = 0;
            return FALSE;//超时退出
        }
    }

    return TRUE;
}



/*************************************************
Function:	u32 Load_vfs(int max_open_file)
Description:	虚拟文件系统初始化
Input:	max_open_file       最多同时打开的文件数
Output:	无
Return:	-1      不成功
        >=0     该FS的ID
Others:
*************************************************/

extern u32 vfs_init(int max_open_file);
u32 Load_vfs(int max_open_file)
{
    vfs_init(max_open_file);
    bdisk_ready = FALSE;
    g_need_format = FALSE;
    return 0;
}

/*************************************************
Function:	void set_file_format_path(char *rootdir,char*rec_format,char *sos_format,char *pic_format, char *rec_format1)
Description:	设置录像目录名及录像文件名、保护文件名格式
Input:	rootdir       目录像目录名，比如DCIM
		rec_format    录像文件名格式，比如rec?????.avi，长度必须是8.3，'？'号部分由程序自动填序号。
		sos_format    保护录像文件名格式，比如sos?????.avi，长度必须是8.3，'？'号部分由程序自动填序号。
		pic_format    图片文件名格式，同上
Output:	无
Return:	无
        无
Others:
*************************************************/
void set_file_format_path(char *rootdir,char*rec_format,char *sos_format,char *pic_format,char *rootdir1, char *rec_format1,char *sos_format1)
{
    int i;

    strcpy(root_dir,rootdir);
    strcpy(root_dir1,rootdir1);
    strcpy(rec_file_format,rec_format);
    strcpy(pro_file_format,sos_format);
    strcpy(pic_file_format,pic_format);
    strcpy(rec_file_format1,rec_format1);
    strcpy(pro_file_format1,sos_format1);

    name_fix_len=0;
    while(rec_file_format[name_fix_len]!='?')
    {
        name_fix_len++;
        if(name_fix_len == 8)
            break;
    }
    printf("name_fix_len = %d \n",name_fix_len);

    for(i=0; i<name_fix_len; i++) //转成大写
    {
        char a;
        a = rec_file_format[i];
        if ((a>='a') && (a<='z'))
        {
            a -='a';
            a +='A';
            rec_file_format[i] = a;
        }

        a = pro_file_format[i];
        if ((a>='a') && (a<='z'))
        {
            a -='a';
            a +='A';
            pro_file_format[i] = a;
        }

        a = pic_file_format[i];
        if ((a>='a') && (a<='z'))
        {
            a -='a';
            a +='A';
            pic_file_format[i] = a;
        }

        a = rec_file_format1[i];
        if ((a>='a') && (a<='z'))
        {
            a -='a';
            a +='A';
            rec_file_format1[i] = a;
        }

        a = pro_file_format1[i];
        if ((a>='a') && (a<='z'))
        {
            a -='a';
            a +='A';
            pro_file_format1[i] = a;
        }
    }
}

extern void num_of_last_time_for_record(void);
/*************************************************
Function:	BOOL dev_fs_msg_ctrl(u32 msg, void *parm)
Description:	文件选择器消息处理函数
Input:	msg       消息号
		*param    消息参数
Output:	无
Return:	TRUE	消息已处理
		FALSE   消息未处理
Others: 该函数为维护存储驱动器之用，所以只处理MSG_DEVICECHANGE消息
*************************************************/
BOOL file_sel_msg_ctrl(u32 msg, void *parm)
#ifdef WIN32
{
    if (msg)
        device_on_off("\\\\.\\PhysicalDrive1",TRUE);
    else
        device_on_off("\\\\.\\PhysicalDrive1",FALSE);
    return TRUE;
}
#else
{
    msg_devicechange_t *devsta;

    u32 cluster_size;
    u32 need_space;
    u32 free_space;

    VFS_ENTER_MUTEX();

    if(1)
    {
        //case MSG_DEVICECHANGE :		//设备变化
        devsta =  parm ;
        puts(devsta->hdev->dev_io->name);
        if (devsta->hdev->dev_io->device_type < DEV_LOGIC_DISK) //物理设备
        {
            if(devsta->sta == DEV_ONLINE)
            {
                //挂载存储设备。挂载文件系统、分区、广播盘符插入等
                printf("\nonline %x \r\n",(unsigned int)devsta->hdev);
                device_on_off(devsta->hdev,TRUE);

            }
            else
            {
                //卸载存储设备。卸载文件系统、广播盘符拔出等
                printf("\n\n\n----------------------offline\r\n\n\n\n");
                device_on_off(devsta->hdev,FALSE);

            }

        }
        else if (devsta->hdev->dev_io->device_type == DEV_LOGIC_DISK) //逻辑盘符
        {
            devsta=  parm ;
            if(devsta->sta == DEV_ONLINE)
            {
                printf("\n DEV_LOGIC_DISK dev is on %c\n", devsta->hdev->dev_io->name[0]);
                if (devsta->hdev->dev_io->name[0] == 'B')
                {
                    char tmp_root_dir[] = "B:/DCIMA\0";
                    char tmp_root_dir1[] = "B:/DCIMB\0";
                    char rec_file_name_format[] = "rec?????.AVI\0";//"rec?????.mov\0";
                    char rec_file_name_format1[] = "rec?????.AVI\0";
                    char pro_file_name_format[] = "sos?????.AVI\0";//"sos?????.mov\0";
                    char pro_file_name_format1[] = "sos?????.AVI\0";
                    char jpg_file_name_format[] = "jpg?????.JPG\0";
                    set_file_format_path(tmp_root_dir,rec_file_name_format,pro_file_name_format,jpg_file_name_format,
                                         tmp_root_dir1, rec_file_name_format1, pro_file_name_format1);

                    if (!init_record_dir())
                    {
                        puts("\n\ninit record fail\n\n");
                    }
                    if (freeSpace(800, 0)==FALSE)
                    {
                        if(get_menu_statu()==0 && ((mode_ctrl.cur_mode & USB_MODE) == 0) &&(get_parking_status()== 0) ){

                            //if(app_sd_io_dect() == 0 && g_startup_finish == 1){//卡在线的情况下
                            if(g_startup_finish == 1){//卡在线的情况下 //5203 JOHNSON 20160509
                                display_note(M154);
                            }
                        }
                    }

                    g_need_format = FALSE;

                    //簇大小不符合
                    cluster_size = get_clust_size('B');
                    if ((cluster_size < 64) && (cluster_size > 0)){
                        g_need_format = TRUE;
                        if(g_startup_finish && get_menu_statu()== 0){
                            //if(app_sd_io_dect() == 0) //5203 JOHNSON 20160509
                            {//卡在线的情况下
                                display_note(M155);
                            }
                        }
                        puts("\n cluster_size err \n");
                    }

                    bdisk_ready = TRUE;
                }
            }
            else
            {
                printf("\n DEV_LOGIC_DISK dev is off %c\n", devsta->hdev->dev_io->name[0]);
                if (devsta->hdev->dev_io->name[0] == 'B')
                    bdisk_ready = FALSE;
                    g_need_format = FALSE;
            }

        }

        VFS_EXIT_MUTEX();
        return TRUE;

    }

    VFS_EXIT_MUTEX();
    return FALSE;
}
#endif

/*************************************************
Function:	s32 video_name_to_index(char * filename)
Description:	录像文件名转成序号
Input:	filename	文件名

Output:	无
Return:	>=0 	正常序号
		< 0		文件名不符合
Others: rec00005.avi 则返回5
*************************************************/

s32 video_name_to_index(char * filename)
{
    int i;
    for(i=0; i<name_fix_len; i++) //转成大写
    {
        char a;
        a = filename[i];
        if ((a>='a') && (a<='z'))
        {
            a -='a';
            a +='A';
            filename[i] = a;
        }
    }
//    printf("\n ee %s \n",filename) ;
//    printf("\n ee %s \n",pro_file_format) ;
    if ((strncmp(filename,rec_file_format,name_fix_len)==0)
        ||(strncmp(filename,rec_file_format1,name_fix_len)==0)

        ||(strncmp(filename, pro_file_format, name_fix_len)==0)

        ||(strncmp(filename,pro_file_format1,name_fix_len)==0)
        ||(strncmp(filename,pic_file_format,name_fix_len)==0)
       )
    {
        if ((strncmp(&filename[9],&rec_file_format[9],name_fix_len)==0)
            ||(strncmp(&filename[9],&pro_file_format[9],name_fix_len)==0)
            ||(strncmp(&filename[9],&pic_file_format[9],name_fix_len)==0)
           )
        {
            int index = 0;
            for(i=name_fix_len; i<8; i++)
            {
                if ((filename[i]>'9')||(filename[i]<'0'))
                    return -1;
                index*=10;
                index+=(filename[i] - '0');
            }
            return index;
        }
    }
    return -1;
}

/*************************************************
Function:	s32 femu_callback(void *fileparam, void *pram2callback)
Description:	枚举当天录像目录下录像文件回调函数
Input:	fileparam        FILE_PARAM文件信息
		pram2callback    枚举时由主程序传过来的参数，由用户自己定义
Output:	无
Return:	TRUE	结束枚举
		FALSE   结束枚举
Others: 该函数为维护存储驱动器之用，所以只处理MSG_DEVICECHANGE消息
*************************************************/
s32 femu_callback(void *fileparam, void *pram2callback)
{
    u32 emu_type;

    FILE_PARAM * curFile = (FILE_PARAM*)fileparam;

    emu_type = (u32)pram2callback;


    if (emu_type == 0)
    {
        int this_file_index = video_name_to_index((char*)curFile->fname);

        if(this_file_index != -1)
        {
             if(strncmp((char*)curFile->fname, pro_file_format, name_fix_len)==0)///Burgess_151210
            {
//                user_define.protect_file_num ++ ;
                user_define.total_protect_size += (curFile->fsize >> 20);
                printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
            }
        }
        if (current_index < this_file_index)
            current_index = this_file_index;
    }
    else if (emu_type == 1)
    {
        int this_file_index = video_name_to_index((char*)curFile->fname);

        if(this_file_index != -1)
        {
            if(strncmp((char*)curFile->fname,pro_file_format1,name_fix_len)==0)
            {
//                user_define.protect_file_num1 ++ ;
                user_define.total_protect_size += (curFile->fsize >> 20);
                printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
            }
        }
        if (current_index1 < this_file_index)
            current_index1 = this_file_index;
    }
//    if((user_define.protect_file_num*file_size(0, get_cycle_time())+user_define.protect_file_num1*file_size(1, get_cycle_time())) >
//                               ALLOCATE_40_PER_FOR_PROTECT)
//        user_define.note_protect_full = 2;
//    else if((user_define.protect_file_num*file_size(0, get_cycle_time())+user_define.protect_file_num1*file_size(1, get_cycle_time())) >
//                               ALLOCATE_30_PER_FOR_PROTECT)
//        user_define.note_protect_full = 1;
//    else user_define.note_protect_full = 0;
    if(user_define.total_protect_size >ALLOCATE_40_PER_FOR_PROTECT)
        user_define.note_protect_full = 2;
    else if(user_define.total_protect_size >ALLOCATE_30_PER_FOR_PROTECT)
        user_define.note_protect_full = 1;
    else user_define.note_protect_full = 0;

    return FALSE;
}

/************************************************
Function:	BOOL init_record_dir()
Description:	初始化录像目录
Input:	无
		无
Output:	无
Return:	TRUE:OK
		FALSE:FAIL
Others: 录像目录为年4位月日各2位，
如20140801表示2014年8月1日的录像
目录，若客户对录像的目录要求有所
不同，则在此修改即可。
*************************************************/
#include "irtc.h"
RTC_TIME rtc_time;
BOOL init_record_dir()
{
//    FILE_ENTER_MUTEX();
    if (f_chdir((u8*)root_dir)!=FR_OK)
    {
        puts("no root dir,create it, ");
        if (f_mkdir((u8*)root_dir, 0)==FR_OK)
        {
            puts("OK.\n");
            f_chdir((u8*)root_dir);
        }
        else
        {
            puts("fail,again\n");
            f_mkdir((u8*)root_dir, 0);
//            FILE_EXIT_MUTEX();
            return FALSE;
        }
    }

    if (f_chdir((u8*)root_dir1)!=FR_OK)
    {
        printf("no root dir,create it, ");
        if (f_mkdir((u8*)root_dir1,0)==FR_OK)
        {
            printf("OK.\n");
            f_chdir((u8*)root_dir1);
        }
        else
        {
            printf("fail,again\n");
            f_mkdir((u8*)root_dir1,0);
//            FILE_EXIT_MUTEX();
            return FALSE;
        }
    }
//    user_define.protect_file_num = 0;
//    user_define.protect_file_num1 = 0;
    user_define.total_protect_size = 0;
    user_define.note_protect_full = 0;

    current_index = -1;
    current_index1 = -1;
    f_emu((u8*)"B:/DCIMA\0",(void *)0,femu_callback);
    f_emu((u8*)"B:/DCIMB\0",(void *)1,femu_callback);
    current_index++;
    current_index1++;
    //bdisk_ready = TRUE;
    g_need_format = FALSE;

//    FILE_EXIT_MUTEX();
    return TRUE;
}


u32 file_size(u8 f_mode, u16 f_len)
{
    u32 f_size = 0;
    switch (f_len)
    {
    case 60:
        if (f_mode == 0)
        {
            f_size = 100;
        }
        else
        {
            f_size = 60;
        }

        break;
    case 120:

        if (f_mode == 0)
        {
            f_size = 200;
        }
        else
        {
            f_size = 120;
        }
        break;
    case 180:
        if (f_mode == 0)
        {
            f_size = 300;
        }
        else
        {
            f_size = 120;
        }
        break;
    default :
        if (f_mode == 0)
        {
            f_size = 100;
        }
        else
        {
            f_size = 60;
        }
        break;
    }



    return f_size;

}
/************************************************
Function:	file_t *create_video_file()
Description:	创建视频文件
Input:	无
		无
Output:	无
Return:	NULL:	不成功
		非NULL: 成功
Others: 按照录像文件名格式，比如rec?????.avi，顺序
生成图像文件，若名字生成规则有变动，则在此函数修改
*************************************************/
file_t *create_video_file_mov()//mov
{
    int i;
    file_t *fp;
    char video_dir[64];
    FILE_ENTER_MUTEX();
    int index = current_index;
    printf("create_video_file_mov \r\n ") ;
    //将找到的目录与更目录拼接 生成   "B:/DCIMA\0" 样式的路径
    memset(video_dir, 0x00, sizeof(video_dir));
    strcpy((char *)video_dir, root_dir);//"B:/DCIMA\0"
    int len = strlen(video_dir);
    video_dir[len] = '\0';

    static u8 flag = 0;
    if (flag == 0)
    {
        flag = 1;
        if(f_chdir((u8 *)video_dir) != FR_OK)
        {
            if(f_mkdir((u8*)video_dir,0) == FR_OK)
            {
                f_chdir((u8 *)video_dir);
                puts("mkdir ok\n");
            }
            else
            {
                puts("mkdir err\n");
            }
        }
    }

    for(i=7; i>=name_fix_len; i--)
    {
        int tmp = index /10;
        rec_file_format[i] = index - (tmp*10);
        rec_file_format[i] += '0';
        index = tmp;
    }

    video_dir[len] = '/';
    strcpy((char *)video_dir+len+1, (const char *)rec_file_format);

    fp = fopen((u8*)video_dir,openCreate);


//    u32 f_size = file_size(0,cycle_time);
    u32 f_size = file_size(0, get_cycle_time());
//    mutex_f_Seek(fp, 0, 300*1024*1024);


    if (fp != NULL)
    {
        fseek(fp, 0, f_size*1024*1024);
        fseek(fp, 0, 0);
        current_index++;
    }



    FILE_EXIT_MUTEX();
    return fp;
}
/************************************************
Function:	file_t *create_video_file()
Description:	创建视频文件
Input:	无
		无
Output:	无
Return:	NULL:	不成功
		非NULL: 成功
Others: 按照录像文件名格式，比如rec?????.avi，顺序
生成图像文件，若名字生成规则有变动，则在此函数修改
*************************************************/
file_t *create_video_file_avi()
{
    int i;
    file_t *fp;
    char video_dir[64];
FILE_ENTER_MUTEX();
printf("create_video_file_avi \r\n ") ;
    int index = current_index1;

    //将找到的目录与更目录拼接 生成   "B:/DCIM/20140906\0" 样式的路径
    memset(video_dir, 0x00, sizeof(video_dir));
    strcpy((char *)video_dir, root_dir1);//"B:/DCIMB\0"
    int len = strlen(video_dir);
    video_dir[len] = '\0';

    static u8 flag = 0;
    if (flag == 0)
    {
        flag = 1;
        if(f_chdir((u8 *)video_dir) != FR_OK)
        {
            if(f_mkdir((u8*)video_dir,0) == FR_OK)
            {
                f_chdir((u8 *)video_dir);
                puts("mkdir ok\n");
            }
            else
            {
                puts("mkdir err\n");
            }
        }
    }
//FILE_EXIT_MUTEX();



    for(i=7; i>=name_fix_len; i--)
    {
        int tmp = index /10;
        rec_file_format1[i] = index - (tmp*10);
        rec_file_format1[i] += '0';
        index = tmp;
    }

    video_dir[len] = '/';
    strcpy((char *)video_dir+len+1, (const char *)rec_file_format1);




    {
        fp = fopen((u8*)video_dir,openCreate);
    }

//    u32 f_size = file_size(1, cycle_time);
    u32 f_size = file_size(1, get_cycle_time());
//    mutex_f_Seek(fp, 0, 180*1024*1024);
    fseek(fp, 0, f_size*1024*1024);

    fseek(fp, 0, 0);
    if (fp!=NULL)
        current_index1++;

FILE_EXIT_MUTEX();
    return fp;
}
/************************************************
Function:	file_t *create_video_file()
Description:	创建图片文件
Input:	无
		无
Output:	无
Return:	NULL:	不成功
		非NULL: 成功
Others: 按照录像文件名格式，比如pic?????.jpg，顺序
生成图片文件，若名字生成规则有变动，则在此函数修改
*************************************************/
file_t *create_pic_file_data_camera()
{
    int i;
    file_t *fp;
    char video_dir[64];

    FILE_ENTER_MUTEX();
    int index = current_index;

      //将找到的目录与更目录拼接 生成   "B:/DCIMA\0" 样式的路径
    memset(video_dir, 0x00, sizeof(video_dir));
    strcpy((char *)video_dir, root_dir);//"B:/DCIMA\0"
    int len = strlen(video_dir);
    video_dir[len] = '\0';
//    video_dir[len] = '/';

    static u8 flag = 0;
    if (flag == 0)
    {
        flag = 1;
        if(f_chdir((u8 *)video_dir) != FR_OK)
        {
            if(f_mkdir((u8*)video_dir,0) == FR_OK)
            {
                f_chdir((u8 *)video_dir);
                puts("mkdir ok\n");
            }
            else
            {
                if (f_mkdir((u8*)video_dir,0) == FR_OK)
                {
                    f_chdir((u8 *)video_dir);
                    puts("mkdir ok\n");
                }
                else
                {
                    puts("mkdir err\n");
                }
            }
        }
    }



    for(i=7; i>=name_fix_len; i--)
    {
        int tmp = index /10;
        pic_file_format[i] = index - (tmp*10);
        pic_file_format[i] += '0';
        index = tmp;
    }

    video_dir[len] = '/';
    strcpy((char *)video_dir+len+1, (const char *)pic_file_format);




    {
        fp = fopen((u8*)video_dir,openCreate);
    }


    if (fp != NULL)
        current_index++;

    FILE_EXIT_MUTEX();
    return fp;
}

/************************************************
Function:	file_t *create_video_file()
Description:	创建图片文件
Input:	无
		无
Output:	无
Return:	NULL:	不成功
		非NULL: 成功
Others: 按照录像文件名格式，比如pic?????.jpg，顺序
生成图片文件，若名字生成规则有变动，则在此函数修改
*************************************************/
file_t *create_pic_file_analog_camera()
{
    int i;
    file_t *fp;
    char video_dir[64];

    FILE_ENTER_MUTEX();
    int index = current_index1;

      //将找到的目录与更目录拼接 生成   "B:/DCIMA\0" 样式的路径
    memset(video_dir, 0x00, sizeof(video_dir));
    strcpy((char *)video_dir, root_dir1);//"B:/DCIMB\0"
    int len = strlen(video_dir);
    video_dir[len] = '\0';

    static u8 flag = 0;
    if (flag == 0)
    {
        flag = 1;
        if(f_chdir((u8 *)video_dir) != FR_OK)
        {
            if(f_mkdir((u8*)video_dir,0) == FR_OK)
            {
                f_chdir((u8 *)video_dir);
                puts("mkdir ok\n");
            }
            else
            {
                if (f_mkdir((u8*)video_dir,0) == FR_OK)
                {
                    f_chdir((u8 *)video_dir);
                    puts("mkdir ok\n");
                }
                else
                {
                    puts("mkdir err\n");
                }
            }
        }
    }


    for(i=7; i>=name_fix_len; i--)
    {
        int tmp = index /10;
        pic_file_format[i] = index - (tmp*10);
        pic_file_format[i] += '0';
        index = tmp;
    }

    video_dir[len] = '/';
    strcpy((char *)video_dir+len+1, (const char *)pic_file_format);

    {
        fp = fopen((u8*)video_dir,openCreate);
    }

    if (fp != NULL)
        current_index1++;

    FILE_EXIT_MUTEX();
    return fp;
}





/*************************************************
Function:	u32 video_dir_2index(char * video_dir)
Description:	将录像目录名转成u32，高2byte为年，月和日各占1byte
Input:	video_dir        录像目录名
Output:	无
Return:	0	    目录不合规定
		其它    正常值
Others:
*************************************************/
u32 video_dir_2index(char * video_dir)
{
    u32 value = 0;

    if (strlen(video_dir)!=8)
        return value;

    value = video_dir[0] - '0';
    value *=10;
    value += video_dir[1] - '0';
    value *=10;
    value += video_dir[2] - '0';
    value *=10;
    value += video_dir[3] - '0';

    value<<=16;

    value+=(((video_dir[4] - '0')*10+(video_dir[5] - '0'))<<8);
    value+=((video_dir[6] - '0')*10+(video_dir[7] - '0'));

    return value;
}

/*************************************************
Function:	s32 femu_vdir_callback(void *fileparam, void *pram2callback)
Description:	枚举所有录像目录回调函数
Input:	fileparam        FILE_PARAM文件信息
		pram2callback    枚举时由主程序传过来的参数，由用户自己定义
Output:	无
Return:	TRUE	结束枚举
		FALSE   继续枚举
Others:
*************************************************/
char current_video_dir[13];
u32 first_video_dir_index; //最早的目录索引号
u32 now_video_dir_index;//当前目录索引号
int index_begin,index_end;
s32 femu_vdir_callback(void *fileparam, void *pram2callback)
{
    u32 emu_type;
    u32 i;
    int this_file_index;
    FILE_PARAM * curFile = (FILE_PARAM*)fileparam;

    emu_type = (u32)pram2callback;

    if (emu_type == 0)//找比指定目录晚的最早录像目录
    {
        if ((curFile->fattrib & 0x10) == 0x10)
        {
            for(i=0; i<strlen((char*)(curFile->fname)); i++)
            {
                char c = curFile->fname[i];
                if ((c>'9')||(c<'0'))
                    break;
            }
            if (i==8)
            {
                u32 this_index = video_dir_2index((char*)(curFile->fname));
                if (this_index > first_video_dir_index)
                {
                    if ((this_index<=now_video_dir_index)||(now_video_dir_index == first_video_dir_index))
                    {
                        now_video_dir_index = this_index;
                        strcpy(current_video_dir,(char*)(curFile->fname));
                    }
                }
            }
            //printf("%s\n",curFile->fname);
        }
    }
    if (emu_type == 1)//找指定目录的录像文件范围
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            this_file_index = video_name_to_index((char*)(curFile->fname));
            if (this_file_index != -1)
            {
                if (index_end < this_file_index)
                    index_end = this_file_index;
                if (index_begin > this_file_index)
                    index_begin = this_file_index;
            }
        }
    }
    else if (emu_type == 2)//找指定目录的可删除的最小文件
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            this_file_index = video_name_to_index((char*)(curFile->fname));
            if (this_file_index != -1)
            {
                if(strncmp(curFile->fname, "SOS", 3)){//不是保护文件
                    if (index_end < this_file_index)
                        index_end = this_file_index;
                    if (index_begin > this_file_index)
                        index_begin = this_file_index;
                }
            }
        }
    }

    return FALSE;
}


/*************************************************
Function:
Description:	删除root_path路径下，file_format格式的文件i
Input: i 文件索引
        file_format 文件格式
        root_dir1   文件夹

Output:
Return:	TRUE 成功
		FALSE 失败
Others:
*************************************************/
u32 delete_video_photo_file(u32 i, char *file_format, char *root_path)
{
    u32 index;
    u32 j;
    u32 ret;

    index = i;
    char path[128];

    for(j=7; j>=name_fix_len; j--)
    {
        int tmp = index /10;
        file_format[j] = index - (tmp*10);
        file_format[j] += '0';
        index = tmp;
    }
    strcpy(path,root_path);
    path[strlen(root_path)] = '/';
    strcpy(path+strlen(root_path)+1,file_format);
    ret = fdelete((u8*)path);//delete avi/mov/jpg file
    if (ret){
//        printf("\n mute_f_delete ok: %s , %s , %08x, %08x\n",path, file_format, i, name_fix_len);
    }else{
        printf("\n mute_f_delete fail: %s, %s , %08x, %08x\n",path, file_format, i, name_fix_len);
    }

    return ret;
}


/*************************************************
Function:
Description:	获取最早文件 和 最迟文件 号
Input:

Output:
Return:	TRUE
		FALSE 目录下没有录像文件了
Others:
*************************************************/
BOOL check_file_begin_end(u8 *root_path)
{
    index_begin = 99999;
    index_end = 0;
    f_emu((u8*)root_path,(void*)1,femu_vdir_callback);
    if ((index_begin == 99999) && (index_end == 0))//目录下没有录像文件了
    {
        return FALSE;
    }

    return TRUE;

}

u32 check_smallest_fnum(u8 *root_path)
{
    index_begin = 99999;
    index_end = 0;
    f_emu((u8*)root_path,(void*)2,femu_vdir_callback);
    if ((index_begin == 99999) && (index_end == 0))//目录下没有录像文件了
    {
        return FALSE;
    }

    return TRUE;

}
static u8 no_enought_space_status = FALSE;

u8 get_no_enought_space_status()
{
    return no_enought_space_status;
}

void clear_no_enought_space_status()
{
    no_enought_space_status = FALSE;
}

/*************************************************
Function:	BOOL freeSpace(u32 need_space, u32 flag)
Description:	删除最早的录像文件，以腾出指定空间
Input:	need_space 指定剩余空间，单位M

Output:	无
Return:	TRUE  成功，达到指定剩余空间
		FALSE 失败
Others:
*************************************************/
BOOL freeSpace(u32 need_space, u32 flag)
{
    u32 free_space;
    int afnum ;
    int bfnum ;
    int del_pic_succ;
    u32 del_status;

//    FILE_ENTER_MUTEX();

    free_space = get_free_space('B');

    need_space <<= 11;
    if (free_space >= need_space)
    {
//        FILE_EXIT_MUTEX();
        return TRUE;
    }

   // #ifdef GUNTHER
    extern u16 get_loop_video_status();
    if(get_loop_video_status() == FALSE)
    {
        puts("\n oooooooooooopppp\n");
        no_enought_space_status = TRUE;
        return FALSE;
    }
//    #endif


//    printf("\nkk free_space : %d\n", free_space);
    del_status = 0;
    while(free_space < need_space) //空间不够时删除最早的文件
    {
        del_status++;
        if(del_status > 3){//防止删除文件不成功时死循环
            puts("\nfreeSpace fail\n");
            return FALSE;
        }

        if( (check_file_begin_end((u8 *)root_dir) == FALSE) || (flag) )
        {
            if (check_file_begin_end((u8 *)root_dir1) == FALSE)
            {
//                FILE_EXIT_MUTEX();
                return FALSE;
            }

            if(check_smallest_fnum((u8 *)root_dir1) == TRUE){
                bfnum = index_begin;
                if(delete_video_photo_file(bfnum, rec_file_format1, root_dir1) == 0){//删除视频失败
                    if(delete_video_photo_file(bfnum, pic_file_format, root_dir1)){//删除图片成功
                        if(del_status){
                            del_status--;
                        }
                    }
                }
            }
            else
            {
               return FALSE;
            }

            free_space = get_free_space('B');
            if (free_space >= need_space)
            {
//                FILE_EXIT_MUTEX();
                return TRUE;
            }

//            for(i = index_begin; i<= index_end; i++)
//            {
//                delete_video_photo_file(i, rec_file_format1, root_dir1);
//                delete_video_photo_file(i, pic_file_format, root_dir1);
//                free_space = get_free_space('B');
//                if (free_space >= need_space)
//                {
//                    FILE_EXIT_MUTEX();
//                    return TRUE;
//                }
//            }

        }
        else
        {
            del_pic_succ = 0;
            if(check_smallest_fnum((u8 *)root_dir) == TRUE){
                afnum = index_begin;
                if(delete_video_photo_file(afnum, rec_file_format, root_dir) == 0){//删除视频失败
                    if(delete_video_photo_file(afnum, pic_file_format, root_dir)){
                        //删除图片成功
                        del_pic_succ = 1;
                    }
                }
            }
            if(check_smallest_fnum((u8 *)root_dir1) == TRUE){
                bfnum = index_begin;
                if(delete_video_photo_file(bfnum, rec_file_format1, root_dir1) == 0){//删除视频失败
                    if(delete_video_photo_file(bfnum, pic_file_format, root_dir1)){
                        //删除图片成功
                        del_pic_succ = 1;
                    }
                }
            }
            if(del_pic_succ && del_status){
                del_status--;
            }

            free_space = get_free_space('B');
            if (free_space >= need_space)
            {
//                FILE_EXIT_MUTEX();
                return TRUE;
            }
            else
            {
                if((check_smallest_fnum((u8 *)root_dir) == FALSE)
                   &&(check_smallest_fnum((u8 *)root_dir1) == FALSE))
                    return FALSE;

            }

//                for(i = index_begin; i<= index_end; i++)
//                {
//                    delete_video_photo_file(i, rec_file_format, root_dir);
//                    delete_video_photo_file(i, pic_file_format, root_dir);
//                    delete_video_photo_file(i, rec_file_format1, root_dir1);
//                    delete_video_photo_file(i, pic_file_format, root_dir1);
//                    free_space = get_free_space('B');
//                    if (free_space >= need_space)
//                    {
//                        FILE_EXIT_MUTEX();
//                        return TRUE;
//                    }
//                }

        }

        free_space =  get_free_space('B');
        if (free_space >= need_space)
        {
//            FILE_EXIT_MUTEX();
            return TRUE;
        }
    }
//    FILE_EXIT_MUTEX();
    return TRUE;



}








