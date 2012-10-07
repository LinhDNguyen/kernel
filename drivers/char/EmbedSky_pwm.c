/*************************************

NAME:EmbedSky_hello.c
COPYRIGHT:www.embedsky.net

*************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <plat/regs-timer.h>
#include <mach/regs-irq.h>
#include <asm/mach/time.h>
#include <linux/clk.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#define DEVICE_NAME     "PWM-Test"

static struct semaphore lock;

static int tq2440_pwm_open(struct inode *inode, struct file *file)
{
	if (!down_trylock(&lock))
		return 0;
	else
		return -EBUSY;
}


static int tq2440_pwm_close(struct inode *inode, struct file *file)
{
	up(&lock);
	return 0;
}


static int tq2440_pwm_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	unsigned long tcfg0;
	unsigned long tcfg1;
	unsigned long tcntb;
	unsigned long tcmpb;
	unsigned long tcon;

	if(cmd <= 0)
	{
		s3c2410_gpio_cfgpin(S3C2410_GPB0, S3C2410_GPB0_OUTP);
		s3c2410_gpio_setpin(S3C2410_GPB0, 0);
	}
	else
	{
		struct clk *clk_p;
		unsigned long pclk;

		//set GPB0 as tout0, pwm output
		s3c2410_gpio_cfgpin(S3C2410_GPB0, S3C2410_GPB0_TOUT0);

		tcon = __raw_readl(S3C2410_TCON);
		tcfg1 = __raw_readl(S3C2410_TCFG1);
		tcfg0 = __raw_readl(S3C2410_TCFG0);

		//prescaler = 50
		tcfg0 &= ~S3C2410_TCFG_PRESCALER0_MASK;
		tcfg0 |= (50 - 1); 

		//mux = 1/16
		tcfg1 &= ~S3C2410_TCFG1_MUX0_MASK;
		tcfg1 |= S3C2410_TCFG1_MUX0_DIV16;

		__raw_writel(tcfg1, S3C2410_TCFG1);
		__raw_writel(tcfg0, S3C2410_TCFG0);

		clk_p = clk_get(NULL, "pclk");
		pclk  = clk_get_rate(clk_p);
		tcntb  = (pclk/128)/arg;
		tcmpb = tcntb>>1;

		__raw_writel(tcntb, S3C2410_TCNTB(0));
		__raw_writel(tcmpb, S3C2410_TCMPB(0));
			
		tcon &= ~0x1f;
		tcon |= 0xb;		//start timer
		__raw_writel(tcon, S3C2410_TCON);

		tcon &= ~2;
		__raw_writel(tcon, S3C2410_TCON);
	}

	return 0;
}


static struct file_operations dev_fops = {
    .owner	=   THIS_MODULE,
    .open	=   tq2440_pwm_open,
    .release	=   tq2440_pwm_close, 
    .ioctl		=   tq2440_pwm_ioctl,
};

static struct miscdevice misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVICE_NAME,
	.fops	= &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	init_MUTEX(&lock);
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
MODULE_DESCRIPTION("PWM Drivers for EmbedSky SKY2440/TQ2440 Board");
