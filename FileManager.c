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
#include "Graphic.h"

/* �o�b�f�f�[�^ */
static UC	*pcg_dat; /* �o�b�f�f�[�^�t�@�C���ǂݍ��݃o�b�t�@ */
static US	pal_dat[ 128 ]; /* �p���b�g�f�[�^�t�@�C���ǂݍ��݃o�b�t�@ */

/* �֐��̃v���g�^�C�v�錾 */
SS File_Load(SC *, void *, size_t, size_t);
SS File_Load_CSV(SC *, US *, US *, US *);
SS PCG_SP_dataload(SC *);
SS PCG_PAL_dataload(SC *);
SS Load_Music_List(	SC *, SC *, SC (*)[256], UI *);
SS Load_SE_List(	SC *, SC *, SC (*)[256], UI *);
SS Load_CG_List(	SC *, SC *, CG_LIST *, UI *);
SS Load_MACS_List(	SC *, SC *, SC (*)[256], UI *);
SS GetFileLength(SC *, SI *);
SS GetFilePICinfo(SC *, BITMAPINFOHEADER *);
SS GetRectangleSise(US *, US, US, US);
void *MyMalloc(SI);
SS MyMfree(void *);

/* �t�@�C���ǂݍ��� */
/* *fname	�t�@�C���� */
/* *ptr		�i�[��̐擪�A�h���X */
/* size		�f�[�^�̃T�C�Y */
/* n		�f�[�^�̌� */
SS File_Load(SC *fname, void *ptr, size_t size, size_t n)
{
	FILE *fp;
	SS ret = 0;

	/* �t�@�C�����J���� */
	fp = fopen(fname, "rb");
	
	if(fp == NULL)
	{
		/* �t�@�C�����ǂݍ��߂܂��� */
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

/* CSV�t�@�C���ǂݍ��� */
/* *fname	�t�@�C���� */
/* *ptr		�i�[��̐擪�A�h���X */
/* *Col		�f�[�^�̍s���̃A�h���X */
/* *Row		�f�[�^�̗񐔂̃A�h���X */
SS File_Load_CSV(SC *fname, US *ptr, US *Col, US *Row)
{
	FILE *fp;
	SS ret = 0;
	US x, y, flag, cnv_flag;
	char buf[1000], *p, *end;
	
	x = 0;
	y = 0;
	flag = 0;
	cnv_flag = 0;
	
	fp = fopen(fname, "r");
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		while(fgets(buf, sizeof(buf), fp) != NULL)
		{
	        p = buf;
			do
			{
				SL num = strtol(p, &end, 0);

//				printf("(%d,%d)=%d ->%s", x, y, num, end);

				if(*end == '\0')
				{
					/* ������̏I��肾���� */
					if(flag==0)
					{
						*Col = x;
					}
					flag = 1;
					break;
				}
				else if(p != end)
				{
					/* �ϊ��ł��� */
					*ptr = (US)num;
					ptr++;

					p = end + 1;
					
					cnv_flag = 1;
					x++;
				}
				else
				{
					/* �ϊ��ł��Ȃ����� */
					p = end + 1;
				}
			}
			while(1);
			
			if(cnv_flag != 0){
				cnv_flag = 0;
				y++;
			}
			x = 0;
		}
		fclose(fp);
	}
	
	*Row = y;
	
	return ret;
}

/* CSV �R�[�X�f�[�^�t�@�C���ǂݍ��� */
/* *fname	�t�@�C���� */
/* *st_ptr	�i�[��̐擪�A�h���X */
/* *Col		�f�[�^�̍s���̃A�h���X */
/* *Row		�f�[�^�̗񐔂̃A�h���X */
SS File_Load_Course_CSV(SC *fname, ST_ROADDATA *st_ptr, US *Col, US *Row)
{
	FILE *fp;
	SS ret = 0;
	US x, y, flag, cnv_flag;
	char buf[1000], *p, *end;
	
	x = 0;
	y = 0;
	flag = 0;
	cnv_flag = 0;
	
	fp = fopen(fname, "r");
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		while(fgets(buf, sizeof(buf), fp) != NULL)
		{
	        p = buf;
			do
			{
				SL num = strtol(p, &end, 0);

//				printf("(%d,%d)=%d ->%s\n", x, y, num, end);

				if(*end == '\0')
				{
					/* ������̏I��肾���� */
					st_ptr++;
					if(flag==0)
					{
						*Col = x;
					}
					flag = 1;
					break;
				}
				else if(p != end)
				{
					/* �ϊ��ł��� */
					switch(x)
					{
						case 0:
						{
							st_ptr->bHeight = (UC)num;
							break;
						}
						case 1:
						{
							st_ptr->bWidth = (UC)num;
							break;
						}
						case 2:
						{
							st_ptr->bAngle = (UC)num;
							break;
						}
						case 3:
						{
							st_ptr->bfriction = (UC)num;
							break;
						}
						case 4:
						{
							st_ptr->bPat = (UC)num;
							break;
						}
						case 5:
						{
							st_ptr->bObject = (UC)num;
							break;
						}
						case 6:
						{
							st_ptr->bRepeatCount = (UC)num;
							break;
						}
						default:
						{
							break;
						}
					}

					p = end + 1;
					
					cnv_flag = 1;
					x++;
				}
				else
				{
					/* �ϊ��ł��Ȃ����� */
					p = end + 1;
				}
			}
			while(1);
			
			if(cnv_flag != 0){
				cnv_flag = 0;
				y++;
			}
			x = 0;
		}
		fclose(fp);
	}
	
	*Row = y;
	
	return ret;
}

