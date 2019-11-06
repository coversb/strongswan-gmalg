/******************************************************************************
*        
*     Copyright (c) 2019 Qianxun SI
*        
*        		3H LAB
*		
*******************************************************************************
*  file name:           gmalg.c
*  author:              Chen Hao
*  version:             1.00
*  file description:    manage GM algorithm
*******************************************************************************
*  revision history:    date               version                  author
*
*  change summary:   2019-1-22             1.00                    Chen Hao
*
******************************************************************************/
/******************************************************************************
* Include Files
******************************************************************************/
#include "gmalg.h"
#include "gmalg_util.h"

#include "gmalg_random.h"
#include "sm2.h"
#include "sm3.h"
#include "sm4.h"
#include <arpa/inet.h>

/******************************************************************************
* Macros
******************************************************************************/
#define D2I_PUB(pub) ((struct ecc_point *)pub->x)
#define D2I_PRI(pri) ((unsigned char *)pri->K)

/******************************************************************************
* Types
******************************************************************************/
typedef struct gmalg_ctx_st {
	sm3_ctx sm3_ctx[1];
	sm4_ctx sm4_ctx[1];
} gmalg_ctx;

/******************************************************************************
* Functions
******************************************************************************/
int GMALG_OpenDevice(
    void **phDeviceHandle)
{
	int rc = 0;
	void *p;

	p = malloc(sizeof(gmalg_ctx));
	if ( p == NULL)
    {
		return -1;
    }

	*phDeviceHandle = p;

	return rc;
}

int GMALG_CloseDevice(
	void *hDeviceHandle)
{
	int rc = 0;

	if(hDeviceHandle)
    {
		free(hDeviceHandle);
    }

	return rc;
}

int GMALG_GenerateRandom(
	void *hDeviceHandle,
	unsigned int uiLength,
	unsigned char *pucRandom)
{
	//gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	int rc = 0;

	rc = vli_get_random(pucRandom, uiLength);

	return rc;
}

int GMALG_pointMul_ECC (
	void *hDeviceHandle,
	ECCrefPublicKey *pucG,
	ECCrefPrivateKey *pucK,
	ECCrefPublicKey *pucP)
{
	int rc = 0;

	sm2_point_mult(D2I_PUB(pucG), D2I_PRI(pucK), D2I_PUB(pucP)); /* U=[x2_]RB */

	return rc;
}

int GMALG_GeneratePublicKey_ECC (
	void *hDeviceHandle,
	ECCrefPrivateKey *pucPrivateKey,
	ECCrefPublicKey *pucPublicKey)
{
	ecc_point pubKey[1];
	int rc = 0;

	rc = sm2_make_pubkey(D2I_PRI(pucPrivateKey), pubKey);

	pucPublicKey->bits = htonl(0x100);
	memcpy(pucPublicKey->x, pubKey->x, ECC_NUMWORD);
	memcpy(pucPublicKey->y, pubKey->y, ECC_NUMWORD);

	return rc;
}

int GMALG_GenerateKeyPair_ECC(
	void *hDeviceHandle,
	ECCrefPublicKey *pucPublicKey,
	ECCrefPrivateKey *pucPrivateKey)
{
	//gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	u8 priKey[ECC_NUMWORD];
	ecc_point pubKey[1];
	int rc = 0;

	rc = sm2_make_prikey(priKey);
	rc = sm2_make_pubkey(priKey, pubKey);

	pucPrivateKey->bits = htonl(0x100);
	memcpy( pucPrivateKey->K, priKey, ECC_NUMWORD);
	pucPublicKey->bits = htonl(0x100);
	memcpy(pucPublicKey->x, pubKey->x, ECC_NUMWORD);
	memcpy(pucPublicKey->y, pubKey->y, ECC_NUMWORD);

	return rc;
}

int GMALG_ExternalSign_ECC(
	void *hDeviceHandle,
	ECCrefPrivateKey *pucPrivateKey,
	unsigned char *pucData,
	unsigned int uiDataLength,
	ECCSignature *pucSignature)

{
	//gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	int rc = 0;

	rc = sm2_sign(pucSignature->r, pucSignature->s, pucPrivateKey->K, pucData);

	return rc;
}

int GMALG_ExternalVerify_ECC(
	void *hDeviceHandle,
	ECCrefPublicKey *pucPublicKey,
	unsigned char *pucDataInput,
	unsigned int uiInputLength,
	ECCSignature *pucSignature)

{
	//gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	int rc = 0;

	rc = sm2_verify(D2I_PUB(pucPublicKey), pucDataInput, pucSignature->r, pucSignature->s);

	return rc;
}

int GMALG_ExternalEncrytp_ECC(
	void *hDeviceHandle,
	ECCrefPublicKey *pucPublicKey,
	unsigned char *pucData,
	unsigned int uiDataLength,
	ECCCipher *pucEncData)

