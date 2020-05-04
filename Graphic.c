#ifndef	GRAPHIC_C
#define	GRAPHIC_C

#include <iocslib.h>

#include "inc/usr_macro.h"
#include "Graphic.h"

#define	CONV_PAL	(0xB7)
#define	TRANS_PAL	(0x00)

/* �֐��̃v���g�^�C�v�錾 */
void G_INIT(void);
void G_MyCar(void);
void G_Background(void);
void G_Palette(void);
SS G_Stretch_Pict(US , US, US, US, UC, US, US, US, US, UC);

/* �֐� */
void G_INIT(void)
{
	UI	nPalette;
	volatile US *VIDEO_REG1 = (US *)0xE82400;
	volatile US *VIDEO_REG2 = (US *)0xE82500;
	volatile US *VIDEO_REG3 = (US *)0xE82600;
	volatile US *V_Sync_end = (US *)0xE8000E;

	CRTMOD(11);				/* �����F�W���𑜓x�A��F�W�� */
	G_CLR_ON();				/* �O���t�B�b�N�̃N���A */
	VPAGE(0b1111);			/* pege(3:0n 2:0n 1:0n 0:0n) */
	WINDOW( X_MIN_DRAW, Y_MIN_DRAW, X_MAX_DRAW, Y_MAX_DRAW);
	HOME(0, X_OFFSET, Y_OFFSET);
	HOME(1, X_OFFSET, Y_OFFSET);
//	HOME(2, X_OFFSET, 416);
//	HOME(3, X_OFFSET, 416);
	WIPE();
//											   210
	*VIDEO_REG1 = Mbset(*VIDEO_REG1,   0x07, 0b001);	/* 512x512 256color 2men */
//											   |||
//											   |++		00.    16 color 4 screen
//											   |++		01.   256 color 2 screen
//											   |++		11. 65535 color 1 screen
//											   +0		VR.  512 x 512
//											   +1		VR. 1024 x1024

//											   DCBA9876543210
	*VIDEO_REG2 = Mbset(*VIDEO_REG2, 0x3FFF, 0b10000111100100);	/* �D�揇�� TX>GR>SP GR0>GR1>GR2>GR3 */
//											   |||||||||||||+0
//											   |||||||||||||+1
//											   ||||||||||||+0
//											   ||||||||||||+1
//											   |||||||||||+0
//											   |||||||||||+1
//											   ||||||||||+0
//											   ||||||||||+1
//											   |||||||||+0
//											   |||||||||+1
//											   ||||||||+0
//											   ||||||||+1
//											   |||||||+0
//											   |||||||+1
//											   ||||||+0
//											   ||||||+1
//											   |||||+0
//											   |||||+1
//											   ||||+0
//											   ||||+1
//											   |||+0
//											   |||+1
//											   ||+0
//											   ||+1
//											   |+0
//											   |+1
//											   +0
//											   +1

//											   FEDCBA9876543210
	*VIDEO_REG3 = Mbset(*VIDEO_REG3,   0x3F, 0b0000000001101111);	/* ���ꃂ�[�h�Ȃ� ���z���512x512 */
//											   |||||||||||||||+0	512x512 Pri0 OFF
//											   |||||||||||||||+1	512x512 Pri0 ON
//											   ||||||||||||||+0		512x512 Pri1 OFF
//											   ||||||||||||||+1		512x512 Pri1 ON
//											   |||||||||||||+0		512x512 Pri2 OFF
//											   |||||||||||||+1		512x512 Pri2 ON
//											   ||||||||||||+0		512x512 Pri3 OFF
//											   ||||||||||||+1		512x512 Pri3 ON
//											   |||||||||||+0		1024x1024 OFF
//											   |||||||||||+1		1024x1024 ON
//											   ||||||||||+0			TEXT OFF
//											   ||||||||||+1			TEXT ON
//											   |||||||||+0			SP OFF
//											   |||||||||+1			SP ON
//											   ||||||||+0	--------
//											   ||||||||+1	--------
//											   |||||||+0	
//											   |||||||+1
//											   ||||||+0
//											   ||||||+1
//											   |||||+0
//											   |||||+1
//											   ||||+0
//											   ||||+1
//											   |||+0
//											   |||+1
//											   ||+0
//											   ||+1
//											   |+0
//											   |+1
//											   +0
//											   +1
	*V_Sync_end = V_SYNC_MAX;	/* �c�̕\���͈͂����߂�(��ʉ��̃S�~�h�~) */

	/* �O���t�B�b�N�p���b�g�̏����� */
	for(nPalette=0; nPalette < 0xFF; nPalette++)
	{
		GPALET( nPalette, SetRGB(0, 0, 0));	/* Black */
	}
}

