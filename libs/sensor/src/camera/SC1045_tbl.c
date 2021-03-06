
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"
#include "SC1045.h"


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








#if 1

SC1045_AE_lut_t SC1045_AE_lut_50Hz_min_25fps[] = {
    /*
 {       1,   0x00,   0,   16},
 {       1,   0x00,   0,   16},
 {       2,   0x00,   0,   16},
 {       2,   0x00,   0,   16},
 {       3,   0x00,   0,   16},
 {       3,   0x00,   0,   16},
 */
 {       4,   0x00,   0,   16},
 {       4,   0x00,   0,   16},
 {       4,   0x00,   0,   16},
 {       5,   0x00,   0,   16},
 {       5,   0x00,   0,   16},
 {       5,   0x00,   0,   16},
 {       6,   0x00,   0,   16},
 {       6,   0x00,   0,   16},
 {       6,   0x00,   0,   16},
 {       7,   0x00,   0,   16},
 {       7,   0x00,   0,   16},
 {       7,   0x00,   0,   16},
 {       8,   0x00,   0,   16},
 {       8,   0x00,   0,   16},
 {       8,   0x00,   0,   16},
 {       9,   0x00,   0,   16},
 {       9,   0x00,   0,   16},
 {       9,   0x00,   0,   16},
 {       10,   0x00,   0,   16},
 {       10,   0x00,   0,   16},
 {       11,   0x00,   0,   16},
 {       11,   0x00,   0,   16},
 {       12,   0x00,   0,   16},
 {       12,   0x00,   0,   16},
 {       13,   0x00,   0,   16},
 {       13,   0x00,   0,   16},
 {       14,   0x00,   0,   16},
 {       14,   0x00,   0,   16},
 {       15,   0x00,   0,   16},
 {       15,   0x00,   0,   16},
 {       16,   0x00,   0,   16},
 {       16,   0x00,   0,   16},
 {       17,   0x00,   0,   16},
 {       17,   0x00,   0,   16},
 {       18,   0x00,   0,   16},
 {       18,   0x00,   0,   16},
 {       19,   0x00,   0,   16},
 {       19,   0x00,   0,   16},
 {       20,   0x00,   0,   16},
 {       20,   0x00,   0,   16},
 {       21,   0x00,   0,   16},
 {       22,   0x00,   0,   16},
 {       23,   0x00,   0,   16},
 {       24,   0x00,   0,   16},
 {       25,   0x00,   0,   16},
 {       26,   0x00,   0,   16},
 {       27,   0x00,   0,   16},
 {       28,   0x00,   0,   16},
 {       29,   0x00,   0,   16},
 {       30,   0x00,   0,   16},
 {       31,   0x00,   0,   16},
 {      32,   0x00,   0,   16},
 {      33,   0x00,   0,   16},
 {      34,   0x00,   0,   16},
 {      35,   0x00,   0,   16},
 {      36,   0x00,   0,   16},
 {      37,   0x00,   0,   16},
 {      38,   0x00,   0,   16},
 {      39,   0x00,   0,   16},
 {      40,   0x00,   0,   16},
 {      42,   0x00,   0,   16},
 {      44,   0x00,   0,   16},
 {      46,   0x00,   0,   16},
 {      48,   0x00,   0,   16},
 {      50,   0x00,   0,   15},
 {      52,   0x00,   0,   15},
 {      54,   0x00,   0,   15},
 {      56,   0x00,   0,   15},
 {      58,   0x00,   0,   15},
 {      60,   0x00,   0,   15},
 {      62,   0x00,   0,   15},
 {      64,   0x00,   0,   15},
 {      68,   0x00,   0,   15},
 {      70,   0x00,   0,   15},
 {      72,   0x00,   0,   15},
 {      74,   0x00,   0,   15},
 {      76,   0x00,   0,   15},
 {      78,   0x00,   0,   15},
 {      80,   0x00,   0,   15},
 {      82,   0x00,   0,   15},
 {      84,   0x00,   0,   15},
 {      87,   0x00,   0,   15},
 {      90,   0x00,   0,   15},
 {      93,   0x00,   0,   15},
 {      96,   0x00,   0,   15},
 {      99,   0x00,   0,   14},
 {      102,   0x00,   0,   14},
 {      105,   0x00,   0,   14},
 {      108,   0x00,   0,   14},
 {      111,   0x00,   0,   14},
 {     114,   0x00,   0,   14},
 {     117,   0x00,   0,   14},
 {     120,   0x00,   0,   14},
 {     124,   0x00,   0,   14},
 {     128,   0x00,   0,   14},
 {     132,   0x00,   0,   14},
 {     136,   0x00,   0,   14},
 {     140,   0x00,   0,   14},
 {     144,   0x00,   0,   13},
 {     148,   0x00,   0,   13},
 {     152,   0x00,   0,   13},
 {     156,   0x00,   0,   13},
 {     160,   0x00,   0,   13},
 {     164,   0x00,   0,   13},
 {     168,   0x00,   0,   13},
 {     172,   0x00,   0,   13},
 {     176,   0x00,   0,   13},
 {     180,   0x00,   0,   13},
 {    188,   0x00,   0,   12},
 {    188,   0x01,   0,   12},
 {    188,   0x02,   0,   12},
 {    188,   0x03,   0,   12},
 {    188,   0x04,   0,   12},
 {    188,   0x06,   0,   12},
 {    188,   0x07,   0,   12},
 {    188,   0x08,   0,   12},
 {    188,   0x09,   0,   12},
 {    188,   0x0a,   0,   12},
 {    188,   0x0b,   0,   12},
 {    188,   0x0c,   0,   12},
 {    188,   0x0d,   0,   12},
 {    188,   0x0e,   0,   12},
 {    188,   0x0f,   0,   12},
 {    188,   0x10,   0,   11},
 {    188,   0x11,   0,   11},
 {    188,   0x12,   0,   11},
 {    188,   0x13,   0,   11},
 {    188,   0x14,   0,   11},
 {    188,   0x15,   0,   11},
 {    188,   0x16,   0,   11},
 {    188,   0x17,   0,   11},
 {    188,   0x18,   0,   11},
 {    188,   0x19,   0,   11},
 {    188,   0x1a,   0,   10},
 {    188,   0x1b,   0,   10},
 {    188,   0x1c,   0,   10},
 {    188,   0x1d,   0,   10},
 {    188,   0x1e,   0,   10},
 {    188,   0x1f,   0,   10},
 {    188,   0x30,   0,   10},
 {    188*2,   0x10,   0,   10},
 {    188*2,   0x11,   0,   9},
 {    188*2,   0x12,   0,   9},
 {    188*2,   0x13,   0,   9},
 {    188*2,   0x14,   0,   9},
 {    188*2,   0x15,   0,   9},
 {    188*2,   0x16,   0,   9},
 {    188*2,   0x17,   0,   9},
 {    188*2,   0x18,   0,   9},
 {    188*2,   0x19,   0,   9},
 {    188*3,   0x10,   0,   8},
 {    188*3,   0x11,   0,   8},
 {    188*3,   0x12,   0,   8},
 {    188*3,   0x13,   0,   8},
 {    188*3,   0x14,   0,   8},
 {    188*3,   0x15,   0,   8},
 {    188*3,   0x16,   0,   8},
 {    188*3,   0x17,   0,   8},
 {    188*3,   0x18,   0,   8},
 {    188*3,   0x19,   0,   8},
 {    188*3,   0x1A,   0,   8},
 {    188*3,   0x1B,   0,   8},
 {    188*3,   0x1C,   0,   8},
 {    188*3,   0x1D,   0,   8},
 {    188*3,   0x1E,   0,   8},
 {    188*3,   0x1F,   0,   8},
 {    188*3,   0x30,   0,   7},
 {    188*3,   0x31,   0,   7},
 {    188*3,   0x32,   0,   7},
 {    188*3,   0x33,   0,   7},
 {    188*3,   0x34,   0,   7},
 {    188*3,   0x35,   0,   7},
 {    188*3,   0x36,   0,   7},
 {    188*3,   0x37,   0,   7},
 {    188*3,   0x38,   0,   6},
 {    188*3,   0x39,   0,   6},
 {    188*3,   0x3a,   0,   6},
 {    188*3,   0x3b,   0,   6},
 {    188*3,   0x3c,   0,   6},
 {    188*3,   0x3d,   0,   6},
 {    188*3,   0x3e,   0,   6},
 {    188*4,   0x38,   0,   5},
 {    188*4,   0x39,   0,   5},
 {    188*4,   0x3a,   0,   5},
 {    188*4,   0x3b,   0,   5},
 {    188*4,   0x3c,   0,   5},
 {    188*4,   0x3d,   0,   5},
 {    188*4,   0x3e,   0,   5},
 {    188*4,   0x3f,   0,   5},
 {    188*4,   0x70,   0,   4},
 {    188*4,   0x71,   0,   4},
 {    188*4,   0x72,   0,   4},
 {    188*4,   0x73,   0,   4},
 {    188*4,   0x74,   0,   4},
 {    188*4,   0x75,   0,   4},
 {    188*4,   0x76,   0,   4},
 {    188*4,   0x77,   0,   4},
 {    188*4,   0x78,   0,   4},
 {    188*4,   0x79,   0,   4},
 {    188*4,   0x7a,   0,   3},
 {    188*4,   0x7b,   0,   3},
 {    188*4,   0x7c,   0,   3},
 {    188*4,   0x7d,   0,   3},
 {    188*4,   0x7e,   0,   3},
 {    188*4,   0x7f,   0,   3},

};


