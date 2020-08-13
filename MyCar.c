#ifndef	MyCar_C
#define	MyCar_C

#include <iocslib.h>
#include <sys/iocs.h>
#include <interrupt.h>

#include "inc/usr_macro.h"
#include "inc/ZMUSIC.H"

#include "MyCar.h"
#include "OverKata.h"
#include "APL_Math.h"
#include "Input.h"
#include "EnemyCAR.h"
#include "MFP.h"
#include "Output_Text.h"
#include "PCG.h"

/* �\���̒�` */

ST_CARDATA	stMyCar = {0};

/* �֐��̃v���g�^�C�v�錾 */
SS	GetMyCar(ST_CARDATA *stDat);
SS	SetMyCar(ST_CARDATA stDat);
SS	UpdateMyCarInfo(SS);
SS	MyCar_Interior(SS, SS);
SS	MyCar_CourseOut(SS);
void	SE_Play(void);

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
	SS	ret = 0;
	static UC bShiftPosFlag[3] = {FALSE};

	US	uTM[6] = { 0, 2857, 1950, 1444, 1096, 761 };/* �ϑ���  1:2.857 2:1.95 3:1.444 4:1.096 5:0.761 */
//	US	uTM_F[6] = { 0, 13390, 9140, 6768, 5137, 3567 };/* �������� */
//	US	uRPM[11] = { 500, 1000, 1500, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000 };	/* Eng��]�� */
//	US	uTRQ[11] = { 120,  130,  150,  170,  210,  200,  180,  170,  140,  120,  100 };	/* �g���N */
	UC	bUpDown = 0;
	
	/* �X�e�A�����O���� */
	if((input & KEY_RIGHT) != 0u)	stMyCar.Steering += 1;	/* �E */
	if((input & KEY_LEFT) != 0u)	stMyCar.Steering -= 1;	/* �� */
	stMyCar.Steering = Mmax(Mmin(stMyCar.Steering, 31), -31);
	
	
	/* �V�t�g���� */
	if( ((input & KEY_A) != 0u) && ((input & KEY_B) == 0u))	/* A�{�^��(only) */
	{
		/* �A�N�Z�������̓V�t�g����֎~ */
	}
	else{
		if(ChatCancelSW((input & KEY_UPPER)!=0u, &bShiftPosFlag[0]) == TRUE)
		{
			if(stMyCar.ubShiftPos != 5)
			{
				bUpDown = 1;
				m_pcmplay(7,3,4);	/* SE:�V�t�g�A�b�v */
			}
			stMyCar.ubShiftPos = Mmin(stMyCar.ubShiftPos+1, 5);	/* �� */
		}
		
		if(ChatCancelSW((input & KEY_LOWER)!=0u, &bShiftPosFlag[1]) == TRUE)
		{
			if(stMyCar.ubShiftPos != 0)
			{
				bUpDown = 2;
				m_pcmplay(7,3,4);	/* SE:�V�t�g�_�E�� */
			}
			stMyCar.ubShiftPos = Mmax(stMyCar.ubShiftPos-1, 0);	/* �� */
		}
	}
#if 0
	stMyCar.ubShiftPos = 2;	/* �e�X�g�p�ϑ��Œ� */
#endif	
	stMyCar.ubShiftPos = Mmax(Mmin(stMyCar.ubShiftPos, 5), 0);
	
	if(bUpDown != 0u) 
	{
		stMyCar.uEngineRPM = (stMyCar.VehicleSpeed * uTM[stMyCar.ubShiftPos]) / 26;
		stMyCar.uEngineRPM = Mmax(Mmin(9000, stMyCar.uEngineRPM), 750);
	}
	
	/* �A�N�Z�� */
	if((input & KEY_A) != 0u){	/* A�{�^�� */
		switch(stMyCar.ubShiftPos)
		{
			case 0:
			{
				stMyCar.uEngineRPM += 200;
				SE_Play();
			}
			break;
			case 1:
			{
				stMyCar.uEngineRPM += 20;
			}
			break;
			case 2:
			{
				stMyCar.uEngineRPM += 15;
			}
			break;
			case 3:
			{
				stMyCar.uEngineRPM += 10;
			}
			break;
			case 4:
			{
				stMyCar.uEngineRPM += 6;
			}
			break;
			case 5:
			{
				stMyCar.uEngineRPM += 3;
			}
			break;
			default:
			{
				stMyCar.uEngineRPM += 100;
			}
			break;
		}
	}
	else{
		/* ��]�� */
		switch(stMyCar.ubShiftPos)
		{
			case 0:
			{
				stMyCar.uEngineRPM -= 200;
			}
			break;
			case 1:
			{
				stMyCar.uEngineRPM -= 40;
			}
			break;
			case 2:
			{
				stMyCar.uEngineRPM -= 30;
			}
			break;
			case 3:
			{
				stMyCar.uEngineRPM -= 20;
			}
			break;
			case 4:
			{
				stMyCar.uEngineRPM -= 12;
			}
			break;
			case 5:
			{
				stMyCar.uEngineRPM -= 6;
			}
			break;
			default:
			{
				stMyCar.uEngineRPM -= 200;
			}
			break;
		}
	}
	stMyCar.uEngineRPM = Mmax(Mmin(9000, stMyCar.uEngineRPM), 750);

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
	
	return ret;
}

