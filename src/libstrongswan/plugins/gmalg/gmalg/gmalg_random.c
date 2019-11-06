/******************************************************************************
*        
*     Copyright (c) 2019 Qianxun SI
*        
*        		3H LAB
*		
*******************************************************************************
*  file name:           gmalg_random.c
*  author:              Chen Hao
*  version:             1.00
*  file description:    get random number from "/dev/urandom"
*******************************************************************************
*  revision history:    date               version                  author
*
*  change summary:   2019-1-22             1.00                    Chen Hao
*
******************************************************************************/
/******************************************************************************
* Include Files
******************************************************************************/
#include "gmalg_random.h"
#include "gmalg_util.h"

/******************************************************************************
* Global Functions
******************************************************************************/
int vli_get_random(u8 *data, u32 len)
{
	int fd = open("/dev/urandom", O_RDONLY);
	int ret = -1;

	if (fd > 0) {
		ret = read(fd, data, len);
		close(fd);

	}

	return ret;
}
