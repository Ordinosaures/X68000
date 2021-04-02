#ifndef	MyCar_C
#define	MyCar_C

#include <limits.h>
#include <iocslib.h>

#include "inc/usr_macro.h"
#include "inc/ZMUSIC.H"

#include "MyCar.h"
#include "OverKata.h"
#include "APL_Math.h"
#include "APL_MACS.h"
#include "Course_Obj.h"
#include "CRTC.h"
#include "Draw.h"
#include "Input.h"
#include "EnemyCAR.h"
#include "FileManager.h"
#include "Graphic.h"
#include "MFP.h"
#include "Music.h"
#include "Output_Text.h"
#include "PCG.h"
#include "Raster.h"
#include "Text.h"

/* �O���[�o���ϐ� */
SS	g_speed = 0;

/* �\���̒�` */
ST_CARDATA	g_stMyCar = {0};

/* �֐��̃v���g�^�C�v�錾 */
SS	MyCar_G_Load(void);
SS	GetMyCar(ST_CARDATA *stDat);
SS	SetMyCar(ST_CARDATA stDat);
SS	MyCarInfo_Init(void);
SS	MyCarInfo_Update(SS);
SS	MyCar_Interior(UC);
SS	MyCar_CourseOut(void);
SS	GetMyCarSpeed(SS *);
void MyCar_Image(void);
void MyCar_Background(void);
static SS	MyCar_Vibration(void);
static SS	MyCar_Mascot(SS);
static SS	MyCar_Tachometer(SS);

/* �֐� */
SS	MyCar_G_Load(void)
{
	SS	ret = 0;
	
	MyCar_Background();	/* �w�i�̕\�� */

	MyCar_Image();		/* ���Ԃ̕\�� */

	return ret;
}

SS	GetMyCar(ST_CARDATA *stDat)
{
	SS	ret = 0;
	*stDat = g_stMyCar;
	return ret;
}

SS	SetMyCar(ST_CARDATA stDat)
{
	SS	ret = 0;
	g_stMyCar = stDat;
	return ret;
}

SS	MyCarInfo_Init(void)
{
	SS	ret = 0;
	
	g_stMyCar.ubCarType		= 0u;	/* �Ԃ̎�� */
	g_stMyCar.uEngineRPM	= 0u;	/* �G���W����]�� */
	g_stMyCar.VehicleSpeed	= 0;	/* �ԑ� */
	g_stMyCar.Steering		= 0;	/* �X�e�A */
	g_stMyCar.ubShiftPos	= 0u;	/* �M�A�i */
	g_stMyCar.ubThrottle	= 0u;		/* �X���b�g���J�x */
	g_stMyCar.ubBrakeLights	= FALSE;	/* �u���[�L���C�g */
	g_stMyCar.ubHeadLights	= FALSE;	/* �w�b�h���C�g */
	g_stMyCar.ubWiper		= FALSE;	/* ���C�p�[ */
	g_stMyCar.bTire			= 0;	/* �^�C���̏�� */
	g_stMyCar.ubOBD			= OBD_NORMAL;	/* �̏�̏�� */
	
	return ret;
}

