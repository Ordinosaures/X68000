#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "inc/usr_define.h"
#include "FileManager.h"

#define _MACS_CALL	(0xD0)
#define _HIMEM		(0xF8)

int32_t	MACS_Play(int8_t *, int32_t, int32_t, int32_t, int32_t);
int32_t	MACS_Load(int8_t *, int32_t);
int32_t	MACS_Load_Hi(int8_t *, int32_t);
int32_t	MACS_CHK(void);
int32_t	HIMEM_CHK(void);
int32_t	PCM8A_CHK(void);
int32_t ADPCM_Stop(void);
void HelpMessage(void);
int16_t main(int16_t, int8_t **);

int32_t g_nCommand = 0;
int32_t g_nBuffSize = -1;
int32_t g_nAbort = 1;
int32_t g_nEffect = 0;
int32_t g_nPCM8Achk = 0;
int32_t g_nRepeat = 1;

/*	MACS_Play()
*	Input:
*		d1.w > �R�}���h�R�[�h
*		d2.l > �o�b�t�@�T�C�Y
*		d3.w > �A�{�[�g�t���O
*		d4.l > ������ʃt���O
*		a1.l > �����A�h���X
*	------------------------------------------------------------------------------
*		d1.w = 0	�A�j���[�V�����Đ�
*					(a1)����n�܂�A�j���f�[�^���Đ����܂��B
*	------------------------------------------------------------------------------
*		d2.l�͒ʏ�-1($FFFFFFFF)�ɂ��ĉ������B
*				�i�A�j���f�[�^�Đ����Ad2.l�̃G���A���z���悤�Ƃ�����,�Đ��𒆎~���܂��j
*	------------------------------------------------------------------------------
*	Output:  d0.l > Status
*------------------------------------------------------------------------------
*	d1.w = 0	�A�j���[�V�����Đ�
*				(a1)����n�܂�A�j���f�[�^���Đ����܂��B
*				d2.l�͒ʏ�-1($FFFFFFFF)�ɂ��ĉ������B
*				�i�A�j���f�[�^�Đ����Ad2.l�̃G���A���z���悤�Ƃ�����
*				�@�Đ��𒆎~���܂��j
*				d4.l�ɂ��Ă͉��\���Q��
*	
*	d1.w = 1	�Đ��󋵌���
*				���^�[���l��0�ȊO�Ȃ�΃A�j���[�V�����Đ����ł�
*	
*	d1.w = 2	���f���N�G�X�g���s
*				�A�j���[�V�����Đ��𒆒f����悤�v�����܂��B
*				d3.w�̓��e�ɂ�蒆�f��ɏ������s�Ȃ킹�邱�Ƃ��\�ł�
*	
*			d3.w = -1	���f�㉽�����Ȃ�(�ʏ�)
*			d3.w = -2	���f�� d2,d4,a1�̓��e�ɂ��A�j���[�V�����J�n
*			d3.w = -3	���f�� a1.l�̃A�h���X�ɃT�u���[�`���R�[��
*	
*	d1.w = 3	MACS�o�[�W��������
*				d0.w�ɏ풓���Ă���MACS�̃o�[�W�����ԍ���Ԃ��܂��B
*				(Version 1.16�Ȃ�$01_16)
*	
*	d1.w = 4	MACS�A�v���P�[�V������o�^���܂�
*				a1.l�ɃA�v���P�[�V�������ւ̃|�C���^��ݒ肵�܂��B
*				�A�v���P�[�V��������
*					'�`�`.x [�A�v���P�[�V������]',0
*				�Ƃ����t�H�[�}�b�g�ɏ]���ĉ������B���̃R�[���ŃA�v����
*				�o�^���邱�ƂŁAMACS�̏풓�������֎~���邱�Ƃ��ł��܂�
*	
*	d1.w = 5	MACS�A�v���P�[�V�����̓o�^���������܂�
*				a1.l��(d1.w=4)�œo�^�����A�v���P�[�V�������ւ̃|�C���^
*				��n���R�[������Ɠo�^�A�v���P�[�V�������������܂��B
*				�o�^�\�A�v���P�[�V�����͍ő�16�ł��B(Ver1.00)
*	
*	d1.w = 6	MACS�f�[�^�̃f�[�^�o�[�W�����𓾂܂�
*				a1.l��MACS�f�[�^�̐擪�A�h���X�����ăR�[�����܂��B
*				�Ԃ�l�Ƃ���d0.l�Ƀf�[�^���쐬���ꂽMACS�̃o�[�W������
*				����܂��Bd0.l=-1�̎���MACS�f�[�^�ł͂���܂���B
*	
*	d1.w = 7	��ʂ�ޔ����A�g�p�\�ɂ��܂�
*				�A�j���[�V�����X�^�[�g�Ɠ����������s�Ȃ��܂��B
*				���̃R�[���𗘗p���鎞�͕K���A�{�[�g�t���O�ɑΉ�����
*				�������B
*	
*	d1.w = 8	�ޔ�������ʂ����ɖ߂��܂�
*				��ʍď������}���t���O��1�ɂ��ăA�j���[�V�����Đ����s
*				�������͕K�����Ƃł��̃R�[���𔭍s���ĉ������B
*	
*	d1.w = 9	�e�L�X�gVRAM�ɃO���t�B�b�N��W�J���܂�
*				a1.l��MACS�`���O���t�B�b�N�f�[�^�̃A�h���X�Ca2.l�ɓW�J
*				��̃e�L�X�gVRAM�A�h���X���w�肵�ăR�[�����܂��B
*	
*	d1.w = 10	�����������荞�݂�o�^���܂�
*				a1.l�Ɋ��荞�݃��[�`���̃A�h���X�Ad2.w�Ƀt���O*256+�J
*				�E���^���Z�b�g���ăR�[�����܂��B
*				�t���O�͌��ݎg�p����Ă��܂���B0�ɂ��Ă����ĉ������B
*	
*	d1.w = 11	�����������荞�݂��������܂�
*				a1.l�ɓo�^�������荞�݃��[�`���̃A�h���X���Z�b�g����
*				�R�[�����܂��B
*	
*	d1.w = 12	�X���[�v�󋵂��`�F�b�N���܂�
*				���^�[���l��0�ȊO�Ȃ�MACSDRV�̓X���[�v���Ă��܂�
*	
*	d1.w = 13	�X���[�v��Ԃ�ݒ肵�܂�
*				d2.w��0�Ȃ�Đ��\��ԁC0�ȊO�Ȃ�X���[�v���܂��B
*	
*	d1.w = -1	���[�N�G���A�A�h���X�l��
*				d0.l��MACS�������[�N�G���A�̃A�h���X��Ԃ��܂��B
*------------------------------------------------------------------------------
*������ʃt���O (d4.l)
*
*bit 0	�n�[�t�g�[��			 		(1�̎��p���b�g��50%�Â�����)
*bit 1	PCM�J�b�g				 		(1�̎�PCM�̍Đ������Ȃ�)
*bit 2	�L�[�Z���X�A�{�[�gOFF 			(1�̎��L�[���͎��I�����Ȃ�)
*bit 3	��ʍď������}���t���O 			(1�̎��A�j���I������ʂ����������Ȃ�)
*bit 4	�X���[�v��Ԗ��� 				(1�̎�MACSDRV���X���[�v���Ă��鎞�ł��Đ�����)
*bit 5	�O���t�B�b�N�u�q�`�l�\���t���O 	(1�̎����̨����\�������܂܍Đ�����)
*bit 6	�X�v���C�g�\���t���O 			(1�̎����ײĂ�\�������܂܍Đ�����)
*
*bit 7
*  :		Reserved. (�K��0�ɂ��邱��)
*bit31
*
*------------------------------------------------------------------------------
* Status �G���[�R�[�h�ꗗ (d0.l)
*
*-1	MACS�f�[�^�ł͂���܂���
*-2	MACS�̃o�[�W�������Â����܂�
*-3	�f�[�^���o�b�t�@�ɑS�����͂���Ă��܂���
*-4	�A�{�[�g����܂���
*-5	���Ɍ��݃A�j���[�V�����Đ����ł�
*-6	�����ȃt�@���N�V�������w�肵�܂���
*-7	���[�U�[���X�g�������͊��荞�݃��X�g����t�ł�
*-8	���[�U�[���X�g�������͊��荞�݃��X�g�ɓo�^����Ă��܂���
*==============================================================================
*/

