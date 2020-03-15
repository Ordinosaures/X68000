#ifndef	OVERKATA_C
#define	OVERKATA_C

#include <iocslib.h>
#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>
#include <io.h>
#include <math.h>

#include "inc/usr_define.h"
#include "inc/usr_macro.h"

#include "OverKata.h"
#include "Draw.h"
#include "MFP.h"

/* �O���[�o���ϐ� */
SS moni;
SS moni_MAX;
SS speed;
US	map_data[64][64];

/* �ϐ� */
static SS key;

/* �֐��̃v���g�^�C�v�錾 */
void Message_Num(SS, SI, SI);

/* �֐� */
void Message_Num(SS num, SI x, SI y)
{
	char str[64];

	sprintf(str, "%d", num);
	B_PUTMES( 3, x, y, 10-1, str);
}

SS get_key( void )
{
	UI kd_k1,kd_k2_1,kd_k2_2,kd_b;
	static SS repeat_flag_a = KEY_TRUE;
	static SS repeat_flag_b = KEY_TRUE;
	SS ret = 0;
	
	kd_k1 = BITSNS(7);
	kd_k2_1 = BITSNS(8);
	kd_k2_2 = BITSNS(9);
	kd_b = BITSNS(10);
	
	if( !( JOYGET( 0 ) & UP    ) || ( kd_k1 & 0x10 ) || ( kd_k2_1 & 0x10 ) ) ret |= KEY_UPPER;
	if( !( JOYGET( 0 ) & DOWN  ) || ( kd_k1 & 0x40 ) || ( kd_k2_2 & 0x10 ) ) ret |= KEY_LOWER;
	if( !( JOYGET( 0 ) & LEFT  ) || ( kd_k1 & 0x08 ) || ( kd_k2_1 & 0x80 ) ) ret |= KEY_LEFT;
	if( !( JOYGET( 0 ) & RIGHT ) || ( kd_k1 & 0x20 ) || ( kd_k2_2 & 0x02 ) ) ret |= KEY_RIGHT;
	if( !( JOYGET( 0 ) & JOYA  ) || kd_b & 0x20 ) { /* �`�{�^�� */
		if( repeat_flag_a ) {
			ret |= KEY_A;
			repeat_flag_a = KEY_FALSE;
			}
		} else {
			repeat_flag_a = KEY_TRUE;
		}
	if( !( JOYGET( 0 ) & JOYB  ) || kd_b & 0x40 ) { /* �a�{�^�� */
		if( repeat_flag_b ) {
			ret |= KEY_B;
			repeat_flag_b = KEY_FALSE;
			}
		} else {
			repeat_flag_b = KEY_TRUE;
		}
	return ret;
}


