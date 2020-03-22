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


/* �֐��̃v���g�^�C�v�錾 */
void Message_Num(void *, SI, SI, UC);
SS get_key( SI );

/* �֐� */
void Message_Num(void *pNum, SI x, SI y, UC mode)
{
	char str[64];

	switch(mode){
	case MONI_Type_UI:
		{
			UI *num;
			num = (UI *)pNum;
			sprintf(str, "%d", *num);
		}
		break;
	case MONI_Type_SI:
		{
			SI *num;
			num = (SI *)pNum;
			sprintf(str, "%d", *num);
		}
		break;
	case MONI_Type_US:
		{
			US *num;
			num = (US *)pNum;
			sprintf(str, "%d", *num);
		}
		break;
	case MONI_Type_SS:
		{
			SS *num;
			num = (SS *)pNum;
			sprintf(str, "%d", *num);
		}
		break;
	case MONI_Type_UC:
		{
			UC *num;
			num = (UC *)pNum;
			sprintf(str, "%d", *num);
		}
		break;
	case MONI_Type_SC:
		{
			SC *num;
			num = (SC *)pNum;
			sprintf(str, "%d", *num);
		}
		break;
	default:
		{
			US *num;
			num = (US *)pNum;
			sprintf(str, "%d", *num);
		}
		break;
	}
	B_PUTMES( 2, x, y, 10-1, str);
}

