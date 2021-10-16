#ifndef	MFP_C
#define	MFP_C

#include <stdio.h>
#include <iocslib.h>
#include <interrupt.h>

#include "inc/usr_macro.h"

#include "OverKata.h"
#include "CRTC.h"
#include "Draw.h"
#include "Graphic.h"
#include "MFP.h"
#include "Moon.h"
#include "OutPut_Text.h"
#include "PCG.h"
#include "Raster.h"

/* define��` */

/* �O���[�o���ϐ� */
uint8_t				g_bRasterSET[2];
uint16_t			g_uRasterLine[8];

volatile uint8_t	g_ubRasterFirst;
volatile uint16_t	g_uRasterFirstPos;
volatile uint16_t	g_uRasterSideNow;
volatile uint16_t	g_uRasterSide;
volatile uint16_t	g_uRas_NexrCount;
volatile uint16_t	g_uRas_Count;
volatile uint16_t	g_uRasterSkipStatus;

volatile uint16_t	g_uCRT_Tmg;

volatile uint16_t	Hsync_count;
volatile uint16_t	Vsync_count;
volatile uint16_t	Raster_count;
volatile uint16_t	RasterLine_count;

//volatile uint16_t *CRTC_R06		= (uint16_t *)0xE8000Cu;	/* �����\���J�n�ʒu-1 */
//volatile uint16_t *CRTC_R09		= (uint16_t *)0xE80012u;	/* ���X�^�[���荞�݈ʒu */
//volatile uint16_t *CRTC_R12		= (uint16_t *)0xE80018u;	/* �X�N���[��0 X */
//volatile uint16_t *CRTC_R14		= (uint16_t *)0xE8001Cu;	/* �X�N���[��1 X */
//volatile uint16_t *VIDEO_REG3		= (uint16_t *)0xE82600u;	/* �O���t�B�b�N�\�� */
//volatile uint16_t *BG_CTRL		= (uint16_t *)0xEB0808u;
volatile uint16_t *BG0scroll_x	= (uint16_t *)0xEB0800;
volatile uint16_t *BG0scroll_y	= (uint16_t *)0xEB0802;
volatile uint16_t *BG1scroll_x	= (uint16_t *)0xEB0804;
volatile uint16_t *BG1scroll_y	= (uint16_t *)0xEB0806;

/* �X�^�e�B�b�N�ϐ� */
static uint8_t g_bTimer_D;
static volatile uint32_t NowTime;
static volatile uint32_t StartTime;
#if 0
static uint32_t g_unTime_cal=0, g_unTime_cal_PH=0, g_unTime_min_PH=0;
#endif

/* �֐��̃v���g�^�C�v�錾 */
int16_t MFP_INIT(void);
int16_t MFP_EXIT(void);
int16_t MFP_RESET(void);
int16_t TimerD_INIT(void);
int16_t TimerD_EXIT(void);
uint8_t GetNowTime(uint32_t *);	/* ���݂̎��Ԃ��擾���� */
uint8_t SetNowTime(uint32_t);		/* ���݂̎��Ԃ�ݒ肷�� */
uint8_t GetStartTime(uint32_t *);	/* �J�n�̎��Ԃ��擾���� */
uint8_t SetStartTime(uint32_t);	/* �J�n�̎��Ԃ�ݒ肷�� */
uint8_t GetPassTime(uint32_t, uint32_t *);	/* �o�߃^�C�}�[ */
int16_t SetRasterIntData(uint16_t);
int16_t GetRasterIntPos(uint16_t *, uint16_t *, uint16_t *, uint16_t);
static void interrupt Timer_D_Func(void);
#if 0
static void interrupt Hsync_Func(void);
#endif
static void interrupt Raster_Func(void);
static void interrupt Vsync_Func(void);

