/******************************************************************************
*        
*     Copyright (c) 2019 Qianxun SI
*        
*        		3H LAB
*		
*******************************************************************************
*  file name:           gmalg_util.h
*  author:              Chen Hao
*  version:             1.00
*  file description:    common include files and utility functions
*******************************************************************************
*  revision history:    date               version                  author
*
*  change summary:   2019-1-21             1.00                    Chen Hao
*
******************************************************************************/
#ifndef __GMALG_UTIL_H__
#define __GMALG_UTIL_H__

/******************************************************************************
* Include Files
******************************************************************************/
#include "gmalg_type.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/******************************************************************************
* Macros
******************************************************************************/
/*
 * 32-bit integer manipulation macros (big endian)
 */
#define GET_ULONG_BE(n,b,i)                      \
{                                                \
    (n) = ( (uint32) (b)[(i)    ] << 24 )   \
        | ( (uint32) (b)[(i) + 1] << 16 )   \
        | ( (uint32) (b)[(i) + 2] <<  8 )   \
        | ( (uint32) (b)[(i) + 3]       );  \
}

#define PUT_ULONG_BE(n,b,i)                   \
{                                             \
	(b)[(i)    ] = (uint8) ( (n) >> 24 );    \
	(b)[(i) + 1] = (uint8) ( (n) >> 16 );    \
	(b)[(i) + 2] = (uint8) ( (n) >>  8 );    \
	(b)[(i) + 3] = (uint8) ( (n)       );    \
}

/*
 *rotate shift left marco definition
 *
 */
#define ROTL(X,n) (((X)<<(n)) | ((X)>>(32-(n))))
#define SWAP(a,b) { uint32 t = a; a = b; b = t; }

#endif /*__GMALG_UTIL_H__*/

