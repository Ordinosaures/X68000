#ifndef	GRAPHIC_H
#define	GRAPHIC_H

#include "inc/usr_define.h"

#define	CRT_MAX		(3)

enum
{
	POS_TOP,
	POS_CENTER,
	POS_BOTTOM,
	POS_LEFT,
	POS_MID,
	POS_RIGHT,
};

enum
{
	Clear_G=0,
	BackGround_G,
	Object1_G,
	Object2_G,
	Object3_G,
	Object4_G,
	Object5_G,
	Object6_G,
	Enemy1_G,
	Enemy2_G,
	Enemy3_G,
	Enemy4_G,
	Flip_G
};

#define	CG_MAX	(16)
#define	TRANS_PAL	(0x00)

#define FILE_TYPE			(0x424D)	/* "BM"�l */
#define FILE_HEADER_SIZE	(14)		/* BMP�t�@�C���w�b�_�T�C�Y */
#define INFO_HEADER_SIZE	(40)		/* Windows�w�b�_�T�C�Y */
#define DEFAULT_HEADER_SIZE	(FILE_HEADER_SIZE + INFO_HEADER_SIZE)	/* �W���̃w�b�_�T�C�Y */

#pragma pack (push, 1)   //�p�f�B���O���Pbyte�ɕύX

typedef struct tagBITMAPFILEHEADER {
	US	bfType;			/* �t�@�C���^�C�v */
	UL	bfSize;			/* �t�@�C���T�C�Y */
	US	bfReserved1;	/* �\��̈�P */
	US	bfReserved2;	/* �\��̈�Q */
	UL	bfOffBits;		/* �擪����摜�f�[�^�܂ł̃I�t�Z�b�g */
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	UL	biSize;			/* �w�b�_�T�C�Y */
	SL	biWidth;		/* �摜�̕�(px) */
	SL	biHeight;		/* �摜�̍���(px) */
	US	biPlanes;		/* �v���[���� ��ɂP */
	US	biBitCount;		/* �P��f������̃f�[�^�T�C�Y */
	UL	biCompression;	/* ���k�`�� */
	UL	biSizeImage;	/* �摜�f�[�^���̃T�C�Y */
	SL	biXPixPerMeter;	/* �������𑜓x(dot/m) */
	SL	biYPixPerMeter;	/* �c�����𑜓x(dot/m) */
	UL	biClrUsed;		/* �i�[����Ă���p���b�g��(�g�p�F��) */
	UL	biClrImportant;	/* �d�v�ȃp���b�g�̃C���f�b�N�X */
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	UC	rgbBlue;		/* �O�`�Q�T�T */
	UC	rgbGreen;		/* �΂O�`�Q�T�T */
	UC	rgbRed;			/* �ԂO�`�Q�T�T */
	UC	rgbReserved;	/* �\��̈�O */
} RGBQUAD;

#pragma pack (pop)

typedef struct tagPICIMAGE {
	BITMAPFILEHEADER *pBMf;
	BITMAPINFOHEADER *pBMi;
	US	*pImageData;
} PICIMAGE;

typedef struct tagCG_LIST {
	SC	bFileName[256];		/* �p�X */
	UC	ubType;				/* 0:�ʏ�A1:�X�v���C�g���C�N */
	UC	ubTransPal;			/* ���ߐF�p���b�g�ԍ� */
} CG_LIST;

extern	SS	GetCRT(ST_CRT *, SS);
extern	SS	SetCRT(ST_CRT, SS);
extern	SS	CRT_INIT(void);
extern	SS	Get_CG_FileList_MaxNum(UI *);
extern	UC	*Get_CG_FileBuf(UC);
extern	SS	Get_PicImageInfo(UC , UI *, UI *, UI *);
extern	SS	Get_PicImagePallet(UC);
extern	SS	Set_PicImagePallet(UC);
extern	void	CG_File_Load(void);
extern	void	G_INIT(void);
extern	void	G_HOME(void);
extern	void	G_Palette_INIT(void);
extern	void	G_Palette(void);
extern	SS	G_Stretch_Pict( SS , US , SS , US , UC , SS , US, SS, US, UC );
extern	SS	G_BitBlt(SS , US , SS , US , UC , SS , US , SS , US , UC , UC , UC , UC );
extern	SI	G_CLR(void);
extern	SS	G_CLR_AREA(SS, US, SS, US, UC);
extern	SS	G_CLR_ALL_OFFSC(UC);
extern	SS	G_Load(UC, US, US, US);
extern	SS	G_Load_Mem(UC, US, US, US);
extern	SS	APICG_DataLoad2G(SC *, UL, UL, US);
extern	SS	APICG_DataLoad2M(UC, UL, UL, US, US *);
extern	SS	G_Subtractive_Color(US *, US *, US, US, US, UI);

#endif	/* GRAPHIC_H */
