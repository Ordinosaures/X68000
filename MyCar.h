#ifndef	MyCar_H
#define	MyCar_H

#include "inc/usr_define.h"

#define MY_CAR_0_W	(65)
#define MY_CAR_0_H	(52)
#define MY_CAR_1_W	(256)
#define MY_CAR_1_H	(224)

enum{
	OBD_NORMAL,
	OBD_DAMAGE,
	OBD_SPIN_L,
	OBD_SPIN_R,
	OBD_COURSEOUT,
	OBD_MAX
};

/* �\���̒�` */

/* �Ԃ̍\���� */
typedef struct
{
	uint8_t	ubCarType;			/* �Ԃ̎�� */
	uint16_t	uEngineRPM;			/* �G���W����]�� */
	int16_t	VehicleSpeed;		/* �ԑ� */
	int16_t	Steering;			/* �X�e�A */
	uint8_t	ubShiftPos;			/* �M�A�i */
	uint8_t	ubThrottle;			/* �X���b�g���J�x */
	uint8_t	ubBrakeLights;		/* �u���[�L���C�g */
	uint8_t	ubHeadLights;		/* �w�b�h���C�g */
	uint8_t	ubWiper;			/* ���C�p�[ */
	int8_t	bTire;				/* �^�C���̏�� */
	uint8_t	ubOBD;				/* �ԗ��̏�� */
}	ST_CARDATA;

/* extern�錾 */

extern	int16_t	MyCar_G_Load(void);
extern	int16_t	GetMyCar(ST_CARDATA *stDat);
extern	int16_t	SetMyCar(ST_CARDATA stDat);
extern	int16_t	MyCarInfo_Init(void);
extern	int16_t	MyCarInfo_Update(int16_t);
extern	int16_t	MyCar_Interior(uint8_t);
extern	int16_t	MyCar_CourseOut(void);
extern	int16_t	GetMyCarSpeed(int16_t *);

/* ���� */

/*
�ԑ̐��@	��4495�~��1810�~��1480mm
�������@	��2030�~��1470�~��1190mm
�������֋�	2650mm
			�O1545mm/��1545mm
�ŏ����a	5.9m
�Œፂ		140mm
�^�C��		�O�ցF245/40R18
			��ցF245/40R18
			�^�C�����a 65.3cm
			�^�C���̗L�����a(0.3265m)
�u���[�L	�O�F�x���`���[�e�b�h�f�B�X�N
			��F�x���`���[�e�b�h�f�B�X�N
�ԗ��d��	1530kg

�G���W������
�����@�^��	4B11
�C���z��	����4�C��
�r�C��		1998cc
���k��		9.0
�z�C����	�^�[�{
�ō��o��	313PS[230kW]/6500rpm
�ő�g���N	43.7kgm[429Nm]/3500rpm

�ϑ���		1��:2.857
			2��:1.95
		 	3��:1.444
		 	4��:1.096
		 	5��:0.761
������			4.687

�i�P�j���ʂ���1530kg�̕��̂ɍ�p����d��W��
		W�i�m�j�����ʂ��i�����j�~�d�͉����x9.81�im�^sec2�j
		 	��	1530�ikg�j�~ 9.81�im�^sec2�j
			 �� 15162.3�m
�i�Q�j���C�͂� ���ʂm�ł��邩��A
		���i�m�j��1.0�~15162.3�i�m�j��15162.3�m
		�A�X�t�@���g�F0.5�`1.0�A�����F0.4�`0.5�A��F0.2�`0.5
�i�R�j���̂𓮂����ɂ́A���C�͂� �ɓ������͂��K�v�ł��邩��
		�e�i�m�j����	��15162.3�m

�i�S�j�쓮�́��g���N�~�ŏI�M����i���ϑ���~������j���^�C�����a

�i�T�j�����x���i�g���N�|���C��R�|��C��R�j���ԗ��d��

�i�U�j��C��R �� 1/2 x ��C���x �~ �O�ʓ��e�ʐ� �~ ���x�̂Q�� �~ CD�l�i��C��R�W���j
*/

#endif	/* MyCar_H */
