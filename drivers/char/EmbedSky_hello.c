/*************************************

NAME:EmbedSky_hello.c
COPYRIGHT:www.embedsky.net

*************************************/

#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");                

static int __init EmbedSky_hello_init(void)
{

    printk("<1>\n     Hello,EmbedSky!\n");
    printk("<1>\nThis is first driver program.\n\n");

    return 0;
}

static void __exit EmbedSky_hello_exit(void)
{
    printk("<1>\n     Exit!\n");
    printk("<1>\nGoodbye EmbedSky!\n\n");
}                                    

module_init(EmbedSky_hello_init);
module_exit(EmbedSky_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("www.embedsky.net");
MODULE_DESCRIPTION("SKY2440/TQ2440 Board First module test");
