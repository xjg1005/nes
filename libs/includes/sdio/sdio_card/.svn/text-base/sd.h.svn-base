/*
 *  include/linux/mmc/sd.h
 *
 *  Copyright (C) 2005-2007 Pierre Ossman, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */

#ifndef _SD_H_
#define _SD_H_
#define MMC_CMD_RETRIES 3
/* OCR bit definitions */
#define SD_OCR_S18R		(1 << 24)    /* 1.8V switching request */
#define SD_ROCR_S18A		SD_OCR_S18R  /* 1.8V switching accepted by card */
#define SD_OCR_XPC		(1 << 28)    /* SDXC power control */
#define SD_OCR_CCS		(1 << 30)    /* Card Capacity Status */

/*
 * SD_SWITCH argument format:
 *
 *      [31] Check (0) or switch (1)
 *      [30:24] Reserved (0)
 *      [23:20] Function group 6
 *      [19:16] Function group 5
 *      [15:12] Function group 4
 *      [11:8] Function group 3
 *      [7:4] Function group 2
 *      [3:0] Function group 1
 */

/*
 * SD_SEND_IF_COND argument format:
 *
 *	[31:12] Reserved (0)
 *	[11:8] Host Voltage Supply Flags
 *	[7:0] Check Pattern (0xAA)
 */

/*
 * SCR field definitions
 */

#define SCR_SPEC_VER_0		0	/* Implements system specification 1.0 - 1.01 */
#define SCR_SPEC_VER_1		1	/* Implements system specification 1.10 */
#define SCR_SPEC_VER_2		2	/* Implements system specification 2.00-3.0X */

/*
 * SD bus widths
 */
#define SD_BUS_WIDTH_1		0
#define SD_BUS_WIDTH_4		2

/*
 * SD_SWITCH mode
 */
#define SD_SWITCH_CHECK		0
#define SD_SWITCH_SET		1

/*
 * SD_SWITCH function groups
 */
#define SD_SWITCH_GRP_ACCESS	0

/*
 * SD_SWITCH access modes
 */
#define SD_SWITCH_ACCESS_DEF	0
#define SD_SWITCH_ACCESS_HS	1

#endif /* _SD_H_ */
