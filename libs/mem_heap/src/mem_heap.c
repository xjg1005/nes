/**
 * @file
 * Dynamic memory manager
 *
 * This is a lightweight replacement for the standard C library malloc().
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *         Simon Goldschmidt
 *
 */

#include "mem_heap.h"
#include "printf.h"
//#include <string.h>



typedef unsigned char u8_t;
typedef signed char err_t;
typedef unsigned short u16_t;
typedef signed short s16_t;
typedef unsigned long u32_t,mem_ptr_t;

/* Define (sn)printf formatters for these lwIP types */
#define U16_F                   "u"
#define S16_F                   "d"
#define X16_F                   "x"
#define U32_F                   "u"
#define S32_F                   "d"
#define X32_F                   "x"

#define MEM_HEAP_NO_SYS                      0	 //0:使用操作系统,1:不使用操作系统


/** All allocated blocks will be MEM_HEAP_MIN_SIZE bytes big, at least!
 * MEM_HEAP_MIN_SIZE can be overridden to suit your needs. Smaller values save space,
 * larger values could prevent too small blocks to fragment the RAM too much. */
#define MEM_HEAP_MIN_SIZE                    32

#define MEM_HEAP_STATS                       1   //统计功能


/** print debug message only if debug message type is enabled...
 */
#define MEM_HEAP_PLATFORM_ASSERT(x) \
    do \
    {   printf("Assertion \"%s\" failed at line %d in %s\n", x, __LINE__, __FILE__);while(1); \
    } while(0)

#define MEM_HEAP_PLATFORM_DIAG(x)  do {printf x;} while(0)

//#define MEM_HEAP_NOASSERT

/** flag for LWIP_DEBUGF to enable that debug message */
#define MEM_HEAP_DBG_ON                     0x80U
/** flag for LWIP_DEBUGF to disable that debug message */
#define MEM_HEAP_DBG_OFF                    0x00U
/** flag for LWIP_DEBUGF to halt after printing this debug message */
#define MEM_HEAP_DBG_HALT                   0x08U

#define MEM_HEAP_DEBUG                      (MEM_HEAP_DBG_ON)


/*
   ------------------------------------
   ---------- MEM_HEAP options ----------
   ------------------------------------
*/
/**
 * MEM_HEAP_ALIGNMENT: should be set to the alignment of the CPU
 *    4 byte alignment -> #define MEM_HEAP_ALIGNMENT 4
 *    2 byte alignment -> #define MEM_HEAP_ALIGNMENT 2
 */
#define MEM_HEAP_ALIGNMENT                   32

/**
 * MEM_SIZE: the size of the heap memory. If the application will send
 * a lot of data that needs to be copied, this should be set high.
 */

#define MEM_HEAP_SIZE                        (5*1024*1024)//(900*1024 )//




/* MEM_HEAP_SIZE would have to be aligned, but using 64000 here instead of
 * 65535 leaves some room for alignment...
 */
//#if MEM_HEAP_SIZE > 64000L
typedef unsigned int mem_size_t;
//#else
//typedef unsigned short mem_size_t;
//#endif /* MEM_HEAP_SIZE > 64000 */

/**
 * Set this to 1 if you want to free PBUF_RAM pbufs (or call mem_free()) from
 * interrupt context (or another context that doesn't allow waiting for a
 * semaphore).
 * If set to 1, mem_malloc will be protected by a semaphore and SYS_ARCH_PROTECT,
 * while mem_free will only use SYS_ARCH_PROTECT. mem_malloc SYS_ARCH_UNPROTECTs
 * with each loop so that mem_free can run.
 *
 * ATTENTION: As you can see from the above description, this leads to dis-/
 * enabling interrupts often, which can be slow! Also, on low memory, mem_malloc
 * can need longer.
 */


#ifdef MEM_HEAP_DEBUG

