#include "processus.h"
#include <stdio.h>
#include <cpu.h>
#include "timer.h"

process_t process_table[MAX_PROCESSES];
int32_t pid_actif = -1;
process_t *courant = NULL;
int32_t proc_count = 0;

void proc_launcher(void (*proc)(void))
{
    proc();
    fin_processus();
}

static void setup_initial_context(process_t *p, void (*f)(void))
{
    p->context.regs[0] = (uint64_t)proc_launcher;
    p->context.regs[1] = (uint64_t)(p->stack + PROCESS_STACK_SIZE);
    p->context.regs[17] = (uint64_t)f;
    p->context.regs[16] = (uint64_t)proc_launcher;
}

void init_proc(void)
{
    pid_actif = -1;
    courant = NULL;
    proc_count = 0;

    process_t *idle_p = &process_table[0];
    idle_p->pid = 0;
    strncpy(idle_p->name, "idle", PROCESS_NAME_LEN);
    idle_p->state = ELU;
    idle_p->wakeup_time = 0;

    courant = idle_p;
    pid_actif = 0;
    proc_count = 1;
}

int32_t cree_processus(void (*f)(void), const char *name)
{
    process_t *pnew = NULL;
    int32_t pid_new = -1;

    for (int k = 1; k < MAX_PROCESSES; k++) {
        if (k < proc_count) {
            if (process_table[k].state == MORT) {
                pnew = &process_table[k];
                pid_new = k;
                break;
            }
        } else if (k == proc_count) {
            pnew = &process_table[k];
            pid_new = k;
            break;
        }
    }

    if (!pnew) {
        printf("Table pleine (MAX=%d)\n", MAX_PROCESSES);
        return -1;
    }

    if (pid_new == proc_count)
        proc_count++;

    pnew->pid = pid_new;
    strncpy(pnew->name, name, PROCESS_NAME_LEN - 1);
    pnew->name[PROCESS_NAME_LEN - 1] = '\0';
    pnew->state = ACTIVABLE;
    pnew->wakeup_time = 0;

    setup_initial_context(pnew, f);

    return pnew->pid;
}

void ordonnance(void)
{
    int32_t prev_pid = pid_actif;
    process_t *prev = courant;
    uint32_t now = nbr_secondes();

    for (int k = 0; k < proc_count; k++) {
        if (process_table[k].state == ENDORMI &&
            process_table[k].wakeup_time <= now)
        {
            process_table[k].state = ACTIVABLE;
        }
    }

    int32_t suiv = prev_pid;

    for (;;) {
        suiv = (suiv + 1) % proc_count;
        if (process_table[suiv].state == ACTIVABLE ||
            process_table[suiv].state == ELU)
        {
            break;
        }
    }

    if (prev->state == ELU)
        prev->state = ACTIVABLE;

    process_table[suiv].state = ELU;
    courant = &process_table[suiv];
    pid_actif = suiv;

    ctx_sw(prev->context.regs, courant->context.regs);
}

void fin_processus(void)
{
    if (courant->pid == 0) {
        printf("ERREUR\n");
        return;
    }

    courant->state = MORT;
    ordonnance();
}

void dors(uint64_t sec)
{
    if (courant->pid == 0)
        return;

    courant->state = ENDORMI;
    courant->wakeup_time = nbr_secondes() + sec;

    ordonnance();
}

int64_t mon_pid(void)
{
    return courant ? courant->pid : -1;
}

char *mon_nom(void)
{
    return courant ? courant->name : "N/A";
}

void idle()
{
    for (;;) {
        enable_it();
        hlt();
        disable_it();
    }
}

void proc1(void)
{
    for (int i = 0; i < 4; i++) {
        printf("[t = %u] processus %s pid = %i\n",
               nbr_secondes(), mon_nom(), mon_pid());
        dors(2);
    }
}

void proc2(void)
{
    for (int i = 0; i < 2; i++) {
        printf("[t = %u] processus %s pid = %i\n",
               nbr_secondes(), mon_nom(), mon_pid());
        cree_processus(proc1, "NEW_CHILD");
        dors(3);
    }
}

void proc3(void)
{
    for (int i = 0; i < 3; i++) {
        printf("[t = %u] processus %s pid = %i\n",
               nbr_secondes(), mon_nom(), mon_pid());
        dors(5);
    }
}