SS	MyCarInfo_Update(SS input)
{
	SS	ret = 0;
	SS	speed_min;
	SS	Slope;
	SS	Angle;
	SS	myCarSx, myCarEx, myCarSy, myCarEy;

	US	uTM[6] = { 0, 2857, 1950, 1444, 1096, 761 };/* �ϑ���  1:2.857 2:1.95 3:1.444 4:1.096 5:0.761 */
//	US	uTM_F[6] = { 0, 13390, 9140, 6768, 5137, 3567 };/* �������� */
	US	uRPM[11] = { 500, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000 };	/* Eng��]�� */
	US	uTRQ[11] = { 120,  130,  150,  170,  210,  200,  180,  170,  140,  120,  100 };	/* �g���N */

	UC	bAxis;
	UC	bUpDown = 0;
	UC	bHit = FALSE;
	UI	time;
	UI	i;
	UI	uEnemyNum = 0;
	UC bMode;

	ST_CRT			stCRT;
	ST_ROAD_INFO	stRoadInfo;
	ST_ENEMYCARDATA	stEnemyCar = {0};

	static UC bShiftPosFlag[3] = {FALSE};
	static UC bSpin = 0;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	UC	bDebugMode;
	US	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
	GetGameMode(&bMode);
	GetCRT(&stCRT, bMode);
	GetRoadInfo(&stRoadInfo);
	Slope = stRoadInfo.slope;
	Angle = stRoadInfo.angle;

	GetStartTime(&time);	/* �J�n�������擾 */
	
	/* �X�e�A�����O���� */
	if((input & KEY_RIGHT) != 0u)
	{
		g_stMyCar.Steering += g_speed << 4;	/* �E */
		g_stMyCar.uEngineRPM -= 3;	/* �p���X�e�쓮�� */

		if(bSpin == 1)	/* �E�X�s�� */
		{
			bSpin = 0;	/* �X�s���L�����Z�� */
			ADPCM_Play(4);	/* �u���[�L�� */
		}
	}
	else if((input & KEY_LEFT) != 0u)
	{
		g_stMyCar.Steering -= g_speed << 4;	/* �� */
		g_stMyCar.uEngineRPM -= 3;	/* �p���X�e�쓮�� */

		if(bSpin == 2)	/* ���X�s�� */
		{
			bSpin = 0;	/* �X�s���L�����Z�� */
			ADPCM_Play(4);	/* �u���[�L�� */
		}
	}
	else
	{
		
	}
	
	if(g_stMyCar.ubOBD == OBD_DAMAGE)	/* �̏ᒆ */
	{
		/* �X�s������ */
		if(bSpin == 1)		
		{
			g_stMyCar.Steering += g_speed << 3;	/* �E */
		}
		else if(bSpin == 2)
		{
			g_stMyCar.Steering -= g_speed << 3;	/* �� */
		}
		else
		{
			/* �ێ� */
		}
	}
	/* �����蔻��̐��� */
	myCarSx = ROAD_CT_POINT + (g_stMyCar.Steering >> 8) - 8;
	myCarEx = myCarSx + 16;
	myCarSy = Y_MAX_WINDOW - 32;
	myCarEy = myCarSy + 16;
	
	/* �V�t�g���� */
	if( ((input & KEY_A) != 0u) && ((input & KEY_B) == 0u))	/* A�{�^��(only) */
	{
		/* �A�N�Z�������̓V�t�g����֎~ */
	}
	else{
		if(ChatCancelSW((input & KEY_UPPER)!=0u, &bShiftPosFlag[0]) == TRUE)
		{
			if(g_stMyCar.ubShiftPos != 5)
			{
				bUpDown = 1;
				ADPCM_Play(7);	/* SE:�V�t�g�A�b�v */
			}
			g_stMyCar.ubShiftPos = Mmin(g_stMyCar.ubShiftPos+1, 5);	/* �� */
		}
		
		if(ChatCancelSW((input & KEY_LOWER)!=0u, &bShiftPosFlag[1]) == TRUE)
		{
			if(g_stMyCar.ubShiftPos != 0)
			{
				bUpDown = 2;
				ADPCM_Play(7);	/* SE:�V�t�g�_�E�� */
			}
			g_stMyCar.ubShiftPos = Mmax(g_stMyCar.ubShiftPos-1, 0);	/* �� */
		}
	}
#if 0
	g_stMyCar.ubShiftPos = 2;	/* �e�X�g�p�ϑ��Œ� */
#endif	
	g_stMyCar.ubShiftPos = Mmax(Mmin(g_stMyCar.ubShiftPos, 5), 0);
	
	if(bUpDown != 0u) 
	{
		g_stMyCar.uEngineRPM = (g_stMyCar.VehicleSpeed * uTM[g_stMyCar.ubShiftPos]) / 26;
		g_stMyCar.uEngineRPM = Mmax(Mmin(9000, g_stMyCar.uEngineRPM), 750);
	}
	
	/* ��]�����Z�o */
	bAxis = 10;
	for( i=0; i<10; i++ )
	{
		if( g_stMyCar.uEngineRPM <= (uRPM[i] + ((uRPM[i+1] - uRPM[i]) >> 1)) )
		{
			bAxis = i;
			break;
		}
	}
	
	/* �A�N�Z�� */
	if((input & KEY_A) != 0u)		/* A�{�^�� */
	{
		g_stMyCar.ubThrottle	= Minc(g_stMyCar.ubThrottle, 1u);	/* �X���b�g���J�x */
		g_stMyCar.uEngineRPM += (uTM[g_stMyCar.ubShiftPos] + uTRQ[bAxis]) >> 7;		/* ��]�� */

		/* �G���W���̉� */
		{
			static	UI	unExplosion_time = 0;
			SS	rpm;

			rpm	= Mmax(g_stMyCar.uEngineRPM, 1);
			
			if( (time - unExplosion_time) >  (150 - Mdiv64(rpm)) )	/* ��]���̃G���W����(60000 / rpm) */
			{
				unExplosion_time = time;
//				SE_Play_Fast(6);	/* FM���ʉ��Đ�(����) */
//				SE_Play(6);		/* FM���ʉ��Đ� */
			}
			else
			{
				/* �������Ȃ� */
			}
		}
	}
	else{
		static	UI	unFuelCut_time = 0;
		SS	rpm;
		rpm	= Mmax(g_stMyCar.uEngineRPM, 1);
		
		g_stMyCar.ubThrottle = Mdec(g_stMyCar.ubThrottle, 1u);	/* �X���b�g���J�x */
		g_stMyCar.uEngineRPM -= uTM[g_stMyCar.ubShiftPos] >> 8;		/* ��]�� */
		
		/* �G���W���̉�(�R���J�b�g) */
		if( (time - unFuelCut_time) >  (150 - Mdiv64(rpm)) )	/* ��]���̃G���W����(60000 / rpm) */
		{
			unFuelCut_time = time;
			if(g_stMyCar.ubThrottle > 0)
			{
//				SE_Play(7);	/* FM���ʉ��Đ� */
			}
			else
			{
//				SE_Play(8);	/* FM���ʉ��Đ� */
			}
		}
	}
	
	/* �u���[�L */
	if( (input & KEY_B) != 0U )	/* B�{�^�� */
	{
		if(g_stMyCar.VehicleSpeed > 5)
		{
			ADPCM_Play(4);	/* �u���[�L�� */
		}
		g_stMyCar.VehicleSpeed = Mdec((US)g_stMyCar.VehicleSpeed, 1u);	/* �ϑ���ŕς����� */
		g_stMyCar.ubBrakeLights = TRUE;	/* �u���[�L�����v ON */
	}
	else
	{
		g_stMyCar.ubBrakeLights = FALSE;	/* �u���[�L�����v OFF */
	}
	
	/* �Փ˔��� */
	if(		(g_stMyCar.VehicleSpeed != 0) 		/* �ԑ����� */
		&&  (g_stMyCar.ubOBD == OBD_NORMAL) )	/* �̏Ⴕ�Ă��Ȃ� */
	{
		for(i = 0; i < ENEMYCAR_MAX; i++)
		{
			GetEnemyCAR(&stEnemyCar, i);
			if( stEnemyCar.ubAlive == TRUE )
			{
				if(    (myCarSx > stEnemyCar.sx)
					&& (myCarEx < stEnemyCar.ex)
					&& (myCarSy > stEnemyCar.sy)
					&& (myCarEy < stEnemyCar.ey))
				{
					/* �Փ˔��� */
					bHit = TRUE;
					uEnemyNum = i;

					/* �X�e�A�����O�̏�ԂŃX�s�� */
					if((myCarSx - stEnemyCar.sx) >= (stEnemyCar.ex - myCarEx) )
					{
						bSpin = 1;	/* �E�X�s�� */
					}
					else
					{
						bSpin = 2;	/* ���X�s�� */
					}
				}
			}
		}
	}
	
	/* �Փ˔��� */
	if( bHit == TRUE )
	{
		g_stMyCar.ubOBD = OBD_DAMAGE;	/* �̏� */
		g_stMyCar.uEngineRPM = g_stMyCar.uEngineRPM >> 1;		/* 1/2 */
	}

	/* ��]���N���b�v */
	g_stMyCar.uEngineRPM = Mmax(Mmin(9000, g_stMyCar.uEngineRPM), 750);

	/* �ԑ� */
	if(g_stMyCar.ubShiftPos == 0u)		/* �j���[�g���� */
	{
		g_stMyCar.VehicleSpeed = Mdec((US)g_stMyCar.VehicleSpeed, 1u);
	}
	else
	{
		if(g_stMyCar.ubBrakeLights == TRUE)
		{
			/* �ԑ�����G���W����]�����Z�o */
			g_stMyCar.uEngineRPM = (g_stMyCar.VehicleSpeed * uTM[g_stMyCar.ubShiftPos] ) / 26u;	
			/* ��]���N���b�v */
			g_stMyCar.uEngineRPM = Mmax(Mmin(9000, g_stMyCar.uEngineRPM), 750);
		}
		/* �ϑ���  1:2.857 2:1.95 3:1.444 4:1.096 5:0.761 ������ 4.687 �^�C������2052.1mm */
		/* �^�C�������~�U�O�~��]���^�i�P�O�O�O�~�ϑ���~������j */
		g_stMyCar.VehicleSpeed = (SS)(((UI)26 * g_stMyCar.uEngineRPM) / uTM[g_stMyCar.ubShiftPos]);	
	}
	/* �ԑ��N���b�v */
	g_stMyCar.VehicleSpeed = Mmax(Mmin(310, g_stMyCar.VehicleSpeed), 0);

	/* �ԑ��i�Q�[�����j */
	if( (g_stMyCar.ubBrakeLights == TRUE)		/* �u���[�L�����vON */
	||  (g_stMyCar.ubShiftPos == 0u)		)	/* �j���[�g���� */
	{
		speed_min = 0;
	}
	else
	{
		speed_min = 1;
	}

	/* �ԑ��i�Q�[�����j */
	g_speed = g_stMyCar.VehicleSpeed >> 3;	/* 1LSB 10km/h */
	g_speed = Mmax(Mmin(g_speed, 31), speed_min);

	/* �R�[�i�����O */
#if 1
	if( g_speed != 0u )
	{
		if(Angle != 0)
		{
			g_stMyCar.Steering += Angle * g_speed;	/* �o�����X�����v */
		}
//		else if(Angle < 0)
//		{
//			g_stMyCar.Steering -= Angle * g_speed;	/* �o�����X�����v */
//		}
		else
		{
			/* �O��l�ێ� */
		}
	}
	else{
		/* �O��l�ێ� */
	}
#endif
	
	/* �X�e�A�����O�N���b�v */
	g_stMyCar.Steering = Mmax(Mmin(g_stMyCar.Steering, 3800), -3800);

	/* �Փ˔��� */
	if( bHit == TRUE )
	{
		GetEnemyCAR(&stEnemyCar, uEnemyNum);
		stEnemyCar.VehicleSpeed = g_stMyCar.VehicleSpeed + 5;
		SetEnemyCAR(stEnemyCar, uEnemyNum);	/* �G�Ԃ̍X�V */
	}

	ret = (SS)bHit;
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	if(bDebugMode == TRUE)
	{
		US color;
		switch(g_stMyCar.ubOBD)
		{
			case OBD_NORMAL:
			{
				color = 0x03;
				break;
			}
			case OBD_DAMAGE:
			{
				color = 0x13;
				break;
			}
			case OBD_SPIN:
			{
				color = 0x19;
				break;
			}
			case OBD_COURSEOUT:
			{
				color = 0x08;
				break;
			}
			default:
			{
				color = 0x14;
				break;
			}
		}
		
		Draw_Box(
			stCRT.hide_offset_x + myCarSx,
			stCRT.hide_offset_y + myCarSy,
			stCRT.hide_offset_x + myCarEx,
			stCRT.hide_offset_y + myCarEy, color, 0xFFFF);
	}
#endif
	
	return ret;
}

