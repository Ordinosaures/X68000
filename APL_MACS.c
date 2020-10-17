#ifndef	APL_MACS_C
#define	APL_MACS_C

#include <iocslib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/usr_macro.h"
#include "APL_MACS.h"

#include "Moon.h"
#include "FileManager.h"

#define	MACS_MAX	(16)

#ifdef 	MACS_MOON

/* �O���[�o���ϐ� */
static SC	mov_list[MACS_MAX][256]	=	{0};
static UI	mov_list_max	=	0u;

/* �\���̒�` */

#endif	/* MACS_MOON */


/* �֐��̃v���g�^�C�v�錾 */
void MOV_INIT(void);
SI MOV_Play(UC);
static UI moon_check(void);

/* �֐� */
void MOV_INIT(void)
{
#ifdef 	MACS_MOON
	US	i;
	SI	moon_chk = 0;

	/* MACS�Đ����邽�߂̃h���C�o�풓�`�F�b�N */
	if(moon_check() == 0)
	{
		puts("MACSDRV.X��MOON.X �풓��������");
		exit(0);
	}
	
	/* �O���t�B�b�N���X�g */
	Load_MACS_List("data\\mov\\mov_list.txt", mov_list, &mov_list_max);
	for(i = 0; i < mov_list_max; i++)
	{
		moon_chk = MoonRegst(mov_list[i]);	/* �������֓o�^ */
		if(moon_chk != 0)
		{
			printf("MoonRegst = %d\n", moon_chk);
		}
		printf("MACS File %2d = %s\n", i, mov_list[i] );
	}
#endif	/* MACS_MOON */
}

/* ����Đ� */
SI MOV_Play(UC bPlayNum)
{
	SI	ret=0;

#ifdef 	MACS_MOON
	SI	moon_stat = 0;
	
	if(bPlayNum >= mov_list_max)return ret;

#if 1	/* MOON���g���ꍇ */

	moon_stat = MoonPlay(mov_list[bPlayNum]);	/* �Đ� */
	if(moon_stat != 0)
	{
		printf("MoonPlay  = %d\n", moon_stat);
	}
//	MACS_Sleep();	/* �X���[�v */

#else	/* MACSDRV�P�Ƃ̏ꍇ */
	
	SC	*pBuff;
	SI	FileSize = 0;
	GetFileLength(mov_list[bPlayNum], &FileSize);	/* �t�@�C���̃T�C�Y�擾 */
	printf("MacsData = %d\n", FileSize);
	pBuff = (SC*)malloc(FileSize);		/* �������m�� */
	File_To_Mem(mov_list[bPlayNum], pBuff, FileSize );	/* �t�@�C���ǂݍ��݂��烁�����֕ۑ� */
	MACS_Play(pBuff);	/* �Đ� */
	free(pBuff);
#endif

#endif	/* MACS_MOON */

	return	ret;
}

static UI moon_check(void)
{
	UI	ret = 0;
#ifdef 	MACS_MOON
	UL *v = (UL *)( B_LPEEK((UL *)(0x1d1*4)) + 2 );
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

