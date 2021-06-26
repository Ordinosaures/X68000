#ifndef	TEXT_C
#define	TEXT_C

#include <iocslib.h>

#include "inc/usr_macro.h"
#include "Text.h"

#include "APL_Math.h"
#include "CRTC.h"
#include "Graphic.h"
#include "MFP.h"
#include "MyCar.h"
#include "OutPut_Text.h"
#include "PCG.h"
#include "Score.h"

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
void T_Main(void);
int16_t T_PutTextInfo(ST_TEXTINFO);
int16_t T_Scroll(uint32_t, uint32_t);
int32_t T_Box(int16_t, int16_t, int16_t, int16_t, uint16_t, uint8_t);
int32_t T_Fill(int16_t, int16_t, int16_t, int16_t, uint16_t, uint8_t);
int32_t T_xLine(int16_t, int16_t, int16_t w, uint16_t, uint8_t);
int32_t T_yLine(int16_t, int16_t, int16_t h, uint16_t, uint8_t);
int16_t T_Circle(int16_t, int16_t, int16_t, int16_t, uint16_t, uint8_t);
int16_t T_Get_TextInfo(ST_TEXTINFO *);
int16_t T_Set_TextInfo(ST_TEXTINFO);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_INIT(void)
{
	B_CUROFF();			/* �J�[�\���������܂� */
	MS_CUROF();			/* �}�E�X�J�[�\���������܂� */
	SKEY_MOD(0, 0, 0);	/* �\�t�g�E�F�A�L�[�{�[�h�������܂� */
	T_Clear();			/* �e�L�X�g�N���A */
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_EXIT(void)
{
	B_CURON();	/* �J�[�\����\�����܂� */
	T_Clear();	/* �e�L�X�g�N���A */
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_Clear(void)
{
	struct _txfillptr stTxFill;
	
	stTxFill.vram_page = 0;
	stTxFill.x = 0;
	stTxFill.y = 0;
	stTxFill.x1= 1023;
	stTxFill.y1= 1023;
	stTxFill.fill_patn = 0;
	
	_iocs_scroll(8, 0, 0);	/* 8:�e�L�X�g��� */
//	_iocs_txrascpy(256 / 4 * 256, 256 / 4, 0b1111);	/* �e�L�X�g��ʃN���A */
	_iocs_txfill(&stTxFill);
	stTxFill.vram_page = 1;
	_iocs_txfill(&stTxFill);
	stTxFill.vram_page = 2;
	_iocs_txfill(&stTxFill);
	stTxFill.vram_page = 3;
	_iocs_txfill(&stTxFill);
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
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

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_SetBG_to_Text(void)
{
	int16_t	i;
	uint16_t	x, y;

	/* �e�L�X�g�G���A�ɍ�ƃf�[�^�W�J1 */
	x = 0;
	y = 256;
	BG_TextPut("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", x, y);//232
	
	/* �e�L�X�g�G���A�ɍ�ƃf�[�^�W�J2 */
	x = 0;
	y += 8;//240	248+8=256�ȍ~��NG
	for(i=0; i < 10; i++)
	{
		BG_PutToText(   0x80+ (i<<1) + 0, x + BG_WIDTH * i,	y,				BG_Normal, TRUE);	/* ������i�㑤�j*/
		BG_PutToText(   0x80+ (i<<1) + 1, x + BG_WIDTH * i,	y+BG_HEIGHT,	BG_Normal, TRUE);	/* ������i�����j*/
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_TopScore(void)
{
	uint32_t	i,e;
	uint32_t	uWidth, uHeight, uFileSize;
	uint16_t	x = 0, y = 0;
	
	Get_PicImageInfo( MYCAR_CG, &uWidth, &uHeight, &uFileSize );	/* �摜�̏����擾 */
	
	for(i=0; i < MYCAR_IMAGE_MAX; i++)
	{
		/* TOP */
		BG_PutToText(  1, (BG_WIDTH * 0) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 1) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 2) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 3) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  3, (BG_WIDTH * 4) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �E�� */
		for(e = 0; e < 5; e++)
		{
			BG_PutToText(e+4, (BG_WIDTH * e) + (uWidth * x),  8 + (Y_OFFSET * y) , BG_Normal, FALSE);
		}
		BG_PutToText(  1, (BG_WIDTH * 0) + (uWidth * x),  16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 1) + (uWidth * x),  16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 2) + (uWidth * x),  16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 3) + (uWidth * x),  16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  3, (BG_WIDTH * 4) + (uWidth * x),  16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �E�� */
		
		x++;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_Time(void)
{
	uint32_t	i,e;
	uint32_t	uWidth, uHeight, uFileSize;
	uint16_t	x = 0, y = 0;
	
	Get_PicImageInfo( MYCAR_CG, &uWidth, &uHeight, &uFileSize );	/* �摜�̏����擾 */

	for(i=0; i < MYCAR_IMAGE_MAX; i++)
	{
		/* TIME */
		BG_PutToText(  1, (BG_WIDTH * 12) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 13) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 14) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 15) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 16) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  3, (BG_WIDTH * 17) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �E�� */
		BG_PutToText(  4, (BG_WIDTH * 12) + (uWidth * x),  8 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		for(e = 0; e < 5; e++)
		{
			BG_PutToText(e+10, (BG_WIDTH * (13 + e)) + (uWidth * x),  8 + (Y_OFFSET * y) , BG_Normal, FALSE);
		}
		BG_PutToText(  1, (BG_WIDTH * 12) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 13) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 14) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 15) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 16) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  3, (BG_WIDTH * 17) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �E�� */

		x++;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_Score(void)
{
	uint32_t	i,e;
	uint32_t	uWidth, uHeight, uFileSize;
	uint16_t	x = 0, y = 0;
	
	Get_PicImageInfo( MYCAR_CG, &uWidth, &uHeight, &uFileSize );	/* �摜�̏����擾 */

	for(i=0; i < MYCAR_IMAGE_MAX; i++)
	{
		/* SCORE */
		BG_PutToText(  1, (BG_WIDTH * 17) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 18) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 19) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 20) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 21) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 22) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		BG_PutToText( 15, (BG_WIDTH * 23) + (uWidth * x),  0 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �E�� */
		BG_PutToText(  4, (BG_WIDTH * 17) + (uWidth * x),  8 + (Y_OFFSET * y) , BG_Normal, FALSE);	/* �� */
		for(e = 0; e < 6; e++)
		{
			BG_PutToText(e+16, (BG_WIDTH * (18 + e)) + (uWidth * x),  8 + (Y_OFFSET * y) , BG_Normal, FALSE);
		}
		BG_PutToText(  1, (BG_WIDTH * 17) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* ���� */
		BG_PutToText(  2, (BG_WIDTH * 18) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 19) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 20) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 21) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText(  2, (BG_WIDTH * 22) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �� */
		BG_PutToText( 15, (BG_WIDTH * 23) + (uWidth * x), 16 + (Y_OFFSET * y) , BG_H_rev, FALSE);	/* �E�� */

		x++;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_Speed(void)
{
	uint32_t	i;
	uint32_t	uWidth, uHeight, uFileSize;
	uint16_t	x = 0, y = 0;
	
	Get_PicImageInfo( MYCAR_CG, &uWidth, &uHeight, &uFileSize );	/* �摜�̏����擾 */

	for(i=0; i < MYCAR_IMAGE_MAX; i++)
	{
		/* SPEED */
		BG_TextPut("SPEED", 148 + (uWidth * x), 24 + (Y_OFFSET * y) );
		/* KM */
		BG_TextPut("KM",    216 + (uWidth * x), 24 + (Y_OFFSET * y) );

		x++;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_Gear(void)
{
	uint32_t	i;
	uint32_t	uWidth, uHeight, uFileSize;
	uint16_t	x = 0, y = 0;
	
	Get_PicImageInfo( MYCAR_CG, &uWidth, &uHeight, &uFileSize );	/* �摜�̏����擾 */

	for(i=0; i < MYCAR_IMAGE_MAX; i++)
	{
		/* GEAR */
		BG_TextPut("GEAR", 152 + (uWidth * x), 32 + (Y_OFFSET * y) );

		x++;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void T_Main(void)
{
	uint32_t time_now;
	static uint32_t time_old = 0;
	uint32_t unStart_time, unTimer;
	uint16_t uTimeCounter;
	static uint32_t unPassTime = 0;
	ST_CARDATA	stMyCar = {0};
	ST_SCORE stScore;
	
	S_Get_ScoreInfo(&stScore);	/* �X�R�A���̎擾 */

	GetMyCar(&stMyCar);			/* ���Ԃ̏����擾 */

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
			uTimeCounter = (uint16_t)(unTimer / 1000);
		}
		g_stTextInfo.uTimeCounter = uTimeCounter;
	}
	time_old = unStart_time;
	
	/* Score */
	g_stTextInfo.ulScore = stScore.ulScore;

	/* Top Score */
	g_stTextInfo.ulScoreMax = stScore.ulScoreMax;
	
	/* Speed */
	g_stTextInfo.uVs = stMyCar.VehicleSpeed;
	
	/* Gear */
	g_stTextInfo.uShiftPos = (uint16_t)stMyCar.ubShiftPos;
	
	/* �`�� */
	T_PutTextInfo(g_stTextInfo);
	
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t T_PutTextInfo(ST_TEXTINFO stTextInfo)
{
	int16_t	ret = 0;
	
	uint16_t	x, y;
	uint32_t	uWidth, uHeight, uFileSize;

	x = stTextInfo.uPosX;
	y = stTextInfo.uPosY;

	Get_PicImageInfo( MYCAR_CG, &uWidth, &uHeight, &uFileSize );	/* �摜�̏����擾 */
	
	/* Top Score */
	Text_To_Text2(stTextInfo.ulScoreMax,				(uWidth * x) +  36, (Y_OFFSET * y) +  8, FALSE, "%8d");
	/* Score */
	Text_To_Text2(stTextInfo.ulScore,					(uWidth * x) + 192, (Y_OFFSET * y) +  8, FALSE, "%8d");
	/* Time Count */
	Text_To_Text2((uint64_t)stTextInfo.uTimeCounter,	(uWidth * x) + 108, (Y_OFFSET * y) + 24,  TRUE, "%3d");
	/* Speed */
	Text_To_Text2((uint64_t)stTextInfo.uVs,				(uWidth * x) + 190, (Y_OFFSET * y) + 24, FALSE, "%3d");
	/* Gear */
	Text_To_Text2((uint64_t)stTextInfo.uShiftPos,		(uWidth * x) + 206, (Y_OFFSET * y) + 32, FALSE,  "%d");

	/* �\�����W�ύX */
	T_Scroll( (uWidth * x), (Y_OFFSET * y) );	/* �e�L�X�g��ʃX�N���[�� */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t T_Scroll(uint32_t uPosX, uint32_t uPosY)
{
	int16_t	ret = 0;
	volatile uint16_t *CRTC_R10 = (uint16_t *)0xE80014u;	/* �e�L�X�gX�����X�N���[�� */
	volatile uint16_t *CRTC_R11 = (uint16_t *)0xE80016u;	/* �e�L�X�gY�����X�N���[�� */

	*CRTC_R10 = Mbset(*CRTC_R10, 0x03FF, uPosX);	/* CRTC R10 */
	*CRTC_R11 = Mbset(*CRTC_R11, 0x03FF, uPosY);	/* CRTC R11 */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int32_t T_Box(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t line_style, uint8_t color)
{
	int32_t	ret = 0;
	struct _tboxptr stTxBox;
	
	stTxBox.x = x1;
	stTxBox.y = y1;
	stTxBox.x1= x2;
	stTxBox.y1= y2;
	stTxBox.line_style = line_style;
	
	if((color == 0) && (line_style == 0))
	{
		color = 0x0F;
	}

	if((color & 0x01) != 0u)
	{
		stTxBox.vram_page = 0;
		_iocs_txbox(&stTxBox);
	}
	if((color & 0x02) != 0u)
	{
		stTxBox.vram_page = 1;
		_iocs_txbox(&stTxBox);
	}
	if((color & 0x04) != 0u)
	{
		stTxBox.vram_page = 2;
		_iocs_txbox(&stTxBox);
	}
	if((color & 0x08) != 0u)
	{
		stTxBox.vram_page = 3;
		_iocs_txbox(&stTxBox);
	}
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int32_t T_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t line_style, uint8_t color)
{
	int32_t	ret = 0;
	struct _txfillptr stTxFill;
	
	stTxFill.x = x1;
	stTxFill.y = y1;
	stTxFill.x1= x2;
	stTxFill.y1= y2;
	stTxFill.fill_patn = line_style;
	
	if((color == 0) && (line_style == 0))
	{
		color = 0x0F;
	}
	
	if((color & 0x01) != 0u)
	{
		stTxFill.vram_page = 0;
		_iocs_txfill(&stTxFill);
	}
	if((color & 0x02) != 0u)
	{
		stTxFill.vram_page = 1;
		_iocs_txfill(&stTxFill);
	}
	if((color & 0x04) != 0u)
	{
		stTxFill.vram_page = 2;
		_iocs_txfill(&stTxFill);
	}
	if((color & 0x08) != 0u)
	{
		stTxFill.vram_page = 3;
		_iocs_txfill(&stTxFill);
	}
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int32_t T_xLine(int16_t x1, int16_t y1, int16_t w, uint16_t line_style, uint8_t color)
{
	int32_t	ret = 0;
	struct _xlineptr stTxLine;
	
	stTxLine.x = x1;
	stTxLine.y = y1;
	stTxLine.x1= w;
	stTxLine.line_style = line_style;
	
	if((color == 0) && (line_style == 0))
	{
		color = 0x0F;
	}

	if((color & 0x01) != 0u)
	{
		stTxLine.vram_page = 0;
		_iocs_txxline(&stTxLine);
	}
	if((color & 0x02) != 0u)
	{
		stTxLine.vram_page = 1;
		_iocs_txxline(&stTxLine);
	}
	if((color & 0x04) != 0u)
	{
		stTxLine.vram_page = 2;
		_iocs_txxline(&stTxLine);
	}
	if((color & 0x08) != 0u)
	{
		stTxLine.vram_page = 3;
		_iocs_txxline(&stTxLine);
	}
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int32_t T_yLine(int16_t x1, int16_t y1, int16_t h, uint16_t line_style, uint8_t color)
{
	int32_t	ret = 0;
	struct _ylineptr stTyLine;
	
	stTyLine.x = x1;
	stTyLine.y = y1;
	stTyLine.y1= h;
	stTyLine.line_style = line_style;
	
	if((color == 0) && (line_style == 0))
	{
		color = 0x0F;
	}

	if((color & 0x01) != 0u)
	{
		stTyLine.vram_page = 0;
		_iocs_txyline(&stTyLine);
	}
	if((color & 0x02) != 0u)
	{
		stTyLine.vram_page = 1;
		_iocs_txyline(&stTyLine);
	}
	if((color & 0x04) != 0u)
	{
		stTyLine.vram_page = 2;
		_iocs_txyline(&stTyLine);
	}
	if((color & 0x08) != 0u)
	{
		stTyLine.vram_page = 3;
		_iocs_txyline(&stTyLine);
	}
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t T_Circle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t line_style, uint8_t color)
{
	int16_t	ret = 0;
	int16_t	i;
	int16_t	w, h;
	int16_t	cx, cy;
	int16_t	px, py;
	
	x2 += x1;
	y2 += y1;
	
	/* �c�� */
	if(x1 == x2)
	{
		T_yLine(x1, y1, 1, line_style, color);
		return ret;
	}
	
	/* ���� */
	if(y1 == y2)
	{
		T_xLine(x1, y1, 1, line_style, color);
		return ret;
	}
	
	w = Mdiv2(x2 - x1);
	h = Mdiv2(y2 - y1);
	cx = x1 + w;
	cy = y1 + h;
	
#ifdef DEBUG
//	printf("T_Circle=(%d,%d,%d,%d)(%d,%d)(%d,%d)\n",  x1, y1, x2, y2, cx, cy, w, h);
//	KeyHitESC();	/* �f�o�b�O�p */
#endif
	
	for(i=0; i<90; i++)
	{
		/* 1 */
		px = cx + Mdiv256( (w * APL_Cos(i) ) );
		py = cy + Mdiv256( (h * APL_Sin(i) ) );

		T_Fill( cx, py, px - cx, cy - py, line_style, color);
#ifdef DEBUG
//		printf("T_Circle(%d)=(%d,%d,%d,%d)\n", i, cx, py, px - cx, cy - py);
//		KeyHitESC();	/* �f�o�b�O�p */
#endif

		/* 2 */
		px = cx + Mdiv256( (w * APL_Cos(i+90)) );
		py = cy + Mdiv256( (h * APL_Sin(i+90)) );

		T_Fill( px, py, cx - px, cy - py, line_style, color);
#ifdef DEBUG
//		printf("T_Circle(%d)=(%d,%d,%d,%d)\n", i, px, py, cx - px, cy - py);
//		KeyHitESC();	/* �f�o�b�O�p */
#endif

		/* 3 */
		px = cx + Mdiv256( (w * APL_Cos(i+180)) );
		py = cy + Mdiv256( (h * APL_Sin(i+180)) );

		T_Fill( px, cy, cx - px, py - cy, line_style, color);
#ifdef DEBUG
//		printf("T_Circle(%d)=(%d,%d,%d,%d)\n", i, px, cy, cx - px, py - cy);
//		KeyHitESC();	/* �f�o�b�O�p */
#endif

		/* 4 */
		px = cx + Mdiv256( (w * APL_Cos(i+270)) );
		py = cy + Mdiv256( (h * APL_Sin(i+270)) );

		T_Fill( cx, cy, px - cx, py - cy, line_style, color);
#ifdef DEBUG
//		printf("T_Circle(%d)=(%d,%d,%d,%d)\n", i, cx, cy, px - cx, py - cy);
//		KeyHitESC();	/* �f�o�b�O�p */
#endif
	}
		
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t T_Get_TextInfo(ST_TEXTINFO *pTextInfo)
{
	int16_t	ret = 0;

	*pTextInfo = g_stTextInfo;	/* �������擾 */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t T_Set_TextInfo(ST_TEXTINFO TextInfo)
{
	int16_t	ret = 0;

	g_stTextInfo = TextInfo;	/* �X�V */
	
	return ret;
}

#endif	/* TEXT_C */

