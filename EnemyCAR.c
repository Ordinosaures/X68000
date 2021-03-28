#ifndef	ENEMYCAR_C
#define	ENEMYCAR_C

#include <stdlib.h>
#include <iocslib.h>

#include "inc/usr_macro.h"
#include "EnemyCAR.h"
#include "OverKata.h"

#include "APL_Math.h"
#include "CRTC.h"
#include "Draw.h"
#include "Graphic.h"
#include "MFP.h"
#include "Music.h"
#include "MyCar.h"
#include "Raster.h"

/* �\���̒�` */
ST_ENEMYCARDATA	stEnemyCar[ENEMYCAR_MAX] = {0};
ST_ENEMYCARDATA	*g_pStEnemyCar[ENEMYCAR_MAX];

/* �֐��̃v���g�^�C�v�錾 */
SS	InitEnemyCAR(void);
SS	GetEnemyCAR(ST_ENEMYCARDATA *, SS);
SS	SetEnemyCAR(ST_ENEMYCARDATA, SS);
SS	EnemyCAR_main(UC, UC, UC);
SS	SetAlive_EnemyCAR(void);
SS	Put_EnemyCAR(US, US, US, UC);
SS	Sort_EnemyCAR(void);

/* �֐� */
SS	InitEnemyCAR(void)
{
	SS ret = 0;
	SS i;
	
	for(i=0; i<ENEMYCAR_MAX; i++)
	{
		stEnemyCar[i].ubCarType = 0;
		stEnemyCar[i].VehicleSpeed = 80;
		stEnemyCar[i].x = 0;
		stEnemyCar[i].y = 0;
		stEnemyCar[i].z = 4;
		stEnemyCar[i].ubBrakeLights = 0;
		stEnemyCar[i].ubOBD = 0;
		stEnemyCar[i].ubAlive = FALSE;
		
		g_pStEnemyCar[i] = &stEnemyCar[i];
	}
	
	return ret;
}

SS	GetEnemyCAR(ST_ENEMYCARDATA *stDat, SS Num)
{
	SS	ret = 0;
	if(Num < ENEMYCAR_MAX)
	{
		*stDat = *g_pStEnemyCar[Num];
	}
	else
	{
		ret = -1;
	}
	return ret;
}

SS	SetEnemyCAR(ST_ENEMYCARDATA stDat, SS Num)
{
	SS	ret = 0;
	
	if(Num < ENEMYCAR_MAX)
	{
		*g_pStEnemyCar[Num] = stDat;
	}
	else
	{
		ret = -1;
	}
	return ret;
}

