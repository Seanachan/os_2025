#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <asm/current.h>

#define procfs_name "Mythread_info"
#define BUFSIZE  1024
char buf[BUFSIZE]; //kernel buffer

static ssize_t Mywrite(struct file *fileptr, const char __user *ubuf, size_t buffer_len, loff_t *offset){
  /*Your code here*/
  char msg[128] ;
  int msg_len = buffer_len;

  if(msg_len > 127) msg_len = 127;

  if(copy_from_user(msg, ubuf, msg_len)){
    return -EFAULT;
  } 
  msg[msg_len] ='\0';

  int cur_len = strlen(buf);

  if(cur_len < BUFSIZE - 256){
    snprintf(
        buf + cur_len,
        BUFSIZE - cur_len,
        "%s",
        msg
    );

    cur_len = strlen(buf);

    snprintf(
        buf + cur_len,
        BUFSIZE - cur_len,
        "PID: %d, TID: %d, time: %llu\n",
        current->tgid,
        current->pid,
        current->utime/100/1000
    );
  } 

  return buffer_len;
  /****************/
}


static ssize_t Myread(struct file *fileptr, char __user *ubuf, size_t buffer_len, loff_t *offset){
  /*Your code here*/
  int len = strlen(buf);
  
  if(len==0) return 0;

  if (*offset >= len) {
    buf[0] ='\0';
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
    proc_create(procfs_name, 0666, NULL, &Myops);   
    pr_info("My kernel says Hi");
    return 0;
}

static void My_Kernel_Exit(void){
  pr_info("My kernel says GOODBYE");
}

module_init(My_Kernel_Init);
module_exit(My_Kernel_Exit);

MODULE_LICENSE("GPL");
