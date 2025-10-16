#ifndef __CPU_H__
#define __CPU_H__
#include <platform.h>
/* Met le processeur en pause en attente d'une interruption */
__inline__ static void hlt(void)
{
   __asm__ __volatile__("wfi":::"memory");
}

/* Autorise les interruptions au niveau du processeur */
__inline__ static void enable_it(void)
{
   __asm__("csrw mstatus, %0"::"i"(MSTATUS_MIE));
}

/* Interdit les interruptions au niveau du processeur */
__inline__ static void disable_it(void)
{
   __asm__("csrw mstatus, %0"::"r"(0));
}

#endif
