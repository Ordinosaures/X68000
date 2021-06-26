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

/* define��` */
#define	PCG_16x16_AREA	(0x40)

/* �O���[�o���ϐ� */
static ST_PCG	g_stPCG_DATA[PCG_MAX] = {0};

/* �֐��̃v���g�^�C�v�錾 */
void PCG_INIT(void);
void PCG_VIEW(uint8_t);
void PCG_Rotation(uint16_t *, uint16_t *, uint8_t, uint8_t, int16_t, int16_t, uint8_t *, uint8_t, uint16_t, uint16_t);
void BG_TEXT_SET(int8_t *);
int16_t PCG_Main(void);
ST_PCG *PCG_Get_Info(uint8_t);
int16_t PCG_Set_Info(ST_PCG, uint8_t);
int16_t PCG_Load_Data(int8_t *, uint16_t , ST_PCG , uint16_t , uint8_t );
int16_t PCG_Load_PAL_Data(int8_t *, uint16_t, uint16_t);
/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void PCG_INIT(void)
{
	uint32_t	j;
	uint32_t	uPCG_num, uBufSize;
	uint8_t		ubOK;
	
	/* �X�v���C�g�̏����� */
	SP_INIT();			/* �X�v���C�g�̏����� */
	
	/*�X�v���C�g���W�X�^������*/
	for( j = 0x80000000; j < 0x80000000 + 128; j++ )
	{
		SP_REGST(j,-1,0,0,0,0);
	}
#if 0
	for(uint32_t i = 0; i < 256; i++ )
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

	/* �X�v���C�g�Ǘ��p�o�b�t�@�̃N���A */
	for(uPCG_num = 0; uPCG_num < PCG_MAX; uPCG_num++)
	{
		memset(&g_stPCG_DATA[uPCG_num], 0, sizeof(ST_PCG) );
	}
	
#if 0
	for(uPCG_num = 0; uPCG_num < PCG_MAX; uPCG_num++)
	{
		g_stPCG_DATA[uPCG_num].x				= (uPCG_num * 16) & 0xFF;	/* x���W */
		g_stPCG_DATA[uPCG_num].y				= (uPCG_num * 16) & 0xFF;	/* y���W */
		g_stPCG_DATA[uPCG_num].ratio			= 0;			/* �䗦 */
		g_stPCG_DATA[uPCG_num].rad			= 0;			/* ��] */
		g_stPCG_DATA[uPCG_num].Pat_w			= 1;			/* �������̃p�^�[���� */
		g_stPCG_DATA[uPCG_num].Pat_h			= 1;			/* �c�����̃p�^�[���� */
		g_stPCG_DATA[uPCG_num].pPatCodeTbl	= malloc(sizeof(uint32_t));
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl)	= SetBGcode(0, 0, 0x0C, uPCG_num);	/* �p�^�[���R�[�h�e�[�u�� */
		g_stPCG_DATA[uPCG_num].update		= TRUE;			/* �X�V */
		g_stPCG_DATA[uPCG_num].validty		= TRUE;		/* �L�� */
	}
#endif
	
