#ifndef	MUSIC_C
#define	MUSIC_C

#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>
#include <sys/iocs.h>
#include <interrupt.h>

#include "inc/usr_macro.h"
#include "MUSIC.h"
#include "FileManager.h"
#include "Output_Text.h"

#define ZM_V3

#ifdef	ZM_V2
		#include "inc/ZMUSIC.H"
#else
	#ifdef	ZM_V3
		#include "inc/ZMSC3LIB.H"
	#else
		#error "No Music Lib"
	#endif
#endif

#ifdef	ZM_V2
#else
	#ifdef	ZM_V3
	#else
		#error "No Music Lib"
	#endif
#endif

#define ZM_NUM_V2	(0x02)
#define ZM_NUM_V3	(0x03)

#define	MUSIC_MAX	(16)
#define	SOUND_MAX	(16)
#define	ADPCM_MAX	(16)

/* �O���[�o���ϐ� */
static SC	music_list[MUSIC_MAX][256]	=	{0};
static SC	music_dat[MUSIC_MAX][4096]	=	{0};
static SS	music_dat_size[MUSIC_MAX]	=	{0};
static UI	m_list_max	=	0u;

static SC	se_list[SOUND_MAX][256]	=	{0};
static SC	se_dat[SOUND_MAX][4096]	=	{0};
static SS	se_dat_size[SOUND_MAX]	=	{0};
static SI	se_dat_addr[SOUND_MAX]	=	{0};
static UI	s_list_max	=	0u;

static SC	adpcm_list[ADPCM_MAX][256]	=	{0};
static SC	adpcm_dat[ADPCM_MAX][32768]	=	{0};
static SI	adpcm_dat_size[ADPCM_MAX]	=	{0};
static UI	p_list_max	=	0u;

UC	v[] = {
#if 1
/* ���@�~�T�C�� */
/*	AF  OM  WF  SY  SP PMD AMD PMS AMS PAN DUMMY	*/
	60, 15,  0,  0,  0,  0,  0,  0,  0,  3,  0,
/*	AR  DR  SR  RR  SL  OL  KS  ML DT1 DT2 AME	*/
	27, 15,  5,  2,  0,  0,  0,  0,  3,  1,  0,
	31, 18, 18,  6,  7,  0,  0,  0,  3,  2,  0,
	22, 31,  0, 10,  0, 42,  0,  7,  7,  0,  0,
	15, 31,  0,  8,  0,  0,  2,  1,  7,  0,  0
#else
/*	AF  OM  WF  SY  SP PMD AMD PMS AMS PAN DUMMY	*/
	59, 15,  2,  1,200,127,  0,  0,  0,  3,  0,
/*	AR  DR  SR  RR  SL  OL  KS  ML DT1 DT2 AME */
	31,  8,  1,  8,  7, 20,  2,  1,  5,  3,  0,
	31,  8,  8,  7,  5, 24,  1,  2,  1,  1,  0,
	31,  3,  7,  8,  1, 21,  1,  1,  3,  0,  0,
	31,  0,  0,  9,  0,  0,  2,  8,  5,  2,  0
#endif
};

