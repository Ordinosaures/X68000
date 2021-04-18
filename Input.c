#ifndef	INPUT_C
#define	INPUT_C

#include <doslib.h>
#include <iocslib.h>

#include "inc/usr_macro.h"
#include "Input.h"
#include "Music.h"


/* 関数のプロトタイプ宣言 */
uint16_t	get_key(uint16_t *, uint8_t, uint8_t );
uint16_t	DirectInputKeyNum(uint16_t *, uint16_t );
uint8_t	ChatCancelSW(uint8_t , uint8_t *);
int16_t	KeyHitESC(void);

/* 関数 */
uint16_t get_key( uint16_t *key, uint8_t bPlayer, uint8_t mode )
{
	uint32_t kd_k1,kd_k2_1,kd_k2_2,kd_b,kd_b2,kd_b3,kd_b4,kd_b5,kd_b6,kd_b7;
	static int16_t repeat_flag_a = KEY_TRUE;
	static int16_t repeat_flag_b = KEY_TRUE;
	uint16_t ret = 0;
	
	kd_b4	= BITSNS( 0 );
	kd_b3	= BITSNS( 2 );
	kd_b5	= BITSNS( 3 );
	kd_b6	= BITSNS( 4 );
	kd_b2	= BITSNS( 5 );
	kd_b7	= BITSNS( 6 );
	kd_k1	= BITSNS( 7 );
	kd_k2_1	= BITSNS( 8 );
	kd_k2_2	= BITSNS( 9 );
	kd_b	= BITSNS(10 );
	
	if( (kd_b3  & 0x02 ) != 0 ) *key |= KEY_b_Q;		/* Ｑ */
	if( (kd_b4  & 0x02 ) != 0 ) *key |= KEY_b_ESC;	/* ＥＳＣ */
	if( (kd_b7  & 0x01 ) != 0 ) *key |= KEY_b_M;		/* Ｍ */
	if( (kd_b7  & 0x20 ) != 0 ) *key |= KEY_b_SP;	/* スペースキー */
	if( (kd_k1  & 0x01 ) != 0 ) *key |= KEY_b_RLUP;	/* ロールアップ */
	if( (kd_k1  & 0x02 ) != 0 ) *key |= KEY_b_RLDN;	/* ロールダウン */

	if( !( JOYGET( bPlayer ) & UP    ) || ( kd_k1 & 0x10 ) || ( kd_k2_1 & 0x10 ) || ( kd_b3 & 0x04 ) ) *key |= KEY_UPPER;	/* 上 ↑ 8 w */
	if( !( JOYGET( bPlayer ) & DOWN  ) || ( kd_k1 & 0x40 ) || ( kd_k2_2 & 0x10 ) || ( kd_b5 & 0x80 ) ) *key |= KEY_LOWER;	/* 下 ↓ 2 s */
	if( !( JOYGET( bPlayer ) & LEFT  ) || ( kd_k1 & 0x08 ) || ( kd_k2_1 & 0x80 ) || ( kd_b5 & 0x40 ) ) *key |= KEY_LEFT;	/* 左 ← 4 a */
	if( !( JOYGET( bPlayer ) & RIGHT ) || ( kd_k1 & 0x20 ) || ( kd_k2_2 & 0x02 ) || ( kd_b6 & 0x01 ) ) *key |= KEY_RIGHT;	/* 右 → 6 d */
	
	if( !( JOYGET( bPlayer ) & JOYA  ) || ( kd_b  & 0x20 ) || ( kd_b2   & 0x04 ) )	/* Ａボタン or XF1 or z */
	{
		if( repeat_flag_a || (mode != 0u))
		{
			*key |= KEY_A;
			repeat_flag_a = KEY_FALSE;
		}
	}
	else
	{
		repeat_flag_a = KEY_TRUE;
	}
	
	if( !( JOYGET( bPlayer ) & JOYB  ) || ( kd_b  & 0x40 ) || ( kd_b2   & 0x08 ) )	/* Ｂボタン or XF2 or x  */
	{
		if( repeat_flag_b || (mode != 0u))
		{
			*key |= KEY_B;
			repeat_flag_b = KEY_FALSE;
		}
	}
	else
	{
		repeat_flag_b = KEY_TRUE;
	}
	return ret;
}