SS PCG_SP_dataload(SC *fname)
{
	SS ret = 0;

	FILE *fp;

	/*-----------------[ �o�b�f�f�[�^�ǂݍ��� ]-----------------*/

	fp = fopen( fname , "rb" ) ;
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		SI	pcg_size;
		pcg_size = filelength( fileno( fp ) );
		
		pcg_dat = NULL;
		pcg_dat = (UC*)MyMalloc( pcg_size );
		if(pcg_dat != NULL)
		{
			UI i,j;
			
			j = fread( pcg_dat
				,  128		/* 1PCG = 128byte */
				,  PCG_MAX	/* 256PCG */
				,  fp
				) ;
			fclose( fp ) ;

			for( i = 0 ; i < PCG_MAX ; i++ ){
				SP_DEFCG( i, 1, pcg_dat );
				pcg_dat += 128;
			}
		}
	}
	
	return ret;
}

SS PCG_PAL_dataload(SC *fname)
{
	SS ret = 0;

	FILE *fp;
	UI i;

	/*--------[ �X�v���C�g�p���b�g�f�[�^�ǂݍ��݂ƒ�` ]--------*/

	fp = fopen( fname , "rb" ) ;
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		fread( pal_dat
			,  2		/* 1palet = 2byte */
			,  256		/* 16palet * 16block */
			,  fp
			) ;
		fclose( fp ) ;

		/* �X�v���C�g�p���b�g�ɓ]�� */
		for( i = 0 ; i < 256 ; i++ )
		{
			SPALET( (i&15) | (1<<0x1F) , Mdiv16(i)+1 , pal_dat[i] ) ;
		}
	}
	
	return ret;
}

SS Load_Music_List(SC *fpath, SC *fname, SC (*music_list)[256], UI *list_max)
{
	FILE *fp;
	SS ret = 0;
	SC buf[1000], *p;
	SC z_name[256];
	UI i=0, num=0;
	
	sprintf(z_name, "%s%s", fpath, fname);
	fp = fopen(z_name, "r");
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		i = 0;
		
		while(fgets(buf, sizeof(buf), fp) != NULL)
		{
	        p = buf;
			sscanf(p,"%d = %s", &num, z_name);
			if(i == num)
			{
				sprintf(music_list[i], "%s%s", fpath, z_name);
			}
			i++;
		}
		fclose(fp);
	}
	*list_max = i;
	
	return ret;
}

SS Load_SE_List(SC *fpath, SC *fname, SC (*music_list)[256], UI *list_max)
{
	FILE *fp;
	SS ret = 0;
	SC buf[1000], *p;
	SC z_name[256];
	UI i=0, num=0;
	
	sprintf(z_name, "%s%s", fpath, fname);
	fp = fopen(z_name, "r");
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		i = 0;
		
		while(fgets(buf, sizeof(buf), fp) != NULL)
		{
	        p = buf;
			sscanf(p,"%d = %s", &num, z_name);
			if(i == num)
			{
				sprintf(music_list[i], "%s%s", fpath, z_name);
			}
			i++;
		}
		fclose(fp);
	}
	*list_max = i;
	
	return ret;
}

