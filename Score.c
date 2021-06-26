#ifndef	SCORE_C
#define	SCORE_C

#include <stdio.h>

#include "inc/usr_macro.h"
#include "Score.h"

#include "Music.h"
#include "PCG.h"

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
static 	ST_SCORE	g_stScore = {0};
static 	uint8_t		g_bSetScore = FALSE;
static 	uint16_t	g_uScoreNum;

/* �\���̒�` */

/* �֐��̃v���g�^�C�v�錾 */
int16_t S_Get_ScoreInfo(ST_SCORE *);
int16_t S_Set_ScoreInfo(ST_SCORE);
int16_t S_Add_Score(void);
int16_t S_Reset_ScoreID(void);
int16_t S_All_Init_Score(void);
int16_t S_Init_Score(void);
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
int16_t S_Add_Score(void)
{
	int16_t	ret = 0;
	uint64_t	val64;

	val64 = g_stScore.ulScore;
	
	g_uScoreNum = g_uScoreTable[g_stScore.ubScoreID];
	g_bSetScore = TRUE;

	val64 += g_uScoreNum;	/* ���Z */
	
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
	
	/* �X�R�AID�̉��Z */
	if(g_stScore.ubScoreID >= 7)
	{
		/* �������Ȃ� */
	}
	else
	{
		g_stScore.ubScoreID++;
	}
	ADPCM_Play(18);	/* SE�FPON */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t S_Reset_ScoreID(void)
{
	int16_t	ret = 0;

	g_stScore.ubScoreID = 0;	/* ���Z�b�g */
	
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
	int16_t	x, y;
	int16_t	i;
	ST_PCG	*p_stPCG = NULL;

	/* ���Z�b�g */
	g_stScore.ulScore		= 0;
	g_stScore.ubScoreID		= 0;
	g_stScore.ulScoreMax	= 10000;

	g_uScoreNum = 0;
	g_bSetScore = FALSE;

	x = 128;
	y = 160;
	
	for(i = SCORE_PCG_1; i <= SCORE_PCG_4; i++)
	{
		p_stPCG = PCG_Get_Info(i);	/* �X�R�A */
		if(p_stPCG != NULL)
		{
			p_stPCG->x = SP_X_OFFSET + x - (Mdiv2(SP_W) * (i - SCORE_PCG_1));
			p_stPCG->y = SP_Y_OFFSET + y;
			p_stPCG->update	= TRUE;
		}
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
int16_t S_Init_Score(void)
{
	int16_t	ret = 0;

	/* ���Z�b�g */
	g_stScore.ulScore		= 0;
	g_stScore.ubScoreID		= 0;
	
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
	int16_t	x, y;
	int16_t	i;
	int16_t	digit=0;
	uint16_t	number;

	static uint8_t bJump = FALSE;
	static int16_t uAcc;

	ST_PCG	*p_stPCG = NULL;
	
	if(bJump == FALSE)
	{
		if(g_bSetScore == TRUE)
		{
			uAcc = -18;
			bJump = TRUE;
		}
	}
	
	if(bJump == TRUE)
	{
		uAcc += 3;
	}
	number = g_uScoreNum;
	
	do{
		number = number / 10;
		digit++;
	}while(number!=0);
	
	number = g_uScoreNum;
	
	x = 128;
	y = 160;
	
	for(i = SCORE_PCG_1; i <= SCORE_PCG_4; i++)
	{
		p_stPCG = PCG_Get_Info(i);	/* �X�R�A */
		if(p_stPCG != NULL)
		{
			p_stPCG->Anime = number % 10;
			number /= 10;
			
			if(digit > (i - SCORE_PCG_1))
			{
				p_stPCG->x = SP_X_OFFSET + x - (Mdiv2(SP_W) * (i - SCORE_PCG_1));
				p_stPCG->y += uAcc;
				p_stPCG->update	= TRUE;
			}
			else
			{
				p_stPCG->x = SP_X_OFFSET + x - (Mdiv2(SP_W) * (i - SCORE_PCG_1));
				p_stPCG->y = SP_Y_OFFSET + y;
				p_stPCG->update	= TRUE;
			}

			if(p_stPCG->y > (SP_Y_OFFSET + y))
			{
				p_stPCG->y = SP_Y_OFFSET + y;
				if(i == SCORE_PCG_1)
				{
					bJump = FALSE;
					g_bSetScore = FALSE;
				}
			}
		}
	}
	
	return ret;
}
#endif	/* SCORE_C */

