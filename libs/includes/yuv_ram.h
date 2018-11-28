#ifndef YUV_RAM_H
#define YUV_RAM_H

#include "typedef.h"

struct yuv_addr{
	struct yuv_addr *next;
	u8 *y_addr;
	u8 *u_addr;
	u8 *v_addr;
	u8 state;
};


typedef void (*pending_fun)(void *priv, struct yuv_addr *yuv);

struct video_ram{
	//u8 id;
	volatile int vram_df;
	struct yuv_addr yuv[2];
	struct yuv_addr *read;
	struct yuv_addr *write;
	pending_fun pending;
	void *priv;

	u8 is_used;
	u32 saddr;
	u32 eaddr;
};


static inline void yuv_set_priv(struct video_ram *yuv, void *priv)
{
	yuv->priv = priv;
}



//void video_ram_init();
//struct video_ram * video_ram_alloc(u8 id, u16 y_len, u16 u_len, u16 v_len);
//u8 video_ram_free(u8 id, u16 y_len, u16 u_len, u16 v_len);
//int video_ram_avaliable(struct video_ram *ram, u8 index);
//inline void video_ram_pend_on_all(pending_fun pending);
//int video_ram_pend_on(struct video_ram *ram, pending_fun pending);
//int video_ram_try_pend_on(struct video_ram *ram, pending_fun pending);

#endif

