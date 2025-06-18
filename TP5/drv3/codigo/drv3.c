#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>

static dev_t first;
static struct cdev c_dev;
static struct class *cl;

/* operaciones */
static int my_open (struct inode *i, struct file *f){ pr_info("drv3: open\n");  return 0; }
static int my_close(struct inode *i, struct file *f){ pr_info("drv3: close\n"); return 0; }
static ssize_t my_read (struct file *f, char __user *b, size_t l, loff_t *o){
    pr_info("drv3: read\n"); return 0;
}
static ssize_t my_write(struct file *f, const char __user *b, size_t l, loff_t *o){
    pr_info("drv3: write – %zu bytes\n", l); return l;
}

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_close,
    .read    = my_read,
    .write   = my_write,
};

/* init / exit */
static int __init drv3_init(void)
{
    int ret;
    struct device *dev_ret;

    pr_info("drv3: loading\n");

    if ((ret = alloc_chrdev_region(&first, 0, 1, "SdeC_drv3")))
        return ret;

    cl = class_create("chardrv");          /* sin THIS_MODULE en kernels ≥ 5.17 */
    if (IS_ERR(cl)){
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    dev_ret = device_create(cl, NULL, first, NULL, "SdeC_drv3");
    if (IS_ERR(dev_ret)){
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &fops);
    if ((ret = cdev_add(&c_dev, first, 1))){
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }

    pr_info("drv3: <major,minor>=<%d,%d>\n", MAJOR(first), MINOR(first));
    return 0;
}

static void __exit drv3_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    pr_info("drv3: unloaded\n");
}
module_init(drv3_init);
module_exit(drv3_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra SdC");
MODULE_DESCRIPTION("Driver 3 – char device con file_operations");