int32_t	MACS_Play(int8_t *pMacsBuff, int32_t nCommand, int32_t nBuffSize, int32_t nAbort, int32_t nEffect)
{
	struct	_regs	stInReg = {0}, stOutReg = {0};
	int32_t	retReg;	/* d0 */
	
	stInReg.d0 = _MACS_CALL;	/* IOCS _MACS(macsdrv.x) */
	stInReg.d1 = nCommand;		/* �R�}���h�R�[�h(w) */
	stInReg.d2 = nBuffSize;		/* �o�b�t�@�T�C�Y(l) */
	stInReg.d3 = nAbort;		/* �A�{�[�g�t���O(w) */
	stInReg.d4 = nEffect;		/* ������ʃt���O(l) */
	stInReg.a1 = (int32_t)pMacsBuff;	/* MACS�f�[�^�̐擪�A�h���X */
	
	retReg = _iocs_trap15(&stInReg, &stOutReg);	/* Trap 15 */
	return retReg;
}

int32_t	MACS_Load(int8_t *sFileName, int32_t nFileSize)
{
	int32_t ret = 0;
	int8_t	*pBuff = NULL;
	int32_t nLoop;
	
	pBuff = (int8_t*)MyMalloc(nFileSize);	/* �������m�� */
	
	if(pBuff != NULL)
	{
		File_Load(sFileName, pBuff, sizeof(uint8_t), nFileSize );	/* �t�@�C���ǂݍ��݂��烁�����֕ۑ� */
		
		nLoop = g_nRepeat;
		do
		{
			if(g_nRepeat == 0)
			{
				nLoop = 1;
			}
			else
			{
				nLoop--;
			}
			
			ret = MACS_Play(pBuff, g_nCommand, g_nBuffSize, g_nAbort, g_nEffect);	/* �Đ� */
			if(ret < 0)
			{
				break;
			}
		}
		while(nLoop);
		
		MyMfree(pBuff);		/* ��������� */
	}
	
	return ret;
}

