#ifndef	TEXT_C
#define	TEXT_C

#include <iocslib.h>

#include "inc/usr_macro.h"
#include "Text.h"

#include "Graphic.h"
#include "MFP.h"
#include "MyCar.h"
#include "OutPut_Text.h"

/* �\���� */
static ST_TEXTINFO	g_stTextInfo = {0};

/* �֐��̃v���g�^�C�v�錾 */
void T_INIT(void);
void T_EXIT(void);
void T_Clear(void);
void T_PALET(void);
void T_SetBG_to_Text(void);
void T_TopScore(void);
void T_Time(void);
void T_Score(void);
void T_Speed(void);
void T_Gear(void);
void T_Main(UC);
SS T_Scroll(UI, UI);

/* �֐� */
void T_INIT(void)
{
	B_CUROFF();			/* �J�[�\���������܂� */
	MS_CUROF();			/* �}�E�X�J�[�\���������܂� */
	SKEY_MOD(0, 0, 0);	/* �\�t�g�E�F�A�L�[�{�[�h�������܂� */
	T_Clear();			/* �e�L�X�g�N���A */
	
	g_stTextInfo.uScoreMax = 10000;
}

void T_EXIT(void)
{
	B_CURON();	/* �J�[�\����\�����܂� */
	T_Clear();	/* �e�L�X�g�N���A */
}

void T_Clear(void)
{
	struct _txfillptr stTxFill;
	
	stTxFill.vram_page = 0;
	stTxFill.x = 0;
	stTxFill.y = 0;
	stTxFill.x1= 1023;
	stTxFill.y1= 1023;
	stTxFill.fill_patn = 0;
	
	_iocs_scroll(8, 0, 0);	/* �e�L�X�g��� */
//	_iocs_txrascpy(256 / 4 * 256, 256 / 4, 0b1111);	/* �e�L�X�g��ʃN���A */
	_iocs_txfill(&stTxFill);
	stTxFill.vram_page = 1;
	_iocs_txfill(&stTxFill);
	stTxFill.vram_page = 2;
	_iocs_txfill(&stTxFill);
	stTxFill.vram_page = 3;
	_iocs_txfill(&stTxFill);
}

void T_PALET(void)
{
	/* �e�L�X�g�p���b�g�̏�����(Pal0��SP�Ƌ���) */
	TPALET2( 0, SetRGB( 0,  0,  0));	/* Black */
	TPALET2( 1, SetRGB( 1,  0,  0));	/* Black2 */
	TPALET2( 2, SetRGB(31,  0,  0));	/* Red */
	TPALET2( 3, SetRGB(30, 26, 16));	/* Red2 */
	TPALET2( 4, SetRGB(31, 31, 31));	/* White */
	TPALET2( 5, SetRGB(30,  8,  0));	/* Orenge */
	TPALET2( 6, SetRGB(30, 30,  0));	/* Yellow */
	TPALET2( 7, SetRGB( 0, 31,  0));	/* Green */
//	TPALET2( 8, SetRGB( 8,  8,  8));	/* Glay */
	/* 8�`15�O���t�B�b�N���e�L�X�g�������摜�Ŏg�p */
	TPALET2( 8, SetRGB( 4,  4,  4));	/* Glay */
	TPALET2(15, SetRGB(31, 31, 31));	/* White */
}

void T_SetBG_to_Text(void)
{
	SS	i;
	US	x, y;

	/* �e�L�X�g�G���A�ɍ�ƃf�[�^�W�J1 */
	BG_TextPut("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", 256, 0);//232
	/* �e�L�X�g�G���A�ɍ�ƃf�[�^�W�J2 */
	for(i=0; i < 10; i++)
	{
		x = 256;
		y = 8;//240	248+8=256�ȍ~��NG
		BG_PutToText(   0x80+ (i<<1) + 0, x + BG_WIDTH * i,	y,				BG_Normal, TRUE);	/* ������i�㑤�j*/
		BG_PutToText(   0x80+ (i<<1) + 1, x + BG_WIDTH * i,	y+BG_HEIGHT,	BG_Normal, TRUE);	/* ������i�����j*/
	}
}

void T_TopScore(void)
{
	UI	i,e;
	for(i=0; i<2; i++)
	{
		/* TOP */
		BG_PutToText(  1, (BG_WIDTH * 0),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 1),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 2),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 3),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  3, (BG_WIDTH * 4),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �E�� */
		for(e = 0; e < 5; e++)
		{
			BG_PutToText(e+4, (BG_WIDTH * e),  8 + (Y_OFFSET * i) , BG_Normal, FALSE);
		}
		BG_PutToText(  1, (BG_WIDTH * 0),  16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 1),  16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 2),  16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 3),  16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  3, (BG_WIDTH * 4),  16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �E�� */
	}
}

