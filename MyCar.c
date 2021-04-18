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
int16_t	g_speed = 0;
static int16_t g_Input;
static 

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
void MyCar_Background(void);
static int16_t	MyCar_Vibration(void);
static int16_t	MyCar_Mascot(int16_t);
static int16_t	MyCar_Tachometer(int16_t);

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
	
	MyCar_Background();	/* �w�i�̕\�� */

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
	ST_ROAD_INFO	stRoadInfo;
	
	GetRoadInfo(&stRoadInfo);
	Angle = stRoadInfo.angle;
	
	if((g_Input & KEY_RIGHT) != 0u)
	{
		if(g_stMyCar.ubOBD == OBD_SPIN_R)	/* �E�X�s�� */
		{
			ADPCM_Play(4);	/* �u���[�L�� */
		}
		ret = -30;	/* TorqueDW �p���X�e�쓮�� */

		g_stMyCar.Steering += g_speed << 4;	/* �E */
	}
	else if((g_Input & KEY_LEFT) != 0u)
	{
		if(g_stMyCar.ubOBD == OBD_SPIN_L)	/* ���X�s�� */
		{
			ADPCM_Play(4);	/* �u���[�L�� */
		}
		ret = -30;	/* TorqueDW �p���X�e�쓮�� */

		g_stMyCar.Steering -= g_speed << 4;	/* �� */
	}
	else
	{
		
	}
	
	/* �X�s�����̎ԗ����� */
	/* �X�s������ */
	if(g_stMyCar.ubOBD == OBD_SPIN_R)	/* �E�X�s�� */
	{
		g_stMyCar.Steering += g_speed << 3;	/* �E */
	}
	else if(g_stMyCar.ubOBD == OBD_SPIN_L)	/* ���X�s�� */
	{
		g_stMyCar.Steering -= g_speed << 3;	/* �� */
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

	/* �A�N�Z�� */
	if((g_Input & KEY_A) != 0u)		/* A�{�^�� */
	{
		if(g_stMyCar.ubThrottle < 255)
		{
			g_stMyCar.ubThrottle += 1;	/* �X���b�g���J�x */
		}
	}
	else
	{
		if(g_stMyCar.ubThrottle > 16)
		{
			g_stMyCar.ubThrottle -= 16;	/* �X���b�g���J�x */
		}
		else
		{
			g_stMyCar.ubThrottle = 0;	/* �X���b�g���J�x */
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
		ret = 30;	/* TorqueUP �}�X�^�[�o�b�N����̋�C */
		
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
	static uint8_t ubShiftPos_old = 0;
	static uint16_t uEngSound = 0u;

	GetStartTime(&time);	/* �J�n�������擾 */

	/* �V�t�g�`�F���W�ɂ��G���W����]���̍X�V */
	if(g_stMyCar.ubShiftPos != ubShiftPos_old) 
	{
		/* �ԑ�����G���W����]�����Z�o */
		g_stMyCar.uEngineRPM = (g_stMyCar.VehicleSpeed * uTM[g_stMyCar.ubShiftPos]) / 26;
	}
	ubShiftPos_old = g_stMyCar.ubShiftPos;
	
	/* �u���[�L���G���W����]���̍X�V */
	if(g_stMyCar.ubBrakeLights == TRUE)
	{
		/* �ԑ�����G���W����]�����Z�o */
		g_stMyCar.uEngineRPM = (g_stMyCar.VehicleSpeed * uTM[g_stMyCar.ubShiftPos] ) / 26u;	
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
	uTorque_Cal = (uint16_t)Mmax((int16_t)uTRQ[bAxis] + Input_Torque, 0);
	
	/* �A�N�Z�� */
	if(g_stMyCar.ubThrottle > 0)
	{
		if(g_stMyCar.ubShiftPos == 0u)
		{
			g_stMyCar.uEngineRPM += (uTM[g_stMyCar.ubShiftPos] + uTRQ[bAxis]);		/* ��]�� */
		}
		else
		{
			g_stMyCar.uEngineRPM += (uTM[g_stMyCar.ubShiftPos] + uTorque_Cal) >> 7;		/* ��]�� */
		}

		/* �G���W���̉� */
		{
			static	uint32_t	unExplosion_time = 0;
			int16_t	rpm;

			rpm	= Mmax(g_stMyCar.uEngineRPM, 1);
			
			if( (time - unExplosion_time) >  (150 - Mdiv64(rpm)) )	/* ��]���̃G���W����(60000 / rpm) */
			{
				unExplosion_time = time;
				if(uEngSound == 0u)
				{
					M_Play(rpm / 100);
	//				SE_Play_Fast(6);	/* FM���ʉ��Đ�(����) */
	//				SE_Play(7);	/* FM���ʉ��Đ� */
	//				SE_Play(6);	/* FM���ʉ��Đ� */
//					uEngSound = 8u;
				}
			}
			else
			{
				/* �������Ȃ� */
			}
		}
	}
	else{
		static	uint32_t	unFuelCut_time = 0;
		int16_t	rpm;
		rpm	= Mmax(g_stMyCar.uEngineRPM, 1);
		
		if(g_stMyCar.ubShiftPos == 0u)
		{
			g_stMyCar.uEngineRPM -= uTRQ[bAxis];	/* ��]�� */
		}
		else
		{
			g_stMyCar.uEngineRPM -= uTorque_Cal;	/* ��]�� */
		}
		
		/* �G���W���̉�(�R���J�b�g) */
		if( (time - unFuelCut_time) >  (150 - Mdiv64(rpm)) )	/* ��]���̃G���W����(60000 / rpm) */
		{
			unFuelCut_time = time;
			if(g_stMyCar.ubThrottle > 0)
			{
				if(uEngSound == 0u)
				{
					M_Play(rpm / 100);
	//				SE_Play(6);	/* FM���ʉ��Đ� */
	//				SE_Play(7);	/* FM���ʉ��Đ� */
//					uEngSound = 8u;
				}
			}
			else
			{
				if(uEngSound == 0u)
				{
					M_Play(rpm / 100);
	//				SE_Play(8);	/* FM���ʉ��Đ� */
//					uEngSound = 8u;
				}
			}
		}
	}
	uEngSound = Mdec((uint16_t)uEngSound, 1u);
	
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
						g_stMyCar.ubOBD = OBD_SPIN_R;	/* �E�X�s�� */
					}
					else
					{
						g_stMyCar.ubOBD = OBD_SPIN_L;	/* ���X�s�� */
					}
				}
			}
		}
	}
	
	/* �Փˌ�̎ԗ���ԍX�V */
	if( bHit == TRUE )
	{
		g_stMyCar.ubOBD = OBD_DAMAGE;	/* �̏� */
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
				color = 0x03;
				break;
			}
			case OBD_DAMAGE:
			{
				color = 0x13;
				break;
			}
			case OBD_SPIN_L:
			case OBD_SPIN_R:
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
	/* ������ */
	
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
	/* FPS */
