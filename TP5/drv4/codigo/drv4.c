#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

static dev_t first;
static struct cdev  c_dev;
static struct class *cl;
static char c;

/* operaciones */
static int my_open (struct inode *i, struct file *f){ pr_info("drv4: open\n");  return 0; }
static int my_close(struct inode *i, struct file *f){ pr_info("drv4: close\n"); return 0; }

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    pr_info("drv4: read\n");
    if (*off == 0){
        if (copy_to_user(buf, &c, 1)) return -EFAULT;
        (*off)++;                     /* EOF tras primer byte */
        return 1;
    }
    return 0;                         /* fin de archivo */
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("drv4: write\n");
    if (copy_from_user(&c, buf + len - 1, 1)) return -EFAULT;
    return len;
}

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_close,
    .read    = my_read,
    .write   = my_write,
};

/* init / exit */
static int __init drv4_init(void)
{
    int ret;
    struct device *dev_ret;

    pr_info("drv4: loading\n");

    if ((ret = alloc_chrdev_region(&first, 0, 1, "SdeC_drv4"))) return ret;

    cl = class_create("SdeC_drive");          /* ← SIN THIS_MODULE */
    if (IS_ERR(cl)){ unregister_chrdev_region(first,1); return PTR_ERR(cl); }

    dev_ret = device_create(cl, NULL, first, NULL, "SdeC_drv4");
    if (IS_ERR(dev_ret)){
        class_destroy(cl); unregister_chrdev_region(first,1); return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &fops);
    if ((ret = cdev_add(&c_dev, first, 1))){
        device_destroy(cl, first); class_destroy(cl);
        unregister_chrdev_region(first,1); return ret;
    }
    pr_info("drv4: <major,minor>=<%d,%d>\n", MAJOR(first), MINOR(first));
    return 0;
}

static void __exit drv4_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first,1);
    pr_info("drv4: unloaded\n");
}
module_init(drv4_init);
module_exit(drv4_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra SdC");
MODULE_DESCRIPTION("Driver 4 – char device con buffer de 1 byte");
