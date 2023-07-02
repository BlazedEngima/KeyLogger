#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

// Module metadata
MODULE_AUTHOR("BlazedEngima");
MODULE_DESCRIPTION("Keylogger for learning");
MODULE_LICENSE("GPL");

// Custom init and exit methods
static struct proc_dir_entry *proc_entry;

static ssize_t custom_read(
    struct file *file,
    char __user *user_buffer,
    size_t count,
    loff_t *offset
) {
    printk(KERN_INFO "Calling custom read method.");

    char greeting[] = "Hello world!\n";
    int greeting_length = strlen(greeting);

    if (*offset > 0)
        return 0;

    copy_to_user(user_buffer, greeting, greeting_length);
    *offset = greeting_length;

    return greeting_length;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = custom_read
};

static int __init custom_init(void) {
    proc_entry = proc_create("helloworlddriver", 0666, NULL, &fops);
    printk(KERN_INFO "Hello world driver loaded.");
    return 0;
}

static void __exit custom_exit(void) {
    proc_remove(proc_entry);
    printk(KERN_INFO "Hello world driver removed.");
}

module_init(custom_init);
module_exit(custom_exit);