SS Load_CG_List(SC *fpath, SC *fname, CG_LIST *cg_list, UI *list_max)
{
	FILE *fp;
	SS ret = 0;
	SC buf[1000], *p;
	SC z_name[256];
	UI i=0, num=0, bType = 0, bTransPal = 0;
	
	sprintf(z_name, "%s%s", fpath, fname);
	fp = fopen(z_name, "r");
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		i = 0;
		
		while(fgets(buf, sizeof(buf), fp) != NULL)
		{
	        p = buf;
			sscanf(p,"%d= %s %d %d", &num, z_name, &bType, &bTransPal);
#ifdef DEBUG
//			printf("%d=%s,%d\n", num, z_name, bType);
//			KeyHitESC();	/* �f�o�b�O�p */
#endif
			if(i == num)
			{
				sprintf( cg_list->bFileName, "%s%s", fpath, z_name );
				cg_list->ubType = bType;
				cg_list->ubTransPal = bTransPal;
			}
			cg_list++;
			i++;
		}
		fclose(fp);
	}
	*list_max = i;
	
	return ret;
}

SS Load_MACS_List(SC *fpath, SC *fname, SC (*macs_list)[256], UI *list_max)
{
	FILE *fp;
	SS ret = 0;
	SC buf[1000], *p;
	SC z_name[256];
	UI i=0, num=0;
	
	sprintf(z_name, "%s%s", fpath, fname);
	fp = fopen(z_name, "r");
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		i = 0;
		
		while(fgets(buf, sizeof(buf), fp) != NULL)
		{
	        p = buf;
			sscanf(p,"%d = %s", &num, z_name);
			if(i == num)
			{
				sprintf(macs_list[i], "%s%s", fpath, z_name);
			}
			i++;
		}
		fclose(fp);
	}
	*list_max = i;
	
	return ret;
}

