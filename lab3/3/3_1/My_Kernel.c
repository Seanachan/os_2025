#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <asm/current.h>

#define procfs_name "Mythread_info"
#define BUFSIZE  1024
char buf[BUFSIZE];

static ssize_t Mywrite(struct file *fileptr, const char __user *ubuf, size_t buffer_len, loff_t *offset){
    /* Do nothing */
	return buffer_len;
}


static ssize_t Myread(struct file *fileptr, char __user *ubuf, size_t buffer_len, loff_t *offset){
  /*Your code here*/
  struct task_struct *thread;
  int len=0;
  if(*offset==0){
    rcu_read_lock();
    for_each_thread(current, thread){
      len += snprintf(
          buf + len,
          BUFSIZE - len,
          "PID: %d TID: %d Priority: %d State: %u\n",
          current->pid, thread->pid, thread->prio, thread->__state
          );

    } 
    rcu_read_unlock();
  } else{
    len = strlen(buf);
  } 

  if (*offset >= len) {
    return 0;
  }

  // Calculate how much to copy
  int bytes_to_copy = len - *offset;
  if (bytes_to_copy > buffer_len) {
    bytes_to_copy = buffer_len;
  }

  if (copy_to_user(ubuf, buf + *offset, bytes_to_copy)) {
    return -EFAULT;
  }

  // Advance the offset
  *offset += bytes_to_copy;
  return bytes_to_copy;
  /****************/
}

static struct proc_ops Myops = {
    .proc_read = Myread,
    .proc_write = Mywrite,
};

static int My_Kernel_Init(void){
    proc_create(procfs_name, 0644, NULL, &Myops);   
    pr_info("My kernel says Hi");
    return 0;
}

static void My_Kernel_Exit(void){
    pr_info("My kernel says GOODBYE");
}

module_init(My_Kernel_Init);
module_exit(My_Kernel_Exit);

MODULE_LICENSE("GPL");
