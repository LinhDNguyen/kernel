/*
 * =====================================================================================
 *
 *       Filename:  leds.c
 *
 *    Description:  This is a test module.
 *
 *        Version:  1.0
 *        Created:  05/03/2012 11:08:04 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Linh Nguyen (LN), nvl1109@gmail.com
 *        Company:  FPT Software
 *
 * =====================================================================================
 */

/* Necessary includes for drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>     /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    /* O_ACCMODE */
#include <linux/ioport.h>
#include <asm/system.h>     /* cli(), *_flags */
#include <asm/uaccess.h>    /* copy_from/to_user */
#include <asm/io.h>         /* inb, outb */
#include <asm/string.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("TWRK70F120M Leds driver - LinhNV");

#define SUCCESS 0

#define LEDS_MAJOR      119

#define SIM_SCGC5_ADDR  0x40048038
#define SIM_SCGC5_SIZE  4
#define SIM_SCGC5_PORTA_IDX 9

#define PORTA_MUX_ADDR  0x40049000
#define PORTA_MUX_SIZE  0xC8
#define PORT_MUX_M      0x00000700
#define PORT_MUX_S      8
#define PORT_MUX(__x)   (0x00 | ((__x << PORT_MUX_S) & PORT_MUX_M))
#define LED_ORA_MUX_A   0x2C
#define LED_YEL_MUX_A   0x70
#define LED_GRN_MUX_A   0x74
#define LED_BLUE_MUX_A  0x28

#define LED_PORT_SIZE   0x18
#define LED_PORT_REGION 0x400FF000

#define LED_PDOR        0x00
#define LED_PSOR        0x04
#define LED_PCOR        0x08
#define LED_PTOR        0x0C
#define LED_PDIR        0x10
#define LED_PDDR        0x14

#define LED_ORA_IDX     11
#define LED_YEL_IDX     28
#define LED_GRN_IDX     29
#define LED_BLUE_IDX    10

#define ALL_LED_MASK    (0x00 | ((1 << LED_ORA_IDX) | (1 << LED_YEL_IDX) | (1 << LED_GRN_IDX) | (1 << LED_BLUE_IDX)))
#define DEFAULT_LEDS_CLR_M  ((1 << LED_ORA_IDX) | (1 << LED_GRN_IDX))
#define DEFAULT_LEDS_SET_M  ((1 << LED_YEL_IDX) | (1 << LED_BLUE_IDX))
#define LED_ORA_M       (1 << LED_ORA_IDX)
#define LED_YEL_M       (1 << LED_YEL_IDX)
#define LED_GRN_M       (1 << LED_GRN_IDX)
#define LED_BLUE_M      (1 << LED_BLUE_IDX)

/* Function declaration of leds.c */
int leds_open(struct inode *inode, struct file *filp);
int leds_release(struct inode *inode, struct file *filp);
ssize_t leds_read(struct file *filp, char *buf,
                       size_t count, loff_t *f_pos);
ssize_t leds_write(struct file *filp, char *buf,
                       size_t count, loff_t *f_pos);
void leds_exit(void);
int leds_init(void);

/* Structure that declares the common */
/* file access fcuntions */
struct file_operations leds_fops = {
    read: leds_read,
    write: leds_write,
    open: leds_open,
    release: leds_release
};

/* Driver global variables */
/* Major number */
int leds_major = LEDS_MAJOR;

/* Control variable for memory */
/* reservation of the parallel port*/
int port;
long leds_buf;
char cur_buf[200];
char *bufp;
static int Device_Open = 0;

module_init(leds_init);
module_exit(leds_exit);

int leds_init(void) {
    int result;
    long tmp;

    /* Registering device */
    result = register_chrdev(leds_major, "leds", &leds_fops);
    if (result < 0) {
        printk("<1>leds: cannot obtain major number %d\n", leds_major);
        return result;
    }

    /*~~~~~~~~~~~~Enable PORTA clock~~~~~~~~~~~~~*/
    port = check_region(SIM_SCGC5_ADDR, SIM_SCGC5_SIZE);
    if (port) {
        printk("<1>leds: cannot reserve SIM_SCGC5_ADDR\n");
        result = port;
        goto fail;
    }
    request_region(SIM_SCGC5_ADDR, SIM_SCGC5_SIZE, "sim");
    //enable clock for PORTA
    outl(0x00 | (1<<SIM_SCGC5_PORTA_IDX), SIM_SCGC5_ADDR);
    if (!port){
        release_region(SIM_SCGC5_ADDR, SIM_SCGC5_SIZE);
    }

    /*~~~~~~~~~~~~Init LEDs MUX~~~~~~~~~~~~~*/
    port = check_region(PORTA_MUX_ADDR, PORTA_MUX_SIZE);
    if (port) {
        printk("<1>leds: cannot reserve PORTA_MUX_ADDR\n");
        result = port;
        goto fail;
    }
    request_region(PORTA_MUX_ADDR, PORTA_MUX_SIZE, "sim");
    // Set LEDs' MUX to 1
    outl(PORT_MUX(1), (PORTA_MUX_ADDR + LED_ORA_MUX_A));
    outl(PORT_MUX(1), (PORTA_MUX_ADDR + LED_YEL_MUX_A));
    outl(PORT_MUX(1), (PORTA_MUX_ADDR + LED_GRN_MUX_A));
    outl(PORT_MUX(1), (PORTA_MUX_ADDR + LED_BLUE_MUX_A));
    if (!port){
        release_region(PORTA_MUX_ADDR, PORTA_MUX_SIZE);
    }

    /*~~~~~~~~~~~~Init LEDs output~~~~~~~~~~*/
    port = check_region(LED_PORT_REGION, LED_PORT_SIZE);
    if (port) {
        printk("<1>leds: cannot reserve LED_PORT_REGION\n");
        result = port;
        goto fail;
    }
    request_region(LED_PORT_REGION, LED_PORT_SIZE, "leds");
    //-----------init leds as output-----------
    tmp = inl(LED_PORT_REGION + LED_PDDR);      //get current directions
    outl(tmp | (0x00 | ALL_LED_MASK), (LED_PORT_REGION + LED_PDDR));    //Set port data direction register
    //-----------set leds to 1010--------------
    outl(0x00 | DEFAULT_LEDS_SET_M, (LED_PORT_REGION + LED_PSOR));
    outl(0x00 | DEFAULT_LEDS_CLR_M, (LED_PORT_REGION + LED_PCOR));
    //---------------------------

    printk("<1>Inserting leds module\n");
    return 0;

    fail:
        leds_exit();
        return result;
}

