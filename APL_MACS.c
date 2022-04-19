#ifndef	APL_MACS_C
#define	APL_MACS_C

#include <iocslib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/usr_macro.h"
#include "APL_MACS.h"

#include "OverKata.h"
#include "CRTC.h"
#include "FileManager.h"
#include "Graphic.h"
#include "PCG.h"

#ifdef 	MACS_MOON

/* �O���[�o���ϐ� */
int8_t		g_mov_list[MACS_MAX][256]	=	{0};
uint32_t	g_mov_list_max	=	0u;
/* �\���̒�` */

#endif	/* MACS_MOON */

/* �֐��̃v���g�^�C�v�錾 */
void MOV_INIT(void);
int32_t MOV_Play(uint8_t);
static uint32_t moon_check(void);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void MOV_INIT(void)
{
#ifdef 	MACS_MOON
	uint16_t	i;
	int32_t	moon_chk = 0;

	/* MACS�Đ����邽�߂̃h���C�o�풓�`�F�b�N */
	if(moon_check() == 0)
	{
		puts("MACSDRV.X��MOON.X �풓��������");
		exit(0);
	}
	
	/* ���X�g����Moon.x�Ń������o�^ */
	for(i = 0; i < g_mov_list_max; i++)
	{
		moon_chk = MoonRegst(g_mov_list[i]);	/* �������֓o�^ */
		if(moon_chk < 0)
		{
			printf("MoonRegst = %d\n", moon_chk);
		}
	}
#endif	/* MACS_MOON */
}

/* ����Đ� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int32_t MOV_Play(uint8_t bPlayNum)
{
	int32_t	ret=0;

	uint16_t	uCRT_Tmg;

	if(bPlayNum >= g_mov_list_max)return ret;

#ifdef 	MACS_MOON
#if 1	/* MOON���g���ꍇ */
	{
		int32_t	moon_stat = 0;

		moon_stat = MoonPlay(g_mov_list[bPlayNum]);	/* �Đ� */
		if(moon_stat != 0)
		{
			//printf("MoonPlay  = %d\n", moon_stat);
			/* �A�{�[�g(-4)�����ꍇ�́A���������@���K�v�������悤�ȁc */
		}
		else
		{
		}
	//	MACS_Sleep();	/* �X���[�v */
	}
#else	/* MACSDRV�P�Ƃ̏ꍇ */
	{
		int8_t	*pBuff = NULL;
		int32_t	FileSize = 0;

		GetFileLength(g_mov_list[bPlayNum], &FileSize);	/* �t�@�C���̃T�C�Y�擾 */
		printf("MacsData = %d\n", FileSize);
		pBuff = (int8_t*)MyMalloc(FileSize);	/* �������m�� */
		if(pBuff != NULL)
		{
			File_Load(g_mov_list[bPlayNum], pBuff, sizeof(uint8_t), FileSize );	/* �t�@�C���ǂݍ��݂��烁�����֕ۑ� */
			MACS_Play(pBuff);	/* �Đ� */
			MyMfree(pBuff);	/* ��������� */
		}
	}
#endif
#endif	/* MACS_MOON */

	/* ��ʃ��[�h�Đݒ� */
	Get_CRT_Tmg(&uCRT_Tmg);
	CRTC_INIT((uint8_t)uCRT_Tmg);
	G_HOME(1);		/* �z�[���ʒu�Đݒ�(����̌��) */

	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static uint32_t moon_check(void)
{
	uint32_t	ret = 0;
#ifdef 	MACS_MOON
	uint64_t *v = (uint64_t *)( B_LPEEK((uint64_t *)(0x1d1*4)) + 2 );
	if( B_LPEEK( v ) == 'MOON' && B_LPEEK( v+1 ) == 'IOCS' )
	{
		ret = 1;
	}
#else	/* MACS_MOON */
	ret = 1;
#endif	/* MACS_MOON */

	return ret;
}

#endif	/* APL_MACS_C */

