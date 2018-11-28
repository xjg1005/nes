

/*************************************************************
File: FileSelecter.c
Author:xuhuajun
Discriptor:
    �ļ�ѡ��������ʵ��
Version:1.0
Date��2014.8
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
u32 g_need_format;//��ʽ����־


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
        if (bdisk_cnt > 40){//����ȴ�tf���ߣ����TF����ʱ������,��tf�������ر����������ӵ� 50\60\70
            bdisk_cnt = 0;
            return FALSE;//��ʱ�˳�
        }
    }

    return TRUE;
}



/*************************************************
Function:	u32 Load_vfs(int max_open_file)
Description:	�����ļ�ϵͳ��ʼ��
Input:	max_open_file       ���ͬʱ�򿪵��ļ���
Output:	��
Return:	-1      ���ɹ�
        >=0     ��FS��ID
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
Description:	����¼��Ŀ¼����¼���ļ����������ļ�����ʽ
Input:	rootdir       Ŀ¼��Ŀ¼��������DCIM
		rec_format    ¼���ļ�����ʽ������rec?????.avi�����ȱ�����8.3��'��'�Ų����ɳ����Զ�����š�
		sos_format    ����¼���ļ�����ʽ������sos?????.avi�����ȱ�����8.3��'��'�Ų����ɳ����Զ�����š�
		pic_format    ͼƬ�ļ�����ʽ��ͬ��
Output:	��
Return:	��
        ��
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

    for(i=0; i<name_fix_len; i++) //ת�ɴ�д
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
Description:	�ļ�ѡ������Ϣ������
Input:	msg       ��Ϣ��
		*param    ��Ϣ����
Output:	��
Return:	TRUE	��Ϣ�Ѵ���
		FALSE   ��Ϣδ����
Others: �ú���Ϊά���洢������֮�ã�����ֻ����MSG_DEVICECHANGE��Ϣ
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
        //case MSG_DEVICECHANGE :		//�豸�仯
        devsta =  parm ;
        puts(devsta->hdev->dev_io->name);
        if (devsta->hdev->dev_io->device_type < DEV_LOGIC_DISK) //�����豸
        {
            if(devsta->sta == DEV_ONLINE)
            {
                //���ش洢�豸�������ļ�ϵͳ���������㲥�̷������
                printf("\nonline %x \r\n",(unsigned int)devsta->hdev);
                device_on_off(devsta->hdev,TRUE);

            }
            else
            {
                //ж�ش洢�豸��ж���ļ�ϵͳ���㲥�̷��γ���
                printf("\n\n\n----------------------offline\r\n\n\n\n");
                device_on_off(devsta->hdev,FALSE);

            }

        }
        else if (devsta->hdev->dev_io->device_type == DEV_LOGIC_DISK) //�߼��̷�
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

                            //if(app_sd_io_dect() == 0 && g_startup_finish == 1){//�����ߵ������
                            if(g_startup_finish == 1){//�����ߵ������ //5203 JOHNSON 20160509
                                display_note(M154);
                            }
                        }
                    }

                    g_need_format = FALSE;

                    //�ش�С������
                    cluster_size = get_clust_size('B');
                    if ((cluster_size < 64) && (cluster_size > 0)){
                        g_need_format = TRUE;
                        if(g_startup_finish && get_menu_statu()== 0){
                            //if(app_sd_io_dect() == 0) //5203 JOHNSON 20160509
                            {//�����ߵ������
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
Description:	¼���ļ���ת�����
Input:	filename	�ļ���

Output:	��
Return:	>=0 	�������
		< 0		�ļ���������
Others: rec00005.avi �򷵻�5
*************************************************/

