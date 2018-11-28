#ifndef __KEY_SCAN_H__
#define __KEY_SCAN_H__

#include "typedef.h"

enum key_msg
{
    SHORT_KEY_MSG = 1,
    LONG_KEY_MSG,
    REPEAT_KEY_MSG,
    REPEAT_KEY_RELEASE_MSG,
};


typedef struct _key_press
{
    enum key_msg type;
    int number;
} key_press;
key_press *get_key_msg;

void key_scan_init(bool (*get_key_fun)(void *get_key_priv), void (*deal_key_fun)(enum key_msg key_msg, void *deal_key_priv));
int key_scan_reg(void *key_priv, void *deal_key_priv);
void key_scan_set_tick(u8 _scan_tick, u8 _short_key_scan_tick, u8 _long_key_scan_tick, u8 _repeat_key_scan_tick);

#endif
