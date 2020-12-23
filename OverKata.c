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
SI	g_nSuperchk = 0;
SI	g_nCrtmod = 0;
UC	g_mode = 0;
UC	g_mode_rev = 1;
US	g_uDebugNum = 0; 
UC	g_bDebugMode = FALSE;

/* �O���[�o���\���� */

/* �֐��̃v���g�^�C�v�錾 */
SS main(void);
static void App_Init(void);
static void App_exit(void);
SS	BG_main(UC*);
SS	GetDebugNum(US *);
SS	SetDebugNum(US);
SS	GetDebugMode(UC *);
SS	SetDebugMode(UC);

void (*usr_abort)(void);	/* ���[�U�̃A�{�[�g�����֐� */

/* �֐� */
SS main(void)
{
	SS	ret = 0;

	UI	unTime_cal = 0u;
	UI	unTime_cal_PH = 0u;
	
	US	uFreeRunCount=0u;
	
	SS	loop = 1;
	SS	RD[1024] = {0};
	UC	bMode_flag = FALSE;
	UC	bDebugMode = TRUE;
	UC	bDebugMode_flag;
	UC	bFlip = TRUE;
	
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
		US	uOld;
		UI	x = 0, y = 0;
		UI	dx = 0, dy = 0;
		US	count = 0;
		US	*pBuf = NULL;
		US	*pDstGR = NULL;
		US	*pSrcGR = NULL;

		g_uDebugNum = 0x1;
		uOld = !g_uDebugNum;
		/* �X�[�p�[�o�C�U�[���[�h�J�n */
		g_nSuperchk = SUPER(0);

		/* ��� */
		g_nCrtmod = CRT_INIT();

		/* �O���t�B�b�N */
		G_INIT();			/* ��ʂ̏����ݒ� */
		G_Palette_INIT();	/* �O���t�B�b�N�p���b�g�̏����� */
		/* �f�[�^�̓ǂݍ��� */
		CG_File_Load();		/* �O���t�B�b�N�f�[�^�ǂݍ��� */

		/* �e�L�X�g */
		T_INIT();	/* �e�L�X�g�u�q�`�l������ */
		
//		pBuf = (US*)MyMalloc( Mmax((52*512*2), 512*2) );
//		APICG_DataLoad2M( 1, x, y, 0, pBuf);
//		dx = 128;
//		dy = 128;
//		for(y=0; y < 52; y++)
//		{
//			pDstGR = (US *)(0xC00000 + ((dy + y) << 10) + (dx << 1));
//			pSrcGR = pBuf + (y << 9);
//			
//			for(x=0; x < 64; x++)
//			{
//				if(*pSrcGR == 0xB4)
//				{
//					*pSrcGR = 0x00;
//				}
//				*pDstGR = *pSrcGR & 0x00FF;
//				pDstGR++;
//				pSrcGR++;
//			}
//		}
		
		puts("�d�r�b�L�[�ŏI��");
		loop = 1;
		do
		{
			if( ( BITSNS( 0 ) & 0x02 ) != 0 ) loop = 0;	/* �d�r�b�|�[�Y */
			if(loop == 0)break;

			DirectInputKeyNum(&g_uDebugNum, 1);	/* �L�[�{�[�h���琔������� */
			
			if(g_uDebugNum != uOld)
			{
//				pDstGR = (US*)0xC00000;
//				memset(pDstGR, 0x42, 0x80000);
				
				uOld = g_uDebugNum;
				//G_Load_Mem(g_uDebugNum, dx, dy, 0);
				G_Load_Mem( 0, X_OFFSET,	0,			0);	/* �C���e���A */
				G_Load_Mem( 0, X_OFFSET,	Y_OFFSET,	0);	/* �C���e���A */
			}

			if(count == 0)
			{
				x = 0;
				y = 128;
			}
			else
			{
//				UI uWidth, uHeight, uFileSize;
//				Get_PicImageInfo(1, &uWidth, &uHeight, &uFileSize);
//				G_CLR_AREA(x, uWidth, y, uHeight, 0);
//				x = count % 256;
//				y = 64 + ((64 * APL_Cos((x<<1)%360)) >> 8);
//				G_Load_Mem(2, 64, 64, 0);
//				G_Load_Mem(1, x, y, 0);
			}
			//printf("%5d = (%4d, %4d)\n", count, x, y );
			
			count++;

			/* �����҂� */
			vwait(1);
		}
		while( loop );
		
		MyMfree(pBuf);	/* ��������� */

		/* ��� */
		CRTMOD(g_nCrtmod);			/* ���[�h�����Ƃɖ߂� */

		/*�X�[�p�[�o�C�U�[���[�h�I��*/
		SUPER(g_nSuperchk);

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
		SS a,b,c,d;
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
		UI time_st, time_now;
		SS	input = 0;
		
		/* �����ݒ� */
		GetNowTime(&time_st);	/* ���C�������̊J�n�������擾 */
		SetStartTime(time_st);	/* ���C�������̊J�n�������L�� */

		/* �^�X�N���� */
		TaskManage();			/* �^�X�N���Ǘ����� */
		GetTaskInfo(&stTask);	/* �^�X�N�̏��𓾂� */

		/* ���͏��� */
		get_key(&input, 0, 1);	/* �L�[�{�[�h���W���C�X�e�B�b�N���� */
		
		if((input & KEY_b_ESC ) != 0u)		/* �d�r�b�L�[ */
		{
			loop = 0;	/* ���[�v�I�� */
		}
		if(loop == 0)	/* �I������ */
		{
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
					ADPCM_Play(10);	/* SE:���� */
				
					Music_Play(1);	/* ���[�f�B���O�� */
					//Music_Play(3);	/* �Q�[���� */
					
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
				/* �X�v���C�g���a�f�\�� */
				PCG_INIT();		/* �X�v���C�g�^�a�f�̏����� */
				PCG_VIEW(TRUE);	

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
				SetTaskInfo(SCENE_GAME);	/* �Q�[���^�X�N�֐ݒ� */
			}
			break;
			case SCENE_GAME:	/* �Q�[���V�[�� */
			{
				if((input & KEY_b_Q) != 0u)	/* �p */
				{
					/* ���� */
					MOV_Play(1);	/* ���ӂ� */

					SetTaskInfo(SCENE_GAME_E);	/* �Q�[���V�[��(�I������)�֐ݒ� */
				}
				
				/* ���Ԃ̏����擾 */
				MyCarInfo_Update(input);	/* ���Ԃ̏����X�V */
				
				/* ���X�^�[���� */
				Raster_Main(g_mode);
				
				if(stTask.b96ms == TRUE)
				{
					/* �R�[�X�A�E�g���̏��� */
					MyCar_CourseOut();	/* �R�[�X�A�E�g���̃G�t�F�N�g */

					/* �O���t�B�b�N��ʂ̏��� */
					MyCar_Interior(g_mode);	/* ���Ԃ̃C���e���A���� */
				}
				
				/* �e�L�X�g��ʂ̏��� */
				if(stTask.b496ms == TRUE)
				{
					T_Main();
				}

				/* �]�������Ԃŏ��� */
				BG_main(&bFlip);	/* �o�b�N�O�����h���� */
				
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
		}

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
//		BG_TextPut("OverKata", 4, 10);
//		BG_TextPut("OverKata", 128, 128);
//		BG_TextPut("OVER KATA", 128, 128);

		if(bDebugMode == TRUE)
		{
			if(stTask.b496ms == TRUE)
			{
				/* ���j�^ */
//				Message_Num(&	,				 0,  9, 2, MONI_Type_SS, "%3d");
//				Message_Num(&, 				 6,  9, 2, MONI_Type_SS, "%2d");
//				Message_Num(&, 				12,  9, 2, MONI_Type_SS, "%2d");
//				Message_Num(&,		 			20,  9, 2, MONI_Type_SS, "%3d");
				
//				ST_RAS_INFO	stRasInfo;
//				GetRasterInfo(&stRasInfo);
//				Message_Num(&stRasInfo.st,			 0, 10, 2, MONI_Type_US, "%3d");
//				Message_Num(&stRasInfo.mid,			 7, 10, 2, MONI_Type_US, "%3d");
//				Message_Num(&stRasInfo.ed,			13, 10, 2, MONI_Type_US, "%3d");
//				Message_Num(&stRasInfo.size,		20, 10, 2, MONI_Type_US, "%3d");
				
//				Message_Num(&uPal_tmp[uRas_st],	 0, 11, 2, MONI_Type_US, "%3d");
//				Message_Num(&uRas_tmp[uRas_st],	 6, 11, 2, MONI_Type_US, "%3d");
//				Message_Num(&cal_tan,			12, 11, 2, MONI_Type_SS, "%3d");
//				Message_Num(&rad,				12, 11, 2, MONI_Type_FL, "%f");
				
//				Message_Num(&road_height,		 0, 12, 2, MONI_Type_SS, "%3d");
//				Message_Num(&road_slope,	 	 6, 12, 2, MONI_Type_SS, "%3d");
//				Message_Num(&road_distance,		12, 12, 2, MONI_Type_SS, "%3d");
//				Message_Num(&road_angle,		20, 12, 2, MONI_Type_SS, "%3d");
				
				Message_Num(&unTime_cal,	 	 0, 8, 2, MONI_Type_UI, "%3d");
				Message_Num(&unTime_cal_PH,		 6, 8, 2, MONI_Type_UI, "%3d");
				Message_Num(&g_uDebugNum,		12, 8, 2, MONI_Type_US, "%3d");
				
			}
		}
#endif
		uFreeRunCount++;	/* 16bit �t���[�����J�E���^�X�V */

		/* �������Ԍv�� */
		GetNowTime(&time_now);
		if( stTask.bScene == SCENE_GAME )
		{
			unTime_cal = time_now - time_st;	/* LSB:1 UNIT:ms */
			unTime_cal_PH = Mmax(unTime_cal, unTime_cal_PH);
		}

		/* �����҂� */
		vwait(1);
	}
	while( loop );
	
	App_exit();	/* �I������ */

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	{
		UI i=0, j=0;
		ST_RAS_INFO	stRasInfo;
		GetRasterInfo(&stRasInfo);
		
		printf("stRasInfo st,mid,ed,size=(%4d,%4d,%4d,%4d)\n", stRasInfo.st, stRasInfo.mid, stRasInfo.ed, stRasInfo.size);
		
		for(i=stRasInfo.st; i < stRasInfo.ed; i+=RASTER_NEXT)
		{
			US x, y;
			SS pat;
			
			GetRasterIntPos( &x, &y, &pat, i );
			
			printf("[%3d]=(%4d,%4d,%4d)=(%4d), ", i, x, y, pat, (y - RASTER_MIN + i - 128) );
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
	TimerD_INIT();	/* �^�C�}�[D������ */

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

SS BG_main(UC* bFlip)
{
	SS	ret = 0;
	UI	time_now;
	UI	time_st;
	US	BGprocces_ct = 0;
	UC	bNum;
	UC	bFlipStateOld;

	static UC	bFlipState = Clear_G;
	
	GetStartTime(&time_st);	/* �J�n�������擾 */

	bFlipStateOld = bFlipState;

	/* �`�揇���\�[�g���� */
	Sort_EnemyCAR();		/* ���C�o���� */
	Sort_Course_Obj();		/* �R�[�X�I�u�W�F�N�g */
	
	do
	{
		GetNowTime(&time_now);	/* ���ݎ������擾 */
		
		if((time_now - time_st) >= 14)	/* 14ms�ȓ��Ȃ�]�������Ԃŏ������� */
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
				G_CLR_ALL_OFFSC(g_mode);

//				bFlipState = Object1_G;
				bFlipState++;
				*bFlip = FALSE;
				break;
			}
			/* �w�i */
			case BackGround_G:
			{
				/* FPS */
				if(g_mode == 2)
				{
					PutGraphic_To_Symbol("test2", X_OFFSET + 128, Y_OFFSET + 48, 20);
					//G_Load_Mem( 6, X_OFFSET,	Y_OFFSET,	1);	/* �w�i */
				}
				else
				{
					PutGraphic_To_Symbol("test1", X_OFFSET + 128, Y_MIN_DRAW + 48, 20);
					//G_Load_Mem( 6, X_OFFSET,	Y_MIN_DRAW,	1);	/* �w�i */
				}
				bFlipState++;
				*bFlip = FALSE;
				break;
			}
			/* ���V�̖�(E:�E�� / O:����) */
			case Object1_G:
			case Object2_G:
			case Object3_G:
			case Object4_G:
			case Object5_G:
			case Object6_G:
			{
				bNum = bFlipState - Object1_G;
				Course_Obj_main(bNum, g_mode, g_mode_rev);
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
				bNum = bFlipState - Enemy1_G;
				EnemyCAR_main(bNum, g_mode, g_mode_rev);
				bFlipState++;
				*bFlip = FALSE;
				break;
			}
			case Flip_G:
			{
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
	}
	while(1);
	
	return	ret;
}

SS	GetDebugNum(US *uNum)
{
	SS	ret = 0;
	*uNum = g_uDebugNum;
	return ret;
}

SS	SetDebugNum(US uNum)
{
	SS	ret = 0;
	g_uDebugNum = uNum;
	return ret;
}

SS	GetDebugMode(UC *bMode)
{
	SS	ret = 0;
	*bMode = g_bDebugMode;
	return ret;
}

SS	SetDebugMode(UC bMode)
{
	SS	ret = 0;
	g_bDebugMode = bMode;
	return ret;
}


#endif	/* OVERKATA_C */
