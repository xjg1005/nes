#ifndef _HTTPIN_H_
#define _HTTPIN_H_
#include "typedef.h"
typedef enum
{
	HERROR_REDIRECT		        = 1,
	HERROR_OK			        = 0,
	HERROR_MEM			        = -1,
	HERROR_HEADER		        = -2,
	HERROR_RESPOND		        = -3,
	HERROR_SOCK			        = -4,
	HERROR_CALLBACK		        = -5,
	HERROR_UNKNOWN		        = -6,
	HERROR_PARAM		        = -7,
	HERROR_REDIRECT_DEEP		= -8,
}httpin_error;

typedef enum
{
	HTTPIN_HEADER,
	HTTPIN_START,
	HTTPIN_PROGRESS,
	HTTPIN_FINISHED,
	HTTPIN_ABORT,
	HTTPIN_ERROR,
}httpin_status;

/****************************************************/

typedef signed long (*httpin_cb)(void* buf, unsigned long size, void* ctx, unsigned long status);

/****************************************************/

httpin_error httpin_get(const char* url,
                        signed long lowRange,
                        signed long highRange,
						httpin_cb cb,
						void* ctx,
						signed long timeout_secs);
#endif