SC1045_AE_lut_t SC1045_AE_lut_50Hz_min_30fps[] = {

    /*
 {       1,   0x00,   0,   16},
 {       1,   0x00,   0,   16},
 {       2,   0x00,   0,   16},
 {       2,   0x00,   0,   16},
 {       3,   0x00,   0,   16},
 {       3,   0x00,   0,   16},
 */
 {       4,   0x00,   0,   16},
 {       4,   0x00,   0,   16},
 {       4,   0x00,   0,   16},
 {       5,   0x00,   0,   16},
 {       5,   0x00,   0,   16},
 {       5,   0x00,   0,   16},
 {       6,   0x00,   0,   16},
 {       6,   0x00,   0,   16},
 {       6,   0x00,   0,   16},
 {       7,   0x00,   0,   16},
 {       7,   0x00,   0,   16},
 {       7,   0x00,   0,   16},
 {       8,   0x00,   0,   16},
 {       8,   0x00,   0,   16},
 {       8,   0x00,   0,   16},
 {       9,   0x00,   0,   16},
 {       9,   0x00,   0,   16},
 {       9,   0x00,   0,   16},
 {       10,   0x00,   0,   16},
 {       10,   0x00,   0,   16},
 {       11,   0x00,   0,   16},
 {       11,   0x00,   0,   16},
 {       12,   0x00,   0,   16},
 {       12,   0x00,   0,   16},
 {       13,   0x00,   0,   16},
 {       13,   0x00,   0,   16},
 {       14,   0x00,   0,   16},
 {       14,   0x00,   0,   16},
 {       15,   0x00,   0,   16},
 {       15,   0x00,   0,   16},
 {       16,   0x00,   0,   16},
 {       16,   0x00,   0,   16},
 {       17,   0x00,   0,   16},
 {       17,   0x00,   0,   16},
 {       18,   0x00,   0,   16},
 {       18,   0x00,   0,   16},
 {       19,   0x00,   0,   16},
 {       19,   0x00,   0,   16},
 {       20,   0x00,   0,   16},
 {       20,   0x00,   0,   16},
 {       21,   0x00,   0,   16},
 {       22,   0x00,   0,   16},
 {       23,   0x00,   0,   16},
 {       24,   0x00,   0,   16},
 {       25,   0x00,   0,   16},
 {       26,   0x00,   0,   16},
 {       27,   0x00,   0,   16},
 {       28,   0x00,   0,   16},
 {       29,   0x00,   0,   16},
 {       30,   0x00,   0,   16},
 {       31,   0x00,   0,   16},
 {      32,   0x00,   0,   16},
 {      33,   0x00,   0,   16},
 {      34,   0x00,   0,   16},
 {      35,   0x00,   0,   16},
 {      36,   0x00,   0,   16},
 {      37,   0x00,   0,   16},
 {      38,   0x00,   0,   16},
 {      39,   0x00,   0,   16},
 {      40,   0x00,   0,   16},
 {      42,   0x00,   0,   16},
 {      44,   0x00,   0,   16},
 {      46,   0x00,   0,   16},
 {      48,   0x00,   0,   16},
 {      50,   0x00,   0,   15},
 {      52,   0x00,   0,   15},
 {      54,   0x00,   0,   15},
 {      56,   0x00,   0,   15},
 {      58,   0x00,   0,   15},
 {      60,   0x00,   0,   15},
 {      62,   0x00,   0,   15},
 {      64,   0x00,   0,   15},
 {      68,   0x00,   0,   15},
 {      70,   0x00,   0,   15},
 {      72,   0x00,   0,   15},
 {      74,   0x00,   0,   15},
 {      76,   0x00,   0,   15},
 {      78,   0x00,   0,   15},
 {      80,   0x00,   0,   15},
 {      82,   0x00,   0,   15},
 {      84,   0x00,   0,   15},
 {      87,   0x00,   0,   15},
 {      90,   0x00,   0,   15},
 {      93,   0x00,   0,   15},
 {      96,   0x00,   0,   15},
 {      99,   0x00,   0,   14},
 {      102,   0x00,   0,   14},
 {      105,   0x00,   0,   14},
 {      108,   0x00,   0,   14},
 {      111,   0x00,   0,   14},
 {     114,   0x00,   0,   14},
 {     117,   0x00,   0,   14},
 {     120,   0x00,   0,   14},
 {     124,   0x00,   0,   14},
 {     128,   0x00,   0,   14},
 {     132,   0x00,   0,   14},
 {     136,   0x00,   0,   14},
 {     140,   0x00,   0,   14},
 {     144,   0x00,   0,   14},
 {     148,   0x00,   0,   14},
 {     152,   0x00,   0,   14},
 {     156,   0x00,   0,   14},
 {     160,   0x00,   0,   14},
 {     164,   0x00,   0,   14},
 {     168,   0x00,   0,   13},
 {     172,   0x00,   0,   13},
 {     176,   0x00,   0,   13},
 {     180,   0x00,   0,   13},
 {     186,   0x00,   0,   13},
 {     192,   0x00,   0,   13},
 {     198,   0x00,   0,   13},
 {     204,   0x00,   0,   13},
 {     210,   0x00,   0,   13},
 {     218,   0x00,   0,   13},
 {     227,   0x00,   0,   13},
 {    227,   0x00,   0,   12},
 {    227,   0x01,   0,   12},
 {    227,   0x02,   0,   12},
 {    227,   0x03,   0,   12},
 {    227,   0x04,   0,   12},
 {    227,   0x06,   0,   12},
 {    227,   0x07,   0,   12},
 {    227,   0x08,   0,   12},
 {    227,   0x09,   0,   12},
 {    227,   0x0a,   0,   12},
 {    227,   0x0b,   0,   12},
 {    227,   0x0c,   0,   12},
 {    227,   0x0d,   0,   12},
 {    227,   0x0e,   0,   12},
 {    227,   0x0f,   0,   12},
 {    227,   0x10,   0,   11},
 {    227,   0x11,   0,   11},
 {    227,   0x12,   0,   11},
 {    227,   0x13,   0,   11},
 {    227,   0x14,   0,   11},
 {    227,   0x15,   0,   11},
 {    227,   0x16,   0,   11},
 {    227,   0x17,   0,   11},
 {    227,   0x18,   0,   11},
 {    227,   0x19,   0,   11},
 {    227,   0x1a,   0,   10},
 {    227,   0x1b,   0,   10},
 {    227,   0x1c,   0,   10},
 {    227,   0x1d,   0,   10},
 {    227,   0x1e,   0,   10},
 {    227,   0x1f,   0,   10},
 {    227,   0x30,   0,   10},
 {    227*2,   0x10,   0,   10},
 {    227*2,   0x11,   0,   9},
 {    227*2,   0x12,   0,   9},
 {    227*2,   0x13,   0,   9},
 {    227*2,   0x14,   0,   9},
 {    227*2,   0x15,   0,   9},
 {    227*2,   0x16,   0,   9},
 {    227*2,   0x17,   0,   9},
 {    227*2,   0x18,   0,   9},
 {    227*2,   0x19,   0,   9},
 {    227*3,   0x10,   0,   8},
 {    227*3,   0x11,   0,   8},
 {    227*3,   0x12,   0,   8},
 {    227*3,   0x13,   0,   8},
 {    227*3,   0x14,   0,   8},
 {    227*3,   0x15,   0,   8},
 {    227*3,   0x16,   0,   8},
 {    227*3,   0x17,   0,   8},
 {    227*3,   0x18,   0,   8},
 {    227*3,   0x19,   0,   8},
 {    227*3,   0x1A,   0,   8},
 {    227*3,   0x1B,   0,   8},
 {    227*3,   0x1C,   0,   8},
 {    227*3,   0x1D,   0,   8},
 {    227*3,   0x1E,   0,   8},
 {    227*3,   0x1F,   0,   8},
 {    227*3,   0x30,   0,   7},
 {    227*3,   0x31,   0,   7},
 {    227*3,   0x32,   0,   7},
 {    227*3,   0x33,   0,   7},
 {    227*3,   0x34,   0,   7},
 {    227*3,   0x35,   0,   7},
 {    227*3,   0x36,   0,   7},
 {    227*3,   0x37,   0,   7},
 {    227*3,   0x38,   0,   7},
 {    227*3,   0x39,   0,   7},
 {    227*3,   0x3a,   0,   7},
 {    227*3,   0x3b,   0,   7},
 {    227*3,   0x3c,   0,   7},
 {    227*3,   0x3d,   0,   7},
 {    227*3,   0x3e,   0,   6},
 {    227*3,   0x3f,   0,   6},
 {    227*3,   0x70,   0,   6},
 {    227*3,   0x71,   0,   6},
 {    227*3,   0x72,   0,   6},
 {    227*3,   0x73,   0,   6},
 {    227*3,   0x74,   0,   5},
 {    227*3,   0x75,   0,   5},
 {    227*3,   0x76,   0,   5},
};

