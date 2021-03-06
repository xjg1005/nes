#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"
#include "H42.h"


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

H42_AE_lut_t H42_AE_lut_50Hz_min_33fps[] = {
 {      60,   0x0,   0,   16},
 {      61,   0x0,   0,   16},
 {      62,   0x0,   0,   16},
 {      63,   0x0,   0,   16},
 {      64,   0x0,   0,   16},
 {      65,   0x0,   0,   16},
 {      66,   0x0,   0,   16},
 {      67,   0x0,   0,   16},
 {      68,   0x0,   0,   16},
 {      69,   0x0,   0,   16},
 {      70,   0x0,   0,   16},
 {      71,   0x0,   0,   16},
 {      72,   0x0,   0,   16},
 {      73,   0x0,   0,   16},
 {      74,   0x0,   0,   16},
 {      75,   0x0,   0,   16},
 {      76,   0x0,   0,   16},
 {      77,   0x0,   0,   16},
 {      78,   0x0,   0,   16},
 {      79,   0x0,   0,   16},
 {      80,   0x0,   0,   16},
 {      81,   0x0,   0,   16},
 {      82,   0x0,   0,   16},
 {      83,   0x0,   0,   16},
 {      84,   0x0,   0,   16},
 {      85,   0x0,   0,   16},
 {      86,   0x0,   0,   15},
 {      87,   0x0,   0,   15},
 {      88,   0x0,   0,   15},
 {      89,   0x0,   0,   15},
 {      90,   0x0,   0,   15},
 {      91,   0x0,   0,   15},
 {      92,   0x0,   0,   15},
 {      93,   0x0,   0,   15},
 {      94,   0x0,   0,   15},
 {      95,   0x0,   0,   15},
 {      96,   0x0,   0,   15},
 {      97,   0x0,   0,   15},
 {      98,   0x0,   0,   15},
 {      99,   0x0,   0,   15},
 {     100,   0x0,   0,   15},
 {     102,   0x0,   0,   15},
 {     104,   0x0,   0,   15},
 {     106,   0x0,   0,   15},
 {     108,   0x0,   0,   15},
 {     110,   0x0,   0,   15},
 {     112,   0x0,   0,   15},
 {     114,   0x0,   0,   15},
 {     116,   0x0,   0,   15},
 {     118,   0x0,   0,   15},
 {     120,   0x0,   0,   15},
 {     122,   0x0,   0,   15},
 {     124,   0x0,   0,   15},
 {     126,   0x0,   0,   15},
 {     128,   0x0,   0,   15},
 {     130,   0x0,   0,   15},
 {     132,   0x0,   0,   15},
 {     134,   0x0,   0,   15},
 {     136,   0x0,   0,   15},
 {     138,   0x0,   0,   15},
 {     140,   0x0,   0,   15},
 {     143,   0x0,   0,   15},
 {     146,   0x0,   0,   15},
 {     149,   0x0,   0,   15},
 {     153,   0x0,   0,   15},
 {     157,   0x0,   0,   15},
 {     161,   0x0,   0,   15},
 {     165,   0x0,   0,   15},
 {     169,   0x0,   0,   15},
 {     173,   0x0,   0,   14},
 {     177,   0x0,   0,   14},
 {     181,   0x0,   0,   14},
 {     185,   0x0,   0,   14},
 {     190,   0x0,   0,   14},
 {     195,   0x0,   0,   14},
 {     200,   0x0,   0,   14},
 {     206,   0x0,   0,   14},
 {     212,   0x0,   0,   14},
 {     218,   0x0,   0,   14},
 {     224,   0x0,   0,   14},
 {     230,   0x0,   0,   14},
 {     236,   0x0,   0,   14},
 {     242,   0x0,   0,   14},
 {     250,   0x0,   0,   14},
 {     258,   0x0,   0,   14},
 {     266,   0x0,   0,   14},
 {     272,   0x0,   0,   14},
 {     278,   0x0,   0,   14},
 {     286,   0x0,   0,   14},
 {     294,   0x0,   0,   14},
 {     304,   0x0,   0,   14},
 {     314,   0x0,   0,   14},
 {     324,   0x0,   0,   14},
 {     334,   0x0,   0,   14},
 {     344,   0x0,   0,   14},
 {     354,   0x0,   0,   14},
 {     368,   0x0,   0,   14},
 {     384,   0x0,   0,   14},
 {     398,   0x0,   0,   14},
 {     412,   0x0,   0,   14},
 {     426,   0x0,   0,   14},
 {     444,   0x0,   0,   14},
 {     464,   0x0,   0,   14},
 {     482,   0x0,   0,   14},
 {     500,   0x0,   0,   14},
 {     520,   0x0,   0,   14},
 {     540,   0x0,   0,   14},
 {     562,   0x0,   0,   14},
 {     584,   0x0,   0,   14},
 {     608,   0x0,   0,   13},
 {     634,   0x0,   0,   13},
 {     662,   0x0,   0,   13},
 {     690,   0x0,   0,   13},
 {     720,   0x0,   0,   13},
 {     758,   0x0,   0,   13},
 {     798,   0x0,   0,   13},
 {     842,   0x0,   0,   13},
 {     890,   0x0,   0,   13},
 {     940,   0x0,   0,   13},
 {     990,   0x0,   0,   13},
 {    1050,   0x0,   0,   13},
 {    1130,   0x0,   0,   13},
 {    1210,   0x0,   0,   12},
 {    1321,   0x0,   0,   12},
 {    1441,   0x0,   0,   12},
 {    1580,   0x0,   0,   12},
 {    1731,   0x0,   0,   12},
 {    1881,   0x0,   0,   12},
 {    2030,   0x0,   0,   12},
 {    2181,   0x0,   0,   12},
 {    2341,   0x0,   0,   12},
 {    2500,   0x00,   0,   12},
 {    2500,   0x01,   0,   12},
 {    2500,   0x02,   0,   12},
 {    2500,   0x03,   0,   11},
 {    2500,   0x04,   0,   11},
 {    2500,   0x05,   0,   11},
 {    2500,   0x06,   0,   11},
 {    2500,   0x07,   0,   11},
 {    2500,   0x08,   0,   11},
 {    2500,   0x09,   0,   11},
 {    2500,   0x0a,   0,   11},
 {    2500,   0x0b,   0,   11},
 {    2500,   0x0c,   0,   11},
 {    2500,   0x0d,   0,   11},
 {    2500,   0x0e,   0,   11},
 {    2500,   0x0f,   0,   11},
 {    2500,   0x10,   0,   11},
 {    2500,   0x11,   0,   11},
 {    2500,   0x12,   0,   10},
 {    2500,   0x13,   0,   10},
 {    2500,   0x14,   0,   10},
 {    2500,   0x15,   0,   10},
 {    2500,   0x16,   0,   10},
 {    2500,   0x17,   0,   10},
 {    2500,   0x18,   0,   10},
 {    2500,   0x19,   0,   10},
 {    2500,   0x1a,   0,   10},
 {    2500,   0x1b,   0,   10},
 {    2500,   0x1c,   0,   10},
 {    2500,   0x1d,   0,   10},
 {    2500,   0x1e,   0,   10},
 {    2500,   0x1f,   0,   10},
 {    2500,   0x20,   0,   10},
 {    5000,   0x10,   0,   9},
 {    5000,   0x11,   0,   9},
 {    5000,   0x12,   0,   9},
 {    5000,   0x13,   0,   9},
 {    5000,   0x14,   0,   9},
 {    5000,   0x15,   0,   9},
 {    5000,   0x16,   0,   9},
 {    5000,   0x17,   0,   9},
 {    5000,   0x18,   0,   9},
 {    7500,   0x10,   0,   8},
 {    7500,   0x11,   0,   8},
 {    7500,   0x12,   0,   8},
 {    7500,   0x13,   0,   8},
 {    7500,   0x14,   0,   8},
 {    7500,   0x15,   0,   8},
 {    7500,   0x16,   0,   8},
 {    7500,   0x17,   0,   8},
 {    7500,   0x18,   0,   8},
 {    7500,   0x19,   0,   8},
 {    7500,   0x1A,   0,   7},
 {    7500,   0x1B,   0,   7},
 {    7500,   0x1C,   0,   7},
 {    7500,   0x1D,   0,   7},
 {    7500,   0x1E,   0,   7},
 {    7500,   0x1F,   0,   7},
 {    7500,   0x20,   0,   7},
 {    7500,   0x21,   0,   7},
 {    7500,   0x22,   0,   7},
 {    7500,   0x23,   0,   7},
 {    7500,   0x24,   0,   7},
 {    7500,   0x25,   0,   6},
 {    7500,   0x26,   0,   6},
 {    7500,   0x27,   0,   6},
 {    7500,   0x28,   0,   6},
 {    7500,   0x29,   0,   6},
 {    7500,   0x2A,   0,   6},
 {    7500,   0x2B,   0,   6},
 {    7500,   0x2C,   0,   6},
 {    7500,   0x2D,   0,   6},
 {    7500,   0x2E,   0,   6},
 {    7500,   0x2F,   0,   6},
 {    7500,   0x30,   0,   6},
 {    7500,   0x30,   0,   6},
 {    7500,   0x30,   0,   6},
 {    7500,   0x30,   0,   6},
};

