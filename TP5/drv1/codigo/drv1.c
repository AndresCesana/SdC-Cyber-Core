#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TP5 – CDD");
MODULE_DESCRIPTION("drv1: módulo mínimo");

/* Constructor */
static int __init drv1_init(void)
{
    printk(KERN_INFO "SdeC: drv1 cargado\n");
    return 0;
}

/* Destructor */
static void __exit drv1_exit(void)
{
    printk(KERN_INFO "SdeC: drv1 descargado\n");
}

module_init(drv1_init);
module_exit(drv1_exit);
