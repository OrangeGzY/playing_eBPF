#include <uapi/linux/ptrace.h>

struct string {
	size_t len;
	char  *buf;
};

int trace__do_work(struct pt_regs *ctx)
{
	struct string str;
	char buf[80];
	int ret;
        bpf_trace_printk("arg1: %d\\n arg2: 0x%x\\n arg3: %s\\n",PT_REGS_PARM1(ctx),PT_REGS_PARM2(ctx),PT_REGS_PARM3(ctx));
        
        //getchar();
/*
        ret = bpf_probe_read(&str, sizeof(str), (void *) PT_REGS_PARM1(ctx) + sizeof(unsigned long));
	bpf_trace_printk("bpf_probe_read() returned %d\\n", ret);

	ret = bpf_probe_read_str(buf, sizeof(buf), (void *) str.buf);
	bpf_trace_printk("bpf_probe_read() returned %d\\n", ret);

	bpf_trace_printk("buf = %s, len = %u\\n", buf, str.len);
*/
	return 0;
};
