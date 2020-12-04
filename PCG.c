#ifndef	PCG_C
#define	PCG_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iocslib.h>
#include <doslib.h>

#include "inc/usr_macro.h"
#include "PCG.h"
#include "APL_Math.h"
#include "FileManager.h"
#include "Output_Text.h"

/* �֐��̃v���g�^�C�v�錾 */
void PCG_INIT(void);
void PCG_VIEW(UC);
void PCG_Rotation(US *, US *, UC, UC, SS, SS, UC *, UC, US, US);
void BG_TEXT_SET(SC *);

/* �֐� */
void PCG_INIT(void)
{
	UI	j;
	
	/* �X�v���C�g�̏����� */
	SP_INIT();			/* �X�v���C�g�̏����� */
	
	/*�X�v���C�g���W�X�^������*/
	for( j = 0x80000000; j < 0x80000000 + 128; j++ )
	{
		SP_REGST(j,-1,0,0,0,0);
	}
#if 0
	for(UI i = 0; i < 256; i++ )
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

	PCG_SP_dataload("data/sp/BG.SP");	/* �X�v���C�g�̃f�[�^�ǂݍ��� */
	PCG_PAL_dataload("data/sp/BG.PAL");	/* �X�v���C�g�̃p���b�g�f�[�^�ǂݍ��� */
	BG_TEXT_SET("data/map/map.csv");	/* �}�b�v�f�[�^�ɂ��a�f�̔z�u */
}

void PCG_VIEW(UC bSW)
{
	if(bSW == TRUE)
	{
		SP_ON();			/* �X�v���C�g�\�����n�m */
		BGCTRLST(0, 1, 1);	/* �a�f�O�\���n�m */
		BGCTRLST(1, 1, 1);	/* �a�f�P�\���n�m */
	}
	else
	{
		SP_OFF();			/* �X�v���C�g�\�����n�e�e */
		BGCTRLST(0, 1, 0);	/* �a�f�O�\���n�e�e */
		BGCTRLST(1, 1, 0);	/* �a�f�P�\���n�e�e */
	}
}

