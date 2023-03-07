#ifndef	RASTER_C
#define	RASTER_C

#include <iocslib.h>
#include <stdio.h>
#include <stdlib.h>

#include <usr_macro.h>
#include "OverKata.h"

#include "Raster.h"

#include "APL_Math.h"
#include "Course_Obj.h"
#include "CRTC.h"
#include "Draw.h"
#include "EnemyCAR.h"
#include "Graphic.h"
#include "MFP.h"
#include "MyCar.h"
#include "Output_Text.h"
#include "PCG.h"
#include "Score.h"
#include "Text.h"

#define MAPCOL_MAX	(5)

/* �ϐ� */
static uint16_t	g_uRoadAnime = 0u;
static uint16_t	g_uRoadAnime_old = 0u;
static uint16_t	g_uRoadAnimeBase = 0u;
static uint16_t	g_uRoadAnimeBase_old = 0u;
static uint16_t	g_uRoadAnimeCount = 0u;
static uint32_t	unRoadAnimeTime = 0xFFFFFFFFu;

static uint16_t	g_uRoadCycleCount = 0u;
static uint16_t	g_uCounter = 0u;
static uint8_t	g_bRoadInitFlag = TRUE;

static uint16_t	g_uMapCounter = 0u;

static int16_t	PositionSum = 0;
static int16_t	ViewAngle = 0;
static uint8_t	bMode_old = 0xFF;
static uint8_t	ubSide = 0;
static int8_t	bSlope_Init = TRUE;
static int8_t	bAngleFlag = TRUE;
static int16_t	Angle_old = 0;
static uint16_t	uRoad_data_old = 0x80;

static int16_t	g_RoadArray[RASTER_H_MAX] = {0u};
static int16_t	g_RoadSize[RASTER_H_MAX] = {0u};

/* �\���̒�` */
ST_RASTER_INT g_stRasterInt[RASTER_H_MAX] = {0};
static ST_RAS_INFO	g_stRasInfo = {0};

ST_ROAD_INFO	g_stRoadInfo = {0};
ST_ROADDATA	g_stRoadData[ROADDATA_MAX] = {0};

/* �֐��̃v���g�^�C�v�錾 */
int16_t	GetRasterInfo(ST_RAS_INFO *);
int16_t	SetRasterInfo(ST_RAS_INFO);
void	Raster_Init(void);
int16_t	Raster_Main(void);
static uint8_t Raster_Calc_H(int16_t *, int16_t, int16_t);
static int16_t Raster_SetCarLinePos(int16_t *, int16_t, int16_t);
static int16_t Raster_SetDriverView(int16_t *, int16_t, int16_t);
static int16_t Raster_SetBndngCnd(int16_t *, int16_t, int16_t);
static uint8_t Raster_Calc_V(int16_t *, int16_t, int16_t);

int16_t	GetRasterPos(uint16_t *, uint16_t *, uint16_t);	/* ���X�^�[�������ʂ��擾 */