int32_t	MACS_Load_Hi(int8_t *sFileName, int32_t nFileSize)
{
	int32_t ret = 0;
	int8_t	*pBuff = NULL;
	int32_t nLoop;
	
	pBuff = (int8_t*)MyMallocHi(nFileSize);	/* �������m�� */
	
	if(pBuff != NULL)
	{
		File_Load(sFileName, pBuff, sizeof(uint8_t), nFileSize );	/* �t�@�C���ǂݍ��݂��烁�����֕ۑ� */
		
		nLoop = g_nRepeat;
		do
		{
			if(g_nRepeat == 0)
			{
				nLoop = 1;
			}
			else
			{
				nLoop--;
			}
			ret = MACS_Play(pBuff, g_nCommand, g_nBuffSize, g_nAbort, g_nEffect);	/* �Đ� */
			if(ret < 0)
			{
				break;
			}
		}
		while(nLoop);

		MyMfreeHi(pBuff);	/* ��������� */
	}
	
	return ret;
}

int32_t	MACS_CHK(void)
{
	int32_t ret = 0;	/* 0:�풓 !0:�풓���Ă��Ȃ� */
	
	int32_t addr_IOCS;
	int32_t addr_MACSDRV;
	int32_t data;
	
	addr_IOCS = ((0x100 + _MACS_CALL) * 4);
	
	addr_MACSDRV = _iocs_b_lpeek((int32_t *)addr_IOCS);
	addr_MACSDRV += 2;
	
	data = _iocs_b_lpeek((int32_t *)addr_MACSDRV);
	if(data != 'MACS')ret |= 1;	/* �A���}�b�` */
	
	addr_MACSDRV += sizeof(int32_t);
	
	data = _iocs_b_lpeek((int32_t *)addr_MACSDRV);
	if(data != 'IOCS')ret |= 1;	/* �A���}�b�` */
	
	return ret;
}