void main(void)
{
	SI superchk;
	SI crtmod;
	SI a,b,c,d,e,f,g,i,j,k,l,m;
	SI x,y,x1,x2,y1,y2;
	SI px1,px2;
	SI h,w;
	SI col_1, col_2, col_3;
	SI loop;
	SI RD[1024];
	SI time_cal = 0;
	SI time_cal_PH = 0;
	US	ras_tmp[512];
	US	*ptr;
	SI	vx;
	UI	vi, vj;

	/* �f�o�b�O�R�[�i�[ */
#if 0
	/* �}�b�v�f�[�^�ǂݍ��� */
	File_Load_CSV("data/map.csv", map_data, &vi, &vj);
	
	for(y=0;y<64;y++)
	{
		for(x=0;x<64;x++)
		{
			printf("%d,", map_data[y][x]);
		}
		printf("\n");
	}
	
	loop = 1;
	do
	{
		if( ( BITSNS( 0 ) & 0x02 ) != 0 ) loop = 0;	/* �d�r�b�|�[�Y */
		if(loop == 0)break;
	}
	while( loop );
	
	exit(0);
#endif
	
	/* �X�[�p�[�o�C�U�[���[�h�J�n */
	/*�c�n�r�̃X�[�p�[�o�C�U�[���[�h�J�n*/
	superchk = SUPER(0);
	if( superchk < 0 ) {
		puts("���łɓ����Ă�B");
	} else {
		puts("�������B");
	}
	
	/*��ʂ̏����ݒ�*/
	crtmod = CRTMOD(11);	/* �����F�W���𑜓x�A��F�W�� */
	G_CLR_ON();
	VPAGE(1);
	APAGE(0);
	WINDOW( X_MIN_DRAW, Y_MIN_DRAW, X_MAX_DRAW, Y_MAX_DRAW);
	HOME(0, X_OFFSET, Y_OFFSET);
	WIPE();
	
	/*�J�[�\���������܂��B*/
	B_CUROFF();

	/* MFP�֘A�̏����� */
	Init_MFP();

	/*Timer-D�Z�b�g*/
	TIMERDST(Timer_D_Func, 7, 200);	/* 50us(7) x 200cnt = 10ms */
	SetNowTime(0);

	/* �ϐ��̏����� */
	px1 = X_MAX_H;
	px2 = X_MAX_H;
	speed = 0;
	
	for(a=0; a < 1024; a++)
	{
		RD[a] = 0;
	}
	
	/* �����̏����� */
	srandom(TIMEGET());
	for(a=0; a < 16; a++)
	{
		d = a * 64;
		c = (rand() % 5) - 2;
		for(b = d; b < d + 64; b++)
		{
			RD[b] = c;
		}
	}
	
	/* �X�v���C�g�̏����� */
	SP_INIT();			/* �X�v���C�g�̏����� */
	SP_ON();			/* �X�v���C�g�\�����n�m */
	BGCTRLST(0, 0, 1);	/* �a�f�O�\���n�m */
	BGCTRLST(1, 1, 1);	/* �a�f�P�\���n�m */
	/*�X�v���C�g���W�X�^������*/
	for(j = 0x80000000; j < 0x80000000 + 128; j++ ){
		SP_REGST(j,-1,0,0,0,0);
	}
	/* �X�v���C�g�̃f�[�^�ǂݍ��� */
	/* �X�v���C�g�̃p���b�g�f�[�^�ǂݍ��� */
	sp_dataload();

	//	BGCTRLGT(1);				/* BG�R���g���[�����W�X�^�ǂݍ��� */
	BGSCRLST(0,0,0);				/* BG�X�N���[�����W�X�^�ݒ� */
	BGSCRLST(1,0,0);				/* BG�X�N���[�����W�X�^�ݒ� */
	BGTEXTCL(0,SetBGcode(0,0,0,0));	/* BG�e�L�X�g�N���A */
	BGTEXTCL(1,SetBGcode(0,0,0,0));	/* BG�e�L�X�g�N���A */
	//	BGTEXTGT(1,1,0);			/* BG�e�L�X�g�ǂݍ��� */
	
	/* �}�b�v�f�[�^�ǂݍ��� */
	File_Load_CSV("data/map.csv", map_data, &i, &j);
	for(y=0; y<j; y++)
	{
		for(x=0; x<i; x++)
		{
			int pat = 0;
			int offset = 0;
			switch(map_data[y][x])
			{
				case 1:
					pat = 1;
					offset = 0;
					break;
				case 2:
					pat = 2;
					offset = 64;
					break;
				case 3:
					pat = 3;
					offset = 64;
					break;
				case 4:
					pat = 0x0F;
					offset = 64;
					break;
				defult:
					pat = 0x0F;
					offset = 64;
					break;
			}
			BGTEXTST(0,x,y,SetBGcode(0,0,pat,map_data[y][x]+offset));		/* BG�e�L�X�g�ݒ� */
//			BGTEXTST(1,x,y,SetBGcode(0,0,0,90));		/* BG�e�L�X�g�ݒ� */
		}
	}
	moni = i;
	moni_MAX = j;
	
	/* �O���t�B�b�N�p���b�g�̏����� */
	for(a=0; a < 0xFF; a++)
	{
		GPALET( a, SetRGB(31, 31, 31));	/* White */
	}
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

	/* �����ƃR�[�X�̊� */
	Draw_Fill( X_MIN_DRAW,  Y_HORIZON, X_MAX_DRAW,  Y_HORIZON, 1);

	/* ���� */
	for(e = 0; e < 8; e++)
	{
		Draw_Fill((e << 6), 0/*Y_HORIZON-16-1*/, (e << 6) + 10, Y_HORIZON-1, 14);
	}
#if 0
	/* �� */
	Draw_Fill( X_MIN_DRAW, Y_MIN_DRAW, X_MAX_DRAW, Y_MAX_DRAW, 3);

	a = 0;
	c = 1;
	e = 8;
	
	/* ��ʂ̕`�� */
	for(y=Y_HORIZON; y < Y_MAX; y++)
	{
		/* Y */
		h = y - Y_HORIZON;
		y1 = Y_MAX - (y - Y_HORIZON);	/* ������`�� */

		/* X */
		w = y - Y_HORIZON;
		i = 32 * ((float)((Y_MAX - Y_HORIZON) - w) / (Y_MAX - Y_HORIZON));	/* ���Ε������߂� */
		x = ((float)(X_MAX_H - X_MIN) / (Y_MAX - Y_HORIZON)) * w;
		x1 = X_MIN + x;
		x2 = X_MAX - x;
		
		/* �R�[�X��� */
		if(px1 > x2){
			x2 = px1;
		}
		else if(px2 < x1){
			x1 = px2;
		}
		else{
		}
		px1 = x1;
		px2 = x2;
		
		/* ���΂̉��s */
		b = 64 * (2 /(float)(c*(c+1)));	/* 2m�̎��_����̉��s�̔䗦 */
		if(h > b + e){
			if(a == 0){
				a=1;
			}
			else{
				a=0;
			}
			e = h;
			c++;
		}
		
		/* �p���b�g�A�j���[�V���� */
		if( a == 0 ){
			col_1 = 1;
			col_2 = 5;
			col_3 = 11;
		}
		else{
			col_1 = 2;
			col_2 = 15;
			col_3 = 8;
		}
		/* �R�[�X�A�E�g */
		Draw_Line( X_MIN_DRAW,     y1,         x1,   y1,  col_3, 0xFFFF);	/* ���� */
		Draw_Line(         x2,     y1, X_MAX_DRAW,   y1,  col_3, 0xFFFF);	/* �E�� */
			
		/* ��1 */
		Draw_Line(         x1,     y1,         x2,   y1,  col_1, 0xFFFF );

		/* ���� */
		Draw_Line(     x1 - i,     y1,         x1,   y1,  col_2, 0xFFFF );	/* ���� */
		Draw_Line(         x2,     y1,     x2 + i,   y1,  col_2, 0xFFFF );	/* �E�� */
	}
#endif
	
	VDISPST(Vsync_Func, 0, 1);			/* V-Sync���荞�� �A�� */

	a = 0;
	c = 0;
	speed = 0;
	loop = 1;
	do
	{
#if 0		
		US time, time_old, time_now;
		GetNowTime(&time_old);
#endif

		if( ( BITSNS( 2 ) & 0x02 ) != 0 ) loop = 0;	/* �p�ŏI�� */
		if( ( BITSNS( 0 ) & 0x02 ) != 0 ) loop = 0;	/* �d�r�b�|�[�Y */
		if(loop == 0)break;

#if 1
		if(get_key() == KEY_UPPER)	speed += 1; 
		if(get_key() == KEY_LOWER)	speed -= 1; 
		if(get_key() == KEY_RIGHT)	vx += 1; 
		if(get_key() == KEY_LEFT)	vx -= 1; 
		speed = Mmax(Mmin(speed, 256), 0);
		vx = Mmax(Mmin(vx, 7), -8);
#else
		a++;
		if(a >= 1024)
		{
			a = 0;
			c = 0;
		}
		d = Mmin(Mmax(RD[a], -32768), 32767);
		c += d;

		speed = c / 100;
#endif
		
		for(y=0; y < RASTER_MAX; y++)
		{
			ras_tmp[y] = X_MIN;
			if(y >= RASTER_ST){
				ras_tmp[y] += vx * ( (RASTER_ED - RASTER_ST) / (float)(Mmax(y-RASTER_ST, 1)) );
			}
		}
		SetRasterVal(ras_tmp, sizeof(US)*RASTER_MAX);
		
//		moni = ras_tmp[RASTER_ST];
//		moni_MAX = ras_tmp[RASTER_ED];

		Message_Num(speed, 0, 0);
		Message_Num(moni, 0, 1);
		Message_Num(moni_MAX, 0, 2);
		
#if 0		
		/* �������Ԍv�� */
		GetNowTime(&time_now);
		time = time_now - time_old;
		time_cal = time << 4;	/* ���ʕϊ� */
		time_cal_PH = Mmax(time_cal, time_cal_PH);
		Message_Num(time_cal, 0, 3);
		Message_Num(time_cal_PH, 0, 4);
#endif
		/* �����҂� */
		vwait(1);
	}
	while( loop );

	CRTCRAS((void *)0, 0);		/* stop */
	HSYNCST((void *)0);			/* stop */
	VDISPST((void *)0, 0, 0);	/* stop */
	TIMERDST((void *)0, 0, 1);	/* stop */

	BGCTRLST(0, 0, 0);	/* �a�f�O�\���n�m */
	BGCTRLST(1, 1, 0);	/* �a�f�P�\���n�m */
	G_CLR_ON();

	B_CURON();

	CRTMOD(crtmod);
	
	/*�X�[�p�[�o�C�U�[���[�h�I��*/
	SUPER(superchk);
}

#endif	/* OVERKATA_C */
