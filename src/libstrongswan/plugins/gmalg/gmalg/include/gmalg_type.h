/******************************************************************************
*        
*     Copyright (c) 2019 Qianxun SI
*        
*        		3H LAB
*		
*******************************************************************************
*  file name:           gmalg_type.h
*  author:              Chen Hao
*  version:             1.00
*  file description:    define all the base type
*******************************************************************************
*  revision history:    date               version                  author
*
*  change summary:   2019-1-21             1.00                    Chen Hao
*
******************************************************************************/
#ifndef __GMALG_TYPE_H__
#define __GMALG_TYPE_H__

/******************************************************************************
* Include Files
******************************************************************************/


/******************************************************************************
* Types
******************************************************************************/
typedef	unsigned char uint8;
typedef	unsigned short uint16;
typedef	unsigned int uint32;
typedef	unsigned long long uint64;

typedef signed char int8;
typedef	signed short int16;
typedef signed int int32;
typedef signed long long int64;

typedef enum
{
    GMALG_ERR = 0,
    GMALG_OK = 1
}GMALG_STATE;

/*make original source code is ok*/
typedef	unsigned char u8;
typedef	unsigned short u16;
typedef	unsigned int u32;
typedef	unsigned long long u64;
typedef char s8;
typedef	short s16;
typedef int s32;
typedef long long s64;


#endif /*__GMALG_TYPE_H__*/

