#ifndef	USR_DEFINE_H
#define	USR_DEFINE_H

/* �w�C�x���W�i�\�����W�j */
#define V_SYNC_MAX	(240)
#define WIDTH		(256)
#define HEIGHT		(256)
#define X_MIN_DRAW	(0)
#define X_MAX_DRAW	(511)
#define Y_MIN_DRAW	(0)
#define Y_MAX_DRAW	(511)
#define Y_HORIZON	(64)

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

/* �X�v���C�g�o�b�f�p�^�[���ő吔 */
#define	PCG_MAX	(256)

#endif	/* USR_DEFINE_H */
