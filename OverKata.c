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
#include "inc/apicglib.h"

#include "OverKata.h"
#include "Draw.h"
#include "MFP.h"
#include "Input.h"
#include "Output_Text.h"

/* �O���[�o���ϐ� */
SS moni;
SS moni_MAX;
SS speed;
US	map_data[64][64];

/* �֐��̃v���g�^�C�v�錾 */
void main(void);

/* �֐� */
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
	US	ras_tmp[256];
	US	pal_tmp[256];
	US	*ptr;
	US	usV_pat;
	UI	pal = 0;
	US	usFreeRunCount=0;
	UI	uJdge=0;
	UI	uCountNum;
	UI	uSQ;
	SI	nHorizon;
	SI	ras_st, ras_ed;
	SS	vx, vy;
	SS	road;
	SI	road_offset_x, road_offset_y;
	SS	input;
	UC	road_flag;
	float fr;
	
	volatile US *text_addr  = (US *)0xE00000;
	volatile US *V_Sync_end = (US *)0xE8000E;
	volatile US *VIDEO_REG1 = (US *)0xE82400;
	volatile US *VIDEO_REG2 = (US *)0xE82500;
	volatile US *VIDEO_REG3 = (US *)0xE82600;

	/* �f�o�b�O�R�[�i�[ */
