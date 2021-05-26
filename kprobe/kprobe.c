#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/kprobes.h>
#include <linux/ptrace.h>
#include <linux/hardirq.h>
#include <linux/preempt.h>
#include <linux/ftrace.h>
//#include <stdint.h>
//统计do_fork()总共执行了几次



static int total_count = 0;

static void show_regs(struct pt_regs *ctx)
{
    //printk("====================");
    //printk("current: 0x%llx\n",get_current());
    //struct pt_regs *t_ctx = (struct pt_regs *)task_pt_regs(get_current());
    //printk("task_pt_regs: 0x%llx\n",t_ctx->sp); 
    //printk("ctx: 0x%lx\n",ctx);
    //printk("ctx ip: 0x%lx\n",ctx->ip);
    //printk("t_ctx ip: 0x%lx\n",t_ctx->ip);
    //printk("sp: 0x%lx\n",ctx->sp);
    //printk("bp: 0x%lx\n",ctx->bp);
   // printk("====================");
    //return 0;
}


static void hack(void){

    printk("successfully hack ip\n");
    return;

}
//static char hack[] = "sys_write is Hacked!\n";
//前置方法，这里可以拿到方法入参和栈，每次执行do_fork() total_count++
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    total_count++;

   // printk("rdi:0x%lx\n",regs->di);
    printk("rsi:0x%lx\n",regs->si);
    //printk("rdx:0x%lx\n",regs->dx);
    printk("rip:0x%lx\n",regs->ip);
    regs->ip = (unsigned long long)hack;
    //regs->di = 0;
    //regs->si = 0;
    //regs->dx = 0;
    //regs->sp = 0xffffffff;
    //regs->di = 0xffffffff;
    //regs->si = 0xffffffff;
    //regs->dx = 0xffffffff;
    //printk("rdi:0x%lx\nrsi:0x%lx\nrdx:0x%lx\n\n\n",regs->di,regs->si,regs->dx);
    //int fd = 1;
    //regs->di = fd;
    //regs->si = (unsigned long long )hack;
    //regs->dx = sizeof(hack);



    return 0;
}

//后置方法，这里可以拿到方法返回值
static void handler_post(struct kprobe *p, struct pt_regs *regs,
unsigned long flags)
{
    show_regs(regs);
    //printk(KERN_INFO "本次返回值: %ld\n",regs->ax);
    //regs->ip = 0;
    //printk("修改返回值为 0\n");
    //return 0;
}
//方法执行失败的回调函数
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    printk(KERN_INFO "fault_handler: p->addr = 0x%p, trap #%dn",p->addr, trapnr);
    return 0;
}
//通过kprobe这个数据结构，定义要hook的内核方法名称
static struct kprobe kp = {
    .symbol_name = "sys_write",
};



//通过register_kprobe 方法更改内核对应方法的指令
static int kprobe_init(void){
    int ret;
    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;
    kp.fault_handler = handler_fault;
    ret = register_kprobe(&kp);
    if (ret < 0) {
            printk(KERN_INFO "register_kprobe failed, returned %d\n", ret);
            return ret;
    }
    printk(KERN_INFO "Planted kprobe at %p\n", kp.addr);
    return 0;
}

//通过unregister_kprobe卸载hook
static void kprobe_exit(void){
unregister_kprobe(&kp);
    printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
}

//构造内核模块
module_init(kprobe_init);
module_exit(kprobe_exit);
MODULE_INFO(intree, "Y");
MODULE_LICENSE("GPL");