SS get_key( SI mode )
{
	UI kd_k1,kd_k2_1,kd_k2_2,kd_b,kd_b2,kd_b3,kd_b4;
	static SS repeat_flag_a = KEY_TRUE;
	static SS repeat_flag_b = KEY_TRUE;
	SS ret = 0;
	
	kd_k1	= BITSNS(7);
	kd_k2_1	= BITSNS(8);
	kd_k2_2	= BITSNS(9);
	kd_b	= BITSNS(10);
	kd_b2	= BITSNS(5);
	kd_b3	= BITSNS( 2 );
	kd_b4	= BITSNS( 0 );
	
	if( (kd_b3  & 0x02 ) != 0 ) ret |= KEY_b_Q;		/* �p�ŏI�� */
	if( (kd_b4  & 0x02 ) != 0 ) ret |= KEY_b_ESC;	/* �d�r�b�|�[�Y */

	if( !( JOYGET( 0 ) & UP    ) || ( kd_k1 & 0x10 ) || ( kd_k2_1 & 0x10 ) ) ret |= KEY_UPPER;
	if( !( JOYGET( 0 ) & DOWN  ) || ( kd_k1 & 0x40 ) || ( kd_k2_2 & 0x10 ) ) ret |= KEY_LOWER;
	if( !( JOYGET( 0 ) & LEFT  ) || ( kd_k1 & 0x08 ) || ( kd_k2_1 & 0x80 ) ) ret |= KEY_LEFT;
	if( !( JOYGET( 0 ) & RIGHT ) || ( kd_k1 & 0x20 ) || ( kd_k2_2 & 0x02 ) ) ret |= KEY_RIGHT;
	if( !( JOYGET( 0 ) & JOYA  ) || ( kd_b  & 0x20 ) || ( kd_b2   & 0x04 ) ) { /* �`�{�^�� or XF1 or z */
		if( repeat_flag_a || (mode != 0u)) {
			ret |= KEY_A;
			repeat_flag_a = KEY_FALSE;
			}
		} else {
			repeat_flag_a = KEY_TRUE;
		}
	if( !( JOYGET( 0 ) & JOYB  ) || ( kd_b  & 0x40 ) || ( kd_b2   & 0x08 ) ) { /* �a�{�^�� or XF2 or x  */
		if( repeat_flag_b || (mode != 0u)) {
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
	US	pal_tmp[512];
	US	*ptr;
	US	usV_pat;
	UI	pal = 0;
	US	usFreeRunCount=0;
	UI	uJdge=0;
	UI	uCountNum;
	UI	uSQ;
	SI	ras_st;
	SS	vx, vy;
	SS	road;
	SS	road_offset_x, road_offset_y;
	SS	input;
	UC	road_flag;
	struct _fntbuf	stFont;

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
	
	for(a=0; a < 512; a++)
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
	
	pal = 0x0E;
	for(y=0; y<16; y++)
	{
		for(x=0; x<i; x++)
		{
			BGTEXTST(0,x,y,SetBGcode(0,0,pal,map_data[y][x]));		/* BG�e�L�X�g�ݒ� */
			BGTEXTST(1,x,y,SetBGcode(0,0,pal+1,map_data[y][x]));		/* BG�e�L�X�g�ݒ� */
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
		Draw_Fill((e << 6), Y_HORIZON-16-1, (e << 6) + 10, Y_HORIZON-1, 14);
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
	vx = vy =0;
	usFreeRunCount = 0;
	do
	{
		UI unHantei = 0;
		US time, time_old, time_now;
		
		if((usFreeRunCount % 10) == 0){
			GetNowTime(&time_old);
		}
		
		input = get_key(1);
		if(input & KEY_b_Q   ) loop = 0;	/* �p�ŏI�� */
		if(input & KEY_b_ESC ) loop = 0;	/* �d�r�b�|�[�Y */
		if(loop == 0)break;
		
		if(input & KEY_UPPER)	vy += 1;
		if(input & KEY_LOWER)	vy -= 1;
		vy = Mmax(Mmin(vy, 16), -16);

		if(input & KEY_RIGHT)	vx += 1;
		if(input & KEY_LEFT)	vx -= 1;
		vx = Mmax(Mmin(vx, 32), -32);
		
		if(input & KEY_A){
			speed += 1;
		}
		else{
			if((usFreeRunCount % 5) == 0)speed -= 1;
		}
		speed = Mmax(Mmin(speed, 32), 0);

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
		
		d = 0;

		uJdge = (UI)((RASTER_ED - RASTER_ST) * (RASTER_ED - RASTER_ST));

		unHantei = 0;
		for(y=RASTER_ST; y < RASTER_ED; y+=RASTER_NEXT)
		{
			UI num = (UI)y- RASTER_ST;
			UI Anime;

			Anime = (num + (0xF & usFreeRunCount));
			
			/* Y���W */
			pal_tmp[y] = d * 96;
			
			if( Anime > unHantei ){
				unHantei = (float)Anime * 1.2;
				d++;
				if(d >= 4)d=0;
			}
			
			if(y >= (RASTER_ED - RASTER_NEXT)){		/* ���X�^�̍Ō�͌��̍��W�ɖ߂� */
				d = 0;
			}

			/* X���W */
			ras_tmp[y] = num * ((float)vx / 32);
			ras_tmp[y] += road * ( (RASTER_ED - RASTER_ST) / (float)(Mmax(y-RASTER_ST, 1)) );

			/* ���X�^�[��΂��� */
			for(i = 0; i < RASTER_NEXT; i++){
				ras_tmp[y+i] = ras_tmp[y];
				pal_tmp[y+i] = pal_tmp[y];
			}
		}
		SetRasterVal(ras_tmp, sizeof(US)*RASTER_MAX);
		SetRasterPal(pal_tmp, sizeof(US)*RASTER_MAX);
		
#if 0
		stFont.xl = 8;
		stFont.yl = 8;
		memcpy(stFont.buffer, (unsigned char *)(0xEB8000+0x800), stFont.xl * stFont.yl);
		TEXTPUT(128+0,128, &stFont);
		memcpy(stFont.buffer, (unsigned char *)(0xEB8000+0x800+32), stFont.xl * stFont.yl);
		TEXTPUT(128+8,128, &stFont);
		memcpy(stFont.buffer, (unsigned char *)(0xEB8000+0x800+64), stFont.xl * stFont.yl);
		TEXTPUT(128+16,128, &stFont);
		memcpy(stFont.buffer, (unsigned char *)(0xEB8000+0x800+96), stFont.xl * stFont.yl);
		TEXTPUT(128*24,128, &stFont);
#endif

#if 1
		if((usFreeRunCount % 10) == 0){
			
			/* �������Ԍv�� */
			GetNowTime(&time_now);
			time = time_now - time_old;
			time_cal = time;	/* LSB:1 UNIT:ms */
			time_cal_PH = Mmax(time_cal, time_cal_PH);

			/* ���j�^ */
			Message_Num(&speed,			0, 0, MONI_Type_SS);
			Message_Num(&vx, 			0, 1, MONI_Type_SS);
			Message_Num(&vy, 			0, 2, MONI_Type_SS);
			Message_Num(&time_cal,	 	0, 3, MONI_Type_SI);
			Message_Num(&time_cal_PH,	0, 4, MONI_Type_SI);
			Message_Num(&road,			0, 5, MONI_Type_SI);
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

#if	1
	for(x=RASTER_ST-1;x<=RASTER_ED;x++){
		printf("%3d=%3d,", x, pal_tmp[x]);
		if((x % 11) == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
#endif

}

#endif	/* OVERKATA_C */
