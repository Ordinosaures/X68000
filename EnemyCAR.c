#ifndef	ENEMYCAR_C
#define	ENEMYCAR_C

#include <stdio.h>
#include <stdlib.h>
#include <iocslib.h>

#include "inc/usr_macro.h"
#include "EnemyCAR.h"
#include "OverKata.h"

#include "APL_Math.h"
#include "CRTC.h"
#include "Draw.h"
#include "FileManager.h"
#include "Graphic.h"
#include "Input.h"
#include "MFP.h"
#include "Music.h"
#include "MyCar.h"
#include "Raster.h"
#include "Score.h"

/* �O���[�o���ϐ� */
uint8_t		*g_pCG_EnemyCARImageBuf[ENEMYCAR_TYP_MAX][ENEMYCAR_PAT_MAX];
const int16_t	g_nViewRateTbl[ENEMYCAR_PAT_MAX] = {96, 77, 61, 49, 39, 31, 25, 20, 16, 13, 10};
//const int16_t	g_nViewRateTbl[ENEMYCAR_PAT_MAX] = { 198, 158, 126, 101, 79, 65, 51, 41, 33 };

/* �\���̒�` */
ST_ENEMYCARDATA	stEnemyCar[ENEMYCAR_MAX] = {0};
ST_ENEMYCARDATA	*g_pStEnemyCar[ENEMYCAR_MAX];

PICIMAGE	g_stPicEnemyCARImage[ENEMYCAR_TYP_MAX][ENEMYCAR_PAT_MAX];

