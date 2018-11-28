
#include "FileSelecter.h"
#include "app_cfg.h"
#include "my_ui.h"




char dec_video_name_formatA[] = "rec?????.avi\0";
char dec_photo_name_format[] = "JPG?????.JPG\0";
char dec_protect_video_file_formatA[] = "sos?????.avi\0";


pb_fctrl_t pb_fctrl;
u32 video_index_end, video_index_begin;


//函数声明
s32 dec_femu_cb_for_A(void *fileparam, void *pram2callback);
s32 dec_femu_cb_for_B(void *fileparam, void *pram2callback);


u8 check_all_files(void)//检查是否有文件
{

    video_index_end = 0;
    video_index_begin = 99999;
    pb_fctrl.foladerA_file_num = 0;
    pb_fctrl.fldA_pro_fnum = 0;
    f_emu((u8 *)root_dir, (void *)2, dec_femu_cb_for_A);//查找 "B:/DCIMA\0"内文件范围
    if(video_index_begin > video_index_end)
    {
        //pb_fctrl.foladerA_file_num = 0;
        pb_fctrl.foladerA_startFile_index = 0;
        pb_fctrl.foladerA_endFile_index = 0;
        pb_fctrl.cur_findex_A = 0;

    }
    else
    {
        //pb_fctrl.foladerA_file_num = video_index_end - video_index_begin +1;
        pb_fctrl.foladerA_startFile_index = video_index_begin;
        pb_fctrl.foladerA_endFile_index = video_index_end;
        pb_fctrl.cur_findex_A = video_index_end;
    }

    video_index_end = 0;
    video_index_begin = 99999;
    pb_fctrl.foladerB_file_num = 0;
    pb_fctrl.fldB_pro_fnum = 0;
    f_emu((u8 *)root_dir1, (void *)2, dec_femu_cb_for_B);//查找 "B:/DCIMB\0"内文件范围
    if(video_index_begin > video_index_end)
    {
        //pb_fctrl.foladerB_file_num = 0;
        pb_fctrl.foladerB_startFile_index = 0;
        pb_fctrl.foladerB_endFile_index = 0;
        pb_fctrl.cur_findex_B = 0;
    }
    else
    {
        //pb_fctrl.foladerB_file_num = video_index_end - video_index_begin +1;
        pb_fctrl.foladerB_startFile_index = video_index_begin;
        pb_fctrl.foladerB_endFile_index = video_index_end;
        pb_fctrl.cur_findex_B = video_index_end;
    }

    if(pb_fctrl.foladerA_file_num == 0){
        if(pb_fctrl.foladerB_file_num == 0){
            return 1;
        }
        pb_fctrl.current_folder = 1;
    }

    return 0;//have file
}


