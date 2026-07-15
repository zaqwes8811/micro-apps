#define CLASS_NAME "myclass"

#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/cdev.h>

static int my_major;
static char *my_dev_name = "mydrv";
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_dev;

static int __init my_init(void) {
    my_major = register_chrdev(0, my_dev_name, NULL);
    if (my_major < 0) {
        pr_err("[x86_drv] failed to register char device\n");
        return my_major;
    }
    pr_info("[x86_drv] char device %d registered\n", my_major);
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    my_class = class_create(CLASS_NAME);
#else
    my_class = class_create(THIS_MODULE, CLASS_NAME);
#endif
    if (IS_ERR(my_class)) {
        pr_err("[x86_drv] failed to create class\n");
        unregister_chrdev(my_major, my_dev_name);
        return PTR_ERR(my_class);
    }
    pr_info("[x86_drv] class created\n");
    
    my_dev = device_create(my_class, NULL, MKDEV(my_major, 0), NULL, my_dev_name);
    if (IS_ERR(my_dev)) {
        pr_err("[x86_drv] failed to create device\n");
        class_destroy(my_class);
        unregister_chrdev(my_major, my_dev_name);
        return PTR_ERR(my_dev);
    }
    pr_info("[x86_drv] device created\n");
    
    cdev_init(&my_cdev, NULL);
    cdev_add(&my_cdev, MKDEV(my_major, 0), 1);
    pr_info("[x86_drv] cdev added\n");
    
    return 0;
}

static void __exit my_exit(void) {
    cdev_del(&my_cdev);
    device_destroy(my_class, MKDEV(my_major, 0));
    class_destroy(my_class);
    unregister_chrdev(my_major, my_dev_name);
    pr_info("[x86_drv] device removed\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simplest Linux kernel driver");
