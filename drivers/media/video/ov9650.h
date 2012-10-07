#ifndef __S3C2440CAMIF_H__
#define __S3C2440CAMIF_H__


#define OV9650_SCCB_ADDR	0x60
#define OV9650_MANUFACT_ID	0x7FA2
#define OV9650_PRODUCT_ID	0x9650

static struct ov9650_reg
{
	u8 subaddr;
	u8 value;
}
regs[] = {
	/* OV9650 intialization parameter table for SXGA application */        
    {0x12, 0x80}, {0x39, 0x43}, {0x38, 0x12}, {0x37, 0x00}, {0x0e, 0x20},
    {0x1e, 0x0c}, {0x01, 0x80}, {0x02, 0x80}, {0x00, 0x00}, {0x10, 0xf0},
    {0x04, 0x00}, {0x0c, 0x00}, {0x0d, 0x00}, {0x11, 0x80}, {0x12, 0x00},
    {0x14, 0x2e}, {0x15, 0x00}, {0x18, 0xbd}, {0x17, 0x1d}, {0x32, 0xbf}, 
    {0x03, 0x12}, {0x1a, 0x81}, {0x19, 0x01}, {0x3f, 0xa6}, {0x41, 0x02},
    {0x42, 0x08}, {0x1b, 0x00}, {0x16, 0x06}, {0x33, 0xc0}, {0x34, 0xbf},
    {0xa8, 0x80}, {0x96, 0x04}, {0x3a, 0x00}, {0x8e, 0x00}, {0x3c, 0x77}, 
    {0x8b, 0x06}, {0x35, 0x91}, {0x94, 0x88}, {0x95, 0x88}, {0x40, 0xc1}, 
    {0x29, 0x3f}, {0x0f, 0x42}, {0x13, 0xe5}, {0x3d, 0x92}, {0x69, 0x80}, 
    {0x5c, 0x96}, {0x5d, 0x96}, {0x5e, 0x10}, {0x59, 0xeb}, {0x5a, 0x9c},
    {0x5b, 0x55}, {0x43, 0xf0}, {0x44, 0x10}, {0x45, 0x55}, {0x46, 0x86},
    {0x47, 0x64}, {0x48, 0x86}, {0x5f, 0xe0}, {0x60, 0x8c}, {0x61, 0x20},
    {0xa5, 0xd9}, {0xa4, 0x74}, {0x8d, 0x02}, {0x13, 0xe7}, {0x4f, 0x3a}, 
    {0x50, 0x3d}, {0x51, 0x03}, {0x52, 0x12}, {0x53, 0x26}, {0x54, 0x38},
    {0x55, 0x40}, {0x56, 0x40}, {0x57, 0x40}, {0x58, 0x0d}, {0x8c, 0x23},
    {0x3e, 0x02}, {0xa9, 0xb8}, {0xaa, 0x92}, {0xab, 0x0a}, {0x8f, 0xdf},
    {0x90, 0x00}, {0x91, 0x00}, {0x9f, 0x00}, {0x3a, 0x0c}, {0x24, 0x70},
    {0x25, 0x64}, {0x26, 0xc3}, {0x2a, 0x12}, {0x2b, 0x46}, {0x3b, 0x19},
    {0x6c, 0x40}, {0x6d, 0x30}, {0x6e, 0x4b}, {0x6f, 0x60},  
    {0x70, 0x70}, {0x71, 0x70}, {0x72, 0x70}, {0x73, 0x70},
    {0x74, 0x60}, {0x75, 0x60}, {0x76, 0x50}, {0x77, 0x48},
    {0x78, 0x3a}, {0x79, 0x2e}, {0x7a, 0x28}, {0x7b, 0x22},
    {0x7c, 0x04}, {0x7d, 0x07}, {0x7e, 0x10}, {0x7f, 0x28},
    {0x80, 0x36}, {0x81, 0x44}, {0x82, 0x52}, {0x83, 0x60},
    {0x84, 0x6c}, {0x85, 0x78}, {0x86, 0x8c}, {0x87, 0x9e},
    {0x88, 0xbb}, {0x89, 0xd2}, {0x8a, 0xe6},
    {0x6a, 0x41}, {0x66, 0x00},
    {0x3e, 0x00}, {0x3f, 0xa4}
};

