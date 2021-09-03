#ifndef	MUSIC_C
#define	MUSIC_C

#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>
#include <sys/iocs.h>
#include <interrupt.h>

#include "inc/usr_macro.h"
#include "OverKata.h"
#include "MUSIC.h"
#include "FileManager.h"
#include "Output_Text.h"

#define FM_CH_MAX	(8)
#define FM_USE_CH	(1)
#define MML_BUF		(32)
#define MML_BUF_N	(2048)

#if		ZM_V2 == 1
#elif	ZM_V3 == 1
#elif	MC_DRV == 1
#else
	#error "No Music Lib"
#endif

#if		ZM_V2 == 1
	#include "inc/ZMUSIC.H"
#elif	ZM_V3 == 1
	#include "inc/ZMSC3LIB.H"
#elif	MC_DRV == 1
	#include "inc/mcclib.h"
#else
	#error "No Music Lib"
#endif

#define ZM_NUM_V2	(0x02)
#define ZM_NUM_V3	(0x03)
#define MC_DRV_NUM	(0x00600000)

/* �O���[�o���ϐ� */
int8_t		music_list[MUSIC_MAX][256]	=	{0};
uint32_t	m_list_max	=	0u;

#if		ZM_V2 == 1
#elif	ZM_V3 == 1
	int8_t	*music_dat[MUSIC_MAX];
	static int16_t	music_dat_size[MUSIC_MAX]	=	{0};
#elif	MC_DRV == 1
#else
	#error "No Music Lib"
#endif

#if		ZM_V2 == 1
#elif	ZM_V3 == 1
	int8_t		se_list[SOUND_MAX][256]	=	{0};
	uint32_t	s_list_max	=	0u;
	int8_t		*se_dat[SOUND_MAX];
	static int16_t	se_dat_size[SOUND_MAX]	=	{0};
	static int32_t	se_dat_addr[SOUND_MAX]	=	{0};
#elif	MC_DRV == 1
#else
	#error "No Music Lib"
#endif

int8_t		adpcm_list[ADPCM_MAX][256]	=	{0};
uint32_t	p_list_max	=	0u;
int8_t		*adpcm_dat[ADPCM_MAX];
static int32_t	adpcm_dat_size[ADPCM_MAX]	=	{0};

uint8_t	SE_Data[] = {	/* �\���̂ɂ��������悢�H */
//		0x01,										/* (+1)ZMD�̍\�� */
//		0x5A,0x6D,0x75,0x53,0x69,0x43,			/* (+6)ZmuSiC */
//		0x20,										/* (+7)�o�[�W���� */
#if 1
	0x00,0x01,									/* ���t���g���b�N��(.W) */
	0x00,0x00,0x00,0x02,						/* ���t�f�[�^�܂ł̃I�t�Z�b�g(.L) */
	0x00,0x05,									/* ���t��΃`�����l��(.W) -1 */
	0xa0,0x0c,0xb6,0x05,0xb3,0xd1,0x00,0x00,	/* ���t�f�[�^ */
	0x00,0x00,0xe6,0x00,0x3c,0xe8,0x00,0x18,
	0xff,0xff,0xd6,0x00,0x03,0x00,0x00,0xe0,
	0x39,0x00,0x0c,0xff,0xff,0x00,0x00,0x00,
	0x40,0x00,0x01,0x45,0x48,0xff,0xe0,0x45,
	0x00,0x0c,0x00,0x0c,0x00,0x00,0xff,0xc0,
	0x00,0xff,0xff,0xff
#else
	0x00, 0x01, 
	0x00, 0x00, 0x00, 0x02,
	0x00, 0x05,
	0xA0, 0x80, 0xB6, 0x00, 0xB3, 0xD1, 0x00, 0x00,
	0x00, 0x00, 0xBB, 0x00, 0xE8, 0x00, 0x00, 0x00,
	0x00, 0xD6, 0x00, 0x06, 0x00, 0x06, 0x1A, 0x01,
	0xFF, 0x1C, 0x01, 0xFF, 0x1F, 0x01, 0xFF, 0x21,
	0x01, 0xFF, 0x25, 0x01, 0xFF, 0x21, 0x01, 0xFF,
	0x1F, 0x01, 0xFF, 0x1C, 0x01, 0xFF, 0xC1, 0xCF,
	0x06, 0x25, 0x01, 0xFF, 0x1A, 0x01, 0xFF, 0x1C,
	0x01, 0xFF, 0x1E, 0x01, 0xFF, 0x1F, 0x01, 0xFF,
	0xAB, 0x08, 0xC2, 0x00, 0x16, 0xFF
#endif	
};