{
	//gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	int rc = 0;

	rc = sm2_encrypt(D2I_PUB(pucPublicKey), pucData, uiDataLength, pucEncData->C, &pucEncData->L);

	memset(pucEncData->M, 0, ECC_NUMWORD);
	memcpy(pucEncData->x, pucPublicKey->x,ECC_NUMWORD);
	memcpy(pucEncData->y, pucPublicKey->y, ECC_NUMWORD);

	return rc;
}

int GMALG_ExternalDecrypt_ECC(
	void *hDeviceHandle,
	ECCrefPrivateKey *pucPrivateKey,
	ECCCipher *pucEncData,
	unsigned char *pucData,
	unsigned int *puiDataLength)

{
	//gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	unsigned int dataLen = 0;
	int rc = 0;

	rc = sm2_decrypt(pucPrivateKey->K, pucEncData->C, pucEncData->L, pucData, &dataLen);

	return rc;
}

int GMALG_Encrypt(
	void *hDeviceHandle,
	void *pucKey,
	unsigned int uiAlgID,
	unsigned char *pucIV,
	unsigned char *pucData,
	unsigned int uiDataLength,
	unsigned char *pucEncData,
	unsigned int *puiEncDataLength)

{
	gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	int rc = 0;

	switch( uiAlgID)
    {
	    case GMALG_SM1_ECB:
        {
    		sm4_ecb_encrypt(ctx->sm4_ctx, pucKey, pucData, uiDataLength, pucEncData);
            break;
	    }
    	case GMALG_SM1_CBC:
        {
    		sm4_cbc_encrypt(ctx->sm4_ctx, pucKey, pucIV, pucData, uiDataLength, pucEncData);
            break;
    	}
    	case GMALG_SM4_ECB:
        {
    		sm4_ecb_encrypt(ctx->sm4_ctx, pucKey, pucData, uiDataLength, pucEncData);
            break;
    	}
    	case GMALG_SM4_CBC:
        {
    		sm4_cbc_encrypt(ctx->sm4_ctx, pucKey, pucIV, pucData, uiDataLength, pucEncData);
            break;
    	}
    	default:
        {
            rc = -1;
            break;
        }
	}
    
    if(puiEncDataLength)
    {
        *puiEncDataLength = uiDataLength;
    }

	return rc;
}

int GMALG_Decrypt (
	void *hDeviceHandle,
	void *pucKey,
	unsigned int uiAlgID,
	unsigned char *pucIV,
	unsigned char *pucEncData,
	unsigned int uiEncDataLength,
	unsigned char *pucData,
	unsigned int *puiDataLength)

{
	gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	int rc = 0;

	switch( uiAlgID)
    {
    	case GMALG_SM1_ECB:
        {
    		sm4_ecb_decrypt(ctx->sm4_ctx, pucKey, pucEncData, uiEncDataLength, pucData);
            break;
        }
    	case GMALG_SM1_CBC:
        {
    		sm4_cbc_decrypt(ctx->sm4_ctx, pucKey, pucIV, pucEncData, uiEncDataLength, pucData);
            break;
        }
    	case GMALG_SM4_ECB:
        {
    		sm4_ecb_decrypt(ctx->sm4_ctx, pucKey, pucEncData, uiEncDataLength, pucData);
    		break;
    	}
    	case GMALG_SM4_CBC:
        {
    		sm4_cbc_decrypt(ctx->sm4_ctx, pucKey, pucIV, pucEncData, uiEncDataLength, pucData);
    		break;
    	}
    	default:
        {
            rc = -1;
            break;
        }
    }

    if(puiDataLength)
    {
        *puiDataLength = uiEncDataLength;
    }

	return rc;
}

int GMALG_HashInit (
	void *hDeviceHandle,
	ECCrefPublicKey *pucPublicKey,
	unsigned char *pucID,
	unsigned int uiIDLength)

{
	gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	u8 Z[ECC_NUMWORD];
	int rc = 0;

	rc = sm3_init(ctx->sm3_ctx);

	if (uiIDLength) {
		sm3_z(pucID, uiIDLength, (ecc_point *)pucPublicKey->x, Z);
		sm3_update(ctx->sm3_ctx, Z, ECC_NUMWORD);
	}

	return rc;
}

int GMALG_HashUpdate (
	void *hDeviceHandle,
	unsigned char *pucData,
	unsigned int uiDataLength)

{
	gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	int rc = 0;

	rc = sm3_update(ctx->sm3_ctx, pucData, uiDataLength);

	return rc;
}

int GMALG_HashFinal (
	void *hDeviceHandle,
	unsigned char *pucHash,
	unsigned int *puiHashLength)

