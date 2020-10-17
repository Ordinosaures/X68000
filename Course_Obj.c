#ifndef	COURSE_OBJ_C
#define	COURSE_OBJ_C

#include <stdio.h>

#include "inc/usr_macro.h"
#include "Course_Obj.h"
#include "Draw.h"
#include "FileManager.h"
#include "Graphic.h"
#include "MFP.h"
#include "MyCar.h"
#include "Output_Text.h"
#include "Raster.h"

/* �ϐ���` */

/* �\���̒�` */
static ST_COURSE_OBJ	g_stCourse_Obj[COURSE_OBJ_MAX] = {0};

/* �֐��̃v���g�^�C�v�錾 */
SS	InitCourseObj(void);
SS	Course_Obj_main(UC, UC, UC);
SS	Put_CouseObject(SS, SS, US, UC, UC);
SS	Sort_Course_Obj(void);
SS	Load_Course_Data(UC);

/* �֐� */
SS	InitCourseObj(void)
{
	SS ret = 0;
	SS i;
	
	for(i=0; i<COURSE_OBJ_MAX; i++)
	{
		g_stCourse_Obj[i].ubType = 0;
		g_stCourse_Obj[i].x = 48*(i/2);
		g_stCourse_Obj[i].y = 48*(i/2);
		g_stCourse_Obj[i].z = 0;
		g_stCourse_Obj[i].ubAlive = TRUE;
	}
	
	return ret;
}

SS Course_Obj_main(UC bNum, UC bMode, UC bMode_rev)
{
	SS	ret = 0;
	US	uCount;

	GetRoadCycleCount(&uCount);
	uCount &= 0xFFF;

	if(g_stCourse_Obj[bNum].ubAlive == TRUE)
	{
		SS	x, y, z;
		SS	dx, dy, dz;
		SS	x_ofst = 0;
		US	ras_x, ras_y, ras_pat, ras_num;
		UC	bEven;
		SS	Out_Of_Disp;

		ST_CRT	stCRT;
		ST_RAS_INFO	stRasInfo;

		GetCRT(&stCRT, bMode);
		GetRasterInfo(&stRasInfo);

		if((bNum % 2) == 0)
		{
			bEven = TRUE;
		}
		else
		{
			bEven = FALSE;
		}

		x = g_stCourse_Obj[bNum].x;
		y = g_stCourse_Obj[bNum].y;
		z = g_stCourse_Obj[bNum].z;	/* ���ʃJ�E���^�̑O��l */


		if(uCount  != z)	/* ���H�X�V���� */
		{
			if(uCount >= z)
			{
				y += uCount - z;			/* �ω��� */
			}
			else
			{
				y += (0xFFF - z) + uCount;	/* �ω��� */
			}
			
		}
		else
		{
			/* nop */
		}
		ras_num = (US)(stRasInfo.st + y);
		
		x = 4 * (y + (ras_num - 80));

		GetRasterIntPos(&ras_x, &ras_y, &ras_pat, ras_num);
		if(ras_x >= 256)
		{
			x_ofst = (SS)ras_x - 512;
		}

		g_stCourse_Obj[bNum].x = x;
		g_stCourse_Obj[bNum].y = y;
		
		/* 2���֐��I�ɑ��₷�v�f�{���ߖ@�ő��₷�v�f */
		x = x - x_ofst;/*+ (ras_num/4)*/ /*+ ((96 * 16) / (Mmax((96-dx),0) + 16))*/
		
		if(bEven == TRUE)	/* �� */
		{
//			x = 0 - (dy * 3) - ras_x - 48;
			dx = 0 - x;
		}
		else				/* �E */
		{
			dx = x;
		}
		dy = y;

		/* �\���͈� */
		dz = (128 - x_ofst) + PINETREE_1_W;
		
		Out_Of_Disp = Put_CouseObject(
			stCRT.hide_offset_x + (WIDTH>>1) + dx,
			stCRT.hide_offset_y + stRasInfo.horizon,
			//((32) / (8 + dy)),
			Mmin( Mmax( 3 - (((x<<8) / (x + dz))>>5) , 0), 3 ),	/* �������e�����œ_�����^�i�œ_�����{Z�ʒu�j���Q�T�U�{���ĂU�S�Ŋ����� */
			bMode_rev,
			bEven);

		if(Out_Of_Disp < 0)
		{
			g_stCourse_Obj[bNum].ubAlive = FALSE;
		}

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		if( bNum == 0 )
		{
	//		Message_Num(&g_stCourse_Obj[bNum].x,	 0,  9, 2, MONI_Type_SS, "%4d");
	//		Message_Num(&g_stCourse_Obj[bNum].y, 	 6,  9, 2, MONI_Type_SS, "%4d");
	//		Message_Num(&g_stCourse_Obj[bNum].z, 	12,  9, 2, MONI_Type_SS, "%4d");
	//		Message_Num(&ras_x, 			 0, 10, 2, MONI_Type_US, "%4d");
	//		Message_Num(&ras_y, 			 6, 10, 2, MONI_Type_US, "%4d");
	//		Message_Num(&x_ofst, 			 0, 11, 2, MONI_Type_SS, "%4d");
	//		Message_Num(&ras_num,			 6, 11, 2, MONI_Type_US, "%4d");
	//		Message_Num(&bNum,				12, 11, 2, MONI_Type_UC, "%d");
	//		if(bDebugMode == TRUE)	/* �f�o�b�O���[�h */
	//		{
	//			Draw_Pset(x, dy, 0xC2);	/* �f�o�b�O�p���W�ʒu */
	//		}
		}
#endif
	}
	else
	{
		g_stCourse_Obj[bNum].ubType = 0;
		g_stCourse_Obj[bNum].x = 0;
		g_stCourse_Obj[bNum].y = 0;
		g_stCourse_Obj[bNum].ubAlive = TRUE;
	}
	g_stCourse_Obj[bNum].z = uCount;
	
	return ret;
}

