#ifndef	MFP_C
#define	MFP_C

#include <iocslib.h>
#include <interrupt.h>

#include "inc/usr_define.h"
#include "inc/usr_macro.h"

#include "OverKata.h"
#include "MFP.h"

/* �ϐ� */
static volatile US NowTime;
static volatile US ras_count;
static volatile US Hsync_count;
static volatile US Vsync_count;
static US ras_val[1024];

/* �֐��̃v���g�^�C�v�錾 */
UI Init_MFP(void);
void interrupt Timer_D_Func(void);
SI GetNowTime(US *);	/* ���݂̎��Ԃ��擾���� */
SI SetNowTime(US);		/* ���݂̎��Ԃ�ݒ肷�� */
void interrupt Hsync_Func(void);
void interrupt Raster_Func(void);
SI SetRasterVal(void *, size_t);		/* ���X�^�[��p�̃o�b�t�@�ɃR�s�[���� */
void interrupt Vsync_Func(void);
SI vwait(SI);

UI Init_MFP(void)	/* ���݂̎��Ԃ��擾���� */
{
	NowTime = 0;
	ras_count = RASTER_ST;
	Hsync_count = 0;
	Vsync_count = 0;
	
	ras_val[1024];
	
	return 0;
}

void interrupt Timer_D_Func(void)
{
	NowTime++;

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

SI GetNowTime(US *time)	/* ���݂̎��Ԃ��擾���� */
{
	*time = NowTime;
	return 0;
}

SI SetNowTime(US time)	/* ���݂̎��Ԃ�ݒ肷�� */
{
	NowTime = time;
	return 0;
}


void interrupt Hsync_Func(void)
{
	Hsync_count++;
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

void interrupt Raster_Func(void)
{
	volatile US *scroll_x    = (US *)0xE80018;
	volatile US *BGscroll_x  = (US *)0xEB0800;
	volatile US *raster_addr = (US *)0xE80012;
	
	ras_count += RASTER_NEXT;
	if(ras_count >= RASTER_MAX)ras_count = RASTER_ST;
	*raster_addr = ras_count;
	*scroll_x	= ras_val[ras_count];
	*BGscroll_x	= ras_val[ras_count];

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

SI SetRasterVal(void *pSrc, size_t n)
{
	SI ret = 0;
	if(memcpy(ras_val, pSrc, n) == NULL)
	{
		ret = -1;
	}
	return ret;
}

void interrupt Vsync_Func(void)
{
//	VDISPST((void *)0, 0, 0);	/* stop */
	
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

	ras_count = RASTER_ST;
//	ras_count = RASTER_MIN;
	CRTCRAS(Raster_Func, RASTER_ST );	/* ���X�^�[���荞�� */
	Hsync_count = 0;
//	HSYNCST(Hsync_Func);				/* H-Sync���荞�� */
	Vsync_count++;
//	VDISPST(Vsync_Func, 0, 1);			/* V-Sync���荞�� �A�� */
	
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

SI vwait(SI count)				/* ��count�^60�b�҂�	*/
{
	volatile UC *mfp = (UC *)0xe88001;
	
	while(count--){
		while(!((*mfp) & 0b00010000));	/* �����\�����ԑ҂�	*/
		while((*mfp) & 0b00010000);	/* �����A�����ԑ҂�	*/
	}
}

#endif	/* MFP_H */
