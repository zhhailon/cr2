#include <linux/module.h>    
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static char modname[] = "cr2";

struct proc_dir_entry *proc_file_entry;

static int get_cr2(struct seq_file *m, void *v) {
#ifdef __x86_64__
    u64 cr2;
    __asm__ __volatile__(
            "mov %%cr2, %%rax\n\t"
            "mov %%rax, %0\n\t"
            : "=m" (cr2)
            : /* no input */
            : "%rax"
            ); 
    seq_printf(m, "0x%llx\n", cr2);
#elif defined(__i386__)
    /* not implemented */
#endif
    return 0;
}

static int open_callback(struct inode *inode, struct  file *file) {
    return single_open(file, get_cr2, NULL);
}

static const struct file_operations proc_file_fops = {
    .owner   = THIS_MODULE,
    .open    = open_callback,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};

int init_module( void ) {
    pr_info("%s: Installing module\n", modname);
    proc_file_entry = proc_create(modname, 0, NULL, &proc_file_fops);
    if(proc_file_entry == NULL)
        return -ENOMEM;
    return 0;
}

void cleanup_module( void ) {
    remove_proc_entry(modname, NULL);
    pr_info("%s: Removing module\n", modname);
}

MODULE_LICENSE("GPL");
