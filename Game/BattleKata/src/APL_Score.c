#ifndef	SCORE_C
#define	SCORE_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <usr_macro.h>
#include "APL_Score.h"
#include "IF_Draw.h"
#include "IF_Graphic.h"
#include "IF_Text.h"

/* �f�[�^ */
uint32_t	g_uScoreTable[8] = {
	10,		/* 0 */
	20,		/* 1 */
	40,		/* 2 */
	80,		/* 3 */
	160,	/* 4 */
	320,	/* 5 */
	640,	/* 6 */
	1280	/* 7 */
};

/* �O���[�o���ϐ� */
static 	ST_SCORE		g_stScore = {0};
static 	ST_SCORE_POS	g_stScorePos = {0};
static	int8_t	g_bUpdateScore[3];
/* �\���̒�` */

/* �֐��̃v���g�^�C�v�錾 */
int16_t S_Get_ScoreInfo(ST_SCORE *);
int16_t S_Set_ScoreInfo(ST_SCORE);
int16_t S_Add_Score(uint64_t);
int16_t S_Add_Score_Point(uint64_t);
int16_t S_Set_Combo(int16_t);
int16_t S_All_Init_Score(void);
int16_t S_Init_Score(void);
int16_t S_GetPos(int16_t, int16_t);
int16_t S_Clear_Score(void);
int16_t S_Main_Score(void);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_Get_ScoreInfo(ST_SCORE *p_stScore)
{
	int16_t	ret = 0;

	*p_stScore = g_stScore;	/* �������擾 */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_Set_ScoreInfo(ST_SCORE stScore)
{
	int16_t	ret = 0;

	g_stScore = stScore;	/* �X�V */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_Add_Score(uint64_t ulNum)
{
	int16_t	ret = 0;
	uint64_t	val64;

	val64 = g_stScore.ulScore;
	
	g_stScore.uCombo = Mmin(g_stScore.uCombo++, 255);	/* �R���{���Z */

	g_stScore.ulScoreAdd = ulNum;
	val64 += g_stScore.ulScoreAdd * g_stScore.uCombo;	/* ���Z */
	
	/* ���݂̓_�� */
	if(g_stScore.ulScore > val64)	/* �I�[�o�[�t���[�΍� */
	{
		g_stScore.ulScore = -1;	/* �J���X�g */
	}
	else
	{
		g_stScore.ulScore = val64;	/* �X�V */
	}

	/* �ō��̓_�� */
	g_stScore.ulScoreMax = Mmax(g_stScore.ulScoreMax, g_stScore.ulScore);
	
	g_bUpdateScore[0] = TRUE;	/* �X�V */
	g_bUpdateScore[1] = TRUE;	/* �X�V */

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_Add_Score_Point(uint64_t ulNum)
{
	int16_t	ret = 0;
	uint64_t	val64;

	val64 = g_stScore.ulScore;
	
	g_stScore.ulScoreAdd = ulNum;

	val64 += g_stScore.ulScoreAdd;	/* ���Z */
	
	/* ���݂̓_�� */
	if(g_stScore.ulScore > val64)	/* �I�[�o�[�t���[�΍� */
	{
		g_stScore.ulScore = -1;	/* �J���X�g */
	}
	else
	{
		g_stScore.ulScore = val64;	/* �X�V */
	}

	/* �ō��̓_�� */
	g_stScore.ulScoreMax = Mmax(g_stScore.ulScoreMax, g_stScore.ulScore);
	
	g_bUpdateScore[0] = TRUE;	/* �X�V */
	g_bUpdateScore[2] = TRUE;	/* �X�V */

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_Set_Combo(int16_t uNum)
{
	int16_t	ret = 0;

	g_stScore.uCombo = uNum;

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_All_Init_Score(void)
{
	int16_t	ret = 0;

	/* ���Z�b�g */
	g_stScore.ulScore		= 0;
	g_stScore.ulScoreMax	= 100000;
	g_stScore.uCombo		= 0;

	g_bUpdateScore[0] = TRUE;	/* �X�V */

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_Init_Score(void)
{
	int16_t	ret = 0;

	/* ���Z�b�g */
	g_stScore.ulScore		= 0;
	g_stScore.uCombo		= 0;

	g_bUpdateScore[0] = TRUE;	/* �X�V */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_GetPos(int16_t x, int16_t y)
{
	int16_t	ret = 0;

	/* ���Z�b�g */
	g_stScorePos.x_old = g_stScorePos.x;
	g_stScorePos.y_old = g_stScorePos.y;

	g_stScorePos.x = x;
	g_stScorePos.y = y;

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_Clear_Score(void)
{
	int16_t	ret = 0;
	int8_t	sBuf[128];

//	Draw_Fill(g_stScorePos.x_old, g_stScorePos.y_old+1, g_stScorePos.x_old + 48, g_stScorePos.y_old + 11, 0);
	G_CLR_AREA(0, 255, 0, 255, 0);	/* �y�[�W 0 */

	memset(sBuf, 0, sizeof(sBuf));
	strcpy(sBuf, "STAGE CLEAR!!");
	PutGraphic_To_Symbol24(sBuf, 48 + 1, 32 + 1, F_MOJI_BAK);
	PutGraphic_To_Symbol24(sBuf, 48, 32, F_MOJI);
	
	/* �l�������X�R�A */
	memset(sBuf, 0, sizeof(sBuf));
	sprintf(sBuf, "Clear Bonus +%d x %d", g_stScore.ulScoreAdd, g_stScore.uCombo);
	Draw_Message_To_Graphic(sBuf, g_stScorePos.x, g_stScorePos.y, F_MOJI, F_MOJI_BAK);

	memset(sBuf, 0, sizeof(sBuf));
	sprintf(sBuf, "%d", g_stScore.ulScore);
	PutGraphic_To_Symbol24(sBuf, g_stScorePos.x + 32 + 1, g_stScorePos.y + 24 + 1, F_MOJI_BAK);
	PutGraphic_To_Symbol24(sBuf, g_stScorePos.x + 32, g_stScorePos.y + 24, F_MOJI);
	
	return ret;
}
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_Main_Score(void)
{
	int16_t	ret = 0;

	if(g_bUpdateScore[0] == TRUE)
	{
		int8_t	sBuf[128];

		g_bUpdateScore[0] = FALSE;

//		Draw_Fill(g_stScorePos.x_old, g_stScorePos.y_old+1, g_stScorePos.x_old + 48, g_stScorePos.y_old + 11, 0);
		G_CLR_AREA(0, 255, 0, 255, 0);	/* �y�[�W 0 */

		if(g_bUpdateScore[1] == TRUE)
		{
			g_bUpdateScore[1] = FALSE;

			/* �l�������X�R�A */
			memset(sBuf, 0, sizeof(sBuf));
			if(g_stScore.uCombo > 1)
			{
				sprintf(sBuf, "+%dx%d", g_stScore.ulScoreAdd, g_stScore.uCombo);
			}
			else
			{
				sprintf(sBuf, "+%d", g_stScore.ulScoreAdd );
			}
			Draw_Message_To_Graphic(sBuf, g_stScorePos.x, g_stScorePos.y, F_MOJI, F_MOJI_BAK);
		}

		if(g_bUpdateScore[2] == TRUE)
		{
			g_bUpdateScore[2] = FALSE;

			/* �l�������X�R�A */
			memset(sBuf, 0, sizeof(sBuf));
			sprintf(sBuf, "+%d", g_stScore.ulScoreAdd );
			Draw_Message_To_Graphic(sBuf, g_stScorePos.x, g_stScorePos.y, F_MOJI, F_MOJI_BAK);
		}

		/* �X�R�A */
		memset(sBuf, 0, sizeof(sBuf));
		sprintf(sBuf, "Score %8d Hi-Score %8d", g_stScore.ulScore, g_stScore.ulScoreMax);
		Draw_Message_To_Graphic(sBuf, 0, 0, F_MOJI, F_MOJI_BAK);	/* �X�R�A�\�� */
	}

	return ret;
}
#endif	/* SCORE_C */

