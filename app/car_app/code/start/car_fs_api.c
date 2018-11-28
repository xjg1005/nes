#include "car_fs_api.h"





//函数声明
extern file_t *create_video_file();

static u8 g_last_search_mode = 0;
void * last_file_hdl = NULL;
static u8 g_file_path[125];



void * read_file(u8 search_mode, u8 * file_type)
{
//    u8 file[32];
	file_t * file_hdl = NULL;
    static  u32 fid = 0 ;
    u8 pkg_type;


    if(last_file_hdl){
        mutex_f_Close(last_file_hdl);
        last_file_hdl = NULL;
    }


#if 0
    if(search_mode == FS_NEXT_FILE){
        fid++;
    }else if(search_mode == FS_PREV_FILE){
        if(fid){
            fid--;
        }
    }else{
        fid = 0;
    }

    sprintf(file,"B:/%08d.avi",fid) ;
    pkg_type = PKG_TYPE_AVI;

    printf("create file name = %s \r\n",file) ;
    file_hdl = mutex_f_Open(file, openReadOnly);
	if (!file_hdl){
        printf("f_open_faild: %d, %s\n", file );

        sprintf(file,"B:/%08d.jpg",fid) ;
        pkg_type = PKG_TYPE_JPG;

        file_hdl = mutex_f_Open(file, openReadOnly);
        if(!file_hdl){
            printf("f_open_faild: %d, %s\n", file );
            *file_type = 0;
            return NULL;
        }
	}
#else

    if(search_mode == FS_NEXT_FILE){
        if( g_last_search_mode != search_mode ) {
            file_hdl = select_file(2);
        }else{
            file_hdl = select_file(1);
        }
        if(!file_hdl){
            file_hdl = select_file(1);
            if(!file_hdl){
                *file_type = 0;
                return NULL;
            }
        }
    }else if(search_mode == FS_PREV_FILE){
        if(g_last_search_mode != search_mode){
            file_hdl = select_file(2);
        }else{
            file_hdl = select_file(0);
        }
        if(!file_hdl){
            file_hdl = select_file(0);
            if(!file_hdl){
                *file_type = 0;
                return NULL;
            }
        }
    }else{
        file_hdl = select_file(2);
        if(!file_hdl){
            *file_type = 0;
            return NULL;
        }
    }

    g_last_search_mode = search_mode;

#endif


    last_file_hdl = file_hdl;
    if(pb_fctrl.ftype == PKG_TYPE_JPG){
        //memcpy(g_file_path,file,32);
        memcpy(g_file_path,video_path,32);
    }
    *file_type = pb_fctrl.ftype;

    return file_hdl;

}



void * file_io_init(int id )
{
	file_t * file_hdl;
	static u8 flag = 0;


	FILE_ENTER_MUTEX();

	if(id == 1)//录像时的第二路文件
	{
        if(flag){//第一路释放空间失败，则第二路直接退出
            FILE_EXIT_MUTEX();
            return NULL;
        }
	    if (freeSpace(200, 1)==FALSE){
            FILE_EXIT_MUTEX();
            return NULL;
        }
	}
	else
	{
	    flag = 0;
	    if (freeSpace(800, 0)==FALSE){
            if(id == 0){
                flag = 1;
            }

            FILE_EXIT_MUTEX();
            return NULL;
        }
	}
    FILE_EXIT_MUTEX();

    if(id == 0)
    {
     file_hdl = create_video_file_mov();
    }
    else if(id == 1)
    {
      file_hdl = create_video_file_avi();

    }
    else if(id == 2)
    {
      file_hdl =create_pic_file_data_camera() ;
    }
    else
    {
        file_hdl =create_pic_file_analog_camera();
    }

	return file_hdl ;
}





static OS_MUTEX fs_mutex ;

u16 fs_mutex_init()
{
    u16 err;
    err = os_mutex_create(&fs_mutex) ;
    if(err){
        puts("jpeg mutex creat err");
        while(1);
    }

    return err;
}

u16 fs_wait_data_ok()
{
    u16 err  ;

    err = os_mutex_pend (&fs_mutex, 0);
    if(err){
        puts("jpeg mutex pend err");
        while(1);
    }

    return err ;

}

