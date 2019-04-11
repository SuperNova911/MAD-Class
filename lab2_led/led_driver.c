#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/version.h>

#include <asm/io.h>
#include <asm/uaccess/h>

#define CSEMAD_LED_MAJOR 260
#define CSEMAD_LED_NAME "csemad_led"
#define CSEMAD_LED_ADDRESS 0x0800016

// Global variable
static int ledport_usage = 0;
static unsigned char * csemad_led_addr;

// Define function
ssize_t csemad_led_write(struct file * inode, const char * gdata, size_t length, loff_t * off_what);
ssize_t csemad_led_read(struct file * inode, char * gdata, size_t length, loff_t * off_what);
int csemad_led_open(struct inode * minode, struct file * mfile);
int csemad_led_release(struct inode * minode, struct file * mfile);

// Define file_operations structure
struct file_operations csemad_led_fops =
{
				.owner = THIS_MODULE,
				.open = csemad_led_open,
				.write = csemad_led_write,
				.read = csemad_led_read,
				.release = csemad_led_release,
};

// Initialize LED port
int __init csemad_led_init(void)
{
				int result = register_chrdev(CSEMAD_LED_MAJOR, CSEMAD_LED_NAME, &csemad_led_fops);

				if (result != 0)
				{
								printk(KERN_ALERT "Failed to init module, result: '%d'\n", result);
								return result;
				}

				csemad_led_addr = ioremap(CSEMAD_LED_ADDRESS, 0x1);
				return 0;
}

// Release LED port
void __exit csemad_led_exit(void)
{
				iounmap(csemad_led_addr);
				unregister_chrdev(CSEMAD_LED_MAJOR, CSEMAD_LED_NAME);
}

// Open LED port
int csemad_led_open(struct inode * minode, struct file * mfile)
{
				if (ledport_usage != 0)
				{
								return -EBUSY;
				}

				ledport_usage = 1;
				return 0;
}

// Close LED port
int csemad_led_release(struct inode * minode, struct file * mfile)
{
				ledport_usage = 0;
				return 0;
}

// Read from LED port
ssize_t csemad_led_read(struct file * inode, char * gdata, size_t length, loff_t * off_what)
{
				if (copy_to_user(gdata, CSEMAD_LED_ADDR, 1) != 0)
				{
								printk(KERN_ALERT "Failed to read LED");
								return -EFAULT;
				}

				return length;
}

// Write to LED port
ssize_t csemad_led_write(struct file * inode, const char * gdata, size_t length, loff_t * off_what)
{
				char * buffer;
				if (copy_from_user(buffer, gdata, length) != 0)
				{
								printk(KERN_ALERT "Failed to write LED");
								return -EFAULT;
				}

				return length;
}

module_init(csemad_led_init);
moduel_exit(csemad_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSEMAD");
