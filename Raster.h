#ifndef	RASTER_H
#define	RASTER_H

#include "inc/usr_define.h"

#define	ROADDATA_MAX	(1024u)

/* �\���̒�` */
/* ���X�^�[���̍\���� */
typedef struct
{
	US	st;			/* �J�n�ʒu */
	US	mid;		/* ���Ԉʒu */
	US	ed;			/* �I���ʒu */
	US	size;		/* �J�n�ƏI���̍��� */
	US	horizon;	/* ������ */
}	ST_RAS_INFO;

/* ���X�^�[���荞�ݏ��̍\���� */
typedef struct
{
	US	x;
	US	y;
	US	pat;
}	ST_RASTER_INT;

/* ���[�h���̍\���� */
typedef struct
{
	SS	Horizon;		/* �����ʒu */
	SS	Horizon_tmp;	/* �������ʒu */
	SS	Horizon_offset;	/* �����ʒu�̃I�t�Z�b�g */
	
	SS	offset_x;		/*  */
	SS	offset_y;		/*  */
	SS	offset_val;		/*  */
	SS	height;			/*  */
	SS	slope;			/*  */
	SS	angle;			/*  */
	SS	distance;		/*  */
	SS	object;			/*  */
	
}	ST_ROAD_INFO;

/* extern�錾 */
extern void	Raster_Init(void);
extern void Raster_Main(UC);
extern SS GetRasterInfo(ST_RAS_INFO *);
extern SS SetRasterInfo(ST_RAS_INFO);
extern SS GetRasterPos(US *, US *, US);

extern void Road_Init(US);
extern UL GetRoadDataAddr(void);
extern SS GetRoadCycleCount(US *);

#endif	/* RASTER_H */
