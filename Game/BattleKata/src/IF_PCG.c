#ifndef	IF_PCG_C
#define	IF_PCG_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iocslib.h>
#include <doslib.h>
#include <usr_macro.h>
#include <XSP2lib.H>

#include "IF_PCG.h"
#include "BIOS_PCG.h"

#ifdef  CNF_XSP
#else   /* CNF_XSP 0 */
#endif  /* CNF_XSP */

/* �o�b�f�f�[�^ */
/*
	XSP �p PCG �z�u�Ǘ��e�[�u��
	�X�v���C�g PCG �p�^�[���ő�g�p�� + 1 �o�C�g�̃T�C�Y���K�v�B
*/
char pcg_alt[PCG_MAX + 1];		/* XSP �p PCG �z�u�Ǘ��e�[�u��	�X�v���C�g PCG �p�^�[���ő�g�p�� + 1 �o�C�g�̃T�C�Y���K�v�B*/
char pcg_dat[PCG_MAX * 128];	/* �o�b�f�f�[�^�t�@�C���ǂݍ��݃o�b�t�@ */
unsigned short pal_dat[ 256 ];	/* �p���b�g�f�[�^�t�@�C���ǂݍ��݃o�b�t�@ */


/* �\���̒�` */
/* �֐��̃v���g�^�C�v�錾 */
void sp_dataload(void);
void PCG_INIT(void);
void PCG_ON(void);
void PCG_OFF(void);
void PCG_START_SYNC(void);
void PCG_END_SYNC(void);
void PCG_PUT_1x1(short, short, short, short);
void PCG_PUT_2x1(short, short, short, short);
void PCG_PUT_2x2(short, short, short, short);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void sp_dataload(void)
{
	FILE *fp;
	unsigned int i,j;

	/*�X�v���C�g���W�X�^������*/
	for(j = 0x80000000; j < 0x80000000 + 128; j++ ){
		SP_REGST(j,-1,0,0,0,0);
	}
	
	/*-----------------[ �o�b�f�f�[�^�ǂݍ��� ]-----------------*/

	fp = fopen( "data/sp/BK.SP" , "rb" ) ;
//	fp = fopen( "SP_DATA/SAKANA.SP" , "rb" ) ;
	j = fread( pcg_dat
			,  128		/* 1PCG = 128byte */
			,  PCG_MAX	/* 256PCG */
			,  fp
	) ;
	fclose( fp ) ;

//	for( i = 0 ; i < PCG_MAX ; i++ ){
//		SP_DEFCG( i, 1, &pcg_dat[i * 128] );
//	}
	
#if 0
	/*-----------[ PCG �f�[�^���c��ʗp�� 90 �x��] ]-----------*/

	for (i = 0; i < 256; i++) {
		pcg_roll90(&pcg_dat[i * 128], 1);
	}
#endif
	
	/*--------[ �X�v���C�g�p���b�g�f�[�^�ǂݍ��݂ƒ�` ]--------*/

	fp = fopen( "data/sp/BK.PAL" , "rb" ) ;
//	fp = fopen( "SP_DATA/SAKANA.PAL" , "rb" ) ;
	fread( pal_dat
		,  2		/* 1palet = 2byte */
		,  256		/* 16palet * 16block */
		,  fp
		) ;
	fclose( fp ) ;

	/* �X�v���C�g�p���b�g�ɓ]�� */
	for( i = 0 ; i < 256 ; i++ )
	{
		SPALET( (i&15) | (1<<0x1F) , i/16 + 1, pal_dat[i] ) ;
	}
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void PCG_INIT(void)
{
#ifdef  CNF_XSP
    /*---------------------[ XSP �������� ]---------------------*/
    sp_dataload();  /* �X�v���C�g�̃f�[�^�ǂݍ��� */
	
    xsp_on();		/* XSP ON */

	xsp_pcgdat_set(pcg_dat, pcg_alt, sizeof(pcg_alt));  /* PCG �f�[�^�� PCG �z�u�Ǘ����e�[�u�����w�� */

//	xsp_vertical(1);    /* �c��ʃ��[�h */
	/*---------------------[ XSP �������� ]---------------------*/
#else   /* CNF_XSP 0 */
#if 0
	uint32_t	j;
	
	/* �X�v���C�g�̏����� */
	_iocs_sp_init();			/* �X�v���C�g�̏����� */
	
	/*�X�v���C�g���W�X�^������*/
	for( j = 0x80000000; j < 0x80000000 + 128; j++ )
	{
		_iocs_sp_regst(j,-1,0,0,0,0);
	}
	for(uint32_t i = 0; i < 256; i++ )
	{
		_iocs_sp_cgclr(i);			/* �X�v���C�g�N���A */
		_iocs_sp_defcg( i, 1,  );
	}
	
	//	_iocs_bgctrlgt(1);				/* BG�R���g���[�����W�X�^�ǂݍ��� */
	_iocs_bgscrlst(0,0,0);				/* BG�X�N���[�����W�X�^�ݒ� */
	_iocs_bgscrlst(1,0,0);				/* BG�X�N���[�����W�X�^�ݒ� */
	_iocs_bgtextcl(0,SetBGcode(0,0,0,0));	/* BG�e�L�X�g�N���A */
	_iocs_bgtextcl(1,SetBGcode(0,0,0,0));	/* BG�e�L�X�g�N���A */
	//	BGTEXTGT(1,1,0);			/* BG�e�L�X�g�ǂݍ��� */
#endif
    PCG_ON();                           /* �X�v���C�gON */
	PCG_SP_dataload("data/sp/BK.SP");	/* �X�v���C�g�̃f�[�^�ǂݍ��� */
	PCG_PAL_dataload("data/sp/BK.PAL");	/* �X�v���C�g�̃p���b�g�f�[�^�ǂݍ��� */
#endif  /*CNF_XSP*/
}
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void PCG_ON(void)
{
#ifdef  CNF_XSP
    xsp_on();			/* XSP ON */
	xsp_pcgdat_set(pcg_dat, pcg_alt, sizeof(pcg_alt));  /* PCG �f�[�^�� PCG �z�u�Ǘ����e�[�u�����w�� */
#else   /* CNF_XSP 0 */
    PCG_VIEW(1);        /* SP ON */
#endif  /*CNF_XSP*/
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void PCG_OFF(void)
{
#ifdef  CNF_XSP
	xsp_off();			/* XSP OFF */
#else   /* CNF_XSP 0 */
    PCG_VIEW(0);        /* SP OFF */
#endif  /*CNF_XSP*/
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void PCG_START_SYNC(void)
{
#ifdef  CNF_XSP
    xsp_vsync2(1);  /* �������� */
#else   /* CNF_XSP 0 */
    /* �Ȃ� */
#endif  /*CNF_XSP*/
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void PCG_END_SYNC(void)
{
#ifdef  CNF_XSP
    /* �X�v���C�g���ꊇ�\������ */
    xsp_out();
#else   /* CNF_XSP 0 */
    /* �������� */
    
#endif  /*CNF_XSP*/
}

void PCG_PUT_1x1(short x, short y, short pt, short info)
{
#ifdef  CNF_XSP
    xsp_set( x, y, pt, info);
#else   /* CNF_XSP 0 */
#endif  /* CNF_XSP */
}

void PCG_PUT_2x1(short x, short y, short pt, short info)
{
#ifdef  CNF_XSP
    xsp_set( x-8, y, pt+0, info);
    xsp_set( x+8, y, pt+1, info);
#else   /* CNF_XSP 0 */
#endif  /* CNF_XSP */
}

void PCG_PUT_2x2(short x, short y, short pt, short info)
{
#ifdef  CNF_XSP
    xsp_set( x-8, y-8, pt+0, info);
    xsp_set( x+8, y-8, pt+1, info);
    xsp_set( x-8, y+8, pt+2, info);
    xsp_set( x+8, y+8, pt+3, info);
#else   /* CNF_XSP 0 */
#endif  /* CNF_XSP */
}

#endif	/* IF_PCG_C */
