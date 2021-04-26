#ifndef _YIOS_PTRACE_H_
#define _YIOS_PTRACE_H_

#include <yios/types.h>

struct pt_regs {
	struct {
		u64 regs[31];
		u64 sp;
		u64 pc;
		u64 pstate;
	};
};

#endif
