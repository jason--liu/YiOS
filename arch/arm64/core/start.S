#include <yios/mm.h>
#include <asm/linkage.h>
#include <asm/sysreg.h>

.section  .rodata
/*if MMU is disabled an SCTLR_ELX.A is enabled
 *then, Data access need 8 bytes aligned
 */
.align 3
.global el_string1
el_string1:
        .string "Booting at EL"
boot_string:
        .string "- Switch to EL1 -"

.section ".text.boot"

	    b _start

ENTRY(_start)
	    /* Disable all interrupts */
	    msr	    daifset, #2

	    mrs     x0, mpidr_el1
	    and     x0, x0,#0xFF
	    cbz     x0, master
	    b       slave_hang
ENDPROC(_start)

slave_hang:
        yield
	    b slave_hang

master:
	    bl      __init_uart

	    mrs     x5, CurrentEL
	    cmp     x5, #CurrentEL_EL3
	    b.eq    el3_entry
	    b       el2_entry

el1_entry:
        adrp    x0, el_string1
        add     x0, x0, :lo12:boot_string
        bl      put_string_uart
        mov     x0, #10
        bl      put_uart

	    adr     x0, bss_begin
	    adr     x1, bss_end
	    sub     x2, x1, x0
	    mov     x1, xzr
	    bl      memset

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

ENTRY(el3_entry)
        bl get_CurrentEL
        eret
ENDPROC(el3_entry)

ENTRY(el2_entry)
        bl      get_CurrentEL

        /* The Execution state for EL1 is AArch64 */
        ldr     x0, =HCR_HOST_NVHE_FLAGS
        msr     hcr_el2, x0

        ldr     x0, =SCTLR_VALUE_MMU_DISABLED
        msr     sctlr_el1, x0
        mrs x6,sctlr_el1

        /* Holds the saved process state when an exception is taken to EL2 */
        /**
         *   M[3:0], bits [3:0]
         *   AArch64 Exception level and selected Stack Pointer.
         *   0b0000 EL0t.
         *   0b0100 EL1t.
         *   0b0101 EL1h.
         *   0b1000 EL2t.
         *   0b1001 EL2h.
        **/
        ldr     x0, =SPSR_EL1
        msr     spsr_el2, x0

        /* When taking an exception to EL2, holds the address to return to. */
        adr     x0, el1_entry
        msr     elr_el2, x0

        eret

ENDPROC(el2_entry)