SS	MyCar_Interior(UC bMode)
{
	SS	ret = 0;

	SS	Vibration = 0;

	/* �ԑ̂�U�������� */
	Vibration = MyCar_Vibration();
	
	/* �}�X�R�b�g *//* �������� �� */
//	MyCar_Mascot(Vibration);
	
	/* �^�R���[�^�[�j */
	MyCar_Tachometer(Vibration);
	
	/* �n���h�� */
	/* ������ */
	
	return ret;
}


SS	MyCar_CourseOut(void)
{
	SS	ret = 0;
	SS	vx;
	vx	= g_stMyCar.Steering >> 8;

	if( Mabs(vx) > 255 )	/* �R�[�X�O */
	{
		SS	rpm	= g_stMyCar.uEngineRPM;
		
		g_stMyCar.uEngineRPM -= (rpm>>3);	/* �������� */

		g_stMyCar.ubOBD = OBD_COURSEOUT;	/* �R�[�X�A�E�g */
	}
	else
	{
		if(g_stMyCar.ubOBD == OBD_COURSEOUT)
		{
			g_stMyCar.ubOBD = OBD_NORMAL;	/* �ʏ� */
		}
	}
	
	return ret;
}

SS	GetMyCarSpeed(SS *speed)
{
	SS	ret = 0;
	
	*speed = g_speed;

	return ret;
}

