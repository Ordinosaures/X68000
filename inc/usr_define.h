#ifndef	USR_DEFINE_H
#define	USR_DEFINE_H

#include "usr_style.h"

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
#define V_SYNC_MAX	(240)
#define WIDTH		(256)
#define HEIGHT		(256)
#define X_MIN_DRAW	(0)
#define X_MAX_DRAW	(511)
#define Y_MIN_DRAW	(0)
#define Y_MAX_DRAW	(511)
#define Y_HORIZON_0	(128)
#define Y_HORIZON_1	(64)

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
#define RASTER_MIN	(16)//�\���J�n�ʒu(40,16)
#define RASTER_MAX	(256)//�\���I���ʒu(552,256)
#define RASTER_SIZE	(RASTER_MAX-RASTER_MIN)
#define ROAD_SIZE	(96)
#define ROAD_POINT	(128)

/* �X�v���C�g�o�b�f�p�^�[���ő吔 */
#define	PCG_MAX	(256)

#define	BG_WIDTH	(8)
#define	BG_HEIGHT	(8)

/* �摜�T�C�Y */
#define MY_CAR_0_W	(65)
#define MY_CAR_0_H	(52)
#define MY_CAR_1_W	(256)
#define MY_CAR_1_H	(256)
#define ENEMY_CAR_0_W	(67)
#define ENEMY_CAR_0_H	(45)
#define ENEMY_CAR_1_W	(140)
#define ENEMY_CAR_1_H	(105)
#define BG_0_W	(280)
#define BG_0_H	(63)
#define BG_1_W	(280)
#define BG_1_H	(63)
#define BG_0_UNDER	(280)	/* ���� */
#define BG_1_UNDER	(64)

/* �\���� */

/* �R�[�X�̍\���� */
typedef struct
{
	UC	bHeight;	/* ���̕W��	(0x80�Z���^�[) */
	UC	bWidth;		/* ���̕�	(0x80�Z���^�[) */
	UC	bAngle;		/* ���̊p�x	(0x80�Z���^�[) */
	UC	bfriction;	/* ���̖��C	(0x80�Z���^�[) */
	UC	bPat;		/* ���̎��	 */
	UC	bObject;	/* �o���|�C���g�̃I�u�W�F�N�g�̎�� */
	UC	bRepeatCount;	/* �J��Ԃ��� */
}	ST_ROADDATA;

/* �Ԃ̍\���� */
typedef struct
{
	UC	ubCarType;			/* �Ԃ̎�� */
	US	uEngineRPM;			/* �G���W����]�� */
	SS	VehicleSpeed;		/* �ԑ� */
	SS	Steering;			/* �X�e�A */
	UC	ubShiftPos;			/* �M�A�i */
	SC	bThrottle;			/* �X���b�g���J�x */
	UC	ubBrakeLights;		/* �u���[�L���C�g */
	UC	ubHeadLights;		/* �w�b�h���C�g */
	UC	ubWiper;			/* ���C�p�[ */
	SC	bTire;				/* �^�C���̏�� */
	UC	ubOBD;				/* �̏�̏�� */
}	ST_CARDATA;



/* �f�[�^�e�[�u�� */


#endif	/* USR_DEFINE_H */
