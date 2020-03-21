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

#define SetRGB(R,G,B)	(( G << 11) + (R << 6) + (B << 1))

#define SetBGcode(V,H,PAL,PCG)	((V << 15) + (H << 14) + (PAL << 8) + PCG)

#endif	/* USR_MACRO_H */
