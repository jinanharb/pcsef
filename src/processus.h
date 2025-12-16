#ifndef PROCESSUS_H
#define PROCESSUS_H

#include <inttypes.h>
#include <string.h>

#define MAX_PROCESSES 6
#define PROCESS_STACK_SIZE 4096
#define PROCESS_NAME_LEN 32
#define CONTEXT_SIZE 18

typedef enum {
    ELU,
    ACTIVABLE,
    ENDORMI,
    MORT
} PROCESS_STATE;

typedef struct {
    uint64_t regs[CONTEXT_SIZE];
} context_t;

typedef struct {
    int32_t pid;
    char name[PROCESS_NAME_LEN];
    PROCESS_STATE state;
    context_t context;
    uint64_t stack[PROCESS_STACK_SIZE];
    uint64_t wakeup_time;
} process_t;

extern void init_proc(void);
extern int32_t cree_processus(void (*f)(void), const char *name);
extern void ordonnance(void);
extern int64_t mon_pid(void);
extern char *mon_nom(void);
void idle(void);
void proc1(void);
void proc2(void);
void proc3(void);
void dors(uint64_t nbr_secs);
void proc_launcher(void (*proc)(void));
void fin_processus(void);


extern void ctx_sw(uint64_t *ctxt_de_sauvegarde, uint64_t *ctxt_a_restaurer);

#endif
