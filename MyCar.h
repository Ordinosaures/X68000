#ifndef	MyCar_H
#define	MyCar_H

#include "inc/usr_define.h"

/* �\���̒�` */

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

/* extern�錾 */

extern	SS	GetMyCar(ST_CARDATA *stDat);
extern	SS	SetMyCar(ST_CARDATA stDat);
extern	SS	UpdateMyCarInfo(SS);
extern	SS	MyCar_Interior(SS, SS);
extern	SS	MyCar_CourseOut(SS);

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

�i�P�j���ʂ���1530kg�̕��̂ɍ�p����d��W��
		W�i�m�j�����ʂ��i�����j�~�d�͉����x9.81�im�^sec2�j
		 	��	1530�ikg�j�~ 9.81�im�^sec2�j
			 �� 15162.3�m
�i�Q�j���C�͂� ���ʂm�ł��邩��A
		���i�m�j��1.0�~15162.3�i�m�j��15162.3�m
		�A�X�t�@���g�F0.5�`1.0�A�����F0.4�`0.5�A��F0.2�`0.5
�i�R�j���̂𓮂����ɂ́A���C�͂� �ɓ������͂��K�v�ł��邩��
		�e�i�m�j����	��15162.3�m
*/

#endif	/* MyCar_H */
