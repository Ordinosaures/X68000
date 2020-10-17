#ifndef	ENEMYCAR_H
#define	ENEMYCAR_H

#include "inc/usr_define.h"

#define	ENEMYCAR_MAX	(4)
#define ENEMY_CAR_0_W	(67)
#define ENEMY_CAR_0_H	(45)
#define ENEMY_CAR_1_W	(140)
#define ENEMY_CAR_1_H	(105)

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
extern	SS	Sort_EnemyCAR(void);

#endif	/* ENEMYCAR_H */
