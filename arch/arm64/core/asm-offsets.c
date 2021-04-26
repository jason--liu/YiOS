#include <asm/ptrace.h>
#include<yios/types.h>
#include <yios/kbuild.h>

int main(void)
{
        DEFINE(STACK_FRAME_SIZE, sizeof(struct pt_regs));
        DEFINE(S_X0,    offsetof (struct pt_regs, regs[0]));
        DEFINE(S_X1,    offsetof(struct pt_regs, regs[1]));
        DEFINE(S_X2,    offsetof(struct pt_regs, regs[2]));
        DEFINE(S_X3,    offsetof(struct pt_regs, regs[3]));
        DEFINE(S_X4,    offsetof(struct pt_regs, regs[4]));
        DEFINE(S_X5,    offsetof(struct pt_regs, regs[5]));
        DEFINE(S_X6,    offsetof(struct pt_regs, regs[6]));
        DEFINE(S_X7,    offsetof(struct pt_regs, regs[7]));
        DEFINE(S_X8,    offsetof(struct pt_regs, regs[8]));
        DEFINE(S_X10,   offsetof(struct pt_regs, regs[10]));
        DEFINE(S_X12,   offsetof(struct pt_regs, regs[12]));
        DEFINE(S_X14,   offsetof(struct pt_regs, regs[14]));
        DEFINE(S_X16,   offsetof(struct pt_regs, regs[16]));
        DEFINE(S_X18,   offsetof(struct pt_regs, regs[18]));
        DEFINE(S_X20,   offsetof(struct pt_regs, regs[20]));
        DEFINE(S_X22,   offsetof(struct pt_regs, regs[22]));
        DEFINE(S_X24,   offsetof(struct pt_regs, regs[24]));
        DEFINE(S_X26,   offsetof(struct pt_regs, regs[26]));
        DEFINE(S_X28,   offsetof(struct pt_regs, regs[28]));
        DEFINE(S_FP,    offsetof(struct pt_regs, regs[29]));
        DEFINE(S_LR,    offsetof(struct pt_regs, regs[30]));
        DEFINE(S_SP,    offsetof(struct pt_regs, sp));
        DEFINE(S_PSTATE, offsetof(struct pt_regs, pstate));
        DEFINE(S_PC,     offsetof(struct pt_regs, pc));
        return 0;
}
