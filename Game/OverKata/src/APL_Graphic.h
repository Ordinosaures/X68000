#ifndef	APL_GRAPHIC_H
#define	APL_GRAPHIC_H

#include <usr_define.h>

/* ��ʓ��o�ꐔ */
enum
{
	Clear_G=0,
	BackGround_G,
	Object0_G,
	Object1_G,
	Object2_G,
	Object3_G,
	Object4_G,
	Object5_G,
	Object6_G,
	Object7_G,
	Enemy1_G,
	Enemy2_G,
	StartPoint_G,
	GoalPoint_G,
	MyCar_G,
	Debug_View_G,
	Flip_G,
	MAX_G
};

/* �O���t�B�b�N�p�^�[����`�� 0-15 max16 */
#define DUMMY_CG		(0)		/* �_�~�[		(1) */
#define BG_CG			(1)		/* �w�i			(1) */
#define COURSE_OBJ_CG	(2)		/* ��Q��		(kind of 4) */
#define ENEMYCAR_CG		(6)		/* ���C�o����	(kind of 8) */
#define START_PT_CG		(14)	/* �X�^�[�g�ʒu	(1) */
#define GOAL_PT_CG		(15)	/* �S�[���ʒu	(1) */
#define PAL_MAX_CG		(16)	/* �ő�16�p�^�[���o�^ */
/* �O���t�B�b�N�e�L�X�g�p�^�[����`�� max 3 */
#define MYCAR_CG		(16)	/* ����			(kind of 3) */
/* �t���O���t�B�b�N */
#define TITLE_CG		(19)	/* �^�C�g�� */
#define DEMO_CG			(20)	/* �f�� */
	/* �c */

/* �\���̒�` */

/* extern�錾 */
extern	int16_t APL_G_Main(void);

#endif	/* APL_GRAPHIC_H */