#if 0
	for(uPCG_num = 0; uPCG_num < 14; uPCG_num++)
	{
		g_stPCG_DATA[uPCG_num].x				= 16 * uPCG_num;	/* x���W */
		g_stPCG_DATA[uPCG_num].y				= 128;	/* y���W */
		g_stPCG_DATA[uPCG_num].ratio			= 0;	/* �䗦 */
		g_stPCG_DATA[uPCG_num].rad			= 0;	/* ��] */
		g_stPCG_DATA[uPCG_num].Pat_w			= 3;	/* �������̃p�^�[���� */
		g_stPCG_DATA[uPCG_num].Pat_h			= 3;	/* �c�����̃p�^�[���� */
		g_stPCG_DATA[uPCG_num].Pat_size		= 1;	/* �p�^�[���T�C�Y 0:8x8 1:16x16 */
		g_stPCG_DATA[uPCG_num].pPatCodeTbl	= malloc(sizeof(uint32_t) * g_stPCG_DATA[uPCG_num].Pat_w * g_stPCG_DATA[uPCG_num].Pat_h);
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + 0)	= SetBGcode(0, 0, 0x09, 0x40);	/* �p�^�[���R�[�h�e�[�u�� */
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + 1)	= SetBGcode(0, 0, 0x09, 0x41);	/* �p�^�[���R�[�h�e�[�u�� */
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + 2)	= SetBGcode(0, 0, 0x09, 0x42);	/* �p�^�[���R�[�h�e�[�u�� */
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + 3)	= SetBGcode(0, 0, 0x09, 0x50);	/* �p�^�[���R�[�h�e�[�u�� */
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + 4)	= SetBGcode(0, 0, 0x09, 0x51);	/* �p�^�[���R�[�h�e�[�u�� */
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + 5)	= SetBGcode(0, 0, 0x09, 0x52);	/* �p�^�[���R�[�h�e�[�u�� */
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + 6)	= SetBGcode(0, 0, 0x09, 0x60);	/* �p�^�[���R�[�h�e�[�u�� */
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + 7)	= SetBGcode(0, 0, 0x09, 0x61);	/* �p�^�[���R�[�h�e�[�u�� */
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + 8)	= SetBGcode(0, 0, 0x09, 0x62);	/* �p�^�[���R�[�h�e�[�u�� */
		g_stPCG_DATA[uPCG_num].update		= TRUE;		/* �X�V */
		g_stPCG_DATA[uPCG_num].validty		= TRUE;		/* �L�� */
	}
#endif
	
#if 0
	for(uPCG_num = 0; uPCG_num < PCG_NUM_MAX; uPCG_num++)
	{
		FILE *fp;

		fp = fopen( "data/sp/BG.SP" , "rb" ) ;
		
		g_stPCG_DATA[uPCG_num].x			= SP_X_OFFSET + ((SP_W/2) * uPCG_num);	/* x���W */
		g_stPCG_DATA[uPCG_num].y			= SP_Y_OFFSET + 128;	/* y���W */
		g_stPCG_DATA[uPCG_num].Pat_w		= 1;	/* �������̃p�^�[���� */
		g_stPCG_DATA[uPCG_num].Pat_h		= 1;	/* �c�����̃p�^�[���� */
		g_stPCG_DATA[uPCG_num].Pat_AnimeMax	= 1;
		/* �������m�� */
		uBufSize = g_stPCG_DATA[uPCG_num].Pat_w * g_stPCG_DATA[uPCG_num].Pat_h;
		/* �X�v���C�g��`�ݒ� */
		g_stPCG_DATA[uPCG_num].pPatCodeTbl	= (uint32_t*)MyMalloc(sizeof(uint32_t) * uBufSize);
		for(j=0; j < uBufSize; j++)
		{
			*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x00, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
		}
		/* �X�v���C�g�摜�f�[�^ */
		uAnimeBuf = uBufSize * g_stPCG_DATA[uPCG_num].Pat_AnimeMax;
		g_stPCG_DATA[uPCG_num].pPatData	= (uint16_t*)MyMalloc(SP_16_SIZE * uAnimeBuf);
		memset(g_stPCG_DATA[uPCG_num].pPatData, 0, SP_16_SIZE * uAnimeBuf);
		
		fseek(fp, (0x20 * SP_16_SIZE) + (uPCG_num * (SP_16_SIZE / 2)), SEEK_SET);
		fread( g_stPCG_DATA[uPCG_num].pPatData
				,  (SP_16_SIZE / 2)		/* 1PCG = �������T�C�Y */
				,  uAnimeBuf			/* ���p�^�[���g���̂��H */
				,  fp
				);
		
		fclose( fp ) ;
		
		g_stPCG_DATA[uPCG_num].Plane		= 0xFF;		/* �v���[��No. */
		g_stPCG_DATA[uPCG_num].update		= TRUE;		/* �X�V */
		g_stPCG_DATA[uPCG_num].validty		= TRUE;		/* �L�� */
	}
#endif
	
