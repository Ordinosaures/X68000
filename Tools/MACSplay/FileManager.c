#ifndef	FILEMANAGER_C
#define	FILEMANAGER_C

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <io.h>
#include <time.h>
#include <doslib.h>
#include <iocslib.h>

#include "inc/usr_macro.h"

#include "FileManager.h"


/* �֐��̃v���g�^�C�v�錾 */
int16_t File_Load(int8_t *, void *, size_t, size_t);
int16_t File_Save(int8_t *, void *, size_t, size_t);
int16_t GetFileLength(int8_t *, int32_t *);
void *MyMalloc(int32_t);
void *MyMallocHi(int32_t);
int16_t MyMfree(void *);
int16_t	MyMfreeHi(void *);
int32_t	MaxMemSize(int8_t);

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
/* �t�@�C���ǂݍ��� */
/* *fname	�t�@�C���� */
/* *ptr		�i�[��̐擪�A�h���X */
/* size		�f�[�^�̃T�C�Y */
/* n		�f�[�^�̌� */
int16_t File_Load(int8_t *fname, void *ptr, size_t size, size_t n)
{
	FILE *fp;
	int16_t ret = 0;

	/* �t�@�C�����J���� */
	fp = fopen(fname, "rb");
	
	if(fp == NULL)
	{
		/* �t�@�C�����ǂݍ��߂܂��� */
		printf("error:%s�t�@�C����������܂���I\n", fname);
		ret = -1;
	}
	else
	{
		/* �f�[�^�����w�肵�Ȃ��ꍇ */
		if(n == 0)
		{
			/* �t�@�C���T�C�Y���擾 */
			n = filelength(fileno(fp));
		}
		
		/* �t�@�C���ǂݍ��� */
		ret = fread (ptr, size, n, fp);

		/* �t�@�C������� */
		fclose (fp);
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
/* �t�@�C���ۑ� */
/* *fname	�t�@�C���� */
/* *ptr		�i�[��̐擪�A�h���X */
/* size		�f�[�^�̃T�C�Y */
/* n		�f�[�^�̌� */
int16_t File_Save(int8_t *fname, void *ptr, size_t size, size_t n)
{
	FILE *fp;
	int16_t ret = 0;

	/* �t�@�C�����J���� */
	fp = fopen(fname, "rb");
	
	if(fp == NULL)	/* �t�@�C�������� */
	{
		/* �t�@�C�����J���� */
		fp = fopen(fname, "wb");
		fwrite(ptr, size, n, fp);
	}
	else
	{
		/* �t�@�C�������݂���ꍇ�͉������Ȃ� */
	}
	/* �t�@�C������� */
	fclose (fp);

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	GetFileLength(int8_t *pFname, int32_t *pSize)
{
	FILE *fp;
	int16_t ret = 0;
	int32_t	Tmp;

	fp = fopen( pFname , "rb" ) ;
	if(fp == NULL)		/* Error */
	{
		ret = -1;
	}
	else
	{
		Tmp = fileno( fp );
		if(Tmp == -1)	/* Error */
		{
			*pSize = 0;
			ret = -1;
		}
		else
		{
			Tmp = filelength( Tmp );
			if(Tmp == -1)		/* Error */
			{
				*pSize = 0;
				ret = -1;
			}
			else
			{
				*pSize = Tmp;
//				printf("GetFileLength = (%4d, %4d)\n", *pSize, Tmp );
			}
		}

		fclose( fp ) ;
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
void *MyMalloc(int32_t Size)
{
	void *pPtr = NULL;
	
	if(Size >= 0x1000000u)
	{
		printf("error:�������m�ۃT�C�Y���傫�����܂�(0x%x)\n", Size );
	}
	else
	{
		pPtr = _dos_malloc(Size);	/* �������m�� */
//		pPtr = malloc(Size);	/* �������m�� */
		
		if(pPtr == NULL)
		{
			puts("error:���������m�ۂł��܂���ł���");
		}
		else if((uint32_t)pPtr >= 0x81000000)
		{
			if((uint32_t)pPtr >= 0x82000000)
			{
				puts("error:�������s���ł�");
			}
			else
			{
				printf("error:���������m�ۂł��܂���ł���(%d)\n", (uint32_t)pPtr - 0x81000000 );
			}
			pPtr = NULL;
		}
		else
		{
			//printf("Mem Address 0x%p Size = %d[byte]\n", pPtr, Size);
			//printf("�������A�h���X(0x%p)=%d\n", pPtr, Size);
		}
	}
	
	return pPtr;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void *MyMallocHi(int32_t Size)
{
	void *pPtr = NULL;

	struct	_regs	stInReg = {0}, stOutReg = {0};
	int32_t		retReg;	/* d0 */
	
	stInReg.d0 = 0xF8;					/* IOCS _HIMEM */
	stInReg.d1 = 0x03;					/* HIMEM_GETSIZE */
	
	retReg = _iocs_trap15(&stInReg, &stOutReg);	/* Trap 15 */
	if(stOutReg.d0 == 0)
	{
		printf("error:�������T�C�Y%d[MB](%d[byte])/��x�Ɋm�ۂł���ő�̃T�C�Y %d[byte]\n", stOutReg.d0 >> 20u, stOutReg.d0, stOutReg.d1);
		return pPtr;
	}
	
	
	stInReg.d0 = 0xF8;					/* IOCS _HIMEM */
	stInReg.d1 = 0x01;					/* HIMEM_MALLOC */
	stInReg.d2 = Size;					/* �T�C�Y */
	
	retReg = _iocs_trap15(&stInReg, &stOutReg);	/* Trap 15 */
	if(stOutReg.d0 == 0)
	{
		pPtr = (void *)stOutReg.a1;	/* �m�ۂ����������̃A�h���X */
		//printf("HiMem Address 0x%p Size = %d[byte]\n", pPtr, Size);
	}
	else
	{
		puts("error:���������m�ۂ��邱�Ƃ��o���܂���ł���");
	}
	return pPtr;
}


/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	MyMfree(void *pPtr)
{
	int16_t ret = 0;
	uint32_t	result;
	
	if(pPtr == 0)
	{
		puts("���v���Z�X�A�q�v���Z�X�Ŋm�ۂ������������t���ŉ�����܂�");
	}
	
	result = _dos_mfree(pPtr);
	//free(pPtr);
	
	if(result < 0)
	{
		puts("error:����������Ɏ��s");
		ret = -1;
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
int16_t	MyMfreeHi(void *pPtr)
{
	int16_t ret = 0;
	struct	_regs	stInReg = {0}, stOutReg = {0};
	int32_t	retReg;	/* d0 */
	
	if(pPtr == NULL)
	{
		puts("���v���Z�X�A�q�v���Z�X�Ŋm�ۂ������������t���ŉ�����܂�");
	}
	
	stInReg.d0 = 0xF8;					/* IOCS _HIMEM */
	stInReg.d1 = 0x02;					/* HIMEM_FREE */
	stInReg.d2 = (int32_t)pPtr;			/* �m�ۂ����������̐擪�A�h���X */
	
	retReg = _iocs_trap15(&stInReg, &stOutReg);	/* Trap 15 */
	if(stOutReg.d0 < 0)
	{
		puts("error:����������Ɏ��s");
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
int32_t	MaxMemSize(int8_t SizeType)
{
	int32_t ret = 0;
	int32_t i, dummy;
	int32_t chk[2];
	int8_t *ptMem[2];
	
	ptMem[0] = (int8_t *)0x0FFFFF;
	ptMem[1] = (int8_t *)0x100000;
	
	do{
		for(i=0; i<2; i++)
		{
			if((int32_t)ptMem[i] >= 0xC00000)	/* 12MB�̏�� */
			{
				chk[0] = 0;	/* �������[�v�E�o */
				chk[1] = 2;	/* �������[�v�E�o */
				break;
			}
			else
			{
				chk[i] = _dos_memcpy(ptMem[i], &dummy, 1);	/* �o�X�G���[�`�F�b�N */
			}
		}
		
		/* �����������̋��E */
		if( (chk[0] == 0) &&	/* �ǂݏ����ł��� */
			(chk[1] == 2) )		/* �o�X�G���[ */
		{
			break;	/* ���[�v�E�o */
		}
		
		ptMem[0] += 0x100000;	/* +1MB ���Z */
		ptMem[1] += 0x100000;	/* +1MB ���Z */
	}while(1);
	
//	printf("Memory Size = %d[MB](%d[Byte])(0x%p)\n", ((int)ptMem[1])>>20, ((int)ptMem[1]), ptMem[0]);

	switch(SizeType)
	{
	case 0:	/* Byte */
		ret = ((int)ptMem[1]);
		break;
	case 1:	/* KByte */
		ret = ((int)ptMem[1])>>10;
		break;
	case 2:	/* MByte */
	default:
		ret = ((int)ptMem[1])>>20;
		break;
	}
	
	return ret;
}

#endif	/* FILEMANAGER_C */

