#ifndef	MyCar_C
#define	MyCar_C

#include <iocslib.h>

#include "inc/usr_macro.h"
#include "inc/ZMUSIC.H"

#include "MyCar.h"
#include "Input.h"

/* �\���̒�` */

ST_CARDATA	stMyCar = {0};

/* �֐��̃v���g�^�C�v�錾 */
SS	GetMyCar(ST_CARDATA *stDat);
SS	SetMyCar(ST_CARDATA stDat);
SS	UpdateMyCarInfo(SS);
SS	MyCar_Interior(SS, SS);
SS	MyCar_CourseOut(SS);

/* �֐� */

SS	GetMyCar(ST_CARDATA *stDat)
{
	SS	ret = 0;
	*stDat = stMyCar;
	return ret;
}

SS	SetMyCar(ST_CARDATA stDat)
{
	SS	ret = 0;
	stMyCar = stDat;
	return ret;
}

SS	UpdateMyCarInfo(SS input)
{
	static UC bShiftPosFlag[2] = {FALSE};

	US	uTM[6] = { 0, 2857, 1950, 1444, 1096, 761 };/* �ϑ���  1:2.857 2:1.95 3:1.444 4:1.096 5:0.761 */
	US	uRPM[11] = { 500, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000 };	/* Eng��]�� */
	US	uTRQ[11] = { 120,  130,  150,  170,  210,  200,  180,  170,  140,  120,  100 };	/* �g���N */
	
	/* �X�e�A�����O���� */
	if((input & KEY_RIGHT) != 0u)	stMyCar.Steering += 1;	/* �E */
	if((input & KEY_LEFT) != 0u)	stMyCar.Steering -= 1;	/* �� */
	stMyCar.Steering = Mmax(Mmin(stMyCar.Steering, 31), -31);
	
	/* �A�N�Z�� */
	if((input & KEY_A) != 0u){	/* A�{�^�� */
		/* ��]�� */
//			if(b8ms_time == TRUE)
		{
			stMyCar.uEngineRPM += 125;
		}
	}
	else{
		/* ��]�� */
//			if(b8ms_time == TRUE)
		{
			stMyCar.uEngineRPM -= 100;
		}
	}
	stMyCar.uEngineRPM = Mmax(Mmin(9000, stMyCar.uEngineRPM), 750);

	/* �V�t�g���� */
	if(ChatCancelSW((input & KEY_UPPER)!=0u, &bShiftPosFlag[0]) == TRUE)
	{
		if(stMyCar.ubShiftPos != 5)	m_pcmplay(7,3,4);	/* SE:�V�t�g�A�b�v */
		stMyCar.ubShiftPos = Mmin(stMyCar.ubShiftPos+1, 5);	/* �� */
	}
	
	if(ChatCancelSW((input & KEY_LOWER)!=0u, &bShiftPosFlag[1]) == TRUE)
	{
		if(stMyCar.ubShiftPos != 0)	m_pcmplay(7,3,4);	/* SE:�V�t�g�_�E�� */
		stMyCar.ubShiftPos = Mmax(stMyCar.ubShiftPos-1, 0);	/* �� */
	}
#if 0
	stMyCar.ubShiftPos = 2;	/* �e�X�g�p�ϑ��Œ� */
#endif	
	stMyCar.ubShiftPos = Mmax(Mmin(stMyCar.ubShiftPos, 5), 0);
	
	/* �ԑ� */
	if(stMyCar.ubShiftPos == 0u)		/* �j���[�g���� */
	{
		stMyCar.VehicleSpeed -= 1;
	}
	else
	{
		/* �ϑ���  1:2.857 2:1.95 3:1.444 4:1.096 5:0.761 ������ 4.687 �^�C������2052.1mm */
		/* �^�C�������~�U�O�~��]���^�i�P�O�O�O�~�ϑ���~������j */
		stMyCar.VehicleSpeed = (SS)(((UI)26 * stMyCar.uEngineRPM) / uTM[stMyCar.ubShiftPos]);	
	}
	
	/* �u���[�L */
	if( (input & KEY_B) != 0U )	/* B�{�^�� */
	{
		stMyCar.VehicleSpeed -= 30;	/* �ϑ���ŕς����� */
		stMyCar.ubBrakeLights = TRUE;	/* �u���[�L�����v ON */
		m_pcmplay(4,3,4);	/* �u���[�L�� */
	}
	else
	{
		stMyCar.ubBrakeLights = FALSE;	/* �u���[�L�����v OFF */
	}
	stMyCar.VehicleSpeed = Mmax(Mmin(310, stMyCar.VehicleSpeed), 0);
}

SS	MyCar_Interior(SS x, SS y)
{
	SS	ret = 0;
	static	SS	Vibration = 0;
	static	SS	VibrationCT = 0;
	
	/* ��ʂ�h�炷 */
	VibrationCT++;
	if(VibrationCT > 5)
	{
		VibrationCT = 0;
	}
	Vibration = (VibrationCT == 0)?1:0;	/* ��ʂ̐U�� */

	/* ��ʂ̈ʒu */
	HOME(0b01, x, y + Vibration );	/* Screen 0(TPS/FPS) */
	HOME(0b10, x, y );				/* Screen 1 */

	return ret;
}


SS	MyCar_CourseOut(SS	rpm)
{
	SS	ret = 0;
#if	1
	SI	adpcm_sns;
	UI	time;
	static	UI	unExplosion_time = FALSE;
	UC	bExplosion;

	GetStartTime(&time);	/* �J�n�������擾 */
	
	adpcm_sns = m_stat(9/*Mmin(Mmax(25, vx), 31)*/);	/* ADPCM ch1(9) ch2-8(25-31) */
//			Message_Num(&adpcm_sns,	 0, 13, 2, MONI_Type_SI, "%d");
	
	if( (time - unExplosion_time) >  (60000 / rpm) )	/* ��]���̃G���W���� */
	{
		unExplosion_time = time;
		bExplosion = TRUE;
	}
	else
	{
		bExplosion = FALSE;
	}

	/* ��]���̃G���W���� */
	if( bExplosion == TRUE )
	{
		m_pcmplay(11,3,4);
	}
#endif
	return ret;
}

#endif	/* MyCar_C */

