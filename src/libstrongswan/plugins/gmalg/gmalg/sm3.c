/******************************************************************************
*        
*     Copyright (c) 2019 Qianxun SI
*        
*        		3H LAB
*		
*******************************************************************************
*  file name:           sm3.c
*  author:              Chen Hao
*  version:             1.00
*  file description:    sm3 algorithm
*******************************************************************************
*  revision history:    date               version                  author
*
*  change summary:   2019-1-21             1.00                    Chen Hao
*
******************************************************************************/
/******************************************************************************
* Include Files
******************************************************************************/
#include "sm3.h"
#include "gmalg_util.h"

/******************************************************************************
* Macros
******************************************************************************/
#define FF0(x, y, z) ((x) ^ (y) ^ (z))
#define FF1(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define GG0(x, y, z) ((x) ^ (y) ^ (z))
#define GG1(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
#define P0(x) ((x) ^ ROTL((x), 9) ^ ROTL((x), 17))
#define P1(x) ((x) ^ ROTL((x), 15) ^ ROTL((x), 23))

/******************************************************************************
* Variables (Extern, Global and Static)
******************************************************************************/
static uint8 sm3_padding[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/******************************************************************************
* Local Functions
******************************************************************************/
/******************************************************************************
* Function    : sm3_process
* 
* Author      : Chen Hao
* 
* Parameters  : 
* 
* Return      : 
* 
* Description : 
******************************************************************************/
static void sm3_process(sm3_ctx *ctx, uint8 data[64])
{
    uint32 SS1 = 0, SS2 = 0;
    uint32 TT1 = 0, TT2 = 0;
    uint32 W[68] = {0}, W1[64] = {0};
    uint32 A = 0, B = 0, C = 0, D = 0, E = 0, F = 0, G = 0, H = 0;
    uint32 T[64] = {0};
    uint8 idx = 0;

    for (idx = 0; idx < 16; ++idx)
    {
        T[idx] = 0x79CC4519;
    }
    for (idx = 16; idx < 64; ++idx)
    {
        T[idx] = 0x7A879D8A;
    }

    for (idx = 0; idx < 16; ++idx)
    {
        GET_ULONG_BE(W[idx], data, idx*4);
    }

    for (idx = 16; idx < 68; ++idx)
    {
        W[idx] = P1(W[idx-16] ^ W[idx-9] ^ ROTL(W[idx-3], 15)) ^ ROTL(W[idx-13], 7) ^ W[idx-6];;
    }

    for (idx = 0; idx < 64; ++idx)
    {
        W1[idx] = W[idx] ^ W[idx+4];
    }

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];
    E = ctx->state[4];
    F = ctx->state[5];
    G = ctx->state[6];
    H = ctx->state[7];

    for (idx = 0; idx < 16; ++idx)
    {
        SS1 = ROTL((ROTL(A, 12) + E + ROTL(T[idx], idx)), 7);
        SS2 = SS1 ^ ROTL(A, 12);
        TT1 = FF0(A, B, C) + D + SS2 + W1[idx];
        TT2 = GG0(E, F, G) + H + SS1 + W[idx];
        D = C;
        C = ROTL(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTL(F, 19);
        F = E;
        E = P0(TT2);
    }

    for (idx = 16; idx < 64; ++idx)
    {
        SS1 = ROTL((ROTL(A, 12) + E + ROTL(T[idx], idx)), 7);
        SS2 = SS1 ^ ROTL(A, 12);
        TT1 = FF1(A, B, C) + D + SS2 + W1[idx];
        TT2 = GG1(E, F, G) + H + SS1 + W[idx];
        D = C;
        C = ROTL(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTL(F, 19);
        F = E;
        E = P0(TT2);
    }

    ctx->state[0] ^= A;
    ctx->state[1] ^= B;
    ctx->state[2] ^= C;
    ctx->state[3] ^= D;
    ctx->state[4] ^= E;
    ctx->state[5] ^= F;
    ctx->state[6] ^= G;
    ctx->state[7] ^= H;
}

/******************************************************************************
* Function    : sm3_init
* 
* Author      : Chen Hao
* 
* Parameters  : 
* 
* Return      : 
* 
* Description : init sm3 context
******************************************************************************/
GMALG_STATE sm3_init(sm3_ctx *ctx)
{
    memset(ctx, 0, sizeof(sm3_ctx));

    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x7380166F;
    ctx->state[1] = 0x4914B2B9;
    ctx->state[2] = 0x172442D7;
    ctx->state[3] = 0xDA8A0600;
    ctx->state[4] = 0xA96F30BC;
    ctx->state[5] = 0x163138AA;
    ctx->state[6] = 0xE38DEE4D;
    ctx->state[7] = 0xB0FB0E4E;

    return GMALG_OK;
}

/******************************************************************************
* Function    : sm3_update
* 
* Author      : Chen Hao
* 
* Parameters  : 
* 
* Return      : 
* 
* Description : 
******************************************************************************/
GMALG_STATE sm3_update(sm3_ctx *ctx, const uint8 *in, uint32 inLen)
{
    uint32 left = 0;
    uint32 fill = 0;

    if (inLen <= 0)
    {
        return GMALG_ERR;
    }

    left = ctx->total[0] & 0x3F;
    fill = SM3_BLOCK_SIZE - left;

    ctx->total[0] += inLen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < inLen)
    {
        ctx->total[1]++;
    }

    if (left && inLen >= fill)
    {
        memcpy((ctx->buffer + left), (void*)in, fill);
        sm3_process(ctx, ctx->buffer);
        in += fill;
        inLen -= fill;
        left = 0;
    }

    while (inLen >= SM3_BLOCK_SIZE)
    {
        sm3_process(ctx, (uint8*)in);
        in += SM3_BLOCK_SIZE;
        inLen -= SM3_BLOCK_SIZE;
    }

    if (inLen > 0)
    {
        memcpy((ctx->buffer + left), (void*)in, inLen);
    }

    return GMALG_OK;
}

/******************************************************************************
* Function    : sm3_final
* 
* Author      : Chen Hao
* 
* Parameters  : 
* 
* Return      : 
* 
* Description : 
******************************************************************************/
GMALG_STATE sm3_final(sm3_ctx *ctx, uint8 *out)
{
    uint32 last = 0, padn = 0;
    uint32 high = 0, low = 0;
    uint8 msglen[8] = {0};

    high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
    low  = (ctx->total[0] << 3);

    PUT_ULONG_BE(high, msglen, 0);
    PUT_ULONG_BE(low, msglen, 4);

    last = ctx->total[0] & 0x3F;
    padn = (last < 56) ? (56 - last) : (120 - last);

    sm3_update(ctx, (const uint8*)sm3_padding, padn);
    sm3_update(ctx, msglen, 8);

    for (uint8 idx = 0; idx < 8; ++idx)
    {
        PUT_ULONG_BE( ctx->state[idx], out, idx*4);
    }

    return GMALG_OK;
}

/******************************************************************************
* Function    : sm3
* 
* Author      : Chen Hao
* 
* Parameters  : 
* 
* Return      : 
* 
* Description : 
******************************************************************************/
void sm3(const uint8 *msg, uint32 msglen, uint8 dgst[SM3_DIGEST_LENGTH])
{
    if (msg == NULL)
    {
        return;
    }
    
    sm3_ctx ctx;

    sm3_init(&ctx);
    sm3_update(&ctx, msg, msglen);
    sm3_final(&ctx, dgst);

    memset(&ctx, 0, sizeof(sm3_ctx));
}

