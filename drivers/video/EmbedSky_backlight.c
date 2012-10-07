/*************************************

NAME:EmbedSky_backlight.c
COPYRIGHT:www.embedsky.net

*************************************/

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/miscdevice.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <mach/regs-clock.h>
#include <plat/regs-timer.h>
	 
#include <mach/regs-gpio.h>
#include <mach/fb.h>
#include <linux/cdev.h>

#define DEVICE_NAME	"backlight"


static int tq2440_backlight_ioctl(
	struct inode *inode, 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	switch(cmd)
	{
		case 0:
			s3c2410_gpio_setpin(S3C2410_GPG4, 0);
			printk(DEVICE_NAME " Turn Off!\n");
			return 0;
		case 1:
			s3c2410_gpio_setpin(S3C2410_GPG4, 1);
			printk(DEVICE_NAME " Turn On!\n");
			return 0;
		default:
			return -EINVAL;
	}
}

static struct file_operations dev_fops = {
	.owner	=	THIS_MODULE,
	.ioctl	=	tq2440_backlight_ioctl
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	ret = misc_register(&misc);

	printk (DEVICE_NAME" initialized\n");

	s3c2410_gpio_cfgpin(S3C2410_GPG4, S3C2410_GPG4_OUTP);
	return ret;
}


static void __exit dev_exit(void)
{
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("www.embedsky.net");
MODULE_DESCRIPTION("Backlight control for EmbedSky SKY2440/TQ2440 Board");
