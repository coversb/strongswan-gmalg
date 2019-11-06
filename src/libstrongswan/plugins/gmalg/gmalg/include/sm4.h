/******************************************************************************
*        
*     Copyright (c) 2019 Qianxun SI
*        
*        		3H LAB
*		
*******************************************************************************
*  file name:           sm4.h
*  author:              Chen Hao
*  version:             1.00
*  file description:    sm4 algorithm header file
*******************************************************************************
*  revision history:    date               version                  author
*
*  change summary:   2019-1-22             1.00                    Chen Hao
*
******************************************************************************/
#ifndef __SM4_H__
#define __SM4_H__
/******************************************************************************
* Include Files
******************************************************************************/
#include "gmalg_type.h"

/******************************************************************************
* Macros
******************************************************************************/
#define SM4_KEY_LENGTH (16)
#define SM4_BLOCK_SIZE (16)
#define SM4_IV_LENGTH (SM4_BLOCK_SIZE)
#define SM4_NUM_ROUNDS (32)

/******************************************************************************
* Types
******************************************************************************/
typedef struct 
{
	uint32 sk_enc[SM4_NUM_ROUNDS];
	uint32 sk_dec[SM4_NUM_ROUNDS];
} sm4_ctx;

/******************************************************************************
* Extern functions
******************************************************************************/
extern void sm4_ecb_encrypt(sm4_ctx *ctx, uint8 *key, uint8 *in, uint8 len, uint8 *out);
extern void sm4_ecb_decrypt(sm4_ctx *ctx, uint8 *key, uint8 *in, uint8 len, uint8 *out);
extern void sm4_cbc_encrypt(sm4_ctx *ctx, uint8 *key, uint8 *iv, uint8 *in, uint8 len, uint8 *out);
extern void sm4_cbc_decrypt(sm4_ctx *ctx, uint8 *key, uint8 *iv, uint8 *in, uint8 len, uint8 *out);

#endif /* __SM4_H__ */