UC	SE_Data[] = {	/* �\���̂ɂ��������悢�H */
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
SI Music_Play(UC);
SI Music_Stop(void);
SI SE_Play(UC);
SI SE_Play_Fast(UC);
SI ADPCM_Play(UC);
SI Get_ZMD_Trak_Head(UC *, SS);

/* �֐� */
void Init_Music(void)
{
	UI	unZmusicVer;
	UI	unZmusicVerNum;
	UI	i;
	SS	ret;

	/* �T�E���h�풓�m�F */
#ifdef	ZM_V2
	unZmusicVer = zm_ver();
	if(unZmusicVer == 0)		/* 0:�풓�i�V */
	{
		puts("Z-MUSIC���풓���Ă��������B");
		exit(0);
	}
#else
	#ifdef	ZM_V3
//	SC	sTONE[33] = {0};
	
	unZmusicVer = zm_check_zmsc();
	printf("zm_check_zmsc = %d\n", unZmusicVer);
	if((unZmusicVer & 0xFFFF) == 0xFFFF)	/* �풓�i�V */
	{
		puts("Z-MUSIC���풓���Ă��������B");
		exit(0);
	}
	#else
		#error "No Music Lib"
	#endif
#endif
	
#ifdef	ZM_V2
	unZmusicVerNum = ZM_NUM_V2;	/* Ver2.0x���� */
#else
	#ifdef	ZM_V3
	unZmusicVerNum = ZM_NUM_V3;	/* Ver3.0x���� */
	#else
		#error "No Music Lib"
	#endif
#endif
	if((unZmusicVer&0xF000u)>>12 != unZmusicVerNum)	/* Ver���� */
	{
		printf("Z-MUSIC Ver%d���풓���Ă��������B\n", unZmusicVerNum);
		exit(0);
	}

#if 0
	if((unZmusicVer&0xFFFF000u)>>16 == 0u)	/* PCM8���� */
	{
		puts("PCM8A���풓���Ă��������B");
		exit(0);
	}
#endif
	
	/* �T�E���h������ */
#ifdef	ZM_V2
	m_init();		/* ������ */
	m_ch("fm");		/* FM */
#else
	#ifdef	ZM_V3
	ret = zm_init(0);		/* ������ */
	printf("zm_init = %d\n", ret);
	#else
		#error "No Music Lib"
	#endif
#endif

	Music_Stop();	/* ���y��~ */

#ifdef	ZM_V2
	Load_Music_List("data\\music\\m_list.txt", music_list, &m_list_max);
	printf("%s\n", music_list[i]);
#else
	#ifdef	ZM_V3
	/* BGM */
	Load_Music_List("data\\music\\m_list_V3.txt", music_list, &m_list_max);
	for(i = 0; i < m_list_max; i++)
	{
		music_dat_size[i] = File_Load(music_list[i], music_dat[i], sizeof(UC), 0);
		printf("Music File %2d = %s = size(%d[byte])\n", i, music_list[i], music_dat_size[i]);
	}
	/* ���ʉ�(FM) */
	Load_SE_List("data\\se\\s_list_V3.txt", se_list, &s_list_max);
	for(i = 0; i < s_list_max; i++)
	{
		se_dat_size[i] = File_Load(se_list[i], se_dat[i], sizeof(UC), 0);
		se_dat_addr[i] = Get_ZMD_Trak_Head(se_dat[i], se_dat_size[i]);
		printf("Sound Effect File %2d = %s = size(%d[byte](Head[0x%x]))\n", i, se_list[i], se_dat_size[i], se_dat_addr[i]);
	}
	/* ���ʉ�(ADPCM) */
	Load_SE_List("data\\se\\p_list_V3.txt", adpcm_list, &p_list_max);
	for(i = 0; i < p_list_max; i++)
	{
#if 1
	 	adpcm_dat_size[i] = (SI)File_Load(adpcm_list[i], adpcm_dat[i], sizeof(UC), 0);
		printf("ADPCM File %2d = %s = size(%d[byte])\n", i, adpcm_list[i], adpcm_dat_size[i]);
#else
		/* ADPCM�o�^ */
		static SC	*adpcm_addr[ADPCM_MAX];
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
	}
	/* ZPD�o�^ */
//	ret = zm_register_zpd("data\\se\\OverKata_V3.ZPD");
//	printf("zm_register_zpd = %d\n", ret);
	#else
		#error "No Music Lib"
	#endif
#endif
	
#if 0
	exit(0);
#endif
}

void Exit_Music(void)
{
	Music_Stop();	/* ���y��~ */
	
#ifdef	ZM_V2
	m_init();		/* ������ */
#else
	#ifdef	ZM_V3
	zm_init(0);		/* ������ */
	#else
		#error "No Music Lib"
	#endif
#endif
}

/* ���y�Đ� */
SI Music_Play(UC bPlayNum)
{
	SI	ret=0;
	if(bPlayNum > m_list_max)return ret;

	Music_Stop();	/* ���y��~ */
#ifdef	ZM_V2
	zmd_play(&music_list[bPlayNum][0]);	
#else
	#ifdef	ZM_V3
	ret = zm_play_zmd(music_dat_size[bPlayNum], &music_dat[bPlayNum][0]);
	#else
		#error "No Music Lib"
	#endif
#endif
	return	ret;
}

/* ���y��~ */
SI Music_Stop(void)
{
	SI	ret=0;
#ifdef	ZM_V2
	m_stop(0,0,0,0,0,0,0,0,0,0);
#else
	#ifdef	ZM_V3
	zm_stop_all();
	#else
		#error "No Music Lib"
	#endif
#endif
	return	ret;
}

/* ���ʉ�ZMD�̉��t(FM�����Ō��ʉ��Đ�����) */
SI SE_Play(UC bPlayNum)
{
	SI	ret=0;
#ifdef	ZM_V2
	#error "No Music Lib"
#else
	#ifdef	ZM_V3
	SI	HeadNum = 0;
	
	if(bPlayNum > s_list_max)return ret;
	
	HeadNum = se_dat_addr[bPlayNum];
	
	ret = zm_play_zmd_se(&se_dat[bPlayNum][8]);	/* �ᑬ */
	/* ����
		char *zmd:ZMD�̊i�[�o�b�t�@(�w�b�_���܂܂�)
	*/
	#else
		#error "No Music Lib"
	#endif
#endif
	return	ret;
}

/* ���ʉ�ZMD�̉��t(FM�����Ō��ʉ��Đ�����) */
SI SE_Play_Fast(UC bPlayNum)
{
	SI	ret=0;
#ifdef	ZM_V2
	/* ���O�̃f�[�^�����⏉�����ŉ��F�̏��������ʓr�K�v */
	struct	_regs	stInReg = {0}, stOutReg = {0};
	UI	retReg;
//	UI	TrkFreeSize;
	UC	bCh, bTrk;
	SI	level;

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
	stInReg.a1 = (UI)&SE_Data[0];	/* ���t�f�[�^�i�[�A�h���X */
	
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
#else
	#ifdef	ZM_V3
	SI	HeadNum = 0;

	if(bPlayNum > s_list_max)return ret;
	
	HeadNum = se_dat_addr[bPlayNum];
	
	ret = zm_se_play(&se_dat[bPlayNum][HeadNum]);	/* ���� */
	/* ����
		char *zmd:ZMD�̊i�[�o�b�t�@(�g���b�N�E�`�����l�����e�[�u������)
	*/
	#else
		#error "No Music Lib"
	#endif
#endif
	return	ret;
}

/* (AD)PCM���ʉ��̉��t */
SI ADPCM_Play(UC bPlayNum)
{
	SI	ret=0;
	
	if(bPlayNum > p_list_max)return ret;

	/* �F�X����������IOCS���C�u�����̕����g�� */
	if(_iocs_adpcmsns() != 0)	/* �������Ă��� */
	{
		_iocs_adpcmmod(0);	/* ��~ */
	}
	_iocs_adpcmout(adpcm_dat[bPlayNum], 0x403, adpcm_dat_size[bPlayNum]);	/* �Đ� */
#if 0
#ifdef	ZM_V2
	SI	adpcm_sns;
	adpcm_sns = m_stat(9/*Mmin(Mmax(25, vx), 31)*/);	/* ADPCM ch1(9) ch2-8(25-31) */
//	Message_Num(&adpcm_sns,	 0, 13, 2, MONI_Type_SI, "%d");

	m_pcmplay(bPlayNum, 3, 4);
#else
	#ifdef	ZM_V3
//	SI	errnum;
//	SC	errtbl[64]={0};
//	UC	**p;

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

	#else
		#error "No Music Lib"
	#endif
#endif
#endif	
	return	ret;
}

/* ���t�g���b�N�܂ł̃I�t�Z�b�g���擾 */
SI	Get_ZMD_Trak_Head(UC *dat, SS size)
{
	UI	ret = 0;
	
#ifdef	ZM_V3
	UC	*p;
	p = dat + (4*2) + (4*1);
	ret = (*(p+0) << 12) + (*(p+1) << 8) + (*(p+2) << 4) + *(p+3);
#endif	
	
	return	ret;
}

#endif	/* MUSIC_C */
