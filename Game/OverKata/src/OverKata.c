#ifndef	OVERKATA_C
#define	OVERKATA_C

#include <iocslib.h>
#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>
#include <io.h>
#include <math.h>
#include <interrupt.h>

#include "inc/usr_macro.h"
#include "inc/apicglib.h"

#include "OverKata.h"

#include "APL_MACS.h"
#include "APL_Math.h"
#include "CRTC.h"
#include "Course_Obj.h"
#include "DMAC.h"
#include "Draw.h"
#include "EnemyCAR.h"
#include "FileManager.h"
#include "Graphic.h"
#include "Input.h"
#include "MFP.h"
#include "Music.h"
#include "MyCar.h"
#include "Output_Text.h"
#include "PCG.h"
#include "Raster.h"
#include "Score.h"
#include "Task.h"
#include "Text.h"
#include "Trap14.h"

#define	_DEBUG_MODE	/* �f�o�b�O����Ȃ�R�����g�A�E�g���O�� */

#define	BG_SKIP_TIME	(8u)
#define	BG_TIME_OUT		(56u)

/* �O���[�o���ϐ� */
int32_t	g_nSuperchk = 0;
int32_t	g_nIntLevel = 0;
int32_t	g_nIntCount = 0;
int32_t	g_nCrtmod = 0;
int32_t	g_nMaxMemSize;
int16_t	g_CpuTime = 0;
int16_t	g_Stage;
uint8_t	g_mode;
uint8_t	g_mode_rev;
uint8_t	g_Vwait = 1;
uint32_t	g_unTime_cal = 0u;
uint32_t	g_unTime_cal_PH = 0u;
uint8_t	g_bFlip = FALSE;
uint8_t	g_bFlip_old = FALSE;
uint8_t	g_bExit = TRUE;

static uint16_t s_uStartCount = 0;
static uint16_t s_uGoalCount = 0;

volatile uint16_t	g_uGameStatus;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
uint8_t		g_bDebugMode = FALSE;
uint8_t		g_bFPS_PH = 0u;
uint16_t	g_uDebugNum = 0; 
uint32_t	g_unTime_Pass[MAX_G] = {0u};
int16_t		g_DebugPosX, g_DebugPosY;
int16_t		g_DebugHis[10];
#endif

uint16_t	uRoad = 0;
int16_t		Update = 0;
static int16_t Torque = 0xFFFF;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	int16_t dst_x, dst_y;
	int16_t	x_min, y_min;
	int16_t	x_max, y_max;
	uint16_t src_w, src_h;
	uint8_t	bFlag;
#endif

/* enum */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
enum{
	DEBUG_NONE,
	DEBUG_COURSE_OBJ,
	DEBUG_ENEMYCAR,
	DEBUG_MYCAR,
	DEBUG_ROAD,
	DEBUG_RASTER,
	DEBUG_INPUT,
	DEBUG_CPUTIME,
	DEBUG_MEMORY,
	DEBUG_FREE,
	DEBUG_MAX,
};
#endif

/* �O���[�o���\���� */

/* �֐��̃v���g�^�C�v�錾 */
int16_t main(int16_t, int8_t**);
static void App_Init(void);
static void App_exit(void);
int16_t	BG_main(uint32_t);
int16_t	FlipProc(void);
int16_t	SetFlip(uint8_t);
void Set_DI(void);
void Set_EI(void);
int16_t	GetGameMode(uint8_t *);
int16_t	SetGameMode(uint8_t);

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
int16_t	GetDebugNum(uint16_t *);
int16_t	SetDebugNum(uint16_t);
int16_t	GetDebugMode(uint8_t *);
int16_t	SetDebugMode(uint8_t);
int16_t	GetDebugPos(int16_t *, int16_t *);
int16_t	SetDebugPos(int16_t, int16_t);
int16_t	GetDebugHis(int16_t *, int16_t);
int16_t	SetDebugHis(int16_t);
void Debug_View(uint16_t);
#endif