#if 1	/* �e�L�X�g */
	PutGraphic_To_Text( 0, 0, 0			);	/* �C���e���A */
	PutGraphic_To_Text( 0, 0, Y_OFFSET	);	/* �C���e���A */

	T_Fill( 90,        0 + 180, 32, 31, 0, 0);		/* ���[�^�[���i��j */
	T_Fill( 90, Y_OFFSET + 180, 32, 31, 0, 0);		/* ���[�^�[���i���j */

	T_Fill(  0,        0 + 224, 255, 2, 0, 0x0F);	/* ���сi��j */
	T_Fill(  0, Y_OFFSET + 224, 255, 2, 0, 0x0F);	/* ���сi���j */
#else
	int16_t x;

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

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void MyCar_Background(void)
{
	int16_t ret;
	uint16_t height_sum = 0u;
	uint16_t height_sum_o = 0u;
	uint32_t i;
	uint32_t uWidth, uHeight, uFileSize;
	uint32_t uWidth_o, uHeight_o;
	uint32_t uOffset_X = 0u;
	
	ret = G_Load_Mem( ENEMYCAR_CG, uOffset_X,	0,	0);	/* ���C�o���� */
	Get_PicImageInfo( ENEMYCAR_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
	uWidth_o = uWidth;
	uHeight_o = uHeight;

	if(ret >= 0)
	{
		/* 11�p�^�[������� */
		uint32_t	uW_tmp, uH_tmp;
		
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
		uint32_t	uW_tmp, uH_tmp;
		
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

	int16_t	x, y;
	uint8_t	bMode;
	ST_CRT	stCRT;

	static	int16_t	Vibration = 0;
	static	int16_t	VibrationCT = 0;
	static	uint8_t	ubOBD_old = OBD_NORMAL;
	static	int16_t	CrashCount;
	
	GetGameMode(&bMode);
	GetCRT(&stCRT, bMode);

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
			if( g_stMyCar.ubOBD == OBD_DAMAGE )
			{
				ADPCM_Play(12);	/* SE�F�N���b�V�� */
				CrashCount = 30;
			}
			else if( g_stMyCar.ubOBD == OBD_COURSEOUT )
			{
				ADPCM_Play(11);	/* SE�F�R�[�X�O */
				CrashCount = 15;
			}
			else
			{
				/* �������Ȃ� */
			}
		}
		
		if( g_stMyCar.ubOBD == OBD_DAMAGE )
		{
			if(CrashCount == 22)	/* ����SE�� */
			{
				ADPCM_Play(15);	/* �X�L�[���� */
			}
			CrashCount = Mdec((int16_t)CrashCount, 1u);	/* �J�E���^�f�N�������g */

		}
		else if( g_stMyCar.ubOBD == OBD_COURSEOUT )
		{
			if((CrashCount % 2) == 0)	/* ����SE�� */
			{
				ADPCM_Play(11);	/* SE�F�R�[�X�O */
			}
			CrashCount = Mdec((int16_t)CrashCount, 1u);	/* �J�E���^�f�N�������g */
		}
		else
		{
			CrashCount = Mdec((int16_t)CrashCount, 1u);	/* �J�E���^�f�N�������g */
		}

		if(CrashCount == 0)
		{
			/* ���� */
			//MOV_Play(3);	/* �R������ */
			Vibration = 0;
		}
		else
		{
			Vibration = (VibrationCT == 0)?8:0;	/* ��ʂ̐U�� */
		}
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
	
	int16_t	x, y;
	uint16_t	nRatio = 0x80;
	uint8_t	palNum = 0;
	uint8_t	sp_num=0;
	
	volatile uint16_t *PCG_src  = (uint16_t *)0xEBA000;
	volatile uint16_t *PCG_dst  = (uint16_t *)0xEBA180;

	static	uint16_t	rad = 180;
	static	uint8_t	ubRadFlag = TRUE;

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

