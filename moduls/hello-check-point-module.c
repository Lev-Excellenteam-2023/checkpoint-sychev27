#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.03");

static char* my_parameter = "World";
module_param(my_parameter, charp, S_IRUSR | S_IWUSR);

static int __init lkm_example_init(void) {
	printk(KERN_INFO "Hello %s\n", my_parameter); // print it when loading modul into kernel
	return 0;
}

static void __exit lkm_example_exit(void) {
	printk(KERN_INFO "Goodbye, Check Point!\n"); // print it when unloading modul from kernel
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