void leds_exit(void) {
    /* Make major number free! */
    unregister_chrdev(leds_major, "leds");

    /* Make port free! */
    if (!port) {
        release_region(LED_PORT_REGION, LED_PORT_SIZE);
    }
    //---------------------------

    printk("<1>Removing leds module\n");
}

int leds_open(struct inode *inode, struct file *filp) {

    leds_buf = inl(LED_PORT_REGION + LED_PDIR) & ALL_LED_MASK;
    sprintf(cur_buf, "Current LEDs status: \n    ORANGE: %s\n    YELLOW: %s\n    GREEN : %s\n    BLUE  : %s\n",
            ((leds_buf & (1<<LED_ORA_IDX))  == 0 ? "ON" : "OFF"),
            ((leds_buf & (1<<LED_YEL_IDX))  == 0 ? "ON" : "OFF"),
            ((leds_buf & (1<<LED_GRN_IDX))  == 0 ? "ON" : "OFF"),
            ((leds_buf & (1<<LED_BLUE_IDX)) == 0 ? "ON" : "OFF")
    );
    /* Buffer to read the device */
    bufp = cur_buf;

    if (Device_Open)
        return -EBUSY;

    Device_Open++;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

int leds_release(struct inode *inode, struct file *filp) {

    Device_Open--;      /* We're now ready for our next caller */
    /*
     * Decrement the usage count, or else once you opened the file, you'll
     * never get get rid of the module.
     */
    module_put(THIS_MODULE);

    return 0;
}

ssize_t leds_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
    /*
     * Number of bytes actually written to the buffer
     */
    int bytes_read = 0;

    /*
     * If we're at the end of the message,
     * return 0 signifying end of file
     */
    if (*bufp == 0)
        return 0;

    /*
     * Actually put the data into the buffer
     */
    while (count && *bufp) {

        /*
         * The buffer is in the user data segment, not the kernel
         * segment so "*" assignment won't work.  We have to use
         * put_user which copies data from the kernel data segment to
         * the user data segment.
         */
        put_user(*(bufp++), buf++);

        count--;
        bytes_read++;
    }

    /*
     * Most read functions return the number of bytes put into the buffer
     */
    return bytes_read;
}

ssize_t leds_write( struct file *filp, char *buf, size_t count, loff_t *f_pos) {
    char tmp_buf[100] = {0};
    char cmd_buf[100] = {0};
    int len = count;

    tmp_buf[count] = '\0';

    copy_from_user(tmp_buf, buf, count);

    /*Analyze command from input*/
    if(strlen(tmp_buf) <= 2){
        printk("Usage: 0|1|2|3 on/off\n");
    }else{
        char accept_char[] = "0123onf";
        int i = -1, first=-1, last=-1;
        char *pos_p;
        char is_on = 0;

        first = strcspn(tmp_buf, accept_char);
        if(strstr(tmp_buf, "on") > 0){
            pos_p = strstr(tmp_buf, "on");
            last = (unsigned int)pos_p - (unsigned int)tmp_buf + 2;
            is_on = 1;
        }else if(strstr(tmp_buf, "off") > 0){
            pos_p = strstr(tmp_buf, "off");
            last = (unsigned int)pos_p - (unsigned int)tmp_buf + 3;
            is_on = 0;
        }

        if(first<0 || last<0 || (last - first)<=0){
            printk("invalid command.\nUsage: 0|1|2|3 on/off\n");
        }else{
            memcpy(cmd_buf, &tmp_buf[first], last - first);
            printk("cmd: [%s]\n", cmd_buf);
            //find ' ' character
            pos_p = strchr(cmd_buf, ' ');
            last = (unsigned int)pos_p - (unsigned int)cmd_buf;
            for(i=0; i<last; ++i){
                switch(cmd_buf[i]){
                    case '0':
                        printk("LED 0 - ORANGE is %s\n", is_on ? "ON" : "OFF");
                        outl(0x00 | LED_ORA_M, (LED_PORT_REGION + (is_on ? LED_PCOR : LED_PSOR)));
                        break;
                    case '1':
                        printk("LED 1 - YELLOW is %s\n", is_on ? "ON" : "OFF");
                        outl(0x00 | LED_YEL_M, (LED_PORT_REGION + (is_on ? LED_PCOR : LED_PSOR)));
                        break;
                    case '2':
                        printk("LED 2 - GREEN is %s\n", is_on ? "ON" : "OFF");
                        outl(0x00 | LED_GRN_M, (LED_PORT_REGION + (is_on ? LED_PCOR : LED_PSOR)));
                        break;
                    case '3':
                        printk("LED 3 - BLUE is %s\n", is_on ? "ON" : "OFF");
                        outl(0x00 | LED_BLUE_M, (LED_PORT_REGION + (is_on ? LED_PCOR : LED_PSOR)));
                        break;
                    default:
                        printk("Invalid led number\n");
                        break;
                }
            }
        }
    }

    return len;
}