#define MEM_HEAP_DEBUGF(debug, message) do { \
                               if ((debug) & MEM_HEAP_DBG_ON) { \
                                 MEM_HEAP_PLATFORM_DIAG(message); \
                                 if ((debug) & MEM_HEAP_DBG_HALT) { \
                                   while(1); \
                                 } \
                               } \
                             } while(0)
#else  /* MEM_HEAP_DEBUG */
#define MEM_HEAP_DEBUGF(debug, message)
#endif /* MEM_HEAP_DEBUG */

#ifndef MEM_HEAP_NOASSERT
#define MEM_HEAP_ASSERT(message, assertion) do { if(!(assertion)) \
  MEM_HEAP_PLATFORM_ASSERT(message); } while(0)
#else  /* MEM_HEAP_NOASSERT */
#define MEM_HEAP_ASSERT(message, assertion)
#endif /* MEM_HEAP_NOASSERT */


#if MEM_HEAP_STATS

struct stats_mem {
#ifdef MEM_HEAP_DEBUG
  const char *name;
#endif /* MEM_HEAP_DEBUG */
  mem_size_t avail;
  mem_size_t used;
  mem_size_t max;
  u32_t err;
  u32_t illegal;
};

struct stats_ {
  struct stats_mem mem;
};

static struct stats_ mem_stats;

#define STATS_INC(x) ++mem_stats.x
#define STATS_DEC(x) --mem_stats.x
#define STATS_INC_USED(x, y) do { mem_stats.x.used += y; \
                                if (mem_stats.x.max < mem_stats.x.used) { \
                                    mem_stats.x.max = mem_stats.x.used; \
                                } \
                             } while(0)

#define MEM_STATS_AVAIL(x, y) mem_stats.mem.x = y
#define MEM_STATS_INC(x) STATS_INC(mem.x)
#define MEM_STATS_INC_USED(x, y) STATS_INC_USED(mem, y)
#define MEM_STATS_DEC_USED(x, y) mem_stats.mem.x -= y

void mem_stats_init(void)
{
#ifdef MEM_HEAP_DEBUG
  mem_stats.mem.name = "MEM_HEAP";
#endif
}

void malloc_stats(void)
{
  MEM_HEAP_PLATFORM_DIAG(("\nMEM_HEAP %s\n\t", mem_stats.mem.name));
  MEM_HEAP_PLATFORM_DIAG(("avail: %"U32_F"\n\t", (u32_t)mem_stats.mem.avail));
  MEM_HEAP_PLATFORM_DIAG(("used: %"U32_F"\n\t", (u32_t)mem_stats.mem.used));
  MEM_HEAP_PLATFORM_DIAG(("max: %"U32_F"\n\t", (u32_t)mem_stats.mem.max));
  MEM_HEAP_PLATFORM_DIAG(("err: %"U32_F"\n", (u32_t)mem_stats.mem.err));
}

#else
#define MEM_STATS_AVAIL(x, y)
#define MEM_STATS_INC(x)
#define MEM_STATS_INC_USED(x, y)
#define MEM_STATS_DEC_USED(x, y)
#endif


#if !MEM_HEAP_NO_SYS

#include "os_api.h"


typedef OS_MUTEX sys_mutex_t; // type of mutex
static sys_mutex_t mem_mutex;
static err_t sys_mutex_new(void)
{
    u16 err;
    err =os_mutex_create(&mem_mutex) ;

	MEM_HEAP_ASSERT("sys_mutex_new ",err == OS_ERR_NONE );
	if(err)
        return err;

	return 0;
}

/* Protect the heap only by using a semaphore */
#define MEM_HEAP_WAIT_MUTEX_TIMEOUT    0
#define MEM_HEAP_DECL_PROTECT()   u16   oserr
//#define MEM_HEAP_PROTECT()     os_mutex_pend(&mem_mutex,0,MEM_HEAP_WAIT_MUTEX_TIMEOUT)
#define MEM_HEAP_PROTECT()     os_mutex_pend(&mem_mutex,0)
#define MEM_HEAP_UNPROTECT()   os_mutex_post(&mem_mutex)


