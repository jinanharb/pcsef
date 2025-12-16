#include "timer.h"
#include "processus.h"
#include "console.h"
#include "font.h"
#include <string.h>

//Initializing the addresses, frequencies and variables
#define CLINT_TIMER ((uint64_t *)0x0200bff8)
#define CLINT_TIMER_CMP ((uint64_t *)0x02004000)
#define TIMER_FREQ 10000000
#define IT_FREQ 20

uint32_t seconds = 0;
uint64_t nb_ticks = 0;
uint32_t col_timer;

//prints the format of the time in the top right corner
void affiche_time(char *s)
{
    col_timer = 160 - strlen(s);
    for (size_t i = 0; i < strlen(s); i++, col_timer++) 
    {
        char c = s[i];
        int8_t* carac = (int8_t*) font8x8_basic[(int)c];
        char allume;
        for (int i = 0; i < 8; i++) {
            int8_t line = carac[i];
            for (int j = 0; j < 8; j++)
            {
                allume = (line >> j) & 1;
                if (allume == 1) 
                {
                    pixel(i, (col_timer * 8) + j, 0xffffff);
                }
                else 
                {
                    pixel(i, (col_timer * 8) + j, 0x000000);
                }
            }
        }
    }
}

//the function called everytime we interrupt
void trap_handler(uint64_t mcause, uint64_t mie, uint64_t mip)
{
    uint64_t val;
    char s[9];

    (void)mie;
    (void)mip;
    
    if ((mcause & 0x7FFFFFFFFFFFFFFF) == 7) 
    {
        nb_ticks++;
        val = *CLINT_TIMER;
        val = val + (TIMER_FREQ / IT_FREQ);
        *CLINT_TIMER_CMP = val;
        
        seconds = nbr_secondes();
        sprintf(s, "%02u:%02u:%02u", (seconds / 3600), ((seconds % 3600) / 60), (seconds % 60));
        affiche_time(s);
        
        // Call the scheduler
        //ordonnance();
    }
}

//writing the address of the interrupt handler (traitant.S) into the mtvec register 
void init_traitant_timer(void (*traitant)(void))
{
    __asm__("csrw mtvec, %0" :: "r"((uint64_t) traitant));
}

//enabling the timer by setting to 1 the 7th bit of mie
void enable_timer()
{
    uint64_t val;
    __asm__("csrr %0, mie": "=r"(val));
    val |= 1 << 7;
    __asm__("csrw mie, %0" :: "r"(val));
    val = *CLINT_TIMER;
    val = val + (TIMER_FREQ / IT_FREQ);
    *CLINT_TIMER_CMP = val;
}

//helper function used to calculate the nb of seconds based on the ticks 
uint32_t nbr_secondes(void) {
    return (uint32_t)(nb_ticks / 20);
}