SS	MyCar_Interior(SS x, SS y)
{
	SS	ret = 0;
	SS	i;
	UL	code = 0;
	UC	patNum = 0;
	UC	palNum = 0;
	UC	V=0, H=0;
	UC	bHit = FALSE;
	UC	sp_num=0;
	US	nRatio;
	static	SS	Vibration = 0;
	static	SS	VibrationCT = 0;
	static	US	rad = 180;
	static	UC	ubRadFlag = TRUE;
	
	ST_ENEMYCARDATA	stEnemyCar;

	volatile US *PCG_src  = (US *)0xEBA000;
	volatile US *PCG_dst  = (US *)0xEBA180;
	
	/* ��ʂ�h�炷 */
	VibrationCT++;
	if(VibrationCT > 5)
	{
		VibrationCT = 0;
	}
	
	if(stMyCar.VehicleSpeed != 0)
	{
		for(i = 0; i < ENEMYCAR_MAX; i++)
		{
			GetEnemyCAR(&stEnemyCar, i);

			if( (stEnemyCar.ubAlive == TRUE) && (stEnemyCar.z == 0) )
			{
				if( ((stEnemyCar.x - 8) <= stMyCar.Steering) && (stMyCar.Steering <= (stEnemyCar.x + 8)) )
				{
					bHit = TRUE;
				}
			}
		}
	}
	
	if(bHit == TRUE)
	{
		m_pcmplay(10,3,4);
		Vibration = (VibrationCT - 3) * 5;	/* ��ʂ̐U�� */
	}
	else
	{
		Vibration = (VibrationCT == 0)?1:0;	/* ��ʂ̐U�� */
	}

	/* ��ʂ̈ʒu */
	HOME(0b01, x, y + Vibration );	/* Screen 0(TPS/FPS) */
	HOME(0b10, x, y );				/* Screen 1 */

	/* �}�X�R�b�g���h��� */
	if(stMyCar.VehicleSpeed == 0)
	{
		if(rad == 180)
		{
			rad = 180;
		}
		else if(rad < 180)
		{
			rad++;
		}
		else
		{
			rad--;
		}
	}
	else
	{
		US	width;
		
		width = 15 + (stMyCar.VehicleSpeed / 10);
		
		if(ubRadFlag == TRUE)
		{
			rad+=Mmax(stMyCar.VehicleSpeed/30, 1);
			if(rad > 180 + width)ubRadFlag = FALSE;
		}
		else
		{
			rad-=Mmax(stMyCar.VehicleSpeed/30, 1);
			if(rad < 180 - width)ubRadFlag = TRUE;
		}
	}
	

	x = 16 + (( 32 * APL_Sin(rad)) >> 8);
	y =  0 + ((-32 * APL_Cos(rad)) >> 8) + Vibration;
	patNum = 0x40;
	palNum = 9;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 0, y + 0, code, 3);
	patNum = 0x41;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 16, y + 0, code, 3);
	patNum = 0x42;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 32, y + 0, code, 3);
	patNum = 0x50;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 0, y + 16, code, 3);
	patNum = 0x51;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 16, y + 16, code, 3);
	patNum = 0x52;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 32, y + 16, code, 3);
	patNum = 0x60;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 0, y + 32, code, 3);
	patNum = 0x61;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 16, y + 32, code, 3);
	patNum = 0x62;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 32, y + 32, code, 3);

	GetDebugNum(&nRatio);
	PCG_Rotation((US *)PCG_dst, (US *)PCG_src, 3, 3, (nRatio-0x80), 180-rad);
	
	y += 48;
	patNum = 0x43;
	palNum = 9;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 0, y + 0, code, 3);
	patNum = 0x44;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 16, y + 0, code, 3);
	patNum = 0x45;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 32, y + 0, code, 3);
	patNum = 0x53;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 0, y + 16, code, 3);
	patNum = 0x54;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 16, y + 16, code, 3);
	patNum = 0x55;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 32, y + 16, code, 3);
	patNum = 0x63;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 0, y + 32, code, 3);
	patNum = 0x64;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 16, y + 32, code, 3);
	patNum = 0x65;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 32, y + 32, code, 3);

	/* �^�R���[�^�[�j */
	for( i = 0; i < 18; i++ )
	{
		if( ((i*500) <= stMyCar.uEngineRPM) && (stMyCar.uEngineRPM < ((i*500)+500)) )
		{
			/* ��]���ŃX�v���C�g�̃p�^�[�������߂� */
			break;
		}
	}
	palNum = 0x0D;
	x = 90+16;
	y = 180+16 + Vibration;
	if(i <= 6)
	{
		y += 16;
		patNum = 0x70+i;
		V = 1;
		H = 0;
	}
	else if(i <= 12)
	{
		patNum = 0x70+12-i;
		V = 0;
		H = 0;
	}
	else
	{
		x += 16;
		patNum = 0x70+i-12;
		V = 0;
		H = 1;
	}
	
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x, y, code, 3);

	/* �^�R���[�^�[ */
	x = 90+16;
	y = 180+16 + Vibration;
	V = 0;
	H = 0;
	patNum = 0x46;
	palNum = 0x0D;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 0, y + 0, code, 3);

	patNum = 0x47;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 16, y + 0, code, 3);

	patNum = 0x56;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 0, y + 16, code, 3);

	patNum = 0x57;
	code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
	SP_REGST( sp_num++, -1, x + 16, y + 16, code, 3);
	
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

