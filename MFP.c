#ifndef	MFP_C
#define	MFP_C

#include <iocslib.h>
#include <interrupt.h>

#include "inc/usr_macro.h"

#include "OverKata.h"
#include "MFP.h"
#include "Moon.h"
#include "OutPut_Text.h"
#include "Raster.h"

/* �ϐ� */
static uint8_t g_bTimer_D;
static uint8_t g_bRasterSET;
static volatile uint32_t NowTime;
static volatile uint32_t StartTime;
static volatile uint16_t g_uRas_Count;
static volatile uint16_t Hsync_count;
static volatile uint16_t Vsync_count;

static ST_RASTER_INT g_stRasterInt[RASTER_MAX] = {0};

/* �֐��̃v���g�^�C�v�錾 */
int16_t MFP_INIT(void);
int16_t MFP_EXIT(void);
int16_t MFP_RESET(void);
int16_t TimerD_INIT(void);
int16_t TimerD_EXIT(void);
uint8_t GetNowTime(uint32_t *);	/* ���݂̎��Ԃ��擾���� */
uint8_t SetNowTime(uint32_t);		/* ���݂̎��Ԃ�ݒ肷�� */
uint8_t GetStartTime(uint32_t *);	/* �J�n�̎��Ԃ��擾���� */
uint8_t SetStartTime(uint32_t);	/* �J�n�̎��Ԃ�ݒ肷�� */
int16_t SetRasterIntData(void *, size_t);
int16_t GetRasterIntPos(uint16_t *, uint16_t *, uint16_t *, uint16_t);
static void interrupt Timer_D_Func(void);
static void interrupt Hsync_Func(void);
static void interrupt Raster_Func(void);
static void interrupt Vsync_Func(void);
int16_t vwait(int16_t);

/* �֐� */
int16_t MFP_INIT(void)
{
	int32_t	vdispst = -1;
	volatile uint8_t *MFP_TACR = (uint8_t *)0xe88019;
	volatile uint8_t *MFP_TADR = (uint8_t *)0xe8801f;

	/* ���X�^���荞�� */
	g_bRasterSET = FALSE;
	g_uRas_Count = 0;
	memset(g_stRasterInt, 0, sizeof(ST_RASTER_INT) * RASTER_MAX);
	/* H-Sync���荞�� */
	Hsync_count = 0;
	/* V-Sync���荞�� */
	Vsync_count = 0;

	//VDISPST ���񂾂��o�O����iEXCEED.����j
	*MFP_TACR = 0x00;	/* �^�C�}�[�`���~�߂� */
	*MFP_TADR = 0x01;	/* �J�E���^��ݒ�(0=256��) */
	*MFP_TACR = 0x08;	/* �C�x���g�J�E���g���[�h�ɐݒ肷�� */
#ifdef 	MACS_MOON
	vdispst = MACS_Vsync(Vsync_Func);		/* V-Sync���荞�� */
#else	/* MACS_MOON */
	vdispst = VDISPST(Vsync_Func, 0, 1);	/* V-Sync���荞�� �A�� */
#endif	/* MACS_MOON */
//	Message_Num(&vdispst,  0, 10, 2, MONI_Type_SI, "%4d");
	
	return vdispst;
}

int16_t MFP_EXIT(void)
{
	int32_t	vdispst = -1;

	g_bRasterSET = FALSE;

	CRTCRAS((void *)0, 0);		/* stop */
	HSYNCST((void *)0);			/* stop */
#ifdef 	MACS_MOON
	vdispst = MACS_Vsync_R(Vsync_Func);	/* stop */
#else	/* MACS_MOON */
	vdispst = VDISPST((void *)0, 0, 0);	/* stop */
#endif	/* MACS_MOON */
//	Message_Num(&vdispst,  6, 10, 2, MONI_Type_SI, "%4d");

	return vdispst;
}

int16_t MFP_RESET(void)
{
	int16_t	ret = 0;

	volatile uint8_t *MFP_INTCTRL_A  = (uint8_t *)0xE88007;	/* ���荞�݃C�l�[�u�����W�X�^A */
	volatile uint8_t *MFP_TADR  		= (uint8_t *)0xE8801F;	/* �^�C�}�[A�f�[�^���W�X�^ */
	
	*MFP_TADR = 1;					/* �J�E���^�ݒ� */
	*MFP_INTCTRL_A |= 0b01000000;	/* CRTC Raster���荞�݋��� */
//	*MFP_INTCTRL_A |= 0b00100000;	/* Timer-A���荞�݋��� */
	
	return ret;
}

