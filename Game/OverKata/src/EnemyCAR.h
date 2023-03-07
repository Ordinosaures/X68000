#ifndef	ENEMYCAR_H
#define	ENEMYCAR_H

#include "inc/usr_define.h"

#define	ENEMYCAR_MAX	(2)
#define ENEMY_CAR_0_W	(67)
#define ENEMY_CAR_0_H	(45)
#define ENEMY_CAR_1_W	(140)
#define ENEMY_CAR_1_H	(105)
#define	ENEMYCAR_PAT_MAX	(11)
#define	ENEMYCAR_TYP_MAX	(8)

/* �\���̒�` */

/* ���C�o���Ԃ̍\���� */
typedef struct
{
	uint8_t	ubCarType;			/* �Ԃ̎�� */
	int16_t	VehicleSpeed;		/* �ԑ� */
	int16_t	x;					/* X���W */
	int16_t	y;					/* Y���W */
	int16_t	z;					/* Z���W(�{��) */
	int16_t	sx;					/* X���W(�����蔻��J�n���W) */
	int16_t	ex;					/* X���W(�����蔻��I�����W) */
	int16_t	sy;					/* Y���W(�����蔻��J�n���W) */
	int16_t	ey;					/* Y���W(�����蔻��I�����W) */
	uint8_t	ubBrakeLights;		/* �u���[�L���C�g */
	uint8_t	ubOBD;				/* �ԗ��̏�� */
	uint8_t	ubAlive;			/* �\���̏�� */
}	ST_ENEMYCARDATA;

/* extern�錾 */
extern	int16_t	InitEnemyCAR(void);
extern	int16_t	GetEnemyCAR(ST_ENEMYCARDATA *, int16_t);
extern	int16_t	SetEnemyCAR(ST_ENEMYCARDATA, int16_t);
extern	int16_t	EnemyCAR_main(uint8_t, uint8_t, uint8_t);
extern	int16_t	SetAlive_EnemyCAR(void);
extern	int16_t	Put_EnemyCAR(uint16_t, uint16_t, uint16_t, uint8_t, uint8_t);
extern	int16_t	Sort_EnemyCAR(void);

#endif	/* ENEMYCAR_H */
