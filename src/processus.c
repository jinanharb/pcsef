#include "processus.h"
#include <string.h>
#include <stdio.h>

static processus_t table_proc[NBPROC];
static processus_t *actif = NULL;
static int32_t prochain_pid = 0;


static void proc_launcher(void (*proc)(void)) 
{
    proc();
    fin_processus();
}

void init_proc(void) 
{
    for (int i = 0; i < NBPROC; i++) 
    {
        table_proc[i].pid = -1;
        table_proc[i].etat = MORT;
    }
    
    table_proc[0].pid = 0;
    strncpy(table_proc[0].nom, "idle", NOM_PROC_MAX);
    table_proc[0].etat = ELU;
    table_proc[0].reveil = 0;
    
    actif = &table_proc[0];
    prochain_pid = 1;
}

int32_t cree_processus(void (*code)(void), char *nom) 
{
    int i;
    for (i = 0; i < NBPROC; i++) 
    {
        if (table_proc[i].etat == MORT) 
        {
            break;
        }
    }
    
    /*if (i == NBPROC) 
    {
        return -1; // Plus de place
    }*/
    
    table_proc[i].pid = prochain_pid++;
    strncpy(table_proc[i].nom, nom, NOM_PROC_MAX - 1);
    table_proc[i].nom[NOM_PROC_MAX - 1] = '\0';
    table_proc[i].etat = ACTIVABLE;
    table_proc[i].reveil = 0;
    
    table_proc[i].registres[1] = (uint64_t)&table_proc[i].pile[STACK_SIZE];

    table_proc[i].registres[0] = (uint64_t)proc_launcher;

    table_proc[i].registres[10] = (uint64_t)code;
    
    return table_proc[i].pid;
}

void ordonnance(void) 
{
    processus_t *ancien = actif;
    
    uint32_t temps = nbr_secondes();
    // the problem here is that NBPROC is fixe but it should be = to how much processes have been created
    for (int i = 0; i < NBPROC; i++) 
    {
        if (table_proc[i].etat == ENDORMI && table_proc[i].reveil <= temps) 
        {
            table_proc[i].etat = ACTIVABLE;
        }
    }
    
    int idx_actif = actif - table_proc;
    int i = (idx_actif + 1) % NBPROC;
    
    while (i != idx_actif) {
        if (table_proc[i].etat == ACTIVABLE || table_proc[i].etat == ELU) {
            if (table_proc[i].pid != -1 && table_proc[i].etat != MORT) {
                break;
            }
        }
        i = (i + 1) % NBPROC;
    }
    
    if (i == idx_actif) 
    {
        if (actif->etat == MORT) 
        {
            for (i = 0; i < NBPROC; i++) {
                if (table_proc[i].etat != MORT && table_proc[i].pid != -1) {
                    break;
                }
            }
            if (i == NBPROC) {
                i = 0; 
            }
        } else 
        {
            return;
        }
    }
    
    if (ancien->etat == ELU) 
    {
        ancien->etat = ACTIVABLE;
    }
    
    actif = &table_proc[i];
    if (actif->etat == ACTIVABLE) {
        actif->etat = ELU;
    }
    
    if (ancien != actif) 
    {
        ctx_sw(ancien->registres, actif->registres);
    }
}

int32_t mon_pid(void) 
{
    return actif->pid;
}

char *mon_nom(void) 
{
    return actif->nom;
}

void dors(uint32_t nbr_secs) 
{
    actif->etat = ENDORMI;
    actif->reveil = nbr_secondes() + nbr_secs;
    ordonnance();
}

void fin_processus(void) 
{
    actif->etat = MORT;
    actif->pid = -1;
    ordonnance();
}
