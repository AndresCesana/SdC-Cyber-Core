#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

static dev_t first;

static int __init drv2_init(void)
{
    int ret;
    printk(KERN_INFO "SdeC_drv2 Registrado exitosamente..!!\n");
    ret = alloc_chrdev_region(&first, 0, 3, "SdeC_Driver2");
    if (ret < 0) return ret;
    printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));
    return 0;
}

static void __exit drv2_exit(void)
{
    unregister_chrdev_region(first, 3);
    printk(KERN_INFO "SdeC_drv2 dice Adios mundo cruel..!!\n");
}

module_init(drv2_init);
module_exit(drv2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra Sistemas de Computación");
MODULE_DESCRIPTION("Nuestro segundo driver de SdeC");
