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
#define Minc(x,y) ((x < (x + y)) ? (x + y) : -1)

#define SetRGB(R,G,B)	(( G << 11) + (R << 6) + (B << 1))
#define GetR(color)	(( color >> 6) & 0x1Fu)
#define GetG(color)	(( color >> 11) & 0x1Fu)
#define GetB(color)	(( color >> 1) & 0x1Fu)

#define SetBGcode(V,H,PAL,PCG)	(0xCFFFU & (((V & 0x01U)<<15U) | ((H & 0x01U)<<14U) | ((PAL & 0xFU)<<8U) | (PCG & 0xFFU)))

#endif	/* USR_MACRO_H */
