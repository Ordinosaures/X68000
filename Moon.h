#ifndef	MOON_H
#define	MOON_H

#include "inc/usr_define.h"

#define	MACS_MOON	/* MACSDRV��MOON���g���Ƃ��͒�`���� */

/* �\���̒�` */
/* extern�錾 */
SI MoonPlay(void *);
SI MoonRegst(void *);
SI MACS_Play(void *);
SI MACS_Vsync(void *);
SI MACS_Vsync_R(void *);
SI MACS_Sleep(void);

#endif	/* MOON_H */