H42_AE_lut_t H42_AE_lut_min_30fps[] = {
 {      60,   0x0,   0,   16},
 {      61,   0x0,   0,   16},
 {      62,   0x0,   0,   16},
 {      63,   0x0,   0,   16},
 {      64,   0x0,   0,   16},
 {      65,   0x0,   0,   16},
 {      66,   0x0,   0,   16},
 {      67,   0x0,   0,   16},
 {      68,   0x0,   0,   16},
 {      69,   0x0,   0,   16},
 {      70,   0x0,   0,   16},
 {      71,   0x0,   0,   16},
 {      72,   0x0,   0,   16},
 {      73,   0x0,   0,   16},
 {      74,   0x0,   0,   16},
 {      75,   0x0,   0,   16},
 {      76,   0x0,   0,   16},
 {      77,   0x0,   0,   16},
 {      78,   0x0,   0,   16},
 {      79,   0x0,   0,   16},
 {      80,   0x0,   0,   16},
 {      81,   0x0,   0,   16},
 {      82,   0x0,   0,   16},
 {      83,   0x0,   0,   16},
 {      84,   0x0,   0,   16},
 {      85,   0x0,   0,   16},
 {      86,   0x0,   0,   15},
 {      87,   0x0,   0,   15},
 {      88,   0x0,   0,   15},
 {      89,   0x0,   0,   15},
 {      90,   0x0,   0,   15},
 {      91,   0x0,   0,   15},
 {      92,   0x0,   0,   15},
 {      93,   0x0,   0,   15},
 {      94,   0x0,   0,   15},
 {      95,   0x0,   0,   15},
 {      96,   0x0,   0,   15},
 {      97,   0x0,   0,   15},
 {      98,   0x0,   0,   15},
 {      99,   0x0,   0,   15},
 {     100,   0x0,   0,   15},
 {     102,   0x0,   0,   15},
 {     104,   0x0,   0,   15},
 {     106,   0x0,   0,   15},
 {     108,   0x0,   0,   15},
 {     110,   0x0,   0,   15},
 {     112,   0x0,   0,   15},
 {     114,   0x0,   0,   15},
 {     116,   0x0,   0,   15},
 {     118,   0x0,   0,   15},
 {     120,   0x0,   0,   15},
 {     122,   0x0,   0,   15},
 {     124,   0x0,   0,   15},
 {     126,   0x0,   0,   15},
 {     128,   0x0,   0,   15},
 {     130,   0x0,   0,   15},
 {     132,   0x0,   0,   15},
 {     134,   0x0,   0,   15},
 {     136,   0x0,   0,   15},
 {     138,   0x0,   0,   15},
 {     140,   0x0,   0,   15},
 {     143,   0x0,   0,   15},
 {     146,   0x0,   0,   15},
 {     149,   0x0,   0,   15},
 {     153,   0x0,   0,   15},
 {     157,   0x0,   0,   15},
 {     161,   0x0,   0,   15},
 {     165,   0x0,   0,   15},
 {     169,   0x0,   0,   15},
 {     173,   0x0,   0,   14},
 {     177,   0x0,   0,   14},
 {     181,   0x0,   0,   14},
 {     185,   0x0,   0,   14},
 {     190,   0x0,   0,   14},
 {     195,   0x0,   0,   14},
 {     200,   0x0,   0,   14},
 {     206,   0x0,   0,   14},
 {     212,   0x0,   0,   14},
 {     218,   0x0,   0,   14},
 {     224,   0x0,   0,   14},
 {     230,   0x0,   0,   14},
 {     236,   0x0,   0,   14},
 {     242,   0x0,   0,   14},
 {     250,   0x0,   0,   14},
 {     258,   0x0,   0,   14},
 {     266,   0x0,   0,   14},
 {     272,   0x0,   0,   14},
 {     278,   0x0,   0,   14},
 {     286,   0x0,   0,   14},
 {     294,   0x0,   0,   14},
 {     304,   0x0,   0,   14},
 {     314,   0x0,   0,   14},
 {     324,   0x0,   0,   14},
 {     334,   0x0,   0,   14},
 {     344,   0x0,   0,   14},
 {     354,   0x0,   0,   14},
 {     368,   0x0,   0,   14},
 {     384,   0x0,   0,   14},
 {     398,   0x0,   0,   14},
 {     412,   0x0,   0,   14},
 {     426,   0x0,   0,   14},
 {     444,   0x0,   0,   14},
 {     464,   0x0,   0,   14},
 {     482,   0x0,   0,   14},
 {     500,   0x0,   0,   14},
 {     520,   0x0,   0,   14},
 {     540,   0x0,   0,   14},
 {     562,   0x0,   0,   14},
 {     584,   0x0,   0,   14},
 {     608,   0x0,   0,   13},
 {     634,   0x0,   0,   13},
 {     662,   0x0,   0,   13},
 {     690,   0x0,   0,   13},
 {     720,   0x0,   0,   13},
 {     758,   0x0,   0,   13},
 {     798,   0x0,   0,   13},
 {     842,   0x0,   0,   13},
 {     890,   0x0,   0,   13},
 {     940,   0x0,   0,   13},
 {     990,   0x0,   0,   13},
 {    1050,   0x0,   0,   13},
 {    1130,   0x0,   0,   13},
 {    1210,   0x0,   0,   12},
 {    1321,   0x0,   0,   12},
 {    1441,   0x0,   0,   12},
 {    1580,   0x0,   0,   12},
 {    1731,   0x0,   0,   12},
 {    1881,   0x0,   0,   12},
 {    2030,   0x0,   0,   12},
 {    2181,   0x0,   0,   12},
 {    2341,   0x0,   0,   12},
 {    2500,   0x00,   0,   12},
 {    2500,   0x01,   0,   12},
 {    2500,   0x02,   0,   12},
 {    2500,   0x03,   0,   11},
 {    2500,   0x04,   0,   11},
 {    2500,   0x05,   0,   11},
 {    2500,   0x06,   0,   11},
 {    2500,   0x07,   0,   11},
 {    2500,   0x08,   0,   11},
 {    2500,   0x09,   0,   11},
 {    2500,   0x0a,   0,   11},
 {    2500,   0x0b,   0,   11},
 {    2500,   0x0c,   0,   11},
 {    2500,   0x0d,   0,   11},
 {    2500,   0x0e,   0,   11},
 {    2500,   0x0f,   0,   11},
 {    2500,   0x10,   0,   11},
 {    2500,   0x11,   0,   11},
 {    2500,   0x12,   0,   10},
 {    2500,   0x13,   0,   10},
 {    2500,   0x14,   0,   10},
 {    2500,   0x15,   0,   10},
 {    2500,   0x16,   0,   10},
 {    2500,   0x17,   0,   10},
 {    2500,   0x18,   0,   10},
 {    2500,   0x19,   0,   10},
 {    2500,   0x1a,   0,   10},
 {    2500,   0x1b,   0,   10},
 {    2500,   0x1c,   0,   10},
 {    2500,   0x1d,   0,   10},
 {    2500,   0x1e,   0,   10},
 {    2500,   0x1f,   0,   10},
 {    2500,   0x20,   0,   10},
 {    5000,   0x10,   0,   9},
 {    5000,   0x11,   0,   9},
 {    5000,   0x12,   0,   9},
 {    5000,   0x13,   0,   9},
 {    5000,   0x14,   0,   9},
 {    5000,   0x15,   0,   9},
 {    5000,   0x16,   0,   9},
 {    5000,   0x17,   0,   9},
 {    5000,   0x18,   0,   9},
 {    7500,   0x10,   0,   8},
 {    7500,   0x11,   0,   8},
 {    7500,   0x12,   0,   8},
 {    7500,   0x13,   0,   8},
 {    7500,   0x14,   0,   8},
 {    7500,   0x15,   0,   8},
 {    7500,   0x16,   0,   8},
 {    7500,   0x17,   0,   8},
 {    7500,   0x18,   0,   8},
 {    7500,   0x19,   0,   8},
 {    7500,   0x1A,   0,   7},
 {    7500,   0x1B,   0,   7},
 {    7500,   0x1C,   0,   7},
 {    7500,   0x1D,   0,   7},
 {    7500,   0x1E,   0,   7},
 {    7500,   0x1F,   0,   7},
 {    7500,   0x20,   0,   7},
 {    7500,   0x21,   0,   7},
 {    8333,   0x21,   0,   6},
 {    8333,   0x22,   0,   6},
 {    8333,   0x23,   0,   6},
 {    8333,   0x24,   0,   6},
 {    8333,   0x25,   0,   6},
 {    8333,   0x26,   0,   6},
 {    8333,   0x27,   0,   6},
 {    8333,   0x28,   0,   6},
 {    8333,   0x29,   0,   6},
 {    8333,   0x2A,   0,   6},
 {    8333,   0x2B,   0,   6},
 {    8333,   0x2C,   0,   6},
 {    8333,   0x2D,   0,   6},
 {    8333,   0x2E,   0,   6},
 {    8333,   0x2F,   0,   6},
 {    8333,   0x30,   0,   6},
 {    8333,   0x30,   0,   6},
 {    8333,   0x30,   0,   6},
};

