#ifndef	COURSE_OBJ_C
#define	COURSE_OBJ_C

#include <stdio.h>

#include "inc/usr_macro.h"
#include "Course_Obj.h"
#include "OverKata.h"

#include "APL_Math.h"
#include "CRTC.h"
#include "Draw.h"
#include "FileManager.h"
#include "Graphic.h"
#include "MFP.h"
#include "MyCar.h"
#include "Output_Text.h"
#include "Raster.h"

const int16_t	g_nObjViewRateTbl[COURSE_OBJ_PAT_MAX] = { 198, 158, 126, 101, 79, 65, 51, 41, 33 };

/* �ϐ���` */
uint8_t		*g_pCG_Course_ObjImageBuf[COURSE_OBJ_TYP_MAX][COURSE_OBJ_PAT_MAX];

/* �\���̒�` */
static ST_COURSE_OBJ	g_stCourse_Obj[COURSE_OBJ_MAX] = {0};
PICIMAGE	g_stPicCourse_ObjImage[COURSE_OBJ_TYP_MAX][COURSE_OBJ_PAT_MAX];

/* �֐��̃v���g�^�C�v�錾 */
int16_t	InitCourseObj(void);
int16_t	GetCourseObj(ST_COURSE_OBJ *, int16_t);
int16_t	SetCourseObj(ST_COURSE_OBJ, int16_t);
int16_t	Course_Obj_main(uint8_t, uint8_t, uint8_t);
int16_t	Put_CouseObject(int16_t, int16_t, uint16_t, uint8_t, uint8_t, uint8_t);
int16_t	Sort_Course_Obj(void);
int16_t	Load_Course_Obj(int16_t);
int16_t	Load_Course_Data(uint8_t);
int16_t	Move_Course_BG(uint8_t);
int16_t	Set_Course_TmpObject(uint16_t, uint8_t);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	InitCourseObj(void)
{
	int16_t ret = 0;
	int16_t i;
	
	for(i=0; i<COURSE_OBJ_MAX; i++)
	{
		g_stCourse_Obj[i].ubType = 0;
		g_stCourse_Obj[i].x = 0;
		g_stCourse_Obj[i].y = Mmul4(COURSE_OBJ_H_MAX * i);
		g_stCourse_Obj[i].z = COURSE_OBJ_PAT_MAX - 1;
		g_stCourse_Obj[i].uTime = 0xFFFF;
		g_stCourse_Obj[i].ubAlive = TRUE;
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
int16_t	GetCourseObj(ST_COURSE_OBJ *stDat, int16_t Num)
{
	int16_t	ret = 0;
	if(Num < COURSE_OBJ_MAX)
	{
		*stDat = g_stCourse_Obj[Num];
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
int16_t	SetCourseObj(ST_COURSE_OBJ stDat, int16_t Num)
{
	int16_t	ret = 0;
	
	if(Num < COURSE_OBJ_MAX)
	{
		g_stCourse_Obj[Num] = stDat;
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
int16_t Course_Obj_main(uint8_t bNum, uint8_t bMode, uint8_t bMode_rev)
{
	int16_t	ret = 0;
	uint16_t	uCount;

	GetRoadCycleCount(&uCount);

	if(g_stCourse_Obj[bNum].ubAlive == TRUE)
	{
		int16_t	x, y, z;
		int16_t	dx, dy, dz;
		int16_t	mx, my;
		uint16_t	ras_x, ras_y, ras_pat, ras_num;
		uint8_t	bEven;
		int16_t	Out_Of_Disp;
		uint8_t	ubType;
		uint16_t	uTime;

		ST_CRT	stCRT;
		ST_ROAD_INFO	stRoadInfo;
		ST_RAS_INFO		stRasInfo;
		
		GetCRT(&stCRT, bMode);
		GetRoadInfo(&stRoadInfo);
		GetRasterInfo(&stRasInfo);

		if((bNum & 0x01) == 0)
		{
			bEven = TRUE;
		}
		else
		{
			bEven = FALSE;
		}

		/* �O��l */
		x = g_stCourse_Obj[bNum].x;
		y = g_stCourse_Obj[bNum].y;
		z = g_stCourse_Obj[bNum].z;
		ubType = g_stCourse_Obj[bNum].ubType;
		uTime = g_stCourse_Obj[bNum].uTime;	/* ���ʃJ�E���^�̑O��l */

		
		if(uTime == 0xFFFF)
		{
			uTime = uCount;
		}

		my = uCount - uTime;	/* �ω��� */
		if(my > 0)
		{
			y += Mmax(my >> z, 1);
		}
		
		/* �ԕ�(�Б�)�́A4m�ƒ�`����B */
		/* 4m��192dot�ł���΁A1dot������0.021m�ƒ�`����B */
		/* ��ʉ���1dot�̓��������Ȃ��̂ŉ�ʉ���192m��ɂƂȂ�B */
		ras_num = Mmax(Mmin(y, stRasInfo.ed), 2);		/* ���X�^�[���̔z��ԍ����Z�o */
		
		ret = GetRasterIntPos(&ras_x, &ras_y, &ras_pat, ras_num, FALSE);	/* �z��ԍ��̃��X�^�[���擾 */

		/* �Z���^�[ */
		mx = (int16_t)Mu10b_To_s8b(ras_x);	/* 0-1023 => (-256 < 0 < 255) */
		x += Mmul2(y);
		
		/* ���� */
		if(bEven == TRUE)	/* �� */
		{
			mx += (int16_t)ROAD_CT_POINT - x;
		}
		else				/* �E */
		{
			mx += (int16_t)ROAD_CT_POINT + x;
		}
		/* ������ */
		dx = mx;
		dy = stRasInfo.st;
		/* �������e�����œ_�����^�i�œ_�����{Z�ʒu�j���Q�T�U�{���ĂU�S�Ŋ�����(/4pat) */
		//dz = Mmin( Mmax( 3 - (((z<<8) / (z + ROAD_ED_POINT))>>5) , 0), 3 );
		/* �������e�����œ_�����^�i�œ_�����{Z�ʒu�j���Q�T�U�{����16�Ŋ�����(=7pat) */
		//dz = Mmin( Mmax( 7 - (Mdiv16(Mmul256(z) / (z + ROAD_ED_POINT))) , 0), 7);
		dz = z;
		if(x >= g_nObjViewRateTbl[z])
		{
			dz = Mdec(z, 1);
			y = 0;
		}
		else
		{
			/* �O��l�ێ� */
		}
		z = dz;
		
		/* �`�� */
		Out_Of_Disp = Put_CouseObject(	stCRT.hide_offset_x + dx,
										stCRT.hide_offset_y + dy,
										dz,
										bMode_rev,
										ubType,
										bEven);
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		SetDebugPos(stCRT.hide_offset_x + dx, dx);
#endif
		
		if(Out_Of_Disp != 0)
		{
			x = 0;
			y = 0;
			z = COURSE_OBJ_PAT_MAX - 1;
			g_stCourse_Obj[bNum].ubAlive = FALSE;
		}

		g_stCourse_Obj[bNum].x = x;
		g_stCourse_Obj[bNum].y = y;
		g_stCourse_Obj[bNum].z = z;
		g_stCourse_Obj[bNum].uTime = uCount;
	}
	else
	{
		g_stCourse_Obj[bNum].ubType++;
		if(g_stCourse_Obj[bNum].ubType >= COURSE_OBJ_TYP_MAX)
		{
			g_stCourse_Obj[bNum].ubType = 0u;
		}
		g_stCourse_Obj[bNum].x = 0;
		g_stCourse_Obj[bNum].y = 0;
		g_stCourse_Obj[bNum].z = COURSE_OBJ_PAT_MAX - 1;
		g_stCourse_Obj[bNum].uTime = uCount;
		g_stCourse_Obj[bNum].ubAlive = TRUE;
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
int16_t	Put_CouseObject(int16_t x, int16_t y, uint16_t Size, uint8_t ubMode, uint8_t ubType, uint8_t ubPos)
{
	int16_t	ret = 0;

	int16_t PatNumber;
	uint16_t	*pSrcBuf = NULL;
	uint32_t	uWidth=0, uHeight=0;
	uint8_t		ubPos_H;
	BITMAPINFOHEADER *pInfo;
	
	if(ubType >= COURSE_OBJ_TYP_MAX)return -1;
	if(Size >= COURSE_OBJ_PAT_MAX)return -1;
	
	PatNumber = COURSE_OBJ_CG + ubType;
	
	pSrcBuf = g_stPicCourse_ObjImage[ubType][Size].pImageData;
	pInfo 	= g_stPicCourse_ObjImage[ubType][Size].pBMi;

	uWidth	= pInfo->biWidth;
	uHeight	= pInfo->biHeight;
	
	if(ubPos == TRUE)	/* �� */
	{
		ubPos_H = POS_RIGHT;
	}
	else				/* �E */
	{
		ubPos_H = POS_LEFT;
	}
	ret = G_BitBlt_From_Mem( x, y, 0, pSrcBuf, uWidth, uHeight, ubMode, ubPos_H, POS_CENTER, PatNumber);
	
	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	Sort_Course_Obj(void)
{
	int16_t	ret = 0;
	int16_t	i, ofst;
	int16_t	count = 0;
	int16_t	num;
	ST_COURSE_OBJ	stTmp;
	
	num = Mdiv2(COURSE_OBJ_MAX);
	
	while(1)
	{
		for(i=0; i<num; i++)
		{
			ofst = 0;	/* �� */
			if(g_stCourse_Obj[i+ofst].x > g_stCourse_Obj[i+ofst+2].x)
			{
				stTmp = g_stCourse_Obj[i+ofst+2];
				g_stCourse_Obj[i+ofst+2] = g_stCourse_Obj[i+ofst];
				g_stCourse_Obj[i+ofst] = stTmp;
				count++;
			}
			
			ofst = 1;	/* �E */
			if(g_stCourse_Obj[i+ofst].x > g_stCourse_Obj[i+ofst+2].x)
			{
				stTmp = g_stCourse_Obj[i+ofst+2];
				g_stCourse_Obj[i+ofst+2] = g_stCourse_Obj[i+ofst];
				g_stCourse_Obj[i+ofst] = stTmp;
				count++;
			}
		}
		if(count == 0)	/* �\�[�g���� */
		{
			break;
		}
		else{			/* �ă\�[�g */
			count = 0;
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
int16_t	Load_Course_Obj(int16_t Num)
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

	int32_t	Size;
	uint32_t	uSize8x = 0;
	uint16_t	*pDstBuf = NULL;
	BITMAPFILEHEADER *pFile;
	BITMAPINFOHEADER *pInfo;
	
	if(Num >= COURSE_OBJ_TYP_MAX)return -1;
	PatNumber = COURSE_OBJ_CG + Num;

	ret = G_Load_Mem( PatNumber, 0, 0, 0 );	/* ���V�̖� */
	if(ret != 0)return ret;
	
//	ret = G_Load_Mem( PatNumber, uOffset_X,	0,	0);	/* ���V�̖� */
	pSrcBuf = Get_PicImageInfo( PatNumber, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
	uWidth_dst = uWidth;
	uHeight_dst = uHeight;
#ifdef DEBUG
//	printf("debug1(0x%p)=(%d,%d)\n", pSrcBuf, uWidth, uHeight );
#endif

	/* PIC�w�b�_�Ƀ��������蓖�� */
	g_stPicCourse_ObjImage[Num][i].pBMf = (BITMAPFILEHEADER*)MyMalloc( FILE_HEADER_SIZE );
	g_stPicCourse_ObjImage[Num][i].pBMi = (BITMAPINFOHEADER*)MyMalloc( INFO_HEADER_SIZE );
	pFile = g_stPicCourse_ObjImage[Num][i].pBMf;
	pInfo = g_stPicCourse_ObjImage[Num][i].pBMi;
	pInfo->biWidth = uWidth;
	pInfo->biHeight = uHeight;
	/* �������̃T�C�Y���Z */
	uSize8x = ((((pInfo->biWidth)+7)/8) * 8);	/* 8�̔{�� */
	Size = (pInfo->biHeight) * uSize8x * sizeof(uint16_t);
	pFile->bfSize = Size;		/* �������T�C�Y�ݒ� */
	/* �������m�� */
//	g_stPicCourse_ObjImage[Num][i].pImageData = NULL;
	if(g_stPicCourse_ObjImage[Num][i].pImageData != NULL)	/* Null�`�F�b�N */
	{
		MyMfree(g_stPicCourse_ObjImage[Num][i].pImageData);	/* ��������� */
	}
	g_stPicCourse_ObjImage[Num][i].pImageData = (uint16_t*)MyMalloc( Size );	/* �������̊m�� */
	memset(g_stPicCourse_ObjImage[Num][i].pImageData, 0, Size);					/* �������N���A */
//	memcpy(g_stPicCourse_ObjImage[Num][i].pImageData, pSrcBuf, Size);			/* �}�X�^�[���烄�V�̖؎ԗp�̃o�b�t�@�ɃR�s�[ */
	ret = G_Copy_Pict_To_Mem(	g_stPicCourse_ObjImage[Num][i].pImageData, uWidth, uHeight, pSrcBuf, uWidth, uHeight);	/* �}�X�^�[���烄�V�̖ؗp�̃o�b�t�@�ɃR�s�[ */
#ifdef DEBUG
//	printf("debug2(0x%p)=(%d,%d)\n", g_stPicCourse_ObjImage[Num][0].pImageData, uWidth, uHeight );
//	ret = G_BitBlt_From_Mem( 0, 0, 0, g_stPicCourse_ObjImage[Num][0].pImageData, uWidth, uHeight, 0xFF, POS_LEFT, POS_TOP);
//	KeyHitESC();	/* �f�o�b�O�p */
#endif

	if(ret >= 0)
	{
		/* COURSE_OBJ_PAT_MAX�p�^�[������� */
		uint32_t	uW_tmp, uH_tmp;
		
		height_sum = 0;
		height_sum_o = 0;

		for(i=1; i<COURSE_OBJ_PAT_MAX; i++)
		{
			/* PIC�w�b�_�Ƀ��������蓖�� */
			g_stPicCourse_ObjImage[Num][i].pBMf = (BITMAPFILEHEADER*)MyMalloc( FILE_HEADER_SIZE );
			g_stPicCourse_ObjImage[Num][i].pBMi = (BITMAPINFOHEADER*)MyMalloc( INFO_HEADER_SIZE );
			pFile = g_stPicCourse_ObjImage[Num][i].pBMf;
			pInfo = g_stPicCourse_ObjImage[Num][i].pBMi;

			/* �k����̃T�C�Y(W) */
			uW_tmp = uWidth_dst << 3;
			uWidth_dst = Mmul_0p10(uW_tmp);
			uWidth_dst = Mmax(uWidth_dst, 8);

			/* �k����̃T�C�Y(H) */
			uH_tmp = uHeight_dst << 3;
			uHeight_dst = Mmul_0p10(uH_tmp);
			uHeight_dst = Mmax(uHeight_dst, 1);

			pInfo->biWidth = uWidth_dst;
			pInfo->biHeight = uHeight_dst;
			/* �������̃T�C�Y���Z */
			uSize8x = ((((pInfo->biWidth)+7)/8) * 8);	/* 8�̔{�� */
			Size = (pInfo->biHeight) * uSize8x * sizeof(uint16_t);
			pFile->bfSize = Size;		/* �������T�C�Y�ݒ� */
			/* �������m�� */
//			g_stPicCourse_ObjImage[Num][i].pImageData = NULL;
			if(g_stPicCourse_ObjImage[Num][i].pImageData != NULL)	/* Null�`�F�b�N */
			{
				MyMfree(g_stPicCourse_ObjImage[Num][i].pImageData);	/* ��������� */
			}
			g_stPicCourse_ObjImage[Num][i].pImageData = (uint16_t*)MyMalloc( Size );	/* �������̊m�� */
			memset(g_stPicCourse_ObjImage[Num][i].pImageData, 0, Size);				/* �������N���A */
#ifdef DEBUG
//			printf("debug1(%d,0x%p)=(%d,%d)\n", i, g_stPicCourse_ObjImage[Num][i].pBMi, pInfo->biWidth, pInfo->biHeight );
//			KeyHitESC();	/* �f�o�b�O�p */
//			printf("debug2(%d,0x%p)=%d\n", i, g_stPicCourse_ObjImage[Num][i].pBMf, pFile->bfSize);
//			KeyHitESC();	/* �f�o�b�O�p */
#endif
			
			pDstBuf = g_stPicCourse_ObjImage[Num][i].pImageData;		/* ��Ɨp�̃|�C���^�ɃZ�b�g */
			pSrcBuf = g_stPicCourse_ObjImage[Num][i-1].pImageData;	/* ��Ɨp�̃|�C���^�ɃZ�b�g */
#ifdef DEBUG
//			printf("debug3(%d,0x%p)(%d,%d)\n", i, pDstBuf, uWidth_dst, uHeight_dst);
//			printf("debug4(%d,0x%p)(%d,%d)\n", i-1, pSrcBuf, uWidth, uHeight);
//			KeyHitESC();	/* �f�o�b�O�p */
#endif
			/* �k���R�s�[ */
			G_Stretch_Pict_To_Mem(	pDstBuf,	uWidth_dst,	uHeight_dst,
									pSrcBuf,	uWidth,		uHeight);
			/* ���̏k���� */
			height_sum += uHeight;
#ifdef DEBUG
//			ret = G_BitBlt_From_Mem( 0, height_sum, 0, g_stPicCourse_ObjImage[Num][i].pImageData, uWidth_dst, uHeight_dst, 0xFF, POS_TOP, POS_LEFT);
//			KeyHitESC();	/* �f�o�b�O�p */
#endif
			uWidth = uWidth_dst;
			uHeight = uHeight_dst;
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
int16_t	Load_Course_Data(uint8_t bCourseNum)
{
	int16_t	ret = 0;
	int8_t	str[256];
	uint16_t	x, y;
	ST_ROADDATA *p_stRoadData;
	
	if(bCourseNum == 0u)bCourseNum = 1u;
	
	/* �R�[�X�f�[�^�ǂݍ��� */
	p_stRoadData = (ST_ROADDATA *)GetRoadDataAddr();
	sprintf(str, "data/map/course%02d.csv", bCourseNum);
	File_Load_Course_CSV(str, p_stRoadData, &x, &y);

	ret = y;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	Move_Course_BG(uint8_t bMode)
{
	int16_t	ret = 0;
	
	int16_t	Angle;
	int16_t	Move;
	ST_RAS_INFO	stRasInfo;
	ST_ROAD_INFO	stRoadInfo;
	ST_CARDATA	stMyCar = {0};
	static int16_t	BG_x = X_OFFSET;
	
	GetRasterInfo(&stRasInfo);
	GetRoadInfo(&stRoadInfo);
	Angle = stRoadInfo.angle;

	GetMyCar(&stMyCar);			/* ���Ԃ̏����擾 */
	
	if(stMyCar.VehicleSpeed != 0u)
	{
		if(Angle != 0)
		{
			BG_x = Angle;
		}
		else
		{
			/* �ێ� */
		}
	}
	BG_x = BG_x & 0x3FF;
	
	/* �w�i�𓮂��� */
	Move = 65 + 8 - stRasInfo.st;	/* �w�i�̍��� + �I�t�Z�b�g - �����ʒu */
	if(Move < 0)
	{
		Move = Move << 2;
	}
	G_Scroll(BG_x, Y_MIN_DRAW +        0 + Move, 1);	/* �w�i�X�N���[�� */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	Set_Course_TmpObject(uint16_t uCount, uint8_t ubPictNum)
{
	int16_t	ret = 0;

	uint8_t	ubMode;
	int8_t	bZoom;
	int16_t	x, y;
	uint16_t	ras_x, ras_y, ras_pat, ras_num;
	uint16_t	*pSrcBuf = NULL;
	uint32_t	uWidth, uHeight, uFileSize;
	ST_CRT			stCRT;
	ST_RAS_INFO		stRasInfo;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode;
	uint16_t	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
	GetGameMode(&ubMode);	/* ���݂̃��[�h�擾 */

	pSrcBuf = Get_PicImageInfo( ubPictNum, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */

	ras_num = uCount;
	
	GetRasterInfo(&stRasInfo);
	ret = GetRasterIntPos(&ras_x, &ras_y, &ras_pat, ras_num, FALSE);	/* �z��ԍ��̃��X�^�[���擾 */
	
	GetCRT(&stCRT, ubMode);	/* ���݂̉�ʍ��W���擾 */
	x = stCRT.hide_offset_x - Mu10b_To_s8b((int16_t)ras_x) + Mdiv2(uWidth);
	y = stCRT.hide_offset_y + Mu10b_To_s8b((int16_t)ras_y);
	
	bZoom = (int8_t)uCount - 9;

	/* �`��Ώۂ�OFF�X�N���[���� */
	if(ubMode == 1)ubMode = 2;
	else ubMode = 1;
	
	G_Stretch_Pict_toVRAM(x, y, 0, ubMode, POS_MID, POS_CENTER, pSrcBuf, uWidth, uHeight, bZoom, ubPictNum);

#ifdef DEBUG
//	printf("H(%d)=(%d, %d, %d)(%d, %d)(%d, %d)\n", ubMode, uCount, ubPictNum, bZoom, x, y, uWidth, uHeight);
//	KeyHitESC();	/* �f�o�b�O�p */
#endif

	return ret;
}

#endif	/* COURSE_OBJ_C */

