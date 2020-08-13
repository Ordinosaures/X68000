#ifndef	GRAPHIC_C
#define	GRAPHIC_C

#include <iocslib.h>
#include <string.h>

#include "inc/usr_macro.h"
#include "Graphic.h"
#include "Draw.h"

#define	CONV_PAL	(0xB4)
#define	TRANS_PAL	(0x00)

/* �O���[�o���\���� */
ST_CRT	stCRT[CRT_MAX] = {0};

/* �֐��̃v���g�^�C�v�錾 */
SS	GetCRT(ST_CRT *, SS);
SS	SetCRT(ST_CRT, SS);
void G_INIT(void);
void G_MyCar(void);
void G_Background(void);
void G_Palette(void);
SS	G_Stretch_Pict( SS , US , SS , US , UC , SS , US, SS, US, UC );
SS G_BitBlt(SS , US , SS , US , UC , SS , US , SS , US , UC , UC , UC , UC );
SS G_CLR_AREA(SS, US, SS, US, UC);
SS G_CLR_ALL_OFFSC(UC);

/* �֐� */
SS	GetCRT(ST_CRT *stDat, SS Num)
{
	SS	ret = 0;
	
	if(Num < CRT_MAX)
	{
		*stDat = stCRT[Num];
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}

SS	SetCRT(ST_CRT stDat, SS Num)
{
	SS	ret = 0;
	
	if(Num < CRT_MAX)
	{
		stCRT[Num] = stDat;
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}

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

	/* CRT�̐ݒ� */
	stCRT[0].view_offset_x	= X_OFFSET;
	stCRT[0].view_offset_y	= Y_MIN_DRAW;
	stCRT[0].hide_offset_x	= X_OFFSET;
	stCRT[0].hide_offset_y	= Y_OFFSET;
	stCRT[0].BG_offset_x	= 0;
	stCRT[0].BG_offset_y	= 0;
	stCRT[0].BG_under		= BG_0_UNDER;
	
	stCRT[1].view_offset_x	= X_OFFSET;
	stCRT[1].view_offset_y	= Y_OFFSET;
	stCRT[1].hide_offset_x	= X_OFFSET;
	stCRT[1].hide_offset_y	= Y_MIN_DRAW;
	stCRT[1].BG_offset_x	= 0;
	stCRT[1].BG_offset_y	= 32;
	stCRT[1].BG_under		= BG_1_UNDER;

	stCRT[2].view_offset_x	= X_OFFSET;
	stCRT[2].view_offset_y	= Y_MIN_DRAW;
	stCRT[2].hide_offset_x	= X_OFFSET;
	stCRT[2].hide_offset_y	= Y_OFFSET;
	stCRT[2].BG_offset_x	= 0;
	stCRT[2].BG_offset_y	= 32;
	stCRT[2].BG_under		= BG_1_UNDER;
}

void G_MyCar(void)
{
	SS x,y;
//	SS x_offset, y_offset;

	APAGE(0);		/* �O���t�B�b�N�̏������� */

	G_Palette();	/* �O���t�B�b�N�p���b�g�̐ݒ� */	/* ���ߐF */

	/* �摜�������������Ă��炢��Ȃ����� */
	
	/* �摜�ϊ� */
//	for(y=Y_OFFSET; y<=(Y_OFFSET)+MY_CAR_1_H; y++)
	for(y=0; y<=Y_MAX_DRAW; y++)
	{
		for(x=X_OFFSET; x<=X_OFFSET+MY_CAR_1_W; x++)
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
	Draw_Fill(X_OFFSET + 90, 			180 + 1,	X_OFFSET + 90 + 31, 			180 + 31 - 1,	 TRANS_PAL);	/* ���������� */
	Draw_Fill(X_OFFSET + 90, Y_OFFSET + 180 + 1,	X_OFFSET + 90 + 31, Y_OFFSET +	180 + 31 - 1,	 TRANS_PAL);	/* ���������� */
#if 0	/* TPS */
	x_offset = X_OFFSET + ((WIDTH>>1) - (MY_CAR_0_W>>1));
	y_offset = V_SYNC_MAX - RASTER_MIN - MY_CAR_0_H - 16;
	
	for(y = y_offset; y < (y_offset + MY_CAR_0_H); y++)
	{
		for(x = x_offset; x < x_offset + MY_CAR_0_W; x++)
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
	Draw_Fill(X_OFFSET, (y_offset + MY_CAR_0_H), X_OFFSET + WIDTH, V_SYNC_MAX, 0xFF);
#else
#endif
}

void G_Background(void)
{
	SS i;
#if 1	/* �摜�������������Ă��炢��Ȃ����� */
	SS x,y;
	US height_sum = 0u;

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
	height_sum = 0u;
	for(i=1; i<4; i++)
	{
		height_sum += (ENEMY_CAR_1_H >> (i-1));
		
		G_Stretch_Pict(
						0,				ENEMY_CAR_1_W>>i,
						height_sum,		ENEMY_CAR_1_H>>i,
						1,
						0,	ENEMY_CAR_1_W,
						0,	ENEMY_CAR_1_H,
						1);
	}

	/* �摜�ϊ�(���V�̖�) */
	for(y=0; y<PINETREE_1_H; y++)
	{
		for(x=140; x<=140+PINETREE_1_W; x++)
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
	height_sum = 0u;
	for(i=1; i<4; i++)
	{
		height_sum += (PINETREE_1_H >> (i-1));

		G_Stretch_Pict(
						140,		140+PINETREE_1_W>>i,
						height_sum,		PINETREE_1_H>>i,
						1,
						140,	140+PINETREE_1_W,
						0,		PINETREE_1_H,
						1);
	}

#if 0
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
#endif

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

SS G_Stretch_Pict(	SS dst_x, US dst_w, SS dst_y, US dst_h, UC ubDstScrn,
					SS src_x, US src_w, SS src_y, US src_h, UC ubSrcScrn)
{
	SS	ret = 0;
	US	*pDstGR,	*pSrcGR;
	UI	DstGR_H,	SrcGR_H;
	SS	dst_ex,	dst_ey;
	SS	src_ex,	src_ey;
	SS	x, y;
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
		pDstGR = (US *)(DstGR_H + ((y << 10) + (dst_x << 1)));

		pSrcGR = (US *)(SrcGR_H + ((((src_y + (y - dst_y)) * rate_y) << 10) + (src_x << 1)));
	
		for(x = dst_x; x < dst_ex; x++)
		{
			*pDstGR = *pSrcGR;
			
			pDstGR++;
			pSrcGR+=rate_x;
		}
	}
#else

#endif
	return	ret;
}

/* �摜�̃R�s�[ */
/* �����T�C�Y�̉摜��C�ӂ̈ʒu�ɕ`�悷�� */
/* �\�����x,y���W���摜�̒��S�Ƃ����ʒu */
SS G_BitBlt(SS dst_x, US dst_w, SS dst_y, US dst_h, UC ubDstScrn,
			SS src_x, US src_w, SS src_y, US src_h, UC ubSrcScrn,
			UC ubMode, UC ubV, UC ubH)
{
	SS	ret = 0;
	US	*pDstGR,	*pSrcGR;
	UI	DstGR_H,	SrcGR_H;
	SS	dst_ex,	dst_ey;
	SS	src_ex,	src_ey;
	SS	x, y;
	SS	x_min, y_min;
	SS	x_max, y_max;
//	US	rate_x, rate_y;

	switch(ubV)
	{
		case POS_TOP:
		{
			break;
		}
		case POS_CENTER:
		default:
		{
			dst_y	= dst_y - (dst_h>>1);
			break;
		}
		case POS_BOTTOM:
		{
			dst_y	= dst_y - dst_h;
			break;
		}
	}

	switch(ubH)
	{
		case POS_LEFT:
		{
			break;
		}
		case POS_MID:
		default:
		{
			dst_x	= dst_x - (dst_w>>1);
			break;
		}
		case POS_RIGHT:
		{
			dst_x	= dst_x - dst_w;
			break;
		}
	}
	
	dst_ex	= dst_x + dst_w;
	dst_ey	= dst_y + dst_h;
	src_ex	= src_x + src_w;
	src_ey	= src_y + src_h;

	/* �{�����Z */
	if(src_w != dst_w)return -1;
	if(src_h != dst_h)return -1;
	
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
	
	/* �\���G���A���N���b�v */
	switch(ubMode)
	{
		case 0:
		{
			x_min = X_OFFSET;
			x_max = X_MAX_DRAW;
			y_min = Y_MIN_DRAW;
			y_max = V_SYNC_MAX;
			break;
		}
		case 1:
		default:
		{
			x_min = X_OFFSET;
			x_max = X_MAX_DRAW;
			y_min = Y_OFFSET;
			y_max = Y_OFFSET + V_SYNC_MAX;
			break;
		}
		case 2:
		{
			x_min = X_OFFSET;
			x_max = X_MAX_DRAW;
			y_min = Y_MIN_DRAW;
			y_max = Y_MIN_DRAW + V_SYNC_MAX;
			break;
		}
	}

	if(dst_ex < x_min)		/* ��ʊO */
	{
		return -1;
	}
	else if((dst_x < x_min) && (dst_ex >= x_min))
	{
		src_x += x_min - dst_x;
		dst_x = x_min;
	}
	else if((dst_x >= x_min) && (dst_ex > x_max))
	{
		src_ex -= dst_ex - X_MAX_DRAW;
		dst_ex = X_MAX_DRAW;
	}
	else if(dst_x > x_max)	/* ��ʊO */
	{
		return -1;
	}
	else						/* �͈͓� */
	{
		/* �������Ȃ� */
	}
	
	if(dst_ey < y_min)		/* ��ʊO */
	{
		return -1;
	}
	else if((dst_y < y_min) && (dst_ey >= y_min))
	{
		src_y += y_min - dst_y;
		dst_y = y_min;
	}
	else if((dst_y < y_max) && (dst_ey > y_max))
	{
		src_ey -= dst_ey - y_max;
		dst_ey = y_max;
	}
	else if(dst_y > y_max)	/* ��ʊO */
	{
		return -1;
	}
	else						/* �͈͓� */
	{
		/* �������Ȃ� */
	}

	for(y = dst_y; y < dst_ey; y++)
	{
		/* �A�h���X�Z�o */
		pDstGR = (US *)(DstGR_H + ((y << 10) + (dst_x << 1)));

		pSrcGR = (US *)(SrcGR_H + (((src_y + (y - dst_y)) << 10) + (src_x << 1)));
	
		for(x = dst_x; x < dst_ex; x++)
		{
			if(*pSrcGR != 0)
			{
				*pDstGR = *pSrcGR;
			}
			pDstGR++;
			pSrcGR++;
		}
	}
	return	ret;
}

/* ��ʂ̃N���A */
SS G_CLR_AREA(SS x, US w, SS y, US h, UC Screen)
{
	SS	ret = 0;
	SS	i=0;
	UL	ulGR_H;
	UL	ulPoint;
	UI	unSize;
#if 0
	US	data[512] = {0};
	SI	nMode;
	UC *DMA_DCR;
	UC *DMA_GCR;
	
	struct _chain stSrcImage;
	struct _chain stDstImage[256];

	if(DMAMODE() != 0u)return -1;
#endif
#if 0
	volatile US *CRTC_R21 = (US *)0xE8002Au;	/* �e�L�X�g�E�A�N�Z�X�E�Z�b�g�A�N���A�[P.S */
	volatile US *CRTC_480 = (US *)0xE80480u;	/* CRTC����|�[�g */
#endif

	switch(Screen)
	{
		case 0:
		{
			ulGR_H = 0xC00000;	/* Screen0 */
			break;
		}
		case 1:
		default:
		{
			ulGR_H = 0xC80000;	/* Screen1 */
			break;
		}
	}
	
	unSize = (w << 1);
	ulPoint = (y << 10u) + (x << 1);
#if 0
	nMode = 0b10000100;
	/*		  ||||||++------	(bit1,0)	DAC(addr2)	00:none	01:inc	10:dec	*/
	/*		  ||||++--------	(bit3,2)	MAC(addr1)	00:none	01:inc	10:dec	*/
	/*		  |+++----------	(bit6,5,4)	(Reserved)	0				*/
	/*		  +-------------	(bit7)		�]������	0:addr1->addr2	1:addr2->addr1	*/

	stSrcImage.addr = &data[0];
	stSrcImage.len  = 512;
	for(i=0; i<h; i++)
	{
		stDstImage[i].addr =  (void *)(ulGR_H + ulPoint + (i << 10u));
		stDstImage[i].len  =  unSize;
	}
	
	DMA_DCR = (UC *)0xE84084u;
	*DMA_DCR = Mbset(*DMA_DCR, ((UC)0b11000000), ((UC)0b10000000));	/* XRM(bit7,6) 10: */
	DMA_GCR = (UC *)0xE840FFu;
	*DMA_GCR = Mbset(*DMA_GCR, ((UC)0b0011), ((UC)0b0011));/* BR(bit1,0) 11:6.25% */
	
	DMAMOV_A(&stDstImage[0],	/* addr1 */
			&data[0],			/* addr2 */
			nMode,				/* mode */
//		Mmax((SI)h/16, 1));
			(SI)h);				/* len */
#endif

#if 0
	for(i=0; i<h; i++)
	{
//		DMAMOVE(&data[0],									/* addr1 */
//				(void *)(ulGR_H + ulPoint + (i * 0x400u)),	/* addr2 */
//				nMode,										/* mode */
//				(SI)unSize);								/* len */
	}
#endif

#if 1
	for(i=0; i<h; i++)
	{
		memset((void *)(ulGR_H + ulPoint + (i << 10u)), 0x00u, unSize);
	}
#endif

#if 0
	/* �K���e�L�X�g�\�������̌�ɍs������ */
	/* �N���A�ɂ����鎞�Ԃ�1/55fps�Ƃ̂���@Tea_is_Apple������ */
	if((*CRTC_480 & 0x02u) == 0u)	/* �N���A���s���łȂ� */
	{
		*CRTC_R21 = Mbset(*CRTC_R21, 0x0Fu, 0x0Cu);	/* SCREEN1 �����N���AON / SCREEN0 �����N���AOFF */
		*CRTC_480 = Mbset(*CRTC_480, 0x02u, 0x02u);	/* �N���A���s */
	}
#endif
	return	ret;
}

SS G_CLR_ALL_OFFSC(UC bMode)
{
	SS	ret = 0;
	
	/* �`��\�g�Đݒ� */
	WINDOW( stCRT[bMode].hide_offset_x, 
			stCRT[bMode].hide_offset_y,
			stCRT[bMode].hide_offset_x + WIDTH,
			stCRT[bMode].hide_offset_y + 152);	
	/* ���� */
	G_CLR_AREA(stCRT[bMode].hide_offset_x, WIDTH, stCRT[bMode].hide_offset_y, 152, 1);	/* Screen1 ���� */

	return	ret;
}

#endif	/* GRAPHIC_C */
