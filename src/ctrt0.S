
    .equ STACK_SIZE,   4096

    .section .text.init,"ax",@progbits
    .globl entry,console_putbytes,__console_putbytes__

entry:
    # set up stack pointer at some zeroed address range
    la sp, stacks + STACK_SIZE

    # jump to libfemto_start_main
    j kernel_start

    .weak console_putbytes,__console_putbytes__
__console_putbytes__:
    ret
console_putbytes:
    j __console_putbytes__
    ret

    .bss
    .align 8
    .global stacks
stacks:
    .skip STACK_SIZE
