#ifndef	RASTER_C
#define	RASTER_C

#include <iocslib.h>
#include <stdio.h>
#include <stdlib.h>

#include "inc/usr_macro.h"
#include "OverKata.h"

#include "Raster.h"

#include "APL_Math.h"
#include "Course_Obj.h"
#include "Draw.h"
#include "EnemyCAR.h"
#include "Graphic.h"
#include "MFP.h"
#include "MyCar.h"
#include "Output_Text.h"
#include "Text.h"

/* �ϐ� */
static US	g_uRoadAnime;
static US	g_uRoadAnime_old;
static US	g_uRoadAnimeBase;
static US	g_uRoadAnimeBase_old;
static US	g_uRoadAnimeCount;

static US	g_uRoadCycleCount;
static US	g_uCounter;

/* �\���̒�` */
static ST_RAS_INFO	g_stRasInfo = {0};

static ST_ROAD_INFO	g_stRoadInfo = {0};
static ST_ROADDATA	g_stRoadData[ROADDATA_MAX] = {0};

/* �֐��̃v���g�^�C�v�錾 */
SS	GetRasterInfo(ST_RAS_INFO *);
SS	SetRasterInfo(ST_RAS_INFO);
void	Raster_Init(void);
void	Raster_Main(UC);
static UC Raster_Calc_H(SS *, SS, SS);
static UC Raster_Calc_V(SS *, SS, SS);

SS	GetRasterPos(US *, US *, US);	/* ���X�^�[�������ʂ��擾 */

void Road_Init(US);
static void Get_Road_Pat(US *, UC);
static void Set_Road_Pat_offset(SS);
static SS Road_Pat_Update(US);
static void Road_Pat_Base_Update(void);
static void Road_Pat_Start(void);
static void Road_Pat_Stop(void);
static SS	Set_Road_Height(void);
static SS	Set_Road_Slope(void);
static SS	Set_Road_Angle(void);
static SS	Set_Road_Distance(void);
static SS	Set_Road_Object(void);
static void	Road_Pat_Main(void);
SS	GetRoadCycleCount(US *);
UL	GetRoadDataAddr(void);
SS	SetHorizon(UC);

/* �֐� */

SS GetRasterInfo(ST_RAS_INFO *stDat)
{
	SS	ret = 0;
	
	*stDat = g_stRasInfo;
	
	return ret;
}

SS SetRasterInfo(ST_RAS_INFO stDat)
{
	SS	ret = 0;
	
	g_stRasInfo = stDat;
	
	return ret;
}

void Raster_Init(void)
{
	memset(&g_stRasInfo, 0, sizeof(ST_RAS_INFO));
}