u16 fs_data_ok()
{
    u16 err  ;

    err = os_mutex_post(&fs_mutex) ;
    if(err){
        puts("jpeg mutex post err");
        while(1);
    }

    return err;
}

u32 mute_f_rename(u8* oldpath ,u8* newpath)
{

    u32 ret;

    VFS_ENTER_MUTEX();
    ret = frename(oldpath ,newpath);
    VFS_EXIT_MUTEX();

    return ret;
}



u32 mute_f_chdir(u8* path)
{
    u32 ret;

    VFS_ENTER_MUTEX();
    ret = f_chdir(path);
    VFS_EXIT_MUTEX();

    return ret;
}



u32 mute_f_mkdir(u8* path, u32 opt)
{
    u32 ret;

    VFS_ENTER_MUTEX();
    ret = f_mkdir(path, opt);
    VFS_EXIT_MUTEX();

    return ret;
}


u32 mute_f_delete(u8* path)
{
    u32 ret;

    VFS_ENTER_MUTEX();
    ret = fdelete(path);
    VFS_EXIT_MUTEX();

    return ret;

}


u32 mute_f_emu(u8* path, void *pram2callback, s32 (*callback)(void *fileparam, void *pram2callback))
{
    u32 ret;

    VFS_ENTER_MUTEX();
    ret = f_emu(path, pram2callback, callback);
    VFS_EXIT_MUTEX();

    return ret;

}

u32 mute_f_get_free_space(u8 drv)
{
    u32 ret;

    VFS_ENTER_MUTEX();
    ret = get_free_space(drv);
    VFS_EXIT_MUTEX();

    return ret;
}

u32 mute_f_mkfs_drv(char drv)
{
    u32 ret ;
    VFS_ENTER_MUTEX();
    ret = mkfs_drv(drv);
    VFS_EXIT_MUTEX();

    return ret;
}



file_t * mutex_f_Open(u8  *path, u32 openMode)
{
    file_t * ret;

printf("mutex_f_Open %s \r\b" ,path ) ;

VFS_ENTER_MUTEX();
    ret = fopen(path, openMode);
VFS_EXIT_MUTEX();
printf("ok %x\r\n",ret) ;

    return ret;
}

u32 mutex_f_Write(void  *pFile, void  *pBuf, u32 len)
{
    u32 ret;

VFS_ENTER_MUTEX();
    ret = fwrite(pFile, pBuf, len);
VFS_EXIT_MUTEX();

    return ret;
}

u32 mutex_f_Read(void   * pFile ,void   *pBuf , u32 btr)
{
    u32 ret;

VFS_ENTER_MUTEX();
    ret = fread(pFile, pBuf, btr);
VFS_EXIT_MUTEX();

    return ret;
}

tbool mutex_f_Seek (void   * pFile, u32 type, u32 offsize)
{
    tbool ret;

VFS_ENTER_MUTEX();

    ret = fseek(pFile, type, offsize);

VFS_EXIT_MUTEX();

    return ret;
}

u32 mutex_f_Tell(void   * pFile)
{
    u32 ret;
VFS_ENTER_MUTEX();
    ret = ftell(pFile);
VFS_EXIT_MUTEX();
    return ret;
}

void mutex_f_Close(void  *pFile)
{
VFS_ENTER_MUTEX();
    fclose(pFile);
VFS_EXIT_MUTEX();
}
u32 mutex_f_Size(void  *pFile)
{

    u32 file_size;

VFS_ENTER_MUTEX();
    file_size = get_file_length(g_file_path);
VFS_EXIT_MUTEX();

    return file_size;
}


u32 mutex_f_size(char  *pfile_path)
{
    u32 file_size;

VFS_ENTER_MUTEX();
    file_size = get_file_length(pfile_path);
VFS_EXIT_MUTEX();

    return file_size;
}

u8 mutex_f_get_file_attr(char  *pfile_path)
{
    u8 attr;

VFS_ENTER_MUTEX();
    attr = get_file_attr(pfile_path);
VFS_EXIT_MUTEX();

    return attr;
}