void G_MyCar(void)
{
#if 1	/* �摜�������������Ă��炢��Ȃ����� */
	SS x,y;

	APAGE(0);		/* �O���t�B�b�N�̏������� */

	G_Palette();	/* �O���t�B�b�N�p���b�g�̐ݒ� */	/* ���ߐF */
	
	/* �摜�ϊ� */
	for(y=Y_OFFSET-32-1; y<(Y_OFFSET-32)+MY_CAR_1_H+1; y++)
	{
		for(x=X_OFFSET-1; x<X_OFFSET+MY_CAR_1_W+1; x++)
		{
			US color;
			
			Draw_Pget(x, y, &color);

			switch(color)
			{
				case TRANS_PAL:
				{
					color = 0x01;	/* ���ߐF���s���ߐF */
					break;
				}
				case CONV_PAL:
				{
					color = TRANS_PAL;	/* �ϊ��ΏېF�����ߐF */
					break;
				}
				default:
				{
					/* �������Ȃ�*/
					break;
				}
			}
			Draw_Pset(x, y, color);
		}
	}

	for(y= 165; y<(165 + MY_CAR_0_H); y++)
	{
		for(x=(X_OFFSET + 96); x<(X_OFFSET + 96 + MY_CAR_0_W); x++)
		{
			US color;
			
			Draw_Pget(x, y, &color);

			switch(color)
			{
				case TRANS_PAL:
				{
					color = 0x01;	/* ���ߐF���s���ߐF */
					break;
				}
				case CONV_PAL:
				{
					color = TRANS_PAL;	/* �ϊ��ΏېF�����ߐF */
					break;
				}
				default:
				{
					/* �������Ȃ�*/
					break;
				}
			}
			Draw_Pset(x, y, color);
		}
	}
#else
#endif
}

void G_Background(void)
{
	SS i;
#if 1	/* �摜�������������Ă��炢��Ȃ����� */
	SS x,y;

	APAGE(1);		/* �O���t�B�b�N�̏������� */

	G_Palette();	/* �O���t�B�b�N�p���b�g�̐ݒ� */	/* ���ߐF */
	
	/* �摜�ϊ�(���C�o����) */
	for(y=0; y<ENEMY_CAR_1_H; y++)
	{
		for(x=0; x<ENEMY_CAR_1_W; x++)
		{
			US color;
			
			Draw_Pget(x, y, &color);

			switch(color)
			{
				case TRANS_PAL:
				{
					color = 0x01;	/* ���ߐF���s���ߐF */
					break;
				}
				case CONV_PAL:
				{
					color = TRANS_PAL;	/* �ϊ��ΏېF�����ߐF */
					break;
				}
				default:
				{
					/* �������Ȃ�*/
					break;
				}
			}
			Draw_Pset(x, y, color);
		}
	}
	/* �p�^�[���� */
	for(i=0; i<4; i++)
	{
		G_Stretch_Pict(
						0,					ENEMY_CAR_1_W>>i,
						ENEMY_CAR_1_H * i,	ENEMY_CAR_1_H>>i,
						1,
						0,	ENEMY_CAR_1_W,
						0,	ENEMY_CAR_1_H,
						1);
	}

	/* �摜�ϊ�(�w�i) */
	for(y=Y_OFFSET + 4; y<(Y_OFFSET + 4) + BG_1_H; y++)
	{
		for(x=X_OFFSET-32; x<X_OFFSET - 32 + BG_1_W; x++)
		{
			US color;
			
			Draw_Pget(x, y, &color);

			switch(color)
			{
				case TRANS_PAL:
				{
					color = 0x01;	/* ���ߐF���s���ߐF */
					break;
				}
				case CONV_PAL:
				{
					color = TRANS_PAL;	/* �ϊ��ΏېF�����ߐF */
					break;
				}
				default:
				{
					/* �������Ȃ�*/
					break;
				}
			}
			Draw_Pset(x, y, color);
		}
	}

#else
	SS e;
	
	APAGE(2);				/* �O���t�B�b�N�̏������� */

	/* �����ƃR�[�X�̊� */
	Draw_Fill( X_MIN_DRAW,  Y_HORIZON + Y_OFFSET, X_MAX_DRAW,  Y_HORIZON + Y_OFFSET, 1);

	/* ���� */
	for(e = 0; e < 8; e++)
	{
		Draw_Fill((e << 6), Y_HORIZON-16-1 + Y_OFFSET, (e << 6) + 10, Y_HORIZON-1 + Y_OFFSET, 0xFF);
	}
#endif
}