DECLARE_MUTEX(regs_mutex);

static DECLARE_MUTEX(bus_lock);

#define SIO_C		S3C2410_GPE14
#define SIO_D		S3C2410_GPE15

#define State(x)		s3c2410_gpio_getpin(x)
#define High(x)		do{s3c2410_gpio_setpin(x,1); smp_mb();}while(0)
#define Low(x)		do{s3c2410_gpio_setpin(x,0); smp_mb();}while(0)

#define WAIT_STABLE()	do{udelay(10);}while(0)
#define WAIT_CYCLE()	do{udelay(90);}while(0)

#define CFG_READ(x)	do{s3c2410_gpio_cfgpin(x,S3C2410_GPIO_INPUT);smp_mb();}while(0)
#define CFG_WRITE(x)	do{s3c2410_gpio_cfgpin(x,S3C2410_GPIO_OUTPUT);smp_mb();}while(0)


#define MIN_C_WIDTH		32
#define MIN_C_HEIGHT		48
#define MAX_C_WIDTH		1280
#define MAX_C_HEIGHT		1024

#define MIN_P_WIDTH		32
#define MIN_P_HEIGHT		48
#define MAX_P_WIDTH		1280
#define MAX_P_HEIGHT		1024

enum
{
	CAMIF_BUFF_INVALID = 0,
	CAMIF_BUFF_RGB565 = 1,
	CAMIF_BUFF_RGB24 = 2,
	CAMIF_BUFF_YCbCr420 = 3,
	CAMIF_BUFF_YCbCr422 = 4
};

/* image buffer for s3c2440 camif. */
struct tq2440_camif_buffer
{
	int state;
	ssize_t img_size;

	unsigned int order;
	unsigned long virt_base;
	unsigned long phy_base;
};

/* for tq2440_camif_dev->state field. */
enum
{
	CAMIF_STATE_FREE = 0,		// not openned
	CAMIF_STATE_READY = 1,	// openned, but standby
	CAMIF_STATE_PREVIEWING = 2,	// in previewing
	CAMIF_STATE_CODECING = 3	// in capturing
};

/* for tq2440_camif_dev->cmdcode field. */
enum
{
	CAMIF_CMD_NONE	= 0,
	CAMIF_CMD_SFMT	= 1<<0,		// source image format changed.
	CAMIF_CMD_WND	= 1<<1,		// window offset changed.
	CAMIF_CMD_ZOOM	= 1<<2,		// zoom picture in/out
	CAMIF_CMD_TFMT	= 1<<3,		// target image format changed.
	CAMIF_CMD_P2C		= 1<<4,		// need camif switches from p-path to c-path
	CAMIF_CMD_C2P		= 1<<5,		// neet camif switches from c-path to p-path

	CAMIF_CMD_STOP	= 1<<16		// stop capture
};

/* main s3c2440 camif structure. */
struct tq2440_camif_dev
{
	/* for sub-devices */
	struct list_head devlist;

	/* minor device */
	struct video_device * vfd;

	/* hardware clock. */
	struct clk * clk;

	/* reference count. */
	struct mutex rcmutex;
	int rc;

	/* the input images's format select. */
	int input;

	/* source(input) image size. */
	int srcHsize;
	int srcVsize;

	/* windowed image size. */
	int wndHsize;
	int wndVsize;

	/* codec-path target(output) image size. */
	int coTargetHsize;
	int coTargetVsize;

	/* preview-path target(preview) image size. */
	int preTargetHsize;
	int preTargetVsize;

