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

/* ＰＣＧデータ */
/*
	XSP 用 PCG 配置管理テーブル
	スプライト PCG パターン最大使用数 + 1 バイトのサイズが必要。
*/
char pcg_alt[PCG_MAX + 1];		/* XSP 用 PCG 配置管理テーブル	スプライト PCG パターン最大使用数 + 1 バイトのサイズが必要。*/
char pcg_dat[PCG_MAX * 128];	/* ＰＣＧデータファイル読み込みバッファ */
unsigned short pal_dat[ 256 ];	/* パレットデータファイル読み込みバッファ */


/* 構造体定義 */
/* 関数のプロトタイプ宣言 */
void sp_dataload(void);
void PCG_INIT(void);
void PCG_ON(void);
void PCG_OFF(void);
void PCG_START_SYNC(void);
void PCG_END_SYNC(void);
void PCG_PUT_1x1(short, short, short, short);
void PCG_PUT_2x1(short, short, short, short);
void PCG_PUT_2x2(short, short, short, short);

/* 関数 */
/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
void sp_dataload(void)
{
	FILE *fp;
	unsigned int i,j;

	/*スプライトレジスタ初期化*/
	for(j = 0x80000000; j < 0x80000000 + 128; j++ ){
		SP_REGST(j,-1,0,0,0,0);
	}
	
	/*-----------------[ ＰＣＧデータ読み込み ]-----------------*/

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
	/*-----------[ PCG データを縦画面用に 90 度回転 ]-----------*/

	for (i = 0; i < 256; i++) {
		pcg_roll90(&pcg_dat[i * 128], 1);
	}
#endif
	
	/*--------[ スプライトパレットデータ読み込みと定義 ]--------*/

	fp = fopen( "data/sp/BK.PAL" , "rb" ) ;
//	fp = fopen( "SP_DATA/SAKANA.PAL" , "rb" ) ;
	fread( pal_dat
		,  2		/* 1palet = 2byte */
		,  256		/* 16palet * 16block */
		,  fp
		) ;
	fclose( fp ) ;

	/* スプライトパレットに転送 */
	for( i = 0 ; i < 256 ; i++ )
	{
		SPALET( (i&15) | (1<<0x1F) , i/16 + 1, pal_dat[i] ) ;
	}
}

/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
void PCG_INIT(void)
{
#ifdef  CNF_XSP
    /*---------------------[ XSP を初期化 ]---------------------*/
    sp_dataload();  /* スプライトのデータ読み込み */
	
    xsp_on();		/* XSP ON */

	xsp_pcgdat_set(pcg_dat, pcg_alt, sizeof(pcg_alt));  /* PCG データと PCG 配置管理をテーブルを指定 */

//	xsp_vertical(1);    /* 縦画面モード */
	/*---------------------[ XSP を初期化 ]---------------------*/
#else   /* CNF_XSP 0 */
#if 0
	uint32_t	j;
	
	/* スプライトの初期化 */
	_iocs_sp_init();			/* スプライトの初期化 */
	
	/*スプライトレジスタ初期化*/
	for( j = 0x80000000; j < 0x80000000 + 128; j++ )
	{
		_iocs_sp_regst(j,-1,0,0,0,0);
	}
	for(uint32_t i = 0; i < 256; i++ )
	{
		_iocs_sp_cgclr(i);			/* スプライトクリア */
		_iocs_sp_defcg( i, 1,  );
	}
	
	//	_iocs_bgctrlgt(1);				/* BGコントロールレジスタ読み込み */
	_iocs_bgscrlst(0,0,0);				/* BGスクロールレジスタ設定 */
	_iocs_bgscrlst(1,0,0);				/* BGスクロールレジスタ設定 */
	_iocs_bgtextcl(0,SetBGcode(0,0,0,0));	/* BGテキストクリア */
	_iocs_bgtextcl(1,SetBGcode(0,0,0,0));	/* BGテキストクリア */
	//	BGTEXTGT(1,1,0);			/* BGテキスト読み込み */
#endif
    PCG_ON();                           /* スプライトON */
	PCG_SP_dataload("data/sp/BK.SP");	/* スプライトのデータ読み込み */
	PCG_PAL_dataload("data/sp/BK.PAL");	/* スプライトのパレットデータ読み込み */
#endif  /*CNF_XSP*/
}
/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
void PCG_ON(void)
{
#ifdef  CNF_XSP
    xsp_on();			/* XSP ON */
	xsp_pcgdat_set(pcg_dat, pcg_alt, sizeof(pcg_alt));  /* PCG データと PCG 配置管理をテーブルを指定 */
#else   /* CNF_XSP 0 */
    PCG_VIEW(1);        /* SP ON */
#endif  /*CNF_XSP*/
}

/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
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
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
void PCG_START_SYNC(void)
{
#ifdef  CNF_XSP
    xsp_vsync2(1);  /* 垂直同期 */
#else   /* CNF_XSP 0 */
    /* なし */
#endif  /*CNF_XSP*/
}

/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
void PCG_END_SYNC(void)
{
#ifdef  CNF_XSP
    /* スプライトを一括表示する */
    xsp_out();
#else   /* CNF_XSP 0 */
    /* 垂直同期 */
    
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

