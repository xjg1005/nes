#ifndef _MEM_HEAP_H_
#define _MEM_HEAP_H_

#ifdef __cplusplus
extern "C" {
#endif

void *malloc(unsigned int size);
void *calloc(unsigned int count, unsigned int size);
void *realloc(void *rmem, unsigned int newsize);
void  free(void *mem);
void mem_heap_stats_display(void);

#ifdef __cplusplus
}
#endif

#endif /* _MEM_HEAP_H_ */
