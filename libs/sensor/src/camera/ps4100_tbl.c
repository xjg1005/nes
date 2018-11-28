
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"
#include "ps4100.h"


/*
LV 18 and above: Bright reflection off a sunlit object, including reflections off the sea
LV 17 White object in direct sunlight
LV 16 Light gray object or skin in full sunlight
LV 15 Gray card in full sunlight; typical exposure for ugly front-lit noon daylight photos
LV 14 Typical light level for side-lit daylight shots in good afternoon light
LV 13 Typical shadow cast in a daylight scene; cloudy bright days
LV 12 California bright overcast
LV 11
LV 10 Dark, dreary overcast day in Boston, London or Paris
LV 9
LV 8
LV 7 Typical indoors; light outdoors about 10 minutes after sunset
LV 6
LV 5
LV 4
LV 3 Brightly lit night street scenes
LV 2 Typical night street scenes
LV 1 Dark scenes outdoors at night
LV 0 LV Zero is defined as the light level that requires a 1 second exposure at f/1 with ISO/ASA100 speed film.
*/
PS4100_AE_lut_t PS4100_AE_lut_50Hz_min_30fps[] = {
 {       10,   0x00,   0x40,   16},
 {       11,   0x00,   0x40,   16},
 {       12,   0x00,   0x40,   16},
 {       13,   0x00,   0x40,   16},
 {       14,   0x00,   0x40,   16},
 {       15,   0x00,   0x40,   16},
 {       16,   0x00,   0x40,   16},
 {       17,   0x00,   0x40,   16},
 {       18,   0x00,   0x40,   16},
 {       19,   0x00,   0x40,   16},
 {       20,   0x00,   0x40,   16},
 {       21,   0x00,   0x40,   16},
 {       22,   0x00,   0x40,   16},
 {       23,   0x00,   0x40,   16},
 {       24,   0x00,   0x40,   16},
 {       25,   0x00,   0x40,   16},
 {       26,   0x00,   0x40,   16},
 {       27,   0x00,   0x40,   16},
 {       28,   0x00,   0x40,   16},
 {       29,   0x00,   0x40,   16},
 {       30,   0x00,   0x40,   16},
 {       31,   0x00,   0x40,   16},
 {       32,   0x00,   0x40,   16},
 {       33,   0x00,   0x40,   16},
 {       34,   0x00,   0x40,   16},
 {       35,   0x00,   0x40,   16},
 {       36,   0x00,   0x40,   16},
 {       37,   0x00,   0x40,   16},
 {       38,   0x00,   0x40,   16},
 {       39,   0x00,   0x40,   16},
 {       40,   0x00,   0x40,   16},
 {       42,   0x00,   0x40,   16},
 {       44,   0x00,   0x40,   16},
 {       46,   0x00,   0x40,   16},
 {       48,   0x00,   0x40,   16},
 {       50,   0x00,   0x40,   16},
 {       52,   0x00,   0x40,   16},
 {       55,   0x00,   0x40,   16},
 {       59,   0x00,   0x40,   16},
 {       64,   0x00,   0x40,   16},
 {       72,   0x00,   0x40,   16},
 {       80,   0x00,   0x40,   16},
 {       88,   0x00,   0x40,   16},
 {       96,   0x00,   0x40,   16},
 {      104,   0x00,   0x40,   16},
 {      112,   0x00,   0x40,   16},
 {      120,   0x00,   0x40,   16},
 {      132,   0x00,   0x40,   16},
 {      144,   0x00,   0x40,   16},
 {      156,   0x00,   0x40,   16},
 {      168,   0x00,   0x40,   16},
 {      180,   0x00,   0x40,   16},
 {      192,   0x00,   0x40,   16},
 {      204,   0x00,   0x40,   16},
 {      216,   0x00,   0x40,   16},
 {      228,   0x00,   0x40,   16},
 {      240,   0x00,   0x40,   16},
 {      252,   0x00,   0x40,   15},
 {      264,   0x00,   0x40,   15},
 {      276,   0x00,   0x40,   15},
 {      288,   0x00,   0x40,   15},
 {      300,   0x00,   0x40,   15},
 {      312,   0x00,   0x40,   15},
 {      324,   0x00,   0x40,   15},
 {      336,   0x00,   0x40,   15},
 {      360,   0x00,   0x40,   15},
 {      376,   0x00,   0x40,   15},
 {      392,   0x00,   0x40,   15},
 {      408,   0x00,   0x40,   15},
 {      424,   0x00,   0x40,   15},
 {      448,   0x00,   0x40,   15},
 {      472,   0x00,   0x40,   15},
 {      496,   0x00,   0x40,   15},
 {      528,   0x00,   0x40,   15},
 {      560,   0x00,   0x40,   15},
 {      592,   0x00,   0x40,   15},
 {      632,   0x00,   0x40,   15},
 {      672,   0x00,   0x40,   15},
 {      712,   0x00,   0x40,   14},
 {      752,   0x00,   0x40,   14},
 {      824,   0x00,   0x40,   14},
 {      896,   0x00,   0x40,   14},
 {      968,   0x00,   0x40,   14},
 {     1040,   0x00,   0x40,   14},
 {     1112,   0x00,   0x40,   14},
 {     1184,   0x00,   0x40,   14},
 {     1264,   0x00,   0x40,   14},
 {     1344,   0x00,   0x40,   14},
 {     1424,   0x00,   0x40,   14},
 {     1504,   0x00,   0x40,   14},
 {     1592,   0x00,   0x40,   14},
 {     1680,   0x00,   0x40,   14},
 {     1776,   0x00,   0x40,   14},
 {     1880,   0x00,   0x40,   14},
 {     1992,   0x00,   0x40,   14},
 {     2112,   0x00,   0x40,   14},
 {     2256,   0x00,   0x40,   14},
 {     2408,   0x00,   0x40,   13},
 {     2576,   0x00,   0x40,   13},
 {     2744,   0x00,   0x40,   13},
 {     2880,   0x00,   0x40,   13},
 {     3008,   0x00,   0x40,   13},
 {     3184,   0x00,   0x40,   13},
 {     3384,   0x00,   0x40,   13},
 {     3600,   0x00,   0x40,   13},
 {     3880,   0x00,   0x40,   13},
 {     4160,   0x00,   0x40,   13},
 {     4580,   0x00,   0x40,   13},
 {     5004,   0x00,   0x40,   13},
 {     5404,   0x00,   0x40,   13},
 {     5850,   0x00,   0x40,   13},
 {     6300,   0x00,   0x40,   13},
 {     6850,   0x00,   0x40,   13},
 {     7400,   0x00,   0x40,   13},
 {     8050,   0x00,   0x40,   13},
 {     8650,   0x00,   0x40,   13},
 {     9300,   0x00,   0x40,   13},
 {    10000,   0x00,   0x40,   12},
 {    10000,   0x01,   0x40,   12},
 {    10000,   0x02,   0x40,   12},
 {    10000,   0x03,   0x40,   12},
 {    10000,   0x04,   0x40,   12},
 {    10000,   0x05,   0x40,   12},
 {    10000,   0x06,   0x40,   12},
 {    10000,   0x07,   0x40,   12},
 {    10000,   0x08,   0x40,   12},
 {    10000,   0x0a,   0x40,   12},
 {    10000,   0x0b,   0x40,   12},
 {    10000,   0x0c,   0x40,   12},
 {    10000,   0x0d,   0x40,   12},
 {    10000,   0x0e,   0x40,   12},
 {    10000,   0x0f,   0x40,   12},
 {    10000,   0x10,   0x40,   11},
 {    10000,   0x11,   0x40,   11},
 {    10000,   0x12,   0x40,   11},
 {    10000,   0x13,   0x40,   11},
 {    10000,   0x14,   0x40,   11},
 {    10000,   0x15,   0x40,   11},
 {    10000,   0x16,   0x40,   11},
 {    10000,   0x17,   0x40,   11},
 {    10000,   0x18,   0x40,   11},
 {    10000,   0x19,   0x40,   11},
 {    10000,   0x1a,   0x40,   10},
 {    10000,   0x1b,   0x40,   10},
 {    10000,   0x1c,   0x40,   10},
 {    10000,   0x1d,   0x40,   10},
 {    10000,   0x1e,   0x40,   10},
 {    10000,   0x1f,   0x40,   10},
 {    20000,   0x0e,   0x40,   10},
 {    20000,   0x0f,   0x40,   10},
 {    20000,   0x10,   0x40,   10},
 {    20000,   0x11,   0x40,   10},
 {    20000,   0x12,   0x40,   9},
 {    20000,   0x13,   0x40,   9},
 {    20000,   0x14,   0x40,   9},
 {    20000,   0x15,   0x40,   9},
 {    20000,   0x16,   0x40,   9},
 {    20000,   0x17,   0x40,   9},
 {    20000,   0x18,   0x40,   9},
 {    30000,   0x10,   0x40,   8},
 {    30000,   0x11,   0x40,   8},
 {    30000,   0x12,   0x40,   8},
 {    30000,   0x13,   0x40,   8},
 {    30000,   0x14,   0x40,   8},
 {    30000,   0x15,   0x40,   8},
 {    30000,   0x16,   0x40,   8},
 {    30000,   0x17,   0x40,   8},
 {    30000,   0x18,   0x40,   8},
 {    30000,   0x19,   0x40,   8},
 {    30000,   0x1A,   0x40,   8},
 {    30000,   0x1B,   0x40,   8},
 {    30000,   0x1C,   0x40,   8},
 {    30000,   0x1d,   0x40,   8},
 {    30000,   0x1E,   0x40,   8},
 {    30000,   0x1F,   0x40,   8},
 {    30000,   0x20,   0x40,   7},
 {    30000,   0x21,   0x40,   7},
 {    30000,   0x22,   0x40,   7},
 {    30000,   0x23,   0x40,   7},
 {    30000,   0x24,   0x40,   7},
 {    30000,   0x25,   0x40,   7},
 {    30000,   0x26,   0x40,   7},
 {    30000,   0x27,   0x40,   7},
 {    30000,   0x28,   0x40,   7},
 {    30000,   0x29,   0x40,   7},
 {    30000,   0x2a,   0x40,   7},
 {    30000,   0x2b,   0x40,   7},
 {    30000,   0x2c,   0x40,   7},
 {    30000,   0x2d,   0x40,   7},
 {    30000,   0x2e,   0x40,   7},
 {    30000,   0x2f,   0x40,   7},
 {    30000,   0x30,   0x40,   6},
 {    30000,   0x31,   0x40,   6},
 {    30000,   0x32,   0x40,   6},
 {    30000,   0x33,   0x40,   6},
 {    30000,   0x34,   0x40,   6},
 {    30000,   0x35,   0x40,   6},
 {    30000,   0x36,   0x40,   6},
 {    30000,   0x37,   0x40,   6},
 {    30000,   0x38,   0x40,   6},
 {    30000,   0x39,   0x40,   5},
 {    30000,   0x3a,   0x40,   5},

};





