#ifndef	TEXT_C
#define	TEXT_C

#include <iocslib.h>

#include "inc/usr_macro.h"
#include "Text.h"
#include "OutPut_Text.h"

/* �֐��̃v���g�^�C�v�錾 */
void T_INIT(void);
void TextClear(void);
void T_TopScore(void);
void T_Time(void);
void T_Score(void);

/* �֐� */
void T_INIT(void)
{
	/*�J�[�\���������܂��B*/
	B_CUROFF();
	MS_CUROF();
	SKEY_MOD(0, 0, 0);
	TextClear();

	/* �e�L�X�g�p���b�g�̏�����(Pal0��SP�Ƌ���) */
	TPALET2( 0, SetRGB( 0,  0,  0));	/* Black */
	TPALET2( 1, SetRGB( 1,  0,  0));	/* Black2 */
	TPALET2( 2, SetRGB(31,  0,  0));	/* Red */
	TPALET2( 3, SetRGB(30, 26, 16));	/* Red2 */
	TPALET2( 4, SetRGB(31, 31, 31));	/* White */
	TPALET2( 5, SetRGB(30,  8,  0));	/* Orenge */
	TPALET2( 6, SetRGB(30, 30,  0));	/* Yellow */
	TPALET2( 7, SetRGB( 0, 31,  0));	/* Green */
}

void TextClear(void)
{
	_iocs_txrascpy(256 / 4 * 256, 256 / 4, 0b1111);	/* �e�L�X�g��ʃN���A */
}

void T_TopScore(void)
{
	UI	e;
	/* TOP */
	BG_PutToText(  1, (BG_WIDTH * 0),  0, BG_Normal, FALSE);	/* ���� */
	BG_PutToText(  2, (BG_WIDTH * 1),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 2),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 3),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  3, (BG_WIDTH * 4),  0, BG_Normal, FALSE);	/* �E�� */
	for(e = 0; e < 5; e++)
	{
		BG_PutToText(e+4, (BG_WIDTH * e),  8, BG_Normal, FALSE);
	}
	BG_PutToText(  1, (BG_WIDTH * 0),  16, BG_H_rev, FALSE);	/* ���� */
	BG_PutToText(  2, (BG_WIDTH * 1),  16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 2),  16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 3),  16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  3, (BG_WIDTH * 4),  16, BG_H_rev, FALSE);	/* �E�� */
}

void T_Time(void)
{
	UI	e;
	/* TIME */
	BG_PutToText(  1, (BG_WIDTH * 12),  0, BG_Normal, FALSE);	/* ���� */
	BG_PutToText(  2, (BG_WIDTH * 13),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 14),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 15),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 16),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  3, (BG_WIDTH * 17),  0, BG_Normal, FALSE);	/* �E�� */
	BG_PutToText(  4, (BG_WIDTH * 12),  8, BG_Normal, FALSE);	/* �� */
	for(e = 0; e < 5; e++)
	{
		BG_PutToText(e+10, (BG_WIDTH * (13 + e)),  8, BG_Normal, FALSE);
	}
	BG_PutToText(  1, (BG_WIDTH * 12), 16, BG_H_rev, FALSE);	/* ���� */
	BG_PutToText(  2, (BG_WIDTH * 13), 16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 14), 16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 15), 16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 16), 16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  3, (BG_WIDTH * 17), 16, BG_H_rev, FALSE);	/* �E�� */

}

void T_Score(void)
{
	UI	e;
	/* SCORE */
	BG_PutToText(  1, (BG_WIDTH * 17),  0, BG_Normal, FALSE);	/* ���� */
	BG_PutToText(  2, (BG_WIDTH * 18),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 19),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 20),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 21),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 22),  0, BG_Normal, FALSE);	/* �� */
	BG_PutToText( 15, (BG_WIDTH * 23),  0, BG_Normal, FALSE);	/* �E�� */
	BG_PutToText(  4, (BG_WIDTH * 17),  8, BG_Normal, FALSE);	/* �� */
	for(e = 0; e < 6; e++)
	{
		BG_PutToText(e+16, (BG_WIDTH * (18 + e)),  8, BG_Normal, FALSE);
	}
	BG_PutToText(  1, (BG_WIDTH * 17), 16, BG_H_rev, FALSE);	/* ���� */
	BG_PutToText(  2, (BG_WIDTH * 18), 16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 19), 16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 20), 16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 21), 16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText(  2, (BG_WIDTH * 22), 16, BG_H_rev, FALSE);	/* �� */
	BG_PutToText( 15, (BG_WIDTH * 23), 16, BG_H_rev, FALSE);	/* �E�� */

}
#endif	/* TEXT_C */

