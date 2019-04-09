#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/major.h>

MODULE_LICENSE("GPL");

int result;

int skeleton_open(struct inode * inode, struct file * filp)
{
				printk(KERN_ALERT "Device Open!!\n");
				return 0;
}

int skeleton_release(struct inode * inode, struct file * filp)
{
				printk(KERN_ALERT "Device Release!!\n");
				return 0;
}

int skeleton_read(struct file * filp, char * buf, size_t count, loff_t * f_pos)
{
				printk(KERN_ALERT "Device Read!!\n");
				return 0;
}

long skeleton_ioctl(struct file * filp, unsigned int cmd, unsigned long arg)
{
				printk(KERN_ALERT "Device Ioctl!!\n");
				return 0;
}

int skeleton_write(struct file * filp, const char * buf, size_t count, loff_t * f_pos)
{
				printk(KERN_ALERT "Device Write!!\n");
				return 0;
}

struct file_operations skeleton_fops = 
{
				.open = skeleton_open,
				.release = skeleton_release,
				.read = skeleton_read,
				.unlocked_ioctl = skeleton_ioctl,
				.write = skeleton_write
};

static int skeleton_init(void)
{
				printk(KERN_ALERT "Skeleton module init!!\n");
				result = register_chrdev(248, "skeleton", &skeleton_fops);
				printk(KERN_ALERT "return value=%d\n", result);
				return 0;
}

static void skeleton_exit(void)
{
				printk(KERN_ALERT "Skeleton module exit!!\n");
}

module_init(skeleton_init);
module_exit(skeleton_exit);