int32_t	HIMEM_CHK(void)
{
	int32_t ret = 0;	/* 0:�풓 !0:�풓���Ă��Ȃ� */
	
	int32_t addr_IOCS;
	int32_t addr_HIMEM;
	int32_t data;
	int8_t data_b;
	
	addr_IOCS = ((0x100 + _HIMEM) * 4);
	
	addr_HIMEM = _iocs_b_lpeek((int32_t *)addr_IOCS);
	addr_HIMEM -= 6;
	
	data = _iocs_b_lpeek((int32_t *)addr_HIMEM);
	if(data != 'HIME')ret |= 1;	/* �A���}�b�` */
//	printf("data %x == %x\n", data, 'HIME');
	
	addr_HIMEM += 4;
	
	data_b = _iocs_b_bpeek((int32_t *)addr_HIMEM);
	if(data_b != 'M')ret |= 1;	/* �A���}�b�` */
//	printf("data %x == %x\n", data_b, 'M');
	
	return ret;
}

int32_t	PCM8A_CHK(void)
{
	struct	_regs	stInReg = {0}, stOutReg = {0};
	int32_t	retReg;	/* d0 */
	
	stInReg.d0 = 0x67;			/* IOCS _ADPCMMOD($67) */
	stInReg.d1 = 'PCMA';		/* PCM8A�̏풓�m�F */
	
	retReg = _iocs_trap15(&stInReg, &stOutReg);	/* Trap 15 */
	
	return retReg;
}

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

void HelpMessage(void)
{
	puts("MACS�f�[�^�̍Đ����s���܂��B");
	puts("  68030�ȍ~�Ɍ��胁�C��������������Ȃ��ꍇ�̓n�C���������g���čĐ����s���܂��B");
	puts("  ���n�C��������ADPCM���Đ�����ۂ́APCM8A.X -w18 �ł̏풓");
	puts("    �������́A060turbo.sys -ad ��ݒ肭�������B");
	puts("");
	puts("Usage:MACSplay.x [Option] file");
	puts("Option:-D  �p���b�g��50%�Â�����");
	puts("       -P  PCM�̍Đ������Ȃ�");
	puts("       -K  �L�[���͎��A�I�����Ȃ�");
	puts("       -C  �A�j���I�����A��ʂ����������Ȃ�");
	puts("       -SL MACSDRV���X���[�v���Ă��鎞�ł��Đ�����");
	puts("       -GR ���̨����\�������܂܍Đ�����");
	puts("       -SP ���ײĂ�\�������܂܍Đ�����");
	puts("       -AD PCM8A.X�̏풓���`�F�b�N���Ȃ�(060turbo.sys -ad�ōĐ�����ꍇ)");
	puts("       -Rx ���s�[�g�Đ�����x=1�`255��Ax=0(�������[�v)");
}

