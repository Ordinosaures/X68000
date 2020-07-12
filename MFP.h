#ifndef	MFP_H
#define	MFP_H

#include <string.h>
#include "inc/usr_define.h"

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

extern SS Init_MFP(void);
extern void interrupt Timer_D_Func(void);
extern void interrupt Hsync_Func(void);
extern void interrupt Raster_Func(void);
extern void interrupt Vsync_Func(void);
extern SS vwait(SS);
extern SS GetNowTime(UI *);		/* ���݂̎��Ԃ��擾���� */
extern SS SetNowTime(UI);		/* ���݂̎��Ԃ�ݒ肷�� */
extern SS GetStartTime(UI *);	/* �J�n�̎��Ԃ��擾���� */
extern SS SetStartTime(UI);		/* �J�n�̎��Ԃ�ݒ肷�� */
extern SS GetRasterPos(US *, US *, US);
extern SS GetRasterInfo(ST_RAS_INFO *);
extern SS SetRasterInfo(ST_RAS_INFO);

extern SS SetRasterVal(void *, size_t);	/* ���X�^�[��p�̃o�b�t�@�ɃR�s�[���� */
extern SS SetRasterPal(void *, size_t);	/* ���X�^�[��p(PAL)�̃o�b�t�@�ɃR�s�[���� */


#endif	/* MFP_H */
