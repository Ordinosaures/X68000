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
#include "Task.h"
#include "Text.h"
#include "Trap14.h"

/* �O���[�o���ϐ� */
int32_t	g_nSuperchk = 0;
int32_t	g_nCrtmod = 0;
uint8_t	g_mode = 0;
uint8_t	g_mode_rev = 1;
uint16_t	g_uDebugNum = 0; 
uint8_t	g_bDebugMode = FALSE;
int16_t	g_CpuTime = 0;
uint32_t	g_unTime_cal = 0u;
uint32_t	g_unTime_cal_PH = 0u;
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
uint32_t	g_unTime_Pass[6] = {0u};
#endif
int16_t	g_Input;

enum{
	DEBUG_NONE,
	DEBUG_COURSE_OBJ,
	DEBUG_ENEMYCAR,
	DEBUG_MYCAR,
	DEBUG_RASTER,
	DEBUG_CPUTIME,
	DEBUG_MAX,
};

/* �O���[�o���\���� */

/* �֐��̃v���g�^�C�v�錾 */
int16_t main(void);
static void App_Init(void);
static void App_exit(void);
int16_t	BG_main(uint8_t*);
int16_t	GetGameMode(uint8_t *);
int16_t	SetGameMode(uint8_t);
int16_t	GetDebugNum(uint16_t *);
int16_t	SetDebugNum(uint16_t);
int16_t	GetDebugMode(uint8_t *);
int16_t	SetDebugMode(uint8_t);

void (*usr_abort)(void);	/* ���[�U�̃A�{�[�g�����֐� */

