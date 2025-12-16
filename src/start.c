#include <inttypes.h>
#include <stdio.h>
#include <cpu.h>
#include "processus.h"
#include "timer.h"


void kernel_start(void) {

    //enable_it();
    init_traitant_timer(mon_traitant);
    enable_timer();
    //disable_it();

    efface_ecran();

    // Initialize process
    init_proc();
    int32_t pid = mon_pid();
    char *nom = mon_nom();
    printf("Processus actif PID: %d\n", pid);
    printf("Process actif nom: %s\n\n", nom);

    printf("creation de proc1\n");
    pid = cree_processus(proc1, "proc1");
    ordonnance();
    printf("Processus actif PID: %d\n\n", pid);
    
    printf("creation de proc2\n");
    pid = cree_processus(proc2, "proc2");
    printf("Processus actif PID: %d\n\n", pid);

    printf("creation de proc3\n");
    pid = cree_processus(proc3, "proc3");
    printf("Processus actif PID: %d\n\n", pid);

    idle();

    printf("Tests Done\n");

}