#else



SC1045_AE_lut_t SC1045_AE_lut_50Hz_min_25fps[] = {
 {       10,   0x00,   0,   16},
 {       11,   0x00,   0,   16},
 {       12,   0x00,   0,   16},
 {       13,   0x00,   0,   16},
 {       14,   0x00,   0,   16},
 {       15,   0x00,   0,   16},
 {       16,   0x00,   0,   16},
 {       17,   0x00,   0,   16},
 {       18,   0x00,   0,   16},
 {       19,   0x00,   0,   16},
 {       20,   0x00,   0,   16},
 {       21,   0x00,   0,   16},
 {       22,   0x00,   0,   16},
 {       24,   0x00,   0,   16},
 {       26,   0x00,   0,   16},
 {       28,   0x00,   0,   16},
 {       30,   0x00,   0,   16},
 {       32,   0x00,   0,   16},
 {       34,   0x00,   0,   16},
 {       36,   0x00,   0,   16},
 {       38,   0x00,   0,   16},
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
 {    10000,   0x09,   0,   12},
 {    10000,   0x0a,   0,   12},
 {    10000,   0x0b,   0,   12},
 {    10000,   0x0c,   0,   12},
 {    10000,   0x0d,   0,   12},
 {    10000,   0x0e,   0,   12},
 {    10000,   0x0f,   0,   12},
 {    10000,   0x10,   0,   11},
 {    10000,   0x11,   0,   11},
 {    10000,   0x12,   0,   11},
 {    10000,   0x13,   0,   11},
 {    10000,   0x14,   0,   11},
 {    10000,   0x15,   0,   11},
 {    10000,   0x16,   0,   11},
 {    10000,   0x17,   0,   11},
 {    10000,   0x18,   0,   11},
 {    10000,   0x19,   0,   11},
 {    10000,   0x1a,   0,   10},
 {    10000,   0x1b,   0,   10},
 {    10000,   0x1c,   0,   10},
 {    10000,   0x1d,   0,   10},
 {    10000,   0x1e,   0,   10},
 {    10000,   0x1f,   0,   10},
 {    10000,   0x30,   0,   10},
 {    20000,   0x10,   0,   10},
 {    20000,   0x11,   0,   9},
 {    20000,   0x12,   0,   9},
 {    20000,   0x13,   0,   9},
 {    20000,   0x14,   0,   9},
 {    20000,   0x15,   0,   9},
 {    20000,   0x16,   0,   9},
 {    20000,   0x17,   0,   9},
 {    20000,   0x18,   0,   9},
 {    20000,   0x19,   0,   9},
 {    30000,   0x10,   0,   8},
 {    30000,   0x11,   0,   8},
 {    30000,   0x12,   0,   8},
 {    30000,   0x13,   0,   8},
 {    30000,   0x14,   0,   8},
 {    30000,   0x15,   0,   8},
 {    30000,   0x16,   0,   8},
 {    30000,   0x17,   0,   8},
 {    30000,   0x18,   0,   8},
 {    30000,   0x19,   0,   8},
 {    30000,   0x1A,   0,   8},
 {    30000,   0x1B,   0,   8},
 {    30000,   0x1C,   0,   8},
 {    30000,   0x1D,   0,   8},
 {    30000,   0x1E,   0,   8},
 {    30000,   0x1F,   0,   8},
 {    30000,   0x30,   0,   7},
 {    30000,   0x31,   0,   7},
 {    30000,   0x32,   0,   7},
 {    30000,   0x33,   0,   7},
 {    30000,   0x34,   0,   7},
 {    30000,   0x35,   0,   7},
 {    30000,   0x36,   0,   7},
 {    30000,   0x37,   0,   7},
 {    30000,   0x38,   0,   6},
 {    30000,   0x39,   0,   6},
 {    30000,   0x3a,   0,   6},
 {    30000,   0x3b,   0,   6},
 {    30000,   0x3c,   0,   6},
 {    30000,   0x3d,   0,   6},
 {    30000,   0x3e,   0,   6},
 {    40000,   0x38,   0,   5},
 {    40000,   0x39,   0,   5},
 {    40000,   0x3a,   0,   5},
 {    40000,   0x3b,   0,   5},
 {    40000,   0x3c,   0,   5},
 {    40000,   0x3d,   0,   5},
 {    40000,   0x3e,   0,   5},
 {    40000,   0x3f,   0,   5},
 {    40000,   0x70,   0,   4},
 {    40000,   0x71,   0,   4},
 {    40000,   0x72,   0,   4},
 {    40000,   0x73,   0,   4},
 {    40000,   0x74,   0,   4},
 {    40000,   0x75,   0,   4},
 {    40000,   0x76,   0,   4},
 {    40000,   0x77,   0,   4},
 {    40000,   0x78,   0,   4},
 {    40000,   0x79,   0,   4},
 {    40000,   0x7a,   0,   3},
 {    40000,   0x7b,   0,   3},
 {    40000,   0x7c,   0,   3},
 {    40000,   0x7d,   0,   3},
 {    40000,   0x7e,   0,   3},
 {    40000,   0x7f,   0,   3},

};


