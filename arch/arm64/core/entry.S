#include <asm/linkage.h>
#include <asm/asm-offsets.h>
#include <asm/assembler.h>

/*
 * Bad Abort numbers
 *-----------------
 */
#define BAD_SYNC        0
#define BAD_IRQ         1
#define BAD_FIQ         2
#define BAD_ERROR       3

        .macro invalid, reason
        mov     x0, sp
        mov     x1, #\reason
        mrs x2, esr_el1
        b       do_bad_mode
        .endm

/*
 * Exception vector entry
 */
        .macro ventry label
        .align  7
        b       \label
        .endm


        .macro trap_entry
        sub sp, sp, #STACK_FRAME_SIZE
        stp x0, x1, [sp,#16 * 0]
        stp x2, x3, [sp,#16 * 1]
        stp x4, x5, [sp,#16 * 2]
        stp x6, x7, [sp,#16 * 3]
        stp x8, x9, [sp,#16 * 4]
        stp x10, x11, [sp,#16 * 5]
        stp x12, x13, [sp,#16 * 6]
        stp x14, x15, [sp,#16 * 7]
        stp x16, x17, [sp,#16 * 8]
        stp x18, x19, [sp,#16 * 9]
        stp x20, x21, [sp,#16 * 10]
        stp x22, x23, [sp,#16 * 11]
        stp x24, x25, [sp,#16 * 12]
        stp x26, x27, [sp,#16 * 13]
        stp x28, x29, [sp,#16 * 14]

        add x21, sp,#STACK_FRAME_SIZE
        mrs x22,elr_el1
        mrs x23,spsr_el1

        stp lr,x21, [sp, #S_LR]
        stp x22, x23, [sp, #S_PC]
        .endm

        .macro trap_exit
        ldp x21, x22, [sp, #S_PC]      // load elr, spsr
        msr elr_el1, x21
        msr spsr_el1, x22

        ldp x0, x1, [sp,#16 * 0]
        ldp x2, x3, [sp,#16 * 1]
        ldp x4, x5, [sp,#16 * 2]
        ldp x6, x7, [sp,#16 * 3]
        ldp x8, x9, [sp,#16 * 4]
        ldp x10, x11, [sp,#16 * 5]
        ldp x12, x13, [sp,#16 * 6]
        ldp x14, x15, [sp,#16 * 7]
        ldp x16, x17, [sp,#16 * 8]
        ldp x18, x19, [sp,#16 * 9]
        ldp x20, x21, [sp,#16 * 10]
        ldp x22, x23, [sp,#16 * 11]
        ldp x24, x25, [sp,#16 * 12]
        ldp x26, x27, [sp,#16 * 13]
        ldp x28, x29, [sp,#16 * 14]

        ldr lr, [sp, #S_LR]
        add sp, sp,#STACK_FRAME_SIZE
        .endm

.align  11
ENTRY(vector_table)
	    /* Current EL with SP0 */
	    ventry el1_sync_invalid         // Synchronous EL1t
	    ventry el1_irq_invalid          // IRQ EL1t
	    ventry el1_fiq_invalid          // FIQ EL1t
	    ventry el1_error_invalid        // Error EL1t

	    /* Current EL with SPx */
	    ventry el1_sync_invalid                 // Synchronous EL1h
	    ventry el1_irq                  // IRQ EL1h
	    ventry el1_fiq_invalid          // FIQ EL1h
	    ventry el1_error_invalid        // Error EL1h

	    /* Lower EL using AArch64 */
	    ventry el0_sync                 // Synchronous 64-bit EL0
        ventry el0_irq                  // IRQ 64-bit EL0
        ventry el0_fiq_invalid          // FIQ 64-bit EL0
	    ventry el0_error_invalid        // Error 64-bit EL0

	    /* Lower EL using AArch32 */
	    ventry el0_sync_invalid         // Synchronous 32-bit EL0
	    ventry el0_irq_invalid          // IRQ 32-bit EL0
	    ventry el0_fiq_invalid          // FIQ 32-bit EL0
	    ventry el0_error_invalid        // Error 32-bit EL0
END(vector_table)


 /*
 * SYNC & IRQ exception handler.
 */
    .align 6
el1_sync:
        trap_entry
	    mov     x0, sp
	    bl      do_sync
	    trap_exit
	    eret
ENDPROC(el1_sync)

tsk .req x28
    .align 6
el1_irq:
	    trap_entry
	    mov     x0, sp
	    bl      do_irq
        get_thread_info tsk
        ldr w24, [tsk, #TI_PREEMPT]
        cbnz w24, 1f
        ldr w0, [tsk, #NEED_RESCHED]
        cbz w0, 1f
        bl el1_preempt
1:      trap_exit
	    eret
ENDPROC(el1_irq)

el1_preempt:
    mov x24,lr
    bl preempt_schedule_irq
    ret x24

.align 6
el0_sync:
	    trap_entry
	    mov     x0, sp
	    bl      do_sync
	    trap_exit
	    eret
ENDPROC(el0_sync)

    .align 6
el0_irq:
	    trap_entry
	    mov     x0, sp
	    bl      do_irq
	    trap_exit
	    eret
ENDPROC(el0_irq)


 el0_sync_invalid:
    invalid BAD_SYNC
ENDPROC(el0_sync_invalid)

el0_irq_invalid:
    invalid BAD_IRQ
ENDPROC(el0_irq_invalid)

el0_fiq_invalid:
    invalid BAD_FIQ
ENDPROC(el0_fiq_invalid)

el0_error_invalid:
    invalid BAD_ERROR
ENDPROC(el0_error_invalid)

el1_sync_invalid:
    invalid BAD_SYNC
ENDPROC(el1_sync_invalid)

el1_irq_invalid:
    invalid BAD_IRQ
ENDPROC(el1_irq_invalid)

el1_fiq_invalid:
    invalid BAD_FIQ
ENDPROC(el1_fiq_invalid)

el1_error_invalid:
    invalid BAD_ERROR
ENDPROC(el1_error_invalid)

string_t                        :
.string "t"
  .global traigger_alignment
traigger_alignment:
  ldr x0,=0x80002
 ldr x1,[x0]
  //str wzr,[x0]
  ret

ENTRY(ret_to_user)
    invalid BAD_ERROR
END(ret_to_user)

ENTRY(ret_from_fork)
    bl schedule_tail /* new thread need to clean up prev thread's garbage*/
    cbz x19, 1f  /* not a kernel thread */
    mov x0, x20
    blr x19
1:
    b ret_to_user
ENDPROC(ret_from_fork)

/*
 * cpu_switch_to(struct task_struct *prev,
 *              struct task_struct *next) ;
 */
ENTRY(cpu_switch_to)
    add x8, x0,#THREAD_CPU_CONTEXT
    mov x9, sp
    stp x19,x20, [x8],#16
	stp x21,x22, [x8],#16
	stp x23,x24, [x8],#16
	stp x25,x26, [x8],#16
	stp x27,x28, [x8],#16
	stp x29,x9, [x8],#16
	str x30, [x8]

	add x8, x1,#THREAD_CPU_CONTEXT
	ldp x19,x20, [x8],#16
	ldp x21,x22, [x8],#16
	ldp x23,x24, [x8],#16
	ldp x25,x26, [x8],#16
	ldp x27,x28, [x8],#16
	ldp x29,x9, [x8],#16
	ldr x30, [x8]
	mov sp, x9
	msr	sp_el0, x1
	ret
ENDPROC(cpu_switch_to)