s32 video_name_to_index(char * filename)
{
    int i;
    for(i=0; i<name_fix_len; i++) //ת�ɴ�д
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
Description:	ö�ٵ���¼��Ŀ¼��¼���ļ��ص�����
Input:	fileparam        FILE_PARAM�ļ���Ϣ
		pram2callback    ö��ʱ�������򴫹����Ĳ��������û��Լ�����
Output:	��
Return:	TRUE	����ö��
		FALSE   ����ö��
Others: �ú���Ϊά���洢������֮�ã�����ֻ����MSG_DEVICECHANGE��Ϣ
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
Description:	��ʼ��¼��Ŀ¼
Input:	��
		��
Output:	��
Return:	TRUE:OK
		FALSE:FAIL
Others: ¼��Ŀ¼Ϊ��4λ���ո�2λ��
��20140801��ʾ2014��8��1�յ�¼��
Ŀ¼�����ͻ���¼���Ŀ¼Ҫ������
��ͬ�����ڴ��޸ļ��ɡ�
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
Description:	������Ƶ�ļ�
Input:	��
		��
Output:	��
Return:	NULL:	���ɹ�
		��NULL: �ɹ�
Others: ����¼���ļ�����ʽ������rec?????.avi��˳��
����ͼ���ļ������������ɹ����б䶯�����ڴ˺����޸�
*************************************************/
file_t *create_video_file_mov()//mov
{
    int i;
    file_t *fp;
    char video_dir[64];
    FILE_ENTER_MUTEX();
    int index = current_index;
    printf("create_video_file_mov \r\n ") ;
    //���ҵ���Ŀ¼���Ŀ¼ƴ�� ����   "B:/DCIMA\0" ��ʽ��·��
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
Description:	������Ƶ�ļ�
Input:	��
		��
Output:	��
Return:	NULL:	���ɹ�
		��NULL: �ɹ�
Others: ����¼���ļ�����ʽ������rec?????.avi��˳��
����ͼ���ļ������������ɹ����б䶯�����ڴ˺����޸�
*************************************************/
file_t *create_video_file_avi()
{
    int i;
    file_t *fp;
    char video_dir[64];
FILE_ENTER_MUTEX();
printf("create_video_file_avi \r\n ") ;
    int index = current_index1;

    //���ҵ���Ŀ¼���Ŀ¼ƴ�� ����   "B:/DCIM/20140906\0" ��ʽ��·��
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
Description:	����ͼƬ�ļ�
Input:	��
		��
Output:	��
Return:	NULL:	���ɹ�
		��NULL: �ɹ�
Others: ����¼���ļ�����ʽ������pic?????.jpg��˳��
����ͼƬ�ļ������������ɹ����б䶯�����ڴ˺����޸�
*************************************************/
file_t *create_pic_file_data_camera()
{
    int i;
    file_t *fp;
    char video_dir[64];

    FILE_ENTER_MUTEX();
    int index = current_index;

      //���ҵ���Ŀ¼���Ŀ¼ƴ�� ����   "B:/DCIMA\0" ��ʽ��·��
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
Description:	����ͼƬ�ļ�
Input:	��
		��
Output:	��
Return:	NULL:	���ɹ�
		��NULL: �ɹ�
Others: ����¼���ļ�����ʽ������pic?????.jpg��˳��
����ͼƬ�ļ������������ɹ����б䶯�����ڴ˺����޸�
*************************************************/
file_t *create_pic_file_analog_camera()
{
    int i;
    file_t *fp;
    char video_dir[64];

    FILE_ENTER_MUTEX();
    int index = current_index1;

      //���ҵ���Ŀ¼���Ŀ¼ƴ�� ����   "B:/DCIMA\0" ��ʽ��·��
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
Description:	��¼��Ŀ¼��ת��u32����2byteΪ�꣬�º��ո�ռ1byte
Input:	video_dir        ¼��Ŀ¼��
Output:	��
Return:	0	    Ŀ¼���Ϲ涨
		����    ����ֵ
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
Description:	ö������¼��Ŀ¼�ص�����
Input:	fileparam        FILE_PARAM�ļ���Ϣ
		pram2callback    ö��ʱ�������򴫹����Ĳ��������û��Լ�����
Output:	��
Return:	TRUE	����ö��
		FALSE   ����ö��
Others:
*************************************************/
char current_video_dir[13];
u32 first_video_dir_index; //�����Ŀ¼������
u32 now_video_dir_index;//��ǰĿ¼������
int index_begin,index_end;
s32 femu_vdir_callback(void *fileparam, void *pram2callback)
{
    u32 emu_type;
    u32 i;
    int this_file_index;
    FILE_PARAM * curFile = (FILE_PARAM*)fileparam;

    emu_type = (u32)pram2callback;

    if (emu_type == 0)//�ұ�ָ��Ŀ¼�������¼��Ŀ¼
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
    if (emu_type == 1)//��ָ��Ŀ¼��¼���ļ���Χ
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
    else if (emu_type == 2)//��ָ��Ŀ¼�Ŀ�ɾ������С�ļ�
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            this_file_index = video_name_to_index((char*)(curFile->fname));
            if (this_file_index != -1)
            {
                if(strncmp(curFile->fname, "SOS", 3)){//���Ǳ����ļ�
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
Description:	ɾ��root_path·���£�file_format��ʽ���ļ�i
Input: i �ļ�����
        file_format �ļ���ʽ
        root_dir1   �ļ���

Output:
Return:	TRUE �ɹ�
		FALSE ʧ��
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
Description:	��ȡ�����ļ� �� ����ļ� ��
Input:

Output:
Return:	TRUE
		FALSE Ŀ¼��û��¼���ļ���
Others:
*************************************************/
BOOL check_file_begin_end(u8 *root_path)
{
    index_begin = 99999;
    index_end = 0;
    f_emu((u8*)root_path,(void*)1,femu_vdir_callback);
    if ((index_begin == 99999) && (index_end == 0))//Ŀ¼��û��¼���ļ���
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
    if ((index_begin == 99999) && (index_end == 0))//Ŀ¼��û��¼���ļ���
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
Description:	ɾ�������¼���ļ������ڳ�ָ���ռ�
Input:	need_space ָ��ʣ��ռ䣬��λM

Output:	��
Return:	TRUE  �ɹ����ﵽָ��ʣ��ռ�
		FALSE ʧ��
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
    while(free_space < need_space) //�ռ䲻��ʱɾ��������ļ�
    {
        del_status++;
        if(del_status > 3){//��ֹɾ���ļ����ɹ�ʱ��ѭ��
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
                if(delete_video_photo_file(bfnum, rec_file_format1, root_dir1) == 0){//ɾ����Ƶʧ��
                    if(delete_video_photo_file(bfnum, pic_file_format, root_dir1)){//ɾ��ͼƬ�ɹ�
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
                if(delete_video_photo_file(afnum, rec_file_format, root_dir) == 0){//ɾ����Ƶʧ��
                    if(delete_video_photo_file(afnum, pic_file_format, root_dir)){
                        //ɾ��ͼƬ�ɹ�
                        del_pic_succ = 1;
                    }
                }
            }
            if(check_smallest_fnum((u8 *)root_dir1) == TRUE){
                bfnum = index_begin;
                if(delete_video_photo_file(bfnum, rec_file_format1, root_dir1) == 0){//ɾ����Ƶʧ��
                    if(delete_video_photo_file(bfnum, pic_file_format, root_dir1)){
                        //ɾ��ͼƬ�ɹ�
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