PS4100_AE_lut_t PS4100_AE_lut_50Hz_min_25fps[] = {
 {       10,   0x00,   0x40,   16},
 {       11,   0x00,   0x40,   16},
 {       12,   0x00,   0x40,   16},
 {       13,   0x00,   0x40,   16},
 {       14,   0x00,   0x40,   16},
 {       15,   0x00,   0x40,   16},
 {       16,   0x00,   0x40,   16},
 {       17,   0x00,   0x40,   16},
 {       18,   0x00,   0x40,   16},
 {       19,   0x00,   0x40,   16},
 {       20,   0x00,   0x40,   16},
 {       21,   0x00,   0x40,   16},
 {       22,   0x00,   0x40,   16},
 {       23,   0x00,   0x40,   16},
 {       24,   0x00,   0x40,   16},
 {       25,   0x00,   0x40,   16},
 {       26,   0x00,   0x40,   16},
 {       27,   0x00,   0x40,   16},
 {       28,   0x00,   0x40,   16},
 {       29,   0x00,   0x40,   16},
 {       30,   0x00,   0x40,   16},
 {       31,   0x00,   0x40,   16},
 {       32,   0x00,   0x40,   16},
 {       33,   0x00,   0x40,   16},
 {       34,   0x00,   0x40,   16},
 {       35,   0x00,   0x40,   16},
 {       36,   0x00,   0x40,   16},
 {       37,   0x00,   0x40,   16},
 {       38,   0x00,   0x40,   16},
 {       39,   0x00,   0x40,   16},
 {       40,   0x00,   0x40,   16},
 {       42,   0x00,   0x40,   16},
 {       44,   0x00,   0x40,   16},
 {       46,   0x00,   0x40,   16},
 {       48,   0x00,   0x40,   16},
 {       50,   0x00,   0x40,   16},
 {       52,   0x00,   0x40,   16},
 {       55,   0x00,   0x40,   16},
 {       59,   0x00,   0x40,   16},
 {       64,   0x00,   0x40,   16},
 {       72,   0x00,   0x40,   16},
 {       80,   0x00,   0x40,   16},
 {       88,   0x00,   0x40,   16},
 {       96,   0x00,   0x40,   16},
 {      104,   0x00,   0x40,   16},
 {      112,   0x00,   0x40,   16},
 {      120,   0x00,   0x40,   16},
 {      132,   0x00,   0x40,   16},
 {      144,   0x00,   0x40,   16},
 {      156,   0x00,   0x40,   16},
 {      168,   0x00,   0x40,   16},
 {      180,   0x00,   0x40,   16},
 {      192,   0x00,   0x40,   16},
 {      204,   0x00,   0x40,   16},
 {      216,   0x00,   0x40,   16},
 {      228,   0x00,   0x40,   16},
 {      240,   0x00,   0x40,   16},
 {      252,   0x00,   0x40,   15},
 {      264,   0x00,   0x40,   15},
 {      276,   0x00,   0x40,   15},
 {      288,   0x00,   0x40,   15},
 {      300,   0x00,   0x40,   15},
 {      312,   0x00,   0x40,   15},
 {      324,   0x00,   0x40,   15},
 {      336,   0x00,   0x40,   15},
 {      360,   0x00,   0x40,   15},
 {      376,   0x00,   0x40,   15},
 {      392,   0x00,   0x40,   15},
 {      408,   0x00,   0x40,   15},
 {      424,   0x00,   0x40,   15},
 {      448,   0x00,   0x40,   15},
 {      472,   0x00,   0x40,   15},
 {      496,   0x00,   0x40,   15},
 {      528,   0x00,   0x40,   15},
 {      560,   0x00,   0x40,   15},
 {      592,   0x00,   0x40,   15},
 {      632,   0x00,   0x40,   15},
 {      672,   0x00,   0x40,   15},
 {      712,   0x00,   0x40,   14},
 {      752,   0x00,   0x40,   14},
 {      824,   0x00,   0x40,   14},
 {      896,   0x00,   0x40,   14},
 {      968,   0x00,   0x40,   14},
 {     1040,   0x00,   0x40,   14},
 {     1112,   0x00,   0x40,   14},
 {     1184,   0x00,   0x40,   14},
 {     1264,   0x00,   0x40,   14},
 {     1344,   0x00,   0x40,   14},
 {     1424,   0x00,   0x40,   14},
 {     1504,   0x00,   0x40,   14},
 {     1592,   0x00,   0x40,   14},
 {     1680,   0x00,   0x40,   14},
 {     1776,   0x00,   0x40,   14},
 {     1880,   0x00,   0x40,   14},
 {     1992,   0x00,   0x40,   14},
 {     2112,   0x00,   0x40,   14},
 {     2256,   0x00,   0x40,   14},
 {     2408,   0x00,   0x40,   13},
 {     2576,   0x00,   0x40,   13},
 {     2744,   0x00,   0x40,   13},
 {     2880,   0x00,   0x40,   13},
 {     3008,   0x00,   0x40,   13},
 {     3184,   0x00,   0x40,   13},
 {     3384,   0x00,   0x40,   13},
 {     3600,   0x00,   0x40,   13},
 {     3880,   0x00,   0x40,   13},
 {     4160,   0x00,   0x40,   13},
 {     4580,   0x00,   0x40,   13},
 {     5004,   0x00,   0x40,   13},
 {     5404,   0x00,   0x40,   13},
 {     5850,   0x00,   0x40,   13},
 {     6300,   0x00,   0x40,   13},
 {     6850,   0x00,   0x40,   13},
 {     7400,   0x00,   0x40,   13},
 {     8050,   0x00,   0x40,   13},
 {     8650,   0x00,   0x40,   13},
 {     9300,   0x00,   0x40,   13},
 {    10000,   0x00,   0x40,   12},
 {    10000,   0x01,   0x40,   12},
 {    10000,   0x02,   0x40,   12},
 {    10000,   0x03,   0x40,   12},
 {    10000,   0x04,   0x40,   12},
 {    10000,   0x05,   0x40,   12},
 {    10000,   0x06,   0x40,   12},
 {    10000,   0x07,   0x40,   12},
 {    10000,   0x08,   0x40,   12},
 {    10000,   0x0a,   0x40,   12},
 {    10000,   0x0b,   0x40,   12},
 {    10000,   0x0c,   0x40,   12},
 {    10000,   0x0d,   0x40,   12},
 {    10000,   0x0e,   0x40,   12},
 {    10000,   0x0f,   0x40,   12},
 {    10000,   0x10,   0x40,   11},
 {    10000,   0x11,   0x40,   11},
 {    10000,   0x12,   0x40,   11},
 {    10000,   0x13,   0x40,   11},
 {    10000,   0x14,   0x40,   11},
 {    10000,   0x15,   0x40,   11},
 {    10000,   0x16,   0x40,   11},
 {    10000,   0x17,   0x40,   11},
 {    10000,   0x18,   0x40,   11},
 {    10000,   0x19,   0x40,   11},
 {    10000,   0x1a,   0x40,   10},
 {    10000,   0x1b,   0x40,   10},
 {    10000,   0x1c,   0x40,   10},
 {    10000,   0x1d,   0x40,   10},
 {    10000,   0x1e,   0x40,   10},
 {    10000,   0x1f,   0x40,   10},
 {    20000,   0x0e,   0x40,   10},
 {    20000,   0x0f,   0x40,   10},
 {    20000,   0x10,   0x40,   10},
 {    20000,   0x11,   0x40,   10},
 {    20000,   0x12,   0x40,   9},
 {    20000,   0x13,   0x40,   9},
 {    20000,   0x14,   0x40,   9},
 {    20000,   0x15,   0x40,   9},
 {    20000,   0x16,   0x40,   9},
 {    20000,   0x17,   0x40,   9},
 {    20000,   0x18,   0x40,   9},
 {    30000,   0x10,   0x40,   8},
 {    30000,   0x11,   0x40,   8},
 {    30000,   0x12,   0x40,   8},
 {    30000,   0x13,   0x40,   8},
 {    30000,   0x14,   0x40,   8},
 {    30000,   0x15,   0x40,   8},
 {    30000,   0x16,   0x40,   8},
 {    30000,   0x17,   0x40,   8},
 {    30000,   0x18,   0x40,   8},
 {    40000,   0x10,   0x40,   7},
 {    40000,   0x11,   0x40,   7},
 {    40000,   0x12,   0x40,   7},
 {    40000,   0x13,   0x40,   7},
 {    40000,   0x14,   0x40,   7},
 {    40000,   0x15,   0x40,   7},
 {    40000,   0x16,   0x40,   7},
 {    40000,   0x17,   0x40,   7},
 {    40000,   0x18,   0x40,   7},
 {    40000,   0x19,   0x40,   7},
 {    40000,   0x1A,   0x40,   7},
 {    40000,   0x1B,   0x40,   7},
 {    40000,   0x1C,   0x40,   7},
 {    40000,   0x1d,   0x40,   7},
 {    40000,   0x1E,   0x40,   7},
 {    40000,   0x1F,   0x40,   7},
 {    40000,   0x20,   0x40,   6},
 {    40000,   0x21,   0x40,   6},
 {    40000,   0x22,   0x40,   6},
 {    40000,   0x23,   0x40,   6},
 {    40000,   0x24,   0x40,   6},
 {    40000,   0x25,   0x40,   6},
 {    40000,   0x26,   0x40,   6},
 {    40000,   0x27,   0x40,   6},
 {    40000,   0x28,   0x40,   6},
 {    40000,   0x29,   0x40,   5},
 {    40000,   0x2a,   0x40,   5},
 {    40000,   0x2b,   0x40,   5},
 {    40000,   0x2c,   0x40,   5},
 {    40000,   0x2d,   0x40,   5},
 {    40000,   0x2e,   0x40,   5},
 {    40000,   0x2f,   0x40,   5},
 {    40000,   0x30,   0x40,   5},
 {    40000,   0x31,   0x40,   5},
 {    40000,   0x32,   0x40,   6},
 {    40000,   0x33,   0x40,   6},
 {    40000,   0x34,   0x40,   6},
 {    40000,   0x35,   0x40,   6},
 {    40000,   0x36,   0x40,   6},
 {    40000,   0x37,   0x40,   6},
 {    40000,   0x38,   0x40,   6},
 {    40000,   0x39,   0x40,   5},
 {    40000,   0x3a,   0x40,   5},
};