void Raster_Main(UC bMode)
{
	SS	view_offset_x, view_offset_y;
	SS	hide_offset_x, hide_offset_y;
	SS	BG_offset_x, BG_offset_y;
	SS	BG_under;
	
	US	uRas_y;
	US	uRas_st, uRas_mid, uRas_ed, uRas_size;

	ST_CRT			stCRT = {0};
	ST_RASTER_INT	stRasterInt[RASTER_MAX] = {0};

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	UC	bDebugMode;
	US	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
	/* ���[�h�ؑւɂ��ݒ�l�̕ύX */
	GetCRT(&stCRT, bMode);
	view_offset_x	= stCRT.view_offset_x;
	view_offset_y	= stCRT.view_offset_y;
	hide_offset_x	= stCRT.hide_offset_x;
	hide_offset_y	= stCRT.hide_offset_y;
	BG_offset_x		= stCRT.BG_offset_x;
	BG_offset_y		= stCRT.BG_offset_y;
	BG_under		= stCRT.BG_under;

	/* ���[�h�p�^�[�� */
	Road_Pat_Main();

	/* �������̐ݒ� */
	SetHorizon(bMode);

	/* ������ */
	uRas_st = 0;
	uRas_mid = 0;
	uRas_ed = 0;
	uRas_size = 0xFFFF;

	/* ���X�^�[�ʒu */
	if(bMode == 0)		/* TPS */
	{
		uRas_ed = Mmin(RASTER_MAX, V_SYNC_MAX);	/* ���X�^�[�I���ʒu */
	}
	else
	{
		uRas_ed = Mmin(g_stRoadInfo.Horizon + RASTER_MIN + ROAD_SIZE, V_SYNC_MAX);	/* ���X�^�[�I���ʒu */
		//uRas_ed = Mmin(RASTER_MAX, V_SYNC_MAX);	/* ���X�^�[�I���ʒu */
	}
	
	uRas_st = Mmax(Mmin(g_stRoadInfo.Horizon + RASTER_MIN, RASTER_MAX), RASTER_MIN);	/* ���X�^�[�J�n�ʒu */
	uRas_st += (uRas_st % 2);	/* �����ɂ���(for����RASTER_NEXT���C���N�������g����̂�) */
//	uRas_ed -= (uRas_ed % 2);	/* �����ɂ��� */
	uRas_size = uRas_ed - uRas_st;			/* ���X�^�[�͈̔� */
	uRas_mid = Mmin((uRas_st + g_stRoadInfo.distance), uRas_ed - 16);	/* �������̈ʒu���ω����Ȃ��|�C���g *//* �n�C�E�F�C�X�^�[�Ȃ� 64 = ROAD_SIZE*2/3 */
	
	g_stRasInfo.st			= uRas_st;
	g_stRasInfo.mid			= uRas_mid;
	g_stRasInfo.ed			= uRas_ed;
	g_stRasInfo.size		= uRas_size;
	g_stRasInfo.horizon		= g_stRoadInfo.Horizon;
	SetRasterInfo(g_stRasInfo);
	
	for(uRas_y=uRas_st; uRas_y < uRas_ed; uRas_y+=RASTER_NEXT)
	{
		SS	num;
		SS	rev;
		SS	ras_cal_x=0;
		SS	ras_cal_y=0;
		US	ras_pat=0;
		UC	bRoad_pat;
		
		num = uRas_y - uRas_st;			/* 0 -> uRas_size */
		rev = uRas_ed - uRas_y;			/* uRas_size -> 0 */
		
		/* X���W */
		Raster_Calc_H( &ras_cal_x, num, rev );

		/* Y���W */
		bRoad_pat = Raster_Calc_V( &ras_cal_y, num, rev );
		
		/* ���[�h�p�^�[�� */
		Get_Road_Pat(&ras_pat, bRoad_pat);	/* ���[�h�̃p�^�[�������擾 */
		Set_Road_Pat_offset(num);			/* ���[�h�p�^�[��臒l�̍X�V */

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		if(bDebugMode == TRUE)	/* �f�o�b�O���[�h */
		{
			SS	col;
			
			/* x */
			Draw_Pset(	view_offset_x + (WIDTH >> 1) - ras_cal_x,
						view_offset_y + g_stRoadInfo.Horizon + num,
						0xB4);
			/* ������ */
			Draw_Line(	uRas_st + view_offset_x,
						(g_stRoadInfo.Horizon+ view_offset_y)+1,
						uRas_ed + view_offset_x, 
						(g_stRoadInfo.Horizon+ view_offset_y)+1,
						0x35,
						0xFFFF);

			/* y */
			col = 0xFF;
			Draw_Pset(	uRas_y + view_offset_x,
						(g_stRoadInfo.Horizon + RASTER_MIN + view_offset_y) - (ras_cal_y-g_stRoadInfo.Horizon_offset),
						col);
		}
#endif
		stRasterInt[uRas_y].x = (US)ras_cal_x;
		stRasterInt[uRas_y].y = (US)ras_cal_y;
		stRasterInt[uRas_y].pat = ras_pat;
	}
	
	stRasterInt[0].y = uRas_st;		/* ���荞�݈ʒu�̐ݒ� */
	stRasterInt[1].y = BG_offset_y;	/* ��̃R���g���[�� */
	
	SetRasterIntData(stRasterInt, sizeof(ST_RASTER_INT) * RASTER_MAX);
}

static UC Raster_Calc_H(SS *x, SS Num, SS RevNum)
{
	UC bRet = FALSE;

	if(Num == 0u)
	{
		*x = 0;	/* �ŏ� */
	}
#if 0
	else if( uRas_y < uRas_mid )				/* �������̈ʒu���ω� *//* 64 = ROAD_SIZE*2/3 */
	{
		ras_cal_x = ((num * stMyCar.Steering) >> 4) + (g_stRoadInfo.angle * (uRas_mid - uRas_st) / num);
	}
#endif
	else
	{
		ST_CARDATA stMyCar = {0};

		GetMyCar(&stMyCar);			/* ���Ԃ̏����擾 */

		*x = Mmax( Mmin( (((Num * stMyCar.Steering) >> 4) + ((g_stRoadInfo.angle * g_stRasInfo.size) / Num)), 256), -256);
	}
	
	if(*x < 0)
	{
		*x = 512 + *x;
	}
	
	return bRet;
}