uint16_t	DirectInputKeyNum(uint16_t *uVal, uint16_t uDigit)
{
	uint16_t ret = 0;
	static uint16_t uNum[10] = {0};
	static uint16_t uCount = 0;
	static uint8_t bKeyFlag1 = FALSE;
	static uint8_t bKeyFlag2 = FALSE;
	uint32_t i, kd_g0, kd_g1;
	
	if(uDigit > 10)return -1;
	
	kd_g0	= (BITSNS( 0 ) & 0xFCu);
	kd_g1	= (BITSNS( 1 ) & 0x0Fu);

	if(kd_g0 != 0)
	{
		if(bKeyFlag1 == FALSE)
		{
			bKeyFlag1 = TRUE;
			
			for(i = 0; i < 8; i++)
			{
				if( ((kd_g0 >> i) & 0x01) != 0u )
				{
					uNum[uCount] = i - 1;
					break;
				}
			}
			uCount++;
			switch(uCount)	/* SE */
			{
			case 1:
				ADPCM_Play(1);
				break;
			case 2:
				ADPCM_Play(2);
				break;
			case 3:
				ADPCM_Play(3);
				break;
			default:
				ADPCM_Play(1);
				break;
			}
		}
	}
	else
	{
		bKeyFlag1 = FALSE;
	}
	
	if(kd_g1 != 0)
	{
		if(bKeyFlag2 == FALSE)
		{
			bKeyFlag2 = TRUE;

			for(i = 0; i < 8; i++)
			{
				if( ((kd_g1 >> i) & 0x01) != 0u )
				{
					uNum[uCount] = (i + 7) % 10;
					break;
				}
			}
			uCount++;
			
			switch(uCount)	/* SE */
			{
			case 1:
				ADPCM_Play(1);
				break;
			case 2:
				ADPCM_Play(2);
				break;
			case 3:
				ADPCM_Play(3);
				break;
			default:
				ADPCM_Play(1);
				break;
			}
		}
	}
	else
	{
		bKeyFlag2 = FALSE;
	}

	if( ( (kd_g0 != 0) || (kd_g1 != 0) )
		&& (uCount == uDigit) )
	{
		uint16_t uCal = 0;
		uint16_t uD = 1;
		
		for(i = 0; i < uDigit; i++)
		{
			if(i == 0)
			{
				uCal += uNum[uDigit-1];
			}
			else
			{
				uCal += uNum[(uDigit-1) - i] * uD;
			}
			uD *= 10;
		}
		*uVal = uCal;
		uCount = 0;
		ret = 1;
	}
	
	return ret;
}

/* チャタリング防止SW */
uint8_t	ChatCancelSW(uint8_t bJudge, uint8_t *bFlag)
{
	uint8_t	ret = FALSE;
	
	if(bJudge != 0u)
	{
		if(*bFlag == TRUE)
		{
			ret = TRUE;
			*bFlag = FALSE;
		}
	}
	else
	{
		*bFlag = TRUE;
	}
	return ret;
}

#if 0
#ifdef DEBUG	/* デバッグコーナー */
		/* テスト用入力 */
		if(bDebugMode == TRUE)
		{
			/* キーボードから数字を入力 */
			DirectInputKeyNum(&g_uDebugNum, 3);
			/* キー操作 */
#if 1
			/* 角度変更 */
			//if(ChatCancelSW((input & KEY_UPPER)!=0u, &bKeyUP_flag) == TRUE)	vy += 1;	/* 上 */
			//if(ChatCancelSW((input & KEY_LOWER)!=0u, &bKeyDOWN_flag) == TRUE)	vy -= 1;	/* 下 */
			if((input & KEY_UPPER)!=0u)	vy += 1;	/* 上 */
			if((input & KEY_LOWER)!=0u)	vy -= 1;	/* 下 */
#else
			/* ランプ操作 */
			if(bUpDown_flag == 0){
				vy += 1;
				if(vy > 45)
				{
					vy = 0;
					bUpDown_flag = 1;
				}
			}
			else{
				vy -= 1;
				if(vy < -45)
				{
					vy = 0;
					bUpDown_flag = 0;
				}
			}
#endif
			vy = Mmax(Mmin(vy, 45), -45);
		}
#endif
#endif

int16_t	KeyHitESC(void)
{
	int16_t	ret = 0;
	static uint8_t bFlag = 0;
	
	{
		uint32_t	loop = 1;
		do
		{
			if( ( BITSNS( 0 ) & 0x02 ) != 0 )	/* ＥＳＣ */
			{
				bFlag = 1u;
			}
			else if(bFlag == 1u)
			{
				_dos_kflushio(0xFF);	/* キーバッファをクリア */
				bFlag = 0u;
				loop = 0;				/* ループ脱出 */
			}
			else
			{
				bFlag = 0u;
			}
			if(loop == 0)break;
		}
		while( loop );
	}
	
	return ret;
}
	
#endif	/* INPUT_C */

