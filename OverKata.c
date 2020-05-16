#ifndef	OVERKATA_C
#define	OVERKATA_C

#include <iocslib.h>
#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>
#include <io.h>
#include <math.h>

#include "inc/usr_macro.h"
#include "inc/apicglib.h"
#include "inc/ZMUSIC.H"

#include "OverKata.h"
#include "APL_Math.h"
#include "Draw.h"
#include "MFP.h"
#include "MyCar.h"
#include "Input.h"
#include "Output_Text.h"

/* �O���[�o���ϐ� */
SS moni = 0;
SS moni_MAX = 0;
SS speed = 0;
UC g_mode = 0;
ST_ROADDATA	stRoadData[256] = {0};
ST_CARDATA	stMyCar = {0};

/* �֐��̃v���g�^�C�v�錾 */
void main(void);

/* �֐� */
void main(void)
{
	static US uScoreMax = 10000;
	UC	road_flag = 0;
	UC	updown_flag = 0;
	US	x,y;
	US	ras_tmp[256] = {0};
	US	ras_debug[256] = {0};
	US	ras_tmp_final;
	US	pal_tmp[256] = {0};
	US	usFreeRunCount=0;
	US	uCountNum;
	US	ras_st, ras_ed, ras_size;
	SS	i;
	SS	loop;
	SS	RD[1024] = {0};
	SS	nHorizon, nHorizon_tmp, nHorizon_ras, nHorizon_offset;
	SS	vx, vy;
	SS	road;
	SS	road_offset_x, road_offset_y, road_offset_val;
	SS	view_offset_x, view_offset_y;
	US	uRoad_rate, uRoad_strch;
	SS	input;
	UI	start_time;
	UI	time_cal = 0;
	UI	time_cal_PH = 0;
	SI	superchk;
	SI	crtmod;
	UC	mode_flag = 0;
	UC	bKeyUP_flag = FALSE;
	UC	bKeyDOWN_flag = FALSE;
	UC	bUpdate;
	UC	bDebugMode;
	UC	bDebugMode_flag;
	US	uDebugNum;
	UC	bCounter;
	
	UI	unZmusicVer;
	
	UI	unExplosion_time;
	UC	bExplosion;
	
	UI	un8ms_time;
	UC	b8ms_time;
	UI	un16ms_time;
	UC	b16ms_time;
	UI	un32ms_time;
	UC	b32ms_time;
	UI	un96ms_time;
	UC	b96ms_time;
	
	SS	rpm, Vs, speed_min;
	SS	ShiftPos;
	UC	bShiftPosFlag;
	US	uRoadAnime, uRoadAnime_old, uRoadAnimeBase, uRoadAnimeBase_old;
	US	uRoadAnimeCount;
	UI	unRoadAnimeTime;
	SS	nVibration;
	FLOAT	rad;
	US	cal_tan;
	volatile US *CRTC_R21 = (US *)0xE8002Au;	/* �e�L�X�g�E�A�N�Z�X�E�Z�b�g�A�N���A�[P.S */
	volatile US *CRTC_480 = (US *)0xE80480u;	/* CRTC����|�[�g */

	/* �f�o�b�O�R�[�i�[ */
#if 0
	/* �����R�ɃR�[�h�������Ă� */
	rad = APL_Atan2(1,1);
	printf("%f\n", rad);
	/* �����R�ɃR�[�h�������Ă� */
	loop = 1;
	do
	{
		if( ( BITSNS( 0 ) & 0x02 ) != 0 ) loop = 0;	/* �d�r�b�|�[�Y */
		if(loop == 0)break;
	}
	while( loop );
	
	exit(0);
#endif
	/* �T�E���h�풓�m�F */
	unZmusicVer = zm_ver();
	if(unZmusicVer == 0)	/* 0:�풓�i�V */
	{
		puts("Z-MUSIC Ver2���풓���Ă��������B");
		exit(0);
	}
	if((unZmusicVer&0xF000u)>>12 != 2u)	/* Ver2.0x���� */
	{
		puts("Z-MUSIC Ver2���풓���Ă��������B");
		exit(0);
	}
	if((unZmusicVer&0xFFFF000u)>>16 == 0u)	/* PCM8���� */
	{
		puts("PCM8A���풓���Ă��������B");
		exit(0);
	}
	
	/* �X�[�p�[�o�C�U�[���[�h�J�n */
	/*�c�n�r�̃X�[�p�[�o�C�U�[���[�h�J�n*/
	superchk = SUPER(0);
	if( superchk < 0 ) {
		puts("���łɃX�[�p�[�o�C�U�[���[�h");
	} else {
		puts("�X�[�p�[�o�C�U�[���[�h�J�n");
	}
	
	crtmod = CRTMOD(-1);	/* ���݂̃��[�h��Ԃ� */
	
	/* �T�E���h������ */
	m_init();		/* ������ */
	m_ch("fm");		/* FM */
	m_stop(0,0,0,0,0,0,0,0,0,0);	/* ���y��~ */
	zmd_play("data\\music\\X68K.ZMD");	/* ���[�f�B���OBGM */

	/* �R�[�X�f�[�^�̓ǂݍ��� */
	File_Load_Course_CSV("data/map/course01.csv", &stRoadData[0], &x, &y);
	
	/* �O���t�B�b�N�\�� */
	G_INIT();		/*��ʂ̏����ݒ�*/
	APICG_DataLoad("data/cg/Over_A.pic"	, X_OFFSET, 		Y_OFFSET - 32,	0);	/* FPS */
	APICG_DataLoad("data/cg/Over_B.pic"	, X_OFFSET + 96,			  165,	0);	/* TPS */
	G_MyCar();		/* ���Ԃ̕\�� */
	APICG_DataLoad("data/cg/Over_C.pic"	, 0,							0,	1);	/* ���C�o���� */
	APICG_DataLoad("data/cg/Over_D.pic"	, X_OFFSET - 32,	Y_OFFSET +  4,	1);	/* �w�i */
	G_Background();	/* �w�i�̕\�� */
	
	/* �X�v���C�g�^�a�f�\�� */
	PCG_INIT();							/* �X�v���C�g�^�a�f�̏����� */
	PCG_SP_dataload("data/sp/BG.SP");	/* �X�v���C�g�̃f�[�^�ǂݍ��� */
	PCG_PAL_dataload("data/sp/BG.PAL");	/* �X�v���C�g�̃p���b�g�f�[�^�ǂݍ��� */
	BG_TEXT_SET("data/map/map.csv");	/* �}�b�v�f�[�^�ɂ��a�f�̔z�u */
	
	/* �e�L�X�g�\�� */
	T_INIT();		/* �e�L�X�g�u�q�`�l������ */
	T_TopScore();	/* TOP */
	T_Time();		/* TIME */
	T_Score();		/* SCORE */
	T_Speed();		/* SPEED */
	T_Gear();		/* GEAR */
	BG_TextPut("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 232);
	for(i=0; i < 10; i++)
	{
		x = 0;
		y = 240;
		BG_PutToText(   0x80+ (i<<1) + 0, x + BG_WIDTH * i,	y,				BG_Normal, TRUE);	/* ������i�㑤�j*/
		BG_PutToText(   0x80+ (i<<1) + 1, x + BG_WIDTH * i,	y+BG_HEIGHT,	BG_Normal, TRUE);	/* ������i�����j*/
	}
	
	/* �ϐ��̏����� */
	speed = 0;
	road_offset_x = 0;
	road_offset_y = 0;
	road_offset_val = 0;
	g_mode = 1;
	ShiftPos = 1;		/* 1�� */
	rpm = 0;
	Vs = 0;
	speed = 0;
	road = 0;
	loop = 1;
	vx = vy =0;
	usFreeRunCount = 0;
	uCountNum = 0;
	bUpdate = FALSE;
	uRoadAnimeBase = 0;
	uRoadAnimeBase_old = 0;
	uRoadAnime = 0;
	uRoadAnime_old = 0;
	uRoadAnimeCount = 0;
	nVibration = 0;
	bDebugMode = TRUE;
	uDebugNum = 0x80;
	bCounter = 0;

	/* ���� */
	{
		/* �����̏����� */
		SS a,b,c,d;
		a = 0;
		b = 0;
		c = 0;
		for(a=0; a < 1024; a++)
		{
			RD[a] = 0;
		}

		srandom(TIMEGET());
		for(a=0; a < 16; a++)
		{
			d = a * 64;
			c = (rand() % 5) - 2;
			for(b = d; b < d + 64; b++)
			{
				RD[b] = c;
			}
		}
	}
	
	/* ���y */
	m_stop(0,0,0,0,0,0,0,0,0,0);	/* ���y��~ */
//	zmd_play("data\\music\\PT034MK.ZMD");	/* BGM */
//	zmd_play("data\\music\\kyaraban.zmd");	/* BGM */
//	zmd_play("data\\music\\KATAYS3X.zmd");	/* BGM */

	/* MFP */
	Init_MFP();						/* MFP�֘A�̏����� */
	TIMERDST(Timer_D_Func, 7, 20);	/* Timer-D�Z�b�g */	/* 50us(7) x 20cnt = 1ms */
	SetNowTime(0);					/* ���Ԃ̏����� */
	GetNowTime(&start_time);		/* Time Count�p */
	VDISPST(Vsync_Func, 0, 1);		/* V-Sync���荞�� �A�� */
	
	do
	{
		US uCount = 0;
		UI time, time_old, time_now;
		
		GetNowTime(&time_old);
		
		/* ���Ԕ��� */
		if( (time_old - un8ms_time) > 8)	/* 8ms���� */
		{
			un8ms_time = time_old;
			b8ms_time = TRUE;
		}
		else
		{
			b8ms_time = FALSE;
		}
		
		if( (time_old - un16ms_time) > 16)	/* 16ms���� */
		{
			un16ms_time = time_old;
			b16ms_time = TRUE;
		}
		else
		{
			b16ms_time = FALSE;
		}

		if( (time_old - un32ms_time) > 32)	/* 32ms���� */
		{
			un32ms_time = time_old;
			b32ms_time = TRUE;
		}
		else
		{
			b32ms_time = FALSE;
		}

		if( (time_old - un96ms_time) > 96)	/* 96ms���� */
		{
			un96ms_time = time_old;
			b96ms_time = TRUE;
		}
		else
		{
			b96ms_time = FALSE;
		}
		/* ���͏��� */
		input = get_key(1);	/* �L�[�{�[�h���W���C�X�e�B�b�N���� */
		if((input & KEY_b_Q   ) != 0u) loop = 0;	/* �p�ŏI�� */
		if((input & KEY_b_ESC ) != 0u) loop = 0;	/* �d�r�b�|�[�Y */
		if(loop == 0)break;
		
		if((input & KEY_b_M   ) != 0u)	/* �l�Ń��[�h�ؑ� */
		{
			if( mode_flag == FALSE )
			{
				mode_flag = TRUE;
				if(g_mode == 0u)g_mode = 1u;
				else			g_mode = 0u;
			}
		}
		else
		{
			mode_flag = FALSE;
		}
		
		if((input & KEY_b_SP   ) != 0u)	/* �X�y�[�X�Ńf�o�b�OON/OFF */
		{
			if( bDebugMode_flag == FALSE )
			{
				bDebugMode_flag = TRUE;
				if(bDebugMode == FALSE)	bDebugMode = TRUE;
				else					bDebugMode = FALSE;
			}
		}
		else
		{
			bDebugMode_flag = FALSE;
		}
		
		if(b96ms_time == TRUE)
		{
			/* ���[�h�ؑւɂ��ݒ�l�̕ύX */
			switch(g_mode)
			{
				case 0:		/* Screen 0(TPS) */
				{
					view_offset_x = X_OFFSET;
					view_offset_y = Y_MIN_DRAW;
					break;
				}
				case 1:		/* Screen 0(FPS) */
				{
					view_offset_x = X_OFFSET;
					view_offset_y = Y_OFFSET;
					break;
				}
				default:
				{
					view_offset_x = 0;
					view_offset_y = 0;
					break;
				}
			}
		}
#if 1
		/* �R�[�X�f�[�^�ǂݍ��� */
		vy = (SS)stRoadData[bCounter].bHeight - 0x80;
#else
		/* �L�[���� */
		if((input & KEY_UPPER) != 0u)
		{
			if(bKeyUP_flag == TRUE)
			{
				vy += 1;
				bKeyUP_flag = FALSE;
			}
		}
		else
		{
			bKeyUP_flag = TRUE;
		}
		if((input & KEY_LOWER) != 0u)
		{
			if(bKeyDOWN_flag == TRUE)
			{
				vy -= 1;
				bKeyDOWN_flag = FALSE;
			}
		}
		else
		{
			bKeyDOWN_flag = TRUE;
		}

		/* �����v���� */
		if( ((usFreeRunCount % (RD[usFreeRunCount & 0x03FFu])) == 0)
			&& ((usFreeRunCount % 5) == 0)
			&& (speed != 0u) )
		{
			if(updown_flag == 0){
				vy += 1;
				if(vy > 24)
				{
					updown_flag = 1;
				}
			}
			else{
				vy -= 1;
				if(vy < -28)
				{
					updown_flag = 0;
				}
			}
		}

#endif
		vy = Mmax(Mmin(vy, 24), -28);

		/* ���Ԃ̏����擾 */
		GetMyCarInfo(&stMyCar, input);
		rpm			= stMyCar.uEngineRPM;
		Vs			= stMyCar.VehicleSpeed;
		ShiftPos	= stMyCar.ubShiftPos;
		vx			= stMyCar.Steering;
		
		if( (stMyCar.ubBrakeLights == TRUE)		/* �u���[�L�����vON */
		||  (stMyCar.ubShiftPos == 0)		)	/* �j���[�g���� */
		{
			speed_min = 0;
		}
		else
		{
			speed_min = 1;
		}

		/* �ԑ��i�Q�[�����j */
		speed = Vs / 10;
		speed = Mmax(Mmin(speed, 31), speed_min);
		
		/* �R�[�i�[�̕\�� */
#if 1
		/* �R�[�X�f�[�^�ǂݍ��� */
		road = (SS)stRoadData[bCounter].bAngle - 0x80;
#else
		/* �����v���� */
		if( ((usFreeRunCount % (RD[usFreeRunCount & 0x03FFu])) == 0)
			&& ((usFreeRunCount % 3) == 0)
			&& (speed != 0u) )
		{
			if(road_flag == 0){
				road += 1;
				if(road > 12)road_flag = 1;
			}
			else{
				road -= 1;
				if(road < -12)road_flag = 0;
			}
		}

		/* �Œ� */
		road = 0;
#endif
		road = Mmax(Mmin(road, 12), -12);

#if 1
//		if(b8ms_time == TRUE)
		{
//			memset(&pal_tmp[0], 0, sizeof(pal_tmp));
//			memset(&ras_tmp[0], 0, sizeof(ras_tmp));

			/* ������ */
			/* ���X�^�[���荞�ݏ����̊J�n�E�I���ʒu */
			switch(g_mode)
			{
				case 0:		/* TPS */
				{
					nHorizon_tmp = Y_HORIZON_0;		/* �������ʒu */
					nHorizon = Mmax(Mmin(nHorizon_tmp + vy, Y_HORIZON_0), RASTER_MIN);	/* �����ʒu */
					ras_ed = Mmin(V_SYNC_MAX, RASTER_MAX);	/* ���X�^�[�I���ʒu */
					break;
				}
				case 1:		/* FPS */
				{
					nHorizon_tmp = Y_HORIZON_1;		/* �������ʒu */
					nHorizon = Mmax(Mmin(nHorizon_tmp + vy, Y_HORIZON_1), RASTER_MIN);	/* �����ʒu */
					ras_ed = Mmin(Y_HORIZON_1 + RASTER_MIN + ROAD_SIZE, V_SYNC_MAX);	/* ���X�^�[�I���ʒu */
					break;
				}
				default:	/* FPS */
				{
					nHorizon_tmp = Y_HORIZON_1;
					ras_ed = Mmin(V_SYNC_MAX, RASTER_MAX);	/* ���X�^�[�I���ʒu */
					break;
				}
			}
			nHorizon_offset = ROAD_POINT - nHorizon_tmp;	/* ���ۂ̃f�[�^�ƕ\���ʒu�Ƃ̍��� */
			ras_st = Mmax(Mmin(nHorizon + RASTER_MIN, RASTER_MAX), RASTER_MIN);	/* ���X�^�[�J�n�ʒu */
			ras_size = ras_ed - ras_st;			/* ���X�^�[�͈̔� */
			pal_tmp[0] = ras_st;				/* ���荞�݈ʒu�̐ݒ� */
			
			uRoad_rate = (ras_size << 12) / ROAD_SIZE;
			uRoad_strch = 0;
			uCount = 0;
			
			/* �p�x�Z�o */
			if(Mmax(ras_size-ROAD_SIZE, 0) == 0)
			{
				rad = 0.0;
			}
			else
			{
				rad = APL_Atan2( ROAD_SIZE, ras_size-ROAD_SIZE);
			}
			cal_tan = APL_Tan256((US)rad);
			
			/* ���[�h�p�^�[��*/
			/* �Z���^�[���C���̒����ƊԊu�͂T�� */
			/* 60fps=17ms/f */
			/* �ő呬�x307km/h(85m/s,0.0853m/ms) 1f=1.45m�i�� */
			/* �Œᑬ�x6km/h(1.7m/s,0.0017m/ms) 1f=0.028m�i�� */
			/* 1LSB 10km/h(2.8m/s,0.0028m/ms) 1f=0.047m�i�� */
			/* 1times Cal 1m=4dot (17ms/f x 1/0.047 / speed) */
			if(speed == 0u)	/* �ԑ�0km/h */
			{
				uRoadAnime = uRoadAnime_old;
				uRoadAnimeBase = uRoadAnimeBase_old;
				uRoadAnimeCount = 0;
				
				unRoadAnimeTime = time_old;	/* �O��l���X�V */
			}
			else if( (time_old - unRoadAnimeTime) < (362 / speed) )	/* �X�V�������� */
			{
				uRoadAnime = uRoadAnime_old;
				uRoadAnimeBase = uRoadAnimeBase_old;
				uRoadAnimeCount = 0;
			}
			else
			{
				unRoadAnimeTime = time_old;	/* �O��l���X�V */

				uRoadAnime++;
				if(uRoadAnime >= 4u)
				{
					uRoadAnime = 0;
				}
				
				uRoadAnimeBase++;
				if(uRoadAnimeBase >= 4)
				{	
					uRoadAnimeBase = 0;
					bCounter++;			/*  8bit �t���[�����J�E���^�X�V */
				}

			}
			
			for(y=ras_st; y < ras_ed; y++)
			{
				SS num;
				US offset;
				US ras_cal;
				US uBG_pat[4][4] = {288,   0,  96, 192,
					 				192, 288,   0,  96,
					 				 96, 192, 288,   0,
					 				  0,  96, 192, 288};
				
				num = y - ras_st;
				offset = ras_size - num;
				
				/* X���W */
				if(num == 0u)
				{
					ras_tmp[y] = 0;	/* �ŏ� */
				}
				else
				{
					//ras_tmp[y] = num * ((float)vx / 16);
					ras_tmp[y] = (num * vx) >> 4;										/* HUYE����̃A�h�o�C�X�ӏ� */
					//ras_tmp[y] += road * ( (RASTER_MAX - ras_st) / (float)(Mmax(y-ras_st, 1)) );
					ras_tmp[y] += (Mmin(Mmax((road * ras_size), -256), 512) / num );	/* HUYE����̃A�h�o�C�X�ӏ� */
				}
				ras_tmp_final = ras_tmp[y];

				/* Y���W */
				if(num == 0u)	/* �����0or512�łȂ��Ƌ�ɓ����o�Ă��� */	/* �Ղ̂Ђ� ����̃A�h�o�C�X�ӏ� */
				{
					ras_cal = 0;
					ras_debug[y] = cal_tan;
					road_offset_y = 3;
					road_offset_val = 1;
					uRoadAnime_old = uRoadAnime;
					uRoadAnimeBase_old = uRoadAnimeBase;
					uCount = 0;
				}
				else if(vy < 0)	/* ����� */
				{
					uRoad_strch = (offset * cal_tan) >> 8;
					ras_debug[y] = uRoad_strch;
					ras_cal = nHorizon_offset + uRoad_strch + uBG_pat[uRoadAnimeBase][uRoadAnime];
				}
				else if(vy > 0)	/* ���� */
				{
					uRoad_strch = (offset * cal_tan) >> 8;
					ras_debug[y] = uRoad_strch;
					//ras_cal = nHorizon_offset + vy + uRoad_strch + uBG_pat[uRoadAnimeBase][uRoadAnime];
					ras_cal = nHorizon_offset + uRoad_strch + uBG_pat[uRoadAnimeBase][uRoadAnime];
				}
				else			/* ���R */
				{
					ras_cal = nHorizon_offset + uBG_pat[uRoadAnimeBase][uRoadAnime];
				}
				if(ras_cal <   0)ras_cal += 512;
				if(ras_cal > 512)ras_cal -= 512;
				pal_tmp[y] = ras_cal;
				
				/* ���[�h�p�^�[��*/
				uRoadAnimeCount++;
				if(uRoadAnimeCount >= road_offset_y)
				{
					if(
						((ras_size>>2) > num) 
					)
					{
						road_offset_y = 4;
					}
					else
					{
						road_offset_y += road_offset_val;
					}
					uRoadAnimeCount = 0;
					uRoadAnime++;
				}
				if(uRoadAnime >= 4u)
				{
					uRoadAnime = 0;
				}
				
				uCount++;
			}
			SetRasterVal(ras_tmp, sizeof(US)*RASTER_MAX);
			SetRasterPal(pal_tmp, sizeof(US)*RASTER_MAX);
		}
#endif
		if(b96ms_time == TRUE)
		{
			/* �������׉��P�v */
			UI nTimeCounter, nPassTime, nTimer;
			US uScore;
			/* Score */
			uScore = usFreeRunCount;
			Text_To_Text(uScore, 192, 8, FALSE, "%7d");
			/* Top Score */
			uScoreMax = Mmax(uScore, uScoreMax);
			Text_To_Text(uScoreMax, 40, 8, FALSE, "%7d");
			/* Time Count */
			GetNowTime(&time_now);
			nPassTime = (time_now - start_time);
			nTimer = 120000 - nPassTime;
			
			if(120000 < nTimer)
			{
				nTimeCounter = 0;
			}
			else
			{
				nTimeCounter = nTimer / 1000;
			}
			Text_To_Text(nTimeCounter, 112, 24, TRUE, "%3d");
			/* Speed */
			Text_To_Text(Vs, 208, 24, FALSE, "%3d");
			/* Gear */
			Text_To_Text(ShiftPos, 224, 32, FALSE, "%d");
		}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */

#if	1
		if( (input & KEY_B) != 0U )	/* B�{�^�� */
		{
			SI	adpcm_sns;
			
			adpcm_sns = m_stat(9/*Mmin(Mmax(25, vx), 31)*/);	/* ADPCM ch1(9) ch2-8(25-31) */
//			Message_Num(&adpcm_sns,	 0, 13, 2, MONI_Type_SI, "%d");
			
			if( (time_old - unExplosion_time) >  (60000 / rpm) )	/* ��]���̃G���W���� */
			{
				unExplosion_time = time_old;
				bExplosion = TRUE;
			}
			else
			{
				bExplosion = FALSE;
			}

			/* ��]���̃G���W���� */
			if( bExplosion == TRUE )
			{
				m_pcmplay(11,3,4);
			}
		}
		else
		{
		}
#endif
//		BG_TextPut("OverKata", 4, 10);
//		BG_TextPut("OverKata", 128, 128);
//		BG_TextPut("OVER KATA", 128, 128);
#endif
#ifdef DEBUG
		if(bDebugMode == TRUE)
		{
			if(b96ms_time == TRUE)
			{
				/* ���j�^ */
				Message_Num(&rpm,	 			12,  6, 2, MONI_Type_SS, "%4d");
				
				Message_Num(&time_cal,	 		 0,  8, 2, MONI_Type_UI, "%3d");
				Message_Num(&time_cal_PH,		10,  8, 2, MONI_Type_UI, "%3d");
				
				Message_Num(&speed,				 0,  9, 2, MONI_Type_SS, "%3d");
				Message_Num(&vx, 				 6,  9, 2, MONI_Type_SS, "%2d");
				Message_Num(&vy, 				12,  9, 2, MONI_Type_SS, "%2d");
				
				Message_Num(&ras_st,			 0, 10, 2, MONI_Type_US, "%3d");
				Message_Num(&ras_ed,			 7, 10, 2, MONI_Type_US, "%3d");
				Message_Num(&ras_size,			13, 10, 2, MONI_Type_US, "%3d");
				Message_Num(&nHorizon_offset,	20, 10, 2, MONI_Type_SS, "%3d");
				
				Message_Num(&pal_tmp[ras_st],	 0, 11, 2, MONI_Type_US, "%d");
				Message_Num(&rad,				12, 11, 2, MONI_Type_FL, "%f");
				
				Message_Num(&ras_tmp[ras_st],	 0, 12, 2, MONI_Type_SS, "%d");
				Message_Num(&road,			 	 6, 12, 2, MONI_Type_US, "%d");
				Message_Num(&uDebugNum,		 	12, 12, 2, MONI_Type_US, "%d");
			}
		}
#endif
		/* �`��̃N���A���� */	/* �K���e�L�X�g�\�������̌�ɍs������ */
		if(b96ms_time == TRUE)
		{
			if((*CRTC_480 & 0x02u) == 0u)	/* �N���A���s���łȂ� */
			{
				*CRTC_R21 = Mbset(*CRTC_R21, 0x0Fu, 0x0Cu);	/* SCREEN1 �����N���AON / SCREEN0 �����N���AOFF */
				*CRTC_480 = Mbset(*CRTC_480, 0x02u, 0x02u);	/* �N���A���s */
			}
		}
		/* �`�揈�� */
		if(b32ms_time == TRUE)
		{
			if(speed <= 8)								/* ������� */
			{
				uCountNum = Mdec(uCountNum, 1);
				bUpdate = TRUE;
			}
			else if( (speed > 8) && (speed < 16) )		/* �ێ�*/
			{
			}
			else
			{
				uCountNum++;							/* ������ */
				bUpdate = TRUE;
			}
			if(uCountNum >= 64)uCountNum = 0;
			if(uCountNum <= 0)uCountNum = 0;
			
			/* ��ʂ�h�炷 */
			if((speed == 0) && (rpm == 0)){	/* ��� */
				nVibration = 0;
			}
			else{			/* ���s�� */
				if((usFreeRunCount % 5) == 0)nVibration = 1;	/* ��ʂ̐U�� */
				else nVibration = 0;
			}

			/* ��ʂ̈ʒu */
			HOME(0b01, view_offset_x, view_offset_y + nVibration );	/* Screen 0(TPS/FPS) */
			HOME(0b10, view_offset_x, view_offset_y );		/* Screen 1 */

			if( bUpdate == TRUE )
			{
				/* ���C�o���� */
				US	j,w,h;
				i = uCountNum;
				j = (64-uCountNum)>>4;	/* ���Ԋu�{���ɂȂ��Ă���̂ŉ��P�v */
				w = ENEMY_CAR_1_W>>j;
				h = ENEMY_CAR_1_H>>j;
				/* x��y���A�{���ω����ɍ��W���Z���^�[�����̂��߈�a������ */
				x = view_offset_x + (WIDTH>>1) - ras_tmp[Mmin(ras_st + uCountNum, ras_ed)];//ras_tmp_final
				y = view_offset_y + nHorizon + i;

				G_BitBlt(	x,	w,
							y,	h,
							1,
							0,					w,
							ENEMY_CAR_1_H * j,	h,
							1,
							g_mode);
#if 0
				/* ��Q�� */
				w = MY_CAR_0_W>>j;
				h = MY_CAR_0_W>>j;
				for(i=0; i<2; i++)
				{
					US obj_x, obj_y;
					obj_x = x-32;
					obj_y = y;
					Draw_Fill(obj_x,obj_y,obj_x-w,obj_y-6,0xC2u);
					obj_x = x+32;
					obj_y = y;
					Draw_Fill(obj_x,obj_y,obj_x+w,obj_y-6,0xC2u);
				}
#endif
			}
		}

		usFreeRunCount++;	/* 16bit �t���[�����J�E���^�X�V */
#if 1
		/* �������Ԍv�� */
		GetNowTime(&time_now);
		time = time_now - time_old;
		time_cal = time;	/* LSB:1 UNIT:ms */
		time_cal_PH = Mmax(time_cal, time_cal_PH);
#endif
		/* �����҂� */
		vwait(1);
	}
	while( loop );

	CRTCRAS((void *)0, 0);		/* stop */
	HSYNCST((void *)0);			/* stop */
	VDISPST((void *)0, 0, 0);	/* stop */
	TIMERDST((void *)0, 0, 1);	/* stop */
	
	BGCTRLST(0, 0, 0);	/* �a�f�O�\���n�m */
	BGCTRLST(1, 1, 0);	/* �a�f�P�\���n�m */
	G_CLR_ON();

	B_CURON();
	TextClear();

	m_stop(0,0,0,0,0,0,0,0,0,0);	/* ���y��~ */
	m_init();		/* ������ */

	CRTMOD(crtmod);			/* ���[�h�����Ƃɖ߂� */
	
	_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */

	/*�X�[�p�[�o�C�U�[���[�h�I��*/
	SUPER(superchk);

#if 1
	printf("pal_tmp[0]�̒��g = %d(vy=%d)\n", pal_tmp[0], vy );
	for(y=ras_st; y < ras_ed; y++)
	{
		printf("[%3d]=(%5d),", y, pal_tmp[y] );
		//printf("[%3d]=(%5d),", y, ras_debug[y] );
		if((y%5) == 0)printf("\n");
	}
	printf("\n");
#endif
	
}

#endif	/* OVERKATA_C */
