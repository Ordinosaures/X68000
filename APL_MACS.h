#ifndef	APL_MACS_H
#define	APL_MACS_H

#include "inc/usr_define.h"
#include "Moon.h"

#ifdef 	MACS_MOON
/* define��` */
#define	MACS_MAX	(16)

/* extern�錾 */
extern int8_t	g_mov_list[MACS_MAX][256];
extern uint32_t	g_mov_list_max;
#endif	/* MACS_MOON */

/* �\���̒�` */

/* extern�錾 */
extern void	MOV_INIT(void);
extern int32_t	MOV_Play(uint8_t);

#endif	/* APL_MACS_H */