void MyCar_Image(void)
{
	/* FPS */
#if 1	/* �e�L�X�g */
	PutGraphic_To_Text( 0, 0, 0			);	/* �C���e���A */
	PutGraphic_To_Text( 0, 0, Y_OFFSET	);	/* �C���e���A */

	T_Fill( 90,        0 + 180, 32, 31, 0, 0);		/* ���[�^�[���i��j */
	T_Fill( 90, Y_OFFSET + 180, 32, 31, 0, 0);		/* ���[�^�[���i���j */

	T_Fill(  0,        0 + 224, 255, 2, 0, 0x0F);	/* ���сi��j */
	T_Fill(  0, Y_OFFSET + 224, 255, 2, 0, 0x0F);	/* ���сi���j */
#else
	SS x;

	G_Load_Mem( 0, X_OFFSET,	0,			0);	/* �C���e���A */
	G_Load_Mem( 0, X_OFFSET,	Y_OFFSET,	0);	/* �C���e���A */
	/* ���[�^�[�̌� */
	for(x = 0; x < 17; x++)
	{
		Draw_Circle(X_OFFSET + 106,				195,	x, TRANS_PAL, 0, 360, 255);	/* ���������� */
		Draw_Circle(X_OFFSET + 106, Y_OFFSET +	195,	x, TRANS_PAL, 0, 360, 255);	/* ���������� */
	}
	/* �]���̓h��Ԃ� */
	Draw_Fill(X_OFFSET, (       0 + MY_CAR_1_H), X_OFFSET + MY_CAR_1_W,        0 + V_SYNC_MAX, 0x01);
	Draw_Fill(X_OFFSET, (Y_OFFSET + MY_CAR_1_H), X_OFFSET + MY_CAR_1_W, Y_OFFSET + V_SYNC_MAX, 0x01);

	Draw_Fill(X_OFFSET + 90, 			180 + 1,	X_OFFSET + 90 + 31, 			180 + 31 - 1,	 TRANS_PAL);	/* ���������� */
	Draw_Fill(X_OFFSET + 90, Y_OFFSET + 180 + 1,	X_OFFSET + 90 + 31, Y_OFFSET +	180 + 31 - 1,	 TRANS_PAL);	/* ���������� */
#endif
	

#if 0	/* TPS */
#else
#endif
}