static UC Raster_Calc_V(SS *y, SS Num, SS RevNum)
{
	UC bRet = FALSE;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	UC	bDebugMode;
	US	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif

	if(Num == 0u)	/* �����0or512�łȂ��Ƌ�ɓ����o�Ă��� */	/* �Ղ̂Ђ� ����̃A�h�o�C�X�ӏ� */
	{
		*y = 0;
		bRet = TRUE;
		Road_Pat_Start();
	}
#if 1
	else
	{
		SS	Road_strch;

		Road_strch = Num;
		/* �������̃|�C���g�ł���BG�̍��W����ɃR���g���[������ */
		/* �Ⴆ�΁A�������̈ʒu�ɕ\��������BG�̉摜���U�S�������� */
		/* �������X�^���荞�ݗp��Y���W�͂U�S���ϓ����Ȃ���΁A���H�͐����ȓ���`�悷�� */
		/* �������X�^���荞�ݗp��Y���W�͂U�S���猸�Z����΁A���H�͓�����ێ����� */
		/* �������X�^���荞�ݗp��Y���W�͂U�S������Z����΁A���H�͓�����ێ����� */
		*y = Mmax(Mmin(g_stRoadInfo.Horizon_offset + ((SS)uDebugNum - 0x80), 72), 32);
		//*y = Mmax(Mmin(g_stRoadInfo.Horizon_offset + Road_strch + ((SS)uDebugNum - 0x80), 72), 0);
	}
#else		/* �n�C�E�F�C�X�^�[�d�l */
	else if( uRas_y < uRas_mid )				/* �������̈ʒu���ω� *//* 64 = ROAD_SIZE*2/3 */
	{
		SS	Road_strch;
		SS	offset_size;
		SS	cal_tan = 0;

//			offset_size = uRas_mid - uRas_y;	/* uRas_size -> 0 */
		offset_size = (0 - (g_stRoadInfo.height / Num));
		Road_strch = offset_size;
//			Road_strch = (offset_size * cal_tan) >> 8;	/* ��������Ӂ~tan�� */
//			Road_strch = (offset_size * APL_Cos(Num)) >> 8;
//			Road_strch = (offset_size * APL_Sin(Num)) >> 8;
		*y = g_stRoadInfo.Horizon_offset + Road_strch + ((SS)uDebugNum - 0x80);

	}
	else if( uRas_y >= (uRas_ed - 16))		/* �����̂͂ݏo��������⊮ */
	{
		*y = BG_under;

	}
	else	/* ���R */
	{
		if(g_stRoadInfo.height >= 0)	/* ���Ror���� */
		{
			*y = g_stRoadInfo.Horizon_offset - (0 - (g_stRoadInfo.height / Num));
		}
		else	/* �o�� */
		{
			*y = g_stRoadInfo.Horizon_offset - (0 - (g_stRoadInfo.height / Num));
		}

	}
#endif
	
	return bRet;
}

void Road_Init(US uCourseNum)
{
	g_uRoadAnimeBase = 3;
	g_uRoadAnime = 0;
	Road_Pat_Start();
	
	g_uCounter = 0u;
	g_uRoadCycleCount = 0u;

	g_stRoadInfo.Horizon		= 0x00;	/* �����ʒu */
	g_stRoadInfo.Horizon_tmp	= 0x00;	/* �������ʒu */
	g_stRoadInfo.Horizon_offset	= 0x00;	/* �����ʒu�̃I�t�Z�b�g */
	g_stRoadInfo.offset_x		= 0x00;	/*  */
	g_stRoadInfo.offset_y		= 0x00;	/*  */
	g_stRoadInfo.offset_val		= 0x00;	/*  */
	g_stRoadInfo.height			= 0x80;	/*  */
	g_stRoadInfo.slope			= 0x80;	/*  */
	g_stRoadInfo.angle			= 0x80;	/*  */
	g_stRoadInfo.distance		= 0x00;	/*  */
	g_stRoadInfo.object			= 0x00;	/*  */
	
	/* �R�[�X�f�[�^�̓ǂݍ��� */
	Load_Course_Data(uCourseNum);

	Set_Road_Angle();		/* �R�[�X�̋Ȃ��� */
	Set_Road_Height();		/* �R�[�X�̕W�� */
	Set_Road_Slope();		/* �R�[�X�̌��ݒn����W���܂ł̌X�� */
	Set_Road_Distance();	/* �������������ݒn�܂ł̋��� */
	Set_Road_Object();		/* ���C�o���Ԃ̏o�� */
}

