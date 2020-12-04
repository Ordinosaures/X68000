#ifndef	MFP_C
#define	MFP_C

#include <iocslib.h>
#include <interrupt.h>

#include "inc/usr_macro.h"

#include "OverKata.h"
#include "MFP.h"
#include "Moon.h"
#include "OutPut_Text.h"
#include "Raster.h"

/* �ϐ� */
static UC g_bTimer_D;
static UC g_bRasterSET;
static volatile UI NowTime;
static volatile UI StartTime;
static volatile US ras_count;
static volatile US Hsync_count;
static volatile US Vsync_count;

static ST_RASTER_INT g_stRasterInt[RASTER_MAX] = {0};

/* �֐��̃v���g�^�C�v�錾 */
SS MFP_INIT(void);
SS MFP_EXIT(void);
SS MFP_RESET(void);
SS TimerD_INIT(void);
SS TimerD_EXIT(void);
UC GetNowTime(UI *);	/* ���݂̎��Ԃ��擾���� */
UC SetNowTime(UI);		/* ���݂̎��Ԃ�ݒ肷�� */
UC GetStartTime(UI *);	/* �J�n�̎��Ԃ��擾���� */
UC SetStartTime(UI);	/* �J�n�̎��Ԃ�ݒ肷�� */
SS SetRasterIntData(void *, size_t);
SS GetRasterIntPos(US *, US *, US *, US);
static void interrupt Timer_D_Func(void);
static void interrupt Hsync_Func(void);
static void interrupt Raster_Func(void);
static void interrupt Vsync_Func(void);
SS vwait(SS);