int16_t	TimerD_INIT()
{
	int16_t CpuCount=0;
	/* Timer-D���荞�� */
	NowTime = 0;
	StartTime = 0;
	TIMERDST(Timer_D_Func, 7, 20);	/* Timer-D�Z�b�g */	/* 50us(7) x 20cnt = 1ms */
	g_bTimer_D =TRUE;
	SetNowTime(NowTime);	/* ���Ԃ̏����� */
	/* �}�C�R���N���b�N���v�� */
	do
	{
		CpuCount++;
	}
	while(NowTime==0);

	return CpuCount;
}

int16_t	TimerD_EXIT()
{
	TIMERDST((void *)0, 0, 1);	/* stop */
	g_bTimer_D = FALSE;

	return 0;
}

uint8_t GetNowTime(uint32_t *time)	/* ���݂̎��Ԃ��擾���� */
{
	*time = NowTime;
	return g_bTimer_D;
}

uint8_t SetNowTime(uint32_t time)	/* ���݂̎��Ԃ�ݒ肷�� */
{
	NowTime = time;
	return g_bTimer_D;
}

uint8_t GetStartTime(uint32_t *time)	/* �J�n�̎��Ԃ��擾���� */
{
	*time = StartTime;
	return g_bTimer_D;
}

uint8_t SetStartTime(uint32_t time)	/* �J�n�̎��Ԃ�ݒ肷�� */
{
	StartTime = time;
	return g_bTimer_D;
}

int16_t SetRasterIntData(void *pSrc, size_t n)
{
	int16_t ret = 0;
	if(memcpy(g_stRasterInt, pSrc, n) == NULL)
	{
		ret = -1;
	}
	else
	{
		g_bRasterSET = TRUE;
	}
	return ret;
}

int16_t GetRasterIntPos(uint16_t *x, uint16_t *y, uint16_t *pat, uint16_t uNum)
{
	int16_t	ret = 0;

	if(uNum >= RASTER_MAX)
	{
		uNum = RASTER_MAX - 1;
		ret = -1;
	}
	
	if(x !=  NULL)
	{
		*x = g_stRasterInt[uNum].x;
	}
	else
	{
		/* nop */
	}
	
	if(y !=  NULL)
	{
		*y = g_stRasterInt[uNum].y;
	}
	else
	{
		/* nop */
	}

	if(pat !=  NULL)
	{
		*pat = g_stRasterInt[uNum].pat;
	}
	else
	{
		/* nop */
	}
	
	return ret;
}

/* ���荞�݊֐��͏C���q��interrupt�����邱�� */	/* ���ɂ����iKunihiko Ohnaka�j����̃A�h�o�C�X */

