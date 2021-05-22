#!/usr/bin/python

from __future__ import print_function
from bcc import BPF
from bcc.utils import printb
from time import sleep

# load BPF program

prog = """
//#include <sys/ptrace.h>
#include <uapi/linux/ptrace.h>


int trace__do_work(struct pt_regs *ctx)
{
        char new_arg3[] = "aaaaaaaa";
        u64 pid = bpf_get_current_pid_tgid();
        bpf_trace_printk("prog pid: %d\\n",pid);
        bpf_trace_printk("enter hit\\n");
        bpf_trace_printk("arg1: %d\\n arg2: 0x%x\\n arg3: %s\\n",PT_REGS_PARM1(ctx),PT_REGS_PARM2(ctx),PT_REGS_PARM3(ctx));

        bpf_trace_printk("rsp: 0x%llx\\n",ctx->sp);
        bpf_trace_printk("rip: 0x%llx\\n",ctx->ip);
        bpf_trace_printk("rbp: 0x%llx\\n",ctx->bp);
        ctx->di = 0;
        return 0;
};
"""
#prog = ""
prog += """
//#include <uapi/linux/ptrace.h>
int trace__do_work_ret(struct pt_regs *ctx)
{
    bpf_trace_printk("return hit\\n");
    bpf_trace_printk("rsp: 0x%llx\\n",ctx->sp);
    bpf_trace_printk("rbp:  0x%llx\\n",ctx->bp);
    bpf_trace_printk("rip: 0x%llx\\n",ctx->ip);
    return 0;

}



"""

b = BPF(text = prog)
b.attach_uprobe(name="./main", sym="test2", fn_name="trace__do_work")
b.attach_uretprobe(name = "./main", sym ="test2", fn_name="trace__do_work_ret")
b.trace_print()
# sleep until Ctrl-C
try:
	sleep(99999999)
except KeyboardInterrupt:
	pass
