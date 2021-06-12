#ifndef	MyCar_C
#define	MyCar_C

#include <stdio.h>
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

/* define��` */

/* �O���[�o���ϐ� */
int16_t	g_speed = 0;
static int16_t g_Input;
static int16_t g_SteeringDiff;

/* �O���[�o���f�[�^ */
uint16_t	uTM[6] = { 0, 2857, 1950, 1444, 1096, 761 };/* �ϑ���  1:2.857 2:1.95 3:1.444 4:1.096 5:0.761 */
//	uint16_t	uTM_F[6] = { 0, 13390, 9140, 6768, 5137, 3567 };/* �������� */
uint16_t	uRPM[11] = { 500, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000 };	/* Eng��]�� */
uint16_t	uTRQ[11] = { 120,  130,  150,  170,  210,  200,  180,  170,  140,  120,  100 };	/* �g���N */


/* �\���̒�` */
ST_CARDATA	g_stMyCar = {0};

/* �֐��̃v���g�^�C�v�錾 */
int16_t	MyCar_G_Load(void);
int16_t	GetMyCar(ST_CARDATA *stDat);
int16_t	SetMyCar(ST_CARDATA stDat);
int16_t	MyCarInfo_Init(void);
int16_t	MyCarInfo_Update(int16_t);
static int16_t	MyCar_Steering(void);
static int16_t	MyCar_ShiftPos(void);
static int16_t	MyCar_Accel(void);
static int16_t	MyCar_Brake(void);
static int16_t	MyCar_EngineSpeed(int16_t);
static int16_t	MyCar_Crash(void);
static int16_t	MyCar_VehicleSpeed(void);
int16_t	MyCar_Interior(uint8_t);
int16_t	MyCar_CourseOut(void);
int16_t	GetMyCarSpeed(int16_t *);
void MyCar_Image(void);
static int16_t	MyCar_Vibration(void);
static int16_t	MyCar_Mascot(int16_t);
static int16_t	MyCar_Tachometer(int16_t);
static int16_t	MyCar_SteeringPos(int16_t);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	MyCar_G_Load(void)
{
	int16_t	ret = 0;
	
	MyCar_Image();		/* ���Ԃ̕\�� */

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	GetMyCar(ST_CARDATA *stDat)
{
	int16_t	ret = 0;
	*stDat = g_stMyCar;
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetMyCar(ST_CARDATA stDat)
{
	int16_t	ret = 0;
	g_stMyCar = stDat;
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	MyCarInfo_Init(void)
{
	int16_t	ret = 0;
	
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
	g_stMyCar.ubOBD			= OBD_NORMAL;	/* �ԗ��̏�� */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	MyCarInfo_Update(int16_t input)
{
	int16_t	ret = 0;

	int16_t	Torque = 0;

	g_Input = input;				/* ���͒l�X�V */
	
	Torque += MyCar_Steering();		/* �X�e�A�����O���� */
	
	Torque += MyCar_ShiftPos();		/* �V�t�g���� */
	
	Torque += MyCar_Accel();		/* �A�N�Z������ */
	
	Torque += MyCar_Brake();		/* �u���[�L���� */
	
	Torque += MyCar_Crash();		/* �Փ˔��� */
	
	Torque += MyCar_CourseOut();	/* �R�[�X�A�E�g���̏��� */
	
	MyCar_EngineSpeed(Torque);		/* �G���W����]���̎Z�o */
	
	MyCar_VehicleSpeed();			/* �ԑ��̎Z�o */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	�g���N	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	MyCar_Steering(void)
{
	int16_t	ret = 0;

	int16_t	Angle;
	int16_t	SteeringDiff = 0;
	int16_t	ExtFctDiff = 0;
	int16_t	Steering_old;
	int16_t	AnalogMode = 0;

	ST_ROAD_INFO	stRoadInfo;
	JOY_ANALOG_BUF stAnalog_Info;
	
	AnalogMode = GetAnalog_Info(&stAnalog_Info);	/* �A�i���O���擾 */
	
	GetRoadInfo(&stRoadInfo);
	Angle = stRoadInfo.angle;
	Steering_old = g_stMyCar.Steering;
	
	if((g_Input & KEY_RIGHT) != 0u)
	{
		if((g_stMyCar.ubOBD & OBD_SPIN_R) != 0u)	/* �E�X�s�� */
		{
			g_stMyCar.ubOBD = Mbclr(g_stMyCar.ubOBD, OBD_SPIN_R);
			ADPCM_Stop();	/* ���ʉ���~ */
			ADPCM_Play(4);	/* �u���[�L�� */
		}
		ret -= 1;	/* TorqueDW �p���X�e�쓮�� */

		if(AnalogMode == 0)	/* �A�i���O���[�h */
		{
			SteeringDiff += (stAnalog_Info.r_stk_lr - 0x80);
		}
		else
		{
			SteeringDiff += Mmul16(g_speed);	/* �E */
		}
	}
	else if((g_Input & KEY_LEFT) != 0u)
	{
		if((g_stMyCar.ubOBD & OBD_SPIN_L) != 0u)	/* ���X�s�� */
		{
			g_stMyCar.ubOBD = Mbclr(g_stMyCar.ubOBD, OBD_SPIN_L);
			ADPCM_Stop();	/* ���ʉ���~ */
			ADPCM_Play(4);	/* �u���[�L�� */
		}
		ret -= 1;	/* TorqueDW �p���X�e�쓮�� */

		if(AnalogMode == 0)	/* �A�i���O���[�h */
		{
			SteeringDiff -= (0x80 - stAnalog_Info.r_stk_lr);
		}
		else
		{
			SteeringDiff -= Mmul16(g_speed);	/* �� */
		}
	}
	else
	{
		
	}
	g_SteeringDiff = SteeringDiff;
	
	/* �X�s�����̎ԗ����� */
	/* �X�s������ */
	if((g_stMyCar.ubOBD & OBD_SPIN_R) != 0u)	/* �E�X�s�� */
	{
		ret -= 10;	/* TorqueDW �X�s�� */
		
		ExtFctDiff += Mmul8(g_speed);	/* �E */
	}
	else if((g_stMyCar.ubOBD & OBD_SPIN_L) != 0u)	/* ���X�s�� */
	{
		ret -= 10;	/* TorqueDW �X�s�� */

		ExtFctDiff -= Mmul8(g_speed);	/* �� */
	}
	else
	{
		/* �ێ� */
	}
	
	/* �R�[�i�����O */
#if 1
	if( g_speed != 0u )
	{
		if(Angle != 0)
		{
			ret -= g_speed;	/* TorqueDW �n�[�t�X�s�� */
			
			ExtFctDiff += Angle * g_speed;	/* �o�����X�����v */
		}
//		else if(Angle < 0)
//		{
//			ExtFctDiff -= Angle * g_speed;	/* �o�����X�����v */
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
	
	/* �O���v�� */
	g_stMyCar.Steering = Steering_old + SteeringDiff + ExtFctDiff;
	/* �X�e�A�����O�N���b�v */
	g_stMyCar.Steering = Mmax(Mmin(g_stMyCar.Steering, 3800), -3800);
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	MyCar_ShiftPos(void)
{
	int16_t	ret = 0;

	static uint8_t bShiftPosFlag[3] = {FALSE};

	if( ((g_Input & KEY_A) != 0u) && ((g_Input & KEY_B) == 0u))	/* A�{�^��(only) */
	{
		/* �A�N�Z�������̓V�t�g����֎~ */
	}
	else{
		if(ChatCancelSW((g_Input & KEY_UPPER)!=0u, &bShiftPosFlag[0]) == TRUE)
		{
			if(g_stMyCar.ubShiftPos != 5)
			{
				ADPCM_Play(7);	/* SE:�V�t�g�A�b�v */
			}
			g_stMyCar.ubShiftPos = Mmin(g_stMyCar.ubShiftPos+1, 5);	/* �� */
		}
		
		if(ChatCancelSW((g_Input & KEY_LOWER)!=0u, &bShiftPosFlag[1]) == TRUE)
		{
			if(g_stMyCar.ubShiftPos != 0)
			{
				ADPCM_Play(7);	/* SE:�V�t�g�_�E�� */
			}
			g_stMyCar.ubShiftPos = Mmax(g_stMyCar.ubShiftPos-1, 0);	/* �� */
		}
	}
#if 0
	g_stMyCar.ubShiftPos = 2;	/* �e�X�g�p�ϑ��Œ� */
#endif	
	g_stMyCar.ubShiftPos = Mmax(Mmin(g_stMyCar.ubShiftPos, 5), 0);
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	MyCar_Accel(void)
{
	int16_t	ret = 0;
	
	static int8_t	bThrottleON_Count = 0;
	static int8_t	bThrottle_Flag = FALSE;

	/* �A�N�Z�� */
	if(bThrottle_Flag == FALSE)
	{
		if((g_Input & KEY_A) != 0u)		/* A�{�^������ */
		{
			bThrottle_Flag = TRUE;
			bThrottleON_Count = 8;
		}
	}

	if(bThrottle_Flag == TRUE)
	{
		if((g_Input & KEY_A) == 0u)		/* A�{�^������ */
		{
			ret -= 1;	/* TorqueDW �R���J�b�g */
			
			if(bThrottleON_Count > -8)
			{
				bThrottleON_Count -= 1u;
			}
		}
		
		g_stMyCar.ubThrottle += bThrottleON_Count;

		if(g_stMyCar.ubThrottle >= 240)
		{
			g_stMyCar.ubThrottle = 240;
		}
		
		if(g_stMyCar.ubThrottle < 8)
		{
			g_stMyCar.ubThrottle = 0u;
			bThrottle_Flag = FALSE;
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
static int16_t	MyCar_Brake(void)
{
	int16_t	ret = 0;

	if( (g_Input & KEY_B) != 0U )	/* B�{�^�� */
	{
		if(g_stMyCar.VehicleSpeed > 5)
		{
			ADPCM_Play(4);	/* �u���[�L�� */
		}
		ret += 10;	/* TorqueUP �}�X�^�[�o�b�N����̋�C */
		
		g_stMyCar.ubBrakeLights = TRUE;		/* �u���[�L�����v ON */
	}
	else
	{
		g_stMyCar.ubBrakeLights = FALSE;	/* �u���[�L�����v OFF */
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
static int16_t	MyCar_EngineSpeed(int16_t Input_Torque)
{
	int16_t	ret = 0;

	uint8_t	bAxis;
	uint32_t	i;
	uint32_t	time;
	uint16_t	uTorque_Cal;
	int16_t		rpm;
	int16_t		TargetRPM = 0;
	static uint8_t ubShiftPos_N = FALSE;
	static uint8_t ubShiftPos_UP = FALSE;
	static uint8_t ubShiftPos_old = 0;
	static	uint32_t	unExplosion_time = 0xFFFFFFFFu;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode;
	uint16_t	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
	GetStartTime(&time);	/* �J�n�������擾 */

	if(g_stMyCar.ubBrakeLights == TRUE)
	{
		ubShiftPos_N = TRUE;	/* �j���[�g�����|�W�V���� */
	}
	else if(g_stMyCar.ubShiftPos != ubShiftPos_old) 	/* �V�t�g�`�F���W�ɂ��G���W����]���̍X�V */
	{
		ubShiftPos_N = TRUE;	/* �j���[�g�����|�W�V���� */
	}
	else if(g_stMyCar.ubShiftPos == 0u)	/* �j���[�g�����|�W�V���� */
	{
		ubShiftPos_N = TRUE;	/* �j���[�g�����|�W�V���� */
	}
	else
	{
		/* �������Ȃ� */
	}
	ubShiftPos_old = g_stMyCar.ubShiftPos;	/* �O��l�X�V */
	
	if(ubShiftPos_N == TRUE)
	{
		TargetRPM = (g_stMyCar.VehicleSpeed * uTM[g_stMyCar.ubShiftPos]) / 26;	/* �ԑ�����G���W����]�����Z�o */

		if(TargetRPM < g_stMyCar.uEngineRPM)
		{
			ubShiftPos_UP = TRUE;
		}
		else
		{
			ubShiftPos_UP = FALSE;
		}
	}
	else
	{
		ubShiftPos_UP = FALSE;
	}
	
	/* ��]�����Z�o */
	bAxis = 10;
	for( i=0; i<10; i++ )
	{
		if( g_stMyCar.uEngineRPM <= (uRPM[i] + Mdiv2(uRPM[i+1] - uRPM[i])) )
		{
			bAxis = i;
			break;
		}
	}
	/* �ŏI�g���N */
	uTorque_Cal = (uint16_t)Mmax((int16_t)Mdiv256(uTRQ[bAxis] * g_stMyCar.ubThrottle) + Input_Torque, 1);
	g_stMyCar.ubWiper = (uint8_t)uTRQ[bAxis];	/* �� */
	g_stMyCar.bTire = (int8_t)uTorque_Cal;	/* �� */
	
	if(ubShiftPos_N == TRUE)	/* �j���[�g�����|�W�V���� */
	{
		if(g_stMyCar.ubShiftPos == 0u)	/* �j���[�g�����|�W�V���� */
		{
			if( g_stMyCar.ubThrottle > 0 )		/* �A�N�Z��ON�� */
			{
				g_stMyCar.uEngineRPM += (uTM[g_stMyCar.ubShiftPos] + uTRQ[bAxis]);		/* ��]�� */
			}
			else
			{
				if(g_stMyCar.uEngineRPM > uTRQ[bAxis])
				{
					g_stMyCar.uEngineRPM -= Mmax(uTRQ[bAxis], 0);	/* ��]�� */
				}
			}
			ubShiftPos_N = FALSE;
		}
		else
		{
			if(ubShiftPos_UP == TRUE)	/* �V�t�g�A�b�v�ɂ���]�����~ */
			{
				if(g_stMyCar.uEngineRPM > uTRQ[bAxis])
				{
					g_stMyCar.uEngineRPM -= Mmax(uTRQ[bAxis], 0);	/* ��]�� */
				}
				if(TargetRPM > g_stMyCar.uEngineRPM)
				{
					ubShiftPos_N = FALSE;
				}
			}
			else	/* �V�t�g�_�E���ɂ���]���㏸ */
			{
				g_stMyCar.uEngineRPM += (uTM[g_stMyCar.ubShiftPos] + uTRQ[bAxis]);		/* ��]�� */
				
				if(TargetRPM < g_stMyCar.uEngineRPM)
				{
					ubShiftPos_N = FALSE;
				}
			}
		}
	}
	else if( g_stMyCar.ubThrottle > 0 )		/* �A�N�Z��ON�� */
	{
		g_stMyCar.uEngineRPM += (uTM[g_stMyCar.ubShiftPos] + uTorque_Cal) >> 7;	/* ��]�� */
	}
	else	/* �A�N�Z��OFF */
	{
		if(g_stMyCar.uEngineRPM > uTorque_Cal)
		{
			g_stMyCar.uEngineRPM -= Mmax(uTorque_Cal, 100);	/* ��]�� */
		}
	}
	
	/* ��]���N���b�v */
	g_stMyCar.uEngineRPM = Mmax(Mmin(9000, g_stMyCar.uEngineRPM), 750);
	if(g_stMyCar.uEngineRPM >= 9000)
	{
		if(g_stMyCar.ubShiftPos == 0u)
		{
			g_stMyCar.uEngineRPM = 8200;
		}
		else
		{
			g_stMyCar.uEngineRPM = 8700;
		}
	}
	
	/* �G���W���̉� */
	{
		uint32_t uInterval;
		rpm	= Mmax(g_stMyCar.uEngineRPM, 1);
		uInterval = Mmax(Mmin( (40 - Mdiv256(rpm)), 5), 50);
//		uInterval = 10000 - rpm;
		
		if(g_stMyCar.ubThrottle > 0)	/* �A�N�Z��ON */
		{
			if( GetPassTime( uInterval, &unExplosion_time ) != 0u )	/* ��]���̃G���W����(60000 / rpm) */
	//		if( GetPassTime( 40, &unExplosion_time ) != 0u )	/* ��]���̃G���W����(60000 / rpm) */
			{
				M_Play(rpm);
			}
		}
		else	/* �A�N�Z��OFF */
		{
			if( GetPassTime( uInterval, &unExplosion_time ) != 0u )	/* ��]���̃G���W����(60000 / rpm) */
			{
				M_Play(rpm);
			}
		}
	}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	if(bDebugMode == TRUE)
	{
#if 0
		uint8_t	bMode;
		uint8_t	str[80];
		int16_t	x, y;
		ST_CRT	stCRT = {0};
		GetGameMode(&bMode);
		GetCRT(&stCRT, bMode);	/* ��ʏ����擾 */
		
		/* ���W�ݒ� */
		x = stCRT.hide_offset_x;
		y = stCRT.hide_offset_y + 128;
		sprintf(str, "Base(%3d), Input(%3d), Cal(%d)", uTRQ[bAxis], Input_Torque, uTorque_Cal);
		PutGraphic_To_Symbol(str, x, y, 0x03);	/* ���b�Z�[�W�G���A �`�� */
#endif
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
static int16_t	MyCar_Crash(void)
{
	int16_t	ret = 0;

	uint8_t	bHit = FALSE;
	int16_t	myCarSx, myCarEx, myCarSy, myCarEy;
	uint32_t	i;
	uint32_t	uEnemyNum = 0;

	ST_ENEMYCARDATA	stEnemyCar = {0};
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode;
	uint16_t	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
	/* �����蔻��̐��� */
	myCarSx = ROAD_CT_POINT + (g_stMyCar.Steering >> 8) - 8;
	myCarEx = myCarSx + 16;
	myCarSy = Y_MAX_WINDOW - 32;
	myCarEy = myCarSy + 16;

	
	/* �Փ˔��� */
	if(		(g_stMyCar.VehicleSpeed != 0) 		/* �ԑ����� */
		&&  (g_stMyCar.ubOBD == OBD_NORMAL) )	/* ���� */
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
						ADPCM_Play(15);	/* �X�L�[���� */
						g_stMyCar.ubOBD |= OBD_SPIN_R;	/* �E�X�s�� */
					}
					else
					{
						ADPCM_Play(15);	/* �X�L�[���� */
						g_stMyCar.ubOBD |= OBD_SPIN_L;	/* ���X�s�� */
					}
				}
			}
		}
	}
	
	/* �Փˌ�̎ԗ���ԍX�V */
	if( bHit == TRUE )
	{
		ADPCM_Play(12);	/* SE�F�N���b�V�� */
		g_stMyCar.ubOBD |= OBD_DAMAGE;	/* �̏� */
		g_stMyCar.uEngineRPM = g_stMyCar.uEngineRPM >> 1;		/* 1/2 */
	}

	/* �G�Ԃ̎ԑ� �X�V */
	if( bHit == TRUE )
	{
		GetEnemyCAR(&stEnemyCar, uEnemyNum);
		stEnemyCar.VehicleSpeed = g_stMyCar.VehicleSpeed + 5;
		SetEnemyCAR(stEnemyCar, uEnemyNum);	/* �G�Ԃ̍X�V */
	}
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	if(bDebugMode == TRUE)
	{
		uint16_t color;
		uint8_t	bMode;
		
		ST_CRT	stCRT;
		
		switch(g_stMyCar.ubOBD)
		{
			case OBD_NORMAL:
			{
				color = 0x0B;
				break;
			}
			case OBD_DAMAGE:
			{
				color = 0x0A;
				break;
			}
			case OBD_SPIN_L:
			case OBD_SPIN_R:
			{
				color = 0x0C;
				break;
			}
			case OBD_COURSEOUT:
			{
				color = 0x0E;
				break;
			}
			default:
			{
				color = 0x00;
				break;
			}
		}
		
		GetGameMode(&bMode);
		GetCRT(&stCRT, bMode);
		
		Draw_Box(
			stCRT.hide_offset_x + myCarSx,
			stCRT.hide_offset_y + myCarSy,
			stCRT.hide_offset_x + myCarEx,
			stCRT.hide_offset_y + myCarEy, color, 0xFFFF);
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
static int16_t	MyCar_VehicleSpeed(void)
{
	int16_t	ret = 0;

	int16_t	speed_min;
	int16_t VehicleSpeed_old, VS_sub;
	
	VehicleSpeed_old = g_stMyCar.VehicleSpeed;

	/* �ԑ� */
	if(g_stMyCar.ubShiftPos == 0u)		/* �j���[�g���� */
	{
		g_stMyCar.VehicleSpeed = Mdec((uint16_t)g_stMyCar.VehicleSpeed, 1u);	/* ���s��R�Ō��� */
	}
	else
	{
		/* �ϑ���  1:2.857 2:1.95 3:1.444 4:1.096 5:0.761 ������ 4.687 �^�C������2052.1mm */
		/* �^�C�������~�U�O�~��]���^�i�P�O�O�O�~�ϑ���~������j */
		g_stMyCar.VehicleSpeed = (int16_t)(((uint32_t)26 * g_stMyCar.uEngineRPM) / uTM[g_stMyCar.ubShiftPos]);
		/* �z�C���X�s�� */
		if(VehicleSpeed_old <= g_stMyCar.VehicleSpeed)
		{
			VS_sub = g_stMyCar.VehicleSpeed - VehicleSpeed_old;
		}
		else
		{
			VS_sub = 0;
		}
		if(VS_sub > 20)
		{
			g_stMyCar.VehicleSpeed = VehicleSpeed_old + 1;
		}
		
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
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	MyCar_Interior(uint8_t bMode)
{
	int16_t	ret = 0;

	int16_t	Vibration = 0;

	/* �ԑ̂�U�������� */
	Vibration = MyCar_Vibration();
	
	/* �}�X�R�b�g *//* �������� �� */
	MyCar_Mascot(Vibration);
	
	/* �^�R���[�^�[�j */
	MyCar_Tachometer(Vibration);
	
	/* �n���h�� */
	MyCar_SteeringPos(Vibration);
	
	return ret;
}


/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	MyCar_CourseOut(void)
{
	int16_t	ret = 0;
	int16_t	vx;
	vx	= g_stMyCar.Steering;

	if( Mabs(vx) > 2800 )	/* �R�[�X�O */
	{
		int16_t	rpm	= g_stMyCar.uEngineRPM;
		
		g_stMyCar.uEngineRPM -= (rpm>>3);	/* �������� */

		g_stMyCar.ubOBD |= OBD_COURSEOUT;	/* �R�[�X�A�E�g */

//		ADPCM_Play(11);	/* SE�F�R�[�X�O */

	}
	else
	{
		if((g_stMyCar.ubOBD & OBD_COURSEOUT) != 0u)
		{
			g_stMyCar.ubOBD = Mbclr(g_stMyCar.ubOBD, OBD_COURSEOUT);	/* �R�[�X�A�E�g���� */
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
int16_t	GetMyCarSpeed(int16_t *speed)
{
	int16_t	ret = 0;
	
	*speed = g_speed;

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void MyCar_Image(void)
{
	int32_t i;
	int32_t x = 0, y = 0;
	
	/* FPS */
	for(i=0; i < MYCAR_IMAGE_MAX; i++)
	{
		uint32_t	uWidth, uHeight, uFileSize;
		uint16_t	uCG_Num;
		
		uCG_Num = MYCAR_CG + i;
		CG_File_Load(uCG_Num);	/* �O���t�B�b�N�̓ǂݍ��� */

		Get_PicImageInfo( uCG_Num, &uWidth, &uHeight, &uFileSize );	/* �摜�̏����擾 */
		
		/* �e�L�X�g */
		PutGraphic_To_Text( uCG_Num, uWidth * x, Y_OFFSET * y );			/* �C���e���A */

//		T_Fill( (uWidth * x) + 82, (Y_OFFSET * y) + 188, 32, 31, 0, 0);	/* ���[�^�[�� */
		T_Circle( 82 + (uWidth * x), 188 + (Y_OFFSET * y), 32, 32, 0, 0);	/* ���[�^�[�� */

#ifdef DEBUG
//		T_Fill( (uWidth * x) + (16 * i), (Y_OFFSET * y) + 164, 16, 16, 0, 0);	/* �f�o�b�O�p */
#endif
		T_Fill( uWidth * x, (Y_OFFSET * y) + 224, 255, 2, 0, 0x0F);		/* ���� */
		
		x++;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	MyCar_Vibration(void)
{
	int16_t ret = 0;

	static	int16_t	Vibration = 0;
	static	int16_t	VibrationCT = 0;
	static	uint8_t	ubOBD_old = OBD_NORMAL;
	static	int16_t	CrashCount;

	/* ��ʂ�h�炷 */
	VibrationCT++;
	if(VibrationCT > 5)
	{
		VibrationCT = 0;
	}
	
	if(g_stMyCar.ubOBD == OBD_NORMAL)
	{
		Vibration = (VibrationCT == 0)?1:0;	/* ��ʂ̐U�� */
	}
	else
	{
		if(ubOBD_old == OBD_NORMAL)
		{
			if( (g_stMyCar.ubOBD & OBD_DAMAGE) != 0u )
			{
				CrashCount = 30;
			}
			else if( (g_stMyCar.ubOBD & OBD_COURSEOUT) != 0u )
			{
				CrashCount = 15;
			}
			else
			{
				/* �������Ȃ� */
			}
		}

		if(CrashCount == 0)
		{
			g_stMyCar.ubOBD = Mbclr(g_stMyCar.ubOBD, (OBD_DAMAGE|OBD_COURSEOUT));
			Vibration = 0;
		}
		else
		{
			CrashCount = Mdec((int16_t)CrashCount, 1u);	/* �J�E���^�f�N�������g */
			
			Vibration = (VibrationCT == 0)?8:0;	/* ��ʂ̐U�� */
		}
	}
	ubOBD_old = g_stMyCar.ubOBD;	/* �O��l�X�V */
	
	ret = Vibration;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	MyCar_Mascot(int16_t Vibration)
{
	int16_t ret = 0;

	uint16_t	nRatio = 0x80;
#if 0
	int16_t	x, y;
	uint8_t	palNum = 0;
	uint8_t	sp_num=0;
	
	volatile uint16_t *PCG_src  = (uint16_t *)0xEBA000;
	volatile uint16_t *PCG_dst  = (uint16_t *)0xEBA180;

	static	uint16_t	rad = 180;
	static	uint8_t	ubRadFlag = TRUE;
#endif

#ifdef	DEBUG
	GetDebugNum(&nRatio);
#endif

#if 0
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
		uint16_t	width;
		
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

	PCG_Rotation((uint16_t *)PCG_dst, (uint16_t *)PCG_src, 3, 3, x, y, &sp_num, palNum, (nRatio-0x80), 180-rad);

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
static int16_t	MyCar_Tachometer(int16_t Vibration)
{
	int16_t ret = 0;
	
	int16_t	i;
	uint8_t	patNum = 0;
	uint8_t	palNum = 0;
	uint8_t	V=0, H=0;
	uint8_t	sp_num=0;
	int16_t	x, y;

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
	x = 82+16;
	y = 188+16 - Vibration;
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
	x = 82+16;
	y = 188+16 - Vibration;
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

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static int16_t	MyCar_SteeringPos(int16_t Vibration)
{
	int16_t ret = 0;

	uint8_t	patNum = 0;
	uint8_t	palNum = 0;
	uint8_t	V=0, H=0;
	uint8_t	sp_num=5;
	int16_t	x, y;
	uint32_t	uWidth, uHeight, uFileSize;
	ST_TEXTINFO	stTextInfo;

	Get_PicImageInfo( MYCAR_CG, &uWidth, &uHeight, &uFileSize );	/* �摜�̏����擾 */
	
	/* �X�e�A�����O�ʒu */
	x = 16 + Mdiv16(g_SteeringDiff) + 16 - 2;
	y = 96 + Mabs(Mdiv16(g_SteeringDiff)) + 16 + 2;
	V = 0;
	H = 0;
	palNum = 0x0C;
	patNum = 0x4A;
	SP_REGST( sp_num++, -1, x + 0, y + 0, SetBGcode(V, H, palNum, patNum), 3);
	
	/* �X�e�A�����O */
	x = 0+16;
	y = 96+16;
	V = 0;
	H = 0;
	palNum = 0x0B;
	patNum = 0x48;
	SP_REGST( sp_num++, -1, x + 0, y + 0, SetBGcode(V, H, palNum, patNum), 3);
	patNum = 0x49;
	SP_REGST( sp_num++, -1, x + 16, y + 0, SetBGcode(V, H, palNum, patNum), 3);
	patNum = 0x58;
	SP_REGST( sp_num++, -1, x + 0, y + 16, SetBGcode(V, H, palNum, patNum), 3);
	patNum = 0x59;
	SP_REGST( sp_num++, -1, x + 16, y + 16, SetBGcode(V, H, palNum, patNum), 3);

	/* ��ʂ�؂�ւ��� */
	if(g_SteeringDiff == 0)
	{
		x = 1;
		y = 0;
	}
	else if(g_SteeringDiff < 0)
	{
		if(Mabs(g_SteeringDiff) > 0x40)
		{
			x = 0;
			y = 0;
		}
		else
		{
			x = 1;
			y = 0;
		}
	}
	else
	{
		if(Mabs(g_SteeringDiff) > 0x40)
		{
			x = 2;
			y = 0;
		}
		else
		{
			x = 1;
			y = 0;
		}
	}

	/* �e�L�X�g��ʂ̃|�W�V�������X�V���� */
	T_Get_TextInfo(&stTextInfo);
	stTextInfo.uPosX = x;	/* X���W */
	stTextInfo.uPosY = y;	/* Y���W */
	T_Set_TextInfo(stTextInfo);
	
	return ret;
}

#endif	/* MyCar_C */