/* �\���̒�` */

/* �֐��̃v���g�^�C�v�錾 */
void Init_Music(void);
void Exit_Music(void);
int32_t Music_Play(uint8_t);
int32_t Music_Stop(void);
int32_t SE_Play(uint8_t);
int32_t SE_Play_Fast(uint8_t);
int32_t ADPCM_Play(uint8_t);
int32_t ADPCM_Stop(void);
int32_t Get_ZMD_Trak_Head(uint8_t *, int16_t);
int32_t M_SetMusic(uint32_t);
int32_t M_Play(int16_t);

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void Init_Music(void)
{
	uint32_t	i;
#if		(ZM_V2 == 1) || (ZM_V3 == 1)
	uint32_t	unZmusicVer;
	uint32_t	unZmusicVerNum;
#endif

	/* �T�E���h�풓�m�F */
#if		ZM_V2 == 1
	unZmusicVer = zm_ver();
	if(unZmusicVer == 0)		/* 0:�풓�i�V */
	{
		puts("Z-MUSIC���풓���Ă��������B");
		exit(0);
	}
	unZmusicVerNum = ZM_NUM_V2;	/* Ver2.0x���� */
#elif	ZM_V3 == 1
	int16_t	ret;
//	int8_t	sTONE[33] = {0};
	
	unZmusicVer = zm_check_zmsc();
	printf("zm_check_zmsc = %d\n", unZmusicVer);
	if((unZmusicVer & 0xFFFF) == 0xFFFF)	/* �풓�i�V */
	{
		puts("Z-MUSIC���풓���Ă��������B");
		exit(0);
	}
	unZmusicVerNum = ZM_NUM_V3;	/* Ver3.0x���� */
#elif	MC_DRV == 1
	if( MC_INIT() < MC_DRV_NUM )
	{
		puts( "mcdrv ���풓���ĂȂ����o�[�W�������Â��I" );
		exit(0);
	}
#else
	#error "No Music Lib"
#endif
	
#if		(ZM_V2 == 1) || (ZM_V3 == 1)
	if((unZmusicVer&0xF000u)>>12 != unZmusicVerNum)	/* Ver���� */
	{
		printf("Z-MUSIC Ver%d���풓���Ă��������B\n", unZmusicVerNum);
		exit(0);
	}
#elif	MC_DRV == 1
	/* �������Ȃ� */
#else
	#error "No Music Lib"
#endif


#if 0
	if((unZmusicVer&0xFFFF000u)>>16 == 0u)	/* PCM8���� */
	{
		puts("PCM8A���풓���Ă��������B");
		exit(0);
	}
#endif
	
	/* �T�E���h������ */
#if		ZM_V2 == 1
	m_init();		/* ������ */
	m_ch("fm");		/* FM */
	for(i = 0; i < 8; i++)
	{
		int32_t	err = 0;
		uint16_t uCh, uTrk;

		uCh = i + 1;
		uTrk = i + 1;
		
		/* �g���b�N�o�b�t�@�ݒ� */
		err = m_alloc( uTrk, MML_BUF_N );	/* trk(1-80) */
		if(err != 0)
		{
			printf("m_alloc error %d-(%d,%d)\n", err, uTrk, MML_BUF_N);
		}
		/* �`�����l���ƃg���b�N�̊���t�� */
		err = m_assign( uCh, uTrk );	/* ch(FM:1-8 ADPCM:9 MIDI:10-25 PCM8:26-32) trk(1-80) */
		if(err != 0)
		{
			printf("m_assign error %d\n", err);
		}
	}
#elif	ZM_V3 == 1
	ret = zm_init(0);		/* ������ */
	printf("zm_init = %d\n", ret);
#elif	MC_DRV == 1
	/* �������Ȃ� */
#else
	#error "No Music Lib"
#endif

	Music_Stop();	/* ���y��~ */

	/* BGM */
#if		ZM_V2 == 1
	/* �������Ȃ� */
#elif	ZM_V3 == 1
	for(i = 0; i < m_list_max; i++)
	{
		int32_t	FileSize;
		
		GetFileLength(music_list[i], &FileSize);
		music_dat[i] = (int8_t*)MyMalloc(FileSize);
		memset(music_dat[i], 0, FileSize);
		/* �������ɓo�^ */
		music_dat_size[i] = File_Load(music_list[i], music_dat[i], sizeof(int8_t), 0);
		printf("Music File %2d = %s = size(%d[byte])\n", i, music_list[i], music_dat_size[i]);
	}
#elif	MC_DRV == 1
	/* �������Ȃ� */
#else
	#error "No Music Lib"
#endif
	
	/* ���ʉ�(FM) */
#if		ZM_V2 == 1
	/* �������Ȃ� */
#elif	ZM_V3 == 1
	for(i = 0; i < s_list_max; i++)
	{
		int32_t	FileSize;
		
		GetFileLength(se_list[i], &FileSize);
		se_dat[i] = (int8_t*)MyMalloc(FileSize);
		memset(se_dat[i], 0, FileSize);
		/* �������ɓo�^ */
		se_dat_size[i] = File_Load(se_list[i], se_dat[i], sizeof(int8_t), 0);
		se_dat_addr[i] = Get_ZMD_Trak_Head(se_dat[i], se_dat_size[i]);
		printf("Sound Effect File %2d = %s = size(%d[byte](Head[0x%x]))\n", i, se_list[i], se_dat_size[i], se_dat_addr[i]);
	}
#elif	MC_DRV == 1
	/* �������Ȃ� */
#else
	#error "No Music Lib"
#endif

#if		ZM_V2 == 1
	/* ZPD�o�^ */
#elif	ZM_V3 == 1
	/* ZPD�o�^ */
//	ret = zm_register_zpd("data\\se\\OverKata_V3.ZPD");
//	printf("zm_register_zpd = %d\n", ret);
#elif	MC_DRV == 1
	/* PDX�o�^ */
#else
	#error "No Music Lib"
#endif

	/* ���ʉ�(ADPCM) */
	for(i = 0; i < p_list_max; i++)
	{
#if 1
		int32_t	FileSize;
		
		GetFileLength(adpcm_list[i], &FileSize);
		adpcm_dat[i] = (int8_t*)MyMalloc(FileSize);
		memset(adpcm_dat[i], 0, FileSize);
		/* �������ɓo�^ */
	 	adpcm_dat_size[i] = (int32_t)File_Load(adpcm_list[i], adpcm_dat[i], sizeof(int8_t), FileSize);
//		printf("ADPCM File %2d = %s = size(%d[byte]=%d)\n", i, adpcm_list[i], adpcm_dat_size[i], FileSize);
#else
	#if		ZM_V3 == 1
		/* ADPCM�o�^ */
		static int8_t	*adpcm_addr[ADPCM_MAX];
		sprintf( sTONE, "%s", "test" );
		sprintf( adpcm_dat[i], "%s%c%s%c", sTONE, 0, adpcm_list[i], 0 );
		printf("adpcm_dat %2d = %s\n", i, adpcm_dat[i]);
		adpcm_addr[i] = zm_pcm_read( 0x8000+i, 0, -1, 0, adpcm_dat[i] );
		/* ����
			int num:�o�^�ԍ� (�g�[���ԍ��̏ꍇ:0�`32767)
							 (���F�ԍ��̏ꍇ:$8000+(0�`32767)
			int flag:(AD)PCM���H�R�}���h(PPC)��(AD)PCM�f�[�^���H���邩(-1:YES,0:NO)
			int type:�o�^����(-1:ADPCM,1=16BitPCM,0=V2�݊�)
			int orig:�I���W�i���L�[:0-127
			char *param:�o�^�f�[�^��񂨂��PPC��i�[�o�b�t�@(���l�Q��)
			
			�߂�l	0�ȊO:char *param������̃A�h���X
					0:�G���[
		*/
		printf("ADPCM File %2d = %s = addr(0x%x)\n", i, adpcm_list[i], adpcm_addr[i]);
	#endif
#endif
	}
	
#if 0
	exit(0);
#endif
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void Exit_Music(void)
{
	Music_Stop();	/* ���y��~ */
	
#if		ZM_V2 == 1
	m_init();		/* ������ */
#elif	ZM_V3 == 1
	zm_init(0);		/* ������ */
#elif	MC_DRV == 1
	MC_QUIT();		/* �I������ */
#else
	#error "No Music Lib"
#endif
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* ���y�Đ� */
int32_t Music_Play(uint8_t bPlayNum)
{
	int32_t	ret=0;
	if(bPlayNum > m_list_max)return ret;

	Music_Stop();	/* ���y��~ */
#if		ZM_V2 == 1
	zmd_play(&music_list[bPlayNum][0]);	
#elif	ZM_V3 == 1
	ret = zm_play_zmd(music_dat_size[bPlayNum], &music_dat[bPlayNum][0]);
#elif	MC_DRV == 1
	MC_MMCP_PLAY(&music_list[bPlayNum][0]);
#else
	#error "No Music Lib"
#endif

	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* ���y��~ */
int32_t Music_Stop(void)
{
	int32_t	ret=0;
	
#if		ZM_V2 == 1
	m_stop(0,0,0,0,0,0,0,0,0,0);
#elif	ZM_V3 == 1
	zm_stop_all();
#elif	MC_DRV == 1
#else
	#error "No Music Lib"
#endif
	
	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* ���ʉ�ZMD�̉��t(FM�����Ō��ʉ��Đ�����) */
int32_t SE_Play(uint8_t bPlayNum)
{
	int32_t	ret=0;

#if		ZM_V2 == 1
#elif	ZM_V3 == 1
	int32_t	HeadNum = 0;
	
	if(bPlayNum > s_list_max)return ret;
	
	HeadNum = se_dat_addr[bPlayNum];
	
	ret = zm_play_zmd_se(&se_dat[bPlayNum][8]);	/* �ᑬ */
	/* ����
		char *zmd:ZMD�̊i�[�o�b�t�@(�w�b�_���܂܂�)
	*/
#elif	MC_DRV == 1
#else
	#error "No Music Lib"
#endif

	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* ���ʉ�ZMD�̉��t(FM�����Ō��ʉ��Đ�����) */
int32_t SE_Play_Fast(uint8_t bPlayNum)
{
	int32_t	ret=0;
	
#if		ZM_V2 == 1
	/* ���O�̃f�[�^�����⏉�����ŉ��F�̏��������ʓr�K�v */
	struct	_regs	stInReg = {0}, stOutReg = {0};
	uint32_t	retReg;
//	uint32_t	TrkFreeSize;
	uint8_t	bCh, bTrk;
	int32_t	level;

	if(bPlayNum > m_list_max)return ret;

	bCh		= 6;
	bTrk	= 6;
	
	if(m_stat(bCh) != 0u)	/* bCh�����t�����ǂ������� */
	{
		Music_Stop();
	}

//	TrkFreeSize = m_free(bTrk);
//	Message_Num(&TrkFreeSize,	6,	10,	2, MONI_Type_UI, "%4d");
//	m_init();
//	m_stop(bTrk,0,0,0,0,0,0,0,0,0);
//	m_mute(6);
//	m_alloc(bTrk, 1000);
//	m_assign(bCh, bTrk);
//	m_vset(bTrk, v);

	m_play(bTrk,0,0,0,0,0,0,0,0,0);

	level = intlevel(7);	/* ���荞�݋֎~ */
	
	stInReg.d0 = 0xF0;				/* ZMUSIC.X�ɂ��IOCS�R�[�� */
	stInReg.d1 = 0x12;				/* se_play $12�iZMUSIC���̃t�@���N�V�����R�[���j */
	stInReg.d2 = bTrk;				/* ���t�g���b�NNo */
	stInReg.a1 = (uint32_t)&SE_Data[0];	/* ���t�f�[�^�i�[�A�h���X */
	
	retReg = _iocs_trap15(&stInReg, &stOutReg);	/* Trap 15 */
	{
//			Message_Num(&retReg,		22,	3,	2, MONI_Type_PT, "0x%x");
//			Message_Num(&stOutReg.a1,	22,	4,	2, MONI_Type_SI, "0x%x");
//			Message_Num(&stOutReg.a2,	22,	5,	2, MONI_Type_SI, "0x%x");
//			Message_Num(&stOutReg.a3,	22,	6,	2, MONI_Type_SI, "0x%x");
//			Message_Num(&stOutReg.a4,	22,	7,	2, MONI_Type_SI, "0x%x");
//			Message_Num(&stOutReg.a5,	22,	8,	2, MONI_Type_SI, "0x%x");
//			Message_Num(&stOutReg.a6,	22,	9,	2, MONI_Type_SI, "0x%x");
	}
	intlevel(level);		/* ���荞�݉��� */
#elif	ZM_V3 == 1
	int32_t	HeadNum = 0;

	if(bPlayNum > s_list_max)return ret;
	
	HeadNum = se_dat_addr[bPlayNum];
	
	ret = zm_se_play(&se_dat[bPlayNum][HeadNum]);	/* ���� */
	/* ����
		char *zmd:ZMD�̊i�[�o�b�t�@(�g���b�N�E�`�����l�����e�[�u������)
	*/
#elif	MC_DRV == 1
#else
	#error "No Music Lib"
#endif

	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* (AD)PCM���ʉ��̉��t */
int32_t ADPCM_Play(uint8_t bPlayNum)
{
	int32_t	ret=0;
	
	if(bPlayNum > p_list_max)return ret;

	/* �F�X����������IOCS���C�u�����̕����g�� */
	_iocs_adpcmout(adpcm_dat[bPlayNum], Mmul256(4) + 3, adpcm_dat_size[bPlayNum]);	/* �Đ� */
#if 0
	
#ifdef	ZM_V2
	int32_t	adpcm_sns;
	adpcm_sns = m_stat(9/*Mmin(Mmax(25, vx), 31)*/);	/* ADPCM ch1(9) ch2-8(25-31) */
//	Message_Num(&adpcm_sns,	 0, 13, 2, MONI_Type_SI, "%d");

	m_pcmplay(bPlayNum, 3, 4);
#elif	ZM_V3 == 1
//	int32_t	errnum;
//	int8_t	errtbl[64]={0};
//	uint8_t	**p;

	if(bPlayNum > p_list_max)return ret;
	
	//ret = zm_se_adpcm1( -1, 0x40, 4, 3, adpcm_dat_size[bPlayNum], 0, 7, adpcm_dat[bPlayNum]);
	/* ����
		char data_type:�f�[�^�^�C�v(-1:ADPCM,1:16bitPCM,2:8bitPCM)
		char volume:����(0-127,64:�W��)
		char frq:�Đ����g��(0:3.9kHz 1:5.2kHz 2:7.8kHz 3:10.4kHz 4:15.6kHz)
		char pan:�p���|�b�g(0-3 / $80+0-127)
		int size:�Đ�����ADPCM�̃f�[�^�T�C�Y
		short priority:���ʉ��D��x(0-255/0���ł��Ⴂ)
		short ch:�Đ��`�����l��(0-15)
		char *adpcm:(AD)PCM�f�[�^�A�h���X
	*/
	//ret = zm_se_adpcm2( -1, 0x40, 4, 3, bPlayNum, 0, 0);
	/*	����
		char data_type:�f�[�^�^�C�v(-1:ADPCM,1:16bitPCM,2:8bitPCM)
		char volume:����(0-127,64:�W��)
		char frq:�Đ����g��(0:3.9kHz 1:5.2kHz 2:7.8kHz 3:10.4kHz 4:15.6kHz)
		char pan:�p���|�b�g(0-3 / $80+0-127)
		int data_number:PCM�f�[�^�ԍ�(d15��1�Ȃ�Ή��F�ԍ�($8000+0-32767),
				      0�Ȃ�΃m�[�g�ԍ�(0-32767)�w��)
		short priority:���ʉ��D��x(0-255/0���ł��Ⴂ)
		short ch:�Đ��`�����l��(0-15)
	*/
	if(ret != 0)
	{
	//	errnum = zm_store_error(ret, 10, &p);
		/* ����
			int err (���16):�G���[�̔��������t�@���N�V�����ԍ�(0-32767)
					(����16):�G���[�R�[�h(0-65535)
			int err=-1�Ƃ����ꍇ�͖₢���킹�݂̂ŃX�g�A�͍s��Ȃ�
			int noferr:�G���[�̍��v���l�Ɋ���Z���邩
			char **addr:�G���[�X�g�b�N�o�b�t�@�̐擪�A�h���X���i�[����ϐ�

			�߂�l	���݃X�g�b�N���Ă���G���[�̌�
		*/
	//	zm_print_error(0, 1, errnum, 0, 0, 0, &errtbl[0], &p);
		
		/* ����
			int mode:���b�Z�[�W�̏o�͐�(0:���,1:�o�b�t�@)
			int lang:���b�Z�[�W�̌���(0:�p��,1:���{��)
			int noferr:�G���[�̐�
			char *zmd:ZMD�̃t�@�C����(�����ꍇ��0)
			char *srcaddr:�\�[�X(ZMS)�̃A�h���X(�����ꍇ��0)
			char *srcname:�\�[�X(ZMS)�t�@�C����(�����ꍇ��0)
			char *errtbl:�G���[�X�g�b�N�o�b�t�@�̐擪�A�h���X
			char **buff:�o�̓o�b�t�@�̐擪�A�h���X���i�[����ϐ�
		    (int mode=1�̎��̂ݗL��,�o�b�t�@�̏I�[�ɂ�0)
		*/
	}
//	Message_Num(&ret,	 11, 12, 2, MONI_Type_SI, "0x%x");

#elif	MC_DRV == 1
#else
	#error "No Music Lib"
#endif
	
#endif	
	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* (AD)PCM���ʉ��̒�~ */
int32_t ADPCM_Stop(void)
{
	int32_t	ret=0;
	
	if(_iocs_adpcmsns() != 0)	/* �������Ă��� */
	{
		_iocs_adpcmmod(1);	/* ���~ */
		_iocs_adpcmmod(0);	/* �I�� */
	}
	
	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* ���t�g���b�N�܂ł̃I�t�Z�b�g���擾 */
int32_t	Get_ZMD_Trak_Head(uint8_t *dat, int16_t size)
{
	int32_t	ret = 0;
	
#if		ZM_V2 == 1
#elif	ZM_V3 == 1
	uint8_t	*p;
	p = dat + (4*2) + (4*1);
	ret = (*(p+0) << 12) + (*(p+1) << 8) + (*(p+2) << 4) + *(p+3);
#elif	MC_DRV == 1
#else
	#error "No Music Lib"
#endif
	
	return	ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int32_t	M_SetMusic(uint32_t uNum)
{
	int32_t	ret = 0;
#if		ZM_V2 == 1
	uint32_t	i = 0u;
	int32_t	err = 0;
	
	/* �g���b�N�̊���t���ύX */
	for(i = 0; i < FM_USE_CH; i++)
	{
		int32_t	ch, trk;
		
		ch = (FM_CH_MAX - FM_USE_CH + 1) + i;
		trk = 60 + ch;
		
		err = m_alloc( trk, MML_BUF );
		if(err != 0)
		{
			printf("m_alloc error %d-(%d,%d)\n", err, trk, MML_BUF);
		}
		
		err = m_assign( ch, trk );	/* ch(FM:1-8 ADPCM:9 MIDI:10-25 PCM8:26-32) trk(1-80) */
		if(err != 0)
		{
			printf("m_assign error %d\n", err);
		}
	}

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
/* �G���W���� */
int32_t	M_Play(int16_t Key)
{
	int32_t	ret = 0;
	
#if		ZM_V2 == 1
	
	uint8_t	uMML[MML_BUF];
	int32_t	err = 0;
	int32_t	uCh = 0;
	int32_t	uTrk = 0;
	static uint8_t ubChanel = 0u;
	static uint8_t ubCount = 0u;

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode;
	uint16_t	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
	uCh = (FM_CH_MAX - FM_USE_CH + 1u) + ubChanel;
	uTrk = 60 + uCh;
	if(ubChanel < FM_USE_CH - 1u)
	{
		ubChanel++;
	}
	else
	{
		ubChanel = 0u;
	}

	err = m_alloc( uTrk, MML_BUF );	/* trk(1-80) */
	if(err != 0)
	{
		printf("m_alloc error %d-(%d,%d)\n", err, uTrk, MML_BUF);
	}
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	if(bDebugMode == TRUE)
	{
		if(Key > 0x80)
		{
			sprintf(uMML, "@%dv15o2l4q1@k%d d+&", uDebugNum, Mdiv8(Key) );	/* OK */
		}
		else
		{
			sprintf(uMML, "@137v15o2l4q1@k%d d+&", Mdiv8(Key) );	/* OK */
		}
	}
	else
#endif
	{
		sprintf(uMML, "@137v15o2l4q1@k%d d+&", Mdiv8(Key) );	/* OK */
	}
	err = m_trk( uTrk, uMML );	
	if(err != 0)
	{
		printf("m_trk error %d-(%d,%s,%d)\n", err, uTrk, uMML, ubCount);
	}
	/* ���ӁFX-BASIC��m_play�̈����́A�`�����l���BZ-MUSIC�́A�g���b�N�ԍ� */
	/* 10�̈����̓`�����l���������̂ōĐ��������g���b�NNo��ݒ肷�� */
	err = m_play(uTrk,'NASI','NASI','NASI','NASI','NASI','NASI','NASI','NASI','NASI');	/* 1 */
	if(err != 0)
	{
		printf("m_play error %d %d %d\n", err, uCh, uTrk);
	}
	
	ubCount++;
	
#endif	
	return ret;
}
#endif	/* MUSIC_C */