/* �֐��̃v���g�^�C�v�錾 */
int16_t	InitEnemyCAR(void);
int16_t	GetEnemyCAR(ST_ENEMYCARDATA *, int16_t);
int16_t	SetEnemyCAR(ST_ENEMYCARDATA, int16_t);
int16_t	EnemyCAR_main(uint8_t, uint8_t, uint8_t);
int16_t	SetAlive_EnemyCAR(void);
int16_t	Put_EnemyCAR(uint16_t, uint16_t, uint16_t, uint8_t, uint8_t);
int16_t	Sort_EnemyCAR(void);
int16_t	Load_EnemyCAR(int16_t);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	InitEnemyCAR(void)
{
	int16_t ret = 0;
	int16_t i;
	
	for(i=0; i<ENEMYCAR_MAX; i++)
	{
		stEnemyCar[i].ubCarType = 0;
		stEnemyCar[i].VehicleSpeed = 60;
		stEnemyCar[i].x = 0;
		stEnemyCar[i].y = 0;
		stEnemyCar[i].z = ENEMYCAR_PAT_MAX - 1;
		stEnemyCar[i].ubBrakeLights = 0;
		stEnemyCar[i].ubOBD = 0;
		stEnemyCar[i].ubAlive = FALSE;
		
		g_pStEnemyCar[i] = &stEnemyCar[i];
	}
	
	for(i=0; i<ENEMYCAR_TYP_MAX; i++)
	{
		Load_EnemyCAR(i);	/* �摜��VRAM�ɓW�J */
	}
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	GetEnemyCAR(ST_ENEMYCARDATA *stDat, int16_t Num)
{
	int16_t	ret = 0;
	if(Num < ENEMYCAR_MAX)
	{
		*stDat = *g_pStEnemyCar[Num];
	}
	else
	{
		ret = -1;
	}
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetEnemyCAR(ST_ENEMYCARDATA stDat, int16_t Num)
{
	int16_t	ret = 0;
	
	if(Num < ENEMYCAR_MAX)
	{
		*g_pStEnemyCar[Num] = stDat;
	}
	else
	{
		ret = -1;
	}
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	EnemyCAR_main(uint8_t bNum, uint8_t bMode, uint8_t bMode_rev)
{
	int16_t	ret = 0;
	uint16_t	uCount;
	static uint16_t	uTime = 0xFFFF;
	static uint16_t	uTimeDiff = 0;
	
	GetRoadCycleCount(&uCount);
	
	if(bNum == 0)
	{
		if(uTime == 0xFFFF)
		{
			uTime = uCount;
		}
		uTimeDiff = uCount - uTime;
		uTime = uCount;
	}
	
	if(bNum >= ENEMYCAR_MAX)
	{
		ret = -1;
		return ret;
	}
	
	if(g_pStEnemyCar[bNum]->ubAlive == TRUE)
	{
		int16_t	i, y_ofst;
		int16_t	x, y, z;
		int16_t	cal, dis;
		int16_t	dx, dy, dz;
		int16_t	mx, my;
		uint16_t	ras_x, ras_y, ras_pat, ras_num;
		int16_t	Out_Of_Disp = 0;
		uint8_t	ubType;
		ST_CARDATA	stMyCar;
		ST_CRT		stCRT;
		ST_RAS_INFO	stRasInfo;
		ST_ROAD_INFO	stRoadInfo;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		uint8_t	bDebugMode;
		GetDebugMode(&bDebugMode);
#endif
		ras_x = 0;
		ras_y = 0;
		
		x = g_pStEnemyCar[bNum]->x;
		y = g_pStEnemyCar[bNum]->y;
		z = g_pStEnemyCar[bNum]->z;
		ubType = g_pStEnemyCar[bNum]->ubCarType;
		
		GetCRT(&stCRT, bMode);
		GetMyCar(&stMyCar);
		GetRasterInfo(&stRasInfo);
		GetRoadInfo(&stRoadInfo);

		/* ���C�o���ԂƂ̋��� */
		if(uTimeDiff == 0)
		{
			/* �ω��Ȃ� */
		}
		else
		{
			dis = stMyCar.VehicleSpeed - g_pStEnemyCar[bNum]->VehicleSpeed;
			
			if(g_pStEnemyCar[bNum]->VehicleSpeed == 0)
			{
				y += 1;
			}
			else if(dis == 0)
			{
				/* �ω��i�V */
			}
			else if(dis > 0)
			{
				y += 1;
			}
			else
			{
				y -= 1;
			}
		}
		
		if(y < 0)
		{
			Out_Of_Disp = -1;	/* ����z���ꂽ */
		}
		
		ras_num = Mmin( y, stRasInfo.ed );	/* ���X�^�[���̔z��ԍ����Z�o */

		if(ras_num > 2)
		{
			y_ofst = 0;
			for(i = ras_num; i < stRasInfo.ed; i++)
			{
				uint16_t	ras_x_o, ras_y_o, ras_pat_o;
				ret = GetRasterIntPos(&ras_x_o, &ras_y_o, &ras_pat_o, i - 1, FALSE);	/* �z��ԍ��̃��X�^�[���擾 */
				ret = GetRasterIntPos(&ras_x, &ras_y, &ras_pat, i, FALSE);	/* �z��ԍ��̃��X�^�[���擾 */
				if((ras_pat_o != 0) && (ras_pat == 0))	/* �ŏ���0�̋��E�Őݒ肷�� */
				{
					break;
				}
				y_ofst++;
			}
		}
		else
		{
			ret = GetRasterIntPos(&ras_x, &ras_y, &ras_pat, ras_num, FALSE);	/* �z��ԍ��̃��X�^�[���擾 */
			y_ofst = 0;
		}
		y += y_ofst;
		my = y;
		
		/* �Z���^�[ */
		mx = (int16_t)ROAD_CT_POINT + (int16_t)Mu10b_To_s9b(ras_x);	/* 0-1023 => (-512 < 0 < 512) */
		
		if(x < 8)
		{
			cal = Mmul16(x) * my;
			mx -= Mdiv128(cal);
		}
		else
		{
			cal = x-8;
			cal = Mmul16(cal) * my;
			mx += Mdiv128(cal);
		}

		dx = mx;
		dy = my + stRasInfo.st;
		dz = z;

		/* �������e�����œ_�����^�i�œ_�����{Z�ʒu�j��256�{����16�Ŋ����� pat 0-10 */
		do
		{
			if(Mmul2(my) >= g_nViewRateTbl[z])
			{
				dz = Mdec(z, 1);
	//			y = 0;
			}
			else
			{
				/* �O��l�ێ� */
				break;
			}
			z = dz;
		}
		while(1);

		if( Out_Of_Disp == 0 )	/* ��������艺�� */
		{
			/* �����蔻��̐ݒ� */
			g_pStEnemyCar[bNum]->sx = dx - (Mdiv2(ENEMY_CAR_1_W >> dz));
			g_pStEnemyCar[bNum]->ex = dx + (Mdiv2(ENEMY_CAR_1_W >> dz));
			g_pStEnemyCar[bNum]->sy = dy - (Mdiv2(ENEMY_CAR_1_H >> dz));
			g_pStEnemyCar[bNum]->ey = dy + (Mdiv2(ENEMY_CAR_1_H >> dz));
			
			/* �`�� */
			Out_Of_Disp = Put_EnemyCAR(	stCRT.hide_offset_x + dx,
										stCRT.hide_offset_y + dy,
										dz,
										bMode_rev, ubType);
			
			if(my >= Y_MAX_WINDOW)	/* �ǂ��������� */
			{
				ADPCM_Play(14);	/* SE�F�����Ԓʉ� */
				S_Add_Score();	/* ���_ */
				
				g_pStEnemyCar[bNum]->ubAlive = FALSE;
			}
		}
		else
		{
			ADPCM_Play(16);	/* SE�FWAKAME */
			
			g_pStEnemyCar[bNum]->ubAlive = FALSE;
		}

		g_pStEnemyCar[bNum]->x = x;
		g_pStEnemyCar[bNum]->y = y;
		g_pStEnemyCar[bNum]->z = z;
	}
	else
	{
		/* nop */
	}

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetAlive_EnemyCAR(void)
{
	int16_t	ret = 0;
	int16_t	i;
	ST_CARDATA	stMyCar;

	GetMyCar(&stMyCar);

	for(i=0; i<ENEMYCAR_MAX; i++)
	{
		if(g_pStEnemyCar[i]->ubAlive == FALSE)
		{
			uint32_t	rand = 0;
			/* ���E�̋��� */
			rand = random();
			rand &= 0x0Fu;
			
			g_pStEnemyCar[i]->ubCarType = rand & 0x07u;
			if(g_pStEnemyCar[i]->ubCarType < 4)
			{
				g_pStEnemyCar[i]->VehicleSpeed = Mmax(Mmin(stMyCar.VehicleSpeed-10, 240), 60);
			}
			else
			{
				g_pStEnemyCar[i]->VehicleSpeed = 0;
			}
			g_pStEnemyCar[i]->x = rand;
			g_pStEnemyCar[i]->y = 0;
			g_pStEnemyCar[i]->z = ENEMYCAR_PAT_MAX - 1;
			g_pStEnemyCar[i]->sx = 0;
			g_pStEnemyCar[i]->ex = 0;
			g_pStEnemyCar[i]->sy = 0;
			g_pStEnemyCar[i]->ey = 0;
			g_pStEnemyCar[i]->ubBrakeLights = 0;
			g_pStEnemyCar[i]->ubOBD = 0;
			g_pStEnemyCar[i]->ubAlive = TRUE;
			break;
		}
	}
	ret = i;
	
	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	Put_EnemyCAR(uint16_t x, uint16_t y, uint16_t Size, uint8_t ubMode, uint8_t ubType)
{
	int16_t	ret = 0;

#if 1
	int16_t PatNumber;
	uint16_t	*pSrcBuf = NULL;
	uint32_t	uWidth=0, uHeight=0;
	BITMAPINFOHEADER *pInfo;
	
	if(ubType >= ENEMYCAR_TYP_MAX)return -1;
	if(Size >= ENEMYCAR_PAT_MAX)return -1;
	
	pSrcBuf = g_stPicEnemyCARImage[ubType][Size].pImageData;
	pInfo 	= g_stPicEnemyCARImage[ubType][Size].pBMi;

	uWidth	= pInfo->biWidth;
	uHeight	= pInfo->biHeight;
	PatNumber = ENEMYCAR_CG + ubType;
	
	
	ret = G_BitBlt_From_Mem( x, y, 0, pSrcBuf, uWidth, uHeight, ubMode, POS_MID, POS_CENTER, PatNumber);
#else
	int16_t	i;
	uint16_t	w, h;
	uint32_t	uWidth, uHeight, uFileSize;
	uint32_t	uWidth_o, uHeight_o;
	uint32_t	uW_tmp, uH_tmp;
	uint16_t	height_sum = 0u;
	uint16_t	height_sum_o = 0u;
	
	w = uWidth;
	h = uHeight;
	
	Get_PicImageInfo( ENEMYCAR_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
	uWidth_o = uWidth;
	uHeight_o = uHeight;
	
	for(i=1; i<=Size; i++)
	{
		/* �k����̃T�C�Y */
		height_sum_o += uHeight_o;
		uW_tmp = uWidth_o << 3;
		uWidth_o = Mmul_0p10(uW_tmp);
		uH_tmp = uHeight_o << 3;
		uHeight_o = Mmul_0p10(uH_tmp);
		
		/* ���̏k���� */
		height_sum += uHeight;
		uWidth = uWidth_o;
		uHeight = uHeight_o;
	}
	
	ret = G_BitBlt(	x,	w,	y,	h,	0,
					0,	0+w,	height_sum,	h,	0,
					ubMode, POS_MID, POS_CENTER);
#endif
	
	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	Sort_EnemyCAR(void)
{
	int16_t	ret = 0;
	int16_t	i;
	int16_t	count = 0;
	ST_ENEMYCARDATA	*pStEnemyCar_Tmp;
	
	while(1)
	{
		for(i=0; i<ENEMYCAR_MAX-1; i++)
		{
			if(g_pStEnemyCar[i]->y > g_pStEnemyCar[i+1]->y)
			{
				pStEnemyCar_Tmp = g_pStEnemyCar[i+1];
				g_pStEnemyCar[i+1] = g_pStEnemyCar[i];
				g_pStEnemyCar[i] = pStEnemyCar_Tmp;
				count = 0;
			}
			else
			{
				count++;
			}
		}
		if(count >= (ENEMYCAR_MAX - 1))
		{
			break;
		}
	}
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	Load_EnemyCAR(int16_t Num)
{
	int16_t	ret = 0;
	
	int16_t PatNumber;
	uint16_t height_sum = 0u;
	uint16_t height_sum_o = 0u;
	uint32_t i=0u;
	uint32_t uWidth, uHeight, uFileSize;
	uint32_t uWidth_dst, uHeight_dst;
//	uint32_t uOffset_X = 0u;
	uint16_t *pSrcBuf = NULL;
#if 1
	int32_t	Size;
	uint32_t	uSize8x = 0;
	uint16_t	*pDstBuf = NULL;
	BITMAPFILEHEADER *pFile;
	BITMAPINFOHEADER *pInfo;
	
#endif
	if(Num >= ENEMYCAR_TYP_MAX)return -1;
	PatNumber = ENEMYCAR_CG + Num;

	ret = G_Load_Mem( PatNumber, 0, 0, 0 );	/* ���C�o���� */
	if(ret != 0)return ret;

//	ret = G_Load_Mem( PatNumber, uOffset_X,	0,	0);	/* ���C�o���� */
	pSrcBuf = Get_PicImageInfo( PatNumber, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
	uWidth_dst = uWidth;
	uHeight_dst = uHeight;
#ifdef DEBUG
//	printf("debug1(0x%p)=(%d,%d)\n", pSrcBuf, uWidth, uHeight );
#endif

#if 1
	/* PIC�w�b�_�Ƀ��������蓖�� */
	g_stPicEnemyCARImage[Num][i].pBMf = (BITMAPFILEHEADER*)MyMalloc( FILE_HEADER_SIZE );
	g_stPicEnemyCARImage[Num][i].pBMi = (BITMAPINFOHEADER*)MyMalloc( INFO_HEADER_SIZE );
	pFile = g_stPicEnemyCARImage[Num][i].pBMf;
	pInfo = g_stPicEnemyCARImage[Num][i].pBMi;
	pInfo->biWidth = uWidth;
	pInfo->biHeight = uHeight;
	/* �������̃T�C�Y���Z */
	uSize8x = ((((pInfo->biWidth)+7)/8) * 8);	/* 8�̔{�� */
	Size = (pInfo->biHeight) * uSize8x * sizeof(uint16_t);
	pFile->bfSize = Size;		/* �������T�C�Y�ݒ� */
	/* �������m�� */
//	g_stPicEnemyCARImage[Num][i].pImageData = NULL;
	if(g_stPicEnemyCARImage[Num][i].pImageData != NULL)	/* Null�`�F�b�N */
	{
		MyMfree(g_stPicEnemyCARImage[Num][i].pImageData);	/* ��������� */
	}
	g_stPicEnemyCARImage[Num][i].pImageData = (uint16_t*)MyMalloc( Size );	/* �������̊m�� */
	memset(g_stPicEnemyCARImage[Num][i].pImageData, 0, Size);				/* �������N���A */
//	memcpy(g_stPicEnemyCARImage[Num][i].pImageData, pSrcBuf, Size);			/* �}�X�^�[���烉�C�o���ԗp�̃o�b�t�@�ɃR�s�[ */
	ret = G_Copy_Pict_To_Mem(	g_stPicEnemyCARImage[Num][i].pImageData, uWidth, uHeight, pSrcBuf, uWidth, uHeight);	/* �}�X�^�[���烉�C�o���ԗp�̃o�b�t�@�ɃR�s�[ */
#ifdef DEBUG
//	printf("debug2(0x%p)=(%d,%d)\n", g_stPicEnemyCARImage[Num][0].pImageData, uWidth, uHeight );
//	ret = G_BitBlt_From_Mem( 0, 0, 0, g_stPicEnemyCARImage[Num][0].pImageData, uWidth, uHeight, 0xFF, POS_LEFT, POS_TOP);
//	KeyHitESC();	/* �f�o�b�O�p */
#endif
	
#endif

	if(ret >= 0)
	{
		/* ENEMYCAR_PAT_MAX�p�^�[������� */
		uint32_t	uW_tmp, uH_tmp;
		
		height_sum = 0;
		height_sum_o = 0;

		for(i=1; i <= ENEMYCAR_PAT_MAX; i++)
		{
#if 1
			/* PIC�w�b�_�Ƀ��������蓖�� */
			g_stPicEnemyCARImage[Num][i].pBMf = (BITMAPFILEHEADER*)MyMalloc( FILE_HEADER_SIZE );
			g_stPicEnemyCARImage[Num][i].pBMi = (BITMAPINFOHEADER*)MyMalloc( INFO_HEADER_SIZE );
			pFile = g_stPicEnemyCARImage[Num][i].pBMf;
						pInfo = g_stPicEnemyCARImage[Num][i].pBMi;
#else
			/* �k����̃T�C�Y */
			height_sum_o += uHeight_dst;
#endif
			/* �k����̃T�C�Y(W) */
			uW_tmp = uWidth_dst << 3;
			uWidth_dst = Mmul_0p10(uW_tmp);
			uWidth_dst = Mmax(uWidth_dst, 8);
			
			/* �k����̃T�C�Y(H) */
			uH_tmp = uHeight_dst << 3;
			uHeight_dst = Mmul_0p10(uH_tmp);
			uHeight_dst = Mmax(uHeight_dst, 1);
#if 1
			pInfo->biWidth = uWidth_dst;
			pInfo->biHeight = uHeight_dst;
			/* �������̃T�C�Y���Z */
			uSize8x = ((((pInfo->biWidth)+7)/8) * 8);	/* 8�̔{�� */
			Size = (pInfo->biHeight) * uSize8x * sizeof(uint16_t);
			pFile->bfSize = Size;		/* �������T�C�Y�ݒ� */
			/* �������m�� */
//			g_stPicEnemyCARImage[Num][i].pImageData = NULL;
			if(g_stPicEnemyCARImage[Num][i].pImageData != NULL)	/* Null�`�F�b�N */
			{
				MyMfree(g_stPicEnemyCARImage[Num][i].pImageData);	/* ��������� */
			}
			g_stPicEnemyCARImage[Num][i].pImageData = (uint16_t*)MyMalloc( Size );	/* �������̊m�� */
			memset(g_stPicEnemyCARImage[Num][i].pImageData, 0, Size);				/* �������N���A */
#ifdef DEBUG
//			printf("debug1(%d,0x%p)=(%d,%d)\n", i, g_stPicEnemyCARImage[Num][i].pBMi, pInfo->biWidth, pInfo->biHeight );
//			KeyHitESC();	/* �f�o�b�O�p */
//			printf("debug2(%d,0x%p)=%d\n", i, g_stPicEnemyCARImage[Num][i].pBMf, pFile->bfSize);
//			KeyHitESC();	/* �f�o�b�O�p */
#endif
			
			pDstBuf = g_stPicEnemyCARImage[Num][i].pImageData;		/* ��Ɨp�̃|�C���^�ɃZ�b�g */
			pSrcBuf = g_stPicEnemyCARImage[Num][i-1].pImageData;	/* ��Ɨp�̃|�C���^�ɃZ�b�g */
#ifdef DEBUG
//			printf("debug3(%d,0x%p)(%d,%d)\n", i, pDstBuf, uWidth_dst, uHeight_dst);
//			printf("debug4(%d,0x%p)(%d,%d)\n", i-1, pSrcBuf, uWidth, uHeight);
//			KeyHitESC();	/* �f�o�b�O�p */
#endif
			/* �k���R�s�[ */
			G_Stretch_Pict_To_Mem(	pDstBuf,	uWidth_dst,	uHeight_dst,
									pSrcBuf,	uWidth,		uHeight);
#else
			/* �`�� */
			G_Stretch_Pict( 
							0 + uOffset_X,	uWidth_dst + uOffset_X,
							height_sum_o,	uHeight_dst,
							0,
							0 + uOffset_X,	uWidth + uOffset_X,
							height_sum,		uHeight,
							0);
#endif
			/* ���̏k���� */
			height_sum += uHeight;
#ifdef DEBUG
//			ret = G_BitBlt_From_Mem( 0, height_sum, 0, g_stPicEnemyCARImage[Num][i].pImageData, uWidth_dst, uHeight_dst, 0xFF, POS_TOP, POS_LEFT);
//			KeyHitESC();	/* �f�o�b�O�p */
#endif
			uWidth = uWidth_dst;
			uHeight = uHeight_dst;
		}
	}
	
	return ret;
}

#endif	/* ENEMYCAR_C */

