/* === drv4.c : Character Device con file_operations + udev === */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>

static dev_t first;
static struct cdev  c_dev;
static struct class *cl;

/* ---- file operations ---- */
static int my_open  (struct inode *i, struct file *f){ pr_info("drv4: open\n");  return 0; }
static int my_close (struct inode *i, struct file *f){ pr_info("drv4: close\n"); return 0; }
static ssize_t my_read (struct file *f, char __user *b, size_t l, loff_t *o){
    pr_info("drv4: read\n"); return 0;
}
static ssize_t my_write(struct file *f, const char __user *b, size_t l, loff_t *o){
    pr_info("drv4: write – %zu bytes\n", l); return l;
}

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_close,
    .read    = my_read,
    .write   = my_write,
};

/* ---- init / exit ---- */
static int __init drv4_init(void)
{
    int ret;
    pr_info("drv4: loading\n");
    if ((ret = alloc_chrdev_region(&first, 0, 1, "SdeC_drv4"))) return ret;

    cl = class_create(THIS_MODULE, "chardrv");
    if (IS_ERR(cl)) goto err_class;

    if (IS_ERR(device_create(cl, NULL, first, NULL, "SdeC_drv4")))
        goto err_dev;

    cdev_init(&c_dev, &fops);
    if ((ret = cdev_add(&c_dev, first, 1))) goto err_cdev;

    pr_info("drv4: <major,minor> = <%d,%d>\n", MAJOR(first), MINOR(first));
    return 0;
err_cdev:  device_destroy(cl, first);
err_dev :  class_destroy(cl);
err_class: unregister_chrdev_region(first, 1);
    return ret;
}

static void __exit drv4_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    pr_info("drv4: unloaded\n");
}
module_init(drv4_init);
module_exit(drv4_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra SdC");
MODULE_DESCRIPTION("Driver 4 – char device con file_operations");