SS	EnemyCAR_main(UC bNum, UC bMode, UC bMode_rev)
{
	SS	ret = 0;
	
	if(bNum < ENEMYCAR_MAX)
	{
		if(g_pStEnemyCar[bNum]->ubAlive == TRUE)
		{
			SS	x, y, z;
			SS	cal;
			SS	dx, dy, dz;
			SS	mx, my, dist;
			US	ras_x, ras_y, ras_pat, ras_num;
			SS	Out_Of_Disp = 0;
			ST_CARDATA	stMyCar;
			ST_CRT		stCRT;
			ST_RAS_INFO	stRasInfo;
			ST_ROAD_INFO	stRoadInfo;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
			UC	bDebugMode;
			GetDebugMode(&bDebugMode);
#endif
			ras_x = 0;
			ras_y = 0;
			
			x = g_pStEnemyCar[bNum]->x;
			y = g_pStEnemyCar[bNum]->y;
			z = g_pStEnemyCar[bNum]->z;

			GetCRT(&stCRT, bMode);
			GetMyCar(&stMyCar);
			GetRasterInfo(&stRasInfo);
			GetRoadInfo(&stRoadInfo);

			dist = stMyCar.VehicleSpeed - g_pStEnemyCar[bNum]->VehicleSpeed;

			cal = Mdiv256(dist * y);	/* ���s�̔䗦�ňړ��ʂ��ς�� */
			if(cal == 0)
			{
				if(dist == 0)
				{
					/* �������Ȃ� */
				}
				else if(dist > 0)
				{
					dist = 1;
				}
				else
				{
					dist = -1;
				}
				
			}
			else
			{
				dist = cal;
			}
			
			/* ���C�o���ԂƂ̋��� */
			if(dist >= 0)
			{
				y += Mmax(dist, 1);
			}
			else
			{
				dist = Mabs(dist);
				y -= dist;
				y = Mmax( y, stRasInfo.st );
				if(y == stRasInfo.st)	/* ����z���ꂽ */
				{
					Out_Of_Disp = -1;
				}
			}

			ras_num = Mmax( Mmin( y, stRasInfo.ed ), stRasInfo.st + RASTER_NEXT);	/* ���X�^�[���̔z��ԍ����Z�o */
			/* �ʒu */
			y = Mmax(y, ras_num);
			my = Mmax(y - 32, 0);

			ret = GetRasterIntPos(&ras_x, &ras_y, &ras_pat, ras_num);	/* �z��ԍ��̃��X�^�[���擾 */
			
			/* �Z���^�[ */
			if( ras_x < 256 )	/* ���J�[�u */
			{
				mx = ROAD_CT_POINT + (  0 - ras_x);
			}
			else	/* �E�J�[�u */
			{
				mx = ROAD_CT_POINT + (512 - ras_x);
			}
			
			if(x < 8)
			{
				cal = Mmul16(x) * my;
				mx -= Mdiv128(cal);
			}
			else
			{
				cal = x-8;
				cal = Mmul16(cal) * my;
				mx += Mdiv128(cal);
			}

			if( (y > 0) && (ret >= 0) && (y < Y_MAX_WINDOW) && (Out_Of_Disp == 0))
			{
				dx = mx;
				dy = y;
				/* �������e�����œ_�����^�i�œ_�����{Z�ʒu�j��256�{����16�Ŋ����� pat 0-10 */
				z = Mmin( Mmax( 10 - ( Mdiv10( Mmul256(my) / (my + ROAD_ED_POINT) ) ) , 0), 10 );
				dz = z;
				
				/* �����蔻��̐ݒ� */
				g_pStEnemyCar[bNum]->sx = dx - (Mdiv2(ENEMY_CAR_1_W >> dz));
				g_pStEnemyCar[bNum]->ex = dx + (Mdiv2(ENEMY_CAR_1_W >> dz));
				g_pStEnemyCar[bNum]->sy = dy - (Mdiv2(ENEMY_CAR_1_H >> dz));
				g_pStEnemyCar[bNum]->ey = dy + (Mdiv2(ENEMY_CAR_1_H >> dz));
				
				/* �`�� */
				Out_Of_Disp = Put_EnemyCAR(	stCRT.hide_offset_x + dx,
											stCRT.hide_offset_y + dy,
											dz,
											bMode_rev);
				if(Out_Of_Disp < 0)	/* �`��̈�O */
				{
					/* �o���|�C���g�ŕ`��̈�O�ƂȂ�̂ŗv���� */
//					g_pStEnemyCar[bNum]->ubAlive = FALSE;
				}
				else
				{
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
					if(bDebugMode == TRUE)
					{
						Draw_Box(
							stCRT.hide_offset_x + dx - ((ENEMY_CAR_1_W >> dz)>>2),
							stCRT.hide_offset_y + dy - ((ENEMY_CAR_1_H >> dz)>>1),
							stCRT.hide_offset_x + dx + ((ENEMY_CAR_1_W >> dz)>>2),
							stCRT.hide_offset_y + dy + ((ENEMY_CAR_1_H >> dz)>>1), 0x13, 0xFFFF);
					}
#endif
				}
			}
			else
			{
				if(y >= Y_MAX_WINDOW)	/* �ǂ��������� */
				{
					ADPCM_Play(14);	/* SE�F�����Ԓʉ� */
				}
				if(Out_Of_Disp < 0)
				{
					ADPCM_Play(16);	/* SE�FWAKAME */
				}
				g_pStEnemyCar[bNum]->ubAlive = FALSE;
			}

			g_pStEnemyCar[bNum]->x = x;
			g_pStEnemyCar[bNum]->y = y;
			g_pStEnemyCar[bNum]->z = z;
		}
		else
		{
			/* nop */
		}
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}

SS	SetAlive_EnemyCAR(void)
{
	SS	ret = 0;
	SS	i;
	ST_CARDATA	stMyCar;

	GetMyCar(&stMyCar);

	for(i=0; i<ENEMYCAR_MAX; i++)
	{
		if(g_pStEnemyCar[i]->ubAlive == FALSE)
		{
			UI	rand = 0;
			/* ���E�̋��� */
			rand = random();
			rand &= 0x0Fu;
			
			g_pStEnemyCar[i]->ubCarType = 0;
			g_pStEnemyCar[i]->VehicleSpeed = Mmax(Mmin(stMyCar.VehicleSpeed, 110), 80);
			g_pStEnemyCar[i]->x = rand;
			g_pStEnemyCar[i]->y = 0;
			g_pStEnemyCar[i]->z = 4;
			g_pStEnemyCar[i]->sx = 0;
			g_pStEnemyCar[i]->ex = 0;
			g_pStEnemyCar[i]->sy = 0;
			g_pStEnemyCar[i]->ey = 0;
			g_pStEnemyCar[i]->ubBrakeLights = 0;
			g_pStEnemyCar[i]->ubOBD = 0;
			g_pStEnemyCar[i]->ubAlive = TRUE;
			break;
		}
	}
	ret = i;
	
	return	ret;
}

SS	Put_EnemyCAR(US x, US y, US Size, UC ubMode)
{
	SS	ret = 0;
	SS	i;
	US	w, h;
	UI	uWidth, uHeight, uFileSize;
	UI	uWidth_o, uHeight_o;
	UI	uW_tmp, uH_tmp;
	US	height_sum = 0u;
	US	height_sum_o = 0u;
	
	Get_PicImageInfo( ENEMYCAR_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
	uWidth_o = uWidth;
	uHeight_o = uHeight;
	
	for(i=1; i<=Size; i++)
	{
		/* �k����̃T�C�Y */
		height_sum_o += uHeight_o;
		uW_tmp = uWidth_o << 3;
		uWidth_o = Mmul_p1(uW_tmp);
		uH_tmp = uHeight_o << 3;
		uHeight_o = Mmul_p1(uH_tmp);
		
		/* ���̏k���� */
		height_sum += uHeight;
		uWidth = uWidth_o;
		uHeight = uHeight_o;
	}
	
	w = uWidth;
	h = uHeight;

	ret = G_BitBlt(	x,	w,	y,	h,	0,
					0,	0+w,	height_sum,	h,	0,
					ubMode, POS_MID, POS_CENTER);
	
	return	ret;
}

SS	Sort_EnemyCAR(void)
{
	SS	ret = 0;
	SS	i;
	SS	count = 0;
	ST_ENEMYCARDATA	*pStEnemyCar_Tmp;
	
	while(1)
	{
		for(i=0; i<ENEMYCAR_MAX-1; i++)
		{
			if(g_pStEnemyCar[i]->y > g_pStEnemyCar[i+1]->y)
			{
				pStEnemyCar_Tmp = g_pStEnemyCar[i+1];
				g_pStEnemyCar[i+1] = g_pStEnemyCar[i];
				g_pStEnemyCar[i] = pStEnemyCar_Tmp;
				count = 0;
			}
			else
			{
				count++;
			}
		}
		if(count >= (ENEMYCAR_MAX - 1))
		{
			break;
		}
	}
	
	return ret;
}

#endif	/* ENEMYCAR_C */

