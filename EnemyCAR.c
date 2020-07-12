#ifndef	ENEMYCAR_C
#define	ENEMYCAR_C

#include "inc/usr_macro.h"
#include "EnemyCAR.h"
#include "Graphic.h"
#include "MyCar.h"

/* �\���̒�` */
ST_ENEMYCARDATA	stEnemyCar[ENEMYCAR_MAX] = {0};

/* �֐��̃v���g�^�C�v�錾 */
SS	InitEnemyCAR(void);
SS	GetEnemyCAR(ST_ENEMYCARDATA *, SS);
SS	SetEnemyCAR(ST_ENEMYCARDATA, SS);
SS	EnemyCAR_main(UC, UC, UC);
SS	SetAlive_EnemyCAR(void);
SS	Put_EnemyCAR(US, US, US, UC);

/* �֐� */
SS	InitEnemyCAR(void)
{
	SS ret = 0;
	SS i;
	
	for(i=0; i<ENEMYCAR_MAX; i++)
	{
		stEnemyCar[i].ubCarType = 0;
		stEnemyCar[i].VehicleSpeed = 120;
		stEnemyCar[i].x = 0;
		stEnemyCar[i].y = 0;
		stEnemyCar[i].z = 4;
		stEnemyCar[i].ubBrakeLights = 0;
		stEnemyCar[i].ubOBD = 0;
		stEnemyCar[i].ubAlive = FALSE;
	}
	
	return ret;
}

SS	GetEnemyCAR(ST_ENEMYCARDATA *stDat, SS Num)
{
	SS	ret = 0;
	if(Num < ENEMYCAR_MAX)
	{
		*stDat = stEnemyCar[Num];
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
		stEnemyCar[Num] = stDat;
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
		if(stEnemyCar[bNum].ubAlive == TRUE)
		{
			SS	x, y, z;
			US	ras_x, ras_y;
			ST_CARDATA	stMyCar;
			ST_CRT		stCRT;
			ST_RAS_INFO	stRasInfo;

			ras_x = 0;
			ras_y = 0;
			
			x = stEnemyCar[bNum].x;
			y = stEnemyCar[bNum].y;
			z = stEnemyCar[bNum].z;

			GetCRT(&stCRT, bMode);
			GetMyCar(&stMyCar);

			//y += ((stMyCar.VehicleSpeed - stEnemyCar[bNum].VehicleSpeed) / 10);
			y += RASTER_NEXT;

			GetRasterInfo(&stRasInfo);
			GetRasterPos(&ras_x, &ras_y, (US)(stRasInfo.st + y));
			x = ras_x;
			z = ((4 * 6) / (6 + y));
			
			stEnemyCar[bNum].x = x;
			stEnemyCar[bNum].y = y;
			stEnemyCar[bNum].z = z;
			
			if((stRasInfo.st + y) < (stRasInfo.ed - 16))
			{
				Put_EnemyCAR(	stCRT.hide_offset_x + (WIDTH>>1) - x,
								stCRT.hide_offset_y + stRasInfo.horizon + y,
								z,
								bMode_rev);
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
				{
					Draw_Pset(	stCRT.hide_offset_x + (WIDTH>>1) - x,
								stCRT.hide_offset_y + stRasInfo.horizon + y,
								0xBE);	/* �f�o�b�O�p */
				}
#endif
			}
			else
			{
				stEnemyCar[bNum].ubAlive = FALSE;
			}
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

	for(i=0; i<ENEMYCAR_MAX; i++)
	{
		if(stEnemyCar[i].ubAlive == FALSE)
		{
			stEnemyCar[i].ubCarType = 0;
			stEnemyCar[i].VehicleSpeed = 120;
			stEnemyCar[i].x = 0;
			stEnemyCar[i].y = 0;
			stEnemyCar[i].z = 4;
			stEnemyCar[i].ubBrakeLights = 0;
			stEnemyCar[i].ubOBD = 0;
			stEnemyCar[i].ubAlive = TRUE;
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
	US	height_sum = 0u;
	
	w = ENEMY_CAR_1_W >> Size;
	h = ENEMY_CAR_1_H >> Size;

	for(i = 1; i <= Size; i++)
	{
		height_sum += (ENEMY_CAR_1_H >> (i-1));
	}

	G_BitBlt(	x,	w,					y,	h,	1,
				0,	0+w,	height_sum,	h,	1,
				ubMode, POS_MID, POS_BOTTOM);
	
	return	ret;
}

#endif	/* ENEMYCAR_C */