SS	GetFileLength(SC *pFname, SI *pSize)
{
	FILE *fp;
	SS ret = 0;
	SI	Tmp;

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

SS	GetFilePICinfo(SC *pFname, BITMAPINFOHEADER *info)
{
	FILE *fp;
	SS ret = 0;
	UC	bFlag = FALSE;
	UC	*pBuf;
	UC	sBuf[128] = {0};
	US	uWord;
	UI	i;

	fp = fopen( pFname , "rb" ) ;
	if(fp == NULL)
	{
		ret = -1;
	}
	else
	{
		memset(sBuf, 0, sizeof(UC) * 128);	/* �o�b�t�@�N���A */
		pBuf = &sBuf[0];	/* �擪�A�h���X */

		/* PIC */
		fread( pBuf, sizeof(UC), 3, fp );
		if(strncmp( pBuf, "PIC", 3 ) == 0)
		{
			/* ��v */
//			printf("%s�t�@�C���ł�\n", pBuf );
			bFlag = TRUE;
		}
		else
		{
			bFlag = FALSE;
			printf("error:PIC�t�@�C���ł͂���܂��� = %s\n", pBuf );
		}
		
		memset(sBuf, 0, sizeof(UC) * 128);	/* �o�b�t�@�N���A */
		pBuf = &sBuf[0];	/* �擪�A�h���X */

		/* �g���w�b�_���H */
		fread( pBuf, sizeof(UC), 3, fp );
		if(strncmp( pBuf, "/MM", 3 ) == 0)
		{
			/* ��v */
//			printf("�g���t�@�C���ł�\n", pBuf );

			memset(sBuf, 0, sizeof(UC) * 128);	/* �o�b�t�@�N���A */
			pBuf = &sBuf[0];	/* �擪�A�h���X */

			for(i=0; i < 128; i++)
			{
				/* �g���w�b�_���� */
				fread( pBuf, sizeof(UC), 1, fp );
				if(strncmp( pBuf, "/", 1 ) == 0)
				{
					*pBuf = 0x00;
//					printf("%s\n", &sBuf[0]);
					
					memset(sBuf, 0, sizeof(UC) * 128);	/* �o�b�t�@�N���A */
					pBuf = &sBuf[0];	/* �擪�A�h���X */

					fread( pBuf, sizeof(UC), 1, fp );
					/* �g���w�b�_�I������ */
					if(strncmp( &sBuf[0], ":", 1 ) == 0)
					{
						break;
					}
					else
					{
						pBuf++;
					}
					
					fread( pBuf, sizeof(UC), 1, fp );
					/* �摜�f�[�^�̍�Ҕ��� */
					if(strncmp( &sBuf[0], "AU", 2 ) == 0)
					{
//						printf("�摜�f�[�^�̍��:");
						memset(sBuf, 0, sizeof(UC) * 128);	/* �o�b�t�@�N���A */
						pBuf = &sBuf[0];	/* �擪�A�h���X */
					}
					/* �摜�̃��[�h���W */
					else if(strncmp( &sBuf[0], "XY", 2 ) == 0)
					{
//						printf("XY:");
						memset(sBuf, 0, sizeof(UC) * 128);	/* �o�b�t�@�N���A */
						pBuf = &sBuf[0];	/* �擪�A�h���X */
					}
					else
					{
						pBuf++;
					}
				}
				else
				{
					pBuf++;
				}
			}
			
			memset(sBuf, 0, sizeof(UC) * 128);	/* �o�b�t�@�N���A */
			pBuf = &sBuf[0];	/* �擪�A�h���X */

			/* �R�����g�I���܂œǂݍ��� */
			for(i=0; i < 128; i++)
			{
				fread( pBuf, sizeof(UC), 1, fp);
				if(*pBuf == 0x1Au)
				{
					if(i != 0u)
					{
//						printf("�R�����g(%d)�F%s\n", i, sBuf[0] );
					}
					bFlag = TRUE;
					break;
				}
				pBuf++;
			}
		}
		else
		{
			/* ���ʂ̃w�b�_�t�@�C���ł��� */
			pBuf += 3;
			bFlag = FALSE;

			for(i=0; i < 128; i++)
			{
				fread( pBuf, sizeof(UC), 1, fp);
				if(*pBuf == 0x1Au)
				{
//					printf("�R�����g(%d)�F%s\n", i, sBuf[0] );
					bFlag = TRUE;
					break;
				}
				pBuf++;
			}
		}

		/* �_�~�[ */
		if(bFlag == TRUE)
		{
			bFlag = FALSE;

			memset(sBuf, 0, sizeof(UC) * 128);	/* �o�b�t�@�N���A */
			pBuf = &sBuf[0];	/* �擪�A�h���X */
			
			while(1)
			{
				fread( pBuf, sizeof(UC), 1, fp);
				if(*pBuf == 0x00u)
				{
					bFlag = TRUE;
					break;
				}
				pBuf++;
			}
		}

		/* �\�񕶎�(0) */
		if(bFlag == TRUE)
		{
			bFlag = FALSE;
			fread( pBuf, sizeof(UC), 1, fp);
			if(*pBuf == 0x00u)
			{
				bFlag = TRUE;
			}
		}

		/* �^�C�v�E���[�h */
		if(bFlag == TRUE)
		{
			bFlag = FALSE;
			fread( pBuf, sizeof(UC), 1, fp);
			if(*pBuf == 0x00u)
			{
//				printf("Type/Mode�F%d\n", *pBuf );
				bFlag = TRUE;
			}
		}
		
		if(bFlag == TRUE)
		{
			/* �F�̃r�b�g�� */
			fread( &uWord, sizeof(US), 1, fp);
			info->biBitCount = uWord;
//			printf("color bit�F%d\n", uWord );

			/* �w�����̃T�C�Y */
			fread( &uWord, sizeof(US), 1, fp);
			info->biWidth	= (SL)uWord;
//			printf("X,Y�F(%4d,", uWord );

			/* �x�����̃T�C�Y */
			fread( &uWord, sizeof(US), 1, fp);
			info->biHeight	= (SL)uWord;
//			printf("%4d)\n", uWord );
		}

		fclose( fp ) ;
	}
	
	return ret;
}

SS	GetRectangleSise(US *uSize, US uWidth, US uHeight, US uMaxSize)
{
	SS ret = 0;
	
	*uSize = Mmax( ((((uWidth+ 7u) & 0xFFF8u) * uHeight) << uMaxSize),  (512u << uMaxSize) );
	
	return ret;
}

void *MyMalloc(SI Size)
{
	void *pPtr = NULL;
	
	if(Size >= 0x1000000u)
	{
		printf("�������m�ۃT�C�Y���傫�������I(0x%x)\n", Size );
	}
	else
	{
		pPtr = _dos_malloc(Size);	/* �������m�� */
//		pPtr = malloc(Size);	/* �������m�� */
		
		if(pPtr == NULL)
		{
			puts("���������m�ۂł��܂���ł���");
		}
		else if((UI)pPtr >= 0x81000000)
		{
			if((UI)pPtr >= 0x82000000)
			{
				puts("�������s���ł�");
			}
			else
			{
				printf("���������m�ۂł��܂���ł���(%d)\n", (UI)pPtr - 0x81000000 );
			}
			pPtr = NULL;
		}
		else
		{
			//printf("�������A�h���X(0x%p)=%d\n", pPtr, Size);
		}
	}
	
	return pPtr;
}

SS	MyMfree(void *pPtr)
{
	SS ret = 0;
	UI	result;
	
	if(pPtr == 0)
	{
		puts("���v���Z�X�A�q�v���Z�X�Ŋm�ۂ������������t���ŉ�����܂�");
	}
	
	result = _dos_mfree(pPtr);
	//free(pPtr);
	
	if(result < 0)
	{
		ret = -1;
	}
	
	return ret;
}
#endif	/* FILEMANAGER_C */