#if 1
	srand(1);
	
	/* ���ԃX�v���C�g */
	for(uPCG_num = 0; uPCG_num < PCG_NUM_MAX; uPCG_num++)
	{
		ST_PCG stPCG;
		
		/* �����܂��������͕s�v */
		stPCG.x			= SP_X_OFFSET + (rand() % 256);	/* x���W */
		stPCG.y			= SP_Y_OFFSET + (rand() % 256);	/* y���W */
		stPCG.dx		= rand() % 2;	/* �ړ���x */
		stPCG.dy		= rand() % 2;	/* �ړ���y */
		stPCG.Anime		= 0;	/* ���݂̃A�j�� */
		stPCG.Anime_old	= 0;	/* �O��̃A�j�� */
		
		switch(uPCG_num)
		{
			case MYCAR_PCG_NEEDLE:	/* �^�R���[�^�[�j */
			{
				stPCG.Pat_w		= 1;
				stPCG.Pat_h		= 1;
				stPCG.Pat_AnimeMax	= 32;
				stPCG.Pat_DataMax	= 8;
				
				PCG_Load_Data("data/sp/BG.SP", 0x40, stPCG, uPCG_num, 0);
				PCG_Load_PAL_Data("data/sp/BG.PAL", 0x0D, 0x0D);
				
				uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;

				/* �X�v���C�g��`�ݒ� */
				for(j=0; j < uBufSize; j++)
				{
					if(j < 8)
						*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(1, 1, 0x0D, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
					else if(j < 16)
						*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 1, 0x0D, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
					else if(j < 24)
						*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x0D, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
					else
						*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(1, 0, 0x0D, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
				}
				
				ubOK = TRUE;
				break;
			}
			case MYCAR_PCG_TACHO:	/* �^�R���[�^�[ */
			{
				stPCG.Pat_w		= 2;
				stPCG.Pat_h		= 2;
				stPCG.Pat_AnimeMax	= 1;
				stPCG.Pat_DataMax	= 4;
				
				PCG_Load_Data("data/sp/BG.SP", 0x48, stPCG, uPCG_num, 0);
				PCG_Load_PAL_Data("data/sp/BG.PAL", 0x0D, 0x0D);
				
				uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;

				/* �X�v���C�g��`�ݒ� */
				for(j=0; j < uBufSize; j++)
				{
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x0D, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
				}

				ubOK = TRUE;
				break;
			}
			case MYCAR_PCG_STEERING_POS:	/* �X�e�A�����O�|�W�V���� */
			{
				stPCG.Pat_w		= 1;
				stPCG.Pat_h		= 1;
				stPCG.Pat_AnimeMax	= 1;
				stPCG.Pat_DataMax	= 1;
				
				PCG_Load_Data("data/sp/BG.SP", 0x3F, stPCG, uPCG_num, 0);
				PCG_Load_PAL_Data("data/sp/BG.PAL", 0x0C, 0x0C);
				
				uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;
				/* �X�v���C�g��`�ݒ� */
				for(j=0; j < uBufSize; j++)
				{
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x0C, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
				}
				
				ubOK = TRUE;
				break;
			}
			case MYCAR_PCG_STEERING:	/* �X�e�A�����O */
			{
				stPCG.Pat_w			= 2;
				stPCG.Pat_h			= 2;
				stPCG.Pat_AnimeMax	= 1;
				stPCG.Pat_DataMax	= 4;
				
				PCG_Load_Data("data/sp/BG.SP", 0x4C, stPCG, uPCG_num, 0);
				PCG_Load_PAL_Data("data/sp/BG.PAL", 0x0B, 0x0B);
				
				uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;

				/* �X�v���C�g��`�ݒ� */
				for(j=0; j < uBufSize; j++)
				{
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x0B, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
				}
				
				ubOK = TRUE;
				break;
			}
			case SCORE_PCG_1:	/* �X�R�A�P */
			case SCORE_PCG_2:	/* �X�R�A�Q */
			case SCORE_PCG_3:	/* �X�R�A�R */
			case SCORE_PCG_4:	/* �X�R�A�S */
			{
				stPCG.Pat_w		= 1;
				stPCG.Pat_h		= 1;
				stPCG.Pat_AnimeMax	= 10;
				stPCG.Pat_DataMax	= 10;
				
				PCG_Load_Data("data/sp/BG.SP", 0x20, stPCG, uPCG_num, 1);
				/* �e�L�X�g���p�̂O�ԃp���b�g���g�p */
				
				uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;

				/* �X�v���C�g��`�ݒ� */
				for(j=0; j < uBufSize; j++)
				{
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x00, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
				}
				
				ubOK = TRUE;
				break;
			}
			case ROAD_PCG_ARROW:				/* ��� */
			{
				stPCG.Pat_w		= 1;
				stPCG.Pat_h		= 1;
				stPCG.Pat_AnimeMax	= 6;
				stPCG.Pat_DataMax	= 6;
				
				PCG_Load_Data("data/sp/BG.SP", 0xB0, stPCG, uPCG_num, 0);
				PCG_Load_PAL_Data("data/sp/MASUO.PAL", 0x01, 0x07);
				
				uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;
				/* �X�v���C�g��`�ݒ� */
				for(j=0; j < uBufSize; j++)
				{
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x07, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
				}
				
				ubOK = TRUE;
				break;
			}
			case ETC_PCG_SONIC:				/* �\�j�b�N */
			{
				stPCG.Pat_w			= 3;
				stPCG.Pat_h			= 3;
				stPCG.Pat_AnimeMax	= 1;
				stPCG.Pat_DataMax	= 9;
				
				PCG_Load_Data("data/sp/BG.SP", 0xA0, stPCG, uPCG_num, 0);
				PCG_Load_PAL_Data("data/sp/BG.PAL", 0x09, 0x09);
				
				uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;

				/* �X�v���C�g��`�ݒ� */
				for(j=0; j < uBufSize; j++)
				{
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x09, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
				}
				
				ubOK = TRUE;
				break;
			}
			case ETC_PCG_KISARA:				/* �L�T�� */
			{
				stPCG.Pat_w			= 2;
				stPCG.Pat_h			= 4;
				stPCG.Pat_AnimeMax	= 1;
				stPCG.Pat_DataMax	= 8;
				
				PCG_Load_Data("data/sp/BG.SP", 0x90, stPCG, uPCG_num, 0);
				PCG_Load_PAL_Data("data/sp/MASUO.PAL", 0x01, 0x07);
				
				uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;

				/* �X�v���C�g��`�ݒ� */
				for(j=0; j < uBufSize; j++)
				{
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x07, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
				}
				
				ubOK = TRUE;
				break;
			}
			case ETC_PCG_MAN:				/* �s�N�g�N */
			{
				stPCG.x			= SP_X_OFFSET + 128;	/* x���W */
				stPCG.y			= SP_Y_OFFSET + 128;	/* y���W */
				stPCG.Anime		= 0;	/* ���݂̃A�j�� */
				stPCG.Anime_old	= 0;	/* �O��̃A�j�� */
				stPCG.Pat_w		= 2;
				stPCG.Pat_h		= 4;
				stPCG.Pat_AnimeMax	= 8;
				stPCG.Pat_DataMax	= 8*8;
				
				PCG_Load_Data("data/sp/BG.SP", 0x50, stPCG, uPCG_num, 0);
				PCG_Load_PAL_Data("data/sp/MASUO.PAL", 0x01, 0x07);
				
				uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;

				/* �X�v���C�g��`�ݒ� */
				for(j=0; j < uBufSize; j++)
				{
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x07, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
				}
				
				ubOK = TRUE;
				break;
			}
			default:
			{

				ubOK = FALSE;
				break;
			}
		}
		
		if(ubOK == TRUE)
		{
			g_stPCG_DATA[uPCG_num].update	= TRUE;
			g_stPCG_DATA[uPCG_num].validty	= TRUE;
		}
		else
		{
			g_stPCG_DATA[uPCG_num].Pat_w		= 0;
			g_stPCG_DATA[uPCG_num].Pat_h		= 0;
			g_stPCG_DATA[uPCG_num].update	= FALSE;
			g_stPCG_DATA[uPCG_num].validty	= FALSE;
		}
		g_stPCG_DATA[uPCG_num].Plane		= 0xFF;		/* �v���[��No. */
#ifdef DEBUG
//		printf("g_stPCG_DATA[%d]=0x%p\n", uPCG_num, &g_stPCG_DATA[uPCG_num]);
//		KeyHitESC();	/* �f�o�b�O�p */
#endif
	}
#endif

}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void PCG_VIEW(uint8_t bSW)
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

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void PCG_Rotation(uint16_t *pDst, uint16_t *pSrc, uint8_t bX_num, uint8_t bY_num, int16_t pos_x, int16_t pos_y, uint8_t *sp_num, uint8_t palNum, uint16_t ratio, uint16_t rad)
{
	int16_t	x, y, vx, vy;
	int16_t	width, height;
	uint64_t	uPCG;
	uint64_t	uPCG_ARY[8];
	uint64_t	*pDstWork, *pSrcWork;
	uint8_t	*pBuf, *pDstBuf = NULL, *pSrcBuf = NULL;
	uint64_t	uADDR;
	uint64_t	uPointer_ADR, uPointer_ADR_X, uPointer_ADR_Y, uPointer_ADR_subX, uPointer_ADR_subY;
	uint16_t	uMem_size;
	uint8_t	bShift;
	uint64_t	code = 0;
	uint8_t	V=0, H=0;
	uint8_t	spNum;	
//	int8_t	bEx_num=0;
	
	/* src size */
	width =  16 * (int16_t)bX_num;
	height = 16 * (int16_t)bY_num;
	uMem_size = width * height * sizeof(uint8_t);
	
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
	pSrcBuf = (uint8_t*)MyMalloc( (int32_t)uMem_size );
	if( pSrcBuf == NULL )
	{
		return;
	}
	pBuf = pSrcBuf;

	/* PCG -> CG */
	uPointer_ADR = (uint64_t)pSrc;
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
					pSrcWork = (uint64_t *)uADDR;	/* 0��2/1��3 */

					uPCG = (uint64_t)*pSrcWork;
					
					*pBuf = (uint8_t)((uPCG >> 28ul) & 0x0Fu);
					pBuf++;
					*pBuf = (uint8_t)((uPCG >> 24ul) & 0x0Fu);
					pBuf++;
					*pBuf = (uint8_t)((uPCG >> 20ul) & 0x0Fu);
					pBuf++;
					*pBuf = (uint8_t)((uPCG >> 16ul) & 0x0Fu);
					pBuf++;
					*pBuf = (uint8_t)((uPCG >> 12ul) & 0x0Fu);
					pBuf++;
					*pBuf = (uint8_t)((uPCG >>  8ul) & 0x0Fu);
					pBuf++;
					*pBuf = (uint8_t)((uPCG >>  4ul) & 0x0Fu);
					pBuf++;
					*pBuf = (uint8_t)((uPCG        ) & 0x0Fu);
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
	width =  16 * Mmax(((int16_t)bX_num + bEx_num), 1);
	height = 16 * Mmax(((int16_t)bY_num + bEx_num), 1);
	uMem_size = width * height * sizeof(uint8_t);
#endif
	
	pDstBuf = (uint8_t*)MyMalloc( (int32_t)uMem_size );
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
		int16_t	dx, dy;
		int16_t	width_h, height_h;
		int16_t	cos, sin;
		uint8_t	*pDstGR, *pSrcGR;
		
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

	uPointer_ADR = (uint64_t)pDst;
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
				
					uPCG_ARY[7] = (uint64_t)*pBuf;
					pBuf++;
					uPCG_ARY[6] = (uint64_t)*pBuf;
					pBuf++;
					uPCG_ARY[5] = (uint64_t)*pBuf;
					pBuf++;
					uPCG_ARY[4] = (uint64_t)*pBuf;
					pBuf++;
					uPCG_ARY[3] = (uint64_t)*pBuf;
					pBuf++;
					uPCG_ARY[2] = (uint64_t)*pBuf;
					pBuf++;
					uPCG_ARY[1] = (uint64_t)*pBuf;
					pBuf++;
					uPCG_ARY[0] = (uint64_t)*pBuf;
					pBuf++;
					
					uPCG = 	((uPCG_ARY[7] << 28ul) & 0xF0000000ul) |
							((uPCG_ARY[6] << 24ul) & 0x0F000000ul) |
							((uPCG_ARY[5] << 20ul) & 0x00F00000ul) |
							((uPCG_ARY[4] << 16ul) & 0x000F0000ul) |
							((uPCG_ARY[3] << 12ul) & 0x0000F000ul) |
							((uPCG_ARY[2] <<  8ul) & 0x00000F00ul) |
							((uPCG_ARY[1] <<  4ul) & 0x000000F0ul) |
							((uPCG_ARY[0]        ) & 0x0000000Ful);

					pDstWork = (uint64_t *) uADDR;
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
	uPointer_ADR = (uint64_t)pDst;
	uPointer_ADR_Y = 0ul;
	spNum = *sp_num;
	
	for(y = 0; y < bY_num; y++)	/* ��s�� */
	{
		uPointer_ADR_X = 0ul;

		for(x = 0; x < bX_num; x++)	/* ��� */
		{
			uint8_t	patNum;
			patNum = (uint8_t)( (uint64_t) ( (uPointer_ADR - 0xEB8000ul) + uPointer_ADR_Y + uPointer_ADR_X ) / 0x80ul); /*0x43*/;
			code = (uint64_t)( 0xCFFFU & ( ((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((palNum & 0xFU)<<8U) | (patNum & 0xFFU) ) );
			SP_REGST( spNum++, -1, pos_x + (x * 16), pos_y + (y * 16), code, 3);
			*sp_num = spNum;
			uPointer_ADR_X += 0x80ul;
		}
		uPointer_ADR_Y += 0x800ul;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void BG_TEXT_SET(int8_t *fname)
{
	uint16_t	usV_pat;
	uint32_t	pal = 0;
	uint16_t	i,j;
	int32_t	x,y;
	uint16_t	map_data[64][64];

	/* �}�b�v�f�[�^�ǂݍ��� */
	File_Load_CSV( fname, (uint16_t *)&map_data[0][0], &i, &j);
	
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

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t PCG_Main(void)
{
	int16_t	ret = 0;
	int16_t	uWidth = 0, uHeight = 0;
	int16_t	x, y, count, pat_size, Anime_pat_size, pat_data;
	int16_t	uPCG_num;
	uint8_t	Plane_num;
	uint8_t	Anime_num, Anime_num_old, DataMax;
	static uint16_t	s_uPCG_Num = PCG_16x16_AREA;

	for(uPCG_num = 0; uPCG_num < PCG_NUM_MAX; uPCG_num++)
	{
		if(g_stPCG_DATA[uPCG_num].validty == FALSE)
		{
			continue;
		}
		
		if( g_stPCG_DATA[uPCG_num].Pat_h == 0 )
		{
			continue;
		}
		
		if( g_stPCG_DATA[uPCG_num].Pat_w == 0 )
		{
			continue;
		}

		/* �v���[���ԍ� */
		Plane_num = g_stPCG_DATA[uPCG_num].Plane;
		
		/* �p�^�[���T�C�Y */
		pat_size = g_stPCG_DATA[uPCG_num].Pat_w * g_stPCG_DATA[uPCG_num].Pat_h;
		
		/* �A�j���[�V�����ԍ� */
		Anime_num_old = g_stPCG_DATA[uPCG_num].Anime_old;
		Anime_num = g_stPCG_DATA[uPCG_num].Anime;
		DataMax = g_stPCG_DATA[uPCG_num].Pat_DataMax;
		
		Anime_pat_size = Anime_num * pat_size;
		
		/* �X�v���C�g�ݒ� */
		count = 0;
		uHeight = 0;
		for( y = 0; y < g_stPCG_DATA[uPCG_num].Pat_h; y++ )
		{
			uWidth = 0;
			for( x = 0; x < g_stPCG_DATA[uPCG_num].Pat_w; x++ )
			{
				pat_data = (count + (Anime_pat_size));
				
				/* PCG���`�A�Ē�`���� */
				if(	g_stPCG_DATA[uPCG_num].Plane == 0xFF ) /* ���� */
				{
					if(count == 0)	/* �擪�̂� */
					{
						Plane_num = s_uPCG_Num;	/* �v���[���ԍ����� */
					}
					
					/* PCG���`���ăp�^�[���f�[�^�������� */
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + pat_data) &= ~0xFFU;
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + pat_data) |= s_uPCG_Num;
					
					/* ����������PCG�G���A�փR�s�[ */
					_iocs_sp_defcg( (Plane_num + count) & (PCG_MAX-1),
									SP_16x16,
						g_stPCG_DATA[uPCG_num].pPatData + Mmul64(count + ((Anime_pat_size) % DataMax)) );	/* 64 = 128byte */
					
					s_uPCG_Num++;
					if(s_uPCG_Num >= PCG_MAX)
					{
						s_uPCG_Num = PCG_16x16_AREA;
					}
				}
				else if(Anime_num_old != Anime_num)	/* PCG���`�͂��̂܂܂ŃA�j���[�V�����ύX */
				{
					/* ����������PCG�G���A�փR�s�[ */
					_iocs_sp_defcg( (Plane_num + count) & (PCG_MAX-1),
									SP_16x16,
						g_stPCG_DATA[uPCG_num].pPatData + Mmul64(count + ((Anime_pat_size) % DataMax)) );	/* 64 = 128byte */
				}
				
				/* �\�� */
				if(	g_stPCG_DATA[uPCG_num].update == TRUE)	/* �X�V�A�� */
				{
					/* �p�^�[���f�[�^�������� */
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + pat_data) &= ~0xFFU;
					*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + pat_data) |= (Plane_num + count);
					
					/* �X�v���C�g���W�X�^�̐ݒ� */
					_iocs_sp_regst( (Plane_num + count) & (SP_PLN_MAX-1),
									-1,
									g_stPCG_DATA[uPCG_num].x + uWidth,
									g_stPCG_DATA[uPCG_num].y + uHeight,
									*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + pat_data),
									3);
				}
				
				uWidth += SP_W;
				count++;
			}
			uHeight += SP_H;
		}
		
		g_stPCG_DATA[uPCG_num].Plane = Plane_num;
		g_stPCG_DATA[uPCG_num].Anime_old = Anime_num;
		g_stPCG_DATA[uPCG_num].update = FALSE;	/* �X�V�ς݂ɂ��� */
	}

#ifdef DEBUG
//	printf("PCG(%d)=(%d,%d)(0x%4x)\n", uPCG_num, g_stPCG_DATA[uPCG_num].x + uWidth, g_stPCG_DATA[uPCG_num].y + uHeight, *(g_stPCG_DATA[uPCG_num].pPatCodeTbl) );
//	KeyHitESC();	/* �f�o�b�O�p */
#endif
	
	return ret;
}
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
ST_PCG *PCG_Get_Info(uint8_t uNum)
{
	ST_PCG *ret;

	ret = (ST_PCG *)&g_stPCG_DATA[uNum];	/* �������擾 */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t PCG_Set_Info(ST_PCG stPCG, uint8_t uNum)
{
	int16_t	ret = 0;

	g_stPCG_DATA[uNum] = stPCG;	/* �X�V */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t PCG_Load_Data(int8_t *sFileName, uint16_t uPCG_data_ofst,
						ST_PCG stPCG, uint16_t uPCG_num, uint8_t uPCG_Type)
{
	int16_t	ret = 0;
	uint32_t	j;
	uint32_t	uBufSize;

	/* �R�s�[ */
	g_stPCG_DATA[uPCG_num] = stPCG;
	
	/* �������m�� */
	uBufSize = stPCG.Pat_w * stPCG.Pat_h * stPCG.Pat_AnimeMax;
	
	/* �X�v���C�g��`�ݒ� */
	g_stPCG_DATA[uPCG_num].pPatCodeTbl	= (uint32_t*)MyMalloc(sizeof(uint32_t) * uBufSize);	/* 4byte�~�T�C�Y�� */
	
	for(j=0; j < uBufSize; j++)
	{
		*(g_stPCG_DATA[uPCG_num].pPatCodeTbl + j)	= SetBGcode(0, 0, 0x00, 0xFF);	/* �p�^�[���R�[�h�e�[�u�� */
	}
	
	/* �X�v���C�g�摜�f�[�^ */
	uBufSize = stPCG.Pat_DataMax;
	
	g_stPCG_DATA[uPCG_num].pPatData		= (uint16_t*)MyMalloc(SP_16_SIZE * uBufSize);		/* 128byte�~�A�j���[�V������ */
	
	memset(g_stPCG_DATA[uPCG_num].pPatData, 0, SP_16_SIZE * uBufSize);	/* 0�N���A */
	
	/* �X�v���C�g�f�[�^�ǂݍ��� */
	{
		uint32_t uSP_MoveSize;
		FILE *fp;

		fp = fopen( sFileName, "rb" ) ;	/* �t�@�C���I�[�v�� */
		if(fp == NULL)
		{
			return -1;
		}
		
		switch(uPCG_Type)
		{
			case 0:
			{
				fseek(fp, (uPCG_data_ofst * SP_16_SIZE), SEEK_SET);	/* �ړI�f�[�^�̐擪�ʒu */
				
				fread( g_stPCG_DATA[uPCG_num].pPatData		/* �p�^�[���̊i�[�� */
					,  SP_16_SIZE							/* 1PCG = �������T�C�Y 16x16 */
					,  uBufSize								/* ���p�^�[���g���̂��H */
					,  fp	);								/* �t�@�C���|�C���^ */
				break;
			}
			case 1:
			{
				uSP_MoveSize = Mdiv2(SP_16_SIZE);
				
				for(j=0; j < stPCG.Pat_DataMax; j++)
				{
					uint16_t *pPt = g_stPCG_DATA[uPCG_num].pPatData;
					
					fseek( fp, (uPCG_data_ofst * SP_16_SIZE) + (j * uSP_MoveSize), SEEK_SET);	/* �ړI�f�[�^�̐擪�ʒu */
					
					fread( pPt + (uSP_MoveSize * j)
							,  uSP_MoveSize		/* 1PCG = �������T�C�Y */
							,  1				/* ���p�^�[���g���̂��H */
							,  fp
							);
				}
				break;
			}
			case 2:
			{
				uSP_MoveSize = Mdiv4(SP_16_SIZE);

				for(j=0; j < stPCG.Pat_DataMax; j++)
				{
					uint16_t *pPt = g_stPCG_DATA[uPCG_num].pPatData;
					
					fseek( fp, (uPCG_data_ofst * SP_16_SIZE) + (j * uSP_MoveSize), SEEK_SET);	/* �ړI�f�[�^�̐擪�ʒu */
					
					fread( pPt + (uSP_MoveSize * j)
							,  uSP_MoveSize		/* 1PCG = �������T�C�Y */
							,  1				/* ���p�^�[���g���̂��H */
							,  fp
							);
				}
				break;
			}
			default:
			{
				uSP_MoveSize = SP_16_SIZE;
				break;
			}
		}
		fclose( fp ) ;		/* �t�@�C���N���[�Y */
	}
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t PCG_Load_PAL_Data(int8_t *sFileName, uint16_t uPAL_data_ofst, uint16_t uPAL_Set_num)
{
	int16_t	ret = 0;
	
	FILE *fp;
	uint32_t i;
	uint16_t	pal_dat[ 128 ]; /* �p���b�g�f�[�^�t�@�C���ǂݍ��݃o�b�t�@ */

	if(uPAL_data_ofst == 0)return -1;
	
	/*--------[ �X�v���C�g�p���b�g�f�[�^�ǂݍ��݂ƒ�` ]--------*/

	fp = fopen( sFileName , "rb" ) ;
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		fseek( fp, 2 * 16 * (uPAL_data_ofst - 1), SEEK_SET);	/* �ړI�f�[�^�̐擪�ʒu */
		
		fread( pal_dat
			,  2		/* 1palet = 2byte */
			,  16		/* 16palet * 16block */
			,  fp
			) ;
		fclose( fp ) ;

		/* �X�v���C�g�p���b�g�ɓ]�� */
		for( i = 0 ; i < 16 ; i++ )
		{
			_iocs_spalet( (0x80 | (i & 0x0F)), uPAL_Set_num, pal_dat[i] ) ;
		}
	}
	
	return ret;
}
#endif	/* PCG_C */

