/******************************************************************************
*        
*     Copyright (c) 2019 Qianxun SI
*        
*        		3H LAB
*		
*******************************************************************************
*  file name:           utest_sm3.c
*  author:              Chen Hao
*  version:             1.00
*  file description:    unit test for sm3
*******************************************************************************
*  revision history:    date               version                  author
*
*  change summary:   2019-1-21             1.00                    Chen Hao
*
******************************************************************************/
/******************************************************************************
* Include Files
******************************************************************************/
#include "gmalg_util.h"
#include "gmalg_debug.h"
#include "sm3.h"

int main(int argc, char **agv)
{
    uint8 in[] = {0x33, 0x66, 0x77, 0x99, 0x00};
    uint8 out[SM3_DIGEST_LENGTH] = {0};
    
    sm3(in, sizeof(in), out);

	GMALG_DBG_DUMP("HASH", out, SM3_DIGEST_LENGTH);

	return 0;
}
