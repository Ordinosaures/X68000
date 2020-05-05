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
UC g_mode;

/* �֐��̃v���g�^�C�v�錾 */
void main(void);

/* �֐� */
void main(void)
{
	UC	road_flag = 0;
	UC	updown_flag = 0;
	US	x1,x2,y1,y2;
	US	x,y;
	US	ras_tmp[256];
	US	pal_tmp[256];
	US	*ptr;
	US	usFreeRunCount=0;
	US	uJdge=0;
	US	uCountNum;
	US	uSQ;
	US	ras_st, ras_ed, ras_size;
	SS	a,b,c,d,e,f,i;
	SS	loop;
	SS	RD[1024];
	SS	nHorizon, nHorizon_tmp, nHorizon_ras, nHorizon_offset;
	SS	vx, vy;
	SS	road;
	SS	road_offset_x, road_offset_y;
	SS	view_offset_x, view_offset_y;
	US	uRoad_rate, uRoad_strch;
	SS	input;
	UI	start_time;
	UI	time_cal = 0;
	UI	time_cal_PH = 0;
	SI	superchk;
	SI	crtmod;
	UC	mode_flag = 0;
	UC	bUpdate;
	
	volatile US *CRTC_R21 = (US *)0xE8002Au;	/* �e�L�X�g�E�A�N�Z�X�E�Z�b�g�A�N���A�[P.S */
	volatile US *CRTC_480 = (US *)0xE80480u;	/* CRTC����|�[�g */

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
	
	crtmod = CRTMOD(-1);	/* ���݂̃��[�h��Ԃ� */
	
	/* �O���t�B�b�N�\�� */
	G_INIT();		/*��ʂ̏����ݒ�*/
	APICG_DataLoad("data/cg/Over_A.pic"	, X_OFFSET, 		Y_OFFSET - 32,	0);	/* FPS */
	APICG_DataLoad("data/cg/Over_B.pic"	, X_OFFSET + 96,			  165,	0);	/* TPS */
	G_MyCar();		/* ���Ԃ̕\�� */
	APICG_DataLoad("data/cg/Over_C.pic"	, 0,							0,	1);	/* ���C�o���� */
	APICG_DataLoad("data/cg/Over_D.pic"	, X_OFFSET - 32,	Y_OFFSET +  4,	1);	/* �w�i */
	G_Background();	/* �w�i�̕\�� */
	
	/* �X�v���C�g�^�a�f�\�� */
	PCG_INIT();							/* �X�v���C�g�^�a�f�̏����� */
	PCG_SP_dataload("data/sp/BG.SP");	/* �X�v���C�g�̃f�[�^�ǂݍ��� */
	PCG_PAL_dataload("data/sp/BG.PAL");	/* �X�v���C�g�̃p���b�g�f�[�^�ǂݍ��� */
	BG_TEXT_SET("data/map/map.csv");	/* �}�b�v�f�[�^�ɂ��a�f�̔z�u */
	
	/* �e�L�X�g�\�� */
	T_INIT();		/* �e�L�X�g�u�q�`�l������ */
	T_TopScore();	/* TOP */
	T_Time();		/* TIME */
	T_Score();		/* SCORE */
	BG_TextPut("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 232);
	for(i=0; i < 10; i++)
	{
		x = 0;
		y = 240;
		BG_PutToText(   0x80+ (i<<1) + 0, x + BG_WIDTH * i,	y,				BG_Normal, TRUE);	/* ������i�㑤�j*/
		BG_PutToText(   0x80+ (i<<1) + 1, x + BG_WIDTH * i,	y+BG_HEIGHT,	BG_Normal, TRUE);	/* ������i�����j*/
	}

	/* �ϐ��̏����� */
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
	
	Init_MFP();						/* MFP�֘A�̏����� */
	TIMERDST(Timer_D_Func, 7, 20);	/* Timer-D�Z�b�g */	/* 50us(7) x 20cnt = 1ms */
	SetNowTime(0);					/* ���Ԃ̏����� */
	GetNowTime(&start_time);		/* Time Count�p */
	VDISPST(Vsync_Func, 0, 1);		/* V-Sync���荞�� �A�� */
	
	/* ���y */
	
	
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	g_mode = 1;
	speed = 0;
	road = 0;
	loop = 1;
	vx = vy =0;
	usFreeRunCount = 0;
	uCountNum = 0;
	bUpdate = FALSE;
	
	do
	{
		SI nHantei = 0;
		US uCount = 0;
		UI time, time_old, time_now;
		
		GetNowTime(&time_old);
		
		input = get_key(1);
		if((input & KEY_b_Q   ) != 0u) loop = 0;	/* �p�ŏI�� */
		if((input & KEY_b_ESC ) != 0u) loop = 0;	/* �d�r�b�|�[�Y */
		if(loop == 0)break;
		
		if((input & KEY_b_M   ) != 0u)	/* �l�Ń��[�h�ؑ� */
		{
			if( mode_flag == FALSE )
			{
				mode_flag = TRUE;
				if(g_mode == 0u)g_mode = 1u;
				else			g_mode = 0u;
			}
		}
		else
		{
			mode_flag = FALSE;
		}
		
		/* ���[�h�ؑւɂ��ݒ�l�̕ύX */
		switch(g_mode)
		{
			case 0:		/* Screen 0(TPS) */
			{
				view_offset_x = X_OFFSET;
				view_offset_y = Y_MIN_DRAW;
				break;
			}
			case 1:		/* Screen 0(FPS) */
			{
				view_offset_x = X_OFFSET;
				view_offset_y = Y_OFFSET;
				break;
			}
			default:
			{
				view_offset_x = 0;
				view_offset_y = 0;
				break;
			}
		}
		
#if 1
		if( ((usFreeRunCount % (RD[usFreeRunCount & 0x03FFu])) == 0)
			&& ((usFreeRunCount % 5) == 0)
			&& (speed != 0u) )
		{
			if(updown_flag == 0){
				vy += 1;
				if(vy > 24)
				{
					updown_flag = 1;
				}
			}
			else{
				vy -= 1;
				if(vy < -28)
				{
					updown_flag = 0;
				}
			}
		}
#else
		/* �� */
		if((input & KEY_UPPER) != 0u)	vy += 1;
		/* �� */
		if((input & KEY_LOWER) != 0u)	vy -= 1;
#endif
		vy = Mmax(Mmin(vy, 24), -28);

		/* �E */
		if((input & KEY_RIGHT) != 0u)	vx += 1;
		/* �� */
		if((input & KEY_LEFT) != 0u)	vx -= 1;
		vx = Mmax(Mmin(vx, 32), -32);
		
		/* A�{�^�� */
		if((input & KEY_A) != 0u){
			if((usFreeRunCount % 5) == 0)speed += 1;
		}
		else{
			speed -= 1;
		}
		speed = Mmax(Mmin(speed, 32), 0);
		
		/* B�{�^�� */
		if( (input & KEY_B) != 0U )
		{
		}
		else
		{
		}

		/* �R�[�i�[�̕\�� */
#if 1
		if( ((usFreeRunCount % (RD[usFreeRunCount & 0x03FFu])) == 0)
			&& ((usFreeRunCount % 3) == 0)
			&& (speed != 0u) )
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

#if 1
		if(speed == 0){
			d = e;
		}
		
		/* ������ */
		/* ���X�^�[���荞�ݏ����̊J�n�E�I���ʒu */
		switch(g_mode)
		{
			case 0:		/* TPS */
			{
				nHorizon_tmp = Y_HORIZON_0;
				nHorizon = Mmax(Mmin(nHorizon_tmp + vy, Y_HORIZON_0), RASTER_MIN);
				break;
			}
			case 1:		/* FPS */
			{
				nHorizon_tmp = Y_HORIZON_1;
				nHorizon = Mmax(Mmin(nHorizon_tmp + vy, Y_HORIZON_1), RASTER_MIN);
				break;
			}
			default:	/* FPS */
			{
				nHorizon_tmp = Y_HORIZON_1;
				break;
			}
		}
		nHorizon_offset = ROAD_POINT - nHorizon;	/* ���ۂ̃f�[�^�ƕ\���ʒu�Ƃ̍��� */
		ras_st = Mmax(Mmin(nHorizon + RASTER_MIN - RASTER_NEXT, RASTER_MAX - RASTER_NEXT), RASTER_MIN);	/* ���X�^�[�J�n�ʒu */
//		ras_ed = Mmin(Mmax(ras_st + ROAD_SIZE, 154+RASTER_MIN), RASTER_MAX);				/* ���X�^�[�I���ʒu */
		ras_ed = RASTER_MAX;			/* ���X�^�[�I���ʒu */
		ras_size = ras_ed - ras_st;		/* ���X�^�[�͈̔� */
		pal_tmp[0] = ras_st;			/* ���荞�݈ʒu�̐ݒ� */
		
		uRoad_rate = ras_size / ROAD_SIZE;
		uRoad_strch = 0;
		uCount = 0;
		
		for(y=ras_st; y < ras_ed; y+=RASTER_NEXT)
		{
			SI num = y - ras_st;
			SI Anime;
			US uBG_pat[4] = {0, 96, 192, 288};
			US ras_cal;
			
			Anime = num;
			
			/* Y���W */
			if(num == 0u)	/* �����0or512�łȂ��Ƌ�ɓ����o�Ă��� */	/* �Ղ̂Ђ� ����̃A�h�o�C�X�ӏ� */
			{
				ras_cal = 0 + uBG_pat[0];
				nHantei = 0;
				road_offset_y = 0;
				c = 0;
				e = d;
				uCount = 0;
			}
			else if(vy < 0)	/* ����� */
			{
				if(uCount > uRoad_rate)
				{
					uRoad_strch ++;
					if(uRoad_strch >= ROAD_SIZE)uRoad_strch=ROAD_SIZE;
					uCount = 0;
				}
				if(nHorizon_offset - uRoad_strch < 0)
				{
					ras_cal = 512 + nHorizon_offset - uRoad_strch + uBG_pat[d];
				}
				else
				{
					ras_cal = nHorizon_offset - uRoad_strch + uBG_pat[d];
				}
			}
			else if(vy > 0)	/* ���� */
			{
				if(uCount > uRoad_rate)
				{
					uRoad_strch ++;
					uCount = 0;
				}
				ras_cal = nHorizon_offset + vy - uRoad_strch + uBG_pat[d];
			}
			else							/* ���R */
			{
				ras_cal = nHorizon_offset + uBG_pat[d];
			}
			pal_tmp[y] = ras_cal;
			
			if( Anime > nHantei + f ){		/* ���΂̐F�̊Ԋu */
				switch(c)
				{
				case 0:
					road_offset_y++;
					if(num > (ras_size>>1))c++;
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
			//ras_tmp[y] = num * ((float)vx / 16);
			ras_tmp[y] = (num * vx) >> 4;										/* HUYE����̃A�h�o�C�X�ӏ� */
			//ras_tmp[y] += road * ( (RASTER_MAX - ras_st) / (float)(Mmax(y-ras_st, 1)) );
			ras_tmp[y] += (Mmin(Mmax((road * ras_size), -256), 512) / Mmax(y-ras_st,1 ));	/* HUYE����̃A�h�o�C�X�ӏ� */
			
			/* ���X�^�[��΂��� */
			for(i = 0; i < RASTER_NEXT; i++){
				ras_tmp[y+i] = ras_tmp[y];
				pal_tmp[y+i] = pal_tmp[y];
			}
			uCount++;
		}
		SetRasterVal(ras_tmp, sizeof(US)*RASTER_MAX);
		SetRasterPal(pal_tmp, sizeof(US)*RASTER_MAX);
#endif
		if((usFreeRunCount % 10) == 0)
		{
			/* �������׉��P�v */
			UI nTimeCounter, nPassTime, nTimer;
			US uScore;
			static US uScoreMax = 3000;
			/* Score */
			uScore = usFreeRunCount;
#if 1
			Text_To_Text(uScore, 176, 8, FALSE);
#else
			BG_Number( uScore, 200, 8);
#endif
			/* Top Score */
			uScoreMax = Mmax(uScore, uScoreMax);
#if 1
			Text_To_Text(uScoreMax, 16, 8, FALSE);
#else
			BG_Number( uScoreMax, 40, 8);
#endif
			/* Time Count */
			GetNowTime(&time_now);
			nPassTime = (time_now - start_time);
			nTimer = 60000 - nPassTime;
			
			if(60000 < nTimer)
			{
				nTimeCounter = 0;
			}
			else
			{
				nTimeCounter = nTimer / 1000;
			}
#if 1
			Text_To_Text(nTimeCounter, 108, 24, TRUE);
#else
			BG_TimeCounter( nTimeCounter, (BG_WIDTH * 16), 24);
#endif

		}
#ifdef DEBUG
		/* �f�o�b�O�R�[�i�[ */
//		BG_TextPut("OverKata", 4, 10);
//		BG_TextPut("OverKata", 128, 128);
//		BG_TextPut("OVER KATA", 128, 128);
#endif
#ifdef DEBUG
		if((usFreeRunCount % 10) == 0)
		{
			/* ���j�^ */
			Message_Num(&time_cal,	 		 0,  8, 2, MONI_Type_UI);
			Message_Num(&time_cal_PH,		10,  8, 2, MONI_Type_UI);
			Message_Num(&speed,				 0,  9, 2, MONI_Type_SS);
			Message_Num(&vx, 				 6,  9, 2, MONI_Type_SS);
			Message_Num(&vy, 				12,  9, 2, MONI_Type_SS);
			Message_Num(&ras_st,			 0, 10, 2, MONI_Type_US);
			Message_Num(&ras_ed,			 7, 10, 2, MONI_Type_US);
			Message_Num(&ras_size,			13, 10, 2, MONI_Type_US);
			Message_Num(&pal_tmp[ras_st],	 0, 11, 2, MONI_Type_US);
			Message_Num(&uRoad_rate,		 6, 11, 2, MONI_Type_US);
			Message_Num(&uRoad_strch,		12, 11, 2, MONI_Type_US);
			Message_Num(&ras_tmp[ras_st],	 0, 12, 2, MONI_Type_SS);
			Message_Num(&road,			 	 6, 12, 2, MONI_Type_SS);

			if((*CRTC_480 & 0x02u) == 0u)	/* �N���A���s���łȂ� */
			{
				*CRTC_R21 = Mbset(*CRTC_R21, 0x0Fu, 0x0Cu);	/* SCREEN1 �����N���AON / SCREEN0 �����N���AOFF */
				*CRTC_480 = Mbset(*CRTC_480, 0x02u, 0x02u);	/* �N���A���s */
				bUpdate = FALSE;
			}
		}
#endif
		/* �`�揈�� */
		/* �`��̃N���A���� */	/* �K���e�L�X�g�\�������̌�ɍs������ */
#if 0
		if( bUpdate == TRUE )	/* �O��`��X�V������ */
		{
		}
		else
#endif
		{
			if(speed <= 8)								/* ������� */
			{
				uCountNum = Mdec(uCountNum, 1);
				bUpdate = TRUE;
			}
			else if( (speed > 8) && (speed < 31) )		/* �ێ�*/
			{
			}
			else
			{
				uCountNum++;							/* ������ */
				bUpdate = TRUE;
			}
		}
		if(uCountNum >= 64)uCountNum = 0;
		if(uCountNum <= 0)uCountNum = 0;

		/* ��ʂ�h�炷 */
		if(speed == 0){	/* ��� */
			f = 0;
		}
		else{			/* ���s�� */
			if((usFreeRunCount % 5) == 0)f = 1;	/* ��ʂ̐U�� */
			else f = 0;
		}
		

		/* ��ʂ̈ʒu */
		HOME(0b01, view_offset_x, view_offset_y + f );	/* Screen 0(TPS/FPS) */
		HOME(0b10, view_offset_x, view_offset_y );		/* Screen 1 */

		if( bUpdate == TRUE )
		{
			/* ���C�o���� */
			US	j,w,h;
			i = uCountNum;
			j = (64-uCountNum)>>4;
			w = ENEMY_CAR_1_W>>j;
			h = ENEMY_CAR_1_H>>j;
			x = view_offset_x + (WIDTH>>1) - ras_tmp[ras_st + (uCountNum << 1)];
			y = view_offset_y + nHorizon + i - 6;

			G_BitBlt(	x,	w,
						y,	h,
						1,
						0,					w,
						ENEMY_CAR_1_H * j,	h,
						1,
						g_mode);
#if 0
			/* ��Q�� */
			w = MY_CAR_0_W>>j;
			h = MY_CAR_0_W>>j;
			for(i=0; i<2; i++)
			{
				US obj_x, obj_y;
				obj_x = x-32;
				obj_y = y;
				Draw_Fill(obj_x,obj_y,obj_x-w,obj_y-6,0xC2u);
				obj_x = x+32;
				obj_y = y;
				Draw_Fill(obj_x,obj_y,obj_x+w,obj_y-6,0xC2u);
			}
#endif
		}

		usFreeRunCount++;	/* �t���[�����J�E���^�X�V */
#if 1
		/* �������Ԍv�� */
		GetNowTime(&time_now);
		time = time_now - time_old;
		time_cal = time;	/* LSB:1 UNIT:ms */
		time_cal_PH = Mmax(time_cal, time_cal_PH);
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
	TextClear();

	CRTMOD(crtmod);			/* ���[�h�����Ƃɖ߂� */
	
	_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */

	/*�X�[�p�[�o�C�U�[���[�h�I��*/
	SUPER(superchk);

#if 1
	printf("pal_tmp[0]�̒��g = %d(vy=%d)\n", pal_tmp[0], vy );
	for(y=ras_st; y < RASTER_MAX; y+=RASTER_NEXT)
	{
		printf("[%3d]=(%5d),", y, pal_tmp[y] );
		if((y%5) == 0)printf("\n");
	}
	printf("\n");
#endif
	
}

#endif	/* OVERKATA_C */