void T_Time(void)
{
	UI	i,e;
	for(i=0; i<2; i++)
	{
		/* TIME */
		BG_PutToText(  1, (BG_WIDTH * 12),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 13),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 14),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 15),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 16),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  3, (BG_WIDTH * 17),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �E�� */
		BG_PutToText(  4, (BG_WIDTH * 12),  8 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		for(e = 0; e < 5; e++)
		{
			BG_PutToText(e+10, (BG_WIDTH * (13 + e)),  8 + (Y_OFFSET * i) , BG_Normal, FALSE);
		}
		BG_PutToText(  1, (BG_WIDTH * 12), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 13), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 14), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 15), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 16), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  3, (BG_WIDTH * 17), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �E�� */
	}
}

void T_Score(void)
{
	UI	i,e;
	for(i=0; i<2; i++)
	{
		/* SCORE */
		BG_PutToText(  1, (BG_WIDTH * 17),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 18),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 19),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 20),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 21),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 22),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		BG_PutToText( 15, (BG_WIDTH * 23),  0 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �E�� */
		BG_PutToText(  4, (BG_WIDTH * 17),  8 + (Y_OFFSET * i) , BG_Normal, FALSE);	/* �� */
		for(e = 0; e < 6; e++)
		{
			BG_PutToText(e+16, (BG_WIDTH * (18 + e)),  8 + (Y_OFFSET * i) , BG_Normal, FALSE);
		}
		BG_PutToText(  1, (BG_WIDTH * 17), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 18), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 19), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 20), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 21), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 22), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText( 15, (BG_WIDTH * 23), 16 + (Y_OFFSET * i) , BG_H_rev, FALSE);	/* �E�� */
	}
}

void T_Speed(void)
{
	UI	i;
	for(i=0; i<2; i++)
	{
		/* SPEED */
		BG_TextPut("SPEED", 164, 24 + (Y_OFFSET * i) );
		/* KM */
		BG_TextPut("KM", 232, 24 + (Y_OFFSET * i) );
	}
}

void T_Gear(void)
{
	UI	i;
	for(i=0; i<2; i++)
	{
		/* GEAR */
		BG_TextPut("GEAR", 172, 32 + (Y_OFFSET * i) );
	}
}

void T_Main(UC bMode)
{
	UI time_now;
	static UI time_old = 0;
	UI unStart_time, unTimer;
	US uTimeCounter;
	static UI unPassTime = 0;
	ST_CRT		stCRT = {0};
	ST_CARDATA	stMyCar = {0};

	GetMyCar(&stMyCar);			/* ���Ԃ̏����擾 */

	/* ���[�h�ؑւɂ��ݒ�l�̕ύX */
	GetCRT(&stCRT, bMode);

	/* ���ݎ��� */
	GetNowTime(&time_now);
	/* �J�n���� */
	GetStartTime(&unStart_time);
	
	/* Time Count */
	if(time_old != 0)
	{
		unPassTime += (unStart_time - time_old);
		unTimer = 120000 - unPassTime;
		if(120000 < unTimer)
		{
			uTimeCounter = 0;
		}
		else
		{
			uTimeCounter = (US)(unTimer / 1000);
		}
		g_stTextInfo.uTimeCounter = uTimeCounter;
	}
	time_old = unStart_time;
	
	/* Score */
	g_stTextInfo.uScore = unPassTime / 1000;

	/* Top Score */
	g_stTextInfo.uScoreMax = Mmax(g_stTextInfo.uScore, g_stTextInfo.uScoreMax);
	
	/* Speed */
	g_stTextInfo.uVs = stMyCar.VehicleSpeed;
	
	/* Gear */
	g_stTextInfo.uShiftPos = (US)stMyCar.ubShiftPos;

	/* ���W */
	g_stTextInfo.uPosX = 0u;	/* X���W */
	g_stTextInfo.uPosY = stCRT.view_offset_y;	/* Y���W */
	
	/* �`�� */
	PutTextInfo(g_stTextInfo);
}

SS T_Scroll(UI uPosX, UI uPosY)
{
	SS	ret = 0;
	volatile US *CRTC_R10 = (US *)0xE80014u;	/* �e�L�X�gX�����X�N���[�� */
	volatile US *CRTC_R11 = (US *)0xE80016u;	/* �e�L�X�gY�����X�N���[�� */

	*CRTC_R10 = Mbset(*CRTC_R10, 0x03FF, uPosX);	/* CRTC R10 */
	*CRTC_R11 = Mbset(*CRTC_R11, 0x03FF, uPosY);	/* CRTC R11 */
	
	return ret;
}

#endif	/* TEXT_C */

