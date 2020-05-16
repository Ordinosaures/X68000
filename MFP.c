#ifndef	MFP_C
#define	MFP_C

#include <iocslib.h>
#include <interrupt.h>

#include "inc/usr_macro.h"

#include "OverKata.h"
#include "MFP.h"

/* �ϐ� */
static volatile UI NowTime;
static volatile US ras_count;
static volatile US Hsync_count;
static volatile US Vsync_count;

static US ras_val[256];
static US ras_pal[256];

/* �֐��̃v���g�^�C�v�錾 */
SS Init_MFP(void);
void interrupt Timer_D_Func(void);
SS GetNowTime(UI *);	/* ���݂̎��Ԃ��擾���� */
SS SetNowTime(UI);		/* ���݂̎��Ԃ�ݒ肷�� */
void interrupt Hsync_Func(void);
void interrupt Raster_Func(void);

SS SetRasterVal(void *, size_t);		/* ���X�^�[��p�̃o�b�t�@�ɃR�s�[���� */
SS SetRasterPal(void *, size_t);		/* ���X�^�[��p(PAL)�̃o�b�t�@�ɃR�s�[���� */
void interrupt Vsync_Func(void);
SS vwait(SS);

/* �֐� */
SS Init_MFP(void)	/* ���݂̎��Ԃ��擾���� */
{
	NowTime = 0;
	ras_pal[0] = 0;
	ras_count = RASTER_MAX;
	Hsync_count = 0;
	Vsync_count = 0;
	
	return 0;
}

void interrupt Timer_D_Func(void)
{
	NowTime++;

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

SS GetNowTime(UI *time)	/* ���݂̎��Ԃ��擾���� */
{
	*time = NowTime;
	return 0;
}

SS SetNowTime(UI time)	/* ���݂̎��Ԃ�ݒ肷�� */
{
	NowTime = time;
	return 0;
}

/* ���荞�݊֐��͏C���q��interrupt�����邱�� */	/* ���ɂ����iKunihiko Ohnaka�j����̃A�h�o�C�X */
void interrupt Hsync_Func(void)
{
	Hsync_count++;
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

/* ���X�^�[���荞�݂̏������͕K�v�Œ������ */	/* EXCEED.����̃A�h�o�C�X */
/* �Q���C����΂����炢�����x�̌��E */	/* EXCEED.����̃A�h�o�C�X */
void interrupt Raster_Func(void)
{
	volatile US *BG0scroll_x  = (US *)0xEB0800;
	volatile US *BG0scroll_y  = (US *)0xEB0802;
	volatile US *BG1scroll_x  = (US *)0xEB0804;
	volatile US *BG1scroll_y  = (US *)0xEB0806;
	volatile US *CRTC_R09 = (US *)0xE80012u;	/* ���X�^�[���荞�݈ʒu */
	volatile US *CRTC_R12 = (US *)0xE80018u;	/* �X�N���[��0 X */
	volatile US *CRTC_R14 = (US *)0xE8001Cu;	/* �X�N���[��1 X */

	US nNum = ras_count;
	
	ras_count += RASTER_NEXT;		/* ���̃��X�^���荞�݈ʒu�̌v�Z */
	*CRTC_R09 = ras_count;			/* ���̃��X�^���荞�݈ʒu�̐ݒ� */

	*BG0scroll_x	= ras_val[nNum];	/* BG0��X���W�̐ݒ� */
	*BG0scroll_y	= ras_pal[nNum];	/* BG0��Y���W�̐ݒ� */
	*BG1scroll_x	= 256;				/* BG1��X���W�̐ݒ� */
	*BG1scroll_y	= ras_pal[1];		/* BG1��Y���W�̐ݒ� */
//	*CRTC_R12		= ras_val[nNum] + X_OFFSET;			/* GR��X���W�̐ݒ� */
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

SS SetRasterVal(void *pSrc, size_t n)
{
	SS ret = 0;
	if(memcpy(ras_val, pSrc, n) == NULL)
	{
		ret = -1;
	}
	return ret;
}

SS SetRasterPal(void *pSrc, size_t n)
{
	SS ret = 0;
	if(memcpy(ras_pal, pSrc, n) == NULL)
	{
		ret = -1;
	}
	return ret;
}

void interrupt Vsync_Func(void)
{

	//	VDISPST((void *)0, 0, 0);	/* stop */
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
	
	ras_count = ras_pal[0];
	CRTCRAS( Raster_Func, ras_count );	/* ���X�^�[���荞�� */
	Hsync_count = 0;
//	HSYNCST(Hsync_Func);				/* H-Sync���荞�� */
	Vsync_count++;
//	VDISPST(Vsync_Func, 0, 1);			/* V-Sync���荞�� �A�� */

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

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

SS vwait(SS count)				/* ��count�^60�b�҂�	*/
{
	volatile UC *mfp = (UC *)0xe88001;
	
	while(count--){
		while(!((*mfp) & 0b00010000));	/* �����\�����ԑ҂�	*/
		while((*mfp) & 0b00010000);	/* �����A�����ԑ҂�	*/
	}
}

#endif	/* MFP_H */
