.global ctx_sw
.text
/* void ctx_sw(uint64_t *ctxt_de_sauvegarde, uint64_t *ctxt_a_restaurer)
   ctxt_de_sauvegarde : registre a0
   ctxt_a_restaurer   : registre a1
*/
ctx_sw:
    /* Sauvegarde des registres importants dans le tableau d'adresse a2 */
    mv a2, a0

    sd ra,     0*8(a2)
    sd sp,     1*8(a2)
    sd gp,     2*8(a2)
    sd tp,     3*8(a2)
    sd s0,     4*8(a2)
    sd s1,     5*8(a2)
    sd s2,     6*8(a2)
    sd s3,     7*8(a2)
    sd s4,     8*8(a2)
    sd s5,     9*8(a2)
    sd s6,    10*8(a2)
    sd s7,    11*8(a2)
    sd s8,    12*8(a2)
    sd s9,    13*8(a2)
    sd s10,   14*8(a2)
    sd s11,   15*8(a2)

    csrrs s1,mepc,zero
    sd s1,  16*8(a2) 

    
    /* Restauration de l'adresse de sauvegarde pour les étapes suivantes */
    mv a0, a2

    ld a0,   17*8(a1)
 
    ld s1,   16*8(a1)
    csrrw zero,mepc,s1
    
    ld ra,     0*8(a1)
    ld sp,     1*8(a1)
    ld gp,     2*8(a1)
    ld tp,     3*8(a1)
    ld s0,     4*8(a1)
    ld s1,     5*8(a1)
    ld s2,     6*8(a1)
    ld s3,     7*8(a1)
    ld s4,     8*8(a1)
    ld s5,     9*8(a1)
    ld s6,    10*8(a1)
    ld s7,    11*8(a1)
    ld s8,    12*8(a1)
    ld s9,    13*8(a1)
    ld s10,   14*8(a1)
    ld s11,   15*8(a1)
    
    ret