s32 dec_femu_cb_for_A(void *fileparam, void *pram2callback)
{
    static u32 a_index;
    s32 this_index;
    u32 emu_type;
    FILE_PARAM * curFile = (FILE_PARAM*)fileparam;
    emu_type = (u32)pram2callback;


    if (emu_type == 0)//找比指定文件序号小的最大视频文件号
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            this_index = video_name_to_index((char*)(curFile->fname));
            if( (this_index != -1) && (this_index <= pb_fctrl.foladerA_endFile_index) )
            {
                if(pb_fctrl.search_flag == 0)
                {
                    if(pb_fctrl.cur_findex_A == pb_fctrl.foladerA_startFile_index){
                        pb_fctrl.cur_findex_A = pb_fctrl.foladerA_endFile_index;
                        return TRUE;
                    }
                }

                if(pb_fctrl.search_flag == 0)
                {
                    if(pb_fctrl.cur_findex_A > this_index){
                        a_index = pb_fctrl.cur_findex_A;
                        pb_fctrl.cur_findex_A = this_index;
                        pb_fctrl.search_flag = 1;
                    }
                }
                else
                {
                    if( (this_index > pb_fctrl.cur_findex_A) && (this_index < a_index) ){
                        pb_fctrl.cur_findex_A = this_index;
                    }
                }
            }
        }
    }
    else if (emu_type == 1)//找比指定文件序号大的最小视频文件序号
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            this_index = video_name_to_index((char*)(curFile->fname));
            if( (this_index != -1) && (this_index <= pb_fctrl.foladerA_endFile_index) )
            {
                if(pb_fctrl.search_flag == 0)
                {
                    if(pb_fctrl.cur_findex_A == pb_fctrl.foladerA_endFile_index){
                        pb_fctrl.cur_findex_A = pb_fctrl.foladerA_startFile_index;
                        return TRUE;
                    }
                }

                if(pb_fctrl.search_flag == 0)
                {
                    if(pb_fctrl.cur_findex_A < this_index){
                        a_index = pb_fctrl.cur_findex_A;
                        pb_fctrl.cur_findex_A = this_index;
                        pb_fctrl.search_flag = 1;
                    }
                }
                else
                {
                    if((pb_fctrl.cur_findex_A > this_index) && (this_index > a_index)){
                        pb_fctrl.cur_findex_A = this_index;
                    }
                }
            }
        }
    }

    else if (emu_type == 2)//找视频文件范围
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            this_index = video_name_to_index((char*)curFile->fname);
            if (this_index != -1)
            {
                pb_fctrl.foladerA_file_num++;

                if(strncmp(curFile->fname, "SOS", 3) == 0){
                    pb_fctrl.fldA_pro_fnum++;
                }

                if (video_index_end < this_index){
                    video_index_end = this_index;
                }

                if (video_index_begin > this_index){
                    video_index_begin = this_index;
                }
            }

        }

    }

    return FALSE;
}


s32 dec_femu_cb_for_B(void *fileparam, void *pram2callback)
{
    static u32 b_index;
    s32 this_index;
    u32 emu_type;
    FILE_PARAM * curFile = (FILE_PARAM*)fileparam;
    emu_type = (u32)pram2callback;


    if (emu_type == 0)//找比指定文件序号小的最大视频文件号
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            this_index = video_name_to_index((char*)(curFile->fname));
            if( (this_index != -1) && (this_index <= pb_fctrl.foladerB_endFile_index) )
            {
                if(pb_fctrl.search_flag == 0)
                {
                    if(pb_fctrl.cur_findex_B == pb_fctrl.foladerB_startFile_index){
                        pb_fctrl.cur_findex_B = pb_fctrl.foladerB_endFile_index;
                        return TRUE;
                    }
                }

                if(pb_fctrl.search_flag == 0)
                {
                    if(pb_fctrl.cur_findex_B > this_index){
                        b_index = pb_fctrl.cur_findex_B;
                        pb_fctrl.cur_findex_B = this_index;
                        pb_fctrl.search_flag = 1;
                    }
                }
                else
                {
                    if( (this_index > pb_fctrl.cur_findex_B) && (this_index < b_index) ){
                        pb_fctrl.cur_findex_B = this_index;
                    }
                }
            }
        }
    }
    else if (emu_type == 1)//找比指定文件序号大的最小视频文件序号
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            this_index = video_name_to_index((char*)(curFile->fname));
            if( (this_index != -1) && (this_index <= pb_fctrl.foladerB_endFile_index) )
            {
                if(pb_fctrl.search_flag == 0)
                {
                    if(pb_fctrl.cur_findex_B == pb_fctrl.foladerB_endFile_index){
                        pb_fctrl.cur_findex_B = pb_fctrl.foladerB_startFile_index;
                        return TRUE;
                    }
                }

                if(pb_fctrl.search_flag == 0)
                {
                    if(pb_fctrl.cur_findex_B < this_index){
                        b_index = pb_fctrl.cur_findex_B;
                        pb_fctrl.cur_findex_B = this_index;
                        pb_fctrl.search_flag = 1;
                    }
                }
                else
                {
                    if((pb_fctrl.cur_findex_B > this_index) && (this_index > b_index)){
                        pb_fctrl.cur_findex_B = this_index;
                    }
                }
            }
        }
    }

    else if (emu_type == 2)//找视频文件范围
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            this_index = video_name_to_index((char*)curFile->fname);
            if (this_index != -1)
            {
                pb_fctrl.foladerB_file_num++;

                if(strncmp(curFile->fname, "SOS", 3) == 0){
                    pb_fctrl.fldB_pro_fnum++;
                }

                if (video_index_end < this_index){
                    video_index_end = this_index;
                }

                if (video_index_begin > this_index){
                    video_index_begin = this_index;
                }
            }
        }

    }

    return FALSE;
}




