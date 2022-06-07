#include <stdio.h>
#include <stdlib.h>
#include "usr_style.h"
#include "usr_macro.h"

#define VSYNC_sec	(1.0f/55.45767f)

int16_t CGcnv(int16_t, int16_t, int16_t);
int16_t BLTcnv(int16_t, int16_t, int16_t, int16_t);
int16_t MACScnv(int16_t, int16_t, int16_t, int16_t, int16_t);
int16_t main(int16_t, int8_t**);

int16_t CGcnv(int16_t start_cnt, int16_t inc_val, int16_t end_cnt)
{
	FILE *fp;
	int8_t fname[256];
	int16_t ret = 0;
	int16_t cnt;
	
	int8_t sStingDef[16] = "screen 1,3";
	int8_t sStingMes[4][16] = {	"APICG",
								"YGMC /T4 /Z /B",
								"txs -p"
							};

	/* �t�@�C�����J���� */
	strcpy(fname, "0_MakeTxTp.bat");
	fp = fopen(fname, "r");
	if(fp != NULL)
	{
		printf("�t�@�C�������ɑ��݂��܂��B(%s)\n", fname);
		printf("__Clean.bat �����s���Ă�������\n");
		/* �t�@�C������� */
		fclose (fp);
		return -1;
	}
	
	fp = fopen(fname, "w");
	/* �t�@�C���ɏ������� */
	fprintf(fp, "%s\n\n", sStingDef);
	
	for(cnt = start_cnt; cnt <= end_cnt; cnt += inc_val)
	{
		/* �t�@�C���ɏ������� */
		fprintf(fp, "%s CG%05d.bmp\n", sStingMes[0], cnt);
		fprintf(fp, "%s\n", sStingMes[1]);
		fprintf(fp, "%s Tx%05d Tp%05d\n\n", sStingMes[2], cnt, cnt);
	}

	/* �t�@�C������� */
	fclose (fp);
	
	printf("%s ���쐬���܂����B\n",fname);
	
	printf("---------------------------\n");

	/* �t�@�C�����J���� */
	strcpy(fname, "1_MakePCM.bat");
	fp = fopen(fname, "r");
	if(fp != NULL)
	{
		printf("�t�@�C�������ɑ��݂��܂��B(%s)\n", fname);
		printf("__Clean.bat �����s���Ă�������\n");
		/* �t�@�C������� */
		fclose (fp);
		return -1;
	}
	
	fp = fopen(fname, "w");
	/* �t�@�C���ɏ������� */
	fprintf(fp, "pcm3pcm.x AD.WAV AD.PCM\n", sStingDef);
	/* �t�@�C������� */
	fclose (fp);
	
	printf("%s ���쐬���܂����B\n",fname);
	printf("---------------------------\n");
	
	return ret;
}

int16_t BLTcnv(int16_t start_cnt, int16_t inc_val, int16_t end_cnt, int16_t mode)
{
	FILE *fp;
	int16_t ret = 0;
	int16_t i, j, flag, cnt;
	int8_t fname[256];
	int8_t str[256];
	
	i = 0;
	j = 0;
	flag = 0;
	cnt = start_cnt;
	
	switch(mode)
	{
		case 2:
		{
			/* �t�@�C�����J���� */
			strcpy(fname, "LIST_PCM.blk");
			fp = fopen(fname, "r");
			if(fp != NULL)
			{
				printf("�t�@�C�������ɑ��݂��܂��B(%s)\n", fname);
				printf("__Clean.bat �����s���Ă�������\n");
				/* �t�@�C������� */
				fclose (fp);
				return -1;
			}
			
			fp = fopen(fname, "w");
			
			/* �t�@�C���ɏ������� */
			fprintf(fp, "pcmdat:AD.PCM\n");
			fprintf(fp, "pcmend:\n");

			/* �t�@�C������� */
			fclose (fp);
			
			printf("%s ���쐬���܂����B\n",fname);

			printf("---------------------------\n");
			return 0;
		}
		case 3:
		{
			/* �t�@�C�����J���� */
			strcpy(fname, "2_MakeBLK.bat");
			fp = fopen(fname, "r");
			if(fp != NULL)
			{
				printf("�t�@�C�������ɑ��݂��܂��B(%s)\n", fname);
				printf("__Clean.bat �����s���Ă�������\n");
				/* �t�@�C������� */
				fclose (fp);
				return -1;
			}
			
			fp = fopen(fname, "w");
			
			/* �t�@�C���ɏ������� */
			for(cnt = start_cnt; cnt <= end_cnt; cnt += inc_val)
			{
				fprintf(fp, "blk LIST%d_Tx.blk\n", cnt);
				fprintf(fp, "blk LIST%d_Tp.blk\n", cnt);
			}
			fprintf(fp, "blk LIST_PCM.blk\n");

			/* �t�@�C������� */
			fclose (fp);

			printf("%s ���쐬���܂����B\n",fname);
			
			printf("---------------------------\n");
			return 0;
		}
		default:
			break;
	}
	
	do
	{
		/* �t�@�C�����J���� */
		switch(mode)
		{
			default:
			case 0:
			{
				sprintf(fname, "LIST%d_Tx.blk", i);
				strcpy(str, "Tx");
				break;
			}
			case 1:
			{
				sprintf(fname, "LIST%d_Tp.blk", i);
				strcpy(str, "Tp");
				break;
			}
		}
		fp = fopen(fname, "r");
		if(fp != NULL)
		{
			printf("�t�@�C�������ɑ��݂��܂��B(%s)\n", fname);
			printf("__Clean.bat �����s���Ă�������\n");
			/* �t�@�C������� */
			fclose (fp);
			return -1;
		}
		
		fp = fopen(fname, "w");
		do
		{
			/* �t�@�C���ɏ������� */
			fprintf(fp, "%s%05d:%s%05d\n", str, cnt, str, cnt);
			
			cnt += inc_val;
			
			j++;
			if(j >= 500)
			{
				j = 0;
				i++;
				break;
			}
			
			if(cnt > end_cnt)
			{
				flag = 1;
				break;
			}
			
		}while(1);
		
		/* �t�@�C������� */
		fclose (fp);
		
		printf("%s ���쐬���܂����B\n",fname);
		
	}while(flag == 0);

	printf("---------------------------\n");
	
	ret = i;
	
	return ret;
}