/* FM�����Ō��ʉ��Đ����� */
void	SE_Play(void)
{
	static	UC	bInit = TRUE;
	static	US	uDcount = 10;

	struct	_regs	stInReg = {0}, stOutReg = {0};
	UI	stat, TrkFreeSize;
	UC	bCh, bTrk;
	SI	level;

	UC	v[] = {
#if 0
	/* ���@�~�T�C�� */
	/*	AF  OM  WF  SY  SP PMD AMD PMS AMS PAN DUMMY	*/
		60, 15,  0,  0,  0,  0,  0,  0,  0,  3,  0,
	/*	AR  DR  SR  RR  SL  OL  KS  ML DT1 DT2 AME	*/
		27, 15,  5,  2,  0,  0,  0,  0,  3,  1,  0,
		31, 18, 18,  6,  7,  0,  0,  0,  3,  2,  0,
		22, 31,  0, 10,  0, 42,  0,  7,  7,  0,  0,
		15, 31,  0,  8,  0,  0,  2,  1,  7,  0,  0
#else
	/*	AF  OM  WF  SY  SP PMD AMD PMS AMS PAN DUMMY	*/
		59, 15,  2,  1,200,127,  0,  0,  0,  3,  0,
	/*	AR  DR  SR  RR  SL  OL  KS  ML DT1 DT2 AME */
		31,  8,  1,  8,  7, 20,  2,  1,  5,  3,  0,
		31,  8,  8,  7,  5, 24,  1,  2,  1,  1,  0,
		31,  3,  7,  8,  1, 21,  1,  1,  3,  0,  0,
		31,  0,  0,  9,  0,  0,  2,  8,  5,  2,  0
#endif
	};
	
	UC	SE_Data[59] = {	/* �\���̂ɂ��������悢�H */
		//0x01,										/* (+1)ZMD�̍\�� */
		//0x5A,0x6D,0x75,0x53,0x69,0x43,			/* (+6)ZmuSiC */
		//0x20,										/* (+7)�o�[�W���� */
		0x00,0x01,								/* ���t���g���b�N��(.W) 2 */
		0x00,0x00,0x00,0x02,					/* ���t�f�[�^�܂ł̃I�t�Z�b�g(.L) 4*/
		0x00,0x05,								/* ���t��΃`�����l��(.W) 2*/
		0xa0,0x0c,0xb6,0x05,0xb3,0xd1,0x00,0x00,	/*	���t�f�[�^ */
		0x00,0x00,0xe6,0x00,0x3c,0xe8,0x00,0x18,0xff,0xff,0xd6,0x00,0x03,0x00,0x00,0xe0,
		0x39,0x00,0x0c,0xff,0xff,0x00,0x00,0x00,0x40,0x00,0x01,0x45,0x48,0xff,0xe0,0x45,
		0x00,0x0c,0x00,0x0c,0x00,0x00,0xff,0xc0,0x00,0xff,0xff
	};
	
	bCh		= 6;
	bTrk	= 6;
	
	stat = m_stat(bCh);	/* ch6 �����t�����ǂ������� */
	Message_Num(&stat, 			0,	10,	2, MONI_Type_UI, "%4d");
	TrkFreeSize = m_free(bTrk);
	Message_Num(&TrkFreeSize,	6,	10,	2, MONI_Type_UI, "%4d");

	if(stat == 0u)	/* ���t���Ă��Ȃ� */
	{
	//	m_mute(6);
		if(bInit == TRUE)
		{
			bInit = FALSE;
		}
		m_alloc(bTrk, 120);
		m_vset(bTrk, v);
		m_assign(bCh, bTrk);
	
		m_play(bTrk,0,0,0,0,0,0,0,0,0);

		level = intlevel(7);	/* ���荞�݋֎~ */
		
		stInReg.d0 = 0xF0;			/* ZMUSIC.X�ɂ��IOCS�R�[�� */
		stInReg.d1 = 0x12;			/* se_play $12�iZMUSIC���̃t�@���N�V�����R�[���j */
		stInReg.d2 = bTrk;			/* ���t�g���b�NNo */
		stInReg.a1 = (UI)&SE_Data[0];	/* ���t�f�[�^�i�[�A�h���X */
		
		_iocs_trap15(&stInReg, &stOutReg);	/* Trap 15 */

		intlevel(level);		/* ���荞�݉��� */

		uDcount = 5;
	}
	else
	{
		if(uDcount == 0)
		{
			m_stop(bTrk,0,0,0,0,0,0,0,0,0);
			uDcount = 5;
		}
		else
		{
			uDcount--;
		}
	}
}

#endif	/* MyCar_C */