#else /* !MEM_HEAP_NO_SYS */
#define sys_mutex_new()   0
#define SYS_ARCH_DECL_PROTECT(cpu_sr)
#define  SYS_ARCH_PROTECT(cpu_sr)
#define  SYS_ARCH_UNPROTECT(cpu_sr)
#define MEM_HEAP_DECL_PROTECT()
#define MEM_HEAP_PROTECT()
#define MEM_HEAP_UNPROTECT()
#endif

/* lwIP replacement for your libc malloc() */

/**
 * The heap is made up as a list of structs of this type.
 * This does not have to be aligned since for getting its size,
 * we only use the macro SIZEOF_STRUCT_MEM, which automatically alignes.
 */
struct mem {
  /** index (-> ram[next]) of the next struct */
  mem_size_t next;
  /** index (-> ram[prev]) of the previous struct */
  mem_size_t prev;
  /** 1: this area is used; 0: this area is unused */
  u8_t used;
};

/* some alignment macros: we define them here for better source code layout */
#define MIN_SIZE_ALIGNED     MEM_HEAP_ALIGN_SIZE(MEM_HEAP_MIN_SIZE)
#define SIZEOF_STRUCT_MEM    MEM_HEAP_ALIGN_SIZE(sizeof(struct mem))
#define MEM_SIZE_ALIGNED     MEM_HEAP_ALIGN_SIZE(MEM_HEAP_SIZE)

/** Calculate memory size for an aligned buffer - returns the next highest
 * multiple of MEM_HEAP_ALIGNMENT (e.g. MEM_HEAP_ALIGN_SIZE(3) and
 * MEM_HEAP_ALIGN_SIZE(4) will both yield 4 for MEM_HEAP_ALIGNMENT == 4).
 */
#define MEM_HEAP_ALIGN_SIZE(size) (((size) + MEM_HEAP_ALIGNMENT - 1) & ~(MEM_HEAP_ALIGNMENT-1))

/** Align a memory pointer to the alignment defined by MEM_HEAP_ALIGNMENT
 * so that ADDR % MEM_HEAP_ALIGNMENT == 0
 */
#define MEM_HEAP_ALIGN(addr) ((void *)(((mem_ptr_t)(addr) + MEM_HEAP_ALIGNMENT - 1) & ~(mem_ptr_t)(MEM_HEAP_ALIGNMENT-1)))


/** If you want to relocate the heap to external memory, simply define
 * LWIP_RAM_HEAP_POINTER as a void-pointer to that location.
 * If so, make sure the memory at that location is big enough (see below on
 * how that space is calculated). */
#ifndef LWIP_RAM_HEAP_POINTER
/** the heap. we need one struct mem at the end and some room for alignment */
u8_t ram_heap[MEM_SIZE_ALIGNED + (2*SIZEOF_STRUCT_MEM) + MEM_HEAP_ALIGNMENT] __attribute__((section(".mem_heap")));
#define LWIP_RAM_HEAP_POINTER ram_heap
#endif /* LWIP_RAM_HEAP_POINTER */

/** pointer to the heap (ram_heap): for alignment, ram is now a pointer instead of an array */
static u8_t *ram;
/** the last entry, always unused! */
static struct mem *ram_end;
/** pointer to the lowest free block, this is used for faster search */
static struct mem *lfree;

/**
 * "Plug holes" by combining adjacent empty struct mems.
 * After this function is through, there should not exist
 * one empty struct mem pointing to another empty struct mem.
 *
 * @param mem this points to a struct mem which just has been freed
 * @internal this function is only called by mem_free() and mem_trim()
 *
 * This assumes access to the heap is protected by the calling function
 * already.
 */

