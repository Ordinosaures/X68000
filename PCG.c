#ifndef	PCG_C
#define	PCG_C

#include <iocslib.h>

#include "inc/usr_macro.h"
#include "PCG.h"

/* �֐��̃v���g�^�C�v�錾 */
void PCG_INIT(void);
void BG_TEXT_SET(SC *);

/* �֐� */
void PCG_INIT(void)
{
	UI i, j;
	
	/* �X�v���C�g�̏����� */
	SP_INIT();			/* �X�v���C�g�̏����� */
	SP_ON();			/* �X�v���C�g�\�����n�m */
	BGCTRLST(0, 1, 1);	/* �a�f�O�\���n�m */
	BGCTRLST(1, 1, 1);	/* �a�f�P�\���n�m */
//	BGCTRLST(1, 1, 0);	/* �a�f�P�\���n�e�e */
	
	/*�X�v���C�g���W�X�^������*/
	for( j = 0x80000000; j < 0x80000000 + 128; j++ )
	{
		SP_REGST(j,-1,0,0,0,0);
	}
#if 0
	for( i = 0; i < 256; i++ )
	{
		SP_CGCLR(i);			/* �X�v���C�g�N���A */
		SP_DEFCG( i, 1,  );
	}
#endif
	
	//	BGCTRLGT(1);				/* BG�R���g���[�����W�X�^�ǂݍ��� */
	BGSCRLST(0,0,0);				/* BG�X�N���[�����W�X�^�ݒ� */
	BGSCRLST(1,0,0);				/* BG�X�N���[�����W�X�^�ݒ� */
	BGTEXTCL(0,SetBGcode(0,0,0,0));	/* BG�e�L�X�g�N���A */
	BGTEXTCL(1,SetBGcode(0,0,0,0));	/* BG�e�L�X�g�N���A */
	//	BGTEXTGT(1,1,0);			/* BG�e�L�X�g�ǂݍ��� */
}

void BG_TEXT_SET(SC *fname)
{
	US	usV_pat;
	UI	pal = 0;
	US	i,j;
	SI	x,y;
	US	map_data[64][64];

	/* �}�b�v�f�[�^�ǂݍ��� */
	File_Load_CSV( fname, map_data, &i, &j);
	
	for(y=0; y<16; y++)
	{
		for(x=0; x<i; x++)
		{
			if(y < 12)		pal = 0x0E;
			else if(x > 31)	pal = 0x0F;
			else 			pal = 0x0E;
			BGTEXTST(0,x,y,SetBGcode(0,0,pal,map_data[y][x]));		/* BG�e�L�X�g�ݒ� */
			BGTEXTST(1,x,y,SetBGcode(0,0,pal,map_data[y][x]));		/* BG�e�L�X�g�ݒ� */
		}
	}

	usV_pat=0;
	pal = 1;
	for(y=16; y<j; y++)
	{
		for(x=0; x<i; x++)
		{
			if(x < 16 || x > 40){
				BGTEXTST(0,x,y,SetBGcode(0,0,pal,map_data[y][x]));		/* BG�e�L�X�g�ݒ� */
				BGTEXTST(1,x,y,SetBGcode(0,0,pal,map_data[y][x]));		/* BG�e�L�X�g�ݒ� */
			}
			else{	/* �������] */
				BGTEXTST(0,x,y,SetBGcode(0,1,pal,map_data[y][x]));		/* BG�e�L�X�g�ݒ� */
				BGTEXTST(1,x,y,SetBGcode(0,1,pal,map_data[y][x]));		/* BG�e�L�X�g�ݒ� */
			}
		}
		
		usV_pat++;
		if(usV_pat < 12){
		}
		else{
			pal++;
			usV_pat=0;
		}
	}
}

#endif	/* PCG_C */