void get_folader_status(void)
{
    pb_fctrl.current_folder = 0;
    pb_fctrl.no_file = check_all_files();//检查是否有文件
}


void dec_video_index_to_name(u32 index, char *video_format_to_decode,const u8 *video_dir)
{
    u32 tmp, i;
    u32 len = 0;

    for(i=7; i >= 3; i--)
    {
        tmp = index /10;
        video_format_to_decode[i] = index - (tmp*10);
        video_format_to_decode[i] += '0';
        index = tmp;
    }

    for (i = 8; i < strlen(video_format_to_decode); i++)//大写转小写
    {
        if ((video_format_to_decode[i] > 'A') && (video_format_to_decode[i] < 'Z'))
        {
            video_format_to_decode[i] += 32;
        }
    }

    strcpy((char *)video_path, (const char *)video_dir);
    len = strlen((const char *)video_path);
    video_path[len] = '/';
    strcpy((char *)video_path+len+1, (const char *)video_format_to_decode);

}

//search_mode : 0 上一个 ,1 下一个
void * select_file(u8 search_mode)
{
    void * fp;

    puts("\nsearch : ");
    put_u32hex(search_mode);

    if( (search_mode == 0) || (search_mode == 1) )
    {
        if((pb_fctrl.current_folder == 0) && pb_fctrl.foladerA_file_num)
        {

            pb_fctrl.cur_dec_folder = 0;

            pb_fctrl.search_flag = 0;

            f_emu((u8 *)root_dir, (void *)search_mode, dec_femu_cb_for_A);

            dec_video_index_to_name(pb_fctrl.cur_findex_A,dec_video_name_formatA,root_dir);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 0;
                pb_fctrl.ftype = PKG_TYPE_AVI;
                goto _succ;
            }

            dec_video_index_to_name(pb_fctrl.cur_findex_A,dec_protect_video_file_formatA,root_dir);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 1;
                pb_fctrl.ftype = PKG_TYPE_AVI;
                goto _succ;
            }

            dec_video_index_to_name(pb_fctrl.cur_findex_A,dec_photo_name_format,root_dir);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 0;
                pb_fctrl.ftype = PKG_TYPE_JPG;
                goto _succ;
            }
        }

        if((pb_fctrl.current_folder == 1) && pb_fctrl.foladerB_file_num)
        {
            pb_fctrl.cur_dec_folder = 1;

            pb_fctrl.search_flag = 0;
            f_emu((u8 *)root_dir1, (void *)search_mode, dec_femu_cb_for_B);

            dec_video_index_to_name(pb_fctrl.cur_findex_B,dec_video_name_formatA,root_dir1);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 0;
                pb_fctrl.ftype = PKG_TYPE_AVI;
                goto _succ;
            }

            dec_video_index_to_name(pb_fctrl.cur_findex_B,dec_protect_video_file_formatA,root_dir1);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 1;
                pb_fctrl.ftype = PKG_TYPE_AVI;
                goto _succ;
            }

            dec_video_index_to_name(pb_fctrl.cur_findex_B,dec_photo_name_format,root_dir1);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 0;
                pb_fctrl.ftype = PKG_TYPE_JPG;
                goto _succ;
            }
        }
    }
    else
    {
        if((pb_fctrl.current_folder == 0) && pb_fctrl.foladerA_file_num)
        {
            pb_fctrl.cur_dec_folder = 0;

            dec_video_index_to_name(pb_fctrl.cur_findex_A,dec_video_name_formatA,root_dir);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 0;
                pb_fctrl.ftype = PKG_TYPE_AVI;
                goto _succ;
            }

            dec_video_index_to_name(pb_fctrl.cur_findex_A,dec_protect_video_file_formatA,root_dir);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 1;
                pb_fctrl.ftype = PKG_TYPE_AVI;
                goto _succ;
            }

            dec_video_index_to_name(pb_fctrl.cur_findex_A,dec_photo_name_format,root_dir);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 0;
                pb_fctrl.ftype = PKG_TYPE_JPG;
                goto _succ;
            }
        }

        if((pb_fctrl.current_folder == 1) && pb_fctrl.foladerB_file_num)
        {
            pb_fctrl.cur_dec_folder = 1;

            dec_video_index_to_name(pb_fctrl.cur_findex_B,dec_video_name_formatA,root_dir1);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 0;
                pb_fctrl.ftype = PKG_TYPE_AVI;
                goto _succ;
            }

            dec_video_index_to_name(pb_fctrl.cur_findex_B,dec_protect_video_file_formatA,root_dir1);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 1;
                pb_fctrl.ftype = PKG_TYPE_AVI;
                goto _succ;
            }

            dec_video_index_to_name(pb_fctrl.cur_findex_B,dec_photo_name_format,root_dir1);
            fp = mutex_f_Open((u8 *)video_path,openReadOnly);
            if (fp != NULL){
                pb_fctrl.file_protect = 0;
                pb_fctrl.ftype = PKG_TYPE_JPG;
                goto _succ;
            }
        }
    }


    if(pb_fctrl.current_folder == 1){
        if(pb_fctrl.foladerB_file_num == 0){
            pb_fctrl.current_folder = !pb_fctrl.current_folder;
        }
    }else if(pb_fctrl.current_folder == 0){
        if(pb_fctrl.foladerA_file_num == 0){
            pb_fctrl.current_folder = !pb_fctrl.current_folder;
        }
    }

    return NULL;

