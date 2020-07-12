#ifndef	ENEMYCAR_H
#define	ENEMYCAR_H

#include "inc/usr_define.h"

#define	ENEMYCAR_MAX	(4)

/* �\���̒�` */

/* ���C�o���Ԃ̍\���� */
typedef struct
{
	UC	ubCarType;			/* �Ԃ̎�� */
	SS	VehicleSpeed;		/* �ԑ� */
	SS	x;					/* X���W */
	SS	y;					/* Y���W */
	SS	z;					/* Z���W(�{��) */
	UC	ubBrakeLights;		/* �u���[�L���C�g */
	UC	ubOBD;				/* �̏�̏�� */
	UC	ubAlive;			/* �\���̏�� */
}	ST_ENEMYCARDATA;

/* extern�錾 */
extern	SS	InitEnemyCAR(void);
extern	SS	GetEnemyCAR(ST_ENEMYCARDATA *, SS);
extern	SS	SetEnemyCAR(ST_ENEMYCARDATA, SS);
extern	SS	EnemyCAR_main(UC, UC, UC);
extern	SS	SetAlive_EnemyCAR(void);
extern	SS	Put_EnemyCAR(US, US, US, UC);

#endif	/* ENEMYCAR_H */
