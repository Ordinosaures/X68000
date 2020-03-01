#ifndef	MFP_C
#define	MFP_C

#include <iocslib.h>
#include <interrupt.h>
#include "inc/usr_style.h"
#include "inc/usr_define.h"
#include "inc/usr_macro.h"

#include "OverKata.h"
#include "MFP.h"

/* �ϐ� */
static volatile unsigned short NowTime;
static volatile unsigned short ras_count;
static volatile unsigned short Hsync_count;

/* �֐��̃v���g�^�C�v�錾 */
void interrupt Timer_D_Func(void);
void interrupt Hsync_Func(void);
void interrupt Raster_Func(void);
void interrupt Vsync_Func(void);
int vwait(int);
int GetNowTime(unsigned short *);	/* ���݂̎��Ԃ��擾���� */
int SetNowTime(unsigned short);		/* ���݂̎��Ԃ�ݒ肷�� */

void interrupt Timer_D_Func(void)
{
	NowTime++;

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

void interrupt Hsync_Func(void)
{
	HSYNCST((void *)0);		/* stop */
	HSYNCST(Hsync_Func);	/* H-Sync���荞�� */

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

void interrupt Raster_Func(void)
{
	static volatile unsigned short ras_cal = 0;
	static volatile unsigned short ras_val = 0;
	volatile unsigned short *scroll_x = (unsigned short *)0xE80018;
	volatile unsigned short *raster_add = (unsigned short *)0xE80012;
	
//	CRTCRAS((void *)0, 0);	/* stop */
	
	ras_count += RASTER_NEXT;
	ras_val = X_MIN;
	
	/* ���X�^�̈� */
	if((ras_count >= RASTER_ST) && (ras_count <= (RASTER_ST + 20)))
	{
		ras_val += speed;
	}
	else if(ras_count < RASTER_MAX)
	{
		ras_val += speed * ( RASTER_SIZE  / (float)(Mmax((ras_count - RASTER_ST), 1)) );
	}
	else
	{
		ras_count = RASTER_ST;
	}

	*(scroll_x) = ras_val;

	moni = ras_count;
	
//	CRTCRAS(Raster_Func, ras_count);	/* ���X�^�[���荞�� */
	*raster_add = ras_count;
	
	if(Hsync_count != 0U)
	{
		ras_cal++;
	}
	else{
		ras_cal = 0;
	}
	
	if(ras_cal > 32)
	{
		ras_cal = 0;
	}
	moni_MAX = ras_cal;
	
	Hsync_count = 1;

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

void interrupt Vsync_Func(void)
{
	static volatile unsigned short PalAnime = 0;

	VDISPST((void *)0, 0, 0);	/* stop */
	
	if((PalAnime % 10) < 5)
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
	PalAnime++;

	//moni_MAX = Mmax(moni_MAX, moni);

	Hsync_count = 0;
//	HSYNCST(Hsync_Func);				/* H-Sync���荞�� */
	VDISPST(Vsync_Func, 0, 1);

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

int vwait(int count)				/* ��count�^60�b�҂�	*/
{
	volatile char *mfp = (char *)0xe88001;
	
	while(count--){
		while(!((*mfp) & 0b00010000));	/* �����\�����ԑ҂�	*/
		while((*mfp) & 0b00010000);	/* �����A�����ԑ҂�	*/
	}
}

int GetNowTime(unsigned short *time)	/* ���݂̎��Ԃ��擾���� */
{
	*time = NowTime;
	return 0;
}

int SetNowTime(unsigned short time)	/* ���݂̎��Ԃ�ݒ肷�� */
{
	NowTime = time;
	return 0;
}

#endif	/* MFP_H */