{
	gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	int rc = 0;

    rc = sm3_final(ctx->sm3_ctx, pucHash);
    if(puiHashLength)
    {
        *puiHashLength =  SM3_DIGEST_LENGTH;
    }   

	return rc;
}

typedef struct agreement_t {
	u8 ZA[ECC_NUMWORD];
	u8 priKey[ECC_NUMWORD];
	u8 tempPriKey[ECC_NUMWORD];
	ecc_point pubKey[1];
	ecc_point tempPubKey[1];
	u32 keyLen;
} agreement;

int GMALG_GenerateAgreementDataWithECC (
	void *hDeviceHandle,
	ECCrefPrivateKey *pucSponsePrivateKey,
	ECCrefPublicKey *pucSponsorPublicKey,
	unsigned int uiKey,
	unsigned char *pucSponsorID,
	unsigned int uiSponsorIDLength,
	ECCrefPublicKey *pucSponsorTmpPublicKey,
	void **phAgreementHandle)

{
	//gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	agreement *agree = (agreement *)malloc(sizeof(agreement));
	int rc = 0;

	sm2_make_keypair(agree->tempPriKey, agree->tempPubKey);
	sm3_z(pucSponsorID, uiSponsorIDLength, D2I_PUB(pucSponsorPublicKey), agree->ZA);

	agree->keyLen = uiKey;
	memcpy(agree->pubKey->x, pucSponsorPublicKey->x, ECC_NUMWORD);
	memcpy(agree->pubKey->y, pucSponsorPublicKey->y, ECC_NUMWORD);
	memcpy(agree->priKey, pucSponsePrivateKey->K, ECC_NUMWORD);

	pucSponsorTmpPublicKey->bits = htonl(0x100);
	memcpy(pucSponsorTmpPublicKey->x, agree->tempPubKey->x, ECC_NUMWORD);
	memcpy(pucSponsorTmpPublicKey->y, agree->tempPubKey->y, ECC_NUMWORD);

	*phAgreementHandle = agree;

	return rc;
}

int GMALG_GenerateKeyWithECC (
	void *hDeviceHandle,
	unsigned char *pucResponseID,
	unsigned int uiResponseIDLength,
	ECCrefPublicKey *pucResponsePublicKey,
	ECCrefPublicKey *pucResponseTmpPublicKey,
	void *hAgreementHandle,
	void *phKey)

{
	//gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	agreement *agree = (agreement *)hAgreementHandle;
	ecc_point tempPubKey[1];
	u8 ZB[ECC_NUMWORD];
	int rc = 0;

	sm3_z(pucResponseID, uiResponseIDLength, agree->pubKey, ZB);

	sm2_shared_point(agree->priKey, agree->tempPriKey, agree->tempPubKey,
			D2I_PUB(pucResponsePublicKey), D2I_PUB(pucResponseTmpPublicKey),
			tempPubKey);

	sm2_shared_key(tempPubKey, agree->ZA, ZB, agree->keyLen, phKey);

	free(agree);

	return rc;
}

int GMALG_GenerateAgreementDataAndKeyWithECC(
	void *hDeviceHandle,
	ECCrefPrivateKey *pucResponsePrivateKey,
	ECCrefPublicKey *pucResponsePublicKey,
	unsigned int uiKey,
	unsigned char *pucResponseID,
	unsigned int uiResponseIDLength,
	unsigned char *pucSponsorID,
	unsigned int uiSponsorIDLength,
	ECCrefPublicKey *pucSponsorPublicKey,
	ECCrefPublicKey *pucSponsorTmpPublicKey,
	ECCrefPublicKey *pucResponseTmpPublicKey,
	void *phKey)
{
	//gmalg_ctx *ctx = (gmalg_ctx *)hDeviceHandle;
	u8 tempPriKey[ECC_NUMWORD];
	ecc_point tempPubKey[1];
	u8 ZA[ECC_NUMWORD];
	u8 ZB[ECC_NUMWORD];
	int rc = 0;

	sm2_make_keypair(tempPriKey, tempPubKey);

	sm2_shared_point(D2I_PRI(pucResponsePrivateKey), tempPriKey, tempPubKey,
			D2I_PUB(pucSponsorPublicKey), D2I_PUB(pucSponsorTmpPublicKey),
			tempPubKey);

	sm3_z(pucSponsorID, uiSponsorIDLength, D2I_PUB(pucSponsorPublicKey), ZA);
	sm3_z(pucResponseID, uiResponseIDLength, D2I_PUB(pucResponsePublicKey), ZB);

	sm2_shared_key(tempPubKey, ZA, ZB, uiKey, phKey);

	pucResponseTmpPublicKey->bits = htonl(0x100);
	memcpy(pucResponseTmpPublicKey->x, tempPubKey->x, ECC_NUMWORD);
	memcpy(pucResponseTmpPublicKey->y, tempPubKey->y, ECC_NUMWORD);

	return rc;
}