_succ:

    pb_fctrl.current_folder = !pb_fctrl.current_folder;

    return fp;
}

//文件属性
u32 dec_file_pro()
{
    return pb_fctrl.file_protect;
}

u32 dec_only_profile()
{
    u32 total_num;
    u32 pro_fnum;

    total_num = pb_fctrl.foladerA_file_num + pb_fctrl.foladerB_file_num ;
    pro_fnum = pb_fctrl.fldA_pro_fnum + pb_fctrl.fldB_pro_fnum ;

//    puts("\n\n in \n\n");
//    put_u32hex(pb_fctrl.foladerA_file_num);
//    put_u32hex(pb_fctrl.foladerB_file_num);
//    put_u32hex(pb_fctrl.fldA_pro_fnum);
//    put_u32hex(pb_fctrl.fldB_pro_fnum);
//    puts("\n\n out \n\n");

    if(total_num <= pro_fnum ){
        return 1;
    }

    return 0;
}

u32 get_pro_fnum(void)
{
   u32 pro_fnum;
   pro_fnum = pb_fctrl.fldA_pro_fnum + pb_fctrl.fldB_pro_fnum ;
   return pro_fnum;
}
s32 dec_del_file()
{
    char * tmp_root_dir;
    s32 tmp_index;
    s32 ret = 0;


    if(pb_fctrl.file_protect){//保护文件，不能删除
        return 1;
    }


    if(last_file_hdl){
        mutex_f_Close(last_file_hdl);
        last_file_hdl = NULL;
    }


    if(pb_fctrl.cur_dec_folder == 0){
        tmp_index = pb_fctrl.cur_findex_A;
        tmp_root_dir = root_dir;
    }else{
        tmp_index = pb_fctrl.cur_findex_B;
        tmp_root_dir = root_dir1;
    }


    if(pb_fctrl.ftype == PKG_TYPE_AVI){
        if(delete_video_photo_file(tmp_index,dec_video_name_formatA,tmp_root_dir) == FALSE){
            return -1;
        }
    }else{
        if(delete_video_photo_file(tmp_index,dec_photo_name_format,tmp_root_dir) == FALSE){
            return -1;
        }
    }


    if(pb_fctrl.cur_dec_folder == 0)
    {
//        if( (pb_fctrl.cur_findex_A == pb_fctrl.foladerA_startFile_index) ||
//           (pb_fctrl.cur_findex_A == pb_fctrl.foladerA_endFile_index))
        {
            video_index_end = 0;
            video_index_begin = 99999;
            pb_fctrl.foladerA_file_num = 0;
            pb_fctrl.fldA_pro_fnum = 0;
            f_emu((u8 *)root_dir, (void *)2, dec_femu_cb_for_A);//查找 "B:/DCIMA\0"内文件范围
            if(video_index_begin > video_index_end)
            {
                pb_fctrl.foladerA_startFile_index = 0;
                pb_fctrl.foladerA_endFile_index = 0;
                pb_fctrl.cur_findex_A = 0;
            }
            else
            {
//                if(pb_fctrl.cur_findex_A == pb_fctrl.foladerA_endFile_index){
                if(pb_fctrl.cur_findex_A >= pb_fctrl.foladerA_endFile_index){
                    pb_fctrl.cur_findex_A = video_index_end;
                }else {
//                    pb_fctrl.cur_findex_A = video_index_begin;
                    if(pb_fctrl.cur_findex_A)
                        pb_fctrl.cur_findex_A--;
                }
                pb_fctrl.foladerA_startFile_index = video_index_begin;
                pb_fctrl.foladerA_endFile_index = video_index_end;
            }
        }
    }
    else
    {
//        if( (pb_fctrl.cur_findex_B == pb_fctrl.foladerB_startFile_index) ||
//           (pb_fctrl.cur_findex_B == pb_fctrl.foladerB_endFile_index))
        {
            video_index_end = 0;
            video_index_begin = 99999;
            pb_fctrl.foladerB_file_num = 0;
            pb_fctrl.fldB_pro_fnum = 0;
            f_emu((u8 *)root_dir1, (void *)2, dec_femu_cb_for_B);//查找 "B:/DCIMB\0"内文件范围
            if(video_index_begin > video_index_end)
            {
                pb_fctrl.foladerB_startFile_index = 0;
                pb_fctrl.foladerB_endFile_index = 0;
                pb_fctrl.cur_findex_B = 0;
            }
            else
            {
//                if(pb_fctrl.cur_findex_B == pb_fctrl.foladerB_endFile_index){
                if(pb_fctrl.cur_findex_B >= pb_fctrl.foladerB_endFile_index){
                    pb_fctrl.cur_findex_B = video_index_end;
                }else{
//                    pb_fctrl.cur_findex_B = video_index_begin;
                    if(pb_fctrl.cur_findex_B)// = video_index_begin;
                        pb_fctrl.cur_findex_B--;
                }
                pb_fctrl.foladerB_startFile_index = video_index_begin;
                pb_fctrl.foladerB_endFile_index = video_index_end;
            }
        }
    }
    return ret;

}



