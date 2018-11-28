#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"
#include "NT99143.h"


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

NT99143_AE_lut_t NT99143_AE_lut_50Hz[] = {
 {       40,   0x00,   0,   16},
 {       44,   0x00,   0,   16},
 {       48,   0x00,   0,   16},
 {       52,   0x00,   0,   16},
 {       56,   0x00,   0,   16},
 {       64,   0x00,   0,   16},
 {       72,   0x00,   0,   16},
 {       80,   0x00,   0,   16},
 {       88,   0x00,   0,   16},
 {       96,   0x00,   0,   16},
 {      104,   0x00,   0,   16},
 {      112,   0x00,   0,   16},
 {      120,   0x00,   0,   16},
 {      132,   0x00,   0,   16},
 {      144,   0x00,   0,   16},
 {      156,   0x00,   0,   16},
 {      168,   0x00,   0,   16},
 {      180,   0x00,   0,   16},
 {      192,   0x00,   0,   16},
 {      204,   0x00,   0,   16},
 {      216,   0x00,   0,   16},
 {      228,   0x00,   0,   16},
 {      240,   0x00,   0,   16},
 {      252,   0x00,   0,   15},
 {      264,   0x00,   0,   15},
 {      276,   0x00,   0,   15},
 {      288,   0x00,   0,   15},
 {      300,   0x00,   0,   15},
 {      312,   0x00,   0,   15},
 {      324,   0x00,   0,   15},
 {      336,   0x00,   0,   15},
 {      360,   0x00,   0,   15},
 {      376,   0x00,   0,   15},
 {      392,   0x00,   0,   15},
 {      408,   0x00,   0,   15},
 {      424,   0x00,   0,   15},
 {      448,   0x00,   0,   15},
 {      472,   0x00,   0,   15},
 {      496,   0x00,   0,   15},
 {      528,   0x00,   0,   15},
 {      560,   0x00,   0,   15},
 {      592,   0x00,   0,   15},
 {      632,   0x00,   0,   15},
 {      672,   0x00,   0,   15},
 {      712,   0x00,   0,   14},
 {      752,   0x00,   0,   14},
 {      824,   0x00,   0,   14},
 {      896,   0x00,   0,   14},
 {      968,   0x00,   0,   14},
 {     1040,   0x00,   0,   14},
 {     1112,   0x00,   0,   14},
 {     1184,   0x00,   0,   14},
 {     1264,   0x00,   0,   14},
 {     1344,   0x00,   0,   14},
 {     1424,   0x00,   0,   14},
 {     1504,   0x00,   0,   14},
 {     1592,   0x00,   0,   14},
 {     1680,   0x00,   0,   14},
 {     1776,   0x00,   0,   14},
 {     1880,   0x00,   0,   14},
 {     1992,   0x00,   0,   14},
 {     2112,   0x00,   0,   14},
 {     2256,   0x00,   0,   14},
 {     2408,   0x00,   0,   13},
 {     2576,   0x00,   0,   13},
 {     2744,   0x00,   0,   13},
 {     2880,   0x00,   0,   13},
 {     3008,   0x00,   0,   13},
 {     3184,   0x00,   0,   13},
 {     3384,   0x00,   0,   13},
 {     3600,   0x00,   0,   13},
 {     3880,   0x00,   0,   13},
 {     4160,   0x00,   0,   13},
 {     4580,   0x00,   0,   13},
 {     5044,   0x00,   0,   13},
 {     5404,   0x00,   0,   13},
 {     5684,   0x00,   0,   13},
 {     6360,   0x00,   0,   13},
 {     7004,   0x00,   0,   13},
 {     7764,   0x00,   0,   13},
 {     8644,   0x00,   0,   13},
 {     9364,   0x00,   0,   13},
 {    10000,   0x00,   0,   12},
 {    10000,   0x01,   0,   12},
 {    10000,   0x02,   0,   12},
 {    10000,   0x03,   0,   12},
 {    10000,   0x04,   0,   12},
 {    10000,   0x06,   0,   12},
 {    10000,   0x07,   0,   12},
 {    10000,   0x08,   0,   12},
 {    10000,   0x0a,   0,   12},
 {    10000,   0x0c,   0,   12},
 {    10000,   0x0e,   0,   12},
 {    10000,   0x10,   0,   11},
 {    10000,   0x12,   0,   11},
 {    10000,   0x14,   0,   11},
 {    10000,   0x16,   0,   11},
 {    10000,   0x18,   0,   11},
 {    10000,   0x1a,   0,   10},
 {    10000,   0x1c,   0,   10},
 {    10000,   0x1e,   0,   10},
 {    10000,   0x30,   0,   10},
 {    20000,   0x10,   0,   10},
 {    20000,   0x12,   0,   9},
 {    20000,   0x14,   0,   9},
 {    20000,   0x16,   0,   9},
 {    20000,   0x18,   0,   9},
 {    30000,   0x10,   0,   8},
 {    30000,   0x12,   0,   8},
 {    30000,   0x14,   0,   8},
 {    30000,   0x16,   0,   8},
 {    30000,   0x18,   0,   8},
 {    30000,   0x1A,   0,   8},
 {    30000,   0x1C,   0,   8},
 {    30000,   0x1E,   0,   8},
 {    30000,   0x30,   0,   7},
 {    30000,   0x32,   0,   7},
 {    30000,   0x34,   0,   7},
 {    30000,   0x36,   0,   7},
 {    30000,   0x38,   0,   7},
 {    30000,   0x3a,   0,   7},
 {    30000,   0x3c,   0,   7},
 {    30000,   0x3e,   0,   6},
 {    30000,   0x3e,   0,   6},
 {    30000,   0x3e,   0,   6},
#if 0
 {    40000,   0x38,   0,   5},
 {    40000,   0x3a,   0,   5},
 {    40000,   0x3c,   0,   5},
 {    40000,   0x3e,   0,   5},
 {    40000,   0x70,   0,   4},
 {    40000,   0x72,   0,   4},
 {    40000,   0x74,   0,   4},
 {    40000,   0x76,   0,   4},
 {    40000,   0x78,   0,   4},
 {    40000,   0x7a,   0,   3},
 {    40000,   0x7c,   0,   3},
 {    40000,   0x7e,   0,   3},
 {    40000,   0x7f,   0,   3},
#endif
};
