#ifndef	APL_SCORE_H
#define	APL_SCORE_H

#include <usr_define.h>

/* 構造体定義 */
typedef struct{
	uint64_t	ulScoreAdd;
	uint64_t	ulScore;
	uint64_t	ulScoreMax;
	uint16_t	uCombo;
} ST_SCORE;

typedef struct{
	uint16_t	x;
	uint16_t	y;
	uint16_t	x_old;
	uint16_t	y_old;
} ST_SCORE_POS;

/* extern宣言 */
extern	int16_t S_Get_ScoreInfo(ST_SCORE *);
extern	int16_t S_Set_ScoreInfo(ST_SCORE);
extern	int16_t S_Add_Score(uint64_t);
extern	int16_t S_Add_Score_Point(uint64_t);
extern	int16_t S_Set_Combo(int16_t);
extern	int16_t S_All_Init_Score(void);
extern	int16_t S_Init_Score(void);
extern	int16_t S_GetPos(int16_t, int16_t);
extern	int16_t S_Clear_Score(void);
extern	int16_t S_Main_Score(void);

#endif	/* APL_SCORE_H */
