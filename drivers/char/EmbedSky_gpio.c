/*************************************

NAME:EmbedSky_hello.c
COPYRIGHT:www.embedsky.net

*************************************/

#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>
#include <linux/mutex.h>
#include <linux/gpio.h>

#define DEVICE_NAME "GPIO-Control"

#define IOCTL_GPIO_ON	1
#define IOCTL_GPIO_OFF	0

static DEFINE_MUTEX(tq2440btn_mutex);

static unsigned int gpio_table [] =
{
	S3C2410_GPB(5),
	S3C2410_GPB(6),
	S3C2410_GPB(7),
	S3C2410_GPB(8),
};

static long tq2440_gpio_ioctl( 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	if (arg >= 4)
	{
		return -EINVAL;
	}

	switch(cmd)
	{
		case IOCTL_GPIO_ON:
			mutex_lock(&tq2440btn_mutex);
			gpio_set_value(gpio_table[arg], 0);
			mutex_unlock(&tq2440btn_mutex);
			return 0;

		case IOCTL_GPIO_OFF:
			mutex_lock(&tq2440btn_mutex);
			gpio_set_value(gpio_table[arg], 1);
			mutex_unlock(&tq2440btn_mutex);
			return 0;

		default:
			return -EINVAL;
	}
}

static struct file_operations dev_fops = {
	.owner	=	THIS_MODULE,
	.unlocked_ioctl	=	tq2440_gpio_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	int i;
	
	for (i = 0; i < 4; i++)
	{
		s3c_gpio_cfgpin(gpio_table[i], S3C_GPIO_OUTPUT);
		gpio_set_value(gpio_table[i], 1);
	}

	ret = misc_register(&misc);

	printk (DEVICE_NAME" initialized\n");

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
MODULE_DESCRIPTION("GPIO control for EmbedSky SKY2440/TQ2440 Board");
