/******************************************************************************
*        
*     Copyright (c) 2019 Qianxun SI
*        
*        		3H LAB
*		
*******************************************************************************
*  file name:           utest_sm4.c
*  author:              Chen Hao
*  version:             1.00
*  file description:    unit test for sm4
*******************************************************************************
*  revision history:    date               version                  author
*
*  change summary:   2019-1-22             1.00                    Chen Hao
*
******************************************************************************/
/******************************************************************************
* Include Files
******************************************************************************/
#include "gmalg_util.h"
#include "gmalg_debug.h"
#include "sm4.h"

int main(int argc, char **agv)
{
    //unit test for sm4 ecb
	sm4_ctx ctx_ecb;
    uint8 key_ecb[16] = { 0x01, 0x23, 0x45, 0x67, 
                        0x89, 0xab, 0xcd, 0xef, 
                        0xfe, 0xdc, 0xba, 0x98, 
                        0x76, 0x54, 0x32, 0x10};
    uint8 plain_ecb[] = { 0x01, 0x23, 0x45, 0x67, 
                        0x89, 0xab, 0xcd, 0xef, 
                        0xfe, 0xdc, 0xba, 0x98, 
                        0x76, 0x54, 0x32, 0x10};
    uint8 crypter_ecb[16] = {0};
    uint8 *pInEcb = plain_ecb;
    uint8 *pOutEcb = crypter_ecb;

	GMALG_DBG_DUMP("PALIN_ECB", plain_ecb, sizeof(plain_ecb));
	GMALG_DBG_DUMP("KEY_ECB", key_ecb, sizeof(key_ecb));
    for (uint32 idx = 0; idx < 1000000; ++idx)
    {
        sm4_ecb_encrypt(&ctx_ecb, key_ecb, pInEcb, 16, pOutEcb);
        pInEcb = pOutEcb;
    }
    //excepted 59 52 98 c7 c6 fd 27 1f 04 02 f8 04 c3 3d 3f 66
	GMALG_DBG_DUMP("CRYPTER_ECB", crypter_ecb, sizeof(crypter_ecb));
    for (uint32 idx = 0; idx < 1000000; ++idx)
    {
        pInEcb = pOutEcb;
        sm4_ecb_decrypt(&ctx_ecb, key_ecb, pInEcb, 16, pOutEcb);
    }
	GMALG_DBG_DUMP("PALIN_ECB", plain_ecb, sizeof(plain_ecb));

    //unit test for sm4_cbc
    sm4_ctx ctx_cbc;
    uint8 key_cbc[16] = {0x60, 0x9A, 0xEB, 0x29, 
                         0x95, 0x14, 0xA1, 0x79, 
                         0x5E, 0x90, 0xA5, 0x1E, 
                         0xEF, 0x55, 0xC7, 0x04};
    uint8 iv_cbc[16] = {0x2C, 0x95, 0x90, 0x99, 
                        0xFF, 0x80, 0xBC, 0x47, 
                        0xE2, 0x03, 0x5A, 0xD1, 
                        0x9A, 0x56, 0x07, 0xDD};
    uint8 crypter_cbc[48] = {0x2B, 0x9F, 0x46, 0xB6, 
                         0x61, 0x24, 0xDA, 0xD5, 
                         0x31, 0xC9, 0xD7, 0x9A, 
                         0xAB, 0x20, 0xD1, 0xE8, 
                         0x27, 0xDD, 0x51, 0x2E, 
                         0xD5, 0x33, 0xEE, 0xEF, 
                         0x55, 0xEF, 0x07, 0x3F, 
                         0x38, 0xBF, 0xD3, 0x74, 
                         0x0D, 0x2D, 0x13, 0xCA, 
                         0x55, 0x12, 0x51, 0x9B, 
                         0xFB, 0xFD, 0xAF, 0x35, 
                         0xC8, 0x94, 0x8A, 0xF5};
    uint8 plain_cbc[48] = {0};

	GMALG_DBG_DUMP("CRYPTER_CBC", crypter_cbc, sizeof(crypter_cbc));
	GMALG_DBG_DUMP("KEY_CBC", key_cbc, sizeof(key_cbc));
	GMALG_DBG_DUMP("IV_CBC", iv_cbc, sizeof(iv_cbc));
    sm4_cbc_decrypt(&ctx_ecb, key_cbc, iv_cbc, crypter_cbc, 48, plain_cbc);
	GMALG_DBG_DUMP("PALIN_CBC", plain_cbc, sizeof(plain_cbc));
    sm4_cbc_encrypt(&ctx_ecb, key_cbc, iv_cbc, plain_cbc, 48, crypter_cbc);
    //excepted 59 52 98 c7 c6 fd 27 1f 04 02 f8 04 c3 3d 3f 66
	GMALG_DBG_DUMP("CRYPTER_CBC", crypter_cbc, sizeof(crypter_cbc));

	return 0;
}