/* �֐� */
SS MFP_INIT(void)
{
	SI	vdispst = -1;
	volatile UC *MFP_TACR = (UC *)0xe88019;
	volatile UC *MFP_TADR = (UC *)0xe8801f;

	/* ���X�^���荞�� */
	g_bRasterSET = FALSE;
	ras_count = 0;
	memset(g_stRasterInt, 0, sizeof(ST_RASTER_INT) * RASTER_MAX);
	/* H-Sync���荞�� */
	Hsync_count = 0;
	/* V-Sync���荞�� */
	Vsync_count = 0;

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

SS MFP_EXIT(void)
{
	SI	vdispst = -1;

	g_bRasterSET = FALSE;

	CRTCRAS((void *)0, 0);		/* stop */
	HSYNCST((void *)0);			/* stop */
#ifdef 	MACS_MOON
	vdispst = MACS_Vsync_R(Vsync_Func);	/* stop */
#else	/* MACS_MOON */
	vdispst = VDISPST((void *)0, 0, 0);	/* stop */
#endif	/* MACS_MOON */
//	Message_Num(&vdispst,  6, 10, 2, MONI_Type_SI, "%4d");

	return vdispst;
}

SS MFP_RESET(void)
{
	SS	ret = 0;

	volatile UC *MFP_INTCTRL_A  = (UC *)0xE88007;	/* ���荞�݃C�l�[�u�����W�X�^A */
	volatile UC *MFP_TADR  		= (UC *)0xE8801F;	/* �^�C�}�[A�f�[�^���W�X�^ */
	
	*MFP_TADR = 1;					/* �J�E���^�ݒ� */
	*MFP_INTCTRL_A |= 0b01000000;	/* CRTC Raster���荞�݋��� */
//	*MFP_INTCTRL_A |= 0b00100000;	/* Timer-A���荞�݋��� */
	
	return ret;
}

SS	TimerD_INIT()
{
	/* Timer-D���荞�� */
	NowTime = 0;
	StartTime = 0;
	TIMERDST(Timer_D_Func, 7, 20);	/* Timer-D�Z�b�g */	/* 50us(7) x 20cnt = 1ms */
	g_bTimer_D =TRUE;
	SetNowTime(NowTime);	/* ���Ԃ̏����� */

	return 0;
}

SS	TimerD_EXIT()
{
	TIMERDST((void *)0, 0, 1);	/* stop */
	g_bTimer_D = FALSE;

	return 0;
}

UC GetNowTime(UI *time)	/* ���݂̎��Ԃ��擾���� */
{
	*time = NowTime;
	return g_bTimer_D;
}

UC SetNowTime(UI time)	/* ���݂̎��Ԃ�ݒ肷�� */
{
	NowTime = time;
	return g_bTimer_D;
}

UC GetStartTime(UI *time)	/* �J�n�̎��Ԃ��擾���� */
{
	*time = StartTime;
	return g_bTimer_D;
}

UC SetStartTime(UI time)	/* �J�n�̎��Ԃ�ݒ肷�� */
{
	StartTime = time;
	return g_bTimer_D;
}

SS SetRasterIntData(void *pSrc, size_t n)
{
	SS ret = 0;
	if(memcpy(g_stRasterInt, pSrc, n) == NULL)
	{
		ret = -1;
	}
	else
	{
		g_bRasterSET = TRUE;
	}
	return ret;
}

SS GetRasterIntPos(US *x, US *y, US *pat, US uNum)
{
	SS	ret = 0;

	if(uNum < 256)
	{
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
	}
	
	return ret;
}

/* ���荞�݊֐��͏C���q��interrupt�����邱�� */	/* ���ɂ����iKunihiko Ohnaka�j����̃A�h�o�C�X */

static void interrupt Timer_D_Func(void)
{
	NowTime++;

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

static void interrupt Hsync_Func(void)
{
	HSYNCST((void *)0);			/* stop */
	Hsync_count++;
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

/* ���X�^�[���荞�݂̏������͕K�v�Œ������ */	/* EXCEED.����̃A�h�o�C�X */
/* �Q���C����΂����炢�����x�̌��E */	/* EXCEED.����̃A�h�o�C�X */
static void interrupt Raster_Func(void)
{
	volatile US *BG0scroll_x  = (US *)0xEB0800;
	volatile US *BG0scroll_y  = (US *)0xEB0802;
//	volatile US *BG1scroll_x  = (US *)0xEB0804;
//	volatile US *BG1scroll_y  = (US *)0xEB0806;
	volatile US *CRTC_R09 = (US *)0xE80012u;	/* ���X�^�[���荞�݈ʒu */
//	volatile US *CRTC_R12 = (US *)0xE80018u;	/* �X�N���[��0 X */
//	volatile US *CRTC_R14 = (US *)0xE8001Cu;	/* �X�N���[��1 X */

	US nNum = ras_count;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	UC	bDebugMode;
	US	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
	ras_count += RASTER_NEXT;		/* ���̃��X�^���荞�݈ʒu�̌v�Z */
	*CRTC_R09 = ras_count;			/* ���̃��X�^���荞�݈ʒu�̐ݒ� */

	*BG0scroll_x	= g_stRasterInt[nNum].x;	/* BG0��X���W�̐ݒ� */
	*BG0scroll_y	= g_stRasterInt[nNum].y + g_stRasterInt[nNum].pat;	/* BG0��Y���W�̐ݒ� */
//	*BG0scroll_y	= uDebugNum + g_stRasterInt[nNum].pat;	/* BG0��Y���W�̐ݒ� */
//	*BG1scroll_x	= g_stRasterInt[nNum].x;	/* BG1��X���W�̐ݒ� */
//	*BG1scroll_y	= g_stRasterInt[nNum].y + g_stRasterInt[nNum].pat;	/* BG1��Y���W�̐ݒ� */
//	*BG1scroll_x	= 256;						/* BG1��X���W�̐ݒ� *//* ��̃R���g���[�� */
//	*BG1scroll_y	= g_stRasterInt[1].y;		/* BG1��Y���W�̐ݒ� *//* ��̃R���g���[�� */
//	*CRTC_R12		= g_stRasterInt[nNum].x + X_OFFSET;			/* GR��X���W�̐ݒ� */

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

static void interrupt Vsync_Func(void)
{
	volatile US *BG0scroll_x  = (US *)0xEB0800;
	volatile US *BG0scroll_y  = (US *)0xEB0802;
	volatile US *BG1scroll_x  = (US *)0xEB0804;
	volatile US *BG1scroll_y  = (US *)0xEB0806;
//	volatile US *BGCTRL		  = (US *)0xEB0808;

	/* V-Sync���荞�ݏ��� */
#ifdef 	MACS_MOON
	/* �������Ȃ� */
#else	/* MACS_MOON */
//	SI	vdispst = -1;
	vdispst = VDISPST((void *)0, 0, 0);	/* stop */
#endif	/* MACS_MOON */

//	*BGCTRL = Mbclr(*BGCTRL, Bit_9);
	*BG0scroll_x	= 256;					/* BG0��X���W�̐ݒ� *//* ��̃R���g���[�� */
	*BG0scroll_y	= g_stRasterInt[1].y;	/* BG0��Y���W�̐ݒ� *//* ��̃R���g���[�� */
	*BG1scroll_x	= 256;					/* BG1��X���W�̐ݒ� *//* ��̃R���g���[�� */
	*BG1scroll_y	= g_stRasterInt[1].y;	/* BG1��Y���W�̐ݒ� *//* ��̃R���g���[�� */

	/* H-Sync���荞�ݏ��� */
	Hsync_count = 0;
//	HSYNCST(Hsync_Func);					/* H-Sync���荞�� */

	/* ���X�^���荞�� */
	CRTCRAS((void *)0, 0);	/* stop */
	if(g_bRasterSET == TRUE)
	{
		ras_count = g_stRasterInt[0].y;		/* ���荞�݊J�n�ʒu */	/* �����0�Ŏn�܂邱�� */
		CRTCRAS( Raster_Func, ras_count );	/* ���X�^�[���荞�� */
	}
#if 0
	if((Vsync_count % 10) < 5)
	{
		GPALET( 1, SetRGB(16, 16, 16));	/* Glay */
		GPALET( 2, SetRGB(15, 15, 15));	/* D-Glay */
		
		GPALET( 5, SetRGB(31,  0,  0));	/* Red */
		GPALET(15, SetRGB(31, 31, 31));	/* White */
		
		GPALET( 8, SetRGB( 0, 31,  0));	/* Green */
		GPALET(11, SetRGB( 0, 28,  0));	/* Green */
	}
	else
	{
		GPALET( 2, SetRGB(16, 16, 16));	/* Glay */
		GPALET( 1, SetRGB(15, 15, 15));	/* D-Glay */
		
		GPALET(15, SetRGB(31,  0,  0));	/* Red */
		GPALET( 5, SetRGB(31, 31, 31));	/* White */
		
		GPALET(11, SetRGB( 0, 31,  0));	/* Green */
		GPALET( 8, SetRGB( 0, 28,  0));	/* Green */
	}
#endif

	/* V-Sync���荞�ݏ��� */
	Vsync_count++;
#ifdef 	MACS_MOON
	/* �������Ȃ� */
#else	/* MACS_MOON */
	vdispst = VDISPST(Vsync_Func, 0, 1);	/* V-Sync���荞�� �A�� */
#endif	/* MACS_MOON */

#if 0
	{
		volatile US *CRTC_R21 = (US *)0xE8002Au;	/* �e�L�X�g�E�A�N�Z�X�E�Z�b�g�A�N���A�[P.S */
		volatile US *CRTC_480 = (US *)0xE80480u;	/* CRTC����|�[�g */

		if((*CRTC_480 & 0x02u) == 0u)		/* �N���A���s�łȂ� */
		{
			*CRTC_R21 = Mbset(*CRTC_R21, 0x0Fu, 0x0Cu);	/* SCREEN1 �����N���AON / SCREEN0 �����N���AOFF */
			*CRTC_480 = Mbset(*CRTC_480, 0x02u, 0x02u);	/* �N���A���s */
		}
	}
#endif

//	Message_Num(&g_bRasterSET,  0, 10, 2, MONI_Type_UC, "%4d");
//	Message_Num(&Vsync_count, 	6, 10, 2, MONI_Type_US, "%4d");

#ifdef	MACS_MOON
	IRTS();	/* MACSDRV�ł�rts�ň����K�v���� */
#else	/* MACS_MOON */
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
#endif	/* MACS_MOON */
}

SS vwait(SS count)				/* ��count�^60�b�҂�	*/
{
	SS ret = 0;
	volatile UC *mfp = (UC *)0xe88001;
//	volatile US *BGCTRL = (US *)0xEB0808;
//	*BGCTRL = Mbset(*BGCTRL, Bit_9, Bit_9);
	
	while(count--){
		while(!((*mfp) & 0b00010000));	/* �����\�����ԑ҂�	*/
		while((*mfp) & 0b00010000);	/* �����A�����ԑ҂�	*/
	}

	return ret;
}

#endif	/* MFP_H */
