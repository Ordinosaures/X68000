#ifndef	OBJECT_H
#define	OBJECT_H

#include "inc/usr_define.h"

#define	COURSE_OBJ_MAX	(6)

/* ���C�o���Ԃ̍\���� */
typedef struct
{
	UC	ubType;			/* ��� */
	SS	x;				/* X���W */
	SS	y;				/* Y���W */
	SS	z;				/* Z���W(�{��) */
	UC	ubAlive;		/* �\���̏�� */
}	ST_COURSE_OBJ;

extern	SS	InitCourseObj(void);
extern	SS	Course_Obj_main(UC, UC, UC);
extern	SS	Put_CouseObject(SS, SS, US, UC, UC);
extern	SS	GetRoadCycleCount(US *);
extern	SS	SetRoadCycleCount(US);
extern	SS	Sort_Course_Obj(void);
extern	SS	Load_Course_Data(UC);

#endif	/* OBJECT_H */