static void interrupt Timer_D_Func(void)
{
	NowTime++;

	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

static void interrupt Hsync_Func(void)
{
	HSYNCST((void *)0);			/* stop */
	Hsync_count++;
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

/* ���X�^�[���荞�݂̏������͕K�v�Œ������ */	/* EXCEED.����̃A�h�o�C�X */
/* �Q���C����΂����炢�����x�̌��E */	/* EXCEED.����̃A�h�o�C�X */
static void interrupt Raster_Func(void)
{
	volatile uint16_t *BG0scroll_x  = (uint16_t *)0xEB0800;
	volatile uint16_t *BG0scroll_y  = (uint16_t *)0xEB0802;
	volatile uint16_t *BG1scroll_x  = (uint16_t *)0xEB0804;
	volatile uint16_t *BG1scroll_y  = (uint16_t *)0xEB0806;
	volatile uint16_t *CRTC_R09 = (uint16_t *)0xE80012u;	/* ���X�^�[���荞�݈ʒu */
//	volatile uint16_t *CRTC_R12 = (uint16_t *)0xE80018u;	/* �X�N���[��0 X */
//	volatile uint16_t *CRTC_R14 = (uint16_t *)0xE8001Cu;	/* �X�N���[��1 X */
	volatile uint16_t *VIDEO_REG3 = (uint16_t *)0xE82600;

	uint16_t nNum = g_uRas_Count - SP_Y_OFFSET;	/* �����炭������SP_Y_OFFSET�ł͂Ȃ��ACRTC_R06���������̂����� */

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	uint8_t	bDebugMode;
	uint16_t	uDebugNum;
	GetDebugMode(&bDebugMode);
	GetDebugNum(&uDebugNum);
#endif
	
	g_uRas_Count += RASTER_NEXT;		/* ���̃��X�^���荞�݈ʒu�̌v�Z */
	*CRTC_R09 = g_uRas_Count;			/* ���̃��X�^���荞�݈ʒu�̐ݒ� */

//	*CRTC_R12		= g_stRasterInt[nNum].x + X_OFFSET;			/* GR��X���W�̐ݒ� */
	if( g_stRasterInt[0].y < nNum )
	{
		/* �w�i���� */
		*VIDEO_REG3		= Mbset(*VIDEO_REG3,   0x0C, 0b0000000000000000);	/* GRSC1(GR3,GR4)=OFF */
		/* BG0 */
		*BG0scroll_x	= g_stRasterInt[nNum].x;							/* BG0��X���W�̐ݒ� */
		*BG0scroll_y	= g_stRasterInt[nNum].y + g_stRasterInt[nNum].pat;	/* BG0��Y���W�̐ݒ� */
		/* BG1 */
		*BG1scroll_x	= g_stRasterInt[nNum].x;							/* BG1��X���W�̐ݒ� */
		*BG1scroll_y	= g_stRasterInt[nNum].y + g_stRasterInt[nNum].pat;	/* BG1��Y���W�̐ݒ� */
	}
	else
	{
		/* BG0 */
		*BG0scroll_x	= 256;					/* BG0��X���W�̐ݒ� */
		*BG0scroll_y	= g_stRasterInt[1].y;	/* BG0��Y���W�̐ݒ� */
		/* BG1 */
		*BG1scroll_x	= 256;					/* BG1��X���W�̐ݒ� */
		*BG1scroll_y	= g_stRasterInt[1].y;	/* BG1��Y���W�̐ݒ� */
	}
	
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
}

static void interrupt Vsync_Func(void)
{
	volatile uint16_t *BG0scroll_x  = (uint16_t *)0xEB0800;
	volatile uint16_t *BG0scroll_y  = (uint16_t *)0xEB0802;
	volatile uint16_t *BG1scroll_x  = (uint16_t *)0xEB0804;
	volatile uint16_t *BG1scroll_y  = (uint16_t *)0xEB0806;
//	volatile uint16_t *BGCTRL		  = (uint16_t *)0xEB0808;
	volatile uint16_t *VIDEO_REG3 = (uint16_t *)0xE82600;
	volatile uint16_t *CRTC_R06 = (uint16_t *)0xE8000Cu;	/* �����\���J�n�ʒu-1 */
	
	/* V-Sync���荞�ݏ��� */
#ifdef 	MACS_MOON
	/* �������Ȃ� */
#else	/* MACS_MOON */
//	int32_t	vdispst = -1;
	vdispst = VDISPST((void *)0, 0, 0);	/* stop */
#endif	/* MACS_MOON */

//	*BGCTRL = Mbclr(*BGCTRL, Bit_9);
	/* �����̓��X�^���荞�݊J�n�ʒu���̃Y���͍l�����Ȃ��Ă悢 */
	*BG0scroll_x	= 256;					/* BG0��X���W�̐ݒ� *//* ��̃R���g���[�� */
	*BG0scroll_y	= g_stRasterInt[1].y;	/* BG0��Y���W�̐ݒ� *//* ��̃R���g���[�� */
	*BG1scroll_x	= 256;					/* BG1��X���W�̐ݒ� *//* ��̃R���g���[�� */
	*BG1scroll_y	= g_stRasterInt[1].y;	/* BG1��Y���W�̐ݒ� *//* ��̃R���g���[�� */
	
	/* �w�i�\�� */
	*VIDEO_REG3 = Mbset(*VIDEO_REG3,   0x0C, 0b0000000000001100);	/* GR2(GR3,GR4)=ON */
	
	/* H-Sync���荞�ݏ��� */
	Hsync_count = 0;
//	HSYNCST(Hsync_Func);					/* H-Sync���荞�� */

	/* ���X�^���荞�� */
	CRTCRAS((void *)0, 0);	/* stop */
	if(g_bRasterSET == TRUE)
	{
		/* ���j�^�̕\���J�n�ʒu��CRTC_R06���ŏ��̈ʒu�ƂȂ� */
		/* ���X�^�z��̗v�f�ԍ�����ʂ�Y���W */
		/* ������SP/BG�́AX+16,Y+16�̃I�t�Z�b�g������̂� */
		/* ���X�^�z�񂩂�I�t�Z�b�g���𑫂��K�v������ */
		g_uRas_Count = *CRTC_R06 + g_stRasterInt[0].y + SP_Y_OFFSET;	/* ���荞�݊J�n�ʒu */	/* �����0�Ŏn�܂邱�� */
		CRTCRAS( Raster_Func, g_uRas_Count );	/* ���X�^�[���荞�� */
	}
#if 0
	if((Vsync_count % 10) < 5)
	{
		GPALET( 1, SetRGB(16, 16, 16));	/* Glay */
		GPALET( 2, SetRGB(15, 15, 15));	/* D-Glay */
		
		GPALET( 5, SetRGB(31,  0,  0));	/* Red */
		GPALET(15, SetRGB(31, 31, 31));	/* White */
		
		GPALET( 8, SetRGB( 0, 31,  0));	/* Green */
		GPALET(11, SetRGB( 0, 28,  0));	/* Green */
	}
	else
	{
		GPALET( 2, SetRGB(16, 16, 16));	/* Glay */
		GPALET( 1, SetRGB(15, 15, 15));	/* D-Glay */
		
		GPALET(15, SetRGB(31,  0,  0));	/* Red */
		GPALET( 5, SetRGB(31, 31, 31));	/* White */
		
		GPALET(11, SetRGB( 0, 31,  0));	/* Green */
		GPALET( 8, SetRGB( 0, 28,  0));	/* Green */
	}
#endif

	/* V-Sync���荞�ݏ��� */
	Vsync_count++;
#ifdef 	MACS_MOON
	/* �������Ȃ� */
#else	/* MACS_MOON */
	vdispst = VDISPST(Vsync_Func, 0, 1);	/* V-Sync���荞�� �A�� */
#endif	/* MACS_MOON */

#if 0
	{
		volatile uint16_t *CRTC_R21 = (uint16_t *)0xE8002Au;	/* �e�L�X�g�E�A�N�Z�X�E�Z�b�g�A�N���A�[P.S */
		volatile uint16_t *CRTC_480 = (uint16_t *)0xE80480u;	/* CRTC����|�[�g */

		if((*CRTC_480 & 0x02u) == 0u)		/* �N���A���s�łȂ� */
		{
			*CRTC_R21 = Mbset(*CRTC_R21, 0x0Fu, 0x0Cu);	/* SCREEN1 �����N���AON / SCREEN0 �����N���AOFF */
			*CRTC_480 = Mbset(*CRTC_480, 0x02u, 0x02u);	/* �N���A���s */
		}
	}
#endif

//	Message_Num(&g_bRasterSET,  0, 10, 2, MONI_Type_UC, "%4d");
//	Message_Num(&Vsync_count, 	6, 10, 2, MONI_Type_US, "%4d");

#ifdef	MACS_MOON
	IRTS();	/* MACSDRV�ł�rts�ň����K�v���� */
#else	/* MACS_MOON */
	IRTE();	/* ���荞�݊֐��̍Ō�ŕK�����{ */
#endif	/* MACS_MOON */
}

int16_t vwait(int16_t count)				/* ��count�^60�b�҂�	*/
{
	int16_t ret = 0;
	volatile uint8_t *mfp = (uint8_t *)0xe88001;
//	volatile uint16_t *BGCTRL = (uint16_t *)0xEB0808;
//	volatile uint16_t *VIDEO_REG3 = (uint16_t *)0xE82600;
	
//	*BGCTRL = Mbset(*BGCTRL, Bit_9, Bit_9);
	
	while(count--){
		while(!((*mfp) & 0b00010000));	/* �����\�����ԑ҂�	*/
		while((*mfp) & 0b00010000);	/* �����A�����ԑ҂�	*/
	}
//	*VIDEO_REG3 = Mbset(*VIDEO_REG3,   0x0C, 0b0000000000001100);	/* GR2(GR3,GR4)=ON */

	return ret;
}

#endif	/* MFP_H */