int16_t MACScnv(int16_t start_cnt, int16_t inc_val, int16_t end_cnt, int16_t mode, int16_t sec)
{
	FILE *fp;
	int16_t ret = 0;
	int16_t cnt, r_count;
	float f_frame, f_fps, f_spf, f_time, f_r_diff, r_time, r_time_old;
	int16_t frame;
	int8_t fname[256];
	float f1, f2;

	f_time = 0;
	r_time = 0;
	r_time_old = 0;
	frame = end_cnt + 1;	/* �f�ނ̑��� */
	
	switch(mode)
	{
		case 0:
		{
			/* �t�@�C�����J���� */
			strcpy(fname, "MACSsrc.s");
			fp = fopen(fname, "r");
			if(fp != NULL)
			{
				printf("�t�@�C�������ɑ��݂��܂��B(%s)\n", fname);
				printf("__Clean.bat �����s���Ă�������\n");
				/* �t�@�C������� */
				fclose (fp);
				return -1;
			}
			
			fp = fopen(fname, "w");
			
			fprintf(fp, ".include MACS_SCH.h\n\n");
			fprintf(fp, "SET_OFFSET\n\n");
			
			f_frame = (float)frame / inc_val;	/* ���Z�Ώۃt���[�������̎Z�o[f] */
			f_fps = f_frame / (float)sec;		/* ��b�����艽�t���[���Ȃ̂��H�Z�o[fps] */
			f_spf = (float)sec / f_frame;		/* ��t���[�������艽�b�Ȃ̂��H�Z�o[spf] */
			
			printf("%f, %f, %f\n", f_fps, f_spf, VSYNC_sec);
			
			for(cnt = start_cnt; cnt <= end_cnt; cnt += inc_val)
			{
				f_time = ((float)cnt) * f_spf;	/* ���t���[���̎���[sec] Alpha */
				
				if(cnt == 0)	/* ���񂪂O�̏ꍇ */
				{
					r_count = 0;
					r_time = 0;
					f_r_diff = 0;
				}
				else
				{
					f1 = f_time - (r_time + f_r_diff + (((float)(inc_val-0)) * VSYNC_sec));
					
					f2 = f_time - (r_time + f_r_diff + (((float)(inc_val-1)) * VSYNC_sec));

					printf("%4d, %f, %f\n", cnt, f1, f2);
					
					if(fabs(f1) < fabs(f2))
					{
						r_count = inc_val;
					}
					else
					{
						r_count = inc_val - 1;
					}
					
					r_time = r_time + (((float)r_count) * VSYNC_sec);	/* 68�ŕ\�������ꍇ���t���[���ڂ̎��� Beta */
					
					f_r_diff = r_time - f_time + f_r_diff;
				}
				
				printf("%4d, %f, %f, %f\n", cnt, f_time, r_time, f_r_diff);
				
				if( (r_count > 0) || (cnt == start_cnt) )
				{
					fprintf(fp, "DRAW Tx%05d\n", cnt);
					if(cnt != start_cnt)
					{
						fprintf(fp, "WAIT %d\n", (int16_t)r_count);
					}
					fprintf(fp, "PALETTE Tp%05d\n", cnt);
					fprintf(fp, "CHANGE_POSITION\n\n");
				}
				
				if(cnt == start_cnt)
				{
					fprintf(fp, "SCREEN_ON\n\n");
					fprintf(fp, "PCM_PLAY pcmdat,pcmend-pcmdat\n\n");
				}
			}
			
			fprintf(fp, "WAIT 120\n");
			fprintf(fp, "PCM_STOP\n");
			fprintf(fp, "EXIT\n");
			
			/* �t�@�C������� */
			fclose (fp);

			printf("%s ���쐬���܂����B\n",fname);
			
			printf("---------------------------\n");
			return 0;
		}
		case 1:
		{
			int8_t sStingMes[3][32] = {	"has -u MACSsrc",
										"hlk -r MACSsrc",
										"MakeMCS MACSsrc"
									};
			
			/* �t�@�C�����J���� */
			strcpy(fname, "3_MakeMACS.bat");
			fp = fopen(fname, "r");
			if(fp != NULL)
			{
				printf("�t�@�C�������ɑ��݂��܂��B(%s)\n", fname);
				printf("__Clean.bat �����s���Ă�������\n");
				/* �t�@�C������� */
				fclose (fp);
				return -1;
			}
			
			fp = fopen(fname, "w");
			
			/* �t�@�C���ɏ������� */
			fprintf(fp, "%s\n\n", sStingMes[0]);
			fprintf(fp, "%s ", sStingMes[1]);
			for(cnt = start_cnt; cnt <= end_cnt; cnt += inc_val)
			{
				fprintf(fp, "LIST%d_Tx ", cnt);
				fprintf(fp, "LIST%d_Tp ", cnt);
			}
			fprintf(fp, "LIST_PCM\n\n");
			fprintf(fp, "%s\n", sStingMes[2]);

			/* �t�@�C������� */
			fclose (fp);

			printf("%s ���쐬���܂����B\n",fname);
		}
		default:
			break;
	}
	printf("---------------------------\n");

	return ret;
}

