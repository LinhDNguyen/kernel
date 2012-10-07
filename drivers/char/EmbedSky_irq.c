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
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>

#define DEVICE_NAME     "IRQ-Test"

struct button_irq_desc {
    int irq;
    int pin;
    int pin_setting;
    int number;
    char *name;	
};

#if !defined (CONFIG_SKY2440_IRQ_TEST)
static struct button_irq_desc button_irqs [] = {
	{IRQ_EINT1,	S3C2410_GPF1,	S3C2410_GPF1_EINT1,	0, "KEY1"}, /* K1 */
	{IRQ_EINT4,	S3C2410_GPF4,	S3C2410_GPF4_EINT4,	1, "KEY2"}, /* K2 */
	{IRQ_EINT2,	S3C2410_GPF2,	S3C2410_GPF2_EINT2,	2, "KEY3"}, /* K3 */
	{IRQ_EINT0,	S3C2410_GPF0,	S3C2410_GPF0_EINT0,	3, "KEY4"}, /* K4 */
};
#else
static struct button_irq_desc button_irqs [] = {
	{IRQ_EINT9,	S3C2410_GPG1,	S3C2410_GPG1_EINT9,	0, "KEY1"}, /* K1 */
	{IRQ_EINT11,	S3C2410_GPG3,	S3C2410_GPG3_EINT11,	1, "KEY2"}, /* K2 */
	{IRQ_EINT2,	S3C2410_GPF2,	S3C2410_GPF2_EINT2,	2, "KEY3"}, /* K3 */
	{IRQ_EINT0,	S3C2410_GPF0,	S3C2410_GPF0_EINT0,	3, "KEY4"}, /* K4 */
};
#endif
static volatile char key_values [] = {'0', '0', '0', '0'};

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

static volatile int ev_press = 0;


static irqreturn_t irq_interrupt(int irq, void *dev_id)
{
	struct button_irq_desc *button_irqs = (struct button_irq_desc *)dev_id;
	int down;

	down = !s3c2410_gpio_getpin(button_irqs->pin);

	if (down != (key_values[button_irqs->number] & 1))
	{
		key_values[button_irqs->number] = '0' + down;
		ev_press = 1;
		wake_up_interruptible(&button_waitq);
	}

	return IRQ_RETVAL(IRQ_HANDLED);
}


static int tq2440_irq_open(struct inode *inode, struct file *file)
{
	int i;
	int err = 0;

	for (i = 0; i < sizeof(button_irqs)/sizeof(button_irqs[0]); i++)
	{
		if (button_irqs[i].irq < 0)
			continue;
		err = request_irq(button_irqs[i].irq, irq_interrupt, IRQ_TYPE_EDGE_BOTH, 
                          button_irqs[i].name, (void *)&button_irqs[i]);
		if (err)
			break;
	}

	if (err)
	{
		i--;
		for (; i >= 0; i--)
		{
			if (button_irqs[i].irq < 0)
				continue;
			disable_irq(button_irqs[i].irq);
			free_irq(button_irqs[i].irq, (void *)&button_irqs[i]);
		}
		return -EBUSY;
	}

	ev_press = 1;

	return 0;
}


static int tq2440_irq_close(struct inode *inode, struct file *file)
{
	int i;

	for (i = 0; i < sizeof(button_irqs)/sizeof(button_irqs[0]); i++)
	{
		if (button_irqs[i].irq < 0)
			continue;
		free_irq(button_irqs[i].irq, (void *)&button_irqs[i]);
	}

	return 0;
}


static int tq2440_irq_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	unsigned long err;

	if (!ev_press)
	{
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		else
			wait_event_interruptible(button_waitq, ev_press);
	}

	ev_press = 0;

	err = copy_to_user(buff, (const void *)key_values, min(sizeof(key_values), count));

	return err ? -EFAULT : min(sizeof(key_values), count);
}

static unsigned int tq2440_irq_poll( struct file *file, struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &button_waitq, wait);
	if (ev_press)
		mask |= POLLIN | POLLRDNORM;
	return mask;
}


static struct file_operations dev_fops = {
	.owner	=   THIS_MODULE,
	.open	=   tq2440_irq_open,
	.release	=   tq2440_irq_close, 
	.read	=   tq2440_irq_read,
	.poll	=   tq2440_irq_poll,
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
MODULE_DESCRIPTION("IRQ Test for EmbedSky SKY2440/TQ2440 Board");
