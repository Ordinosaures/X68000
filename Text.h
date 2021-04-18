#ifndef	TEXT_H
#define	TEXT_H

#include "inc/usr_define.h"

extern void T_INIT(void);
extern void T_EXIT(void);
extern void T_Clear(void);
extern void T_PALET(void);
extern void T_SetBG_to_Text(void);
extern void T_TopScore(void);
extern void T_Time(void);
extern void T_Score(void);
extern void T_Speed(void);
extern void T_Gear(void);
extern void T_Main(uint8_t);
extern int16_t T_Scroll(uint32_t, uint32_t);
extern int32_t T_Box(int16_t, int16_t, int16_t, int16_t, uint16_t, uint8_t);
extern int32_t T_Fill(int16_t, int16_t, int16_t, int16_t, uint16_t, uint8_t);
extern int32_t T_xLine(int16_t, int16_t, int16_t w, uint16_t, uint8_t);
extern int32_t T_yLine(int16_t, int16_t, int16_t h, uint16_t, uint8_t);

#endif	/* TEXT_H */