int16_t main(int16_t argc, int8_t** argv)
{
	int16_t ret = 0;
	int16_t cnt[4];
	
	if (argc == 5)
	{
		cnt[0] = atoi(argv[1]);
		cnt[1] = atoi(argv[2]);
		cnt[2] = atoi(argv[3]);
		cnt[3] = atoi(argv[4]);
		
		if((cnt[0] == cnt[1]) && (cnt[0] == cnt[2]))
		{
			return -1;
		}
		else
		{
			if(cnt[1] <= 0)
			{
				return -1;
			}
			else
			{
				printf("CG%05d �` CG%05d �܂� %d ����������o�b�`�t�@�C�����쐬���܂�(���v�F%d��)\n", cnt[0], cnt[2], cnt[1],((cnt[2]+1)-(cnt[0]+1))/cnt[1]);
				printf("����̒����� %d��%d�b �ł�\n", cnt[3] / 60, cnt[3] % 60);
				printf("---------------------------\n");
				/* �摜�ϊ� */
				CGcnv(cnt[0], cnt[1], cnt[2]);
				/* �f�[�^Obj���� */
				ret = BLTcnv(cnt[0], cnt[1], cnt[2], 0);
				BLTcnv(cnt[0], cnt[1], cnt[2], 1);
				BLTcnv(0, 0, 0, 2);
				BLTcnv(0, 1, ret, 3);
				/* �f�[�^�ϊ� */
				MACScnv(cnt[0], cnt[1], cnt[2], 0, cnt[3]);
				MACScnv(0, 1, ret, 1, 0);
			}
		}
	}
	else
	{
		printf("MACScnv.x <�J�n�ԍ�> <�����l> <�I���ԍ�> <����Đ�����[s] or PCM�̎���[s]>\n");
		printf("ex. >MACScnv.x 0 4 1620 81\n");
		printf("------------------------------\n");
		printf("�ϊ��ɕK�v�ȃc�[�����t�@�C���F\n");
		printf("=< 0_MakeTxTp.bat >=============\n");
		printf("    �摜�\���@�@�@�@�FAPICG.r\n");
		printf("    ��ʃ��[�h�ύX�@�FYGMC.x\n");
		printf("    �e�L�X�g��ʕۑ��Ftxs.x\n");
		printf("=< 1_MakePCM.bat >==============\n");
		printf("    Wav to ADPCM�@�@�Fpcm3pcm.x\n");
		printf("=< 2_MakeBLK.bat >==============\n");
		printf("    �o�C�i�������@�@�Fblk.x\n");
		printf("=< 3_MakeMACS.bat >=============\n");
		printf("    MACS�w�b�_�@�@�@�FMACS_SCH.h\n");
		printf("    �A�Z���u���@�@�@�Fhas.x\n");
		printf("    �����J�@�@�@�@�@�Fhlk.x\n");
		printf("    MACS�f�[�^�ϊ��@�FMakeMCS.x\n");
		printf("------------------------------\n");
		printf("ver1.0.1\n");
	}
	
#if 0
	if (strcmp(argv[1], "help") == 0)
	{
	}
	else if (strcmp(argv[1], "add") == 0 && argc == 4)
	{
		printf("add:%d\n", atoi(argv[2]) + atoi(argv[3]));
	}
	else if (strcmp(argv[1], "sub") == 0 && argc == 4)
	{
		printf("sub:%d\n", atoi(argv[2]) - atoi(argv[3]));
	}
	else
	{
		printf("�T�|�[�g����Ă��Ȃ��R�}���h���C�������ł�\n");
	}
#endif
	return 0;
}

