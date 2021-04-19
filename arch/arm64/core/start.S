#include <yios/string.h>
#include <yios/mm.h>
#include <asm/linkage.h>

.section  .rodata
;; .align 3
.globl el_string1
el_string1:
        .string "Booting at EL"

.section ".text.boot"

    b _start

ENTRY(_start)
    mrs x0, mpidr_el1
    and x0, x0,#0xFF
    cbz x0, master
    b slave_hang
ENDPROC(_start)

slave_hang:
    b slave_hang

master:
    bl __init_uart
    bl get_CurrentEL

    adr x0,bss_begin
    adr x1,bss_end
    sub x2, x1, x0
    mov x1, xzr
    bl memset

    mov     sp, #LOW_MEMORY
    bl      kernel_main
    b       slave_hang

ENTRY(get_CurrentEL)
        mov x10, x30

        adrp x0, el_string1
        add x0, x0, :lo12:el_string1
        bl put_string_uart

        mrs x5, CurrentEL
        /* get the currentEL value */
        lsr x2, x5, #2
        mov x0, #48
        /* convert to ASCII */
        add x0, x0, x2
        bl put_uart
        /* print the new line tab */
        mov x0, #10
        bl put_uart

        mov x30, x10
        ret
ENDPROC(get_CurrentEL)