void G_Palette(void)
{
	GPALET( 0, SetRGB( 0,  0,  0));	/* Black */
#if 0
	GPALET( 0, SetRGB( 0,  0,  0));	/* Black */
	GPALET( 1, SetRGB(16, 16, 16));	/* Glay1 */
	GPALET( 2, SetRGB(15, 15, 15));	/* D-Glay */
	GPALET( 3, SetRGB( 0,  0, 31));	/* Blue */
	GPALET( 5, SetRGB(31,  0,  0));	/* Red */
	GPALET( 8, SetRGB( 0, 31,  0));	/* Green */
	GPALET( 9, SetRGB( 0, 30,  0));	/* Green */
	GPALET(10, SetRGB( 0, 29,  0));	/* Green */
	GPALET(11, SetRGB( 0, 28,  0));	/* Green */
	GPALET(12, SetRGB( 0, 27,  0));	/* Green */
	GPALET(13, SetRGB( 0, 26,  0));	/* Green */
	GPALET(14, SetRGB(16, 16, 16));	/* Glay2 */
	GPALET(15, SetRGB(31, 31, 31));	/* White */
#endif

#if 0
	Draw_Line(  0, 0, 255, 255, 15, 0xFFFF);	
	Draw_Line(255, 0,   0, 255, 15, 0xFFFF);	
	Draw_Box(   0, 0, 255, 255, 15, 0xFFFF);	
	Draw_Box( 127, 0, 255, 255, 15, 0xFFFF);	
	Draw_Fill(  0, 0, 127, 127, 3);	
	Draw_Fill(255, 0, 255, 127, 5);	
	Draw_Fill(  0, 128, 255, 255, 9);	
	Draw_Fill(255, 128, 255, 255, 15);	
	Draw_Circle(768/2,512/2,512/2,15,0,360,256);
#endif

}

SS G_Stretch_Pict(	US dst_x, US dst_w, US dst_y, US dst_h, UC ubDstScrn,
					US src_x, US src_w, US src_y, US src_h, UC ubSrcScrn)
{
	US	*pDstGR,	*pSrcGR;
	UI	DstGR_H,	SrcGR_H;
	US	dst_ex,	dst_ey;
	US	src_ex,	src_ey;
	US	x, y;
	US	rate_x, rate_y;

	dst_ex	= dst_x + dst_w;
	dst_ey	= dst_y + dst_h;
	src_ex	= src_x + src_w;
	src_ey	= src_y + src_h;

	/* �{�����Z */
	rate_x = src_w / dst_w;
	rate_y = src_h / dst_h;

	/* �A�h���X�Z�o */
	if(ubDstScrn != 0)
	{
		DstGR_H = 0xC80000;	/* Screen1 */
	}
	else{
		DstGR_H = 0xC00000;	/* Screen0 */
	}

	if(ubSrcScrn != 0)
	{
		SrcGR_H = 0xC80000;	/* Screen1 */
	}
	else{
		SrcGR_H = 0xC00000;	/* Screen0 */
	}
#if 1
	for(y = dst_y; y < dst_ey; y++)
	{
		/* �A�h���X�Z�o */
		pDstGR = (UC *)(DstGR_H + ((y << 10) + (dst_x << 1)));

		pSrcGR = (UC *)(SrcGR_H + ((((src_y + (y - dst_y)) * rate_y) << 10) + (src_x << 1)));
	
		for(x = dst_x; x < dst_ex; x++)
		{
			*pDstGR = *pSrcGR;
			
			pDstGR++;
			pSrcGR+=rate_x;
		}
	}
#else

#endif
}
#endif	/* GRAPHIC_C */