static void
plug_holes(struct mem *mem)
{
  struct mem *nmem;
  struct mem *pmem;

  MEM_HEAP_ASSERT("plug_holes: mem >= ram", (u8_t *)mem >= ram);
  MEM_HEAP_ASSERT("plug_holes: mem < ram_end", (u8_t *)mem < (u8_t *)ram_end);
  MEM_HEAP_ASSERT("plug_holes: mem->used == 0", mem->used == 0);

  /* plug hole forward */
  MEM_HEAP_ASSERT("plug_holes: mem->next <= MEM_SIZE_ALIGNED", mem->next <= MEM_SIZE_ALIGNED);

  nmem = (struct mem *)(void *)&ram[mem->next];
  if (mem != nmem && nmem->used == 0 && (u8_t *)nmem != (u8_t *)ram_end) {
    /* if mem->next is unused and not end of ram, combine mem and mem->next */
    if (lfree == nmem) {
      lfree = mem;
    }
    mem->next = nmem->next;
    ((struct mem *)(void *)&ram[nmem->next])->prev = (mem_size_t)((u8_t *)mem - ram);
  }

  /* plug hole backward */
  pmem = (struct mem *)(void *)&ram[mem->prev];
  if (pmem != mem && pmem->used == 0) {
    /* if mem->prev is unused, combine mem and mem->prev */
    if (lfree == mem) {
      lfree = pmem;
    }
    pmem->next = mem->next;
    ((struct mem *)(void *)&ram[mem->next])->prev = (mem_size_t)((u8_t *)pmem - ram);
  }
}

/**
 * Zero the heap and initialize start, end and lowest-free
 */
static void
mem_heap_init(void)
{
  static u8 mem_init_ok = 1 ;

  struct mem *mem;
  if(!mem_init_ok)
  {
      return ;
  }
#if MEM_HEAP_STATS
    mem_stats_init();
#endif
  MEM_HEAP_ASSERT("Sanity check alignment",
    (SIZEOF_STRUCT_MEM & (MEM_HEAP_ALIGNMENT-1)) == 0);

  /* align the heap */
  ram = (u8_t *)MEM_HEAP_ALIGN(LWIP_RAM_HEAP_POINTER);
  /* initialize the start of the heap */
  mem = (struct mem *)(void *)ram;
  mem->next = MEM_SIZE_ALIGNED;
  mem->prev = 0;
  mem->used = 0;
  /* initialize the end of the heap */
  ram_end = (struct mem *)(void *)&ram[MEM_SIZE_ALIGNED];
  ram_end->used = 1;
  ram_end->next = MEM_SIZE_ALIGNED;
  ram_end->prev = MEM_SIZE_ALIGNED;

  /* initialize the lowest-free pointer to the start of the heap */
  lfree = (struct mem *)(void *)ram;

  MEM_STATS_AVAIL(avail, MEM_SIZE_ALIGNED);

  if(sys_mutex_new() != 0) {
    MEM_HEAP_ASSERT("failed to create mem_mutex", 0);
  }

  mem_init_ok = 0 ;
}

/**
 * Put a struct mem back on the heap
 *
 * @param rmem is the data portion of a struct mem as returned by a previous
 *             call to mem_malloc()
 */
void
free(void *rmem)
{
  struct mem *mem;
  MEM_HEAP_DECL_PROTECT();

  if (rmem == NULL) {
//    MEM_HEAP_DEBUGF(MEM_HEAP_DEBUG, ("mem_free(p == NULL) was called.\n"));
    return;
  }
  MEM_HEAP_ASSERT("mem_free: sanity check alignment", (((mem_ptr_t)rmem) & (MEM_HEAP_ALIGNMENT-1)) == 0);

  MEM_HEAP_ASSERT("mem_free: legal memory", (u8_t *)rmem >= (u8_t *)ram &&
    (u8_t *)rmem < (u8_t *)ram_end);

  if ((u8_t *)rmem < (u8_t *)ram || (u8_t *)rmem >= (u8_t *)ram_end) {
    MEM_HEAP_DEBUGF(MEM_HEAP_DEBUG, ("mem_free: illegal memory\n"));
    /* protect mem stats from concurrent access */
    MEM_HEAP_PROTECT();
    MEM_STATS_INC(illegal);
    MEM_HEAP_UNPROTECT();
    return;
  }
  /* protect the heap from concurrent access */
  MEM_HEAP_PROTECT();
  /* Get the corresponding struct mem ... */
  mem = (struct mem *)(void *)((u8_t *)rmem - SIZEOF_STRUCT_MEM);
  /* ... which has to be in a used state ... */

  MEM_HEAP_ASSERT("mem_free: mem->used", mem->used);
  /* ... and is now unused. */
  mem->used = 0;

  if (mem < lfree) {
    /* the newly freed struct is now the lowest */
    lfree = mem;
  }

  MEM_STATS_DEC_USED(used, mem->next - (mem_size_t)(((u8_t *)mem - ram)));

  /* finally, see if prev or next are free also */
  plug_holes(mem);

  MEM_HEAP_UNPROTECT();
}