static void Get_Road_Pat(US *p_uPat, UC bFlag)
{
#if 1
	US	uBG_pat[4][4] = {288,   0,  96, 192, 
		 				 192, 288,   0,  96, 
		 				  96, 192, 288,   0, 
		 				   0,  96, 192, 288} ;
	if(bFlag == TRUE)
	{
		*p_uPat = uBG_pat[3][0];
	}
	else
	{
		*p_uPat = uBG_pat[g_uRoadAnimeBase][g_uRoadAnime];
	}
#else
	*p_uPat = 0;
#endif
}

static void Set_Road_Pat_offset(SS Num)
{
	if( (Num == 0) || (g_stRoadInfo.offset_val == 0) )
	{
		g_stRoadInfo.offset_y = 0;
		g_stRoadInfo.offset_val = 0;
	}
	else
	{
		g_stRoadInfo.offset_y = (Num * g_stRoadInfo.offset_val) / (g_stRoadInfo.offset_val + 96);
	}
	if(Road_Pat_Update(g_stRoadInfo.offset_y) != 0u)
	{
		g_stRoadInfo.offset_val++;
	}
}

static SS Road_Pat_Update(US uThreshold)
{
	SS	ret = 0;
	
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

static void Road_Pat_Start(void)
{
	g_uRoadAnime_old = g_uRoadAnime;
	g_uRoadAnimeBase_old = g_uRoadAnimeBase;
}

static void Road_Pat_Stop(void)
{
	g_uRoadAnime = g_uRoadAnime_old;
	g_uRoadAnimeBase = g_uRoadAnimeBase_old;
	g_uRoadAnimeCount = 0;
}

static SS	Set_Road_Height(void)
{
	SS	ret = 0;
	SS	r_height;
	SS	road_height;
	static SS	road_height_old;
	
	road_height_old = g_stRoadInfo.height;	/* �O��l�X�V */
	
	/* ���� */
#if 1
	r_height = 0;
#else
	if((input & KEY_b_RLUP)!=0u)	r_height += 1;	/* �� */
	if((input & KEY_b_RLDN)!=0u)	r_height -= 1;	/* �� */
	r_height = Mmax(Mmin(r_height, 31), -32);
#endif
	
	/* �R�[�X�f�[�^�ǂݍ��� */
	road_height = (SS)(0x80 - g_stRoadData[g_uCounter].bHeight);	/* ���̕W��	(0x80�Z���^�[) */
	road_height = Mmax(Mmin(road_height, 31), -32);
	//road_height = r_height;	/* �f�o�b�O���� */
	g_stRoadInfo.height = road_height;		/*  */

	// ���፷����X�����o��(-45�`+45) rad = APL_Atan2( road_slope_old - road_slope,  );
	if(road_height != road_height_old)
	{
		if(road_height > road_height_old)
		{
			/* �ێ� */
		}
		else
		{
			/* �ێ� */
		}
		ret = 1;
	}
	else{
		/* �O��l�ێ� */
	}
	
	return ret;
}

static SS	Set_Road_Slope(void)
{
	SS	ret = 0;
	SS	cal_tan = 0;
	SS	cal_cos = 0;
	SS	road_slope;
	static SS	road_slope_old;

	road_slope_old = g_stRoadInfo.slope;		/* �O��l�X�V */
//	road_slope = road_height * -1;
	
#if 1
	if(road_slope == 0)
	{
		cal_tan = 0;
		cal_cos = 0;
	}
	else
	{
		if(road_slope != road_slope_old)
		{
			cal_tan = APL_Tan256(road_slope);	/* tan�� */
			cal_cos = APL_Cos(cal_tan);			/* cos�� */
		}
	}
#else
	/* ��ӂƍ�������p�x�����߂āA�p�x����Y���W�����Z���Ă�����*/
	/* �p�x���Z�͕s�v�Ƃ̂��ƁB�P���ɋ����Ɗp�x���獂�����Z�o�ł��� @runshiwa������w�E */
	if(vy == 0)
	{
		rad = 0.0;
	}
	else
	{
		rad = APL_Atan2( vy, uRas_mid );
	}
	cal_tan = APL_Tan256((SS)rad);
#endif
	g_stRoadInfo.slope		= road_slope;		/*  */
	
	return ret;
}

static SS	Set_Road_Angle(void)
{
	SS	ret = 0;
	SS	road_angle;
	static SS	road_angle_old;
	
	/* �R�[�i�[�̕\�� */
#if 0
	/* �f�o�b�O�R�[�i�[ */
	/* �����v���� */
	if( ((uFreeRunCount % (RD[uFreeRunCount & 0x03FFu])) == 0)
		&& ((uFreeRunCount % 3) == 0)
		&& (speed != 0u) )
	{
		if(bRoad_flag == 0){
			road_angle += 1;
			if(road_angle > 12)bRoad_flag = 1;
		}
		else{
			road_angle -= 1;
			if(road_angle < -12)bRoad_flag = 0;
		}
	}
#endif
	/* �p�x�Z�o */
	road_angle_old = g_stRoadInfo.angle;
#if 1
	road_angle = (SS)g_stRoadData[g_uCounter].bAngle - 0x80;		/* ���̊p�x	(0x80�Z���^�[) */
#else
	road_angle = 0;	/* �f�o�b�O���� */
#endif
	road_angle = Mmax(Mmin(road_angle, 12), -12);
	g_stRoadInfo.angle = road_angle;		/*  */
	
	return ret;
}

static	SS	Set_Road_Distance(void)
{
	SS	ret = 0;
	SS	road_distance;
	static SS road_distance_old;
	
	road_distance_old = g_stRoadInfo.distance;	/* �O��l�X�V */
#if 1
	road_distance = g_stRoadInfo.Horizon_offset;
#else
	road_distance = 0;
#endif
	g_stRoadInfo.distance	= road_distance;	/*  */
	
	return ret;
}

static SS	Set_Road_Object(void)
{
	SS	ret = 0;
	SS	road_object = 0;
	
//	road_object = g_stRoadData[g_uCounter].bObject;	/* �o���|�C���g�̃I�u�W�F�N�g�̎�� */
	if(road_object != 0u)
	{
		SetAlive_EnemyCAR();	/* ���C�o���Ԃ��o�������� */
	}
	g_stRoadInfo.object		= road_object;		/*  */
	
	return ret;
}

static void Road_Pat_Main(void)
{
	static UI	unRoadAnimeTime = 0xFFFFFFFFu;
	UI	time_st = 0u;
	SS	speed;

	GetStartTime(&time_st);		/* �J�n���Ԃ��擾 */
	GetMyCarSpeed(&speed);		/* �ԑ��𓾂� */
	
	if(speed == 0u)	/* �ԑ�0km/h */
	{
		unRoadAnimeTime = time_st;	/* �����X�V */

		Road_Pat_Stop();	/* ���̃A�j����~ */
	}
	else if( (time_st - unRoadAnimeTime) < (362 / speed) )	/* �X�V�������� */
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
		
		g_uCounter++;			/* �R�[�X�f�[�^�̃J�E���^�X�V */
		if(g_uCounter >= ROADDATA_MAX)
		{
			g_uCounter = 0u;
		}

		g_uRoadCycleCount++;	/* �R�[�X�X�V�t���[�����J�E���^ */
	}
	
#if 0
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	Message_Num(&g_uCounter,			12,  8, 2, MONI_Type_US, "%4d");
#endif
#endif
}