int16_t	GetRoadInfo(ST_ROAD_INFO *);
int16_t	SetRoadInfo(ST_ROAD_INFO);
void Road_Init(uint16_t);
void Road_BG_Init(uint16_t);
static void Get_Road_Pat(uint16_t *, uint16_t);
static void Set_Road_Pat_offset(int16_t);
static int16_t Road_Pat_Update(uint16_t);
static void Road_Pat_Base_Update(void);
static void Road_Pat_Start(void);
static void Road_Pat_Stop(void);
static int16_t	Set_Road_Height(void);
static int16_t	Set_Road_Slope(void);
static int16_t	Set_Road_Angle(void);
static int16_t	Set_Road_Distance(void);
static int16_t	Set_Road_Object(void);
int16_t	Road_Pat_Main(uint16_t *);
int16_t	GetRoadCycleCount(uint16_t *);
uint64_t	GetRoadDataAddr(uint16_t);
int16_t	SetHorizon(uint8_t);
static int16_t Load_Road_Background(int16_t);
static int16_t Set_Road_Background_Col(int16_t);
int16_t Road_Map_Draw(uint8_t);
static int16_t Set_Arrow_sp(int16_t);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t GetRasterInfo(ST_RAS_INFO *stDat)
{
	int16_t	ret = 0;
	
	*stDat = g_stRasInfo;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t SetRasterInfo(ST_RAS_INFO stDat)
{
	int16_t	ret = 0;
	
	g_stRasInfo = stDat;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void Raster_Init(void)
{
	uint16_t	i;
	
	g_uRoadAnime = 0u;
	g_uRoadAnime_old = 0u;
	g_uRoadAnimeBase = 0u;
	g_uRoadAnimeBase_old = 0u;
	g_uRoadAnimeCount = 0u;
	unRoadAnimeTime = 0xFFFFFFFFu;

	g_uRoadCycleCount = 0u;
	g_uCounter = 0u;
	g_bRoadInitFlag = TRUE;

	g_uMapCounter = 0u;

	PositionSum = 0;
	ViewAngle = 0;
	bMode_old = 0xFF;
	ubSide = 0;

	bSlope_Init = TRUE;
	bAngleFlag = TRUE;
	Angle_old = 0;
	uRoad_data_old = 0x80;
	
	memset(&g_RoadArray, Y_HORIZON_1, sizeof(int16_t) * RASTER_H_MAX);
	memset(&g_RoadSize, 0x80, sizeof(int16_t) * RASTER_H_MAX);
	
	memset(&g_stRasterInt, 0, sizeof(ST_RASTER_INT) * RASTER_H_MAX);
	memset(&g_stRasInfo, 0, sizeof(ST_RAS_INFO));
	
	memset(&g_stRoadInfo, 0, sizeof(ST_ROAD_INFO));
	memset(&g_stRoadData, 0, sizeof(ST_ROADDATA) * ROADDATA_MAX);
	
	for(i=0; i<RASTER_H_MAX; i++)
	{
		g_stRasterInt[i].x		= 0;
		g_stRasterInt[i].y		= Y_HORIZON_1;
		g_stRasterInt[i].pat	= 0;
	}
	g_stRasterInt[(RASTER_H_MAX - 2) + 0].y = ROAD_ST_POINT;	/* ���荞�݈ʒu�̐ݒ� */
	g_stRasterInt[(RASTER_H_MAX - 2) + 1].y = ROAD_ST_POINT;	/* ���荞�݈ʒu�̐ݒ� */
	g_stRasterInt[(RASTER_H_MAX - 4) + 0].y = ROAD_ST_POINT - Y_HORIZON_1;	/* ���荞�݂�������܂ł̕\���ʒu(���X�^���荞�݈ʒu�ɂ�邸��̍l���͕s�v)	*/
	g_stRasterInt[(RASTER_H_MAX - 4) + 1].y = ROAD_ST_POINT - Y_HORIZON_1;	/* ���荞�݂�������܂ł̕\���ʒu(���X�^���荞�݈ʒu�ɂ�邸��̍l���͕s�v)	*/
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t Raster_Main(void)
{
	int16_t	ret = 0;
	
	uint16_t	uRas_y;
	uint16_t	uRas_st, uRas_mid, uRas_ed, uRas_size;

	int16_t	RoadAngle = 0;
	int16_t	CarAngle = 0;
	int16_t	Diff = 0;
	uint16_t	uRev_uRas_y;
	uint16_t	ras_pat=0;
	int16_t		ras_cal_x=0;
	int16_t		ras_cal_y=0;

	uint8_t bMode;
	uint16_t	uCRT_Tmg;
	
	ST_RASTER_INT	stRasterInt;
	ST_CARDATA 		stMyCar = {0};

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode;
	uint16_t	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
	GetGameMode(&bMode);
#if 0
	if(bMode == bMode_old)
	{
		return ret;	/* ��ʒ��ւ����������Ȃ������ꍇ�́A�������s��Ȃ� */
	}
	else
	{
		ret = 1;	/* �X�V�������� */
	}
#else
	ret = 1;	/* �X�V�������� */
#endif
	
	/* ��̃o�b�t�@�ɋ���/��łQ���m�ۂ��� */
	if(bMode == 1u)		/* �㑤���� */
	{
		ubSide = 1;
	}
	else if(bMode == 2u)	/* �������� */
	{
		ubSide = 0;
	}
	else					/* ���̑� */
	{
		ubSide = 1;
	}
	Get_CRT_Tmg(&uCRT_Tmg);
	
	/* ���Ԃ̏����擾 */
	GetMyCar(&stMyCar);
	CarAngle = stMyCar.Angle;	/* �A���O�� */
	
	RoadAngle = g_stRoadInfo.angle;	/* ���̊p�x */
	
#if 1
	/* ���Ԃ̈ʒu���Z�o */
	Diff = APL_AngleDiff(RoadAngle, CarAngle);	/* �ԂƓ��H�̊p�x�����ŎԂ̈ʒu���ς�� */
//	ViewAngle = - Diff;	/* -128 ot 0 to 128 */
	if(Mabs(Diff) < 4)
	{
		PositionSum = Mdiv2(PositionSum);	/* 2���@ */
		if(PositionSum == -1)
		{
			PositionSum = 0;
		}
	}
	else
	{
		PositionSum += Mdiv8(Diff);
	}
	PositionSum = Mmax(Mmin((PositionSum), 128), -128);
#endif
	
	/* ������ */
	uRas_st = 0;
	uRas_mid = 0;
	uRas_ed = 0;
	uRas_size = 0xFFFF;

	/* ���X�^�[�ʒu */
	g_stRasInfo.st		= SetHorizon(bMode);			/* ���X�^�[�J�n�ʒu(0-128) *//* �������̐ݒ� */
	g_stRasInfo.mid		= g_stRoadInfo.Horizon_Base;	/* ���X�^�[�Œ�ʒu(64) */
	g_stRasInfo.ed		= Y_MAX_WINDOW;					/* ���X�^�[�I���ʒu(160) */
	g_stRasInfo.size	= g_stRasInfo.ed - Mmin(g_stRasInfo.st, g_stRasInfo.mid);	/* ���X�^�[�͈̔� */
	SetRasterInfo(g_stRasInfo);
	
	uRas_st		= g_stRasInfo.st;	/* �J�n�ʒu */
	uRas_mid	= g_stRasInfo.mid;	/* ���Ԉʒu */
	uRas_ed		= g_stRasInfo.ed;	/* �I���ʒu */
	uRas_size	= g_stRasInfo.size;	/* ���X�^�[�͈̔� */
	
	Road_Pat_Start();	/* ����͓��̃p���b�g�̊J�n�ݒ���s�� */
	
	/* �Ȃ����𔽉f */
	g_RoadSize[0] = g_stRoadData[0].bWidth;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
//	SetDebugHis(g_RoadArray[32]);
#endif
	
	/* ���X�^�[���쐬 */
	for(uRas_y=0; uRas_y < uRas_size; uRas_y++ )
	{
		ras_pat=0;
		ras_cal_x=0;
		ras_cal_y=0;
		
		uRev_uRas_y	= uRas_size - uRas_y;	/* uRas_size -> 0 */
		
		/* X���W */
		Raster_Calc_H( &ras_cal_x, uRas_y, uRev_uRas_y );
		
		/* Y���W */
		Raster_Calc_V( &ras_cal_y, uRas_y, uRev_uRas_y );

		/* ���[�h�p�^�[�� */
		Get_Road_Pat(&ras_pat, uRas_y);		/* ���[�h�̃p�^�[�������擾 */
		Set_Road_Pat_offset(uRas_y);		/* ���[�h�p�^�[��臒l�̍X�V */

		stRasterInt.x = M512Roll(ras_cal_x);
		stRasterInt.y = M512Roll(ras_cal_y);
		stRasterInt.pat = ras_pat;

		//memcpy(&g_stRasterInt[Mmul2(uRas_y) + ubSide], &stRasterInt, sizeof(ST_RASTER_INT));
		g_stRasterInt[Mmul2(uRas_y) + ubSide] = stRasterInt;
	}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
//	g_stRasterInt[Mmul2(uRas_y) + ubSide].x = -1;
//	g_stRasterInt[Mmul2(uRas_y) + ubSide].y = -1;
//	g_stRasterInt[Mmul2(uRas_y) + ubSide].pat = -1;
//	SetDebugHis(g_RoadArray[32]);
#endif
	
	/* BG��Y���W��I�ԃ��X�^�̊��荞�݈ʒu�ɑ΂��ĕ\��������BG��Y���W���� */
	/* �����0or512�łȂ��Ƌ�ɓ����o�Ă��� */	/* �Ղ̂Ђ� ����̃A�h�o�C�X�ӏ� */

	if(uCRT_Tmg == 0)	/* ���X�^�Q�x�ǂ݂̏ꍇ(31kHz) */
	{
		g_stRasterInt[(RASTER_H_MAX - 2) + ubSide].y = Mmul2(uRas_st);	/* ���荞�݈ʒu�̐ݒ� */
	}
	else	/* �m���C���^�[���[�X���[�h�̏ꍇ(15kHz) */
	{
		g_stRasterInt[(RASTER_H_MAX - 2) + ubSide].y = uRas_st;	/* ���荞�݈ʒu�̐ݒ� */
	}
	/* BG�p�F�V��̃R���g���[���i0�F���V �` 64�F�_ */
	g_stRasterInt[(RASTER_H_MAX - 4) + ubSide].y = uRas_st;	/* ���荞�݂�������܂ł̕\���ʒu(���X�^���荞�݈ʒu�ɂ�邸��̍l���͕s�v)	*/
	
	SetRasterIntData(ubSide);	/* ���X�^�[���荞�݃f�[�^�ݒ芮�� */
	StartRaster();	/* ���X�^�[���荞�݋��� */
	
	bMode_old = bMode;

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static uint8_t Raster_Calc_H(int16_t *x, int16_t Num, int16_t RevNum)
{
	uint8_t bRet = 0;
	int16_t		Scroll_ofst = 0;
	int16_t		Scroll_w_clp=0;
	int16_t		Ras_x_offset = 0;
	uint16_t	uRas_y_p;
	uint16_t	uRas_size;

	uRas_y_p	= Num + 1;			/* Devide zero */
	uRas_size	= Num + RevNum;
	
	/* �Ԑ��|�W�V���� */
	Raster_SetCarLinePos(&Scroll_ofst, uRas_y_p, uRas_size);

	/* �Ԃ������Ă������ */
	Raster_SetDriverView(&Scroll_w_clp, RevNum, uRas_size);
	
	/* ���H�̋Ȃ��� */
	Raster_SetBndngCnd(&Ras_x_offset, Num, uRas_size);
	
	*x = Scroll_ofst + Scroll_w_clp + Ras_x_offset;
	
	return bRet;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t Raster_SetCarLinePos(int16_t *x, int16_t Count, int16_t Size)
{
	int16_t ret = 0;

	/* �Ԑ��|�W�V���� */
	*x = PositionSum;	/* �ԑ̂̈ړ�(�����ʒu�قǈړ��ʂ����Ȃ�) */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t Raster_SetDriverView(int16_t *x, int16_t Count, int16_t Size)
{
	int16_t ret = 0;

	if(ViewAngle == 0)
	{
	}
	else
	{
		/* =�����̓��H�ɑ΂��� ViewAngle=0�Ő��ʁA127=�������A-128=�E���� */
		*x = (int16_t)APL_sDiv((int32_t)ViewAngle * Count, (int32_t)Size);	/* ���C�p�[�I�Ȑ� */
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
static int16_t Raster_SetBndngCnd(int16_t *x, int16_t Count, int16_t Size)
{
	int16_t ret = 0;
	int16_t Gain;
	
	static int16_t bFlag;
	
	if(	Count == 0 )
	{
		if(g_RoadArray[Count] == g_stRoadInfo.slope)
		{
			bFlag = FALSE;
		}
		else
		{
			bFlag = TRUE;
		}
		g_RoadArray[Count] = g_stRoadInfo.slope;
		
		*x = - g_RoadArray[Count];
	}
	else
	{
		//	*x = - ((g_RoadArray[0] * (Size - Count)) / Size);
		if(bFlag == TRUE)
		{
			Gain = 103;
	
		}
		else
		{
			Gain = 0;
		}
		g_RoadArray[Count] = Mdiv128((g_RoadArray[Count] * (0x80 - Gain)) + (g_RoadArray[Count-1] * Gain));
		*x = - ((g_RoadArray[Count] * (Size - Mdiv4(Count))) / Size);
	}
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
//	if(Count == 1)
//	{
//		//SetDebugHis(*x);
//	}
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
static uint8_t Raster_Calc_V(int16_t *y, int16_t Num, int16_t RevNum)
{
	uint8_t bRet = 0;
	int16_t RoadPoint_y = 0;
	int16_t	Point_y, Point_y_def;
	int16_t	MinClip, MaxClip;
	int16_t	RasStart;
	int16_t	Road_strch = 0;
	int16_t	Size;
	int16_t	Base;
	int16_t	Offset = 0;
	int16_t Gain;
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode;
	uint16_t	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	Gain = 103;
#if 0
	if(	(g_stRasInfo.mid == g_stRoadInfo.Horizon)	/* �������̈ʒu���ʏ��艺���̏��� */
		|| (g_CpuTime < 300) )						/* �ᑬMPU */
	{
		*y = ROAD_ST_POINT - g_stRasInfo.mid;	/* �W���ʒu */
		return bRet;
	}
#endif	
	
	/* Y���W */
//	RasStart = Mminmax(g_stRoadData[0].bWidth, 0, 160);
	RasStart = Mmin(g_stRasInfo.st, g_stRasInfo.mid);
	Base	= g_stRasInfo.mid;
	Size	= g_stRasInfo.size;
	MaxClip = Y_HORIZON_1;	/* BG�̓��i�Ō���j */
#if 1
	/* ���Z�͓����傫���ɂȂ� */
	/* ���Z�͓����g�傷�� */

	if(Num == 0)
	{
		/* ���̂܂� */
	}
	else
	{
		g_RoadSize[Num] = Mdiv128((g_RoadSize[Num] * (0x80 - Gain)) + (g_RoadSize[0] * Gain));
	}
	Offset = g_RoadSize[Num] - 0x80;
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	SetDebugHis(Offset);
#endif
	
	//Point_y_def	= ROAD_ST_POINT - RasStart;	/* BG�̍��W�i���H�̐擪�j- ���X�^���荞�݊J�n�ʒu�iX���W�j = ���H�̐擪 */
	if( Base >= RasStart )	/* ���Ԃ̈ʒu���ʏ���㑤�̏��� */
	{
		//g_stRoadData[0].bWidth
		Point_y_def = Base;	/* ��_ */
		Road_strch = ((Base - RasStart + Offset) * RevNum) / Size;	/* ���荞�݈ʒu���Ƃ̍��W�v�Z */

		MinClip = Mmax(Base - (RasStart + Num), 0);	/* BG�̓��i�擪�j */
		RoadPoint_y = Mmax(Road_strch, MinClip);
		MaxClip = Mmax(Base - (RasStart + Num) + ROAD_SIZE - 1, 0);	/* BG�̓��i�Ō���j */
		RoadPoint_y = Mmin(RoadPoint_y, MaxClip);
//		RoadPoint_y = Road_strch;
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		if(Num == uDebugNum)
		{
			SetDebugPos(RoadPoint_y, Road_strch);
		}
#endif
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		if(Num == 1)
		{
			SetDebugHis(MaxClip);
		}
#endif
		
//		Road_strch = Mdiv256(Mmul256((Size * RevNum)) / ROAD_SIZE);
#if 0		
		Size = g_stRasInfo.mid - g_stRasInfo.st;
		Road_strch	= (-1 * Size * Num) / Mdiv2(Y_MAX_WINDOW);
		Offset = Mdiv8(APL_Sin(Num));
		Road_strch	+= Size - Offset;
#endif
		bRet = 0xFF;
	}
	else	/* �������̈ʒu���ʏ��艺���̏��� */
	{
		Point_y_def = RasStart + Offset;
		Road_strch = -1 * Mdiv256(Mmul256((RasStart - Y_HORIZON_1) * Num) / ROAD_SIZE);
//		Size = g_stRasInfo.st - g_stRasInfo.mid;
//		Road_strch	= (-1 * Size * Num) / Mdiv2(Y_MAX_WINDOW);
//		Offset = Mdiv8(APL_Sin(2*Num));
//		Road_strch	-= Size + Offset;
#if 0		
		Size = (g_stRasInfo.size + (g_stRasInfo.st - g_stRasInfo.mid));
		if(Size != 0)
		{
			Road_strch	= (g_stRasInfo.size / Size) * Num;
		}
		else
		{
			Road_strch	= 0;
		}
#endif
		bRet = 0xb7;
	}
#else
	Road_strch = Mdiv1024( (int16_t)g_stRasInfo.size * APL_Cos(2*(Num+90)) );
//	Road_strch = Num - 64;	/* ���`�ȓo��� */
//	Road_strch = (Num * Num)/200 - 64;	/* �񎟋Ȑ� */
	RoadPoint_y = Mmin((Point_y_def + Road_strch), 96);
	Point_y = RoadPoint_y;	/* �㉺���N���b�v */
#endif

	Point_y = M512Roll(Point_y_def + RoadPoint_y);	/* 0 - 512 */
	*y = Point_y;
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	if((Size -1) == Num)
	{
//		SetDebugHis(Point_y);
	}
#endif
	
	return bRet;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t GetRoadInfo(ST_ROAD_INFO *stDat)
{
	int16_t	ret = 0;
	
	*stDat = g_stRoadInfo;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t SetRoadInfo(ST_ROAD_INFO stDat)
{
	int16_t	ret = 0;
	
	g_stRoadInfo = stDat;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void Road_Init(uint16_t uCourseNum)
{
	uint32_t	i;
	
	g_uRoadAnimeBase = 3;
	g_uRoadAnime = 0;
	g_bRoadInitFlag = TRUE;
	Road_Pat_Start();
	
	g_uCounter = 0u;
	g_uRoadCycleCount = 0u;

	g_stRoadInfo.Horizon		= 0x00;	/* �����ʒu */
	g_stRoadInfo.Horizon_Base	= 0x00;	/* �������ʒu */
	g_stRoadInfo.offset_x		= 0x00;	/*  */
	g_stRoadInfo.offset_y		= 0x00;	/*  */
	g_stRoadInfo.offset_val		= 0x00;	/*  */
	g_stRoadInfo.slope			= 0x00;	/*  */
	g_stRoadInfo.angle			= 0x00;	/*  */
	g_stRoadInfo.distance		= 0x00;	/*  */
	g_stRoadInfo.object			= 0x00;	/*  */
	
	for(i = 0u; i < ROADDATA_MAX; i++)
	{
		g_stRoadData[i].bHeight		= 0x80;	/* ���̕W��	(0x80�Z���^�[) */
		g_stRoadData[i].bWidth		= 0x80;	/* ���̕�	(0x80�Z���^�[) */
		g_stRoadData[i].bAngle		= 0x80;	/* ���̊p�x	(0x80�Z���^�[) */
		g_stRoadData[i].bfriction	= 0x80;	/* ���̖��C	(0x80�Z���^�[) */
		g_stRoadData[i].bPat		= 0x00;	/* ���̎��	 */
		g_stRoadData[i].bObject		= 0x00;	/* �o���|�C���g�̃I�u�W�F�N�g�̎�� */
		g_stRoadData[i].bRepeatCount= 0x00;	/* �J��Ԃ��� */
	}
		
	if(uCourseNum == 0u)
	{
		/* �f�o�b�O���[�h */
		g_stRoadInfo.Courselength = ROADDATA_MAX - 1;
	}
	else
	{
		/* �R�[�X�f�[�^�̓ǂݍ��� */
		g_stRoadInfo.Courselength = Load_Course_Data(uCourseNum);	/* �R�[�X�̑S�� */
	}

	Set_Road_Angle();		/* �R�[�X�̋Ȃ��� */
	Set_Road_Height();		/* �R�[�X�̕W�� */
	Set_Road_Slope();		/* �R�[�X�̌��ݒn����W���܂ł̌X�� */
	Set_Road_Distance();	/* ���������猻�ݒn�܂ł̋��� */
	Set_Road_Object();		/* ���C�o���Ԃ̏o�� */
	
	/* �������̐ݒ� */
	SetHorizon(1);
	
	Raster_Main();	/* �R�[�X�̏��� */
}
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void Road_BG_Init(uint16_t uCourseNum)
{
	uint32_t	i;
	
	/* ���H�̃p���b�g�f�[�^ */
	g_uMapCounter = 0;
	
	if(uCourseNum == 0u)
	{
		/* �f�o�b�O���[�h */
		return;
	}

	Load_Road_Background(uCourseNum);	/* �R�[�X�w�i�̓W�J */
	
	/* �R�[�X��Q���̓W�J */
	for(i = 0u; i < COURSE_OBJ_TYP_MAX; i++)
	{
		Load_Course_Obj(i);		/* �R�[�X��Q���ǂݍ��݂̓W�J */
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static void Get_Road_Pat(uint16_t *p_uPat, uint16_t uNum)
{
#if 1
	uint16_t	uBG_pat[4][4] = {288,   0,  96, 192, 
		 						 192, 288,   0,  96, 
		 						  96, 192, 288,   0, 
		 						   0,  96, 192, 288} ;
//	if(uNum == 0)
//	{
//		*p_uPat = uBG_pat[3][0];
//	}
//	else
	{
		*p_uPat = uBG_pat[g_uRoadAnimeBase][g_uRoadAnime];
	}
#else
	*p_uPat = 0;
#endif
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static void Set_Road_Pat_offset(int16_t Num)
{
	if( (Num == 0) || (g_stRoadInfo.offset_val == 0) )
	{
		if(g_stRasInfo.st <= g_stRasInfo.mid)
		{
			g_stRoadInfo.offset_val = 0;
			g_stRoadInfo.offset_y = 0;
		}
		else
		{
			g_stRoadInfo.offset_val = (g_stRasInfo.st - g_stRasInfo.mid);
			g_stRoadInfo.offset_y = 16;
		}
	}
	else
	{
		int16_t	Offset = 0;
		int16_t Size;
		Offset	= g_stRoadData[0].bWidth - 0x80;
		
		if(g_stRasInfo.st <= g_stRasInfo.mid)
		{
			Size = Num + Mmax(Offset, 0);
		}
		else
		{
			Size = Num + (g_stRasInfo.st - g_stRasInfo.mid) + Mmax(Offset, 0);
//			Size = g_stRasInfo.size + (g_stRasInfo.st - g_stRasInfo.mid);
		}
		//g_stRoadInfo.offset_y = (Num * g_stRoadInfo.offset_val) / Mmax((g_stRoadInfo.offset_val + 96), 1);
		g_stRoadInfo.offset_y = APL_uDiv((Size * g_stRoadInfo.offset_val), (g_stRoadInfo.offset_val + g_stRasInfo.size ));
	}
	if(Road_Pat_Update(g_stRoadInfo.offset_y) != 0u)
	{
		g_stRoadInfo.offset_val++;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t Road_Pat_Update(uint16_t uThreshold)
{
	int16_t	ret = 0;
	
	g_uRoadAnimeCount++;
	if(g_uRoadAnimeCount >= uThreshold)
	{
		g_uRoadAnimeCount = 0;
		g_uRoadAnime++;
		ret = 1;
		
	}
	
	if(g_uRoadAnime >= 4u)
	{
		g_uRoadAnime = 0u;
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
static void Road_Pat_Base_Update(void)
{
	g_uRoadAnimeCount = 0xFFF;	/* �t���A�j���[�V�����h�~ */

	g_uRoadAnime = 0;

	g_uRoadAnimeBase++;
	if(g_uRoadAnimeBase >= 4)
	{	
		g_uRoadAnimeBase = 0;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static void Road_Pat_Start(void)
{
	g_uRoadAnime_old = g_uRoadAnime;
	g_uRoadAnimeBase_old = g_uRoadAnimeBase;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static void Road_Pat_Stop(void)
{
	g_uRoadAnime = g_uRoadAnime_old;
	g_uRoadAnimeBase = g_uRoadAnimeBase_old;
	g_uRoadAnimeCount = 0;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	Set_Road_Height(void)
{
	int16_t	ret = 0;
	int16_t	road_height;
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode;
	uint16_t	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	if(bDebugMode == TRUE)
	{
		if(uDebugNum <= 0x80)
		{
			/* �R�[�X�f�[�^�ǂݍ��� */
			road_height = ((int16_t)g_stRoadData[g_uCounter].bHeight) - 0x80;	/* ���̕W��	(0x80�Z���^�[) */
		}
		else
		{
			road_height = g_stRoadInfo.height;
		}
	}
	else
#endif
	{
		/* �R�[�X�f�[�^�ǂݍ��� */
		road_height = ((int16_t)g_stRoadData[g_uCounter].bHeight) - 0x80;	/* ���̕W��	(0x80�Z���^�[) */
	}
	road_height = Mmax(Mmin(road_height, ROAD_1_MAX), ROAD_1_MIN);		/* +�����F-�Ⴂ */

	g_stRoadInfo.height = road_height;		/* �X�V */

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	Set_Road_Slope(void)
{
	int16_t	ret = 0;
	int16_t AngleDiff = 0;
	int16_t	Angle;
	int16_t	Slope;
	
	Angle = g_stRoadInfo.angle;
	Slope = g_stRoadInfo.slope;
	if(bSlope_Init == TRUE)
	{
		bSlope_Init = FALSE;
		Angle_old = Angle;
	}
	
	if(Angle == Angle_old)		/* stop */
	{
//		Slope = Mdiv2(Slope);	/* 2���@ */
//		if(Slope == -1)	/* �Z�p�V�t�g�̃}�C�i�X���͂O�Ɏ������Ȃ��΍� */
//		{
//			Slope = 0;
//		}
		AngleDiff = 0;
	}
	else
	{
		AngleDiff = APL_AngleDiff(Angle_old, Angle);
	}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	//SetDebugHis(AngleDiff);
#endif
	
	Slope += AngleDiff * 8;
	Slope = Mminmax(Slope, -384, 384);
//	Slope = Mminmax(Slope, -511, 511);
	
	g_stRoadInfo.slope = Slope;
	
	Angle_old = g_stRoadInfo.angle;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	Set_Road_Angle(void)
{
	int16_t	ret = 0;
	int16_t	road_angle_diff = 0;
	int16_t	road_angle = 0;
	int16_t	road_angle_old = 0;

	uint16_t	uRoad_data = 0;
	
	/* �R�[�i�[ */
	uRoad_data = g_stRoadData[g_uCounter].bAngle;	/* �R�[�X�f�[�^ */
	
	uRoad_data = Mdiv256(((192 * g_stRoadData[g_uCounter].bAngle) + ((255 - 192) * uRoad_data_old)));	/* �ꎟ���[�p�X�t�B���^ */
	
	road_angle_diff = (int16_t)((int16_t)uRoad_data_old - (int16_t)uRoad_data);		/* ���̊p�x */

	road_angle_old = g_stRoadInfo.angle;	/* �O��l */
	road_angle = road_angle_old + road_angle_diff;
	
	if(road_angle >= 360)
	{
		road_angle -= 360;
	}
	else if(road_angle < 0)
	{
		road_angle += 360;
	}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
//	SetDebugHis(road_angle);	
#endif
	g_stRoadInfo.angle = road_angle;	/* 0-360�x�̊p�x */

	uRoad_data_old = uRoad_data;
	
	ret = road_angle_diff;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static	int16_t	Set_Road_Distance(void)
{
	int16_t	ret = 0;
	int16_t	road_distance;

	road_distance = 0;
	g_stRoadInfo.distance	= road_distance;	/*  */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	Set_Road_Object(void)
{
	int16_t	ret = 0;
	int16_t	road_object = 0;
	
	road_object = g_stRoadData[g_uCounter].bObject;	/* �o���|�C���g�̃I�u�W�F�N�g�̎�� */
	if(road_object != 0u)
	{
		SetAlive_EnemyCAR();	/* ���C�o���Ԃ��o�������� */
	}
	g_stRoadInfo.object		= road_object;		/*  */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t Road_Pat_Main(uint16_t *uRoadCounter)
{
	int16_t	ret = 0;
	uint32_t	time_st = 0u;
	int16_t	speed = 0;

	/* ���������݂̂̓��� */
	if(g_bRoadInitFlag == TRUE)
	{
		speed = 1;
		time_st = 0xFFFFu;
		unRoadAnimeTime = 0u;

		ret = 1;	/* �X�V���� */
	}
	else
	{
		GetStartTime(&time_st);		/* �J�n���Ԃ��擾 */
		GetMyCarSpeed(&speed);		/* �ԑ��𓾂� */

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		if(*uRoadCounter == 0xFFFFu)	/* �f�o�b�O���[�h */
		{
			speed = 1;
		}
#endif
	}
	
	
	if(speed == 0u)	/* �ԑ�0km/h */
	{
		unRoadAnimeTime = time_st;	/* �����X�V */

		Road_Pat_Stop();	/* ���̃A�j����~ */
	}
	else
	{
		uint32_t uTH;
		uint32_t uTimeDiff;
		
		uTH = (362 / speed);
		uTimeDiff = time_st - unRoadAnimeTime;
		
		if( uTimeDiff < uTH )	/* �X�V�������� */
		{
			/* �Z���^�[���C���̒����ƊԊu�͂T�� */
			/* 60fps=17ms/f */
			/* �ő呬�x307km/h(85m/s,0.0853m/ms) 1f=1.45m�i�� */
			/* �Œᑬ�x6km/h(1.7m/s,0.0017m/ms) 1f=0.028m�i�� */
			/* 1LSB 10km/h(2.8m/s,0.0028m/ms) 1f=0.047m�i�� */
			/* 1times Cal 1m=4dot (17ms/f x 1/0.047 / speed) = (362/speed)*/

			Road_Pat_Stop();	/* ���̃A�j����~ */
		}
		else
		{
			unRoadAnimeTime = time_st;	/* �����X�V */
			
			Road_Pat_Base_Update();	/* ���̃A�j���X�V */

			Set_Road_Angle();		/* �R�[�X�̋Ȃ��� */
			Set_Road_Height();		/* �R�[�X�̕W�� */
			Set_Road_Slope();		/* �R�[�X�̌��ݒn����W���܂ł̌X�� */
			Set_Road_Distance();	/* ���������猻�ݒn�܂ł̋��� */
			
			Set_Road_Object();		/* ���C�o���Ԃ̏o�� */
			
			S_Add_Score_Point(speed * 10);	/* �ԑ��ŉ��_ */
			
			Raster_Main();			/* �R�[�X�̏��� */
			
			g_uCounter++;			/* �R�[�X�f�[�^�̃J�E���^�X�V */
			g_uRoadCycleCount++;	/* �R�[�X�X�V�t���[�����J�E���^ */

			ret = 1;	/* �X�V���� */
		}
	}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	if(*uRoadCounter == 0xFFFFu)	/* �f�o�b�O���[�h */
	{
		g_uCounter = 0;
	}
#endif
	
	*uRoadCounter = g_uCounter;
	
	if(g_uCounter >= g_stRoadInfo.Courselength)
	{
		g_uCounter = 0u;
		
		/* �w�i��ύX */
		g_uMapCounter++;
		if(g_uMapCounter >= MAPCOL_MAX)
		{
			g_uMapCounter = 0;
		}
		Set_Road_Background_Col(g_uMapCounter);

		*uRoadCounter = 0xFFFF;
		ret = 1;	/* �X�V���� */
	}
	
	Set_Arrow_sp(g_stRoadInfo.slope);	/* ��� */
	
	/* ���������݂̂̓��� */
	if(g_bRoadInitFlag == TRUE)
	{
		g_bRoadInitFlag = FALSE;
		GetStartTime(&time_st);		/* �J�n���Ԃ��擾 */
		unRoadAnimeTime = time_st;	/* �����X�V */
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
int16_t	GetRoadCycleCount(uint16_t *p_uCount)
{
	int16_t	ret = 0;
	
	*p_uCount = g_uRoadCycleCount;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
uint64_t GetRoadDataAddr(uint16_t unNum)
{
	return (uint64_t)&g_stRoadData[unNum];
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetHorizon(uint8_t bMode)
{
	int16_t	ret = 0;
	int16_t	Height;

	Height = g_stRoadInfo.height;	/* �����������ύX */

	/* ������ */
	/* ���X�^�[���荞�ݏ����̊J�n�E�I���ʒu */
	/* �p�x�Ƌ����ƕW�����琅�����̈ʒu���Z�o����B */
	/* �p�x�Ƌ����ŋ��߂������ƕW������v���Ă���΁A���j�A�ɐL�k */
	/* �p�x�Ƌ����ŋ��߂��������W�����傫���ꍇ�A�r���ŃJ�b�g����铹 */
	/* �p�x�Ƌ����ŋ��߂��������W�����������ꍇ�A�Z���\�����铹 */
	switch(bMode)
	{
		case 0:		/* TPS */
		{
			g_stRoadInfo.Horizon_Base = Y_HORIZON_0;
			g_stRoadInfo.Horizon = Y_HORIZON_0 - Height;
			ret = Mmax(Mmin(Y_HORIZON_0 - Height, ROAD_0_MAX), ROAD_0_MIN);	/* �����ʒu����i�N���b�v�j */
			break;
		}
		case 1:		/* FPS */
		case 2:		/* FPS */
		default:	/* FPS */
		{
			g_stRoadInfo.Horizon_Base = Y_HORIZON_1;
			g_stRoadInfo.Horizon = Y_HORIZON_1 - Height;
			ret = Mmax(Mmin(Y_HORIZON_1 - Height, ROAD_1_MAX), ROAD_1_MIN);	/* �����ʒu����i�N���b�v�j */
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
static int16_t Load_Road_Background(int16_t Num)
{
	int16_t	ret = 0;
	int16_t	i, count, mod;
	uint16_t	*pImage = NULL;
	uint16_t	*pSrcBuf = NULL;
	uint32_t uWidth, uHeight, uFileSize;
	uint8_t ubPatNumber;
	uint16_t x, y;
	
	ubPatNumber = Mmin(Mmax(BG_CG + Num, BG_CG), BG_CG);
	
	ret = G_Load_Mem( ubPatNumber, 0, 0, 0 );	/* BG_CG */
	if(ret != 0)return ret;
	
	pImage = Get_PicImageInfo( ubPatNumber, &uWidth, &uHeight, &uFileSize );	/* �C���[�W���̎擾 */
	
	if(pImage != NULL)
	{
		if(uWidth != 0u)
		{
			/* X */
			count = X_MAX_DRAW / uWidth;
			mod = X_MAX_DRAW % uWidth;
			
			if(mod != 0)	/* �]�肪����Ȃ���E�܂Ŗ��߂� */
			{
				count++;
			}
			
			/* �������ɕ~���l�߂� */
			for(i = 0; i < count; i++)
			{
				pSrcBuf = pImage;
				x = uWidth * i;
//				y = Y_MIN_DRAW +        0 + Y_HORIZON_1 - uHeight + RASTER_MIN;
				y = Y_MIN_DRAW;
				ret = G_BitBlt_From_Mem( x, y, 1, pSrcBuf, uWidth, uHeight, 0xFF, POS_LEFT, POS_TOP, ubPatNumber);	/* �w�i(�㑤) */
//				G_Load_Mem( ubPatNumber, uWidth * i, Y_MIN_DRAW +        0 + Y_HORIZON_1 - uHeight + RASTER_MIN, 1);	/* �w�i(�㑤) */
			}
		}
		else
		{
			ret = -1;	/* �������Ȃ� */
		}
	}
	else
	{
		ret = -1;		/* �摜���Ȃ� */
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
static int16_t Set_Road_Background_Col(int16_t Num)
{
	int16_t	ret = 0;

	uint32_t	i, j;
	uint16_t uRoadCol[MAPCOL_MAX][4][5] ={
#if 1
		{/* 0 */
			0x9C62,0xEF76,0xEF76,0xEF7A,0xA252,
			0x9C62,0xEF76,0x8F40,0xEF7A,0xA210,
			0xACE6,0x8F40,0xEF76,0xACE6,0x918C,
			0xACE6,0x8F40,0x8F40,0xACE6,0x918C
		},
#endif
#if 1
		{/* 1 */
			0x9C62,0xEF76,0xEF76,0x9C62,0xBE4E,
			0x9C62,0xEF76,0x8F40,0x9C62,0xBE4E,
			0xACE6,0x8F40,0x8F40,0xACE6,0xCED2,
			0xACE6,0x8F40,0xEF76,0xACE6,0xCED2
		},
#endif
#if 1
		{/* 2 */
			0xAF5A,0x9DC0,0x9DC0,0xAF5A,0xDF66,
			0xAF5A,0x9DC0,0xEF72,0xAF5A,0xDF66,
			0xBF5E,0xEF72,0x9DC0,0xBF5E,0xCF62,
			0xBF5E,0xEF72,0xEF72,0xBF5E,0xCF62
		},
#endif
#if 1
		{/* 3 */
			0x739C,0x9CE6,0xA528,0xEF7A,0xAA52,
			0x739C,0x9CE6,0xDEF6,0xEF7A,0xAA52,
			0x6B5A,0xDEF6,0xA528,0x6B5A,0x99CE,
			0x6B5A,0xDEF6,0xDEF6,0x6B5A,0x99CE
		},
#endif
#if 1
		{/* 4 */
			0x8420,0x669A,0x669A,0xDEF6,0xBCD6,
			0x8420,0x669A,0xDF7A,0xDEF6,0xBCD6,
			0x7BDE,0xDF7A,0x669A,0x7BDE,0xCD5A,
			0x7BDE,0xDF7A,0xDF7A,0x7BDE,0xCD5A
		}/* end */
#endif
	};
	
	if(Num >= MAPCOL_MAX)Num = 0;
	
	for(i=0; i < 4; i++)
	{
		PCG_PAL_Change( 8,  i+1, uRoadCol[Num][i][0]);
		PCG_PAL_Change( 9,  i+1, uRoadCol[Num][i][1]);
		PCG_PAL_Change(10,  i+1, uRoadCol[Num][i][2]);
		PCG_PAL_Change(11,  i+1, uRoadCol[Num][i][3]);
		PCG_PAL_Change(12,  i+1, uRoadCol[Num][i][4]);
	}
	
	for(j=0; j < 16; j++)
	{
		PCG_PAL_Change( j,  14, 0xE81E);
	}
	
	for(j=0; j < 16; j++)
	{
		PCG_PAL_Change( j,  15, 0xE81E);
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
int16_t Road_Map_Draw(uint8_t bMode)
{
	int16_t	ret = 0;
	int16_t	col;
	int16_t			x =0, y = 0;
	uint16_t		pos_x, pos_y;
	static int32_t	xofst, yofst, rad;
	static uint16_t s_uCount = 0u;
	
	if(s_uCount >= g_stRoadInfo.Courselength)
	{
		s_uCount = 0;
		return  -1;
	}
	else if(s_uCount == 0)
	{
		_iocs_window( X_MIN_DRAW, Y_MIN_DRAW, X_MAX_DRAW-1, Y_MAX_DRAW-1);	/* �S�̈揑������OK */
		
		G_PaletteSetZero();					/* 0�ԃp���b�g�ύX */
		
		xofst = Mmul256(256);
		yofst = Mmul256(0) + Mmul256((s_uCount/256) * 64);
		
		rad = 0;
	}
	else
	{
#if 0
		g_stRoadData[s_uCount].bHeight;		/* ���̕W��	(0x80�Z���^�[) */
		g_stRoadData[s_uCount].bWidth;			/* ���̕�	(0x80�Z���^�[) */
		g_stRoadData[s_uCount].bAngle;			/* ���̊p�x	(0x80�Z���^�[) */
		g_stRoadData[s_uCount].bfriction;		/* ���̖��C	(0x80�Z���^�[) */
		g_stRoadData[s_uCount].bPat;			/* ���̎��	 */
		g_stRoadData[s_uCount].bObject;		/* �o���|�C���g�̃I�u�W�F�N�g�̎�� */
		g_stRoadData[s_uCount].bRepeatCount;	/* �J��Ԃ��� */
#endif		
	}
	
	
	col = 0x3;	/* White */
	
	rad += 0x80 - g_stRoadData[s_uCount].bAngle;
	rad %= 360;
	
	x = APL_Cos(rad);
	y = APL_Sin(rad);
	
#if 1
	xofst += x;
	xofst += x;	/* bug */
	yofst += y;
#else
	if(xofst >= 0x20000)
	{
		xofst = Mmul256(0);
		yofst = Mmul256(0) + Mmul256((s_uCount/256) * 64);
	}
	else if(xofst <= -X_OFFSET)
	{
		xofst = Mmul256(256);
		yofst = Mmul256(0) + Mmul256((s_uCount/256) * 64);
	}
	else
	{
		xofst += x;
		yofst += y;
	}
#endif
	pos_x = Mdiv512(xofst);
	pos_y = Mdiv512(yofst);

	_iocs_home(0b0000, X_OFFSET + pos_x - 128, Y_OFFSET + pos_y - 128);

	Draw_Pset( X_OFFSET + pos_x, Y_OFFSET + pos_y,	col);
	
	s_uCount++;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t Set_Arrow_sp(int16_t Delta)
{
	int16_t	ret = 0;
	
	int16_t	x, y, mv;
	int16_t AngleDiff;
	
	/*-----------------------------------------------------------------*/
	ST_PCG	*p_stPCG = NULL;
	p_stPCG = PCG_Get_Info(ROAD_PCG_ARROW);	/* ��� */
	/*-----------------------------------------------------------------*/
	
	/*-----------------------------------------------------------------*/
	x = 120 + SP_X_OFFSET;
	y = g_stRasInfo.mid + SP_Y_OFFSET;
	p_stPCG->x = x;
	p_stPCG->y = y;
	/*-----------------------------------------------------------------*/

	AngleDiff = Delta;
	
	
	if(p_stPCG != NULL)
	{
		x = p_stPCG->x;
		y = p_stPCG->y;
		p_stPCG->dy = 0;
		
		if( (x < 80) || (x > 176) || (bAngleFlag == TRUE))
		{
			x = 120 + SP_X_OFFSET;
		}
		
		if(AngleDiff == 0)
		{
			x = 120 + SP_X_OFFSET;
			y = g_stRasInfo.mid + SP_Y_OFFSET;
			p_stPCG->dx = 0;

			p_stPCG->update	= FALSE;

			if(bAngleFlag == TRUE)
			{
				bAngleFlag = FALSE;
			}
			else
			{
				bAngleFlag = TRUE;
			}
		}
		else
		{
			mv = AngleDiff;
			
			if(AngleDiff < 0)
			{
				if(bAngleFlag == TRUE)
				{
					x = 120 + SP_X_OFFSET;
				}
				bAngleFlag = FALSE;
				
				if(mv == 0)
				{
					mv = 1;
				}
				p_stPCG->pPatCodeTbl[0] &= PCG_CODE_MASK;	/* �p�^�[���ȊO���N���A */
				p_stPCG->pPatCodeTbl[0] |= SetBGcode2(0, 1, 0x07);
			}
			else
			{
				if(bAngleFlag == FALSE)
				{
					x = 120 + SP_X_OFFSET;
				}
				bAngleFlag = TRUE;
				
				if(mv == 0)
				{
					mv = -1;
				}
				p_stPCG->pPatCodeTbl[0] &= PCG_CODE_MASK;	/* �p�^�[���ȊO���N���A */
				p_stPCG->pPatCodeTbl[0] |= SetBGcode2(0, 0, 0x07);
			}
			p_stPCG->dx = mv;
			
			p_stPCG->update	= TRUE;
		}
		p_stPCG->x = x;
		p_stPCG->y = y;
	}
	
	return ret;
}

#endif	/* RASTER_C */