/**
 * Adam's mem_malloc() plus solution for bug #17922
 * Allocate a block of memory with a minimum of 'size' bytes.
 *
 * @param size is the minimum size of the requested block in bytes.
 * @return pointer to allocated memory or NULL if no free memory was found.
 *
 * Note that the returned value will always be aligned (as defined by MEM_HEAP_ALIGNMENT).
 */
void *
malloc(mem_size_t size)
{
  mem_size_t ptr, ptr2;
  struct mem *mem, *mem2;

  MEM_HEAP_DECL_PROTECT();

  if (size == 0) {
    return NULL;
  }
   mem_heap_init();

  /* Expand the size of the allocated memory region so that we can
     adjust for alignment. */
  size = MEM_HEAP_ALIGN_SIZE(size);

  if(size < MIN_SIZE_ALIGNED) {
    /* every data block must be at least MIN_SIZE_ALIGNED long */
    size = MIN_SIZE_ALIGNED;
  }

  if (size > MEM_SIZE_ALIGNED) {
    return NULL;
  }

  /* protect the heap from concurrent access */
  MEM_HEAP_PROTECT();


    /* Scan through the heap searching for a free block that is big enough,
     * beginning with the lowest free block.
     */
    for (ptr = (mem_size_t)((u8_t *)lfree - ram); ptr < MEM_SIZE_ALIGNED - size;
         ptr = ((struct mem *)(void *)&ram[ptr])->next) {
      mem = (struct mem *)(void *)&ram[ptr];
      if ((!mem->used) &&
          (mem->next - (ptr + SIZEOF_STRUCT_MEM)) >= size) {
        /* mem is not used and at least perfect fit is possible:
         * mem->next - (ptr + SIZEOF_STRUCT_MEM) gives us the 'user data size' of mem */

        if (mem->next - (ptr + SIZEOF_STRUCT_MEM) >= (size + SIZEOF_STRUCT_MEM + MIN_SIZE_ALIGNED)) {
          /* (in addition to the above, we test if another struct mem (SIZEOF_STRUCT_MEM) containing
           * at least MIN_SIZE_ALIGNED of data also fits in the 'user data space' of 'mem')
           * -> split large block, create empty remainder,
           * remainder must be large enough to contain MIN_SIZE_ALIGNED data: if
           * mem->next - (ptr + (2*SIZEOF_STRUCT_MEM)) == size,
           * struct mem would fit in but no data between mem2 and mem2->next
           * @todo we could leave out MIN_SIZE_ALIGNED. We would create an empty
           *       region that couldn't hold data, but when mem->next gets freed,
           *       the 2 regions would be combined, resulting in more free memory
           */
          ptr2 = ptr + SIZEOF_STRUCT_MEM + size;
          /* create mem2 struct */
          mem2 = (struct mem *)(void *)&ram[ptr2];
          mem2->used = 0;
          mem2->next = mem->next;
          mem2->prev = ptr;
          /* and insert it between mem and mem->next */
          mem->next = ptr2;
          mem->used = 1;

          if (mem2->next != MEM_SIZE_ALIGNED) {
            ((struct mem *)(void *)&ram[mem2->next])->prev = ptr2;
          }
          MEM_STATS_INC_USED(used, (size + SIZEOF_STRUCT_MEM));
        } else {
          /* (a mem2 struct does no fit into the user data space of mem and mem->next will always
           * be used at this point: if not we have 2 unused structs in a row, plug_holes should have
           * take care of this).
           * -> near fit or excact fit: do not split, no mem2 creation
           * also can't move mem->next directly behind mem, since mem->next
           * will always be used at this point!
           */
          mem->used = 1;
          MEM_STATS_INC_USED(used, mem->next - (mem_size_t)((u8_t *)mem - ram));
        }
        if (mem == lfree) {
          struct mem *cur = lfree;
          /* Find next free block after mem and update lowest free pointer */
          while (cur->used && cur != ram_end) {
            cur = (struct mem *)(void *)&ram[cur->next];
          }
          lfree = cur;
          MEM_HEAP_ASSERT("mem_malloc: !lfree->used", ((lfree == ram_end) || (!lfree->used)));
        }
        MEM_HEAP_UNPROTECT();
        MEM_HEAP_ASSERT("mem_malloc: allocated memory not above ram_end.",
         (mem_ptr_t)mem + SIZEOF_STRUCT_MEM + size <= (mem_ptr_t)ram_end);
        MEM_HEAP_ASSERT("mem_malloc: allocated memory properly aligned.",
         ((mem_ptr_t)mem + SIZEOF_STRUCT_MEM) % MEM_HEAP_ALIGNMENT == 0);
        MEM_HEAP_ASSERT("mem_malloc: sanity check alignment",
          (((mem_ptr_t)mem) & (MEM_HEAP_ALIGNMENT-1)) == 0);

        return (u8_t *)mem + SIZEOF_STRUCT_MEM;
      }
    }

  MEM_HEAP_DEBUGF(MEM_HEAP_DEBUG, ("mem_malloc: could not allocate %"S16_F" bytes\n", (s16_t)size));
  MEM_STATS_INC(err);
  MEM_HEAP_UNPROTECT();
  return NULL;
}

