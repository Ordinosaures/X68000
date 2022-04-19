#ifndef	CRTC_H
#define	CRTC_H

#include "inc/usr_define.h"

/* define��` */
#define	CRT_MAX		(3)
#define	GP_VDISP	(0x10)
#define	GP_H_SYNC	(0x80)
#define	GP_V_SYNC	(0x40)

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
extern volatile uint16_t	*CRTC_R06;
extern volatile uint16_t	*CRTC_R09;

extern	void CRTC_INIT(uint8_t);
extern	void CRTC_INIT_Manual(void);
extern	int16_t	GetCRT(ST_CRT *, int16_t);
extern	int16_t	SetCRT(ST_CRT, int16_t);
extern	int16_t	CRT_INIT(void);
extern	int16_t	Get_CRT_Contrast(int8_t *);
extern	int16_t	Set_CRT_Contrast(int8_t);
extern	int16_t	Get_CRT_Tmg(uint16_t *);
extern	int16_t	Set_CRT_Tmg(uint16_t);
extern int16_t wait_vdisp(int16_t);
extern int16_t wait_h_sync(void);
extern int16_t wait_v_sync(void);

#endif	/* CRTC_H */