/*************************************************
Function:	BOOL protect_current_file()
Description:	将刚保存的录像文件转为保护文件
Input:	无

Output:	无
Return:	TRUE  成功
		FALSE 失败
Others: 其实就是将rec00005.avi 改成sos00005.avi;如果
保护方式有变化，则在此函数修改
*************************************************/
BOOL protect_current_file( u8 type )
{
    puts("\n protect_current_file \n");
    u8 ret;
  char soc_file_format[64];
  if(type == 0) //MOV
  {
    dec_video_index_to_name((current_index-1),dec_video_name_formatA,root_dir);//生成完整路径
  }
  else if(type == 1) //avi
  {
    dec_video_index_to_name((current_index1-1),dec_video_name_formatA,root_dir1);//生成完整路径
  }
    strcpy(soc_file_format,(const char *)video_path);
    soc_file_format[9] = 'S';
    soc_file_format[10] = 'O';
    soc_file_format[11] = 'S';
//    printf("video_path %s \n",video_path);
//    printf("video_path %s \n",soc_file_format);
    ret = mute_f_rename((u8*)video_path,(u8*)soc_file_format);
    printf("ret ======%d\n",ret);
    if(ret == 0)
    {
        return FALSE;
    }
    else
    {
        video_path[9] = 'S';
        video_path[10] = 'O';
        video_path[11] = 'S';
        return TRUE;
    }

}




u32 lock_current_file( void )
{
  char soc_file_format[64];
    u32 err;
//  printf("pb_fctrl.file_in_which_folder = %d\n",pb_fctrl.file_in_which_folder);

    strcpy(soc_file_format,(const char *)video_path);
    soc_file_format[9] = 'S';
    soc_file_format[10] = 'O';
    soc_file_format[11] = 'S';

   err =  mute_f_rename((u8*)video_path,(u8*)soc_file_format);
    video_path[9] = 'S';
    video_path[10] = 'O';
    video_path[11] = 'S';
    return err;
}