#if 0
	UI	vi, vj;
	/* �}�b�v�f�[�^�ǂݍ��� */
	File_Load_CSV("data/map.csv", map_data, &vi, &vj);
	
	for(y=0;y<vi;y++)
	{
		for(x=0;x<vj;x++)
		{
			printf("%d,", map_data[y][x]);
		}
		printf("->(%d,%d)\n",vj, vi);
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
	crtmod = CRTMOD(-1);	/* ���݂̃��[�h��Ԃ� */
	CRTMOD(11);				/* �����F�W���𑜓x�A��F�W�� */
	G_CLR_ON();				/* �O���t�B�b�N�̃N���A */
	VPAGE(0b0011);			/* pege(3:0ff 2:Off 1:0n 0:0n) */
	APAGE(0);				/* �O���t�B�b�N�̏������� */
	WINDOW( X_MIN_DRAW, Y_MIN_DRAW, X_MAX_DRAW, Y_MAX_DRAW);
	HOME(0, X_OFFSET, Y_OFFSET);
	HOME(1, X_OFFSET, Y_OFFSET);
	HOME(2, X_OFFSET, Y_OFFSET);
	HOME(3, X_OFFSET, Y_OFFSET);
//	HOME(2, X_OFFSET, 416);
//	HOME(3, X_OFFSET, 416);
	WIPE();
	*VIDEO_REG1 = Mbset(*VIDEO_REG1,   0x07, 0b001);			/* 512x512 256color 2men */
	*VIDEO_REG2 = Mbset(*VIDEO_REG2, 0x3FFF, 0b10000111100100);	/* �D�揇�� TX>GR>SP GR0>GR1>GR2>GR3 */
	*VIDEO_REG3 = Mbset(*VIDEO_REG3,   0x3F, 0b1100011);		/* ���ꃂ�[�h�Ȃ� ���z���512x512 */
	*V_Sync_end = V_SYNC_MAX;			/* �c�̕\���͈͂����߂�(��ʉ��̃S�~�h�~) */
	
	/*�J�[�\���������܂��B*/
	B_CUROFF();
	MS_CUROF();
	SKEY_MOD(0, 0, 0);

	/* MFP�֘A�̏����� */
	Init_MFP();

	/*Timer-D�Z�b�g*/
	TIMERDST(Timer_D_Func, 7, 20);	/* 50us(7) x 20cnt = 1ms */
	SetNowTime(0);

	/* �ϐ��̏����� */
	px1 = X_MAX_H;
	px2 = X_MAX_H;
	speed = 0;
	road_offset_x = 0;
	road_offset_y = 0;
	
	for(a=0; a < 256; a++)
	{
		ras_tmp[a] = 0;
		pal_tmp[a] = 0;
	}

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
	BGCTRLST(0, 1, 1);	/* �a�f�O�\���n�m */
	BGCTRLST(1, 1, 1);	/* �a�f�P�\���n�m */
//	BGCTRLST(1, 1, 0);	/* �a�f�P�\���n�e�e */
	/*�X�v���C�g���W�X�^������*/
	for(j = 0x80000000; j < 0x80000000 + 128; j++ ){
		SP_REGST(j,-1,0,0,0,0);
	}
	/* �X�v���C�g�̃f�[�^�ǂݍ��� */
	PCG_SP_dataload("data/BG.SP");
	/* �X�v���C�g�̃p���b�g�f�[�^�ǂݍ��� */
	PCG_PAL_dataload("data/BG.PAL");

	//	BGCTRLGT(1);				/* BG�R���g���[�����W�X�^�ǂݍ��� */
	BGSCRLST(0,0,0);				/* BG�X�N���[�����W�X�^�ݒ� */
	BGSCRLST(1,0,0);				/* BG�X�N���[�����W�X�^�ݒ� */
	BGTEXTCL(0,SetBGcode(0,0,0,0));	/* BG�e�L�X�g�N���A */
	BGTEXTCL(1,SetBGcode(0,0,0,0));	/* BG�e�L�X�g�N���A */
	//	BGTEXTGT(1,1,0);			/* BG�e�L�X�g�ǂݍ��� */
	
	/* �}�b�v�f�[�^�ǂݍ��� */
	File_Load_CSV("data/map.csv", map_data, &i, &j);
	
	pal = 0x0E;
	for(y=0; y<16; y++)
	{
		for(x=0; x<i; x++)
		{
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
	/* �e�L�X�g�p���b�g�̏�����(Pal0��SP�Ƌ���) */
	TPALET2( 0, SetRGB( 0,  0,  0));	/* Black */
	TPALET2( 1, SetRGB(31, 31, 31));	/* White */
	TPALET2( 2, SetRGB(31,  0,  0));	/* Red */
	TPALET2( 3, SetRGB(30, 26, 16));	/* Red2 */
	TPALET2( 4, SetRGB( 0, 31,  0));	/* Green */
	TPALET2( 5, SetRGB(30,  8,  0));	/* Orenge */
	TPALET2( 6, SetRGB(30, 30,  0));	/* Yellow */
	TPALET2( 7, SetRGB( 1,  0,  0));	/* Black2 */
	
	/* �O���t�B�b�N�p���b�g�̏����� */
	for(a=0; a < 0xFF; a++)
	{
		GPALET( a, SetRGB(0, 0, 0));	/* Black */
	}
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
	
	/* �ԍ� */
	APICG_DataLoad("data/EVO256_gen.pic", X_OFFSET, Y_OFFSET-32);
#if 1
	/* �摜�ϊ� */
	for(y=Y_OFFSET-32-1; y<(Y_OFFSET-32)+HEIGHT+1; y++)
	{
		for(x=X_OFFSET-1; x<X_OFFSET+WIDTH+1; x++)
		{
			US color;
			
			Draw_Pget(x, y, &color);

			switch(color)
			{
				case 0x00:
				{
					color = 0x01;	/* ���ߐF���s���ߐF */
					break;
				}
				case 0x40:
				{
					color = 0x00;	/* �ϊ��ΏېF�����ߐF */
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
	/* �����ƃR�[�X�̊� */
	Draw_Fill( X_MIN_DRAW,  Y_HORIZON + Y_OFFSET, X_MAX_DRAW,  Y_HORIZON + Y_OFFSET, 1);

	/* ���� */
	for(e = 0; e < 8; e++)
	{
		Draw_Fill((e << 6), Y_HORIZON-16-1 + Y_OFFSET, (e << 6) + 10, Y_HORIZON-1 + Y_OFFSET, 14);
	}
	
	VDISPST(Vsync_Func, 0, 1);			/* V-Sync���荞�� �A�� */

	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	speed = 0;
	loop = 1;
	vx = vy =0;
	usFreeRunCount = 0;
	do
	{
		SI nHantei = 0;
		US time, time_old, time_now;
		
		GetNowTime(&time_old);
		
		input = get_key(1);
		if(input & KEY_b_Q   ) loop = 0;	/* �p�ŏI�� */
		if(input & KEY_b_ESC ) loop = 0;	/* �d�r�b�|�[�Y */
		if(loop == 0)break;
		
		if(input & KEY_UPPER)	vy -= 1;
		if(input & KEY_LOWER)	vy += 1;
		vy = Mmax(Mmin(vy, 1023), -1023);

		if(input & KEY_RIGHT)	vx += 1;
		if(input & KEY_LEFT)	vx -= 1;
		vx = Mmax(Mmin(vx, 32), -32);
		
		if(input & KEY_A){
			if((usFreeRunCount % 5) == 0)speed += 1;
		}
		else{
			speed -= 1;
		}
		speed = Mmax(Mmin(speed, 32), 0);
#if 0
		if( (usFreeRunCount % 5) == 0 )
		{
			if(road_flag == 0){
				road += 1;
				if(road > 12)road_flag = 1;
			}
			else{
				road -= 1;
				if(road < -12)road_flag = 0;
			}
		}
#else
		road = 0;
#endif
		HOME(0, X_OFFSET, Y_OFFSET + f );
		HOME(1, X_OFFSET, Y_OFFSET + f );
//		HOME(2, X_OFFSET + vx, Y_OFFSET - vy);
//		HOME(3, X_OFFSET + vx, Y_OFFSET - vy);
		nHorizon = Y_HORIZON + RASTER_MIN;	//(-1  * (Y_HORIZON >> 1) + 47);
		ras_st = Mmax(Mmin(nHorizon, RASTER_MAX), 0);
		ras_ed = ras_st + ROAD_SIZE;
		pal_tmp[0] = ras_st;
		
		if(speed == 0){
			d = e;
			f = 0;
		}
		else{
			if((usFreeRunCount % 5) == 0)f = 1;
			else f = 0;
		}

		for(y=ras_st; y < ras_ed; y+=RASTER_NEXT)
		{
			SI num = y - ras_st;
			SI Anime;
			US uBG_pat[4] = {0, 96, 192, 288};
//			US uBG_pat[4] = {288,192, 96, 0};
			
			Anime = num;
			
			/* Y���W */
			if(num == 0u){
				//pal_tmp[y] = vy;				/* �����0or512�łȂ��Ƌ�ɓ����o�Ă��� */
				nHantei = 0;
				road_offset_y = 0;
				c = 0;
				e = d;
			}
			else{
				pal_tmp[y] = Mmin(uBG_pat[d] + Y_HORIZON + vy, 1024);
//				pal_tmp[y] = Mmin(uBG_pat[d] + vy, 1024);
			}
			if( Anime > nHantei + f ){		/* ���΂̐F�̊Ԋu */
				switch(c)
				{
				case 0:
					road_offset_y++;
					if(num > (ROAD_SIZE>>1))c++;
					break;
				case 1:
					road_offset_y+=2;
					c++;
					break;
				case 2:
					road_offset_y+=4;
					c++;
					break;
				case 3:
					road_offset_y+=8;
					c++;
					break;
				default:
					road_offset_y+=16;
					c++;
					break;
				}
				nHantei = Anime + road_offset_y;

				d++;
				if(d >= 4)d=0;
			}
			
			/* X���W */
			ras_tmp[y] = num * ((float)vx / 16);
			ras_tmp[y] += road * ( (RASTER_MAX - ras_st) / (float)(Mmax(y-ras_st, 1)) );

			/* ���X�^�[��΂��� */
			for(i = 0; i < RASTER_NEXT; i++){
				ras_tmp[y+i] = ras_tmp[y];
				pal_tmp[y+i] = pal_tmp[y];
			}
		}
		SetRasterVal(ras_tmp, sizeof(US)*RASTER_MAX);
		SetRasterPal(pal_tmp, sizeof(US)*RASTER_MAX);
		
		/* �f�o�b�O�R�[�i�[ */
#if 1
//		BG_TextPut("OverKata", 4, 10);
//		BG_TextPut("OverKata", 128, 128);
		BG_TextPut("OVER KATA", 128, 128);
#endif
		
#if 1
		/* �������Ԍv�� */
		GetNowTime(&time_now);
		time = time_now - time_old;
		time_cal = time;	/* LSB:1 UNIT:ms */
		time_cal_PH = Mmax(time_cal, time_cal_PH);

		if((usFreeRunCount % 10) == 0){
			/* ���j�^ */
			Message_Num(&time_cal,	 	0, 0, MONI_Type_SI);
			Message_Num(&time_cal_PH,	0, 1, MONI_Type_SI);
			Message_Num(&speed,			0, 2, MONI_Type_SS);
			Message_Num(&vx, 			0, 3, MONI_Type_SS);
			Message_Num(&vy, 			0, 4, MONI_Type_SS);
			Message_Num(&nHorizon,		0, 5, MONI_Type_SI);
		}
#endif
		usFreeRunCount++;

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

	CRTMOD(crtmod);			/* ���[�h�����Ƃɖ߂� */
	
	_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */

	/*�X�[�p�[�o�C�U�[���[�h�I��*/
	SUPER(superchk);

#if 1
	printf("pal_tmp�̒��g = %d\n", ras_st );
	for(y=ras_st; y < RASTER_MAX; y+=RASTER_NEXT)
	{
		printf("[%3d]=(%5d),", y, (SS)pal_tmp[y] );
		if((y%5) == 0)printf("\n");
	}
	printf("\n");
#endif
	
}

#endif	/* OVERKATA_C */
