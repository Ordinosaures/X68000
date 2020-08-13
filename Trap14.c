#ifndef	TRAP14_C
#define	TRAP14_C

/* system include */
#include <stdio.h>
#include <stdlib.h>
#include <interrupt.h>

#include "inc/usr_macro.h"
#include "Trap14.h"

#ifdef DEV_GCC
	/* Develop �� GCC �g��*/
	DOSCALL INTVCG (short);
	DOSCALL INTVCS (short, void *);
	DOSCALL PRINT (char *);
#else
	#include <doslib.h>
#endif

/*
stdin,stdout XC Ver �ˑ��̂���
sprintf �Ń}�N���ɂ��Ă���
*/
#define Fprint(buf,fmt,mes) \
	do { \
		sprintf ((buf), (fmt), (mes)); \
		PRINT ((buf)); \
	} while (0)

static struct {
	int flag;				/* �G���[��ʃt���O*/
	unsigned short sr_reg;	/* �G���[���� SR */
	int pc_reg;				/* �G���[���� PC */
	char *mes;				/* ���b�Z�[�W */
} trapbuf;

static void (*trap_14)();	/* ���� trap 14 �x�N�^��ۑ�*/

extern int _main;			/* �X�^�[�g�A�b�v�̃_�~�[ */

static void interrupt trap14(void)
{
	PRAMREG (code, d7);	/* �p�����[�^���W�X�^ d7 */
	PRAMREG (add, a6);	/* �p�����[�^���W�X�^ a6 */
	SET_FRAME (a5);		/* �t���[���|�C���^�� a5 �Ɏw�� */
	char *p =(char *)add;

	/* code ����G���[�𕪐� */
	switch (code & 0xFFFF)
	{
		case 2:
			/* �o�X�G���[ */
			trapbuf.flag = code;
			trapbuf.mes = "�o�X�G���[���������܂���";
			break;
		case 3:
			/* �A�h���X�G���[ */
			trapbuf.flag = code;
			trapbuf.mes = "�A�h���X�G���[���������܂���";
			break;
		case 4:
			/* �������Ȗ��� */
			trapbuf.flag = code;
			trapbuf.mes = "�������Ȗ��߂����s���܂���";
		break;
	}
	
	if ((code & 0xFF00))
	{
		if ((code & 0xFF) == 2)
		{
			trapbuf.flag = -1;
			trapbuf.mes = "�h���C�u�̏������ł��Ă��܂���";
		}
	}
	
	if (trapbuf.flag || ((code & 0xFFFF) == 0x1F) || ((code & 0xFFFF) == 0x301F))
	{
		trapbuf.sr_reg = *(short *)p;
		p+=2;
		trapbuf.pc_reg = *(int *)p;
		asm ("\tmove.w #$ff,d0\n\ttrap #15\n");
		IJUMP_RTE();
	}
	else
		IJUMP(trap_14);
}

static void my_abort (void)
{
	char buf[64];
//	void (*usr_abort)(void);	/* ���[�U�̃A�{�[�g�����֐� */
	
	INTVCS(0x2E,trap_14);
	
	if (usr_abort)
		usr_abort();
	
	if (trapbuf.flag > 0)
	{
		extern int _SSTA;
		extern int _PSTA;
		trapbuf.flag = 0;
		Fprint (buf, "%s\r\n", trapbuf.mes);
		Fprint (buf, " pc=%X" , trapbuf.pc_reg);
		Fprint (buf, " offset=%X\r\n", trapbuf.pc_reg - (int) &_main);
		exit(512);
	}
	else
	{
		Fprint (buf, "%s\r\n", trapbuf.mes);
		exit(256);
	}
}

void init_trap14 (void)
{
	trap_14 = (void *)INTVCG (0x2E);
	INTVCS (0x2E, trap14);
	INTVCS (0xFFF2, my_abort);
	INTVCS (0xFFF1, my_abort);
}

#endif	/* TRAP14_C */

