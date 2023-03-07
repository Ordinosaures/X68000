#ifndef	RASTER_H
#define	RASTER_H

#include <usr_define.h>

#define	ROADDATA_MAX	(2048u)

/* �\���̒�` */
/* ���X�^�[���̍\���� */
typedef struct
{
	uint16_t	st;			/* �J�n�ʒu */
	uint16_t	mid_H;		/* ���Ԉʒu(H) */
	uint16_t	mid;		/* ���Ԉʒu(L) */
	uint16_t	ed;			/* �I���ʒu */
	uint16_t	size;		/* �J�n�ƏI���̍��� */
}	ST_RAS_INFO;

/* ���X�^�[���荞�ݏ��̍\���� */
typedef struct
{
	uint16_t	x;
	uint16_t	y;
	uint16_t	pat;
}	ST_RASTER_INT;

/* ���[�h���̍\���� */
typedef struct
{
	int16_t	Horizon;		/* �����ʒu */
	int16_t	Horizon_Base;	/* ������ʒu */
	int16_t	offset_x;		/*  */
	int16_t	offset_y;		/*  */
	int16_t	offset_val;		/*  */
	int16_t	height;			/*  */
	int16_t	slope;			/*  */
	int16_t	angle;			/*  */
	int16_t	distance;		/*  */
	int16_t	object;			/*  */
	int16_t	Courselength;	/* �R�[�X�̑S�� */
}	ST_ROAD_INFO;

/* �R�[�X�̍\���� */
typedef struct
{
	uint8_t	bHeight;	/* ���̕W��	(0x80�Z���^�[) */
	uint8_t	bWidth;		/* ���̕�	(0x80�Z���^�[) */
	uint8_t	bAngle;		/* ���̊p�x	(0x80�Z���^�[) */
	uint8_t	bfriction;	/* ���̖��C	(0x80�Z���^�[) */
	uint8_t	bPat;		/* ���̎��	 */
	uint8_t	bObject;	/* �o���|�C���g�̃I�u�W�F�N�g�̎�� */
	uint8_t	bRepeatCount;	/* �J��Ԃ��� */
}	ST_ROADDATA;


/* extern�錾 */
extern	ST_RASTER_INT	g_stRasterInt[RASTER_H_MAX];
extern	ST_ROAD_INFO	g_stRoadInfo;
extern	ST_ROADDATA		g_stRoadData[ROADDATA_MAX];

extern void	Raster_Init(void);
extern int16_t Raster_Main(void);
extern int16_t GetRasterInfo(ST_RAS_INFO *);
extern int16_t SetRasterInfo(ST_RAS_INFO);
extern int16_t GetRasterPos(uint16_t *, uint16_t *, uint16_t);

extern int16_t	GetRoadInfo(ST_ROAD_INFO *);
extern int16_t	SetRoadInfo(ST_ROAD_INFO);
extern void Road_Init(uint16_t);
extern void Road_BG_Init(uint16_t);
extern uint64_t GetRoadDataAddr(uint16_t);
extern int16_t	Road_Pat_Main(uint16_t *);
extern int16_t GetRoadCycleCount(uint16_t *);
extern int16_t Road_Map_Draw(uint8_t);

#endif	/* RASTER_H */