/**
 * Contiguously allocates enough space for count objects that are size bytes
 * of memory each and returns a pointer to the allocated memory.
 *
 * The allocated memory is filled with bytes of value zero.
 *
 * @param count number of objects to allocate
 * @param size size of the objects to allocate
 * @return pointer to allocated memory / NULL pointer if there is an error
 */
void *calloc(mem_size_t count, mem_size_t size)
{
  void *p;

  /* allocate 'count' objects of size 'size' */
  p = malloc(count * size);
  if (p) {
    /* zero the memory */
    memset(p, 0, count * size);
  }
  return p;
}


void *realloc(void *rmem, mem_size_t newsize)
{
  mem_size_t size;
  mem_size_t ptr, ptr2;
  struct mem *mem, *mem2;
  void *new_rmem = NULL;
  /* use the FREE_PROTECT here: it protects with sem OR SYS_ARCH_PROTECT */
  MEM_HEAP_DECL_PROTECT();

  /* Expand the size of the allocated memory region so that we can
     adjust for alignment. */
  newsize = MEM_HEAP_ALIGN_SIZE(newsize);

  if(newsize < MIN_SIZE_ALIGNED) {
    /* every data block must be at least MIN_SIZE_ALIGNED long */
    newsize = MIN_SIZE_ALIGNED;
  }

  if (newsize > MEM_SIZE_ALIGNED) {
    return NULL;
  }

  mem_heap_init();

  if(rmem == NULL)
  {
      rmem = malloc(newsize);
      return rmem;
  }

  MEM_HEAP_ASSERT("realloc: legal memory", ((u8_t *)rmem >= (u8_t *)ram &&
   (u8_t *)rmem < (u8_t *)ram_end));

  if ((u8_t *)rmem < (u8_t *)ram || (u8_t *)rmem >= (u8_t *)ram_end) {
    MEM_HEAP_DEBUGF(MEM_HEAP_DEBUG, ("realloc: illegal memory\n"));
    /* protect mem stats from concurrent access */
   MEM_HEAP_PROTECT();
    MEM_STATS_INC(illegal);
    MEM_HEAP_UNPROTECT();
    return rmem;
  }
  /* Get the corresponding struct mem ... */
  mem = (struct mem *)(void *)((u8_t *)rmem - SIZEOF_STRUCT_MEM);
  /* ... and its offset pointer */
  ptr = (mem_size_t)((u8_t *)mem - ram);

  size = mem->next - ptr - SIZEOF_STRUCT_MEM;

  if (newsize > size) {


    new_rmem = malloc(newsize);
    if (new_rmem != NULL)
    {
        memmove(new_rmem,rmem,size);
        free(rmem);
        rmem = new_rmem;
        return rmem;
    }
    else
    {
        puts("mem_realloc fail!\n");
         return NULL;
    }

  }
  else if (newsize < size)
  {
      /* protect the heap from concurrent access */
      MEM_HEAP_PROTECT();

      mem2 = (struct mem *)(void *)&ram[mem->next];
      if(mem2->used == 0) {
        /* The next struct is unused, we can simply move it at little */
        mem_size_t next;
        /* remember the old next pointer */
        next = mem2->next;
        /* create new struct mem which is moved directly after the shrinked mem */
        ptr2 = ptr + SIZEOF_STRUCT_MEM + newsize;
        if (lfree == mem2) {
          lfree = (struct mem *)(void *)&ram[ptr2];
        }
        mem2 = (struct mem *)(void *)&ram[ptr2];
        mem2->used = 0;
        /* restore the next pointer */
        mem2->next = next;
        /* link it back to mem */
        mem2->prev = ptr;
        /* link mem to it */
        mem->next = ptr2;
        /* last thing to restore linked list: as we have moved mem2,
         * let 'mem2->next->prev' point to mem2 again. but only if mem2->next is not
         * the end of the heap */
        if (mem2->next != MEM_SIZE_ALIGNED) {
          ((struct mem *)(void *)&ram[mem2->next])->prev = ptr2;
        }
        MEM_STATS_DEC_USED(used, (size - newsize));
        /* no need to plug holes, we've already done that */
      } else if (newsize + SIZEOF_STRUCT_MEM + MIN_SIZE_ALIGNED <= size) {
        /* Next struct is used but there's room for another struct mem with
         * at least MIN_SIZE_ALIGNED of data.
         * Old size ('size') must be big enough to contain at least 'newsize' plus a struct mem
         * ('SIZEOF_STRUCT_MEM') with some data ('MIN_SIZE_ALIGNED').
         * @todo we could leave out MIN_SIZE_ALIGNED. We would create an empty
         *       region that couldn't hold data, but when mem->next gets freed,
         *       the 2 regions would be combined, resulting in more free memory */
        ptr2 = ptr + SIZEOF_STRUCT_MEM + newsize;
        mem2 = (struct mem *)(void *)&ram[ptr2];
        if (mem2 < lfree) {
          lfree = mem2;
        }
        mem2->used = 0;
        mem2->next = mem->next;
        mem2->prev = ptr;
        mem->next = ptr2;
        if (mem2->next != MEM_SIZE_ALIGNED) {
          ((struct mem *)(void *)&ram[mem2->next])->prev = ptr2;
        }
        MEM_STATS_DEC_USED(used, (size - newsize));
        /* the original mem->next is used, so no need to plug holes! */
      }
      /* else {
        next struct mem is used but size between mem and mem2 is not big enough
        to create another struct mem
        -> don't do anyhting.
        -> the remaining space stays unused since it is too small
      } */
      MEM_HEAP_UNPROTECT();
      return rmem;
  }
  else if (newsize == size) {
    /* No change in size, simply return */
    return rmem;
  }
}

