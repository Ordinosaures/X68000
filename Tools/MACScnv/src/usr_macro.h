#ifndef	USR_MACRO_H
#define	USR_MACRO_H

/*
        Mmax - �ő�l�擾 -
*/
/**
 * �w�肳�ꂽ2�̈����̂���,�ő�̂��̂�Ԃ�.
 * @param [in] a        �ő�l��I�ԑΏۂ̕ϐ�1
 * @param [in] b        �ő�l��I�ԑΏۂ̕ϐ�2
 * @retval �ő�l
 * @attention �����͕�����]�������.
 */
#define Mmax(a, b) ((a) > (b) ? (a) : (b))

/*
        Mmin - �ŏ��l�擾 -
*/
/**
 * �w�肳�ꂽ2�̈����̂���,�ŏ��̂��̂�Ԃ�.
 * @param [in] a        �ŏ��l��I�ԑΏۂ̕ϐ�1
 * @param [in] b        �ŏ��l��I�ԑΏۂ̕ϐ�2
 * @retval �ŏ��l
 * @attention �����͕�����]�������.
 */
#define Mmin(a, b) ((a) < (b) ? (a) : (b))

/*
        Mminmax - �ŏ��ő�l�擾 -
*/
/**
 * �w�肳�ꂽ2�̈����̂���,�ŏ��̂��̂�Ԃ�.
 * @param [in] a        �ŏ��l��I�ԑΏۂ̕ϐ�1
 * @param [in] b        �ŏ��l��I�ԑΏۂ̕ϐ�2
 * @param [in] c        �ő�l��I�ԑΏۂ̕ϐ�3
 * @retval �ŏ��ő�l
 * @attention �����͕�����]�������.
 */
#define Mminmax(a, b, c) ((a) > (c) ? (c) : ((a) < (b) ? (a) : (b)))

/*
        Mabs - ��Βl�擾 -
*/
/**
 * �w�肳�ꂽ�����̐�Βl��Ԃ�.
 * @param [in] x        ��Βl�ɂ���Ώۂ̕ϐ�
 * @retval ��Βl
 * @attention �����͕�����]�������.
 */
#define Mabs(x) ((x) < 0 ? -(x) : (x))

/*
        McmpSub - ��r���Č��Z���� -
*/
/**
 * �w�肳�ꂽ�����̐�Βl��Ԃ�.
 * @param [in] x        ��Βl�ɂ���Ώۂ̕ϐ�
 * @retval ��Βl
 * @attention �����͕�����]�������.
 */
#define McmpSub(x,y) ( (x < y) ? (y - x) : (x - y) )

/*
        Mbset - bit�Z�b�g -
*/
/**
 * �w�肳�ꂽ�����̃r�b�g���Z�b�g.
 * @param [in] x        ���̒l
 * @param [in] y        �ύX�������̈�
 * @param [in] z        �ύX�������l
 * @retval �r�b�g�Z�b�g���ꂽ�l
 * @attention �����͕�����]�������.
 */
#define Mbset(x,y,z) (((x) & ~(y)) | (z))

/*
        Mbclr - bit�N���A -
*/
/**
 * �w�肳�ꂽ�����̃r�b�g���N���A.
 * @param [in] x        ���̒l
 * @param [in] y        0�ɂ������̈�
 * @retval �r�b�g�Z�b�g���ꂽ�l
 * @attention �����͕�����]�������.
 */
#define Mbclr(x,y) ((x) & ~(y))

/*
        Mdec - �f�N�������g�i�O�N���b�v�j -
*/
/**
 * �J�E���^�l�����Z�������l�łO�܂Ńf�N�������g
 * @param [in] x        �J�E���^
 * @param [in] y        ���Z�������l
 * @retval ���Z���ꂽ�l
 * @attention �����͕�����]�������.
 */
#define Mdec(x,y) ((x > (x - y)) ? (x - y) : 0)

/*
        Minc - �f�N�������g�i�ő�l�N���b�v�j -
*/
/**
 * �J�E���^�l�����Z�������l�ōő�l�܂ŃC���N�������g
 * @param [in] x        �J�E���^
 * @param [in] y        ���Z�������l
 * @retval ���Z���ꂽ�l
 * @attention �����͕�����]�������.
 */
#define Minc(x,y) ((0 < (x + y)) ? (x + y) : -1)

/* �V�t�g����Z */
#define Mdiv2(x)		(x>>1)
#define Mdiv4(x)		(x>>2)
#define Mdiv8(x)		(x>>3)
#define Mdiv10(x)		((x * 0xCCCDu)>>19)
#define Mdiv16(x)		(x>>4)
#define Mdiv32(x)		(x>>5)
#define Mdiv64(x)		(x>>6)
#define Mdiv128(x)		(x>>7)
#define Mdiv256(x)		(x>>8)
#define Mdiv512(x)		(x>>9)
#define Mdiv1024(x)		(x>>10)
#define Mdiv2048(x)		(x>>11)
#define Mdiv4096(x)		(x>>12)
#define Mdiv8192(x)		(x>>13)
#define Mdiv16384(x)	(x>>14)
#define Mdiv32768(x)	(x>>15)
/* �V�t�g�|���Z */
#define Mmul2(x)		(x<<1)
#define Mmul4(x)		(x<<2)
#define Mmul8(x)		(x<<3)
#define Mmul16(x)		(x<<4)
#define Mmul32(x)		(x<<5)
#define Mmul64(x)		(x<<6)
#define Mmul128(x)		(x<<7)
#define Mmul256(x)		(x<<8)
#define Mmul512(x)		(x<<9)
#define Mmul1024(x)		(x<<10)
#define Mmul2048(x)		(x<<11)
#define Mmul4096(x)		(x<<12)
#define Mmul8192(x)		(x<<13)
#define Mmul16384(x)	(x<<14)
#define Mmul32768(x)	(x<<15)

#define Mmul_p1(x)		((x>>3)-(x>>5)+(x>>6)-(x>>7))	/* x / 10 */
#define Mmul_1p25(x)	(x+(x>>2))	/* x * 1.25 */

#define SetRGB(R,G,B)	(( G << 11) + (R << 6) + (B << 1))
#define GetR(color)	(( color >> 6) & 0x1Fu)
#define GetG(color)	(( color >> 11) & 0x1Fu)
#define GetB(color)	(( color >> 1) & 0x1Fu)

#define SetBGcode(V,H,PAL,PCG)	(0xCFFFU & (((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((PAL & 0xFU)<<8U) | (PCG & 0xFFU)))
#define SetBGcode2(V,H,PAL)		(0xCF00U & (((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((PAL & 0xFU)<<8U) ))

#endif	/* USR_MACRO_H */
