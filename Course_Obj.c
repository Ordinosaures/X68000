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
#include "PCG.h"
#include "Raster.h"

//const int16_t	g_nObjViewRateTbl[COURSE_OBJ_PAT_MAX] = { 198, 158, 126, 101, 79, 65, 51, 41, 33 };	/* X����̏ꍇ */
const int16_t	g_nObjViewRateTbl[COURSE_OBJ_PAT_MAX] = { 99, 79, 63, 50, 39, 32, 25, 20, 16 };
//														{96, 77, 61, 49, 39, 31, 25, 20, 16, 13, 10};
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
		g_stCourse_Obj[i].ubPos = i % 2;
		g_stCourse_Obj[i].x = 0;
		g_stCourse_Obj[i].y = Mmul4(COURSE_OBJ_H_MAX * i);
		g_stCourse_Obj[i].z = COURSE_OBJ_PAT_MAX - 1;
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
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	{
//		SetDebugHis(uTimeDiff);
	}
#endif
	
	if(g_stCourse_Obj[bNum].ubAlive == TRUE)
	{
		int16_t	i, y_ofst;
		int16_t	x, y, z;
		int16_t	dx, dy, dz;
		int16_t	mx, my;
		uint16_t	ras_x, ras_y, ras_pat, ras_num;
		uint8_t	bEven;
		int16_t	Out_Of_Disp;
		int16_t	speed = 0;
		uint8_t	ubType;

		ST_CRT	stCRT;
		ST_ROAD_INFO	stRoadInfo;
		ST_RAS_INFO		stRasInfo;
		ST_CARDATA		stMyCar;
		
		GetCRT(&stCRT, bMode);
		GetRoadInfo(&stRoadInfo);
		GetRasterInfo(&stRasInfo);
		GetMyCar(&stMyCar);
		GetMyCarSpeed(&speed);		/* �ԑ��𓾂� */

		if(g_stCourse_Obj[bNum].ubPos == TRUE)	/* �� */
		{
			bEven = TRUE;	/* �� */
		}
		else
		{
			bEven = FALSE;	/* �E */
		}

		/* �O��l */
		x = g_stCourse_Obj[bNum].x;
		y = g_stCourse_Obj[bNum].y;
		z = g_stCourse_Obj[bNum].z;
		ubType = g_stCourse_Obj[bNum].ubType;

//		y += stMyCar.VehicleSpeed;	/* �ω��� */
		if((speed == 0) || (uTimeDiff == 0) )
		{
			/* �ω��Ȃ� */
		}
		else
		{
			if(((x == 0) && (y == 0)))
			{
				y += 1;
			}
			else
			{
				y += uTimeDiff;
			}
		}
		
		/* �ԕ�(�Б�)�́A4m�ƒ�`����B */
		/* 4m��192dot�ł���΁A1dot������0.021m�ƒ�`����B */
		/* ��ʉ���1dot�̓��������Ȃ��̂ŉ�ʉ���192m��ɂƂȂ�B */
		ras_num = Mmax(Mmin(y, stRasInfo.ed), 2);		/* ���X�^�[���̔z��ԍ����Z�o */
		
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
		
		/* ���� */
		if(bEven == TRUE)	/* �� */
		{
			dx = mx - Mmul2(my);
		}
		else				/* �E */
		{
			dx = mx + Mmul2(my);
		}
		/* ������ */
		dy = stRasInfo.st + my;
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		{
			SetDebugHis(dy);
		}
#endif
		/* �������e�����œ_�����^�i�œ_�����{Z�ʒu�j���Q�T�U�{���ĂU�S�Ŋ�����(/4pat) */
		//dz = Mmin( Mmax( 3 - (((z<<8) / (z + ROAD_ED_POINT))>>5) , 0), 3 );
		/* �������e�����œ_�����^�i�œ_�����{Z�ʒu�j���Q�T�U�{����16�Ŋ�����(=7pat) */
		//dz = Mmin( Mmax( 7 - (Mdiv16(Mmul256(z) / (z + ROAD_ED_POINT))) , 0), 7);
		dz = z;
		do
		{
			if(Mmul2(my) >= g_nObjViewRateTbl[z])
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
		
		/* �`�� */
		Out_Of_Disp = Put_CouseObject(	stCRT.hide_offset_x + dx,
										stCRT.hide_offset_y + dy,
										dz,
										bMode_rev,
										ubType,
										bEven);
		
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		Draw_Line(	stCRT.hide_offset_x,
					stCRT.hide_offset_y + dy,
					stCRT.hide_offset_x + WIDTH, 
					stCRT.hide_offset_y + dy,
					10,
					0xFFFF);
#endif
		if( (y < 0) || (y >= Y_MAX_WINDOW) )
		{
			x = 0;
			y = 0;
			z = COURSE_OBJ_PAT_MAX - 1;
			g_stCourse_Obj[bNum].ubAlive = FALSE;
		}

		g_stCourse_Obj[bNum].x = x;
		g_stCourse_Obj[bNum].y = y;
		g_stCourse_Obj[bNum].z = z;
	}
	else
	{
		int16_t	i, cnt = 0;
		int16_t	MinVal = 32767;
		
		g_stCourse_Obj[bNum].x = 0;
		g_stCourse_Obj[bNum].y = 0;
		g_stCourse_Obj[bNum].z = COURSE_OBJ_PAT_MAX - 1;
		
		for(i=0; i<COURSE_OBJ_MAX; i++)
		{
			if(g_stCourse_Obj[i].ubAlive == TRUE)
			{
				MinVal = Mmin( MinVal, g_stCourse_Obj[i].y );
			}
			else
			{
				if(g_stCourse_Obj[i].ubPos == g_stCourse_Obj[bNum].ubPos)
				{
					cnt++;
				}
				else
				{
				}
			}
		}
		
		if( (MinVal > g_nObjViewRateTbl[6]) && (cnt > 1) )
		{
			g_stCourse_Obj[bNum].ubType++;
			if(g_stCourse_Obj[bNum].ubType >= COURSE_OBJ_TYP_MAX)
			{
				g_stCourse_Obj[bNum].ubType = 0u;
			}
			g_stCourse_Obj[bNum].ubAlive = TRUE;
		}
		
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
//		if((bEven == TRUE) && (bNum == 0) && (bMode_rev == 2))
		{
//			SetDebugHis(uTimeDiff);
		}
#endif
	}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	{
//		SetDebugHis(g_stCourse_Obj[bNum].ubAlive);
	}
#endif
	
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
	ret = G_BitBlt_From_Mem( x, y, 0, pSrcBuf, uWidth, uHeight, ubMode, ubPos_H, POS_BOTTOM, PatNumber);

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
	int16_t	i;
	int16_t	count = 0;
	ST_COURSE_OBJ	stTmp;
	
	
	while(1)
	{
		for(i=0; i<COURSE_OBJ_MAX-1; i++)
		{
			if(g_stCourse_Obj[i].y > g_stCourse_Obj[i+1].y)
			{
				stTmp = g_stCourse_Obj[i+1];
				g_stCourse_Obj[i+1] = g_stCourse_Obj[i];
				g_stCourse_Obj[i] = stTmp;
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

	uint8_t	ubMode, ubModeRev;
	int8_t	bZoom;
	int16_t	i, y_ofst;
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
	
	GetRasterInfo(&stRasInfo);
	GetGameMode(&ubMode);	/* ���݂̃��[�h�擾 */
	GetCRT(&stCRT, ubMode);	/* ���݂̉�ʍ��W���擾 */

	y = uCount;
	
	ras_num = Mmax(Mmin(uCount, stRasInfo.ed), 2);		/* ���X�^�[���̔z��ԍ����Z�o */
	
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
	
	ret = GetRasterIntPos(&ras_x, &ras_y, &ras_pat, ras_num, FALSE);	/* �z��ԍ��̃��X�^�[���擾 */
	
	x = stCRT.hide_offset_x + (int16_t)ROAD_CT_POINT + Mu10b_To_s9b((int16_t)ras_x);
	y = stCRT.hide_offset_y + stRasInfo.st + y;
	
	bZoom = (int8_t)uCount - 9;

	/* �`��Ώۂ�OFF�X�N���[���� */
	if(ubMode == 1)
	{
		ubModeRev = 2;
	}
	else
	{
		ubModeRev = 1;
	}

	pSrcBuf = Get_PicImageInfo( ubPictNum, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
	G_Stretch_Pict_toVRAM(x, y, 0, ubModeRev, POS_MID, POS_CENTER, pSrcBuf, uWidth, uHeight, bZoom, ubPictNum);

#ifdef DEBUG
//	printf("H(%d)=(%d, %d, %d)(%d, %d)(%d, %d)\n", ubMode, uCount, ubPictNum, bZoom, x, y, uWidth, uHeight);
//	KeyHitESC();	/* �f�o�b�O�p */
#endif

	return ret;
}

#endif	/* COURSE_OBJ_C */

