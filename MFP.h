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

extern SS MFP_INIT(void);
extern SS MFP_EXIT(void);
extern SS MFP_RESET(void);
extern SS TimerD_INIT(void);
extern SS TimerD_EXIT(void);
extern UC GetNowTime(UI *);		/* ���݂̎��Ԃ��擾���� */
extern UC SetNowTime(UI);		/* ���݂̎��Ԃ�ݒ肷�� */
extern UC GetStartTime(UI *);	/* �J�n�̎��Ԃ��擾���� */
extern UC SetStartTime(UI);		/* �J�n�̎��Ԃ�ݒ肷�� */
extern SS SetRasterIntData(void *, size_t);
extern SS GetRasterIntPos(US *, US *, US *, US);
extern SS vwait(SS);


#endif	/* MFP_H */