const uint16_t D_RASMAX[2] = { RASTER_L_MAX, RASTER_H_MAX };	/* 0:15kHz 1:31kHz */
const uint16_t D_RAS_INT_MAX[2] = { Y_MAX_WINDOW + 16, 320 + 32};	/* 0:15kHz 1:31kHz */

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t MFP_INIT(void)
{
	int32_t	vdispst = -1;
	volatile uint8_t *MFP_TACR = (uint8_t *)0xe88019;
	volatile uint8_t *MFP_TADR = (uint8_t *)0xe8801f;

	/* ���X�^���荞�� */
	g_ubRasterFirst = TRUE;
	g_uRasterSide = 0;
	g_uRasterSideNow = 0;
	g_bRasterSET[0] = FALSE;
	g_bRasterSET[1] = FALSE;
	g_uRas_Count = 0;
	g_uRas_NexrCount = RASTER_NEXT;	/* ���X�^�[���荞�݃X�L�b�v�J�E���g�ŏ��l�ŏ����� */
	g_uCRT_Tmg = 0;
	memset(g_stRasterInt, 0, sizeof(ST_RASTER_INT) * RASTER_H_MAX);
	/* H-Sync���荞�� */
	Hsync_count = 0;
	/* V-Sync���荞�� */
	Vsync_count = 0;
	/* ���X�^�[���荞�� */
	Raster_count = 0;

	//VDISPST ���񂾂��o�O����iEXCEED.����j
	*MFP_TACR = 0x00;	/* �^�C�}�[�`���~�߂� */
	*MFP_TADR = 0x01;	/* �J�E���^��ݒ�(0=256��) */
	*MFP_TACR = 0x08;	/* �C�x���g�J�E���g���[�h�ɐݒ肷�� */
#ifdef 	MACS_MOON
	vdispst = MACS_Vsync(Vsync_Func);		/* V-Sync���荞�� */
#else	/* MACS_MOON */
	vdispst = VDISPST(Vsync_Func, 0, 1);	/* V-Sync���荞�� �A�� */
#endif	/* MACS_MOON */
//	Message_Num(&vdispst,  0, 10, 2, MONI_Type_SI, "%4d");
	
	return vdispst;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t MFP_EXIT(void)
{
	int32_t	vdispst = -1;

	g_bRasterSET[0] = FALSE;
	g_bRasterSET[1] = FALSE;

	CRTCRAS((void *)0, 0);		/* stop */
	puts("MFP_EXIT CRTCRAS");
	
	HSYNCST((void *)0);			/* stop */
	puts("MFP_EXIT HSYNCST");

#ifdef 	MACS_MOON
	vdispst = MACS_Vsync_R(Vsync_Func);	/* stop */
	puts("MFP_EXIT MACS_Vsync_R");
#else	/* MACS_MOON */
	vdispst = VDISPST((void *)0, 0, 0);	/* stop */
	puts("MFP_EXIT VDISPST");
#endif	/* MACS_MOON */
	
//	Message_Num(&vdispst,  6, 10, 2, MONI_Type_SI, "%4d");

	return vdispst;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t MFP_RESET(void)
{
	int16_t	ret = 0;

	volatile uint8_t *MFP_INTCTRL_A = (uint8_t *)0xE88007;	/* ���荞�݃C�l�[�u�����W�X�^A */
	volatile uint8_t *MFP_TADR  	= (uint8_t *)0xE8801F;	/* �^�C�}�[A�f�[�^���W�X�^ */
	
	*MFP_TADR = 1;					/* �J�E���^�ݒ� */
	*MFP_INTCTRL_A |= 0b01000000;	/* CRTC Raster���荞�݋��� */
//	*MFP_INTCTRL_A |= 0b00100000;	/* Timer-A���荞�݋��� */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	TimerD_INIT()
{
	int16_t CpuCount=0;
	/* Timer-D���荞�� */
	NowTime = 0;
	StartTime = 0;
	TIMERDST(Timer_D_Func, 7, 20);	/* Timer-D�Z�b�g */	/* 50us(7) x 20cnt = 1ms */
//	TIMERDST(Timer_D_Func, 3, 5);	/* Timer-D�Z�b�g */	/* 4us(3) x 5cnt = 20us */
	g_bTimer_D =TRUE;
	SetNowTime(NowTime);	/* ���Ԃ̏����� */
	/* �}�C�R���N���b�N���v�� */
	do
	{
		CpuCount++;
	}
	while(NowTime==0);

	return CpuCount;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	TimerD_EXIT()
{
	TIMERDST((void *)0, 0, 1);	/* stop */
	g_bTimer_D = FALSE;

	return 0;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
uint8_t GetNowTime(uint32_t *time)	/* ���݂̎��Ԃ��擾���� */
{
	*time = NowTime;
	return g_bTimer_D;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
uint8_t SetNowTime(uint32_t time)	/* ���݂̎��Ԃ�ݒ肷�� */
{
	NowTime = time;
	return g_bTimer_D;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
uint8_t GetStartTime(uint32_t *time)	/* �J�n�̎��Ԃ��擾���� */
{
	*time = StartTime;
	return g_bTimer_D;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
uint8_t SetStartTime(uint32_t time)	/* �J�n�̎��Ԃ�ݒ肷�� */
{
	StartTime = time;
	return g_bTimer_D;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	����\1ms */
/*===========================================================================================*/
uint8_t GetPassTime(uint32_t uSetTimer, uint32_t *pOldTimer)	/* �o�߃^�C�}�[ */
{
	uint8_t ret = FALSE;
	uint32_t uTime, uTimeDiff, uTimeMem;
	
	uTime = NowTime;
	uTimeMem = (*pOldTimer);
	
	uTimeDiff = uTime - uTimeMem;
	
	if(uSetTimer <= uTimeDiff)
	{
		*pOldTimer = uTime;
		ret = TRUE;
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
int16_t SetRasterIntData(uint16_t uSide)
{
	int16_t ret = 0;
	
	g_bRasterSET[1] = TRUE;
	g_uRasterSide = uSide;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t GetRasterIntPos(uint16_t *x, uint16_t *y, uint16_t *pat, uint16_t uNum)
{
	int16_t	ret = 0;

	/* ���݃A�N�e�B�u�ȃ��X�^�[���荞�݃f�[�^���擾 */
	uNum = Mmul2(uNum) + g_uRasterSide;

	if(uNum >= D_RASMAX[0])
	{
		uNum = D_RASMAX[0] - 1;
		ret = -1;
	}
	
	if(x !=  NULL)
	{
		*x = g_stRasterInt[uNum].x;
	}
	else
	{
		/* nop */
	}
	
	if(y !=  NULL)
	{
		*y = g_stRasterInt[uNum].y;
	}
	else
	{
		/* nop */
	}
	
	if(pat !=  NULL)
	{
		*pat = g_stRasterInt[uNum].pat;
	}
	else
	{
		/* nop */
	}
	
	return ret;
}

/* ���荞�݊֐��͏C���q��interrupt�����邱�� */	/* ���ɂ����iKunihiko Ohnaka�j����̃A�h�o�C�X */

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static void interrupt Timer_D_Func(void)
{
	NowTime++;

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
#if 0
static void interrupt Hsync_Func(void)
{
	HSYNCST((void *)0);			/* stop */
	Hsync_count++;
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}
#endif
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* ���X�^�[���荞�݂̏������͕K�v�Œ������ */	/* EXCEED.����̃A�h�o�C�X */
/* �Q���C����΂����炢�����x�̌��E */	/* EXCEED.����̃A�h�o�C�X */
static void interrupt Raster_Func(void)
{
	uint16_t nNum;
	
	Set_DI();	/* ���荞�݋֎~�ݒ� */
	
	nNum = (g_uRas_Count - g_uRasterFirstPos) + g_uRasterSideNow;
	
	g_uRas_Count += g_uRas_NexrCount;		/* ���̃��X�^���荞�݈ʒu�̌v�Z */
	*CRTC_R09 = g_uRas_Count;				/* ���̃��X�^���荞�݈ʒu�̐ݒ� */
	
	if(g_uRas_Count < D_RAS_INT_MAX[g_uCRT_Tmg])	/* ���X�^�[���荞�݈ʒu�̍Ō� */
	{
		
		uint16_t x, y;
		
		g_bRasterSET[0] = TRUE;
		
		//	*CRTC_R12		= g_stRasterInt[nNum].x + X_OFFSET;			/* GR��X���W�̐ݒ� */
		if(g_ubRasterFirst == TRUE)
		{
			g_ubRasterFirst = FALSE;	/* ���񏈗��I�� */
			/* �w�i����(���X�^�[���荞�݊J�n�ʒu����O���t�B�b�N�\������߂邱�ƂŁA��������BG��藠���Ɍ�����) */
			*VIDEO_REG3		= Mbset(*VIDEO_REG3,   0x0C, 0b0000000000000000);	/* GRSC1(GR3,GR4)=OFF */

			y = g_stRasterInt[nNum].y;
			
		}
		else
		{
			y = g_stRasterInt[nNum].y + g_stRasterInt[nNum].pat;
		}
		x = g_stRasterInt[nNum].x;

		/* BG0 */
		*BG0scroll_x	= x;	/* BG0��X���W�̐ݒ� */
		*BG0scroll_y	= y;	/* BG0��Y���W�̐ݒ� */
		/* BG1 */
		*BG1scroll_x	= x;	/* BG1��X���W�̐ݒ� */
		*BG1scroll_y	= y;	/* BG1��Y���W�̐ݒ� */
	}
	else
	{
		g_bRasterSET[0] = FALSE;
		
		CRTCRAS((void *)0, 0);		/* stop */
		g_uRas_Count = g_uRasterFirstPos;		/* �ŏ��ɖ߂��Ă��� */
		*CRTC_R09 = 255;			/* ���̃��X�^���荞�݈ʒu�̐ݒ� */
		
		*BG0scroll_y	= D_RAS_INT_MAX[g_uCRT_Tmg];	/* BG0��Y���W�̐ݒ� */
		*BG1scroll_y	= D_RAS_INT_MAX[g_uCRT_Tmg];	/* BG1��Y���W�̐ݒ� */
		
		Raster_count++;	/* ���X�^���荞�݈�A�̏��������� */
	}
	
	RasterLine_count++;	/* ���X�^�[���荞�ݏ����� */
	
	Set_EI();	/* ���荞�݋֎~���� */
	
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static void interrupt Vsync_Func(void)
{
	uint16_t x, y;
#if 0
	uint32_t time_now;
	static uint32_t unTime_old = 0;
#endif
	
//	Set_DI();	/* ���荞�݋֎~�ݒ� */
	
	/* V-Sync���荞�ݏ��� */
	Vsync_count++;
	
	/* V-Sync���荞�ݏ��� */
#ifdef 	MACS_MOON
	/* �������Ȃ� */
	/* MACS_Vsync�o�R�ł���Ί��荞�݋֎~��ԂŎ��s����Ă��邽�߁A���荞�݋֎~���u�s�v */
#else	/* MACS_MOON */
	Set_DI();	/* ���荞�݋֎~�ݒ� */
	vdispst = VDISPST((void *)0, 0, 0);	/* stop */
	Set_EI();	/* ���荞�݋֎~���� */
#endif	/* MACS_MOON */
	
	if( g_bRasterSET[1] == TRUE )	/* ���X�^���荞�݉\ */
	{
		/* �w�i�\���i���荞�݊J�n�O�́A�w�i��\�������Ă����j */
		*VIDEO_REG3 = Mbset(*VIDEO_REG3,   0x0C, 0b0000000000001100);	/* GR2(GR3,GR4)=ON */

		if(g_bRasterSET[0] == TRUE)	/* ���X�^���荞�ݏ����� */
		{
			/* ���X�^���荞�ݏ������Ȃ̂ŉ������Ȃ� */
			g_uRasterSkipStatus = g_uGameStatus;
		}
		else						/* ���X�^���荞�ݏ������łȂ� */
		{
			uint16_t uSide;
			
			g_uRasterSkipStatus = 0;
			
			/* ���X�^���荞�� */
			CRTCRAS((void *)0, 0);	/* stop */

			uSide = g_uRasterSide;
			
			/* ���X�^���荞�ݐݒ� */
			g_ubRasterFirst = TRUE;	/* ���X�^���荞�݂ŏ��񂾂��������邽�߂̃t���O */

			if(g_uCRT_Tmg != 0)	/* ���X�^�Q�x�ǂ݂̏ꍇ(31kHz) */
			{
				/* ���j�^�̕\���J�n�ʒu��CRTC_R06���ŏ��̈ʒu�ƂȂ� */
				/* ���荞�݊J�n�\���ʒu(�\����ʊJ�n�ʒu�{�X�v���C�g�I�t�Z�b�g�{���荞�݂��������W) */
				/* SP/BG�́A���_-16�̃I�t�Z�b�g������̂�SP/BG�ɑ΂��Ċ��荞�݂��s���ꍇ�A�I�t�Z�b�g���𑫂��K�v������ */
				g_uRasterFirstPos = *CRTC_R06 + g_stRasterInt[(RASTER_H_MAX - 2) + uSide].y;
			}
			else				/* �m���C���^�[���[�X���[�h�̏ꍇ(15kHz) */
			{
				/* ���X�^ */
				g_uRasterFirstPos = *CRTC_R06 + g_stRasterInt[(RASTER_H_MAX - 2) + uSide].y + SP_Y_OFFSET;	/* ���荞�݊J�n�ʒu */	/* ���t�B�[���h�̂� */
			}
	//		�C���^�[���[�X�̏ꍇ	g_uRasterFirstPos = Mmul2((*CRTC_R09 - *CRTC_R06)) + g_stRasterInt[0 + uSide].y + SP_Y_OFFSET;	/* ���荞�݊J�n�ʒu */	/* ���t�B�[���h�̂� */
			
			/* ���X�^���荞�� */
			g_uRas_Count = g_uRasterFirstPos;
			g_uRasterSideNow = uSide;
			CRTCRAS( Raster_Func, g_uRas_Count );	/* ���X�^�[���荞�� */

			/* BG */
			x = 256;
			y = g_stRasterInt[(RASTER_H_MAX - 4) + uSide].y;
	//		*BG_CTRL = Mbset(*BG_CTRL, 0x200, 0x000);	/* DISP/CPU=OFF */
			*BG0scroll_x	= x;	/* BG0��X���W�̐ݒ� *//* ��̃R���g���[�� */
			*BG0scroll_y	= y;	/* BG0��Y���W�̐ݒ� *//* ��̃R���g���[�� */
			*BG1scroll_x	= x;	/* BG1��X���W�̐ݒ� *//* ��̃R���g���[�� */
			*BG1scroll_y	= y;	/* BG1��Y���W�̐ݒ� *//* ��̃R���g���[�� */
	//		*BG_CTRL = Mbset(*BG_CTRL, 0x200, 0x200);	/* DISP/CPU=ON */

		}
	}
	else	/* ���X�^���荞�ݕs�� */
	{
#if 0	/* �f�o�b�O�\�� */
		/* ���X�^�[���荞�� */
		Raster_count = 0;
		
		g_unTime_cal = 0;
		g_unTime_cal_PH = 0;
		g_unTime_min_PH = 0;
#endif
	}
	
	if(g_bRasterSET[0] == TRUE)	/* ���X�^���荞�ݏ����� */
	{
		/* ���X�^���荞�ݏ������Ȃ̂ŉ������Ȃ� */
	}
	else						/* ���X�^���荞�ݏ������łȂ� */
	{
		/* ��ʐ؂�ւ� */
		FlipProc();
	}
	
#if 0	/* �f�o�b�O�\�� */
	if( g_bRasterSET[1] == TRUE )
	{
		g_uRasterLine[7] = g_uRasterLine[6];
		g_uRasterLine[6] = g_uRasterLine[5];
		g_uRasterLine[5] = g_uRasterLine[4];
		g_uRasterLine[4] = g_uRasterLine[3];
		g_uRasterLine[3] = g_uRasterLine[2];
		g_uRasterLine[2] = g_uRasterLine[1];
		g_uRasterLine[1] = g_uRasterLine[0];
		g_uRasterLine[0] = RasterLine_count + (g_uRasterSkipStatus * 100);
		
		/* �������Ԍv�� */
		GetNowTime(&time_now);
		g_unTime_cal = time_now - unTime_old;	/* LSB:1 UNIT:ms */
		if(unTime_old != 0)
		{
			g_unTime_min_PH = Mmin(g_unTime_cal, g_unTime_min_PH);
			g_unTime_cal_PH = Mmax(g_unTime_cal, g_unTime_cal_PH);
		}
		unTime_old = time_now;
		
		if(g_bRasterSET[0] == TRUE)	/* ���X�^���荞�ݏ����� */
		{
//			Debug_View(Vsync_count);	/* �f�o�b�O���\�� */
		}
		
		RasterLine_count = 0;
	}
#endif
	
	/* H-Sync���荞�ݏ��� */
	Hsync_count = 0;
#if 0
	HSYNCST(Hsync_Func);	/* H-Sync���荞�� */
#endif
	
#ifdef 	MACS_MOON
	/* �������Ȃ� */
#else	/* MACS_MOON */
	vdispst = VDISPST(Vsync_Func, 0, 1);	/* V-Sync���荞�� �A�� */
#endif	/* MACS_MOON */

#ifdef	MACS_MOON
	IRTS();	/* MACSDRV�ł�rts�ň����K�v���� */
#else	/* MACS_MOON */
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
#endif	/* MACS_MOON */
	
}

#endif	/* MFP_H */