	/* the camera interface state. */
	int state;	// CMAIF_STATE_FREE, CAMIF_STATE_PREVIEWING, CAMIF_STATE_CAPTURING.

	/* for executing camif commands. */
	int cmdcode;				// command code, CAMIF_CMD_START, CAMIF_CMD_CFG, etc.
	wait_queue_head_t cmdqueue;	// wait queue for waiting untile command completed (if in preview or in capturing).
};

/* opened file handle.*/
struct tq2440_camif_fh
{
	/* the camif */
	struct tq2440_camif_dev	* dev;

	/* master flag, only master openner could execute 'set' ioctls. */
	int master;
};

#define S3C244X_CAMIFREG(x) ((x) + camif_base_addr)

/* CAMIF control registers */
#define S3C244X_CISRCFMT		S3C244X_CAMIFREG(0x00)
#define S3C244X_CIWDOFST		S3C244X_CAMIFREG(0x04)
#define S3C244X_CIGCTRL			S3C244X_CAMIFREG(0x08)
#define S3C244X_CICOYSA1		S3C244X_CAMIFREG(0x18)
#define S3C244X_CICOYSA2		S3C244X_CAMIFREG(0x1C)
#define S3C244X_CICOYSA3		S3C244X_CAMIFREG(0x20)
#define S3C244X_CICOYSA4		S3C244X_CAMIFREG(0x24)
#define S3C244X_CICOCBSA1		S3C244X_CAMIFREG(0x28)
#define S3C244X_CICOCBSA2		S3C244X_CAMIFREG(0x2C)
#define S3C244X_CICOCBSA3		S3C244X_CAMIFREG(0x30)
#define S3C244X_CICOCBSA4		S3C244X_CAMIFREG(0x34)
#define S3C244X_CICOCRSA1		S3C244X_CAMIFREG(0x38)
#define S3C244X_CICOCRSA2		S3C244X_CAMIFREG(0x3C)
#define S3C244X_CICOCRSA3		S3C244X_CAMIFREG(0x40)
#define S3C244X_CICOCRSA4		S3C244X_CAMIFREG(0x44)
#define S3C244X_CICOTRGFMT		S3C244X_CAMIFREG(0x48)
#define S3C244X_CICOCTRL		S3C244X_CAMIFREG(0x4C)
#define S3C244X_CICOSCPRERATIO		S3C244X_CAMIFREG(0x50)
#define S3C244X_CICOSCPREDST		S3C244X_CAMIFREG(0x54)
#define S3C244X_CICOSCCTRL		S3C244X_CAMIFREG(0x58)
#define S3C244X_CICOTAREA		S3C244X_CAMIFREG(0x5C)
#define S3C244X_CICOSTATUS		S3C244X_CAMIFREG(0x64)
#define S3C244X_CIPRCLRSA1		S3C244X_CAMIFREG(0x6C)
#define S3C244X_CIPRCLRSA2		S3C244X_CAMIFREG(0x70)
#define S3C244X_CIPRCLRSA3		S3C244X_CAMIFREG(0x74)
#define S3C244X_CIPRCLRSA4		S3C244X_CAMIFREG(0x78)
#define S3C244X_CIPRTRGFMT		S3C244X_CAMIFREG(0x7C)
#define S3C244X_CIPRCTRL		S3C244X_CAMIFREG(0x80)
#define S3C244X_CIPRSCPRERATIO		S3C244X_CAMIFREG(0x84)
#define S3C244X_CIPRSCPREDST		S3C244X_CAMIFREG(0x88)
#define S3C244X_CIPRSCCTRL		S3C244X_CAMIFREG(0x8C)
#define S3C244X_CIPRTAREA		S3C244X_CAMIFREG(0x90)
#define S3C244X_CIPRSTATUS		S3C244X_CAMIFREG(0x98)
#define S3C244X_CIIMGCPT		S3C244X_CAMIFREG(0xA0)

#endif
