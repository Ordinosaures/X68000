#ifndef	MyCar_C
#define	MyCar_C

#include "inc/usr_macro.h"
#include "MyCar.h"
#include "Input.h"

SS	GetMyCarInfo(ST_CARDATA *, SS);

SS	GetMyCarInfo(ST_CARDATA *stCarData, SS input)
{
	static UC bShiftPosFlag[2] = {FALSE};

	US	uTM[6] = { 0, 2857, 1950, 1444, 1096, 761 };/* �ϑ���  1:2.857 2:1.95 3:1.444 4:1.096 5:0.761 */
	
	/* �X�e�A�����O���� */
	if((input & KEY_RIGHT) != 0u)	stCarData->Steering += 1;	/* �E */
	if((input & KEY_LEFT) != 0u)	stCarData->Steering -= 1;	/* �� */
	stCarData->Steering = Mmax(Mmin(stCarData->Steering, 31), -31);
	
	/* �A�N�Z�� */
	if((input & KEY_A) != 0u){	/* A�{�^�� */
		/* ��]�� */
//			if(b8ms_time == TRUE)
		{
			stCarData->uEngineRPM += 125;
		}
	}
	else{
		/* ��]�� */
//			if(b8ms_time == TRUE)
		{
			stCarData->uEngineRPM -= 100;
		}
	}
	stCarData->uEngineRPM = Mmax(Mmin(9000, stCarData->uEngineRPM), 750);

	/* �V�t�g���� */
	if(ChatCancelSW((input & KEY_UPPER)!=0u, &bShiftPosFlag[0]) == TRUE)
	{
		if(stCarData->ubShiftPos != 5)	m_pcmplay(7,3,4);	/* SE:�V�t�g�A�b�v */
		stCarData->ubShiftPos = Mmin(stCarData->ubShiftPos+1, 5);	/* �� */
	}
	
	if(ChatCancelSW((input & KEY_LOWER)!=0u, &bShiftPosFlag[1]) == TRUE)
	{
		if(stCarData->ubShiftPos != 0)	m_pcmplay(7,3,4);	/* SE:�V�t�g�_�E�� */
		stCarData->ubShiftPos = Mmax(stCarData->ubShiftPos-1, 0);	/* �� */
	}
#if 0
	stCarData->ubShiftPos = 2;	/* �e�X�g�p�ϑ��Œ� */
#endif	
	stCarData->ubShiftPos = Mmax(Mmin(stCarData->ubShiftPos, 5), 0);
	
	/* �ԑ� */
	if(stCarData->ubShiftPos == 0u)		/* �j���[�g���� */
	{
		stCarData->VehicleSpeed -= 1;
	}
	else
	{
		/* �ϑ���  1:2.857 2:1.95 3:1.444 4:1.096 5:0.761 ������ 4.687 �^�C������2052.1mm */
		/* �^�C�������~�U�O�~��]���^�i�P�O�O�O�~�ϑ���~������j */
		stCarData->VehicleSpeed = (SS)(((UI)26 * stCarData->uEngineRPM) / uTM[stCarData->ubShiftPos]);	
	}
	
	/* �u���[�L */
	if( (input & KEY_B) != 0U )	/* B�{�^�� */
	{
		stCarData->VehicleSpeed -= 30;	/* �ϑ���ŕς����� */
		stCarData->ubBrakeLights = TRUE;	/* �u���[�L�����v ON */
		m_pcmplay(4,3,4);	/* �u���[�L�� */
	}
	else
	{
		stCarData->ubBrakeLights = FALSE;	/* �u���[�L�����v OFF */
	}
	stCarData->VehicleSpeed = Mmax(Mmin(310, stCarData->VehicleSpeed), 0);
}

#endif	/* MyCar_C */

