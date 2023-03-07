#ifndef	SCORE_C
#define	SCORE_C

#include <stdio.h>

#include <usr_macro.h>
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
static 	uint16_t	g_uScoreNum = 0u;
static 	uint16_t	g_uPassCarNum = 0u;

/* �\���̒�` */

/* �֐��̃v���g�^�C�v�錾 */
int16_t S_Get_ScoreInfo(ST_SCORE *);
int16_t S_Set_ScoreInfo(ST_SCORE);
int16_t S_Add_Score(void);
int16_t S_Add_Score_Point(uint64_t);
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
	g_uPassCarNum++;
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
	//ADPCM_Play(18);	/* SE�FPON */
	
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
	
	val64 += ulNum;	/* ���Z */
	
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
	g_uPassCarNum = 0;
	
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
	g_uPassCarNum = 0;
	g_bSetScore = FALSE;

	x = 176;
	y = 128;
	
	for(i = 0; i < 4; i++)
	{
		p_stPCG = PCG_Get_Info(SCORE_PCG_1 + i);	/* �X�R�A */
		if(p_stPCG != NULL)
		{
			p_stPCG->Anime = 0;
			p_stPCG->x = SP_X_OFFSET + x - (Mdiv2(SP_W) * i);
			p_stPCG->y = SP_Y_OFFSET + y;
			p_stPCG->update	= FALSE;
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

	g_uPassCarNum = 0;
	
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

	ST_PCG	*p_stPCG = NULL;
	
	x = 176;
	y = 128;
	
	if(bJump == FALSE)
	{
		if(g_bSetScore == TRUE)
		{
			bJump = TRUE;
			
			for(i = 0; i < 4; i++)
			{
				p_stPCG = PCG_Get_Info(SCORE_PCG_1 + i);	/* �X�R�A */
				if(p_stPCG != NULL)
				{
					p_stPCG->dx = -18;
					p_stPCG->x = SP_X_OFFSET + x - (Mdiv2(SP_W) * i);
					p_stPCG->y = y;
					p_stPCG->update	= TRUE;
				}
			}
		}
	}
	
	if(bJump == TRUE)
	{
		for(i = 0; i < 4; i++)
		{
			p_stPCG = PCG_Get_Info(SCORE_PCG_1 + i);	/* �X�R�A */
			if(p_stPCG != NULL)
			{
				p_stPCG->dx += 3;
			}
		}
	}
	number = g_uPassCarNum;
	
	do{
		number = number / 10;
		digit++;
	}while(number!=0);
	
	number = g_uPassCarNum;
	
	for(i = 0; i < 4; i++)
	{
		p_stPCG = PCG_Get_Info(SCORE_PCG_1 + i);	/* �X�R�A */
		if(p_stPCG != NULL)
		{
			int16_t	pos;

			p_stPCG->Anime = number % 10;
			number /= 10;
			
			if(digit > i)	/* �\������ */
			{
				p_stPCG->x += p_stPCG->dx;
				p_stPCG->y = y;
			}
			else
			{
				p_stPCG->x = 0;
				p_stPCG->y = 0;
			}
			p_stPCG->update	= TRUE;
			
			pos = SP_X_OFFSET + x - (Mdiv2(SP_W) * i);	/* 臒l */

			if(p_stPCG->x > pos)
			{
				p_stPCG->x = pos;
				if(i == 0)
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

