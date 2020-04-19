#ifndef	MFP_H
#define	MFP_H

#include <string.h>
#include "inc/usr_style.h"

enum{
	DST_none=0,
	DST_1us,
	DST_2p5us,
	DST_4us,
	DST_12p5us,
	DST_16us,
	DST_25us,
	DST_50us,
};

extern UI Init_MFP(void);
extern void interrupt Timer_D_Func(void);
extern void interrupt Hsync_Func(void);
extern void interrupt Raster_Func(void);
extern void interrupt Vsync_Func(void);
extern SI vwait(SI);
extern SI GetNowTime(UI *);		/* ���݂̎��Ԃ��擾���� */
extern SI SetNowTime(UI);		/* ���݂̎��Ԃ�ݒ肷�� */

extern SI SetRasterVal(void *, size_t);	/* ���X�^�[��p�̃o�b�t�@�ɃR�s�[���� */
extern SI SetRasterPal(void *, size_t);	/* ���X�^�[��p(PAL)�̃o�b�t�@�ɃR�s�[���� */


#endif	/* MFP_H */