void PCG_Rotation(US *pDst, US *pSrc, UC bX_num, UC bY_num, SS pos_x, SS pos_y, UC *sp_num, UC palNum, US ratio, US rad)
{
	SS	x, y, vx, vy;
	SS	width, height;
	UL	uPCG;
	UL	uPCG_ARY[8];
	UL	*pDstWork, *pSrcWork;
	UC	*pBuf, *pDstBuf = NULL, *pSrcBuf = NULL;
	UL	uADDR;
	UL	uPointer_ADR, uPointer_ADR_X, uPointer_ADR_Y, uPointer_ADR_subX, uPointer_ADR_subY;
	US	uMem_size;
	UC	bShift;
	UL	code = 0;
	UC	V=0, H=0;
	UC	spNum;	
//	SC	bEx_num=0;
	
	/* src size */
	width =  16 * (SS)bX_num;
	height = 16 * (SS)bY_num;
	uMem_size = width * height * sizeof(UC);
	
	if((ratio > 0) && (ratio <= 16))
	{
		bShift = ratio;	/* def:2048(=8) */
	}
	else
	{
		bShift = 8;
	}
//	Message_Num(&bShift,   0, 9, 2, MONI_Type_UC, "%2d");
//	Message_Num(&bEx_num, 11, 9, 2, MONI_Type_SC, "%2d");
	
	/* ��ƃG���A�m�� */
	pSrcBuf = (UC*)MyMalloc( (SI)uMem_size );
	if( pSrcBuf == NULL )
	{
		return;
	}
	pBuf = pSrcBuf;

	/* PCG -> CG */
	uPointer_ADR = (UL)pSrc;
	uPointer_ADR_Y = 0ul;
	
	for(y = 0; y < bY_num; y++)	/* ��s�� */
	{
		uPointer_ADR_subY = 0ul;
		
		for(vy = 0; vy < 16; vy++)	/* 16bit��(8dot+8dot) */
		{
			uPointer_ADR_X = 0ul;
			
			for(x = 0; x < bX_num; x++)	/* ��� */
			{
				uPointer_ADR_subX = 0ul;
				
				for(vx = 0; vx < 2; vx++)	/* 16bit�� */
				{
					uADDR = uPointer_ADR + uPointer_ADR_Y + uPointer_ADR_subY + uPointer_ADR_X + uPointer_ADR_subX;	/* ���_ */
					pSrcWork = (UL *)uADDR;	/* 0��2/1��3 */

					uPCG = (UL)*pSrcWork;
					
					*pBuf = (UC)((uPCG >> 28ul) & 0x0Fu);
					pBuf++;
					*pBuf = (UC)((uPCG >> 24ul) & 0x0Fu);
					pBuf++;
					*pBuf = (UC)((uPCG >> 20ul) & 0x0Fu);
					pBuf++;
					*pBuf = (UC)((uPCG >> 16ul) & 0x0Fu);
					pBuf++;
					*pBuf = (UC)((uPCG >> 12ul) & 0x0Fu);
					pBuf++;
					*pBuf = (UC)((uPCG >>  8ul) & 0x0Fu);
					pBuf++;
					*pBuf = (UC)((uPCG >>  4ul) & 0x0Fu);
					pBuf++;
					*pBuf = (UC)((uPCG        ) & 0x0Fu);
					pBuf++;
					
					uPointer_ADR_subX += 0x40ul;
				}
				uPointer_ADR_X += 0x80ul;
			}
			uPointer_ADR_subY += 0x04ul;
		}
		uPointer_ADR_Y += 0x800ul;
	}

	/* �摜���� */
#if 0
	/* �X�ɗ̈���g������@�\���������悤�Ƃ������f�O */
	width =  16 * Mmax(((SS)bX_num + bEx_num), 1);
	height = 16 * Mmax(((SS)bY_num + bEx_num), 1);
	uMem_size = width * height * sizeof(UC);
#endif
	
	pDstBuf = (UC*)MyMalloc( (SI)uMem_size );
	if( pDstBuf == NULL )
	{
		MyMfree(pSrcBuf);	/* ��������� */
		return;
	}
	pBuf = pDstBuf;
	memset(pBuf, 0, (size_t)uMem_size);
	
	if((rad == 0) && (bShift == 8))
	{
		memcpy( pDstBuf, pSrcBuf, (size_t)uMem_size );	/* ���̃R�s�[ */
	}
	else
	{
		SS	dx, dy;
		SS	width_h, height_h;
		SS	cos, sin;
		UC	*pDstGR, *pSrcGR;
		
		cos = APL_Cos(rad);
		sin = APL_Sin(rad);
		width_h = width >> 1;
		height_h = height >> 1;

		pSrcGR = pSrcBuf;
		pDstGR = pBuf;
		
		for(y = 0; y < height; y++)
		{
			dy = y - height_h;
			
			for(x = 0; x < width; x++)
			{
				dx = x - width_h;
				
				vx = (((dx * cos) - (dy * sin)) >> bShift) + width_h;
				vy = (((dx * sin) + (dy * cos)) >> bShift) + height_h;
				
				if( (vx >= 0) && (vy >= 0) && (vx < width) && (vy < height) )
				{
					pSrcGR = pSrcBuf + ((vy * width) + vx);
					*pDstGR = *pSrcGR;
				}
				pDstGR++;
			}
		}
	}
	MyMfree(pSrcBuf);	/* ��������� */

	/* CG -> PCG */
	pBuf = pDstBuf;

	uPointer_ADR = (UL)pDst;
	uPointer_ADR_Y = 0ul;

	for(y = 0; y < bY_num; y++)	/* ��s�� */
	{
		uPointer_ADR_subY = 0ul;
		
		for(vy = 0; vy < 16; vy++)	/* 16bit��(8dot+8dot) */
		{
			uPointer_ADR_X = 0ul;
			
			for(x = 0; x < bX_num; x++)	/* ��� */
			{
				uPointer_ADR_subX = 0ul;
				
				for(vx = 0; vx < 2; vx++)	/* 16bit�� */
				{
					uADDR = uPointer_ADR + uPointer_ADR_Y + uPointer_ADR_subY + uPointer_ADR_X + uPointer_ADR_subX;	/* ���_ */
				
					uPCG_ARY[7] = (UL)*pBuf;
					pBuf++;
					uPCG_ARY[6] = (UL)*pBuf;
					pBuf++;
					uPCG_ARY[5] = (UL)*pBuf;
					pBuf++;
					uPCG_ARY[4] = (UL)*pBuf;
					pBuf++;
					uPCG_ARY[3] = (UL)*pBuf;
					pBuf++;
					uPCG_ARY[2] = (UL)*pBuf;
					pBuf++;
					uPCG_ARY[1] = (UL)*pBuf;
					pBuf++;
					uPCG_ARY[0] = (UL)*pBuf;
					pBuf++;
					
					uPCG = 	((uPCG_ARY[7] << 28ul) & 0xF0000000ul) |
							((uPCG_ARY[6] << 24ul) & 0x0F000000ul) |
							((uPCG_ARY[5] << 20ul) & 0x00F00000ul) |
							((uPCG_ARY[4] << 16ul) & 0x000F0000ul) |
							((uPCG_ARY[3] << 12ul) & 0x0000F000ul) |
							((uPCG_ARY[2] <<  8ul) & 0x00000F00ul) |
							((uPCG_ARY[1] <<  4ul) & 0x000000F0ul) |
							((uPCG_ARY[0]        ) & 0x0000000Ful);

					pDstWork = (UL *) uADDR;
					*pDstWork = uPCG;

					uPointer_ADR_subX += 0x40ul;
				}
				uPointer_ADR_X += 0x80ul;
			}
			uPointer_ADR_subY += 0x04ul;
		}
		uPointer_ADR_Y += 0x800ul;
	}
	
	MyMfree(pDstBuf);	/* ��������� */

	/* PCG -> SP */
	uPointer_ADR = (UL)pDst;
	uPointer_ADR_Y = 0ul;
	spNum = *sp_num;
	
	for(y = 0; y < bY_num; y++)	/* ��s�� */
	{
		uPointer_ADR_X = 0ul;

		for(x = 0; x < bX_num; x++)	/* ��� */
		{
			UC	patNum;
			patNum = (UC)( (UL) ( (uPointer_ADR - 0xEB8000ul) + uPointer_ADR_Y + uPointer_ADR_X ) / 0x80ul); /*0x43*/;
			code = (UL)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
			SP_REGST( spNum++, -1, pos_x + (x * 16), pos_y + (y * 16), code, 3);
			*sp_num = spNum;
			uPointer_ADR_X += 0x80ul;
		}
		uPointer_ADR_Y += 0x800ul;
	}
}

void BG_TEXT_SET(SC *fname)
{
	US	usV_pat;
	UI	pal = 0;
	US	i,j;
	SI	x,y;
	US	map_data[64][64];

	/* �}�b�v�f�[�^�ǂݍ��� */
	File_Load_CSV( fname, (US *)&map_data[0][0], &i, &j);
	
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