void MyCar_Background(void)
{
	SS ret;
	US height_sum = 0u;
	US height_sum_o = 0u;
	UI i;
	UI uWidth, uHeight, uFileSize;
	UI uWidth_o, uHeight_o;
	UI uOffset_X = 0u;
	
	ret = G_Load_Mem( ENEMYCAR_CG, uOffset_X,	0,	0);	/* ���C�o���� */
	Get_PicImageInfo( ENEMYCAR_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
	uWidth_o = uWidth;
	uHeight_o = uHeight;

	if(ret >= 0)
	{
		/* 11�p�^�[������� */
		UI	uW_tmp, uH_tmp;
		
		height_sum = 0;
		height_sum_o = 0;

		for(i=1; i<=11u; i++)
		{
			/* �k����̃T�C�Y */
			height_sum_o += uHeight_o;
			uW_tmp = uWidth_o << 3;
			uWidth_o = Mmul_p1(uW_tmp);
			uH_tmp = uHeight_o << 3;
			uHeight_o = Mmul_p1(uH_tmp);
			
			/* �`�� */
			G_Stretch_Pict(
							0 + uOffset_X,	uWidth_o + uOffset_X,
							height_sum_o,	uHeight_o,
							0,
							0 + uOffset_X,	uWidth + uOffset_X,
							height_sum,		uHeight,
							0);
			/* ���̏k���� */
			height_sum += uHeight;
			uWidth = uWidth_o;
			uHeight = uHeight_o;
		}
	}
	
	uOffset_X = 140;
	ret = G_Load_Mem( COURSE_OBJ_CG, uOffset_X,	0,	0);	/* ���V�̖� */
	Get_PicImageInfo( COURSE_OBJ_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
	uWidth_o = uWidth;
	uHeight_o = uHeight;
	
	if(ret >= 0)
	{
		/* 9�p�^�[������� */
		UI	uW_tmp, uH_tmp;
		
		height_sum = 0;
		height_sum_o = 0;

		for(i=1; i<9; i++)
		{
			/* �k����̃T�C�Y */
			height_sum_o += uHeight_o;
			uW_tmp = uWidth_o << 3;
			uWidth_o = Mmul_p1(uW_tmp);
			uH_tmp = uHeight_o << 3;
			uHeight_o = Mmul_p1(uH_tmp);

			/* �`�� */
			G_Stretch_Pict(
							0 + uOffset_X,	uWidth_o + uOffset_X,
							height_sum_o,	uHeight_o,
							0,
							0 + uOffset_X,	uWidth + uOffset_X,
							height_sum,		uHeight,
							0);

			/* ���̏k���� */
			height_sum += uHeight;
			uWidth = uWidth_o;
			uHeight = uHeight_o;
		}
	}

	Get_PicImageInfo( BG_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
	G_Load_Mem( BG_CG, 		0,		Y_MIN_DRAW +        0 + Y_HORIZON_1 - uHeight + 8,	1);	/* �w�i */
	G_Load_Mem( BG_CG, X_OFFSET+16,	Y_MIN_DRAW +        0 + Y_HORIZON_1 - uHeight + 8,	1);	/* �w�i */
	G_Load_Mem( BG_CG, 		0,		Y_MIN_DRAW + Y_OFFSET + Y_HORIZON_1 - uHeight + 8,	1);	/* �w�i */
	G_Load_Mem( BG_CG, X_OFFSET+16,	Y_MIN_DRAW + Y_OFFSET + Y_HORIZON_1 - uHeight + 8,	1);	/* �w�i */
}

static SS	MyCar_Vibration(void)
{
	SS ret = 0;

	SS	x, y;
	UC	bMode;
	ST_CRT	stCRT;

	static	SS	Vibration = 0;
	static	SS	VibrationCT = 0;
	static	UC	ubOBD_old = OBD_NORMAL;
	static	SS	CrashCount;
	
	GetGameMode(&bMode);
	GetCRT(&stCRT, bMode);

	/* ��ʂ�h�炷 */
	VibrationCT++;
	if(VibrationCT > 5)
	{
		VibrationCT = 0;
	}
	
	if((g_stMyCar.ubOBD & (OBD_DAMAGE|OBD_COURSEOUT)) != 0u)
	{
		if(ubOBD_old == OBD_NORMAL)
		{
			ADPCM_Play(12);	/* SE�F�N���b�V�� */
			CrashCount = 30;
		}
		
		if((g_stMyCar.ubOBD & OBD_DAMAGE) != 0u)
		{
			if(CrashCount == 22)	/* ����SE�� */
			{
				ADPCM_Play(15);	/* �X�L�[���� */
			}
			CrashCount = Mdec(CrashCount, 1u);	/* �J�E���^�f�N�������g */

			if(CrashCount == 0)
			{
				/* ���� */
				//MOV_Play(3);	/* �R������ */
				g_stMyCar.ubOBD = OBD_NORMAL;	/* ���� */
			}
		}
		Vibration = (VibrationCT == 0)?8:0;	/* ��ʂ̐U�� */
	}
	else
	{
		Vibration = (VibrationCT == 0)?1:0;	/* ��ʂ̐U�� */
	}
	ubOBD_old = g_stMyCar.ubOBD;	/* �O��l�X�V */

	/* ��ʂ��t���b�v���� */
	GetCRT(&stCRT, bMode);
	x = stCRT.view_offset_x;
	y = stCRT.view_offset_y;
	T_Scroll( 0, y + Vibration   );	/* �e�L�X�g��� */
	
	ret = Vibration;
	
	return ret;
}

static SS	MyCar_Mascot(SS Vibration)
{
	SS ret = 0;
	
	SS	x, y;
	US	nRatio = 0x80;
	UC	palNum = 0;
	UC	sp_num=0;
	
	volatile US *PCG_src  = (US *)0xEBA000;
	volatile US *PCG_dst  = (US *)0xEBA180;

	static	US	rad = 180;
	static	UC	ubRadFlag = TRUE;

#ifdef	DEBUG
	GetDebugNum(&nRatio);
#endif

	/* �}�X�R�b�g���h��� */
	if(g_stMyCar.VehicleSpeed == 0)
	{
		if(rad == 180)
		{
			rad = 180;
		}
		else if(rad < 180)
		{
			rad++;
		}
		else
		{
			rad--;
		}
	}
	else
	{
		US	width;
		
		width = 15 + (g_stMyCar.VehicleSpeed / 10);
		
		if(ubRadFlag == TRUE)
		{
			rad+=Mmax(g_stMyCar.VehicleSpeed/30, 1);
			if(rad > 180 + width)ubRadFlag = FALSE;
		}
		else
		{
			rad-=Mmax(g_stMyCar.VehicleSpeed/30, 1);
			if(rad < 180 - width)ubRadFlag = TRUE;
		}
	}
	

	x = 16 + (( 32 * APL_Sin(rad)) >> 8);
	y =  0 + ((-32 * APL_Cos(rad)) >> 8) - Vibration;

	palNum = 9;

	PCG_Rotation((US *)PCG_dst, (US *)PCG_src, 3, 3, x, y, &sp_num, palNum, (nRatio-0x80), 180-rad);

	return ret;
}


static SS	MyCar_Tachometer(SS Vibration)
{
	SS ret = 0;
	
	SS	i;
	UC	patNum = 0;
	UC	palNum = 0;
	UC	V=0, H=0;
	UC	sp_num=0;
	SS	x, y;

	/* �^�R���[�^�[�j */
	for( i = 0; i < 18; i++ )
	{
		if( ((i*500) <= g_stMyCar.uEngineRPM) && (g_stMyCar.uEngineRPM < ((i*500)+500)) )
		{
			/* ��]���ŃX�v���C�g�̃p�^�[�������߂� */
			break;
		}
	}
	palNum = 0x0D;
	x = 90+16;
	y = 180+16 - Vibration;
	if(i <= 6)
	{
		y += 16;
		patNum = 0x70+i;
		V = 1;
		H = 0;
	}
	else if(i <= 12)
	{
		patNum = 0x70+12-i;
		V = 0;
		H = 0;
	}
	else
	{
		x += 16;
		patNum = 0x70+i-12;
		V = 0;
		H = 1;
	}
	
	SP_REGST( sp_num++, -1, x, y, SetBGcode(V, H, palNum, patNum), 3);

	/* �^�R���[�^�[ */
	x = 90+16;
	y = 180+16 - Vibration;
	V = 0;
	H = 0;
	palNum = 0x0D;

	patNum = 0x46;
	SP_REGST( sp_num++, -1, x + 0, y + 0, SetBGcode(V, H, palNum, patNum), 3);
	patNum = 0x47;
	SP_REGST( sp_num++, -1, x + 16, y + 0, SetBGcode(V, H, palNum, patNum), 3);
	patNum = 0x56;
	SP_REGST( sp_num++, -1, x + 0, y + 16, SetBGcode(V, H, palNum, patNum), 3);
	patNum = 0x57;
	SP_REGST( sp_num++, -1, x + 16, y + 16, SetBGcode(V, H, palNum, patNum), 3);

	return ret;
}

#endif	/* MyCar_C */

