#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yawen Pan");
MODULE_DESCRIPTION("Listing all task(peocess).");
MODULE_VERSION("0.01");

static int __init list_tasks(void){
	printk(KERN_INFO "Entry!\n");
	
	struct task_struct *task;
	for_each_process(task){
		printk(KERN_INFO "name: %s	pid: %d  state:%d\n",\
				task->comm, task->pid, task->state);
	}

	return 0;
}

static void __exit exit_list_tasks(void){
	printk(KERN_INFO, "Exit!\n");
}

module_init(list_tasks);
module_exit(exit_list_tasks);