/* �֐� */
int16_t main(void)
{
	int16_t	ret = 0;

	uint32_t	unTime_cal = 0u;
	uint32_t	unTime_cal_PH = 0u;
	
	uint16_t	uFreeRunCount=0u;
	
	int16_t	loop = 1;
	int16_t	RD[1024] = {0};
	uint8_t	bMode_flag = FALSE;
	uint8_t	bDebugMode = TRUE;
	uint8_t	bDebugMode_flag;
	uint8_t	bFlip = FALSE;
	
	ST_TASK		stTask = {0}; 
	
	usr_abort = App_exit;	/* ��O�����ŏI�����������{���� */

	init_trap14();	/* �f�o�b�O�p�v���I�G���[�n���h�����O */

	/* �ϐ��̏����� */
	g_mode = 1;
	g_mode_rev = 2u;
	g_uDebugNum = 0x80;
	bDebugMode = TRUE;

	/* �f�o�b�O�R�[�i�[ */
#if 0
	puts("�f�o�b�O�R�[�i�[ �J�n");
	/* �����R�ɃR�[�h�������Ă� */
	{
		uint8_t bFlag = FALSE;
		uint8_t bHit = FALSE;
		int16_t	count = 0;
		
		Init_Music();	/* ������(�X�[�p�[�o�C�U�[���[�h���O)	*/
		
		/* �X�[�p�[�o�C�U�[���[�h�J�n */
		g_nSuperchk = SUPER(0);

//		CRTC_INIT();	/* ����𑜓x */
		
//		M_SetMusic(0);	/* �����ݒ� */
//		Music_Play(1);	/* �������̂� */
		
		puts("�d�r�b�L�[�ŏI��");
		loop = 1;
		do
		{
			int16_t	input = 0;
			get_key(&input, 0, 1);	/* �L�[�{�[�h���W���C�X�e�B�b�N���� */
			if((input & KEY_b_ESC ) != 0u)		/* �d�r�b�L�[ */
			{
				loop = 0;	/* ���[�v�I�� */
			}
			
			if(ChatCancelSW((input & KEY_A)!=0u, &bFlag) == TRUE)	/* A�{�^�� */
			{
				M_Play(count);
				bHit = TRUE;
			}
			else
			{
				bHit = FALSE;
			}
			
			if((input & KEY_B) != 0u)	/* B�{�^�� */
			{
				if(count < 9000)
				{
					count++;
				}
			}
			else
			{
				if(count > 0)
				{
					count--;
				}
			}
			printf("Count = %6d, %d\r", count, bHit);

			if(loop == 0)break;
		}
		while( loop );

		/*�X�[�p�[�o�C�U�[���[�h�I��*/
		SUPER(g_nSuperchk);

		/* ���y */
		Exit_Music();			/* ���y��~ */
		
		_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */
	}
	/* �����R�ɃR�[�h�������Ă� */
	puts("�f�o�b�O�R�[�i�[ �I��");
	
	exit(0);
#endif
	
#if 0
	{
		/* �A�h���X�G���[���� */
		char buf[10];

		int *A = (int *)&buf[1];
		int B = *A;
		return B;
	}
#endif
	
	App_Init();	/* ������ */

	SetTaskInfo(SCENE_INIT);	/* �������V�[���֐ݒ� */
	
	/* ���� */
	{
		/* �����̏����� */
		int16_t a,b,c,d;
		a = 0;
		b = 0;
		c = 0;

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
	
	do	/* ���C�����[�v���� */
	{
		uint32_t time_st, time_now;
		int16_t	input = 0;
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		uint8_t	bTimePass = 1;
#endif
		
		/* �����ݒ� */
		GetNowTime(&time_st);	/* ���C�������̊J�n�������擾 */
		SetStartTime(time_st);	/* ���C�������̊J�n�������L�� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		g_unTime_Pass[0] = time_st;
#endif
		
		/* �^�X�N���� */
		TaskManage();			/* �^�X�N���Ǘ����� */
		GetTaskInfo(&stTask);	/* �^�X�N�̏��𓾂� */

		/* ���͏��� */
		get_key(&input, 0, 1);	/* �L�[�{�[�h���W���C�X�e�B�b�N���� */
		g_Input = input;
		if((input & KEY_b_ESC ) != 0u)		/* �d�r�b�L�[ */
		{
			loop = 0;	/* ���[�v�I�� */
		}
		if(loop == 0)	/* �I������ */
		{
			Music_Stop();	/* ���y�Đ� ��~ */
			
			/* ���� */
			MOV_Play(2);	/* �o�C�o�C */
			break;
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

		switch(stTask.bScene)
		{
			case SCENE_INIT:	/* �������V�[�� */
			{
				/* �ϐ��̏����� */
				g_mode = 1;
				g_mode_rev = 2u;
				g_uDebugNum = 0x80;
				uFreeRunCount = 0;

				/* �O���t�B�b�N */
				G_HOME();			/* �O���t�B�b�N�W���ʒu */
				/* �X�v���C�g���a�f */
				PCG_VIEW(FALSE);	/* �X�v���C�g���a�f��\�� */
				/* �e�L�X�g */
				T_INIT();			/* �e�L�X�g�u�q�`�l������ */
				T_PALET();			/* �e�L�X�g�p���b�g�ݒ� */

				SetTaskInfo(SCENE_TITLE_S);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
			}
			break;
			case SCENE_TITLE_S:	/* �^�C�g���V�[��(�J�n����) */
			{
				Music_Play(2);	/* �^�C�g���� */
				
				G_Load_Mem( 8, X_OFFSET, Y_OFFSET, 0 );	/* �^�C�g���摜 */
			
				BG_TextPut("OVER KATA", 96, 128);		/* �^�C�g������ */
				BG_TextPut("PUSH A BUTTON", 80, 160);	/* �{�^�������Ă� */

				SetTaskInfo(SCENE_TITLE);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
			}
			break;
			case SCENE_TITLE:	/* �^�C�g���V�[�� */
			{
				if(input == KEY_A)	/* A�{�^�� */
				{
					Music_Stop();	/* ���y�Đ� ��~ */
					
					ADPCM_Play(10);	/* SE:���� */
				
					SetTaskInfo(SCENE_TITLE_E);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
				}
			}
			break;
			case SCENE_TITLE_E:	/* �^�C�g���V�[��(�I������) */
			{
				/* �X�v���C�g���a�f */
				PCG_VIEW(FALSE);	/* �X�v���C�g���a�f��\�� */

				/* �e�L�X�g�\�� */
				T_Clear();		/* �e�L�X�g�N���A */

				/* �e�L�X�g�\�� */
				G_CLR();		/* �O���t�B�b�N�N���A */

				/* MFP */
				MFP_INIT();	/* ���������� */
				
				SetTaskInfo(SCENE_START_S);	/* �^�C�g���V�[��(�J�n����)�֐ݒ� */
			}
			break;
			case SCENE_DEMO:	/* �f���V�[�� */
			{
				
			}
			break;
			case SCENE_START_S:	/* �Q�[���J�n�V�[��(�J�n����) */
			{
				SetTaskInfo(SCENE_START);	/* �Q�[���X�^�[�g�^�X�N�֐ݒ� */
			}
			break;
			case SCENE_START:	/* �Q�[���J�n�V�[�� */
			{
				/* ���� */
				MOV_Play(0);	/* �X�^�[�g */

				SetTaskInfo(SCENE_START_E);	/* �Q�[���X�^�[�g�^�X�N�֐ݒ� */
			}
			break;
			case SCENE_START_E:	/* �Q�[���J�n�V�[��(�I������) */
			{
				Set_CRT_Contrast(0);	/* �R���g���X�g�� */
				
				/* �X�v���C�g���a�f�\�� */
				PCG_INIT();		/* �X�v���C�g�^�a�f�̏����� */
				PCG_VIEW(TRUE);	/* �X�v���C�g���a�f�\�� */

				/* �e�L�X�g�\�� */
				T_Clear();			/* �e�L�X�g�N���A */
				MyCar_G_Load();		/* ���Ԃ̉摜�ǂݍ���*/
				MyCarInfo_Init();
				
				/* ���C�o���Ԃ̏����� */
				InitEnemyCAR();

				/* �R�[�X�̃I�u�W�F�N�g�̏����� */
				InitCourseObj();

				/* �R�[�X�f�[�^�̏����� */
				Road_Init(1);
				
				/* ���X�^�[���̏����� */
				Raster_Init();
				
				/* �e�L�X�g�\�� */
				T_PALET();		/* �e�L�X�g�p���b�g�ݒ� */
				T_TopScore();	/* TOP */
				T_Time();		/* TIME */
				T_Score();		/* SCORE */
				T_Speed();		/* SPEED */
				T_Gear();		/* GEAR */
				T_SetBG_to_Text();	/* �e�L�X�g�p��Ɨp�f�[�^�W�J */
				
				SetTaskInfo(SCENE_GAME_S);	/* �Q�[��(�J�n����)�^�X�N�֐ݒ� */
			}
			case SCENE_GAME_S:	/* �Q�[���V�[���J�n���� */
			{
//				Music_Play(3);	/* ���C��BGM */
//				Music_Stop();	/* ���y�Đ� ��~ */
//				M_SetMusic(0);	/* ���ʉ��Đ��̐ݒ� */

				Set_CRT_Contrast(-1);	/* �R���g���X�gdef */
				
				SetTaskInfo(SCENE_GAME);	/* �Q�[���^�X�N�֐ݒ� */
			}
			break;
			case SCENE_GAME:	/* �Q�[���V�[�� */
			{
				if((input & KEY_b_Q) != 0u)	/* �p */
				{
					Music_Stop();	/* ���y�Đ� ��~ */

					/* ���� */
					MOV_Play(1);	/* ���ӂ� */

					SetTaskInfo(SCENE_GAME_E);	/* �Q�[���V�[��(�I������)�֐ݒ� */
				}
				
				/* ��ʏ��� */
				if(bFlip == TRUE)
				{
					/* �O���t�B�b�N������ */
					G_CLR_ALL_OFFSC(g_mode);
				}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				if(g_bDebugMode == TRUE)
				{
					GetNowTime(&time_now);
					g_unTime_Pass[bTimePass] = time_now - g_unTime_Pass[0];	/* 1 */
					bTimePass++;
					g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
				}
#endif
				/* ���Ԃ̏����擾 */
				MyCarInfo_Update(input);	/* ���Ԃ̏����X�V */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				if(g_bDebugMode == TRUE)
				{
					GetNowTime(&time_now);
					g_unTime_Pass[bTimePass] = time_now - g_unTime_Pass[0];	/* 2 */
					bTimePass++;
					g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
				}
#endif
				
				/* ���X�^�[���� */
				Raster_Main(g_mode);
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				if(g_bDebugMode == TRUE)
				{
					GetNowTime(&time_now);
					g_unTime_Pass[bTimePass] = time_now - g_unTime_Pass[0];	/* 3 */
					bTimePass++;
					g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
				}
#endif
				
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				if(g_bDebugMode == TRUE)
				{
					GetNowTime(&time_now);
					g_unTime_Pass[bTimePass] = time_now - g_unTime_Pass[0];	/* 4 */
					bTimePass++;
					g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
				}
#endif
				/* �]�������Ԃŏ��� */
				BG_main(&bFlip);	/* �o�b�N�O�����h���� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				if(g_bDebugMode == TRUE)
				{
					GetNowTime(&time_now);
					g_unTime_Pass[bTimePass] = time_now - g_unTime_Pass[0];	/* 5 */
					bTimePass++;
					g_unTime_Pass[0] = time_now;	/* �ꎞ�ۑ� */
				}
#endif
			}
			break;
			case SCENE_GAME_E:	/* �Q�[���V�[��(�I������) */
			{
				/* �X�v���C�g���a�f */
				PCG_VIEW(FALSE);	/* �X�v���C�g���a�f��\�� */
				
				/* �e�L�X�g */
				T_Clear();		/* �e�L�X�g�N���A */
				
				/* MFP */
				MFP_EXIT();		/* MFP�֘A�̉��� */
				
				SetTaskInfo(SCENE_INIT);	/* �������V�[���֐ݒ� */
			}
			break;
			case SCENE_GAME_OVER:	/* �Q�[���I�[�o�[�V�[�� */
			{
			}
			break;
			case SCENE_HI_SCORE:	/* �n�C�X�R�A�����L���O�V�[�� */
			{
				
			}
			break;
			case SCENE_OPTION:		/* �I�v�V�����V�[�� */
			{
				
			}
			break;
			case SCENE_EXIT:		/* �I���V�[�� */
			{
				
			}
			break;
			default:	/* �ُ�V�[�� */
			{
				
			}
			break;
		}

		if( (ChatCancelSW((input & KEY_b_M)!=0u, &bMode_flag) == TRUE) || (bFlip == TRUE) )	/* �l�Ń��[�h�ؑ� */
		{
			int16_t	x, y;
			ST_CRT	stCRT = {0};
			/* ���[�h�`�F���W */
			if(g_mode == 1u)
			{
				g_mode = 2u;
				g_mode_rev = 1u;
			}
			else
			{
				g_mode = 1u;
				g_mode_rev = 2u;
			}
			/* ��ʂ��t���b�v���� */
			GetCRT(&stCRT, g_mode);
			x = stCRT.view_offset_x;
			y = stCRT.view_offset_y;
			/* ��ʂ̈ʒu */
			HOME(0b01, x, y );	/* Screen 0(TPS/FPS) */
			T_Scroll( 0, y  );	/* �e�L�X�g��� */
		}

		uFreeRunCount++;	/* 16bit �t���[�����J�E���^�X�V */

		/* �������Ԍv�� */
		GetNowTime(&time_now);
		if( stTask.bScene == SCENE_GAME )
		{
			unTime_cal = time_now - time_st;	/* LSB:1 UNIT:ms */
			unTime_cal_PH = Mmax(unTime_cal, unTime_cal_PH);
			
			g_unTime_cal = unTime_cal;
			g_unTime_cal_PH = unTime_cal_PH;
		}

		/* �����҂� */
		vwait(1);
	}
	while( loop );
	
	App_exit();	/* �I������ */

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	{
		uint32_t i=0, j=0;
		uint32_t st;
		ST_RAS_INFO	stRasInfo;
		GetRasterInfo(&stRasInfo);
		
		printf("stRasInfo st,mid,ed,size=(%4d,%4d,%4d,%4d)\n", stRasInfo.st, stRasInfo.mid, stRasInfo.ed, stRasInfo.size);
		printf("GetRasterIntPos[i]=(x,y,pat)=(H_pos)\n");
		
		st = stRasInfo.st;
		for(i=st; i < stRasInfo.ed; i+=RASTER_NEXT)
		{
			uint16_t x, y;
			int16_t pat;
			
			GetRasterIntPos( &x, &y, &pat, i );
			
			printf("[%3d]=(%4d,%4d,%4d)=(%4d), ", i, x, y, pat, i - st/*(y + i - ROAD_ST_POINT)*/ );
			if((j%3) == 0)printf("\n");
			j++;
		}
		printf("\n");
	}
#endif
	printf("��������:Real=%3d[ms] PH=%3d[ms]\n", unTime_cal, unTime_cal_PH);
	
	return ret;
}

static void App_Init(void)
{
	puts("App_Init �J�n");

	/* �f�o�b�O�l������ */
	SetDebugNum(0x80);

	/* MFP */
	g_CpuTime = TimerD_INIT();	/* �^�C�}�[D������ */

	/* ���y */
	Init_Music();	/* ������(�X�[�p�[�o�C�U�[���[�h���O)	*/
	Music_Play(1);	/* ���[�f�B���O�� */
	
	/* �X�[�p�[�o�C�U�[���[�h�J�n */
	g_nSuperchk = SUPER(0);
	if( g_nSuperchk < 0 ) {
		puts("���łɃX�[�p�[�o�C�U�[���[�h");
	} else {
		puts("�X�[�p�[�o�C�U�[���[�h�J�n");
	}
	
	/* ��� */
	g_nCrtmod = CRT_INIT();

	/* �O���t�B�b�N */
	G_INIT();			/* ��ʂ̏����ݒ� */
	G_Palette_INIT();	/* �O���t�B�b�N�p���b�g�̏����� */

	/* �f�[�^�̓ǂݍ��� */
	CG_File_Load();		/* �O���t�B�b�N�f�[�^�ǂݍ��� */

	/* ���� */
	MOV_INIT();	/* ���������� */

	/* �X�v���C�g�^�a�f */
	PCG_INIT();	/* �X�v���C�g�^�a�f�̏����� */

	/* �e�L�X�g */
	T_INIT();	/* �e�L�X�g�u�q�`�l������ */

	puts("App_Init �I��");
}

static void App_exit(void)
{
	puts("App_exit �J�n");
	
	/* MFP */
	MFP_EXIT();				/* MFP�֘A�̉��� */
	TimerD_EXIT();			/* Timer-D�̉��� */

	/* ���y */
	Exit_Music();			/* ���y��~ */
	
	/* �e�L�X�g */
	T_EXIT();				/* �e�L�X�g�I������ */

	/* �X�v���C�g���a�f */
	PCG_VIEW(FALSE);		/* �X�v���C�g���a�f��\�� */

	/* ��� */
	CRTMOD(g_nCrtmod);		/* ���[�h�����Ƃɖ߂� */

	MyMfree(0);				/* �S�Ẵ���������� */
	
	_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */

	/*�X�[�p�[�o�C�U�[���[�h�I��*/
	SUPER(g_nSuperchk);
	
	puts("App_exit �I��");
}

int16_t BG_main(uint8_t* bFlip)
{
	int16_t	ret = 0;
	uint32_t	time_now;
	uint32_t	time_st;
	uint16_t	BGprocces_ct = 0;
	uint8_t	bNum;
	uint8_t	bFlipStateOld;
	ST_TASK		stTask = {0}; 

	static uint8_t	bFlipState = Clear_G;
	
	GetStartTime(&time_st);	/* �J�n�������擾 */
	GetTaskInfo(&stTask);	/* �^�X�N�̏��𓾂� */

	bFlipStateOld = bFlipState;

	do
	{
		GetNowTime(&time_now);	/* ���ݎ������擾 */
		
		if((time_now - time_st) >= 500)	/* 28ms�ȓ��Ȃ�]�������Ԃŏ������� */
		{
			break;
		}
		
		/* �w�i�̏��� */
		/* �`��̃N���A���� */
		switch(bFlipState)
		{
			/* �`��̃N���A���� */
			case Clear_G:
			{
				bFlipState++;
				*bFlip = FALSE;
				break;
			}
			/* �w�i */
			case BackGround_G:
			{
				if((time_now - time_st) >= 28)	/* 28ms�ȓ��Ȃ�]�������Ԃŏ������� */
				{
					bFlipState++;
				}
				else
				{
					Move_Course_BG(g_mode);	/* �R�[�X�̓����ɂ��킹�Ĕw�i�𓮂��� */
				}
				
				bFlipState++;
				*bFlip = FALSE;
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
			case Object8_G:
			case Object9_G:
			case ObjectA_G:
			case ObjectB_G:
			case ObjectC_G:
			case ObjectD_G:
			case ObjectE_G:
			case ObjectF_G:
			{
				if((time_now - time_st) >= 28)	/* 28ms�ȓ��Ȃ�]�������Ԃŏ������� */
				{
					bFlipState++;
				}
				else
				{
					bNum = bFlipState - Object0_G;
					if(bNum < COURSE_OBJ_MAX)
					{
						/* �`�揇���\�[�g���� */
						if(bNum == 0)
						{
							Sort_Course_Obj();		/* �R�[�X�I�u�W�F�N�g */
						}
						Course_Obj_main(bNum, g_mode, g_mode_rev);
					}
				}
				
				bFlipState++;
				*bFlip = FALSE;
				break;
			}
			/* ���C�o���� */
			case Enemy1_G:
			case Enemy2_G:
			case Enemy3_G:
			case Enemy4_G:
			{
				if((time_now - time_st) >= 28)	/* 28ms�ȓ��Ȃ�]�������Ԃŏ������� */
				{
					bFlipState++;
				}
				else
				{
					bNum = bFlipState - Enemy1_G;
					if(bNum < ENEMYCAR_MAX)
					{
						/* �`�揇���\�[�g���� */
						if(bNum == 0)
						{
							Sort_EnemyCAR();		/* ���C�o���� */
						}
						
						EnemyCAR_main(bNum, g_mode, g_mode_rev);
					}
				}
				
				bFlipState++;
				*bFlip = FALSE;
				break;
			}
			case MyCar_G:
			{
				if((time_now - time_st) >= 28)	/* 28ms�ȓ��Ȃ�]�������Ԃŏ������� */
				{
					bFlipState++;
				}
				else
				{
					T_Main(g_mode);	/* �e�L�X�g��ʂ̏��� */

					MyCar_Interior(g_mode);	/* ���Ԃ̃C���e���A���� */
				}
				
				bFlipState++;
				*bFlip = FALSE;
				break;
			}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
			case Debug_View_G:
			{
				if(g_bDebugMode == TRUE)
				{
					uint8_t	str[256] = {0};
					static uint8_t ubDispNum = DEBUG_MYCAR;
					static uint8_t ubDispNum_flag = 0;

					if(ChatCancelSW((g_Input & KEY_b_RLUP)!=0u, &ubDispNum_flag) == TRUE)	/* ���[���A�b�v�ŕ\���ؑ� */
					{
						ubDispNum++;
						if(DEBUG_MAX <= ubDispNum)
						{
							ubDispNum = DEBUG_NONE;
						}
					}

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
							GetRasterIntPos( &x, &y, &pat, stRasInfo.st + pos );
							
							sprintf(str, "Ras[st+(%d)]s(%d,%d,%d,%d)i(%3d,%3d,%3d)",
								pos,
								stRasInfo.st, stRasInfo.mid, stRasInfo.ed, stRasInfo.size,
								x, y, pat
							);	/* ���X�^�[��� */
#endif
						}
						break;
					case DEBUG_CPUTIME:
						{
#if 1	/* CPU��� */
//							sprintf(str, "CPU Time%2d[ms](MAX%2d[ms]),Debug(%3d)", g_unTime_cal, g_unTime_cal_PH, g_uDebugNum);	/* �������� */
							sprintf(str, "%d Time[ms]%2d(Max%2d),%2d,%2d,%2d,%2d,%2d", g_CpuTime, g_unTime_cal, g_unTime_cal_PH, 
									g_unTime_Pass[1],
									g_unTime_Pass[2],
									g_unTime_Pass[3],
									g_unTime_Pass[4],
									g_unTime_Pass[5]
							);
#endif
						}
						break;
					default:
						break;
					}
					/* �\�� */
					Put_Message_To_Graphic(str, g_mode);	/* �O���t�B�b�N�̃f�o�b�O�G���A�Ƀ��b�Z�[�W�`�� */
				}
				
				bFlipState++;
				*bFlip = FALSE;
				break;
			}
#endif
			case Flip_G:
			{
				/* ��ʂ��t���b�v���� */
				bFlipState = Clear_G;
				*bFlip = TRUE;
				break;
			}
			default:
			{
				bFlipState = Clear_G;
				*bFlip = FALSE;
				break;
			}
		}
		
		BGprocces_ct++;
		
		if(*bFlip == TRUE)	/* �ؑ֔��莞�͑����[�v�I�� */
		{
			break;
		}
		
		if(bFlipStateOld == bFlipState)	/* �X�e�[�g����������烋�[�v�I�� */
		{
			break;
		}

		if((time_now - time_st) >= 14)	/* 14ms�ȓ��Ȃ�]�������Ԃŏ������� */
		{
			break;
		}
	}
	while(1);
	
	return	ret;
}

int16_t	GetGameMode(uint8_t *bMode)
{
	int16_t	ret = 0;
	*bMode = g_mode;
	return ret;
}

int16_t	SetGameMode(uint8_t bMode)
{
	int16_t	ret = 0;
	g_mode = bMode;
	return ret;
}

int16_t	GetDebugNum(uint16_t *uNum)
{
	int16_t	ret = 0;
	*uNum = g_uDebugNum;
	return ret;
}

int16_t	SetDebugNum(uint16_t uNum)
{
	int16_t	ret = 0;
	g_uDebugNum = uNum;
	return ret;
}

int16_t	GetDebugMode(uint8_t *bMode)
{
	int16_t	ret = 0;
	*bMode = g_bDebugMode;
	return ret;
}

int16_t	SetDebugMode(uint8_t bMode)
{
	int16_t	ret = 0;
	g_bDebugMode = bMode;
	return ret;
}

#endif	/* OVERKATA_C */
