#ifndef	INPUT_C
#define	INPUT_C

#include <stdio.h>
#include <doslib.h>
#include <iocslib.h>

#include <usr_macro.h>
#include "Input.h"
#include "Music.h"

int16_t		g_Input;
int16_t		g_AnalogMode = 0xFFFF;
uint8_t		g_bAnalogStickMode = FALSE;
uint8_t		g_bAnalogStickMode_flag;

JOY_ANALOG_BUF	g_Analog_Info;

/* �֐��̃v���g�^�C�v�錾 */
uint32_t	get_analog_data(uint32_t, JOY_ANALOG_BUF *);
uint16_t	get_keyboard( uint16_t *, uint8_t , uint8_t );
uint16_t	get_djoy(uint16_t *, uint8_t, uint8_t );
uint16_t	get_ajoy(uint16_t *, uint8_t, uint8_t, uint8_t );
int16_t		GetAnalog_Info(JOY_ANALOG_BUF *);
int16_t		SetAnalog_Info(JOY_ANALOG_BUF);
uint16_t	DirectInputKeyNum(uint16_t *, uint16_t );
uint8_t	ChatCancelSW(uint8_t , uint8_t *);
int16_t	KeyHitESC(void);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
uint16_t get_keyboard( uint16_t *key, uint8_t bPlayer, uint8_t mode )
{
	uint16_t ret = 0;
	uint32_t uKeyBoard[16] = {0u};
	static int16_t repeat_flag_a = KEY_TRUE;
	static int16_t repeat_flag_b = KEY_TRUE;
	
	uKeyBoard[0]	= BITSNS( 0 );
	uKeyBoard[2]	= BITSNS( 2 );
	uKeyBoard[3]	= BITSNS( 3 );
	uKeyBoard[4]	= BITSNS( 4 );
	uKeyBoard[5]	= BITSNS( 5 );
	uKeyBoard[6]	= BITSNS( 6 );
	uKeyBoard[7]	= BITSNS( 7 );
	uKeyBoard[8]	= BITSNS( 8 );
	uKeyBoard[9]	= BITSNS( 9 );
	uKeyBoard[10]	= BITSNS(10 );
	
	if( (uKeyBoard[2]  & Bit_1 ) != 0 ) *key |= KEY_b_Q;	/* �p */
	if( (uKeyBoard[0]  & Bit_1 ) != 0 ) *key |= KEY_b_ESC;	/* �d�r�b */
	if( (uKeyBoard[6]  & Bit_0 ) != 0 ) *key |= KEY_b_M;	/* �l */
	if( (uKeyBoard[6]  & Bit_5 ) != 0 ) *key |= KEY_b_SP;	/* �X�y�[�X�L�[ */
	if( (uKeyBoard[7]  & Bit_0 ) != 0 ) *key |= KEY_b_RLUP;	/* ���[���A�b�v */
	if( (uKeyBoard[7]  & Bit_1 ) != 0 ) *key |= KEY_b_RLDN;	/* ���[���_�E�� */
	if( (uKeyBoard[10] & Bit_4 ) != 0 ) *key |= KEY_b_HELP;	/* HELP */
	if( (uKeyBoard[2]  & Bit_0 ) != 0 ) *key |= KEY_b_TAB;	/* TAB */
	if( (uKeyBoard[4]  & Bit_2 ) != 0 ) *key |= KEY_b_G;	/* �f */
	
	if( ( uKeyBoard[7] & Bit_4 ) || ( uKeyBoard[8] & Bit_4 ) || ( uKeyBoard[2] & Bit_2 ) ) *key |= KEY_UPPER;	/* �� �� 8 w */
	if( ( uKeyBoard[7] & Bit_6 ) || ( uKeyBoard[9] & Bit_4 ) || ( uKeyBoard[3] & Bit_7 ) ) *key |= KEY_LOWER;	/* �� �� 2 s */
	if( ( uKeyBoard[7] & Bit_3 ) || ( uKeyBoard[8] & Bit_7 ) || ( uKeyBoard[3] & Bit_6 ) ) *key |= KEY_LEFT;		/* �� �� 4 a */
	if( ( uKeyBoard[7] & Bit_5 ) || ( uKeyBoard[9] & Bit_1 ) || ( uKeyBoard[4] & Bit_0 ) ) *key |= KEY_RIGHT;	/* �E �� 6 d */
	
	if( ( uKeyBoard[10]  & Bit_5 ) || ( uKeyBoard[5]   & Bit_2 ) )	/* �`�{�^�� or XF1 or z */
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
	
	if( ( uKeyBoard[10]  & 0x40 ) || ( uKeyBoard[5]   & 0x08 ) )	/* �a�{�^�� or XF2 or x  */
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
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
uint16_t get_djoy( uint16_t *key, uint8_t bPlayer, uint8_t mode )
{
	uint16_t ret = 0;
	uint32_t uJoyStick = 0u;
	static int16_t repeat_flag_a = KEY_TRUE;
	static int16_t repeat_flag_b = KEY_TRUE;
	
	uJoyStick 		= JOYGET(bPlayer);
	g_AnalogMode = 0xFFFF;	/* �A�i���O���[�h��OFF */
	
	if( !( uJoyStick & UP    ) ) *key |= KEY_UPPER;	/* �� */
	if( !( uJoyStick & DOWN  ) ) *key |= KEY_LOWER;	/* �� */
	if( !( uJoyStick & LEFT  ) ) *key |= KEY_LEFT;	/* �� */
	if( !( uJoyStick & RIGHT ) ) *key |= KEY_RIGHT;	/* �E */
	
	if( !( uJoyStick & JOYA  ) )	/* �`�{�^�� */
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
	
	if( !( uJoyStick & JOYB  )  )	/* �a�{�^�� */
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

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
uint16_t get_ajoy( uint16_t *key, uint8_t bPlayer, uint8_t mode, uint8_t ubConfig )
{
	uint16_t ret = 0;
	int32_t AnalogJoyStick = 0;
	static int16_t repeat_flag_a = KEY_TRUE;
	static int16_t repeat_flag_b = KEY_TRUE;

	JOY_ANALOG_BUF	analog_buf;
	
	AnalogJoyStick	= get_analog_data(bPlayer + 1, &analog_buf);
	if(AnalogJoyStick < 0)return -1;
	else
#ifdef DEBUG
	{
//		printf("JOYDRV3(%d) =0x%2x,0x%2x,0x%2x,0x%2x,0x%b\r", bPlayer + 1, 
//			analog_buf.l_stk_ud,
//			analog_buf.l_stk_lr,
//			analog_buf.r_stk_ud,
//			analog_buf.r_stk_lr,
//			analog_buf.btn_data);
//		KeyHitESC();	/* �f�o�b�O�p */
	}
#endif
	
	if( (analog_buf.btn_data  & AJOY_SELECT	 ) == 0 ) *key |= KEY_b_Q;		/* �p */
	if( (analog_buf.btn_data  & AJOY_START	 ) == 0 ) *key |= KEY_b_ESC;	/* �d�r�b */
	if( (analog_buf.btn_data  & AJOY_E2		 ) == 0 ) *key |= KEY_b_M;		/* �l */
	if( (analog_buf.btn_data  & AJOY_D		 ) == 0 ) *key |= KEY_b_SP;		/* �X�y�[�X�L�[ */
	if( (analog_buf.btn_data  & AJOY_E2		 ) == 0 ) *key |= KEY_b_RLUP;	/* ���[���A�b�v */
	if( (analog_buf.btn_data  & AJOY_E1		 ) == 0 ) *key |= KEY_b_RLDN;	/* ���[���_�E�� */
	
	if(ubConfig == 0u)
	{
		/* X680x0 */
	}
	else
	{
		/* Windows -> USB -> XM6 */
		JOY_ANALOG_BUF	analog_buf_tmp;
		analog_buf_tmp = analog_buf;
		
		/* L�X�e�B�b�N��R�X�e�B�b�N����ւ� */
		analog_buf.l_stk_ud = analog_buf_tmp.r_stk_ud;
		analog_buf.l_stk_lr = analog_buf_tmp.r_stk_lr;
		analog_buf.r_stk_ud = analog_buf_tmp.l_stk_ud;
		analog_buf.r_stk_lr = analog_buf_tmp.l_stk_lr;
	}
	if( analog_buf.l_stk_ud > 0x90 ) *key |= KEY_UPPER;	/* �� */
	if( analog_buf.l_stk_ud < 0x70 ) *key |= KEY_LOWER;	/* �� */
	if( analog_buf.r_stk_lr < 0x70 ) *key |= KEY_LEFT;	/* �� */
	if( analog_buf.r_stk_lr > 0x90 ) *key |= KEY_RIGHT;	/* �E */

	if( !( analog_buf.btn_data & AJOY_A  ))	/* �`�{�^�� */
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
	
	if( !( analog_buf.btn_data & AJOY_B  ))	/* �a�{�^�� */
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

	SetAnalog_Info(analog_buf);	/* �A�i���O���X�V */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	GetAnalog_Info(JOY_ANALOG_BUF *p_stAnalog_Info)
{
	int16_t	ret = 0;
	*p_stAnalog_Info = g_Analog_Info;
	ret = g_AnalogMode;
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetAnalog_Info(JOY_ANALOG_BUF stAnalog_Info)
{
	int16_t	ret = 0;
	g_Analog_Info = stAnalog_Info;
	g_AnalogMode = 0;
	return ret;
}


/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
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

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* �`���^�����O�h�~SW */
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
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		/* �e�X�g�p���� */
		if(bDebugMode == TRUE)
		{
			/* �L�[�{�[�h���琔������� */
			DirectInputKeyNum(&g_uDebugNum, 3);
			/* �L�[���� */
#if 1
			/* �p�x�ύX */
			//if(ChatCancelSW((input & KEY_UPPER)!=0u, &bKeyUP_flag) == TRUE)	vy += 1;	/* �� */
			//if(ChatCancelSW((input & KEY_LOWER)!=0u, &bKeyDOWN_flag) == TRUE)	vy -= 1;	/* �� */
			if((input & KEY_UPPER)!=0u)	vy += 1;	/* �� */
			if((input & KEY_LOWER)!=0u)	vy -= 1;	/* �� */
#else
			/* �����v���� */
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

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	KeyHitESC(void)
{
	int16_t	ret = 0;
	static uint8_t bFlag = 0;
	
	{
		uint32_t	loop = 1;
		do
		{
			if( ( BITSNS( 0 ) & 0x02 ) != 0 )	/* �d�r�b */
			{
				bFlag = 1u;
			}
			else if(bFlag == 1u)
			{
				_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */
				bFlag = 0u;
				loop = 0;				/* ���[�v�E�o */
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

