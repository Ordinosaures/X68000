#ifndef	COURSE_OBJ_C
#define	COURSE_OBJ_C

#include <stdio.h>

#include "inc/usr_macro.h"
#include "Course_Obj.h"
#include "OverKata.h"

#include "CRTC.h"
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
SS	GetCourseObj(ST_COURSE_OBJ *, SS);
SS	SetCourseObj(ST_COURSE_OBJ, SS);
SS	Course_Obj_main(UC, UC, UC);
SS	Put_CouseObject(SS, SS, US, UC, UC);
SS	Sort_Course_Obj(void);
SS	Load_Course_Data(UC);
SS	Move_Course_BG(UC);

/* �֐� */
SS	InitCourseObj(void)
{
	SS ret = 0;
	SS i;
	
	for(i=0; i<COURSE_OBJ_MAX; i++)
	{
		g_stCourse_Obj[i].ubType = 0;
		g_stCourse_Obj[i].x = 0;
		g_stCourse_Obj[i].y = (3 * i);
		g_stCourse_Obj[i].z = 0;
		g_stCourse_Obj[i].uTime = 0xFFFF;
		g_stCourse_Obj[i].ubAlive = TRUE;
	}
	
	return ret;
}

SS	GetCourseObj(ST_COURSE_OBJ *stDat, SS Num)
{
	SS	ret = 0;
	if(Num < COURSE_OBJ_MAX)
	{
		*stDat = g_stCourse_Obj[Num];
	}
	else
	{
		ret = -1;
	}
	return ret;
}

SS	SetCourseObj(ST_COURSE_OBJ stDat, SS Num)
{
	SS	ret = 0;
	
	if(Num < COURSE_OBJ_MAX)
	{
		g_stCourse_Obj[Num] = stDat;
	}
	else
	{
		ret = -1;
	}
	return ret;
}

SS Course_Obj_main(UC bNum, UC bMode, UC bMode_rev)
{
	SS	ret = 0;
	US	uCount;

	GetRoadCycleCount(&uCount);

	if(g_stCourse_Obj[bNum].ubAlive == TRUE)
	{
		SS	x, y, z;
		SS	dx, dy, dz;
		SS	mx, my;
		US	ras_x, ras_y, ras_pat, ras_num;
		UC	bEven;
		SS	Out_Of_Disp;
		US	uTime;

		ST_CRT	stCRT;
		ST_RAS_INFO	stRasInfo;
		ST_ROAD_INFO	stRoadInfo;
		
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		UC	bDebugMode;
		GetDebugMode(&bDebugMode);
#endif
		
		GetCRT(&stCRT, bMode);
		GetRasterInfo(&stRasInfo);
		GetRoadInfo(&stRoadInfo);

		if((bNum & 0x01) == 0)
		{
			bEven = TRUE;
		}
		else
		{
			bEven = FALSE;
		}

		/* �O��l */
		x = g_stCourse_Obj[bNum].x;
		y = g_stCourse_Obj[bNum].y;
		z = g_stCourse_Obj[bNum].z;
		uTime = g_stCourse_Obj[bNum].uTime;	/* ���ʃJ�E���^�̑O��l */
		if(uTime == 0xFFFF)
		{
			uTime = uCount;
		}

		if(uCount > uTime)
		{
			y += (uCount - uTime);	/* �ω��� */
		}
		
		ras_num = Mmax(Mmin((y*y)>>4, stRasInfo.ed), RASTER_NEXT);	/* ���X�^�[���̔z��ԍ����Z�o */
		ret = GetRasterIntPos(&ras_x, &ras_y, &ras_pat, stRasInfo.st + ras_num);	/* �z��ԍ��̃��X�^�[���擾 */
		
		/* �Z���^�[ */
		if( ras_x < 256 )	/* ���J�[�u */
		{
			mx = ROAD_CT_POINT + (  0 - ras_x);
		}
		else	/* �E�J�[�u */
		{
			mx = ROAD_CT_POINT + (512 - ras_x);
		}
		
		/* �ʒu */
		if( stRasInfo.st > stRasInfo.mid )
		{
			my = ras_num + ((stRasInfo.st - stRasInfo.mid) << 1);
		}
		else
		{
			my = ras_num;
		}
		
		x = Mmul2(my);	/* 96�ɑ΂���200�Ȃ̂ł��悻�Q�{ */
		
		/* ���� */
		if(bEven == TRUE)	/* �� */
		{
			dx = mx - x - 16;	/* 16�͓��̍ŏ��� */
		}
		else				/* �E */
		{
			dx = mx + x + 16;	/* 16�͓��̍ŏ��� */
		}
		
		z = x;
		
		if( (my > 0) && (ret >= 0) && (dx < 256))
		{
			
			/* ������ */
			dy = stRasInfo.st;
			/* �������e�����œ_�����^�i�œ_�����{Z�ʒu�j���Q�T�U�{���ĂU�S�Ŋ�����(/4pat) */
			//dz = Mmin( Mmax( 3 - (((z<<8) / (z + ROAD_ED_POINT))>>5) , 0), 3 );
			/* �������e�����œ_�����^�i�œ_�����{Z�ʒu�j���Q�T�U�{����32�Ŋ�����(=7pat) */
			dz = Mmin( Mmax( 7 - (((z<<8) / (z + ROAD_ED_POINT))>>4) , 0), 7 );
			/* �`�� */
			Out_Of_Disp = Put_CouseObject(	stCRT.hide_offset_x + dx,
											stCRT.hide_offset_y + dy,
											dz,
											bMode_rev,
											bEven);
			x = dx;
			z = dz;

			if(Out_Of_Disp < 0)	/* �`��̈�O */
			{
				/* �o���|�C���g�ŕ`��̈�O�ƂȂ�̂ŗv���� */
//				x = 0;
//				y = 0;
//				z = 0;
//				g_stCourse_Obj[bNum].ubAlive = FALSE;
			}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
			if( bDebugMode == TRUE )
			{
				Draw_Pset(	stCRT.hide_offset_x + dx,
							stCRT.hide_offset_y + ras_y, 0xC2);	/* �f�o�b�O�p���W�ʒu */
			}
#endif
		}
		else
		{
			if(dx >= 448)	/* 512����64�������l�i64��菬�����Ə����ɂ����j */
			{
				x = 0;
				y = 0;
				z = 0;
				g_stCourse_Obj[bNum].ubAlive = FALSE;
			}
		}

		g_stCourse_Obj[bNum].x = x;
		g_stCourse_Obj[bNum].y = y;
		g_stCourse_Obj[bNum].z = z;
		g_stCourse_Obj[bNum].uTime = uCount;
	}
	else
	{
		g_stCourse_Obj[bNum].ubType = 0;
		g_stCourse_Obj[bNum].x = 0;
		g_stCourse_Obj[bNum].y = 0;
		g_stCourse_Obj[bNum].z = 0;
		g_stCourse_Obj[bNum].uTime = uCount;
		g_stCourse_Obj[bNum].ubAlive = TRUE;
	}
	
	return ret;
}