SS	Put_CouseObject(SS x, SS y, US Size, UC ubMode, UC ubPos)
{
	SS	ret = 0;
	SS	i;
	SS	dx, dy;
	US	w, h;
	US	height_sum = 0u;
	
	w = PINETREE_1_W >> Size;
	h = PINETREE_1_H >> Size;

	dx = x;
//	dy = y - (h >> 1);
	dy = y;
	
	for(i = 1; i <= Size; i++)
	{
		height_sum += (PINETREE_1_H >> (i-1));
	}
	
	ret = G_BitBlt(	dx,		w,			dy,	h,	1,
					140,	w,	height_sum,	h,	1,
					ubMode, POS_MID, POS_CENTER);
	
	return	ret;
}

SS	Sort_Course_Obj(void)
{
	SS	ret = 0;
	SS	i;
	SS	count = 0;
	ST_COURSE_OBJ	stTmp;
	
	while(1)
	{
		for(i=0; i<COURSE_OBJ_MAX-1; i++)
		{
			if(g_stCourse_Obj[i].y > g_stCourse_Obj[i+1].y)
			{
				stTmp = g_stCourse_Obj[i+1];
				g_stCourse_Obj[i+1] = g_stCourse_Obj[i];
				g_stCourse_Obj[i] = stTmp;
				count = 0;
			}
			else
			{
				count++;
			}
		}
		if(count >= (COURSE_OBJ_MAX - 1))
		{
			break;
		}
	}
	
	return ret;
}

SS	Load_Course_Data(UC bCourseNum)
{
	SS	ret = 0;
	SC	str[256];
	US	x, y;
	ST_ROADDATA *p_stRoadData;
	
	/* �R�[�X�f�[�^�ǂݍ��� */
	p_stRoadData = (ST_ROADDATA *)GetRoadDataAddr();
	sprintf(str, "data/map/course%02d.csv", bCourseNum);
	File_Load_Course_CSV(str, p_stRoadData, &x, &y);

	return ret;
}

#endif	/* COURSE_OBJ_C */