SC1045_AE_lut_t SC1045_AE_lut_50Hz_min_30fps[] = {
 {       10,   0x00,   0,   16},
 {       11,   0x00,   0,   16},
 {       12,   0x00,   0,   16},
 {       13,   0x00,   0,   16},
 {       14,   0x00,   0,   16},
 {       15,   0x00,   0,   16},
 {       16,   0x00,   0,   16},
 {       17,   0x00,   0,   16},
 {       18,   0x00,   0,   16},
 {       19,   0x00,   0,   16},
 {       20,   0x00,   0,   16},
 {       21,   0x00,   0,   16},
 {       22,   0x00,   0,   16},
 {       24,   0x00,   0,   16},
 {       26,   0x00,   0,   16},
 {       28,   0x00,   0,   16},
 {       30,   0x00,   0,   16},
 {       32,   0x00,   0,   16},
 {       34,   0x00,   0,   16},
 {       36,   0x00,   0,   16},
 {       38,   0x00,   0,   16},
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
 {     5004,   0x00,   0,   13},
 {     5404,   0x00,   0,   13},
 {     5850,   0x00,   0,   13},
 {     6300,   0x00,   0,   13},
 {     6850,   0x00,   0,   13},
 {     7400,   0x00,   0,   13},
 {     8050,   0x00,   0,   13},
 {     8650,   0x00,   0,   13},
 {     9300,   0x00,   0,   13},
 {    10000,   0x00,   0,   12},
 {    10000,   0x01,   0,   12},
 {    10000,   0x02,   0,   12},
 {    10000,   0x03,   0,   12},
 {    10000,   0x04,   0,   12},
 {    10000,   0x06,   0,   12},
 {    10000,   0x07,   0,   12},
 {    10000,   0x08,   0,   12},
 {    10000,   0x09,   0,   12},
 {    10000,   0x0a,   0,   12},
 {    10000,   0x0b,   0,   12},
 {    10000,   0x0c,   0,   12},
 {    10000,   0x0d,   0,   12},
 {    10000,   0x0e,   0,   12},
 {    10000,   0x0f,   0,   12},
 {    10000,   0x10,   0,   11},
 {    10000,   0x11,   0,   11},
 {    10000,   0x12,   0,   11},
 {    10000,   0x13,   0,   11},
 {    10000,   0x14,   0,   11},
 {    10000,   0x15,   0,   11},
 {    10000,   0x16,   0,   11},
 {    10000,   0x17,   0,   11},
 {    10000,   0x18,   0,   11},
 {    10000,   0x19,   0,   11},
 {    10000,   0x1a,   0,   10},
 {    10000,   0x1b,   0,   10},
 {    10000,   0x1c,   0,   10},
 {    10000,   0x1d,   0,   10},
 {    10000,   0x1e,   0,   10},
 {    10000,   0x1f,   0,   10},
 {    10000,   0x30,   0,   10},
 {    20000,   0x10,   0,   10},
 {    20000,   0x11,   0,   9},
 {    20000,   0x12,   0,   9},
 {    20000,   0x13,   0,   9},
 {    20000,   0x14,   0,   9},
 {    20000,   0x15,   0,   9},
 {    20000,   0x16,   0,   9},
 {    20000,   0x17,   0,   9},
 {    20000,   0x18,   0,   9},
 {    20000,   0x19,   0,   9},
 {    30000,   0x10,   0,   8},
 {    30000,   0x11,   0,   8},
 {    30000,   0x12,   0,   8},
 {    30000,   0x13,   0,   8},
 {    30000,   0x14,   0,   8},
 {    30000,   0x15,   0,   8},
 {    30000,   0x16,   0,   8},
 {    30000,   0x17,   0,   8},
 {    30000,   0x18,   0,   8},
 {    30000,   0x19,   0,   8},
 {    30000,   0x1A,   0,   8},
 {    30000,   0x1B,   0,   8},
 {    30000,   0x1C,   0,   8},
 {    30000,   0x1D,   0,   8},
 {    30000,   0x1E,   0,   8},
 {    30000,   0x1F,   0,   8},
 {    30000,   0x30,   0,   7},
 {    30000,   0x31,   0,   7},
 {    30000,   0x32,   0,   7},
 {    30000,   0x33,   0,   7},
 {    30000,   0x34,   0,   7},
 {    30000,   0x35,   0,   7},
 {    30000,   0x36,   0,   7},
 {    30000,   0x37,   0,   7},
 {    30000,   0x38,   0,   7},
 {    30000,   0x39,   0,   7},
 {    30000,   0x3a,   0,   7},
 {    30000,   0x3b,   0,   7},
 {    30000,   0x3c,   0,   7},
 {    30000,   0x3d,   0,   7},
 {    30000,   0x3e,   0,   6},
 {    30000,   0x3f,   0,   6},
 {    30000,   0x70,   0,   6},
 {    30000,   0x71,   0,   6},
 {    30000,   0x72,   0,   6},
 {    30000,   0x73,   0,   6},
 {    30000,   0x74,   0,   5},
 {    30000,   0x75,   0,   5},
 {    30000,   0x76,   0,   5},
};
#endif