int16_t main(int16_t argc, int8_t *argv[])
{
	int16_t ret = 0;
	int32_t nOut = 0;
	int32_t	nFileSize = 0;
	int32_t	nFilePos = 0;
	
	puts("MACS data Player�uMACSplay.x�vv1.01a (c)2022 �J�^.");
	
	if(argc > 1)	/* �I�v�V�����`�F�b�N */
	{
		int16_t i;
		
		for(i = 1; i < argc; i++)
		{
			int8_t	bOption;
			int8_t	bFlag;
			int8_t	bFlag2;
			
			bOption	= ((argv[i][0] == '-') || (argv[i][0] == '/')) ? TRUE : FALSE;

			if(bOption == TRUE)
			{
				bFlag	= ((argv[i][1] == '?') || (argv[i][1] == 'h') || (argv[i][1] == 'H')) ? TRUE : FALSE;
				
				if(bFlag == TRUE)
				{
					HelpMessage();	/* �w���v */
					ret = -1;
				}
				
				bFlag	= ((argv[i][1] == 'd') || (argv[i][1] == 'D')) ? TRUE : FALSE;
				if(bFlag == TRUE)
				{
					g_nEffect = Mbset(g_nEffect, 0x7F, Bit_0);
					puts("�p���b�g��50%�Â����܂�");
					continue;
				}

				bFlag	= ((argv[i][1] == 'p') || (argv[i][1] == 'P')) ? TRUE : FALSE;
				if(bFlag == TRUE)
				{
					g_nEffect = Mbset(g_nEffect, 0x7F, Bit_1);
					puts("PCM�̍Đ������܂���");
					continue;
				}

				bFlag	= ((argv[i][1] == 'k') || (argv[i][1] == 'K')) ? TRUE : FALSE;
				if(bFlag == TRUE)
				{
					g_nEffect = Mbset(g_nEffect, 0x7F, Bit_2);
					puts("�L�[���͎��A�I�����܂���");
					continue;
				}

				bFlag	= ((argv[i][1] == 'c') || (argv[i][1] == 'C')) ? TRUE : FALSE;
				if(bFlag == TRUE)
				{
					g_nEffect = Mbset(g_nEffect, 0x7F, Bit_3);
					puts("�A�j���I�����A��ʂ����������Ȃ�");
					continue;
				}
				
				bFlag	= ((argv[i][1] == 's') || (argv[i][1] == 'S')) ? TRUE : FALSE;
				bFlag2	= ((argv[i][2] == 'l') || (argv[i][2] == 'L')) ? TRUE : FALSE;
				if((bFlag == TRUE) && (bFlag2 == TRUE))
				{
					g_nEffect = Mbset(g_nEffect, 0x7F, Bit_4);
					puts("MACSDRV���X���[�v���Ă��鎞�ł��Đ�����");
					continue;
				}

				bFlag	= ((argv[i][1] == 'g') || (argv[i][1] == 'G')) ? TRUE : FALSE;
				bFlag2	= ((argv[i][2] == 'r') || (argv[i][2] == 'R')) ? TRUE : FALSE;
				if((bFlag == TRUE) && (bFlag2 == TRUE))
				{
					g_nEffect = Mbset(g_nEffect, 0x7F, Bit_5);
					puts("���̨����\�������܂܍Đ�����");
					continue;
				}
				
				bFlag	= ((argv[i][1] == 's') || (argv[i][1] == 'S')) ? TRUE : FALSE;
				bFlag2	= ((argv[i][2] == 'p') || (argv[i][2] == 'P')) ? TRUE : FALSE;
				if((bFlag == TRUE) && (bFlag2 == TRUE))
				{
					g_nEffect = Mbset(g_nEffect, 0x7F, Bit_6);
					puts("���ײĂ�\�������܂܍Đ�����");
					continue;
				}

				bFlag	= ((argv[i][1] == 'a') || (argv[i][1] == 'A')) ? TRUE : FALSE;
				bFlag2	= ((argv[i][2] == 'd') || (argv[i][2] == 'D')) ? TRUE : FALSE;
				if((bFlag == TRUE) && (bFlag2 == TRUE))
				{
					g_nPCM8Achk = 1;
					puts("PCM8A.X�̏풓���`�F�b�N���Ȃ�");
					continue;
				}

				bFlag	= ((argv[i][1] == 'r') || (argv[i][1] == 'R')) ? TRUE : FALSE;
				bFlag2	= strlen(&argv[i][2]);
				if((bFlag == TRUE) && (bFlag2 > 0))
				{
					g_nRepeat = atoi(&argv[i][2]);
					if(g_nRepeat < 0)			g_nRepeat = 1;
					else if(g_nRepeat > 255)	g_nRepeat = 1;

					g_nEffect = Mbclr(g_nEffect, Bit_2);	/* -K �L�[���͏I����L���ɂ��� */
					g_nEffect = Mbclr(g_nEffect, Bit_3);	/* -C �A�j���I������ʂ����������Ȃ���L���ɂ��� */
					if(g_nRepeat == 0)
					{
						puts("�������[�v�ōĐ�����");
					}
					else
					{
						printf("%d�񃊃s�[�g�Đ�����\n", g_nRepeat);
					}
					continue;
				}
				
				HelpMessage();	/* �w���v */
				ret = -1;
				break;
			}
			else
			{
				if(GetFileLength(argv[i], &nFileSize) < 0)	/* �t�@�C���̃T�C�Y�擾 */
				{
					printf("�t�@�C����������܂���ł����B%s\n", argv[i]);
				}
				else
				{
					nFilePos = i;
					printf("File Size = %d[MB](%d[KB], %d[byte])\n", nFileSize>>20, nFileSize>>10, nFileSize);
				}
			}
		}
	}
	else
	{
		HelpMessage();	/* �w���v */
		ret = -1;
	}
	
	if(MACS_CHK() != 0)
	{
		puts("error�FMACSDRV���풓���Ă��������I");
		ret = -1;
	}
	
	if(ret == 0)
	{
		//printf("Option = 0x%x\n", g_nEffect);

		if(nFileSize != 0)
		{
			int32_t	nMemSize;
			int32_t	nHiMemChk;
			
			nMemSize = (int32_t)_dos_malloc(-1) - 0x81000000UL;	/* ���C���������̋󂫃`�F�b�N */
			nHiMemChk = HIMEM_CHK();	/* �n�C�����������`�F�b�N */
			
			if(nFileSize <= nMemSize)	/* ���C�������� */
			{
				printf("���C���������ōĐ����܂�(%d[kb] <= %d[kb])\n", nFileSize>>10, nMemSize>>10);
				nOut = MACS_Load(argv[nFilePos], nFileSize);		/* ���C���Đ� */
			}
			else if(nHiMemChk == 0)		/* �n�C���������� */
			{
				int32_t	nChk = PCM8A_CHK();
				
				if( (nChk >= 0) || (g_nPCM8Achk > 0))	/* PCM8A�풓�`�F�b�N or -AD�Ŗ����� */
				{
					puts("�n�C�������ōĐ����܂�");
					nOut = MACS_Load_Hi(argv[nFilePos], nFileSize);	/* �n�C�������Đ� */
				}
				else
				{
					if(nChk < 0)	/* >=0:�풓 <0:�풓���Ă��Ȃ� */
					{
						puts("error:PCM8A���풓����Ă��܂���");
						puts("���n�C���������g���ꍇ�́APCM8A -w18 �ŏ풓���������B");
					}
				}
			}
			else						/* �n�C������ or NG */
			{
				puts("error�F���C���������̋󂫂��s���ł��B");
				puts("error�FHIMEM��������܂���ł����B");
				ret = -1;
			}
		}
	}
	
	if(nOut < 0)
	{
		switch(nOut)
		{
		case -4:
			puts("�Đ��𒆒f���܂����B");
			break;
		default:
			//printf("MACSplay.x = %d\n", nOut);
			break;
		}
	} 
	ADPCM_Stop();	/* �����~�߂� */
	
	return ret;
}