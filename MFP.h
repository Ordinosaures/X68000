#ifndef	MFP_H
#define	MFP_H

#include <string.h>
#include "inc/usr_style.h"

extern UI Init_MFP(void);
extern void interrupt Timer_D_Func(void);
extern void interrupt Hsync_Func(void);
extern void interrupt Raster_Func(void);
extern void interrupt Vsync_Func(void);
extern SI vwait(SI);
extern SI GetNowTime(US *);	/* ���݂̎��Ԃ��擾���� */
extern SI SetNowTime(US);		/* ���݂̎��Ԃ�ݒ肷�� */
extern SI SetRasterVal(void *, size_t);	/* ���X�^�[��p�̃o�b�t�@�ɃR�s�[���� */
extern SI SetRasterPal(void *, size_t);	/* ���X�^�[��p(PAL)�̃o�b�t�@�ɃR�s�[���� */


#endif	/* MFP_H */
