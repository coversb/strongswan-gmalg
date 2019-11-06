/******************************************************************************
*        
*     Copyright (c) 2019 Qianxun SI
*        
*        		3H LAB
*		
*******************************************************************************
*  file name:           sm3.h
*  author:              Chen Hao
*  version:             1.00
*  file description:    sm3 algorithm header file
*******************************************************************************
*  revision history:    date               version                  author
*
*  change summary:   2019-1-21             1.00                    Chen Hao
*
******************************************************************************/
#ifndef __SM3_H__
#define __SM3_H__

/******************************************************************************
* Include Files
******************************************************************************/
#include "gmalg_type.h"

/******************************************************************************
* Macros
******************************************************************************/
#define SM3_DIGEST_LENGTH (32)
#define SM3_BLOCK_SIZE (64)

/******************************************************************************
* Types
******************************************************************************/
typedef struct 
{
	uint32 total[2];    /*!< number of bytes processed  */
	uint32 state[8];    /*!< intermediate digest state  */
	uint8 buffer[64];   /*!< data block being processed */
} sm3_ctx;

/******************************************************************************
* Extern functions
******************************************************************************/
extern GMALG_STATE sm3_init(sm3_ctx *ctx);
extern GMALG_STATE sm3_update(sm3_ctx *ctx, const uint8 *in, uint32 inLen);
extern GMALG_STATE sm3_final(sm3_ctx *ctx, uint8 *out);
extern void sm3(const uint8 *msg, uint32 msglen, uint8 dgst[SM3_DIGEST_LENGTH]);

#endif /* __SM3_H__ */

