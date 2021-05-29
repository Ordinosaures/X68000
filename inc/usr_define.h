#ifndef	USR_DEFINE_H
#define	USR_DEFINE_H

#include "usr_style.h"

#define	DEBUG	/* �f�o�b�O�L����` */

#define FALSE	(0)
#define TRUE	(!FALSE)

#define Bit_0		(0x1)
#define Bit_1		(0x2)
#define Bit_2		(0x4)
#define Bit_3		(0x8)
#define Bit_4		(0x10)
#define Bit_5		(0x20)
#define Bit_6		(0x40)
#define Bit_7		(0x80)
#define Bit_8		(0x100)
#define Bit_9		(0x200)
#define Bit_10		(0x400)
#define Bit_11		(0x800)
#define Bit_12		(0x1000)
#define Bit_13		(0x2000)
#define Bit_14		(0x4000)
#define Bit_15		(0x8000)

/* �w�C�x���W�i�\�����W�j */
#define V_SYNC_MAX		(240)
#define WIDTH			(256)
#define HEIGHT			(256)
#define X_MIN_DRAW		(0)
#define X_MAX_DRAW		(512)
#define X_MAX_DRAW_OF	(32)
#define Y_MIN_DRAW		(0)
#define Y_MAX_DRAW		(512)
#define Y_HORIZON_0		(128)
#define Y_HORIZON_1		(64)
#define Y_MAX_WINDOW	(160)	/* Y_HORIZON_1+ROAD_SIZE */

/* �w�C�x���W�i���z���W�j */
#define X_OFFSET	(224)
#define X_MIN		X_OFFSET
#define X_MAX		(WIDTH+X_OFFSET)
#define X_MAX_H		((WIDTH>>1)+X_OFFSET)

#define Y_OFFSET	(256)
#define Y_MIN		Y_OFFSET
#define Y_MAX		(HEIGHT+Y_OFFSET)
#define Y_MAX_H		((HEIGHT>>1)+Y_OFFSET)

/* ���X�^���(256*256 31kHz�̏ꍇ ���C����2�{�v�Z) */
#define RASTER_NEXT	(2)
#define RASTER_MIN	(16)		//�\���J�n�ʒu(40,16)	/* ���X�^�J�n�ʒu�́ACRTC��R6�Ɠ��� */
#define RASTER_MAX	(256)		//�\���I���ʒu(552,256)

#define ROAD_ST_POINT	(128)	/* BG�̓��̊J�n�ʒu */
#define ROAD_ED_POINT	(224)	/* BG�̓��̏I���ʒu */
#define ROAD_CT_POINT	(124)	/* BG�̓��̒��S�ʒu */
#define ROAD_SIZE		(ROAD_ED_POINT-ROAD_ST_POINT)	/* BG�̓��̃T�C�Y(96) */
#define ROAD_0_MIN		(RASTER_MIN)	/* BG�̓��̍ŏ�(mode0)���� */
#define ROAD_0_MAX		(V_SYNC_MAX)	/* BG�̓��̍ő�(mode0)���� */
#define ROAD_1_MIN		(RASTER_MIN)	/* BG�̓��̍ŏ�(mode1) */
#define ROAD_1_MAX		(Y_MAX_WINDOW)	/* BG�̓��̍ő�(mode1) */

/* �X�v���C�g�o�b�f�p�^�[���ő吔 */
#define	PCG_MAX		(256)
#define	BG_WIDTH	(8)
#define	BG_HEIGHT	(8)
#define BG_0_W		(280)
#define BG_0_H		(63)
#define BG_1_W		(280)
#define BG_1_H		(63)
#define SP_X_OFFSET	(16)
#define SP_Y_OFFSET	(16)

/* �O���t�B�b�N�p�^�[�� 0-7 max8 */
#define DUMMY_CG		(0)	/* �_�~�[(0) */
#define BG_CG			(1)	/* �w�i(1,2,3) */
#define COURSE_OBJ_CG	(4)	/* ��Q��(4,5,6,7) */
#define ENEMYCAR_CG		(8)	/* ���C�o����(8,9,10,11,12,13,14,15) */
/* �O���t�B�b�N�e�L�X�g max 1 */
#define MYCAR_CG		(16)	/* ����(t) */
/* �t���O���t�B�b�N */
#define TITLE_CG		(17)	/* �^�C�g�� */

/* �\���� */

/* �f�[�^�e�[�u�� */

extern void (*usr_abort)(void);	/* ���[�U�̃A�{�[�g�����֐� */

#endif	/* USR_DEFINE_H */