H42_AE_lut_t H42_AE_lut_50Hz_min_25fps[] = {
 {      60,   0x0,   0,   16},
 {      61,   0x0,   0,   16},
 {      62,   0x0,   0,   16},
 {      63,   0x0,   0,   16},
 {      64,   0x0,   0,   16},
 {      65,   0x0,   0,   16},
 {      66,   0x0,   0,   16},
 {      67,   0x0,   0,   16},
 {      68,   0x0,   0,   16},
 {      69,   0x0,   0,   16},
 {      70,   0x0,   0,   16},
 {      71,   0x0,   0,   16},
 {      72,   0x0,   0,   16},
 {      73,   0x0,   0,   16},
 {      74,   0x0,   0,   16},
 {      75,   0x0,   0,   16},
 {      76,   0x0,   0,   16},
 {      77,   0x0,   0,   16},
 {      78,   0x0,   0,   16},
 {      79,   0x0,   0,   16},
 {      80,   0x0,   0,   16},
 {      81,   0x0,   0,   16},
 {      82,   0x0,   0,   16},
 {      83,   0x0,   0,   16},
 {      84,   0x0,   0,   16},
 {      85,   0x0,   0,   16},
 {      86,   0x0,   0,   15},
 {      87,   0x0,   0,   15},
 {      88,   0x0,   0,   15},
 {      89,   0x0,   0,   15},
 {      90,   0x0,   0,   15},
 {      91,   0x0,   0,   15},
 {      92,   0x0,   0,   15},
 {      93,   0x0,   0,   15},
 {      94,   0x0,   0,   15},
 {      95,   0x0,   0,   15},
 {      96,   0x0,   0,   15},
 {      97,   0x0,   0,   15},
 {      98,   0x0,   0,   15},
 {      99,   0x0,   0,   15},
 {     100,   0x0,   0,   15},
 {     102,   0x0,   0,   15},
 {     104,   0x0,   0,   15},
 {     106,   0x0,   0,   15},
 {     108,   0x0,   0,   15},
 {     110,   0x0,   0,   15},
 {     112,   0x0,   0,   15},
 {     114,   0x0,   0,   15},
 {     116,   0x0,   0,   15},
 {     118,   0x0,   0,   15},
 {     120,   0x0,   0,   15},
 {     122,   0x0,   0,   15},
 {     124,   0x0,   0,   15},
 {     126,   0x0,   0,   15},
 {     128,   0x0,   0,   15},
 {     130,   0x0,   0,   15},
 {     132,   0x0,   0,   15},
 {     134,   0x0,   0,   15},
 {     136,   0x0,   0,   15},
 {     138,   0x0,   0,   15},
 {     140,   0x0,   0,   15},
 {     143,   0x0,   0,   15},
 {     146,   0x0,   0,   15},
 {     149,   0x0,   0,   15},
 {     153,   0x0,   0,   15},
 {     157,   0x0,   0,   15},
 {     161,   0x0,   0,   15},
 {     165,   0x0,   0,   15},
 {     169,   0x0,   0,   15},
 {     173,   0x0,   0,   14},
 {     177,   0x0,   0,   14},
 {     181,   0x0,   0,   14},
 {     185,   0x0,   0,   14},
 {     190,   0x0,   0,   14},
 {     195,   0x0,   0,   14},
 {     200,   0x0,   0,   14},
 {     206,   0x0,   0,   14},
 {     212,   0x0,   0,   14},
 {     218,   0x0,   0,   14},
 {     224,   0x0,   0,   14},
 {     230,   0x0,   0,   14},
 {     236,   0x0,   0,   14},
 {     242,   0x0,   0,   14},
 {     250,   0x0,   0,   14},
 {     258,   0x0,   0,   14},
 {     266,   0x0,   0,   14},
 {     272,   0x0,   0,   14},
 {     278,   0x0,   0,   14},
 {     286,   0x0,   0,   14},
 {     294,   0x0,   0,   14},
 {     304,   0x0,   0,   14},
 {     314,   0x0,   0,   14},
 {     324,   0x0,   0,   14},
 {     334,   0x0,   0,   14},
 {     344,   0x0,   0,   14},
 {     354,   0x0,   0,   14},
 {     368,   0x0,   0,   14},
 {     384,   0x0,   0,   14},
 {     398,   0x0,   0,   14},
 {     412,   0x0,   0,   14},
 {     426,   0x0,   0,   14},
 {     444,   0x0,   0,   14},
 {     464,   0x0,   0,   14},
 {     482,   0x0,   0,   14},
 {     500,   0x0,   0,   14},
 {     520,   0x0,   0,   14},
 {     540,   0x0,   0,   14},
 {     562,   0x0,   0,   14},
 {     584,   0x0,   0,   14},
 {     608,   0x0,   0,   13},
 {     634,   0x0,   0,   13},
 {     662,   0x0,   0,   13},
 {     690,   0x0,   0,   13},
 {     720,   0x0,   0,   13},
 {     758,   0x0,   0,   13},
 {     798,   0x0,   0,   13},
 {     842,   0x0,   0,   13},
 {     890,   0x0,   0,   13},
 {     940,   0x0,   0,   13},
 {     990,   0x0,   0,   13},
 {    1050,   0x0,   0,   13},
 {    1130,   0x0,   0,   13},
 {    1210,   0x0,   0,   12},
 {    1321,   0x0,   0,   12},
 {    1441,   0x0,   0,   12},
 {    1580,   0x0,   0,   12},
 {    1731,   0x0,   0,   12},
 {    1881,   0x0,   0,   12},
 {    2030,   0x0,   0,   12},
 {    2181,   0x0,   0,   12},
 {    2341,   0x0,   0,   12},
 {    2500,   0x00,   0,   12},
 {    2500,   0x01,   0,   12},
 {    2500,   0x02,   0,   12},
 {    2500,   0x03,   0,   11},
 {    2500,   0x04,   0,   11},
 {    2500,   0x06,   0,   11},
 {    2500,   0x07,   0,   11},
 {    2500,   0x08,   0,   11},
 {    2500,   0x0a,   0,   11},
 {    2500,   0x0b,   0,   11},
 {    2500,   0x0c,   0,   11},
 {    2500,   0x0d,   0,   11},
 {    2500,   0x0e,   0,   11},
 {    2500,   0x0f,   0,   11},
 {    2500,   0x10,   0,   11},
 {    2500,   0x11,   0,   11},
 {    2500,   0x12,   0,   10},
 {    2500,   0x13,   0,   10},
 {    2500,   0x14,   0,   10},
 {    2500,   0x15,   0,   10},
 {    2500,   0x16,   0,   10},
 {    2500,   0x17,   0,   10},
 {    2500,   0x18,   0,   10},
 {    2500,   0x19,   0,   10},
 {    2500,   0x1a,   0,   10},
 {    2500,   0x1b,   0,   10},
 {    2500,   0x1c,   0,   10},
 {    2500,   0x1d,   0,   10},
 {    2500,   0x1e,   0,   10},
 {    2500,   0x1f,   0,   10},
 {    2500,   0x20,   0,   10},
 {    5000,   0x10,   0,   9},
 {    5000,   0x11,   0,   9},
 {    5000,   0x12,   0,   9},
 {    5000,   0x13,   0,   9},
 {    5000,   0x14,   0,   9},
 {    5000,   0x15,   0,   9},
 {    5000,   0x16,   0,   9},
 {    5000,   0x17,   0,   9},
 {    5000,   0x18,   0,   9},
 {    7500,   0x10,   0,   8},
 {    7500,   0x11,   0,   8},
 {    7500,   0x12,   0,   8},
 {    7500,   0x13,   0,   8},
 {    7500,   0x14,   0,   8},
 {    7500,   0x15,   0,   8},
 {    7500,   0x16,   0,   8},
 {    7500,   0x17,   0,   8},
 {    7500,   0x18,   0,   8},
 {    7500,   0x19,   0,   8},
 {    7500,   0x1A,   0,   7},
 {    7500,   0x1B,   0,   7},
 {    7500,   0x1C,   0,   7},
 {    7500,   0x1D,   0,   7},
 {    7500,   0x1E,   0,   7},
 {    7500,   0x1F,   0,   7},
 {    7500,   0x20,   0,   7},
 {   10000,   0x16,   0,   7},
 {   10000,   0x17,   0,   7},
 {   10000,   0x18,   0,   7},
 {   10000,   0x19,   0,   7},

  {   10000,   0x1a,   0,   7},
   {   10000,   0x1b,   0,   7},
    {   10000,   0x1c,   0,   7},
     {   10000,   0x1d,   0,   7},
      {   10000,   0x1e,   0,   7},
       {   10000,   0x1f,   0,   7},

 {   10000,   0x20,   0,   7},
 {   10000,   0x21,   0,   6},
 {   10000,   0x22,   0,   6},
 {   10000,   0x23,   0,   6},
 {   10000,   0x24,   0,   6},
 {   10000,   0x25,   0,   6},
 {   10000,   0x26,   0,   6},
 {   10000,   0x27,   0,   6},
 {   10000,   0x28,   0,   6},
 {   10000,   0x29,   0,   6},
 {   10000,   0x2A,   0,   6},
 {   10000,   0x2B,   0,   6},
 {   10000,   0x2C,   0,   6},
 {   10000,   0x2D,   0,   6},
 {   10000,   0x2E,   0,   6},
 {   10000,   0x2F,   0,   6},
 {   10000,   0x30,   0,   5},
 {   10000,   0x31,   0,   5},
 {   10000,   0x32,   0,   5},
 {   10000,   0x33,   0,   5},
 {   10000,   0x34,   0,   5},
 {   10000,   0x35,   0,   5},
 {   10000,   0x36,   0,   5},
 {   10000,   0x37,   0,   5},
 {   10000,   0x38,   0,   5},
};