void (*usr_abort)(void);	/* ���[�U�̃A�{�[�g�����֐� */

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t main(int16_t argc, int8_t** argv)
{
	int16_t	ret = 0;

	uint32_t	unTime_cal = 0u;
	uint32_t	unTime_cal_PH = 0u;
	uint32_t	unPass_time = 0xFFFFFFFFu;
	int16_t		DemoCount = 0;

	uint16_t	uFreeRunCount=0u;

	int16_t	loop = 1;
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode = TRUE;
	uint8_t	bDebugMode_flag;
	uint32_t	unDebugCounter1 = 0u;
	uint32_t	unDebugCounter2 = 0u;
#endif
	
	uint8_t	bCRTMode = TRUE;
	uint8_t	bCRTMode_flag;

	uint8_t	bMode;
	
	ST_TASK		stTask = {0}; 
	
	/* �ϐ��̏����� */
	SetGameMode(1);
	g_Stage = 1;
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	g_uDebugNum = 0x80;
	bDebugMode = TRUE;
#endif

	
	
	/* ��O�n���h�����O���� */
	usr_abort = App_exit;	/* ��O�����ŏI�����������{���� */
	init_trap14();			/* �f�o�b�O�p�v���I�G���[�n���h�����O */
#if 0	/* �A�h���X�G���[���� */
	{
		char buf[10];
		int *A = (int *)&buf[1];
		int B = *A;
		return B;
	}
#endif

	App_Init();	/* ������ */
	
	SetTaskInfo(SCENE_INIT);	/* �������V�[���֐ݒ� */
	
	/* ���� */
	srandom(TIMEGET());	/* �����̏����� */
	
	do	/* ���C�����[�v���� */
	{
		uint32_t time_st;
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		uint32_t time_now;
		uint8_t	bTimePass = 0;
#endif
		/* �I������ */
		if(loop == 0)
		{
			break;
		}
		
		/* �����ݒ� */
		GetNowTime(&time_st);	/* ���C�������̊J�n�������擾 */
		SetStartTime(time_st);	/* ���C�������̊J�n�������L�� */
		
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		if(g_bDebugMode == TRUE)
		{
			g_unTime_Pass[0] = time_st;
			bTimePass++;
		}
#endif
		/* �^�X�N���� */
		TaskManage();				/* �^�X�N���Ǘ����� */
		GetTaskInfo(&stTask);		/* �^�X�N�̏��𓾂� */

		/* ���[�h�����n�� */
		GetGameMode(&bMode);
		
		/* �I�� */
		if((g_Input & KEY_b_ESC ) != 0u)	/* �d�r�b�L�[ */
		{
			SetTaskInfo(SCENE_EXIT);	/* �I���V�[���֐ݒ� */
		}

#if 1
		/* CRT 31kHz/15kHz�ؑ� */
		if(ChatCancelSW((g_Input & KEY_b_HELP)!=0u, &bCRTMode_flag) == TRUE)	/* HELP��31kHz/15kHz�ؑ� */
		{
			if(bCRTMode == TRUE)
			{
				CRTC_INIT(0);	/* 31kHz */
				bCRTMode = FALSE;
			}
			else
			{
				CRTC_INIT(1);	/* 15kHz */
				bCRTMode = TRUE;
			}
		}
#endif
		
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		if(ChatCancelSW((g_Input & KEY_b_SP)!=0u, &bDebugMode_flag) == TRUE)	/* �X�y�[�X�Ńf�o�b�OON/OFF */
		{
			if(bDebugMode == FALSE)	bDebugMode = TRUE;
			else					bDebugMode = FALSE;
		}
		SetDebugMode(bDebugMode);
		
		DirectInputKeyNum(&g_uDebugNum, 3);	/* �L�[�{�[�h���琔������� */
#endif
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		if(g_bDebugMode == TRUE)
		{
			GetNowTime(&time_now);
			g_unTime_Pass[bTimePass] = Mmax(time_now - g_unTime_Pass[0], g_unTime_Pass[bTimePass]);	/* 1 */
			bTimePass++;
			g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
		}
#endif
		
		switch(stTask.bScene)
		{
			case SCENE_INIT:	/* �������V�[�� */
			{
				/* �ϐ��̏����� */
				SetGameMode(1);
				SetGameMode(2);
				
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				g_uDebugNum = 0x80;
#endif
				uFreeRunCount = 0;
				
				/* �X�v���C�g���a�f */
				PCG_VIEW(0x00u);	/* �X�v���C�g���a�f��\�� */
				/* �e�L�X�g */
				T_Clear();	/* �e�L�X�g�N���A */
				T_PALET();	/* �e�L�X�g�p���b�g�ݒ� */

#ifndef	_DEBUG_MODE	/* �f�o�b�O����Ȃ� */
				/* �����[�X */
				SetTaskInfo(SCENE_TITLE_S);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
#else
				/* �f�o�b�O�R�[�i�[ */
				SetTaskInfo(SCENE_DEBUG_S);	/* �f�o�b�O(�J�n����)�֐ݒ� */
#endif
				break;
			}
			case SCENE_DEBUG_S:
			{
				puts("�f�o�b�O�R�[�i�[ �J�n");

				T_Clear();			/* �e�L�X�g�N���A */
				
				/* ���X�^�[���̏����� */
				Raster_Init();
				
				/* �R�[�X�f�[�^�̏����� */
				Road_Init(g_Stage);
				
				/* �Q�[�����e�̏����� */
				S_Init_Score();	/* �X�R�A�̏����� */
				
				/* �X�v���C�g���a�f�\�� */
				PCG_INIT();		/* �X�v���C�g�^�a�f�̏����� */
				PCG_VIEW(0x07u);	/* �X�v���C�g���a�f�\�� */

				/* ���� */
				//MyCar_G_Load();		/* ���Ԃ̉摜�ǂݍ��� */
				MyCarInfo_Init();	/* ���Ԃ̏�񏉊��� */
				
				/* �O���t�B�b�N�\�� */
				G_HOME(0);			/* �z�[���ʒu�ݒ� */
				G_CLR();			/* �O���t�B�b�N�N���A */
				G_PaletteSetZero();	/* �O���t�B�b�N�p���b�g�̊�{�ݒ� */
				
				Music_Play(14);	/* ���[�f�B���O�� */
				
				/* ���C�o���Ԃ̏����� */
				InitEnemyCAR();
				
				//G_Load_Mem( START_PT_CG, 0, 0, 0 );	/* �X�^�[�g�Q�[�g */
				//G_Load_Mem( GOAL_PT_CG, 0, 0, 0 );	/* �X�^�[�g�Q�[�g */
				/* �R�[�X�̃I�u�W�F�N�g�̏����� */
				//InitCourseObj();
				
				/* �R�[�X�̔w�i */
				Road_BG_Init(1);

				Debug_View(uFreeRunCount);	/* �f�o�b�O���\�� */
				
				/* �Q�[���X�^�[�g�Ɋւ���ϐ��̏����� */
				s_uStartCount = 0;
				s_uGoalCount = 0;
				g_uGameStatus = 0;	/* �Q�[���� */
				
				SetTaskInfo(SCENE_DEBUG);	/* �f�o�b�O(����)�֐ݒ� */
				break;
			}
			case SCENE_DEBUG:
			{
				Torque = 0;
				Update = 0;
				
				if( (g_bFlip_old == TRUE) && (g_bFlip == FALSE) )	/* �؂�ւ����㔻�� */
				{
					/* ���Ԃ̏��� */
					Update |= MyCarInfo_Update(g_Input, &Torque);
				}
				
				if(stTask.b16ms == TRUE)
				{
					Update |= MyCarInfo_Update16ms(Torque);
				}
				
				if( (g_bFlip_old == TRUE) && (g_bFlip == FALSE) )	/* �؂�ւ����㔻�� */
				{
					/* ���X�^�[���� */
					Update |= Road_Pat_Main(&uRoad);	/* �R�[�X�f�[�^�̍X�V */

					g_uGameStatus = 1;	/* �Q�[���� */

					Update |= Raster_Main();	/* �R�[�X�̏��� */
				}
				
				/* �]�������Ԃŏ��� */
				BG_main(time_st);	/* �o�b�N�O�����h���� */
				
				g_Vwait = 0;	/* No Wait */
				
				break;
			}
			case SCENE_DEBUG_E:
			{
				puts("�f�o�b�O�R�[�i�[ �I��");

				loop = 0;	/* ���[�v�I�� */

				break;
			}
			case SCENE_TITLE_S:	/* �^�C�g���V�[��(�J�n����) */
			{
				SetFlip(FALSE);			/* �t���b�v�֎~ */
				
				Music_Play(2);	/* �^�C�g���� */
				
				/* �O���t�B�b�N */
				G_HOME(1);				/* �z�[���ʒu�ݒ� */
				G_Load_Mem( TITLE_CG, X_OFFSET, 0, 0 );	/* �^�C�g���摜 */
			
				/* �e�L�X�g */
				T_Clear();		/* �e�L�X�g�N���A */
				
				BG_TextPut("OVER KATA", 96, 128);		/* �^�C�g������ */
				BG_TextPut("PUSH A BUTTON", 80, 160);	/* �{�^�������Ă� */

				GetPassTime( 100, &unPass_time );		/* ������ */

				SetTaskInfo(SCENE_TITLE);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_TITLE:	/* �^�C�g���V�[�� */
			{
				int8_t	sStageMes[256] = {0};
				
				if(g_Input == KEY_A)	/* A�{�^�� */
				{
					Music_Stop();	/* ���y�Đ� ��~ */
					
					ADPCM_Play(10);	/* SE:���� */
				
					GetPassTime( 100, &unPass_time );		/* ������ */
					
					SetTaskInfo(SCENE_TITLE_E);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
				}
				
				if((g_Input & KEY_LEFT) != 0u)	/* �� */
				{
					GetPassTime( 100, &unPass_time );		/* ������ */
					g_Stage = Mmax(g_Stage--, 1);
					ADPCM_Play(4);	/* SE:�� */
				}
				else if((g_Input & KEY_RIGHT) != 0u)	/* �E */
				{
					GetPassTime( 100, &unPass_time );		/* ������ */
					g_Stage = Mmin(g_Stage++, 9);
					ADPCM_Play(4);	/* SE:�E */
				}
				else
				{
				}
				g_Vwait = 1;	/* Wait 1 */
				
				sprintf(sStageMes, "COURSE%d", g_Stage);
				BG_TextPut(sStageMes, 104, 144);	/* �X�e�[�W */
				
				
				if( GetPassTime( 30000, &unPass_time ) != 0u )	/* ���莞�ԉ����Ȃ���΃f���V�[���� */
				{
					Music_Stop();	/* ���y�Đ� ��~ */
					
					SetTaskInfo(SCENE_DEMO_S);	/* �f���V�[��(�J�n����)�֐ݒ� */
				}
				break;
			}
			case SCENE_TITLE_E:	/* �^�C�g���V�[��(�I������) */
			{
				/* �X�v���C�g���a�f */
				PCG_VIEW(0x00u);	/* �X�v���C�g���a�f��\�� */

				/* �e�L�X�g�\�� */
				T_Clear();	/* �e�L�X�g�N���A */

				/* �O���t�B�b�N�\�� */
				G_CLR();		/* �O���t�B�b�N�N���A */

				SetTaskInfo(SCENE_START_S);	/* �Q�[���J�n�V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_DEMO_S:	/* �f���V�[��(�J�n����) */
			{
				uint16_t demo;
				DemoCount = 0;
				
				/* �X�v���C�g���a�f */
				PCG_VIEW(0x00u);	/* �X�v���C�g���a�f��\�� */

				/* �e�L�X�g�\�� */
				T_Clear();	/* �e�L�X�g�N���A */

				/* �O���t�B�b�N�\�� */
				G_CLR();		/* �O���t�B�b�N�N���A */
				
				Music_Play(6);	/* �f���� */
				
				for(demo=0; demo<6; demo++)
				{
					G_Load_Mem(DEMO_CG + demo, X_OFFSET, Y_OFFSET + 32, 0);	/* �O���t�B�b�N�̓ǂݍ��� */
//					CG_File_Load(DEMO_CG + demo);		/* �O���t�B�b�N�̓ǂݍ��� */
				}

				SetTaskInfo(SCENE_DEMO);	/* �Q�[���J�n�V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_DEMO:	/* �f���V�[�� */
			{
				if( GetPassTime( 2000, &unPass_time ) != 0u )	/* ���莞�ԉ����Ȃ���Ύ��s */
				{
					if( DemoCount < 6u )	/* �f���V�[�P���X���� */
					{
						G_Load_Mem( DEMO_CG + DemoCount, X_OFFSET, 32, 0 );	/* �f���摜 */
					}
					DemoCount++;	/* ���̉摜 */
				}
				
				if( (DemoCount >= 10u) || /* �f���V�[�P���X���� */
					(g_Input == KEY_A) )	/* A�{�^�� */
				{
					Music_Stop();	/* ���y�Đ� ��~ */
				
					SetTaskInfo(SCENE_DEMO_E);	/* �f���V�[��(�I������)�֐ݒ� */
				}
				break;
			}
			case SCENE_DEMO_E:	/* �f���V�[��(�I������) */
			{
				/* �X�v���C�g���a�f */
				PCG_VIEW(0x00u);	/* �X�v���C�g���a�f��\�� */

				/* �O���t�B�b�N�\�� */
				G_CLR();		/* �O���t�B�b�N�N���A */
				
				/* �e�L�X�g�\�� */
				T_Clear();	/* �e�L�X�g�N���A */

				SetTaskInfo(SCENE_TITLE_S);	/* �Q�[���X�^�[�g�^�X�N�֐ݒ� */
				break;
			}
			case SCENE_START_S:	/* �Q�[���J�n�V�[��(�J�n����) */
			{
				Music_Stop();	/* ���y�Đ� ��~ */
				
				/* �X�v���C�g���a�f */
				PCG_VIEW(0x00u);	/* �X�v���C�g���a�f��\�� */

				/* �O���t�B�b�N�\�� */
				G_CLR();		/* �O���t�B�b�N�N���A */
				
				/* �e�L�X�g�\�� */
				T_Clear();	/* �e�L�X�g�N���A */
				
				/* ���� */
				if(g_uGameStatus == 0)
				{
					MOV_Play(0);	/* �X�^�[�g */
				}
				else
				{
					MOV_Play(3);	/* �E�\������ */
				}

				//Music_Play(10);	/* TRUTH */
				Music_Play(11);	/* Mach-1 */
				
				/* ���X�^�[���̏����� */
				Raster_Init();
				
				/* �R�[�X�f�[�^�̏����� */
				Road_Init(g_Stage);
				
				SetTaskInfo(SCENE_START);	/* �Q�[���X�^�[�g�^�X�N�֐ݒ� */
				break;
			}
			case SCENE_START:	/* �Q�[���J�n�V�[�� */
			{
				/* �R�[�X��`�悷�� */
				if(g_Input == KEY_B)	/* B�{�^�� */
				{
					g_Vwait = 0;	/* No Wait */
				}
				else
				{
					g_Vwait = 1;	/* Wait 1 */
				}
				
				if( Road_Map_Draw(0) < 0 )	/* �R�[�X�`�抮������ */
				{
					g_Vwait = 1;	/* Wait 1 */
					SetTaskInfo(SCENE_START_E);	/* �Q�[���X�^�[�g�^�X�N�֐ݒ� */
				}
				break;
			}
			case SCENE_START_E:	/* �Q�[���J�n�V�[��(�I������) */
			{
				Music_Play(1);	/* ���[�f�B���O�� */

				Set_CRT_Contrast(0);	/* �R���g���X�g�� */
				
				/* �e�L�X�g�\�� */
				T_Clear();			/* �e�L�X�g�N���A */
				
				/* �Q�[�����e�̏����� */
				S_Init_Score();	/* �X�R�A�̏����� */
				
				/* �X�v���C�g���a�f�\�� */
				PCG_INIT();		/* �X�v���C�g�^�a�f�̏����� */
				PCG_VIEW(0x07u);	/* �X�v���C�g���a�f�\�� */

				/* ���� */
				MyCar_G_Load();		/* ���Ԃ̉摜�ǂݍ��� */
				MyCarInfo_Init();	/* ���Ԃ̏�񏉊��� */
				
				/* ���C�o���Ԃ̏����� */
				InitEnemyCAR();

				/* �R�[�X�̃I�u�W�F�N�g�̏����� */
				InitCourseObj();
				
				/* �R�[�X�̔w�i */
				Road_BG_Init(1);
				
				G_Load_Mem( START_PT_CG, 0, 0, 0 );	/* �X�^�[�g�Q�[�g */
				G_Load_Mem( GOAL_PT_CG, 0, 0, 0 );	/* �S�[���Q�[�g */
				
				/* �e�L�X�g�\�� */
				T_PALET();		/* �e�L�X�g�p���b�g�ݒ� */
				T_TopScore();	/* TOP */
				T_Time();		/* TIME */
				T_Score();		/* SCORE */
				T_Speed();		/* SPEED */
				T_Gear();		/* GEAR */
				T_SetBG_to_Text();	/* �e�L�X�g�p��Ɨp�f�[�^�W�J */
				T_TimerStop();	/* �^�C�}�[��~ */
				T_TimerReset();	/* �^�C�}�[���Z�b�g */
				
				/* �O���t�B�b�N�\�� */
				G_HOME(0);			/* �z�[���ʒu�ݒ� */
				G_CLR();			/* �O���t�B�b�N�N���A */
				G_PaletteSetZero();	/* �O���t�B�b�N�p���b�g�̊�{�ݒ� */
				
				Debug_View(uFreeRunCount);	/* �f�o�b�O���\�� */
				
				Music_Stop();	/* ���y�Đ� ��~ */
				
				g_uGameStatus = 0;	/* �Q�[���� */
				
				SetTaskInfo(SCENE_GAME_S);	/* �Q�[��(�J�n����)�^�X�N�֐ݒ� */
				break;
			}
			case SCENE_GAME_S:	/* �Q�[���V�[���J�n���� */
			{
				int16_t	i;
				int16_t	x, y;
				static int16_t	sig_count = 0;

				ST_PCG	*p_stPCG = NULL;

				g_uGameStatus = 2;	/* �X�^�[�g��� */
				
				if(sig_count == 0)
				{
					sig_count = 1;
					
					Music_Play(3);	/* ���C��BGM */
					M_SetMusic(0);	/* ���ʉ��Đ��̐ݒ� */

					Set_CRT_Contrast(-1);	/* �R���g���X�gdef */
					
					/* ���Ԃ̏����擾 */
					MyCarInfo_Update(g_Input, &Torque);
					MyCarInfo_Update16ms(Torque);
					
					/* ���X�^�[���� */
					Road_Pat_Main(&uRoad);	/* �R�[�X�f�[�^�̍X�V */
					Raster_Main();			/* �R�[�X�̏��� */
					
					/* �Q�[���X�^�[�g�Ɋւ���ϐ��̏����� */
					s_uStartCount = 0;
					s_uGoalCount = 0;
				}
				else
				{
					MyCarInfo_Update16ms(0);
				}

				for(i=0; i<3; i++)
				{
					p_stPCG = PCG_Get_Info(ROAD_PCG_SIGNAL_1+i);	/* �V�O�i�������v */
					/* �V�O�i�������v */
					x =  104 + (i * 16) + SP_X_OFFSET;
					y =  72 + SP_Y_OFFSET;
					
					if(p_stPCG != NULL)
					{
						p_stPCG->x = x;
						p_stPCG->y = y;
						if(sig_count < 5)
						{
							p_stPCG->update	= TRUE;
						}
						else
						{
							p_stPCG->update	= FALSE;
						}
					}
				}
				
				if( GetPassTime( 1000, &unPass_time ) != 0u )	/* ���莞�ԉ����Ȃ���Ύ��s */
				{
					switch(sig_count)
					{
					case 1:
						ADPCM_Play(3);	/* SE:3 */
						p_stPCG = PCG_Get_Info(ROAD_PCG_SIGNAL_1);	/* �V�O�i�������v */
						p_stPCG->pPatCodeTbl[0] = SetBGcode(0, 0, 0x01, 0xFF);
						break;
					case 2:
						ADPCM_Play(2);	/* SE:2 */
						p_stPCG = PCG_Get_Info(ROAD_PCG_SIGNAL_2);	/* �V�O�i�������v */
						p_stPCG->pPatCodeTbl[0] = SetBGcode(0, 0, 0x01, 0xFF);
						break;
					case 3:
						ADPCM_Play(1);	/* SE:1 */
						p_stPCG = PCG_Get_Info(ROAD_PCG_SIGNAL_3);	/* �V�O�i�������v */
						p_stPCG->pPatCodeTbl[0] = SetBGcode(0, 0, 0x01, 0xFF);
						break;
					default:
						ADPCM_Play(16);	/* SE:WAKAME */
						p_stPCG = PCG_Get_Info(ROAD_PCG_SIGNAL_1);	/* �V�O�i�������v */
						p_stPCG->pPatCodeTbl[0] = SetBGcode(0, 0, 0x0A, 0xFF);
						p_stPCG = PCG_Get_Info(ROAD_PCG_SIGNAL_2);	/* �V�O�i�������v */
						p_stPCG->pPatCodeTbl[0] = SetBGcode(0, 0, 0x0A, 0xFF);
						p_stPCG = PCG_Get_Info(ROAD_PCG_SIGNAL_3);	/* �V�O�i�������v */
						p_stPCG->pPatCodeTbl[0] = SetBGcode(0, 0, 0x0A, 0xFF);
						break;
					}
					sig_count++;
				}
				
				/* �]�������Ԃŏ��� */
				BG_main(time_st);	/* �o�b�N�O�����h���� */
				
				if(sig_count >= 5)
				{
					sig_count = 0;
					T_TimerStart();	/* �^�C�}�[�J�n */
					
					SetTaskInfo(SCENE_GAME);	/* �Q�[���^�X�N�֐ݒ� */
				}
				break;
			}
			case SCENE_GAME:	/* �Q�[���V�[�� */
			{
				if((g_Input & KEY_b_Q) != 0u)	/* �p */
				{
					SetTaskInfo(SCENE_GAME_E);	/* �Q�[���V�[��(�I������)�֐ݒ� */
				}
				
				
				if( (g_bFlip_old == TRUE) && (g_bFlip == FALSE) )	/* �؂�ւ����㔻�� */
				{
					/* ���Ԃ̏����擾 */
					Update |= MyCarInfo_Update(g_Input, &Torque);	/* ���Ԃ̏����X�V1 */
//					unDebugCounter1++;
				}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				if(g_bDebugMode == TRUE)
				{
					GetNowTime(&time_now);
					g_unTime_Pass[bTimePass] = Mmax(time_now - g_unTime_Pass[0], g_unTime_Pass[bTimePass]);	/* 2 */
					bTimePass++;
					g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
				}
#endif
				if(stTask.b16ms == TRUE)
				{
					Update |= MyCarInfo_Update16ms(Torque);	/* ���Ԃ̏����X�V2 */
//					unDebugCounter2++;
				}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				if(g_bDebugMode == TRUE)
				{
					GetNowTime(&time_now);
					g_unTime_Pass[bTimePass] = Mmax(time_now - g_unTime_Pass[0], g_unTime_Pass[bTimePass]);	/* 3 */
					bTimePass++;
					g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
				}
#endif
				if( (g_bFlip_old == TRUE) && (g_bFlip == FALSE) )	/* �؂�ւ����㔻�� */
				{
					/* ���[�h�p�^�[�� */
					Update |= Road_Pat_Main(&uRoad);	/* �R�[�X�f�[�^�̍X�V */
					if(uRoad == 0xFFFF)
					{
						g_uGameStatus = 4;	/* �S�[�� */
					}
					else if(uRoad >= g_stRoadInfo.Courselength - 16)
					{
						g_uGameStatus = 3;	/* �S�[�����O */
					}
					else if(uRoad <= 8)
					{
						g_uGameStatus = 2;	/* �X�^�[�g */
					}
					else
					{
						g_uGameStatus = 1;	/* �Q�[���� */
					}
					if(Update != 0)
					{
						/* ���X�^�[���� */
						Update |= Raster_Main();	/* �R�[�X�̃��X�^�[���� */
					}
				}
				
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				if(g_bDebugMode == TRUE)
				{
					GetNowTime(&time_now);
					g_unTime_Pass[bTimePass] = Mmax(time_now - g_unTime_Pass[0], g_unTime_Pass[bTimePass]);	/* 4 */
					bTimePass++;
					g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
				}
#endif
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				//SetDebugPos(unDebugCounter1, unDebugCounter2);
#endif
				/* �]�������Ԃŏ��� */
				BG_main(time_st);	/* �o�b�N�O�����h���� */
				
				/* ���̃^�X�N�� */
				if(g_uGameStatus >= 4u)
				{
					GetPassTime( 100, &unPass_time );		/* ������ */
					SetTaskInfo(SCENE_GAME_E);	/* �Q�[���V�[��(�I������)�֐ݒ� */
				}
				
				//g_Vwait = 1;	/* Wait */
				g_Vwait = 0;	/* No Wait */
				
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				if(g_bDebugMode == TRUE)
				{
					GetNowTime(&time_now);
					g_unTime_Pass[bTimePass] = Mmax(time_now - g_unTime_Pass[0], g_unTime_Pass[bTimePass]);	/* 5 */
					bTimePass++;
					g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
				}
#endif
				
				break;
			}
			case SCENE_GAME_E:	/* �Q�[���V�[��(�I������) */
			{
				StopRaster();	/* ���X�^�[���荞�ݏ����X�g�b�v */
				Music_Stop();	/* ���y�Đ� ��~ */
				
				switch(g_uGameStatus)
				{
					case 0:	/* ���̃X�e�[�W */
					case 1:	/* ���̃X�e�[�W */
					case 2:	/* ���̃X�e�[�W */
					case 3:	/* ���̃X�e�[�W */
					{
						break;
					}
					case 4:	/* ���̃X�e�[�W */
					{
						if( GetPassTime( 2000, &unPass_time ) != 0u )	/* ���莞�ԉ����Ȃ���Ύ��s */
						{
							SetTaskInfo(SCENE_NEXT_STAGE_S);	/* ���̃X�e�[�W��(�J�n����)�֐ݒ� */
						}
						break;
					}
					case 5:	/* ���Ԑ؂� */
					{
						SetTaskInfo(SCENE_GAME_OVER_S);		/* �Q�[���I�[�o�[�V�[��(�J�n����)�֐ݒ� */
						break;
					}
					default:	/* �Ӑ}���Ȃ��J�� */
					{
						SetTaskInfo(SCENE_GAME_OVER_S);		/* �Q�[���I�[�o�[�V�[��(�J�n����)�֐ݒ� */
						break;
					}
				}
				break;
			}
			case SCENE_GAME_OVER_S:	/* �Q�[���I�[�o�[�V�[��(�J�n����) */
			{
				SetFlip(FALSE);			/* �t���b�v�֎~ */
				
				/* �O���t�B�b�N�\�� */
				G_CLR();		/* �O���t�B�b�N�N���A */
				
				/* �X�v���C�g���a�f */
				PCG_VIEW(0x00u);	/* �X�v���C�g���a�f��\�� */

				/* �e�L�X�g */
				T_Clear();		/* �e�L�X�g�N���A */

				/* ���� */
				MOV_Play(4);	/* �|�����L�[ */
				
				BG_TextPut("G A M E  O V E R", 64, 128);		/* GAME OVER���� */
				
				Music_Play(16);	/* GameOver�� */
				
				SetTaskInfo(SCENE_GAME_OVER);	/* �Q�[���I�[�o�[�V�[���^�X�N�֐ݒ� */
				break;
			}
			case SCENE_GAME_OVER:	/* �Q�[���I�[�o�[�V�[�� */
			{
				if(g_Input == KEY_B)	/* B�{�^�� */
				{
					SetTaskInfo(SCENE_GAME_OVER_E);	/* �Q�[���I�[�o�[�V�[��(�I������)�^�X�N�֐ݒ� */
				}
				break;
			}
			case SCENE_GAME_OVER_E:	/* �Q�[���I�[�o�[�V�[��(�I������) */
			{
				SetTaskInfo(SCENE_HI_SCORE_S);	/* �n�C�X�R�A�����L���O�V�[��(�J�n����)�^�X�N�֐ݒ� */
				break;
			}
			case SCENE_NEXT_STAGE_S:
			{
				SetFlip(FALSE);			/* �t���b�v�֎~ */
				
				/* �O���t�B�b�N�\�� */
				G_HOME(0);			/* �z�[���ʒu�ݒ� */
				G_CLR();			/* �O���t�B�b�N�N���A */
				G_PaletteSetZero();	/* �O���t�B�b�N�p���b�g�̊�{�ݒ� */
				
				/* �X�v���C�g���a�f */
				PCG_VIEW(0x00u);	/* �X�v���C�g���a�f��\�� */

				/* �e�L�X�g */
				T_Clear();		/* �e�L�X�g�N���A */

				/* ���� */
				MOV_Play(5);	/* ��邶��Ȃ� */
				
				BG_TextPut("WE ARE NOW RUSHING INTO B ZONE.", 0, 128);	/* STAGE �N���A���� */
				BG_TextPut("BE ON YOUR GUARD", 60, 140				);		/* STAGE �N���A���� */
				
				Music_Play(15);	/* �N���A */
				
				g_Stage = Mmin(g_Stage++, 9);	/* ���̃X�e�[�W�� */

				SetTaskInfo(SCENE_NEXT_STAGE);	/* ���̃X�e�[�W�V�[���֐ݒ� */
				break;
			}
			case SCENE_NEXT_STAGE:
			{
				if(g_Input == KEY_B)	/* B�{�^�� */
				{
					SetTaskInfo(SCENE_NEXT_STAGE_E);	/* ���̃X�e�[�W�V�[��(�I������)�֐ݒ� */
				}
				break;
			}
			case SCENE_NEXT_STAGE_E:
			{
				SetTaskInfo(SCENE_START_S);	/* �Q�[���V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_HI_SCORE_S:	/* �n�C�X�R�A�����L���O�V�[��(�J�n����) */
			case SCENE_HI_SCORE:	/* �n�C�X�R�A�����L���O�V�[�� */
			case SCENE_HI_SCORE_E:	/* �n�C�X�R�A�����L���O�V�[��(�I������) */
			{
				SetTaskInfo(SCENE_TITLE_S);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_OPTION_S:		/* �I�v�V�����V�[�� */
			case SCENE_OPTION:		/* �I�v�V�����V�[�� */
			case SCENE_OPTION_E:		/* �I�v�V�����V�[��(�I������) */
			{
				break;
			}
			case SCENE_EXIT:		/* �I���V�[�� */
			{
				Music_Stop();	/* ���y�Đ� ��~ */

				/* ���� */
				MOV_Play(2);	/* �o�C�o�C */
				
				loop = 0;	/* ���[�v�I�� */
				break;
			}
			default:	/* �ُ�V�[�� */
			{
				loop = 0;	/* ���[�v�I�� */
				break;
			}
		}

		/* �^�X�N���� */
		UpdateTaskInfo();		/* �^�X�N�̏����X�V */

		uFreeRunCount++;	/* 16bit �t���[�����J�E���^�X�V */
		
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		/* �������Ԍv�� */
		GetNowTime(&time_now);
		unTime_cal = time_now - time_st;	/* LSB:1 UNIT:ms */
		g_unTime_cal = unTime_cal;
		if( stTask.bScene == SCENE_GAME )
		{
			unTime_cal_PH = Mmax(unTime_cal, unTime_cal_PH);
			g_unTime_cal_PH = unTime_cal_PH;
		}
#endif

		/* �����҂� */
		wait_vdisp(g_Vwait);

	}
	while( loop );

	g_bExit = FALSE;
	App_exit();	/* �I������ */
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	printf("��������:Real=%3d[ms] PH=%3d[ms]\n", unTime_cal, unTime_cal_PH);
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
static void App_Init(void)
{
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init �J�n");
#endif

	g_nMaxMemSize = MaxMemSize(1);
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init MaxMemSize");
#endif
	
	/* �Q�[�����e�̏����� */
	S_All_Init_Score();	/* �X�R�A�̏����� */
	SetDebugNum(0x80);	/* �f�o�b�O�l������ */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init �Q�[�����e�̏�����");
#endif

	/* MFP */
	g_CpuTime = TimerD_INIT();	/* �^�C�}�[D������ */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init MFP");
#endif

	/* ���X�g�t�@�C���̓ǂݍ��� */
	Init_FileList_Load();
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init ���X�g�t�@�C���̓ǂݍ���");
#endif
	
	/* ���y */
	Init_Music();	/* ������(�X�[�p�[�o�C�U�[���[�h���O)	*/
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init ���y");
#endif
	Music_Play(1);	/* ���[�f�B���O�� */

	/* ���� */
	MOV_INIT();	/* ���������� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init ����");
#endif

	/* �e�L�X�g�N���A */
	T_Clear();	/* �e�L�X�g�N���A */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init �e�L�X�g");
#endif
	
	/* ��� */
	g_nCrtmod = CRT_INIT();
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init ���");
#endif
	
	/* �X�[�p�[�o�C�U�[���[�h�J�n */
	g_nSuperchk = _dos_super(0);
	if( g_nSuperchk < 0 ) {
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		puts("App_Init ���ł�_dos_super");
#endif
	} else {
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		puts("App_Init _dos_super�J�n");
#endif
	}

	/* �e�L�X�g */
	T_INIT();	/* �e�L�X�g�u�q�`�l������ */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init T_INIT");
#endif
	
	/* �O���t�B�b�N */
	G_INIT();			/* ��ʂ̏����ݒ� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init �O���t�B�b�N");
#endif
	
	/* �X�v���C�g�^�a�f */
	PCG_INIT();	/* �X�v���C�g�^�a�f�̏����� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init �X�v���C�g�^�a�f");
#endif

	/* MFP */
	MFP_INIT();	/* V-Sync���荞�ݓ��̏��������� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init MFP");
#endif
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init �I��");
#endif
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static void App_exit(void)
{
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �J�n");
#endif
	
	/* �e�L�X�g�N���A */
	T_Clear();	/* �e�L�X�g�N���A */
	
	/* �X�v���C�g���a�f */
	PCG_VIEW(0x00u);		/* �X�v���C�g���a�f��\�� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �X�v���C�g");
#endif

	/* ��� */
	CRTMOD(0x100 + g_nCrtmod);	/* ���[�h�����Ƃɖ߂� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit ���");
#endif

	/* ���y */
	Exit_Music();			/* ���y��~ */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit ���y");
#endif
	
	if(g_bExit == TRUE)
	{
		puts("�G���[���L���b�`�I ESC to skip");
		KeyHitESC();	/* �f�o�b�O�p */
	}
	
	/* MFP */
	TimerD_EXIT();			/* Timer-D�̉��� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit Timer-D");
#endif

	MFP_EXIT();				/* MFP�֘A�̉��� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit MFP");
#endif

	/* �e�L�X�g */
	T_EXIT();				/* �e�L�X�g�I������ */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �e�L�X�g");
#endif

	MyMfree(0);				/* �S�Ẵ���������� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit ������");
#endif

	_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �L�[�o�b�t�@�N���A");
#endif

	/*�X�[�p�[�o�C�U�[���[�h�I��*/
	_dos_super(g_nSuperchk);
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �X�[�p�[�o�C�U�[���[�h�I��");
#endif
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	{
		uint32_t i=0, j=0;
		uint16_t x, y;
		int16_t pat;
		ST_RAS_INFO	stRasInfo;
		GetRasterInfo(&stRasInfo);
		
		printf("====================\n");
		printf("stRasInfo st,mid,ed,size=(%4d,%4d,%4d,%4d)\n", stRasInfo.st, stRasInfo.mid, stRasInfo.ed, stRasInfo.size);
		printf("GetRasterIntPos[i]=(x,y,pat)=(H_pos)\n");
		printf("====================\n");
		for(i=0; i < stRasInfo.size; i++)	/* 2�����\�� */
		{
			SetRasterIntData(0);	/* 1���� */
			GetRasterIntPos( &x, &y, &pat, i, FALSE);
			printf("[%3d,%3d]=(%4d,%4d,%4d), ", j++, i, x, y, pat);
			SetRasterIntData(1);	/* 2���� */
			GetRasterIntPos( &x, &y, &pat, i, FALSE );
			printf("[%3d,%3d]=(%4d,%4d,%4d), ", j++, i, x, y, pat);
			printf("\n");
		}
		printf("==========���荞�݈ʒu�̐ݒ�==========\n");
		SetRasterIntData(0);	/* 1���� */
		GetRasterIntPos( &x, &y, &pat, (RASTER_H_MAX - 2), TRUE);
		printf("[%3d]=(%4d,%4d,%4d), ", (RASTER_H_MAX - 2), x, y, pat);
		SetRasterIntData(1);	/* 2���� */
		GetRasterIntPos( &x, &y, &pat, (RASTER_H_MAX - 2), TRUE );
		printf("[%3d]=(%4d,%4d,%4d), ", (RASTER_H_MAX - 2), x, y, pat);
		printf("\n");
		printf("==========���荞�݂�������܂ł̕\���ʒu==========\n");
		SetRasterIntData(0);	/* 1���� */
		GetRasterIntPos( &x, &y, &pat, (RASTER_H_MAX - 4), TRUE);
		printf("[%3d]=(%4d,%4d,%4d), ", (RASTER_H_MAX - 4), x, y, pat);
		SetRasterIntData(1);	/* 2���� */
		GetRasterIntPos( &x, &y, &pat, (RASTER_H_MAX - 4), TRUE );
		printf("[%3d]=(%4d,%4d,%4d), ", (RASTER_H_MAX - 4), x, y, pat);
		printf("\n");
		printf("====================\n");
	}
#endif
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �I��");
#endif
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t BG_main(uint32_t ulTimes)
{
	int16_t	ret = 0;
	uint32_t	time_now;
	uint16_t	BGprocces_ct = 0;
	uint16_t	uCount;
	uint8_t	bMode, bMode_rev;
	uint8_t	bNum;
	uint8_t	bNumCourse_Obj_Max;
	uint8_t	bNumEnemyCar_Max;
	uint8_t	bFlipStateOld;
	ST_TASK		stTask = {0}; 
	
	static uint8_t	bFlipState = Clear_G;
	static uint16_t uTimer_old = 0xFFFFu;
	
	if(g_bFlip == TRUE)		/* ��ʐ؂�ւ��� */
	{
		return ret;
	}
	
	GetTaskInfo(&stTask);	/* �^�X�N�̏��𓾂� */

	bFlipStateOld = bFlipState;

	if( g_CpuTime < 300 )	/* �ᑬMPU */
	{
		bNumCourse_Obj_Max = 2;
		bNumEnemyCar_Max = 1;
	}
	else
	{
		bNumCourse_Obj_Max = COURSE_OBJ_MAX;
		bNumEnemyCar_Max = ENEMYCAR_MAX;
	}
	
	/* ���[�h�����n�� */
	GetGameMode(&bMode);
	bMode_rev = g_mode_rev;

	/* �R�[�X�T�C�N���J�E���^ */
	GetRoadCycleCount(&uCount);
	
	do
	{
		/* �w�i�̏��� */
		switch(bFlipState)
		{
			/* �`��̃N���A���� */
			case Clear_G:
			{
				/* �O���t�B�b�N */
				G_CLR_ALL_OFFSC();	/* �O���t�B�b�N������ */
				break;
			}
			/* �w�i */
			case BackGround_G:
			{
#if 1
				Move_Course_BG(bMode);	/* �R�[�X�̓����ɂ��킹�Ĕw�i�𓮂��� */
#endif
				break;
			}
			/* ���V�̖�(E:�E�� / O:����) */
			case Object0_G:
			case Object1_G:
			case Object2_G:
			case Object3_G:
			case Object4_G:
			case Object5_G:
			case Object6_G:
			case Object7_G:
			{
#if 1
				bNum = bFlipState - Object0_G;
				if(bNum < bNumCourse_Obj_Max)
				{
					/* �`�揇���\�[�g���� */
					if(bNum == 0)
					{
						Sort_Course_Obj();		/* �R�[�X�I�u�W�F�N�g */
					}
					Course_Obj_main(bNum, bMode, bMode_rev);
				}
#endif
				break;
			}
			/* ���C�o���� */
			case Enemy1_G:
			case Enemy2_G:
			{
#if 1
				bNum = bFlipState - Enemy1_G;
				if(bNum < bNumEnemyCar_Max)
				{
					/* �`�揇���\�[�g���� */
					if(bNum == 0)
					{
						Sort_EnemyCAR();		/* ���C�o���� */
					}
					
					EnemyCAR_main(bNum, bMode, bMode_rev);
				}
#endif
				break;
			}
			/* �X�^�[�g�ʒu */
			case StartPoint_G:
			{
				if(g_uGameStatus == 2)
				{
					Set_Course_TmpObject(uCount - s_uStartCount + 2, START_PT_CG);	/* �X�^�[�g�Q�[�g */
				}
				else
				{
					s_uStartCount = uCount;
				}
				break;
			}
			/* �S�[���ʒu */
			case GoalPoint_G:
			{
				if(g_uGameStatus == 3)
				{
					Set_Course_TmpObject(uCount - s_uGoalCount, GOAL_PT_CG);	/* �S�[���Q�[�g */
				}
				else
				{
					s_uGoalCount = uCount;
				}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				/* �f�o�b�O���X�L�b�v�^�X�N */
				if(stTask.bScene == SCENE_DEBUG)
				{
					Set_Course_TmpObject(g_uDebugNum - 0x80, GOAL_PT_CG);	/* �S�[���Q�[�g */
				}
#endif
				break;
			}
			/* ���Ԃ̏��� */
			case MyCar_G:
			{
				uint16_t uTimer;
				
				MyCar_Interior();		/* ���Ԃ̃C���e���A���� */
				S_Main_Score();			/* �X�R�A�\�� */
				PCG_Main();				/* �X�v���C�g�o�� */
				T_Main(&uTimer);		/* �e�L�X�g��ʂ̏��� */
				if(uTimer_old != uTimer)
				{
					if((uTimer > 0) && (uTimer < 10))
					{
						int16_t PCM_num;
						PCM_num = 20 + uTimer;
						ADPCM_Play(PCM_num);	/* SE:1�`9 */
					}
				}
				uTimer_old = uTimer;
				if(uTimer == 0u)
				{
					g_uGameStatus = 5;	/* �^�C���I�[�o�[ */
				}
				break;
			}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
			case Debug_View_G:
			{
				Debug_View(BGprocces_ct);	/* �f�o�b�O���\�� */
				break;
			}
#endif
			/* ��ʂ��t���b�v���� */
			case Flip_G:	
			{
				SetFlip(TRUE);	/* �t���b�v���� */
				break;
			}
			default:
			{
				break;
			}
		}
		
		bFlipState++;
		
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		/* �f�o�b�O���X�L�b�v�^�X�N */
		if(stTask.bScene == SCENE_DEBUG)
		{
			if(bFlipState == Object0_G)
			{
				bFlipState = Enemy1_G;
			}
		}
#endif
		
		if(bFlipState > Flip_G)
		{
			bFlipState = 0;
			ret = 1;	/* ��ʂ��t���b�v���� */
		}
		BGprocces_ct++;

		GetNowTime(&time_now);	/* ���ݎ������擾 */
		
		if(ret != 0)	/* ���[�v�E�o���� */
		{
			break;
		}
#if 1
		else if((time_now - ulTimes) >= BG_SKIP_TIME)	/* BG_SKIP_TIME[ms]�ȏ�Ȃ珈�����Ȃ� */
		{
			ret = 0;	/* ��ʂ��t���b�v���Ȃ� */
//			break;
		}
		else if((time_now - ulTimes) >= BG_TIME_OUT)	/* BG_TIME_OUT[ms]�ȏ�Ȃ珈�����Ȃ� */
		{
			ret = 0;	/* ��ʂ��t���b�v���Ȃ� */
//			break;
		}
		else
		{
			/* �������Ȃ� */
		}
#endif
	}
	while(1);
	
	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	FlipProc(void)
{
	int16_t	ret = 0;
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint32_t time_now;
	static uint8_t	bFPS = 0u;
	static uint32_t	unTime_FPS = 0u;
#endif
	
	if(g_bFlip == FALSE)	/* �`�撆�Ȃ̂Ńt���b�v���Ȃ� */
	{
		return ret;
	}
	
	SetFlip(FALSE);			/* �t���b�v�֎~ */
	
	/* ��ʐ؂�ւ� */
	G_HOME(0);
	
	/* ���[�h�`�F���W */
	if(g_mode == 1u)		/* �㑤���� */
	{
		SetGameMode(2);
	}
	else if(g_mode == 2u)	/* �������� */
	{
		SetGameMode(1);
	}
	else					/* ���̑� */
	{
		SetGameMode(2);
	}
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	bFPS++;
	GetNowTime(&time_now);
	if( (time_now - unTime_FPS) >= 1000ul )
	{
		g_bFPS_PH = bFPS;
		unTime_FPS = time_now;
		bFPS = 0;
	}
#endif
	ret = 1;

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetFlip(uint8_t bFlag)
{
	int16_t	ret = 0;

	Set_DI();	/* ���荞�݋֎~ */
	g_bFlip_old = g_bFlip;	/* �t���b�v�O��l�X�V */
	g_bFlip = bFlag;
	Set_EI();	/* ���荞�݋��� */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void Set_DI(void)
{
	if(g_nIntCount == 0)
	{
#if 0
		/*�X�[�p�[�o�C�U�[���[�h�I��*/
		if(g_nSuperchk > 0)
		{
			_dos_super(g_nSuperchk);
		}
#endif
		g_nIntLevel = intlevel(6);	/* ���֐ݒ� */
		g_nIntCount = Minc(g_nIntCount, 1);
		
#if 0
		/* �X�[�p�[�o�C�U�[���[�h�J�n */
		g_nSuperchk = _dos_super(0);
#endif
	}
	else
	{
		g_nIntCount = Minc(g_nIntCount, 1);
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void Set_EI(void)
{
	if(g_nIntCount == 1)
	{
		g_nIntCount = Mdec(g_nIntCount, 1);
		
		/* �X�v���A�X���荞�݂̒��̐l��(�ȉ���)��� */
		/* MFP�Ńf�o�C�X���̊����݋֎~�ݒ������ۂɂ͕K��SR����Ŋ����݃��x�����グ�Ă����B*/
		asm ("\ttst.b $E9A001\n\tnop\n");
		/*
			*8255(�ޮ��è��)�̋�ǂ�
			nop		*���O�܂ł̖��߃p�C�v���C���̓���
					*�����b�A���̖��ߏI���܂łɂ�
					*����ȑO�̃o�X�T�C�N���Ȃǂ�
					*�������Ă��邱�Ƃ��ۏ؂����B
		*/
#if 0
		/*�X�[�p�[�o�C�U�[���[�h�I��*/
		if(g_nSuperchk > 0)
		{
			_dos_super(g_nSuperchk);
		}
#endif
		intlevel(g_nIntLevel);	/* ���։��� */
#if 0
		/* �X�[�p�[�o�C�U�[���[�h�J�n */
		g_nSuperchk = _dos_super(0);
#endif
	}
	else
	{
		g_nIntCount = Mdec(g_nIntCount, 1);
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	GetGameMode(uint8_t *bMode)
{
	int16_t	ret = 0;
	
	*bMode = g_mode;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetGameMode(uint8_t bMode)
{
	int16_t	ret = 0;
	
	g_mode = bMode;
	if(bMode == 1)
	{
		g_mode_rev = 2;
	}
	else
	{
		g_mode_rev = 1;
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
int16_t	GetDebugNum(uint16_t *uNum)
{
	int16_t	ret = 0;
	*uNum = g_uDebugNum;
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetDebugNum(uint16_t uNum)
{
	int16_t	ret = 0;
	g_uDebugNum = uNum;
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	GetDebugMode(uint8_t *bMode)
{
	int16_t	ret = 0;
	*bMode = g_bDebugMode;
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetDebugMode(uint8_t bMode)
{
	int16_t	ret = 0;
	g_bDebugMode = bMode;
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	GetDebugPos(int16_t *p_PosX, int16_t *p_PosY)
{
	int16_t	ret = 0;
	*p_PosX = g_DebugPosX;
	*p_PosY = g_DebugPosY;
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetDebugPos(int16_t PosX, int16_t PosY)
{
	int16_t	ret = 0;
	g_DebugPosX = PosX;
	g_DebugPosY = PosY;
	return ret;
}
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	GetDebugHis(int16_t *p_His, int16_t Num)
{
	int16_t	ret = 0;
	if(Num < 0)Num = 0;
	if(Num >= 10)Num = 9;
	*p_His = g_DebugHis[Num];
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetDebugHis(int16_t His)
{
	int16_t	ret = 0;
	int16_t	i;
	static int16_t g_DebugHisCnt = 0;
	
	if(g_DebugHisCnt >= 10)
	{
		for(i=0; i<10-1; i++)
		{
			g_DebugHis[i] = g_DebugHis[i+1];
		}
		g_DebugHis[10-1] = His;
	}
	else
	{
		g_DebugHis[g_DebugHisCnt] = His;
		g_DebugHisCnt++;
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
void Debug_View(uint16_t uFreeRunCount)
{
	uint8_t bMode, bMode_rev;
	
	/* ���[�h�����n�� */
	bMode = g_mode;
	bMode_rev = g_mode_rev;
	
	if(g_bDebugMode == TRUE)
	{
		uint8_t	str[256] = {0};
		
		int16_t	col;
		int16_t	view_offset_x, view_offset_y;
		int16_t	hide_offset_x, hide_offset_y;
		int16_t	BG_offset_x, BG_offset_y;
		
		static uint8_t ubDispNum = DEBUG_FREE;
		static uint8_t ubDispNum_flag = 0;

		ST_CRT	stCRT = {0};
		
		/* ���[�h�ؑւɂ��ݒ�l�̕ύX */
		GetCRT(&stCRT, bMode);
		view_offset_x	= stCRT.view_offset_x;
		view_offset_y	= stCRT.view_offset_y;
		hide_offset_x	= stCRT.hide_offset_x;
		hide_offset_y	= stCRT.hide_offset_y;
		BG_offset_x		= stCRT.BG_offset_x;
		BG_offset_y		= stCRT.BG_offset_y;
		

		if(ChatCancelSW((g_Input & KEY_b_RLUP)!=0u, &ubDispNum_flag) == TRUE)	/* ���[���A�b�v�ŕ\���ؑ� */
		{
			ubDispNum++;
			if(DEBUG_MAX <= ubDispNum)
			{
				ubDispNum = DEBUG_NONE;
			}
		}

		{
			switch(ubDispNum)
			{
			case DEBUG_NONE:
				{
					/* ��\�� */
				}
				break;
			case DEBUG_COURSE_OBJ:
				{
#if 1	/* ��Q����� */
					uint32_t	i = 0;
					ST_COURSE_OBJ	stCourse_Obj = {0};
					i = Mmin(Mmax(g_uDebugNum - 0x80, 0), COURSE_OBJ_MAX-1);
					GetCourseObj(&stCourse_Obj, i);	/* ��Q���̏�� */
					sprintf(str, "C_Obj[%d](%4d,%3d,%d)(%d),Debug(%3d)",
						i, stCourse_Obj.x, stCourse_Obj.y, stCourse_Obj.z, stCourse_Obj.ubAlive,
						g_uDebugNum);	/* ��Q���̏�� */
#endif
				}
				break;
			case DEBUG_ENEMYCAR:
				{
#if 1	/* �G�ԏ�� */
					uint32_t	i = 0;
					ST_ENEMYCARDATA	stEnemyCar = {0};
					i = Mmin(Mmax(uFreeRunCount % ENEMYCAR_MAX, 0), ENEMYCAR_MAX-1);
					GetEnemyCAR(&stEnemyCar, i);	/* ���C�o���Ԃ̏�� */
					sprintf(str, "E[%d] (%d)(%4d,%4d,%4d),spd(%3d)", i,
						stEnemyCar.ubAlive,
						stEnemyCar.x,
						stEnemyCar.y,
						stEnemyCar.z,
						stEnemyCar.VehicleSpeed
					);
					/* ���C�o���Ԃ̓����蔻�� */
					Draw_Box(	stCRT.hide_offset_x + stEnemyCar.sx,
								stCRT.hide_offset_y + stEnemyCar.sy,
								stCRT.hide_offset_x + stEnemyCar.ex,
								stCRT.hide_offset_y + stEnemyCar.ey, 0x03, 0xFFFF);
#endif
				}
				break;
			case DEBUG_MYCAR:
				{
#if 1	/* ���ԏ�� */
					int16_t speed;
					int16_t	myCarSx, myCarEx, myCarSy, myCarEy;
					
					ST_CARDATA	stMyCar;
					GetMyCar(&stMyCar);	/* ���� */
					GetMyCarSpeed(&speed);
					
					/* �N���}�̌��� */
					col = 0x5;
					Draw_Line(	hide_offset_x + Mdiv2(WIDTH)  + Mdiv256(64 * APL_Cos(stMyCar.Angle - 90)),
								hide_offset_y + Mdiv4(HEIGHT) + Mdiv8(HEIGHT) + Mdiv256(64 * APL_Sin(stMyCar.Angle - 90)),
								hide_offset_x + Mdiv2(WIDTH)  + 1, 
								hide_offset_y + Mdiv4(HEIGHT) + Mdiv8(HEIGHT) + 1,
								col,
								0xFFFF);
					
					/* ���̌��� */
					col = 0xB;
					Draw_Line(	hide_offset_x + Mdiv2(WIDTH)  + Mdiv256(64 * APL_Cos(g_stRoadInfo.angle - 90)),
								hide_offset_y + Mdiv4(HEIGHT) + Mdiv8(HEIGHT) + Mdiv256(64 * APL_Sin(g_stRoadInfo.angle - 90)),
								hide_offset_x + Mdiv2(WIDTH)  + 1, 
								hide_offset_y + Mdiv4(HEIGHT) + Mdiv8(HEIGHT) + 1,
								col,
								0xFFFF);
					
					switch(stMyCar.ubOBD)
					{
						case OBD_NORMAL:
						{
							col = 0x0B;
							break;
						}
						case OBD_DAMAGE:
						{
							col = 0x0A;
							break;
						}
						case OBD_SPIN_L:
						case OBD_SPIN_R:
						{
							col = 0x0C;
							break;
						}
						case OBD_COURSEOUT:
						{
							col = 0x0E;
							break;
						}
						default:
						{
							col = 0x00;
							break;
						}
					}
					
					/* �N���}�̈ʒu */
					myCarSx = ROAD_CT_POINT + APL_AngleDiff(g_stRoadInfo.angle, stMyCar.Angle) - 8;	/* �ԂƓ��H�̊p�x�����ŎԂ̈ʒu���ς�� */
					myCarEx = myCarSx + 16;
					myCarSy = Y_MAX_WINDOW - 32;
					myCarEy = myCarSy + 16;
					
					Draw_Box(
						stCRT.hide_offset_x + myCarSx,
						stCRT.hide_offset_y + myCarSy,
						stCRT.hide_offset_x + myCarEx,
						stCRT.hide_offset_y + myCarEy, col, 0xFFFF);
						
					sprintf(str, "C[%d](%4d,%d,%5d,%3d,%3d,%d,%d,%4d,%4d,%d,%d)",
							stMyCar.ubCarType,			/* �Ԃ̎�� */
							stMyCar.uEngineRPM,			/* �G���W����]�� */
							stMyCar.ubShiftPos,			/* �M�A�i */
							stMyCar.Steering,			/* �X�e�A */
							stMyCar.Angle,				/* ���� */
							stMyCar.ubThrottle,			/* �X���b�g���J�x */
							stMyCar.ubBrakeLights,		/* �u���[�L���C�g */
							stMyCar.ubHeadLights,		/* �w�b�h���C�g */
							stMyCar.ubWiper,			/* ���C�p�[ *//* �i���j�}�b�v�g���N */
							stMyCar.bTire,				/* �^�C���̏�� *//* �i���j�g���N */
							stMyCar.ubOBD,				/* �̏�̏�� */
							speed						/* �ԑ� */
					);	/* ���Ԃ̏�� */
#endif
				}
				break;
			case DEBUG_ROAD:
				{
#if 1	/* ���H��� */
					/* ���̌��� */
					col = 0xB;
					Draw_Line(	hide_offset_x + Mdiv2(WIDTH)  + Mdiv256(64 * APL_Cos(g_stRoadInfo.angle - 90)),
								hide_offset_y + Mdiv4(HEIGHT) + Mdiv8(HEIGHT) + Mdiv256(64 * APL_Sin(g_stRoadInfo.angle - 90)),
								hide_offset_x + Mdiv2(WIDTH)  + 1, 
								hide_offset_y + Mdiv4(HEIGHT) + Mdiv8(HEIGHT) + 1,
								col,
								0xFFFF);
					
					sprintf(str, "R=h%3d,s%3d,a%3d,d%3d,o%3d,C%3d,H%3d,Hb%3d,ox%3d,oy%3d,ov%3d",
						g_stRoadInfo.height,		/* �R�[�X�̍��� */
						g_stRoadInfo.slope,			/* �R�[�X�̋Ȃ��� */
						g_stRoadInfo.angle,			/* �R�[�X�̕����i�p�x�j */
						g_stRoadInfo.distance,		/* �R�[�X�̋����H */
						g_stRoadInfo.object,		/* �R�[�X�̃I�u�W�F�N�g */
						g_stRoadInfo.Courselength,	/* �R�[�X�̑S�� */
						g_stRoadInfo.Horizon,		/* �����ʒu */
						g_stRoadInfo.Horizon_Base,	/* ������ʒu */
						g_stRoadInfo.offset_x,		/* �R�[�X�̃p�^�[��X���W�H */
						g_stRoadInfo.offset_y,		/* �R�[�X�̃p�^�[��Y���W */
						g_stRoadInfo.offset_val		/* �R�[�X�̃p�^�[��Y���W�X�V�l */
					);
#endif
				}
				break;
			case DEBUG_RASTER:
				{

#if 1	/* ���X�^�[��� */
					uint16_t x, y;
					int16_t pat;
					int16_t pos;
					int16_t count, linCount;
					ST_RAS_INFO	stRasInfo;
					GetRasterInfo(&stRasInfo);

					{
						/* ���X�^�[�J�n�ʒu */
						col = 0x3;
						Draw_Line(	hide_offset_x + 0,
									hide_offset_y + stRasInfo.st,
									hide_offset_x + WIDTH, 
									hide_offset_y + stRasInfo.st,
									col,
									0xFFFF);
						/* ���X�^�[���Ԉʒu */
						col = 0xF;
						Draw_Line(	hide_offset_x + 0,
									hide_offset_y + stRasInfo.mid,
									hide_offset_x + WIDTH, 
									hide_offset_y + stRasInfo.mid,
									col,
									0xFFFF);
						/* ���X�^�[�I���ʒu */
						col = 0xB;
						Draw_Line(	hide_offset_x + 0,
									hide_offset_y + stRasInfo.ed,
									hide_offset_x + WIDTH, 
									hide_offset_y + stRasInfo.ed,
									col,
									0xFFFF);

#if 0						
						/* ���E���� */
						col = 0x4;
						Draw_Line(	hide_offset_x + Mdiv2(WIDTH)  + Mdiv256(64 * APL_Cos(ViewAngle - 90)),
									hide_offset_y + Mdiv4(HEIGHT) + Mdiv8(HEIGHT) + Mdiv256(64 * APL_Sin(ViewAngle - 90)),
									hide_offset_x + Mdiv2(WIDTH)  + 1, 
									hide_offset_y + Mdiv4(HEIGHT) + Mdiv8(HEIGHT) + 1,
									col,
									0xFFFF);
#endif
						/* �Ȃ��� */
						col = 0x3;
						Draw_Circle(hide_offset_x + Mdiv2(WIDTH) + 1, 
									hide_offset_y + Mdiv4(HEIGHT) + Mdiv8(HEIGHT) + 1,
									Mmax(Mdiv4(g_stRoadInfo.slope) + 1, 8),
									col,
									0, 360, 256);
#if 0						
						/* x */
						col = 0xE;
						/* �Z���^�[ */
						if( ras_cal_x < 256 )	/* ���J�[�u */
						{
							point_x = ROAD_CT_POINT + (  0 - ras_cal_x);
						}
						else	/* �E�J�[�u */
						{
							point_x = ROAD_CT_POINT + (512 - ras_cal_x);
						}
						Draw_Pset(	hide_offset_x + point_x,
									hide_offset_y + g_stRoadInfo.Horizon + uRas_y,
									col);

						/* y */
						col = 0x0C;
						Draw_Pset(	hide_offset_x + uRas_y + 32,
									hide_offset_y + Y_MAX_WINDOW - ras_cal_y,
									col);
#endif
					}
					
					pos = Mmax(Mmin( g_uDebugNum, stRasInfo.size ), 0);
					GetRasterIntPos( &x, &y, &pat, pos, FALSE );
					count = GetRasterCount(&linCount);
					
					sprintf(str, "Ras(%d)s(%d,%d,%d,%d)i(%3d,%3d)[%d]",
						pos,
						stRasInfo.st, stRasInfo.mid, stRasInfo.ed, stRasInfo.size,
						count, linCount,
						bMode
						//x, y, pat
					);	/* ���X�^�[��� */
#endif
				}
				break;
			case DEBUG_INPUT:
				{
#if 1	/* ���͏�� */
					int16_t	AnalogMode = 0;
					JOY_ANALOG_BUF stAnalog_Info;

					AnalogMode = GetAnalog_Info(&stAnalog_Info);	/* �A�i���O���擾 */
					
					if(AnalogMode == 0)	/* �A�i���O���[�h */
					{
						sprintf(str,"R(0x%02x 0x%02x)L(0x%02x 0x%02x)B(%04b|%04b|%04b)",
							stAnalog_Info.r_stk_ud,
							stAnalog_Info.r_stk_lr,
							stAnalog_Info.l_stk_ud,
							stAnalog_Info.l_stk_lr,
							(stAnalog_Info.btn_data & 0xF00) >> 8,
							(stAnalog_Info.btn_data & 0x0F0) >> 4,
							(stAnalog_Info.btn_data & 0x00F)
						);
					}
					else
					{
						sprintf(str,"U(%d)D(%d)L(%d)R(%d)B(%d)A(%d)",
							g_Input & KEY_UPPER,
							g_Input & KEY_LOWER,
							g_Input & KEY_LEFT,
							g_Input & KEY_RIGHT,
							g_Input & KEY_B,
							g_Input & KEY_A
						);
					}
#endif
				}
				break;
			case DEBUG_CPUTIME:
				{
#if 1	/* CPU��� */
//							sprintf(str, "CPU Time%2d[ms](MAX%2d[ms]),Debug(%3d)", g_unTime_cal, g_unTime_cal_PH, g_uDebugNum);	/* �������� */
					sprintf(str, "%d T%2d(Max%2d),%2d,%2d,%2d,%2d,%2d F(%2d)", g_CpuTime, g_unTime_cal, g_unTime_cal_PH, 
							g_unTime_Pass[BackGround_G],
							g_unTime_Pass[Object0_G],
							g_unTime_Pass[Enemy1_G],
							g_unTime_Pass[MyCar_G],
							g_unTime_Pass[Debug_View_G],
							g_bFPS_PH
					);
#endif
				}
				break;
			case DEBUG_MEMORY:
				{
#if 1	/* ��������� */
					int d;
					d = (int)_dos_malloc(-1);
					sprintf(str, "Use %d/%d[KB] Free %d[KB]", g_nMaxMemSize - Mdiv1024(d-0x81000000), g_nMaxMemSize, Mdiv1024(d-0x81000000));
#endif
				}
				break;
			case DEBUG_FREE:
				{
					int16_t	i;
					int16_t	dim[10];
					int16_t	x, y;
#if 0	/* ���ł�OK */
					int16_t x, y, col;
					x = X_MIN_DRAW + X_OFFSET + (uFreeRunCount & 0xFF);
					y = ((Y_MIN_DRAW + Y_OFFSET) * (bMode - 1)) + 120 + g_nIntCount;
						col = 4;
						col = 5;
					
					/* �t���b�v�����m�F */
					Draw_Pset( x, y, col);
#endif
#if 0
					{
						int16_t x, y, col;
						x = X_MIN_DRAW + X_OFFSET + (uFreeRunCount & 0xFF);
						y = ((Y_MIN_DRAW + Y_OFFSET) * (bMode - 1)) + 120 - Mmul8(Raster) - Mmul32(g_bFlip);
						if(bMode == 1)
						{
							col = Mdiv256(uFreeRunCount & 0x300) + 1;
						}
						else
						{
							col = 5;
						}
						/* �t���b�v�����m�F */
						Draw_Pset( x, y, col );
					}
#endif
#if 0
			//			sprintf(str, "Ras %3d, %3d, %3d, V %3d", min, Raster_count, max);
					
			//			sprintf(str, "R=%3d,V=%3d,D=%3d,L=%3d,%3d,%3d,%3d,%3d",
				
					sprintf(str, "V=%3d,L=%3d,R=%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d",
						(Vsync_count & 0xFF),
						(Raster_count & 0xFF),
						g_uRasterLine[0],
						g_uRasterLine[1],
						g_uRasterLine[2],
						g_uRasterLine[3],
						g_uRasterLine[4],
						g_uRasterLine[5],
						g_uRasterLine[6],
						g_uRasterLine[7]
					);
#endif
#if 0
					sprintf(str, "T=%2d x 20[us](%2d)", g_unTime_cal, g_unTime_cal_PH);
#endif
#if 0
					int16_t	ret_a, ret_b;
					static int16_t	vx, vy;
					
					src_w = 64;
					src_h = 64;
					
					if((g_Input & KEY_UPPER) != 0u)	/* �� */
					{
						vy--;
					}
					else
					{
					}
					
					if((g_Input & KEY_LOWER) != 0u)	/* �� */
					{
						vy++;
					}
					else
					{
					}
					dst_y = hide_offset_y + vy;

					if((g_Input & KEY_LEFT) != 0u)	/* �� */
					{
						vx--;
						if(bFlag == FALSE)
						{
						}
						else
						{
						}

					}
					else
					{
					}
					
					if((g_Input & KEY_RIGHT) != 0u)	/* �E */
					{
						vx++;
						
						if(bFlag == FALSE)
						{
						}
						else
						{
						}
					}
					else
					{
					}
					dst_x = hide_offset_x + vx;
					
					if((g_Input & KEY_A) != 0u)	/* A�{�^�� */
					{
						KeyHitESC();	/* �f�o�b�O�p */
					}
					else
					{
					}
					
					if((g_Input & KEY_B) != 0u)	/* B�{�^�� */
					{
						SetTaskInfo(SCENE_DEBUG_E);	/* �f�o�b�O(�I������)�֐ݒ� */
					}
					else
					{
					}
					/* �`�悵�����G���A���犮�S�ɂ͂ݏo�Ă邩�ǂ����`�F�b�N */
					ret_a = G_ClipAREA_Chk_Width(&dst_x, &x_min, &x_max, src_w, bMode_rev, POS_RIGHT);
					
					ret_b = G_ClipAREA_Chk_Height(&dst_y, &y_min, &y_max, src_h, bMode_rev, POS_CENTER);
				
					Draw_Box(dst_x, dst_y, dst_x + src_w, dst_y + src_h, 0x03u, 0xFFFF);	/* �l�p�S�� */
					
					sprintf(str, "(%3d,%3d)(%3d,%3d)(%3d,%3d,%3d,%3d)(%d,%d)", vx, vy, dst_x, dst_y, x_min, y_min, x_max, y_max, ret_a, ret_b);
#endif
#if 0	/* �f�o�b�O�\�� */
					if((g_Input & KEY_UPPER)!=0u)dy = Mdec(dy, 1);
					if((g_Input & KEY_LOWER)!=0u)dy = Minc(dy, 1);
#endif
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
//					GetDebugPos(&x, &y);
//					sprintf(str, "(%4d, %4d)", x, y);
#endif
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
					for(i=0; i<10; i++)
					{
						GetDebugHis(&dim[i], i);
					}
					sprintf(str, "(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
						dim[0], dim[1], dim[2], dim[3], dim[4],
						dim[5], dim[6], dim[7], dim[8], dim[9]);
#endif
				}
				break;
			default:
				break;
			}
			/* �\�� */
			Put_Message_To_Graphic(str, bMode);	/* �O���t�B�b�N�̃f�o�b�O�G���A�Ƀ��b�Z�[�W�`�� */
		}
	}
}

#endif	/* OVERKATA_C */
