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

#define	BG_SKIP_TIME	(8u)
#define	BG_TIME_OUT		(56u)

/* �O���[�o���ϐ� */
int32_t	g_nSuperchk = 0;
int32_t	g_nIntLevel = 0;
int32_t	g_nIntCount = 0;
int32_t	g_nCrtmod = 0;
int32_t	g_nMaxMemSize;
int16_t	g_CpuTime = 0;
int16_t	g_Input;
uint8_t	g_mode = 1;
uint8_t	g_mode_rev = 2;
uint8_t	g_Vwait = 1;
uint32_t	g_unTime_cal = 0u;
uint32_t	g_unTime_cal_PH = 0u;
uint8_t	g_bFlip = FALSE;
uint8_t	g_bFlip_old = FALSE;
uint8_t	g_bExit = TRUE;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
uint8_t		g_bDebugMode = FALSE;
uint8_t		g_bFPS_PH = 0u;
uint16_t	g_uDebugNum = 0; 
uint32_t	g_unTime_Pass[MAX_G] = {0u};
#endif

/* enum */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
enum{
	DEBUG_NONE,
	DEBUG_COURSE_OBJ,
	DEBUG_ENEMYCAR,
	DEBUG_MYCAR,
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
int16_t main(void);
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
int16_t main(void)
{
	int16_t	ret = 0;

	uint32_t	unTime_cal = 0u;
	uint32_t	unTime_cal_PH = 0u;
	uint32_t	unDemo_time = 0xFFFFFFFFu;
	int16_t		DemoCount = 0;

	uint16_t	uFreeRunCount=0u;

	int16_t	loop = 1;
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode = TRUE;
	uint8_t	bDebugMode_flag;
#endif
	
	uint8_t	bAnalogStickMode = FALSE;
	uint8_t	bAnalogStickMode_flag;
	
	uint8_t	bCRTMode = TRUE;
	uint8_t	bCRTMode_flag;

	uint8_t	bBG = TRUE;
	uint8_t	bBG_flag;
	
	uint8_t	bMode;
	
	ST_TASK		stTask = {0}; 
	
	/* �ϐ��̏����� */
	g_mode = 1;
	g_mode_rev = 2u;
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

	/* �f�o�b�O�R�[�i�[ */
#if 0
	puts("�f�o�b�O�R�[�i�[ �J�n");
	/* �����R�ɃR�[�h�������Ă� */
	{
		uint8_t bFlag = FALSE;
		uint8_t bHit = FALSE;
		int32_t	count = 0, count_old = 0;
		
		/* ���X�g�t�@�C���̓ǂݍ��� */
		Init_FileList_Load();
		
		/* �X�[�p�[�o�C�U�[���[�h�J�n */
		g_nSuperchk = _dos_super(0);
		/* ��� */
		g_nCrtmod = CRT_INIT();
		/* �O���t�B�b�N */
		G_INIT();			/* ��ʂ̏����ݒ� */
		G_Palette_INIT();	/* �O���t�B�b�N�p���b�g�̏����� */
		/* �e�L�X�g */
		T_INIT();			/* �e�L�X�g�u�q�`�l������ */
		T_PALET();			/* �e�L�X�g�p���b�g�ݒ� */
		/* �X�v���C�g�^�a�f */
		PCG_INIT();			/* �X�v���C�g�^�a�f�̏����� */
		PCG_VIEW(FALSE);	/* �X�v���C�g���a�f��\�� */

		loop = 1;
		do
		{
			int16_t	input = 0;

			get_keyboard(&input, 0, 1);		/* �L�[�{�[�h���� */
	
			if((input & KEY_b_ESC ) != 0u)		/* �d�r�b�L�[ */
			{
				loop = 0;	/* ���[�v�I�� */
			}
			
			if(ChatCancelSW((input & KEY_A)!=0u, &bFlag) == TRUE)	/* A�{�^�� */
			{
				bHit = TRUE;
			}
			else
			{
				bHit = FALSE;
			}
			
			
			if((input & KEY_B) != 0u)	/* B�{�^�� */
			{
				bHit = TRUE;
			}
			else
			{
			}

			if((input & KEY_UPPER) != 0u)
			{
			}
			else if((input & KEY_LOWER) != 0u)
			{
			}
			else
			{
			}
			
			if((input & KEY_LEFT) != 0u)	
			{
			}
			else if((input & KEY_RIGHT) != 0u)
			{
			}
			else
			{
			}
			
//			printf("p_stPCG[%d]=0x%p\n", i, p_stPCG);
//			KeyHitESC();	/* �f�o�b�O�p */

			if(loop == 0)break;

			/* �����҂� */
			vwait(1);
		}
		while( loop );

		/* ��� */
		CRTMOD(g_nCrtmod);		/* ���[�h�����Ƃɖ߂� */
		
		/*�X�[�p�[�o�C�U�[���[�h�I��*/
		_dos_super(g_nSuperchk);
		
		_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */
	}
	/* �����R�ɃR�[�h�������Ă� */
	puts("�f�o�b�O�R�[�i�[ �I��");
	
	exit(0);
#endif
	
	App_Init();	/* ������ */

	SetTaskInfo(SCENE_INIT);	/* �������V�[���֐ݒ� */
	
	/* ���� */
	srandom(TIMEGET());	/* �����̏����� */
	
	do	/* ���C�����[�v���� */
	{
		uint32_t time_st, time_now;
		int16_t	input = 0;
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
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
		bMode = g_mode;

		/* ���͏��� */
		get_keyboard(&input, 0, 1);		/* �L�[�{�[�h���� */
		if(bAnalogStickMode == TRUE)
		{
			get_ajoy(&input, 0, 1, 1);	/* �A�i���O�W���C�X�e�B�b�N���� 0:X680x0 1:etc */
		}
		else
		{
			get_djoy(&input, 0, 1);		/* �W���C�X�e�B�b�N���� */
		}
		g_Input = input;
		
		/* �I�� */
		if((input & KEY_b_ESC ) != 0u)	/* �d�r�b�L�[ */
		{
			SetTaskInfo(SCENE_EXIT);	/* �I���V�[���֐ݒ� */
		}
		/* �A�i���O�X�e�B�b�N�^�f�W�^���X�e�B�b�N�ؑ� */
		if(ChatCancelSW((input & KEY_b_TAB)!=0u, &bAnalogStickMode_flag) == TRUE)	/* TAB�ŃA�i���O�X�e�B�b�NON/OFF */
		{
			if(bAnalogStickMode == FALSE)	bAnalogStickMode = TRUE;
			else							bAnalogStickMode = FALSE;
		}
		/* CRT 31kHz/15kHz�ؑ� */
		if(ChatCancelSW((input & KEY_b_HELP)!=0u, &bCRTMode_flag) == TRUE)	/* HELP��31kHz/15kHz�ؑ� */
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
		/* BG */
		if(ChatCancelSW((input & KEY_b_G)!=0u, &bBG_flag) == TRUE)	/* �f�o�b�O���ؗp */
		{
			if(bBG == FALSE)	bBG = TRUE;
			else				bBG = FALSE;
		}
		
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		if(ChatCancelSW((input & KEY_b_SP)!=0u, &bDebugMode_flag) == TRUE)	/* �X�y�[�X�Ńf�o�b�OON/OFF */
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
				g_mode = 1;
				g_mode_rev = 2u;
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				g_uDebugNum = 0x80;
#endif
				uFreeRunCount = 0;
				
				/* �O���t�B�b�N */
				G_HOME();			/* �O���t�B�b�N�W���ʒu */
				/* �X�v���C�g���a�f */
				PCG_VIEW(FALSE);	/* �X�v���C�g���a�f��\�� */
				/* �e�L�X�g */
				T_INIT();			/* �e�L�X�g�u�q�`�l������ */
				T_PALET();			/* �e�L�X�g�p���b�g�ݒ� */

				SetTaskInfo(SCENE_TITLE_S);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_TITLE_S:	/* �^�C�g���V�[��(�J�n����) */
			{
				Music_Play(2);	/* �^�C�g���� */
				
				if(CG_File_Load(TITLE_CG) >= 0)	/* �O���t�B�b�N�̓ǂݍ��� */
				{
					G_Load_Mem( TITLE_CG, X_OFFSET, Y_OFFSET, 0 );	/* �^�C�g���摜 */
				}
			
				BG_TextPut("OVER KATA", 96, 128);		/* �^�C�g������ */
				BG_TextPut("PUSH A BUTTON", 80, 160);	/* �{�^�������Ă� */

				GetPassTime( 100, &unDemo_time );		/* ������ */

				SetTaskInfo(SCENE_TITLE);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_TITLE:	/* �^�C�g���V�[�� */
			{
				if(input == KEY_A)	/* A�{�^�� */
				{
					Music_Stop();	/* ���y�Đ� ��~ */
					
					ADPCM_Play(10);	/* SE:���� */
				
					SetTaskInfo(SCENE_TITLE_E);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
				}
				
				if( GetPassTime( 30000, &unDemo_time ) != 0u )	/* ���莞�ԉ����Ȃ���΃f���V�[���� */
				{
					Music_Stop();	/* ���y�Đ� ��~ */
					
//					SetTaskInfo(SCENE_DEMO_S);	/* �f���V�[��(�J�n����)�֐ݒ� */
				}
				break;
			}
			case SCENE_TITLE_E:	/* �^�C�g���V�[��(�I������) */
			{
				/* �X�v���C�g���a�f */
				PCG_VIEW(FALSE);	/* �X�v���C�g���a�f��\�� */

				/* �e�L�X�g�\�� */
				T_INIT();		/* �e�L�X�g�N���A */

				/* �O���t�B�b�N�\�� */
				G_CLR();		/* �O���t�B�b�N�N���A */

				/* MFP */
				MFP_INIT();	/* ���������� */
				
				SetTaskInfo(SCENE_START_S);	/* �Q�[���J�n�V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_DEMO_S:	/* �f���V�[��(�J�n����) */
			{
				uint16_t demo;
				DemoCount = 0;
				
				/* �X�v���C�g���a�f */
				PCG_VIEW(FALSE);	/* �X�v���C�g���a�f��\�� */

				/* �e�L�X�g�\�� */
				T_INIT();		/* �e�L�X�g�N���A */

				/* �O���t�B�b�N�\�� */
				G_CLR();		/* �O���t�B�b�N�N���A */
				
				Music_Play(6);	/* �f���� */
				
				for(demo=0; demo<6; demo++)
				{
					CG_File_Load(DEMO_CG + demo);		/* �O���t�B�b�N�̓ǂݍ��� */
				}

				SetTaskInfo(SCENE_DEMO);	/* �Q�[���J�n�V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_DEMO:	/* �f���V�[�� */
			{
				if( GetPassTime( 2000, &unDemo_time ) != 0u )	/* ���莞�ԉ����Ȃ���Ύ��s */
				{
					if( DemoCount < 6u )	/* �f���V�[�P���X���� */
					{
						G_Load_Mem( DEMO_CG + DemoCount, X_OFFSET, Y_OFFSET+32, 0 );	/* �f���摜 */
					}
					DemoCount++;	/* ���̉摜 */
				}
				
				if( (DemoCount >= 10u) || /* �f���V�[�P���X���� */
					(input == KEY_A) )	/* A�{�^�� */
				{
					Music_Stop();	/* ���y�Đ� ��~ */
				
					SetTaskInfo(SCENE_DEMO_E);	/* �f���V�[��(�I������)�֐ݒ� */
				}
				break;
			}
			case SCENE_DEMO_E:	/* �f���V�[��(�I������) */
			{
				/* �X�v���C�g���a�f */
				PCG_VIEW(FALSE);	/* �X�v���C�g���a�f��\�� */

				/* �O���t�B�b�N�\�� */
				G_CLR();		/* �O���t�B�b�N�N���A */
				
				/* �e�L�X�g�\�� */
				T_INIT();		/* �e�L�X�g�N���A */

				SetTaskInfo(SCENE_TITLE_S);	/* �Q�[���X�^�[�g�^�X�N�֐ݒ� */
				break;
			}
			case SCENE_START_S:	/* �Q�[���J�n�V�[��(�J�n����) */
			{
				/* ���� */
				MOV_Play(0);	/* �X�^�[�g */

				//Music_Play(10);	/* TRUTH */
				Music_Play(11);	/* Mach-1 */
				
				/* ���X�^�[���̏����� */
				Raster_Init();
				
				/* �R�[�X�f�[�^�̏����� */
				Road_Init(1);
				
				G_PaletteSetZero();			/* �O���t�B�b�N�̃p���b�g������ */
				
				SetTaskInfo(SCENE_START);	/* �Q�[���X�^�[�g�^�X�N�֐ݒ� */
				break;
			}
			case SCENE_START:	/* �Q�[���J�n�V�[�� */
			{
				/* �R�[�X��`�悷�� */
				if(input == KEY_B)	/* B�{�^�� */
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
					Music_Play(12);	/* Mach-2 */
					SetTaskInfo(SCENE_START_E);	/* �Q�[���X�^�[�g�^�X�N�֐ݒ� */
				}
				break;
			}
			case SCENE_START_E:	/* �Q�[���J�n�V�[��(�I������) */
			{
//				Music_Play(1);	/* ���[�f�B���O�� */

				Set_CRT_Contrast(0);	/* �R���g���X�g�� */
				
				/* �Q�[�����e�̏����� */
				S_Init_Score();	/* �X�R�A�̏����� */
				
				/* �X�v���C�g���a�f�\�� */
				PCG_INIT();		/* �X�v���C�g�^�a�f�̏����� */
				PCG_VIEW(TRUE);	/* �X�v���C�g���a�f�\�� */

				/* �e�L�X�g�\�� */
				T_Clear();			/* �e�L�X�g�N���A */
				MyCar_G_Load();		/* ���Ԃ̉摜�ǂݍ��� */
				MyCarInfo_Init();	/* ���Ԃ̏�񏉊��� */
				
				/* ���C�o���Ԃ̏����� */
				InitEnemyCAR();

				/* �R�[�X�̃I�u�W�F�N�g�̏����� */
				InitCourseObj();
				
				/* �R�[�X�̔w�i */
				Road_BG_Init(1);
				
				/* �e�L�X�g�\�� */
				T_PALET();		/* �e�L�X�g�p���b�g�ݒ� */
				T_TopScore();	/* TOP */
				T_Time();		/* TIME */
				T_Score();		/* SCORE */
				T_Speed();		/* SPEED */
				T_Gear();		/* GEAR */
				T_SetBG_to_Text();	/* �e�L�X�g�p��Ɨp�f�[�^�W�J */
				
				SetTaskInfo(SCENE_GAME_S);	/* �Q�[��(�J�n����)�^�X�N�֐ݒ� */
				break;
			}
			case SCENE_GAME_S:	/* �Q�[���V�[���J�n���� */
			{
//				Music_Play(3);	/* ���C��BGM */
//				Music_Play(13);	/* Mach-3 */
				Music_Stop();	/* ���y�Đ� ��~ */
				M_SetMusic(0);	/* ���ʉ��Đ��̐ݒ� */

				Set_CRT_Contrast(-1);	/* �R���g���X�gdef */
				
				SetTaskInfo(SCENE_GAME);	/* �Q�[���^�X�N�֐ݒ� */
				break;
			}
			case SCENE_GAME:	/* �Q�[���V�[�� */
			{
				int16_t	Torque = 0;
				int16_t	Raster = 0;

				if((input & KEY_b_Q) != 0u)	/* �p */
				{
					SetTaskInfo(SCENE_GAME_E);	/* �Q�[���V�[��(�I������)�֐ݒ� */
				}
				
				if( (g_bFlip_old == TRUE) && (g_bFlip == FALSE) )	/* �؂�ւ����㔻�� */
				{
					if(bBG == FALSE)
					{
						/* ���Ԃ̏����擾 */
						Torque = MyCarInfo_Update(input);	/* ���Ԃ̏����X�V */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
						if(g_bDebugMode == TRUE)
						{
							GetNowTime(&time_now);
							g_unTime_Pass[bTimePass] = Mmax(time_now - g_unTime_Pass[0], g_unTime_Pass[bTimePass]);	/* 2 */
							bTimePass++;
							g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
						}
#endif
						MyCarInfo_Update16ms(Torque);

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
						if(g_bDebugMode == TRUE)
						{
							GetNowTime(&time_now);
							g_unTime_Pass[bTimePass] = Mmax(time_now - g_unTime_Pass[0], g_unTime_Pass[bTimePass]);	/* 3 */
							bTimePass++;
							g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
						}
#endif
						/* ���X�^�[���� */
						Raster = Raster_Main(bMode);	/* �R�[�X�̏��� */

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
						if(g_bDebugMode == TRUE)
						{
							GetNowTime(&time_now);
							g_unTime_Pass[bTimePass] = Mmax(time_now - g_unTime_Pass[0], g_unTime_Pass[bTimePass]);	/* 4 */
							bTimePass++;
							g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
						}
#endif
					}
					else
					{
						/* ���X�^�[���� */
						Raster = Raster_Main(bMode);	/* �R�[�X�̏��� */
					}
				}
				

				/* �]�������Ԃŏ��� */
				if(bBG == FALSE)
				{
					BG_main(time_st);	/* �o�b�N�O�����h���� */
					
					//g_Vwait = 1;	/* Wait */
					g_Vwait = 0;	/* No Wait */
				}
				else
				{
					/* BG_main�̈ꕔ */
					G_CLR_ALL_OFFSC(bMode);	/* �O���t�B�b�N������ */

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
					Debug_View(uFreeRunCount);	/* �f�o�b�O���\�� */
#endif
					//					Set_DI();	/* ���荞�݋֎~�ݒ� */
					
					SetFlip(TRUE);	/* �t���b�v���� */

//					Set_EI();	/* ���荞�݋֎~���� */
					
					if(input == KEY_B)	/* B�{�^�� */
					{
						g_Vwait = 1;	/* Wait 1 */
					}
					else
					{
						g_Vwait = 0;	/* No Wait */
					}
				}
				
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
				Music_Stop();	/* ���y�Đ� ��~ */

				/* ���� */
				MOV_Play(1);	/* ���ӂ� */

				/* �X�v���C�g���a�f */
				PCG_VIEW(FALSE);	/* �X�v���C�g���a�f��\�� */

				/* �e�L�X�g */
				T_Clear();		/* �e�L�X�g�N���A */
				
				/* MFP */
				MFP_EXIT();		/* MFP�֘A�̉��� */
				
				SetTaskInfo(SCENE_INIT);	/* �������V�[���֐ݒ� */
				break;
			}
			case SCENE_GAME_OVER:	/* �Q�[���I�[�o�[�V�[�� */
			{
				break;
			}
			case SCENE_HI_SCORE:	/* �n�C�X�R�A�����L���O�V�[�� */
			{
				break;
			}
			case SCENE_OPTION:		/* �I�v�V�����V�[�� */
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
	{
		uint32_t i=0, j=4;
		ST_RAS_INFO	stRasInfo;
		GetRasterInfo(&stRasInfo);
		
		printf("====================\n");
		printf("stRasInfo st,mid,ed,size=(%4d,%4d,%4d,%4d)\n", stRasInfo.st, stRasInfo.mid, stRasInfo.ed, stRasInfo.size);
		printf("GetRasterIntPos[i]=(x,y,pat)=(H_pos)\n");
		printf("====================\n");
		for(i=0; i < stRasInfo.size; i++)	/* 2�����\�� */
		{
			uint16_t x, y;
			int16_t pat;
			
			SetRasterIntData(0);	/* 1���� */
			GetRasterIntPos( &x, &y, &pat, i );
			printf("[%3d,%3d]=(%4d,%4d,%4d), ", j++, i, x, y, pat);
			SetRasterIntData(1);	/* 2���� */
			GetRasterIntPos( &x, &y, &pat, i );
			printf("[%3d,%3d]=(%4d,%4d,%4d), ", j++, i, x, y, pat);
			printf("\n");
		}
		printf("====================\n");
	}
#endif
	printf("��������:Real=%3d[ms] PH=%3d[ms]\n", unTime_cal, unTime_cal_PH);
	
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
	puts("App_Init �J�n");

	g_nMaxMemSize = MaxMemSize(1);
	
	/* �Q�[�����e�̏����� */
	S_All_Init_Score();	/* �X�R�A�̏����� */
	SetDebugNum(0x80);	/* �f�o�b�O�l������ */

	/* MFP */
	g_CpuTime = TimerD_INIT();	/* �^�C�}�[D������ */

	/* ���X�g�t�@�C���̓ǂݍ��� */
	Init_FileList_Load();
	
	/* ���y */
	Init_Music();	/* ������(�X�[�p�[�o�C�U�[���[�h���O)	*/
	Music_Play(1);	/* ���[�f�B���O�� */

	/* ���� */
	MOV_INIT();	/* ���������� */

	/* ��� */
	g_nCrtmod = CRT_INIT();
	
	/* �X�[�p�[�o�C�U�[���[�h�J�n */
	g_nSuperchk = _dos_super(0);
	if( g_nSuperchk < 0 ) {
		puts("App_Init ���łɃX�[�p�[�o�C�U�[���[�h");
	} else {
		puts("App_Init �X�[�p�[�o�C�U�[���[�h�J�n");
	}

	/* �e�L�X�g */
	T_INIT();	/* �e�L�X�g�u�q�`�l������ */
	
	/* �O���t�B�b�N */
	G_INIT();			/* ��ʂ̏����ݒ� */
	G_Palette_INIT();	/* �O���t�B�b�N�p���b�g�̏����� */

	/* �X�v���C�g�^�a�f */
	PCG_INIT();	/* �X�v���C�g�^�a�f�̏����� */

	puts("App_Init �I��");
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
	puts("App_exit �J�n");
	
	/* �X�v���C�g���a�f */
	PCG_VIEW(FALSE);		/* �X�v���C�g���a�f��\�� */
	puts("App_exit �X�v���C�g");

	/* ��� */
	CRTMOD(0x100 + g_nCrtmod);	/* ���[�h�����Ƃɖ߂� */
	puts("App_exit ���");

	/* ���y */
	Exit_Music();			/* ���y��~ */
	puts("App_exit ���y");
	
	if(g_bExit == TRUE)
	{
		puts("�G���[���L���b�`�I ESC to skip");
		KeyHitESC();	/* �f�o�b�O�p */
	}
	
	/* MFP */
	TimerD_EXIT();			/* Timer-D�̉��� */
	puts("App_exit Timer-D");
	MFP_EXIT();				/* MFP�֘A�̉��� */
	puts("App_exit MFP");

	/* �e�L�X�g */
	T_EXIT();				/* �e�L�X�g�I������ */
	puts("App_exit �e�L�X�g");

	MyMfree(0);				/* �S�Ẵ���������� */
	puts("App_exit ������");
	
	_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */
	puts("App_exit �L�[�o�b�t�@�N���A");

	/*�X�[�p�[�o�C�U�[���[�h�I��*/
	_dos_super(g_nSuperchk);
	puts("App_exit �X�[�p�[�o�C�U�[���[�h�I��");
	
	puts("App_exit �I��");
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
	uint8_t	bMode, bMode_rev;
	uint8_t	bNum;
	uint8_t	bNumCourse_Obj_Max;
	uint8_t	bNumEnemyCar_Max;
	uint8_t	bFlipStateOld;
	ST_TASK		stTask = {0}; 
	
	static uint8_t	bFlipState = Clear_G;
	
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
	bMode = g_mode;
	bMode_rev = g_mode_rev;
	
//	do
	{
		/* �w�i�̏��� */
		switch(bFlipState)
		{
			/* �`��̃N���A���� */
			case Clear_G:
			{
				G_CLR_ALL_OFFSC(bMode);	/* �O���t�B�b�N������ */
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
			/* ���Ԃ̏��� */
			case MyCar_G:
			{
				MyCar_Interior(bMode);	/* ���Ԃ̃C���e���A���� */
				S_Main_Score();			/* �X�R�A�\�� */
				PCG_Main();				/* �X�v���C�g�o�� */
				T_Main();				/* �e�L�X�g��ʂ̏��� */
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
		if(bFlipState > Flip_G)
		{
			bFlipState = 0;
		}
		BGprocces_ct++;

		GetNowTime(&time_now);	/* ���ݎ������擾 */
		
		if(ret != 0)	/* ���[�v�E�o���� */
		{
			ret = 1;	/* ��ʂ��t���b�v���� */
//			break;
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
//	while(1);
	
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
	int16_t	x, y;
	ST_CRT	stCRT = {0};
	ST_TASK		stTask = {0}; 
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint32_t time_now;
	static uint8_t	bFPS = 0u;
	static uint32_t	unTime_FPS = 0u;
#endif
	g_bFlip_old = g_bFlip;
	
	GetTaskInfo(&stTask);	/* �^�X�N�̏��𓾂� */
	if(stTask.bScene != SCENE_GAME)	/* SCENE_GAME�ȊO�Ȃ�t���b�v���Ȃ� */
	{
		return ret;
	}
	
	if(g_bFlip == FALSE)	/* �`�撆�Ȃ̂Ńt���b�v���Ȃ� */
	{
		return ret;
	}

	SetFlip(FALSE);	/* �t���b�v�֎~ */
	
	/* ��ʂ��t���b�v���� */
	GetCRT(&stCRT, g_mode);
	x = stCRT.hide_offset_x;
	y = stCRT.hide_offset_y;
	_iocs_home( 0b0001, x, y );	/* �I�t�E�X�N���[�����֐ؑ� */
	
	/* ���[�h�`�F���W */
	if(g_mode == 1u)		/* �㑤���� */
	{
		g_mode = 2u;	/* ������ */
		g_mode_rev = 1u;
	}
	else if(g_mode == 2u)	/* �������� */
	{
		g_mode = 1u;	/* �㑤�� */
		g_mode_rev = 2u;
	}
	else					/* ���̑� */
	{
		g_mode = 2u;	/* ������ */
		g_mode_rev = 1u;
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
	g_bFlip = bFlag;
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
void Debug_View(uint16_t uFreeRunCount)
{
	uint8_t bMode;
	
	/* ���[�h�����n�� */
	bMode = g_mode;
	
	if(g_bDebugMode == TRUE)
	{
		uint8_t	str[256] = {0};
		static uint8_t ubDispNum = DEBUG_FREE;
		static uint8_t ubDispNum_flag = 0;

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
					i = Mmin(Mmax(g_uDebugNum, 0), COURSE_OBJ_MAX-1);
					GetCourseObj(&stCourse_Obj, i);	/* ��Q���̏�� */
					sprintf(str, "C_Obj[%d](%4d,%3d,%d)(%6d,%d),Debug(%3d)", i, stCourse_Obj.x, stCourse_Obj.y, stCourse_Obj.z, stCourse_Obj.uTime, stCourse_Obj.ubAlive, g_uDebugNum);	/* ��Q���̏�� */
#endif
				}
				break;
			case DEBUG_ENEMYCAR:
				{
#if 1	/* �G�ԏ�� */
					uint32_t	i = 0;
					ST_ENEMYCARDATA	stEnemyCar = {0};
					i = Mmin(Mmax(g_uDebugNum, 0), ENEMYCAR_MAX-1);
					GetEnemyCAR(&stEnemyCar, i);	/* ���C�o���Ԃ̏�� */
					sprintf(str, "Enemy[%d] (%d)(%4d,%4d,%4d),spd(%3d)", i,
						stEnemyCar.ubAlive,
						stEnemyCar.x,
						stEnemyCar.y,
						stEnemyCar.z,
						stEnemyCar.VehicleSpeed
					);	/* ���C�o���Ԃ̏�� */
#endif
				}
				break;
			case DEBUG_MYCAR:
				{
#if 1	/* ���ԏ�� */
					ST_CARDATA	stMyCar;
					GetMyCar(&stMyCar);	/* ���� */
					sprintf(str, "Car[%d](%4d,%4d,%3d,%d,%d,%3d,%4d,%d)",
							stMyCar.ubCarType,			/* �Ԃ̎�� */
							stMyCar.uEngineRPM,			/* �G���W����]�� */
							stMyCar.Steering,			/* �X�e�A */
							stMyCar.ubThrottle,			/* �X���b�g���J�x */
							stMyCar.ubBrakeLights,		/* �u���[�L���C�g */
							stMyCar.ubHeadLights,		/* �w�b�h���C�g */
							stMyCar.ubWiper,			/* ���C�p�[ */
							stMyCar.bTire,				/* �^�C���̏�� */
							stMyCar.ubOBD				/* �̏�̏�� */
					);	/* ���Ԃ̏�� */
#endif
				}
				break;
			case DEBUG_RASTER:
				{

#if 1	/* ���X�^�[��� */
					uint16_t x, y;
					int16_t pat;
					int16_t pos;
					ST_RAS_INFO	stRasInfo;
					GetRasterInfo(&stRasInfo);

					pos = Mmax(Mmin( g_uDebugNum, stRasInfo.size ), 0);
					GetRasterIntPos( &x, &y, &pat, pos );
					
					sprintf(str, "Ras(%d)s(%d,%d,%d,%d)i(%3d,%3d)(%3d,%3d)[%d]",
						pos,
						stRasInfo.st, stRasInfo.mid, stRasInfo.ed, stRasInfo.size,
						g_stRasterInt[(RASTER_H_MAX - 2)+0].y, g_stRasterInt[(RASTER_H_MAX - 2)+1].y,  
						g_stRasterInt[(RASTER_H_MAX - 4)+0].y, g_stRasterInt[(RASTER_H_MAX - 4)+1].y,
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
					sprintf(str, "%d T%2d(Max%2d),%2d,%2d,%2d,%2d,%2d FPS(%2d)", g_CpuTime, g_unTime_cal, g_unTime_cal_PH, 
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
#if 1	/* ���ł�OK */
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
#if 1
			//			sprintf(str, "Ras %3d, %3d, %3d, V %3d", min, Raster_count, max);
					
			//			sprintf(str, "R=%3d,V=%3d,D=%3d,L=%3d,%3d,%3d,%3d,%3d",
				
					sprintf(str, "V=%3d,L=%3d,R=%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",
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