H42_AE_lut_t H42_AE_lut_50Hz_min_25fps_bk[] = {
 {      60,   0x0,   0,   16},
 {      61,   0x0,   0,   16},
 {      62,   0x0,   0,   16},
 {      63,   0x0,   0,   16},
 {      64,   0x0,   0,   16},
 {      65,   0x0,   0,   16},
 {      66,   0x0,   0,   16},
 {      67,   0x0,   0,   16},
 {      68,   0x0,   0,   16},
 {      69,   0x0,   0,   16},
 {      70,   0x0,   0,   16},
 {      71,   0x0,   0,   16},
 {      72,   0x0,   0,   16},
 {      73,   0x0,   0,   16},
 {      74,   0x0,   0,   16},
 {      75,   0x0,   0,   16},
 {      76,   0x0,   0,   16},
 {      77,   0x0,   0,   16},
 {      78,   0x0,   0,   16},
 {      79,   0x0,   0,   16},
 {      80,   0x0,   0,   16},
 {      81,   0x0,   0,   16},
 {      82,   0x0,   0,   16},
 {      83,   0x0,   0,   16},
 {      84,   0x0,   0,   16},
 {      85,   0x0,   0,   16},
 {      86,   0x0,   0,   15},
 {      87,   0x0,   0,   15},
 {      88,   0x0,   0,   15},
 {      89,   0x0,   0,   15},
 {      90,   0x0,   0,   15},
 {      91,   0x0,   0,   15},
 {      92,   0x0,   0,   15},
 {      93,   0x0,   0,   15},
 {      94,   0x0,   0,   15},
 {      95,   0x0,   0,   15},
 {      96,   0x0,   0,   15},
 {      97,   0x0,   0,   15},
 {      98,   0x0,   0,   15},
 {      99,   0x0,   0,   15},
 {     100,   0x0,   0,   15},
 {     102,   0x0,   0,   15},
 {     104,   0x0,   0,   15},
 {     106,   0x0,   0,   15},
 {     108,   0x0,   0,   15},
 {     110,   0x0,   0,   15},
 {     112,   0x0,   0,   15},
 {     114,   0x0,   0,   15},
 {     116,   0x0,   0,   15},
 {     118,   0x0,   0,   15},
 {     120,   0x0,   0,   15},
 {     122,   0x0,   0,   15},
 {     124,   0x0,   0,   15},
 {     126,   0x0,   0,   15},
 {     128,   0x0,   0,   15},
 {     130,   0x0,   0,   15},
 {     132,   0x0,   0,   15},
 {     134,   0x0,   0,   15},
 {     136,   0x0,   0,   15},
 {     138,   0x0,   0,   15},
 {     140,   0x0,   0,   15},
 {     143,   0x0,   0,   15},
 {     146,   0x0,   0,   15},
 {     149,   0x0,   0,   15},
 {     153,   0x0,   0,   15},
 {     157,   0x0,   0,   15},
 {     161,   0x0,   0,   15},
 {     165,   0x0,   0,   15},
 {     169,   0x0,   0,   15},
 {     173,   0x0,   0,   14},
 {     177,   0x0,   0,   14},
 {     181,   0x0,   0,   14},
 {     185,   0x0,   0,   14},
 {     190,   0x0,   0,   14},
 {     195,   0x0,   0,   14},
 {     200,   0x0,   0,   14},
 {     206,   0x0,   0,   14},
 {     212,   0x0,   0,   14},
 {     218,   0x0,   0,   14},
 {     224,   0x0,   0,   14},
 {     230,   0x0,   0,   14},
 {     236,   0x0,   0,   14},
 {     242,   0x0,   0,   14},
 {     250,   0x0,   0,   14},
 {     258,   0x0,   0,   14},
 {     266,   0x0,   0,   14},
 {     272,   0x0,   0,   14},
 {     278,   0x0,   0,   14},
 {     286,   0x0,   0,   14},
 {     294,   0x0,   0,   14},
 {     304,   0x0,   0,   14},
 {     314,   0x0,   0,   14},
 {     324,   0x0,   0,   14},
 {     334,   0x0,   0,   14},
 {     344,   0x0,   0,   14},
 {     354,   0x0,   0,   14},
 {     368,   0x0,   0,   14},
 {     384,   0x0,   0,   14},
 {     398,   0x0,   0,   14},
 {     412,   0x0,   0,   14},
 {     426,   0x0,   0,   14},
 {     444,   0x0,   0,   14},
 {     464,   0x0,   0,   14},
 {     482,   0x0,   0,   14},
 {     500,   0x0,   0,   14},
 {     520,   0x0,   0,   14},
 {     540,   0x0,   0,   14},
 {     562,   0x0,   0,   14},
 {     584,   0x0,   0,   14},
 {     608,   0x0,   0,   13},
 {     634,   0x0,   0,   13},
 {     662,   0x0,   0,   13},
 {     690,   0x0,   0,   13},
 {     720,   0x0,   0,   13},
 {     758,   0x0,   0,   13},
 {     798,   0x0,   0,   13},
 {     842,   0x0,   0,   13},
 {     890,   0x0,   0,   13},
 {     940,   0x0,   0,   13},
 {     990,   0x0,   0,   13},
 {    1050,   0x0,   0,   13},
 {    1130,   0x0,   0,   13},
 {    1210,   0x0,   0,   12},
 {    1321,   0x0,   0,   12},
 {    1441,   0x0,   0,   12},
 {    1580,   0x0,   0,   12},
 {    1731,   0x0,   0,   12},
 {    1881,   0x0,   0,   12},
 {    2030,   0x0,   0,   12},
 {    2181,   0x0,   0,   12},
 {    2341,   0x0,   0,   12},
 {    2500,   0x00,   0,   11},
 {    2700,   0x00,   0,   11},
 {    2900,   0x00,   0,   11},
 {    3100,   0x00,   0,   11},
 {    3400,   0x00,   0,   11},
 {    3700,   0x00,   0,   10},
 {    4000,   0x00,   0,   10},
 {    4300,   0x00,   0,   10},
 {    4700,   0x00,   0,   10},
 {    5100,   0x00,   0,   10},
 {    5500,   0x00,   0,   10},
 {    6000,   0x00,   0,   10},
 {    6500,   0x00,   0,   9},
 {    7000,   0x00,   0,   9},
 {    7600,   0x00,   0,   9},
 {    8200,   0x00,   0,   9},
 {    8800,   0x00,   0,   9},
 {    9400,   0x00,   0,   9},
 {   10000,   0x00,   0,   9},
 {   10000,   0x01,   0,   9},
 {   10000,   0x02,   0,   9},
 {   10000,   0x03,   0,   9},
 {   10000,   0x04,   0,   8},
 {   10000,   0x05,   0,   8},
 {   10000,   0x06,   0,   8},
 {   10000,   0x08,   0,   8},
 {   10000,   0x09,   0,   8},
 {   10000,   0x0a,   0,   8},
 {   10000,   0x0b,   0,   8},
 {   10000,   0x0c,   0,   7},
 {   10000,   0x0d,   0,   7},
 {   10000,   0x0e,   0,   7},
 {   10000,   0x0f,   0,   7},
 {   10000,   0x10,   0,   7},
 {   10000,   0x11,   0,   7},
 {   10000,   0x12,   0,   7},
 {   10000,   0x13,   0,   7},
 {   10000,   0x14,   0,   7},
 {   10000,   0x15,   0,   7},
 {   10000,   0x16,   0,   7},
 {   10000,   0x17,   0,   7},
 {   10000,   0x18,   0,   7},
 {   10000,   0x19,   0,   7},
 {   10000,   0x20,   0,   7},
 {   10000,   0x21,   0,   6},
 {   10000,   0x22,   0,   6},
 {   10000,   0x23,   0,   6},
 {   10000,   0x24,   0,   6},
 {   10000,   0x25,   0,   6},
 {   10000,   0x26,   0,   6},
 {   10000,   0x27,   0,   6},
 {   10000,   0x28,   0,   6},
 {   10000,   0x29,   0,   6},
 {   10000,   0x2A,   0,   6},
 {   10000,   0x2B,   0,   6},
 {   10000,   0x2C,   0,   6},
 {   10000,   0x2D,   0,   6},
 {   10000,   0x2E,   0,   6},
 {   10000,   0x2F,   0,   6},
 {   10000,   0x30,   0,   5},
 {   10000,   0x31,   0,   5},
 {   10000,   0x32,   0,   5},
 {   10000,   0x33,   0,   5},
 {   10000,   0x34,   0,   5},
 {   10000,   0x35,   0,   5},
 {   10000,   0x36,   0,   5},
 {   10000,   0x37,   0,   5},
 {   10000,   0x38,   0,   5},
};