SS	GetRoadCycleCount(US *p_uCount)
{
	SS	ret = 0;
	
	*p_uCount = g_uRoadCycleCount;
	
	return ret;
}

UL GetRoadDataAddr(void)
{
	return (UL)&g_stRoadData[0];
}

SS	SetHorizon(UC bMode)
{
	SS	ret = 0;
	SS	Horizon=0, Horizon_tmp, Horizon_offset;
	SS	Height;

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
			Horizon_tmp = Y_HORIZON_0;	/* �������ʒu */
			break;
		}
		case 1:		/* FPS */
		default:	/* FPS */
		{
			Horizon_tmp = Y_HORIZON_1;	/* �������ʒu */
			break;
		}
	}
	Height = g_stRoadInfo.height;	/* �����������ύX */
	Horizon = Mmax(Mmin(Horizon_tmp + Height, ROAD_SIZE), RASTER_MIN);	/* �����ʒu����i�N���b�v�j */

	Horizon_offset = ROAD_POINT - Horizon_tmp - RASTER_NEXT;	/* ���ۂ̃f�[�^�ƕ\���ʒu�Ƃ̍��� */
	
	g_stRoadInfo.Horizon		= Horizon;
	g_stRoadInfo.Horizon_tmp	= Horizon_tmp;
	g_stRoadInfo.Horizon_offset	= Horizon_offset;
	
	return ret;
}

#endif	/* RASTER_C */

