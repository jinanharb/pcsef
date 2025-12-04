#ifndef __TIMER_H__
#define __TIMER_H__
#include <inttypes.h>

extern void mon_traitant(void);

void affiche_time(char*);
void trap_handler(uint64_t mcause, uint64_t mie, uint64_t mip);
void init_traitant_timer(void (*traitant)(void));
void enable_timer(void);
uint32_t nbr_secondes(void);

#endif
