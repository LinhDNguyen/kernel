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


#define DEVICE_NAME "GPIO-Control"

/* 应用程序执行ioctl(fd, cmd, arg)时的第2个参数 */
#define IOCTL_GPIO_ON	1
#define IOCTL_GPIO_OFF	0

/* 用来指定LED所用的GPIO引脚 */
static unsigned long gpio_table [] =
{
	S3C2410_GPB5,
	S3C2410_GPB6,
	S3C2410_GPB7,
	S3C2410_GPB8,
};

/* 用来指定GPIO引脚的功能：输出 */
static unsigned int gpio_cfg_table [] =
{
	S3C2410_GPB5_OUTP,
	S3C2410_GPB6_OUTP,
	S3C2410_GPB7_OUTP,
	S3C2410_GPB8_OUTP,
};

static int tq2440_gpio_ioctl(
	struct inode *inode, 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	if (arg > 4)
	{
		return -EINVAL;
	}

	switch(cmd)
	{
		case IOCTL_GPIO_ON:
			// 设置指定引脚的输出电平为0
			s3c2410_gpio_setpin(gpio_table[arg], 0);
			return 0;

		case IOCTL_GPIO_OFF:
			// 设置指定引脚的输出电平为1
			s3c2410_gpio_setpin(gpio_table[arg], 1);
			return 0;

		default:
			return -EINVAL;
	}
}

static struct file_operations dev_fops = {
	.owner	=	THIS_MODULE,
	.ioctl	=	tq2440_gpio_ioctl,
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
		s3c2410_gpio_cfgpin(gpio_table[i], gpio_cfg_table[i]);
		s3c2410_gpio_setpin(gpio_table[i], 0);
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