u32 unlock_current_file( void )
{
  char rec_file_format[64];
    u32 err;
//  printf("pb_fctrl.file_in_which_folder = %d\n",pb_fctrl.file_in_which_folder);

    strcpy(rec_file_format,(const char *)video_path);
    rec_file_format[9] = 'R';
    rec_file_format[10] = 'E';
    rec_file_format[11] = 'C';
//    printf("video_path %s \n",video_path);
//    printf("rec_file_format %s \n",rec_file_format);

    err = mute_f_rename((u8*)video_path,(u8*)rec_file_format);
    video_path[9] = 'R';
    video_path[10] = 'E';
    video_path[11] = 'C';
    return err;

}
s32 enc_femu_cb_for_A(void *fileparam, void *pram2callback)
{
    static u32 a_index;
    s32 this_index;
    u32 emu_type;
    FILE_PARAM * curFile = (FILE_PARAM*)fileparam;
    emu_type = (u32)pram2callback;

    if (emu_type == 2)//查找目录a所有视频文件，并将sos文件重新命名为rec文件
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            //得到一个sos文件
            if(strncmp(curFile->fname, "SOS", 3) == 0)
            {

            //将文件路径拼接完整
                char video_path[64];
                u16 len;
                strcpy((char *)video_path, (const char *)root_dir);
                len = strlen((const char *)video_path);
                video_path[len] = '/';
                strcpy((char *)video_path+len+1, (const char *)curFile->fname);
                printf("\n a org : %s\n",video_path);
            //改名操作
                char rec_file_format[64];
                strcpy(rec_file_format,(const char *)video_path);
                rec_file_format[9] = 'R';
                rec_file_format[10] = 'E';
                rec_file_format[11] = 'C';

                mute_f_rename((u8*)video_path,(u8*)rec_file_format);
                video_path[9] = 'R';
                video_path[10] = 'E';
                video_path[11] = 'C';
                printf("\n a tar : %s\n",rec_file_format);
            }
        }
    }

    return FALSE;
}

s32 enc_femu_cb_for_B(void *fileparam, void *pram2callback)
{
    static u32 a_index;
    s32 this_index;
    u32 emu_type;
    FILE_PARAM * curFile = (FILE_PARAM*)fileparam;
    emu_type = (u32)pram2callback;

    if (emu_type == 2)//查找目录B所有视频文件，并将sos文件重新命名为rec文件
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {
            //得到一个sos文件
            if(strncmp(curFile->fname, "SOS", 3) == 0)
            {

            //将文件路径拼接完整
                char video_path[64];
                u16 len;
                strcpy((char *)video_path, (const char *)root_dir1);
                len = strlen((const char *)video_path);
                video_path[len] = '/';
                strcpy((char *)video_path+len+1, (const char *)curFile->fname);
                printf("\n b org : %s\n",video_path);
            //改名操作
                char rec_file_format[64];
                strcpy(rec_file_format,(const char *)video_path);
                rec_file_format[9] = 'R';
                rec_file_format[10] = 'E';
                rec_file_format[11] = 'C';

                mute_f_rename((u8*)video_path,(u8*)rec_file_format);
                video_path[9] = 'R';
                video_path[10] = 'E';
                video_path[11] = 'C';
                printf("\n b tar : %s\n",rec_file_format);
            }
        }
    }

    return FALSE;
}
void unlock_all_file()
{
//    puts("\n fffffffffffffff 1\n");
    f_emu((u8 *)root_dir, (void *)2, enc_femu_cb_for_A);//查找 "B:/DCIMA\0"内文件,并将所有sos文件改名为rec文件
    f_emu((u8 *)root_dir1, (void *)2, enc_femu_cb_for_B);//查找 "B:/DCIMA\0"内文件,并将所有sos文件改名为rec文件
//    user_define.protect_file_num = 0;
//    user_define.protect_file_num1 = 0;
    user_define.total_protect_size = 0;
    user_define.note_protect_full = 0;
}



