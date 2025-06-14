#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kdev_t.h>

static dev_t first;

static int __init drv2_init(void)
{
    int ret;
    pr_info("SdeC_drv2: Registrado!\n");
    ret = alloc_chrdev_region(&first, 0, 3, "SdeC_Driver2");
    if (ret) return ret;
    pr_info("<Major, Minor inicial>: <%d,%d>\n", MAJOR(first), MINOR(first));
    return 0;
}
static void __exit drv2_exit(void)
{
    unregister_chrdev_region(first, 3);
    pr_info("SdeC_drv2: Desregistrado\n");
}
module_init(drv2_init);
module_exit(drv2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra SdC");
MODULE_DESCRIPTION("Driver 2 – ejemplo MAJOR/MINOR");