SS	Put_CouseObject(SS x, SS y, US Size, UC ubMode, UC ubPos)
{
	SS	ret = 0;
	SS	i;
	US	w, h;
	UC	ubType;
	UC	ubPos_H;
	UI	uWidth, uHeight, uFileSize;
	UI	uWidth_o, uHeight_o;
	UI	uW_tmp, uH_tmp;
	US	height_sum = 0u;
	US	height_sum_o = 0u;
	
	ubType = g_stCourse_Obj[0].ubType;
	
	switch(ubType)
	{
	case 0:
		{
			/* COURSE_OBJ_CG(4) */
			Get_PicImageInfo( COURSE_OBJ_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
			/* 9 */
			//Get_PicImageInfo( COURSE_OBJ_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
		}
		break;
	case 1:
		{
			/* COURSE_OBJ_CG(4) */
			Get_PicImageInfo( COURSE_OBJ_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
		}
		break;
	default:
		{
			/* COURSE_OBJ_CG(4) */
			Get_PicImageInfo( COURSE_OBJ_CG, &uWidth, &uHeight, &uFileSize);	/* �C���[�W���̎擾 */
		}
		break;
	}
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
	
	if(ubPos == TRUE)	/* �� */
	{
		ubPos_H = POS_RIGHT;
	}
	else				/* �E */
	{
		ubPos_H = POS_LEFT;
	}
	
	ret = G_BitBlt(	x,		w,			y,	h,	0,
					140,	w,	height_sum,	h,	0,
					ubMode, ubPos_H, POS_CENTER);
	
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

SS	Move_Course_BG(UC bMode)
{
	SS	ret = 0;
	
	SS	Slope;
	SS	Angle;
	SS	Move;
	ST_RAS_INFO	stRasInfo;
	ST_ROAD_INFO	stRoadInfo;
	ST_CARDATA	stMyCar = {0};
	static SS	BG_x = X_OFFSET;
	
	GetRasterInfo(&stRasInfo);
	GetRoadInfo(&stRoadInfo);
	Slope = stRoadInfo.slope;
	Angle = stRoadInfo.angle;

	GetMyCar(&stMyCar);			/* ���Ԃ̏����擾 */
	
	if(stMyCar.VehicleSpeed != 0u)
	{
		if(Angle != 0)
		{
			if(stMyCar.ubThrottle != 0u)
			{
				BG_x += 0 - (Angle << 1);
			}
			else
			{
				BG_x += 0 - (Angle << 0);
			}
		}
		else
		{
			/* �ێ� */
		}
	}
	BG_x = BG_x & 0x3FF;
	
	/* �w�i�𓮂��� */
	Move = 65 + 8 - stRasInfo.st;	/* �w�i�̍��� + �I�t�Z�b�g - �����ʒu */
	if(Move < 0)
	{
		Move = Move << 2;
	}
	if(bMode == 1)
	{
		G_Scroll(BG_x, Y_MIN_DRAW +        0 + Move, 1);
	}
	else
	{
		G_Scroll(BG_x, Y_MIN_DRAW + Y_OFFSET + Move, 1);
	}
	
	return ret;
}


#endif	/* COURSE_OBJ_C */

