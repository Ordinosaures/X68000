#ifndef	RASTER_H
#define	RASTER_H

#include "inc/usr_define.h"

/* �\���̒�` */

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
extern void Raster_Main(UC);
extern void Road_Pat(void);
extern SS GetRasterInfo(ST_RAS_INFO *);
extern SS SetRasterInfo(ST_RAS_INFO);
extern UL GetRoadDataAddr(void);

#endif	/* RASTER_H */
