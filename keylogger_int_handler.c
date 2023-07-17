#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BlazedEngima");

static int __init driver_init(void) {
  printk(KERN_INFO "Hello World\n");
  return 0;
}

static void __exit driver_exit(void) {
  printk(KERN_INFO "Goodbye World\n");
}

module_init(driver_init);
module_exit(driver_exit);
