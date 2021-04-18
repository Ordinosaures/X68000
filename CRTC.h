#ifndef	CRTC_H
#define	CRTC_H

#include "inc/usr_define.h"

#define	CRT_MAX		(3)

/* �\���̒�` */

/* ��ʏ��̍\���� */
typedef struct
{
	int16_t	view_offset_x;		/* �\����ʂ�X���W�I�t�Z�b�g */
	int16_t	view_offset_y;		/* �\����ʂ�Y���W�I�t�Z�b�g */
	int16_t	hide_offset_x;		/* ��\����ʂ�X���W�I�t�Z�b�g */
	int16_t	hide_offset_y;		/* ��\����ʂ�Y���W�I�t�Z�b�g */
	int16_t	BG_offset_x;		/* BG��ʂ�X���W�I�t�Z�b�g */
	int16_t	BG_offset_y;		/* BG��ʂ�Y���W�I�t�Z�b�g */
}	ST_CRT;

/* extern�錾 */
extern	void CRTC_INIT(void);
extern	void CRTC_INIT_Manual(void);
extern	int16_t	GetCRT(ST_CRT *, int16_t);
extern	int16_t	SetCRT(ST_CRT, int16_t);
extern	int16_t	CRT_INIT(void);
extern	int16_t	Get_CRT_Contrast(int8_t *);
extern	int16_t	Set_CRT_Contrast(int8_t);

#endif	/* CRTC_H */
