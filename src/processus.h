#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__

#include <inttypes.h>
#include "timer.h"

#define NBPROC 10
#define NOM_PROC_MAX 20
#define STACK_SIZE 4096
#define NB_REGISTRES 17


typedef enum {
    ELU,
    ACTIVABLE,
    ENDORMI,
    MORT
} etat_t;

// Structure de processus
typedef struct processus {
    int32_t pid;
    char nom[NOM_PROC_MAX];
    etat_t etat;
    uint64_t registres[NB_REGISTRES];
    uint64_t pile[STACK_SIZE];
    uint64_t reveil;
} processus_t;


void init_proc(void);
int32_t cree_processus(void (*)(void), char *);
void ordonnance(void);
int32_t mon_pid(void);
char *mon_nom(void);
void dors(uint32_t);
void fin_processus(void);

extern void ctx_sw(uint64_t *ancien_contexte, uint64_t *nouveau_contexte);

#endif 
