/* linux/arch/arm/mach-s3c2440/mach-tq2440.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <plat/regs-serial.h>
#include <mach/regs-gpio.h>
#include <mach/regs-lcd.h>

#include <mach/idle.h>
#include <mach/fb.h>
#include <plat/iic.h>

#include <plat/s3c2410.h>
#include <plat/s3c244x.h>
#include <plat/clock.h>
#include <plat/devs.h>
#include <plat/cpu.h>

#include <plat/common-EmbedSky.h>

#include <linux/dm9000.h>

#include <plat/udc.h>

#include <sound/s3c24xx_uda134x.h>

#include <linux/usb/g_hid.h> /* UDC HID Support */

static struct map_desc tq2440_iodesc[] __initdata = {
};

#define UCON S3C2410_UCON_DEFAULT | S3C2410_UCON_UCLK
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE

static struct s3c2410_uartcfg tq2440_uartcfgs[] __initdata = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	}
};

/* LCD driver info */

static struct s3c2410fb_display tq2440_lcd_cfg __initdata = {

	.lcdcon5	= S3C2410_LCDCON5_FRM565 |
			  S3C2410_LCDCON5_INVVLINE |
			  S3C2410_LCDCON5_INVVFRAME |
			  S3C2410_LCDCON5_PWREN |
			  S3C2410_LCDCON5_HWSWP,

	.type		= S3C2410_LCDCON1_TFT,

#if defined(CONFIG_FB_S3C24X0_T240320)
	.width		= 240,
	.height		= 320,

	.pixclock		= 100000, /* HCLK 100 MHz, divisor 4 */
	.setclkval		= 0x4,
	.xres		= 240,
	.yres		= 320,
	.bpp		= 16,
	.left_margin	= 3,	/* for HFPD*/
	.right_margin	= 6,	/* for HBPD*/
	.hsync_len	= 1,	/* for HSPW*/
	.upper_margin	= 2,	/* for VFPD*/
	.lower_margin	= 1,	/* for VBPD*/
	.vsync_len	= 1,	/* for VSPW*/

#elif	defined(CONFIG_FB_S3C24X0_S320240)
	.width		= 320,
	.height		= 240,

	.pixclock		= 80000, /* HCLK 100 MHz, divisor 3 */
	.setclkval		= 0x3,
	.xres		= 320,
	.yres		= 240,
	.bpp		= 16,
	.left_margin	= 15,	/* for HFPD*/
	.right_margin	= 5,	/* for HBPD*/
	.hsync_len	= 8,	/* for HSPW*/
	.upper_margin	= 5,	/* for VFPD*/
	.lower_margin	= 3,	/* for VBPD*/
	.vsync_len	= 15,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_W320240)
	.width		= 320,
	.height		= 240,

	.pixclock		= 80000, /* HCLK 100 MHz, divisor 3 */
	.setclkval		= 0x3,
	.xres		= 320,
	.yres		= 240,
	.bpp		= 16,
	.left_margin	= 28,	/* for HFPD*/
	.right_margin	= 24,	/* for HBPD*/
	.hsync_len	= 42,	/* for HSPW*/
	.upper_margin	= 6,	/* for VFPD*/
	.lower_margin	= 2,	/* for VBPD*/
	.vsync_len	= 12,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_TFT480272)
	.width		= 480,
	.height		= 272,

	.pixclock		= 40000, /* HCLK 100 MHz, divisor 1 */
	.setclkval		= 0x4,
	.xres		= 480,
	.yres		= 272,
	.bpp		= 16,
	.left_margin	= 19,	/* for HFPD*/
	.right_margin	= 10,	/* for HBPD*/
	.hsync_len	= 30,	/* for HSPW*/
	.upper_margin	= 4,	/* for VFPD*/
	.lower_margin	= 2,	/* for VBPD*/
	.vsync_len	= 8,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_TFT640480)
	.width		= 640,
	.height		= 480,

	.pixclock		= 40000, /* HCLK 100 MHz, divisor 1 */
	.setclkval		= 0x1,
	.xres		= 640,
	.yres		= 480,
	.bpp		= 16,
	.left_margin	= 40,	/* for HFPD*/
	.right_margin	= 67,	/* for HBPD*/
	.hsync_len	= 31,	/* for HSPW*/
	.upper_margin	= 5,	/* for VFPD*/
	.lower_margin	= 25,	/* for VBPD*/
	.vsync_len	= 1,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_TFT800480)
	.width		= 800,
	.height		= 480,

	.pixclock		= 40000, /* HCLK 100 MHz, divisor 1 */
	.setclkval		= 0x1,
	.xres		= 800,
	.yres		= 480,
	.bpp		= 16,
	.left_margin	= 15,	/* for HFPD*/
	.right_margin	= 47,	/* for HBPD*/
	.hsync_len	= 95,	/* for HSPW*/
	.upper_margin	= 9,	/* for VFPD*/
	.lower_margin	= 5,	/* for VBPD*/
	.vsync_len	= 1,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_TFT800600)
	.width		= 800,
	.height		= 600,

	.pixclock		= 40000, /* HCLK 100 MHz, divisor 1 */
	.setclkval		= 0x1,
	.xres		= 800,
	.yres		= 600,
	.bpp		= 16,
	.left_margin	= 15,	/* for HFPD*/
	.right_margin	= 47,	/* for HBPD*/
	.hsync_len	= 95,	/* for HSPW*/
	.upper_margin	= 9,	/* for VFPD*/
	.lower_margin	= 5,	/* for VBPD*/
	.vsync_len	= 1,	/* for VSPW*/

#endif
};

static struct s3c2410fb_mach_info tq2440_fb_info __initdata = {
	.displays	= &tq2440_lcd_cfg,
	.num_displays	= 1,
	.default_display = 0,

#if 0
	/* currently setup by downloader */
	.gpccon		= 0xaa940659,
	.gpccon_mask	= 0xffffffff,
	.gpcup		= 0x0000ffff,
	.gpcup_mask	= 0xffffffff,
	.gpdcon		= 0xaa84aaa0,
	.gpdcon_mask	= 0xffffffff,
	.gpdup		= 0x0000faff,
	.gpdup_mask	= 0xffffffff,
#endif

/*	.lpcsel		= ((0xCE6) & ~7) | 1<<4, */
};

/* DM9000 */
static struct resource s3c_dm9k_resource[] = {
    [0] = {
        .start	= S3C2410_CS4,
        .end	= S3C2410_CS4 + 3,
        .flags	= IORESOURCE_MEM,
    },
    [1] = {
        .start	= S3C2410_CS4 + 4,
        .end	= S3C2410_CS4 + 4 + 3,
        .flags	= IORESOURCE_MEM,
    },
    [2] = {
        .start	= IRQ_EINT7,
        .end	= IRQ_EINT7,
        .flags	= IORESOURCE_IRQ | IRQF_TRIGGER_RISING,
    }

};

static struct dm9000_plat_data s3c_dm9k_platdata = {
    .flags	= DM9000_PLATF_16BITONLY,
};

struct platform_device s3c_device_dm9000 = {
    .name		= "dm9000",
    .id			= 0,
    .num_resources	= ARRAY_SIZE(s3c_dm9k_resource),
    .resource		= s3c_dm9k_resource,
    .dev			= {
        .platform_data = &s3c_dm9k_platdata,
    }
};

/* USB device UDC support */

static struct s3c2410_udc_mach_info EmbedSky_udc_cfg __initdata = {
	.pullup_pin = S3C2410_GPG(12),
};

/* UDA1341 */
static struct s3c24xx_uda134x_platform_data s3c24xx_uda134x_data = {
	.l3_clk = S3C2410_GPB(4),
	.l3_data = S3C2410_GPB(3),
	.l3_mode = S3C2410_GPB(2),
	.model = UDA134X_UDA1341,
};

static struct platform_device s3c_device_uda134x = {
	.name 		= "s3c24xx_uda134x",
	.id		= 0,
	.dev 		= {
		.platform_data = &s3c24xx_uda134x_data,
	}
};

static struct platform_device s3c_device_uda134x_codec = {
		.name = "uda134x-codec",
		.id = -1,
};

/* UDC HID Keyboard support */
static struct hidg_func_descriptor s3c_udc_hid_kbd_data = {
	.subclass		= 0, /* No subclass */
	.protocol		= 1, /* Keyboard */
	.report_length		= 8,
	.report_desc_length	= 63,
	.report_desc		= {
		0x05, 0x01,	/* USAGE_PAGE (Generic Desktop)	          */
		0x09, 0x06,	/* USAGE (Keyboard)                       */
		0xa1, 0x01,	/* COLLECTION (Application)               */
		0x05, 0x07,	/*   USAGE_PAGE (Keyboard)                */
		0x19, 0xe0,	/*   USAGE_MINIMUM (Keyboard LeftControl) */
		0x29, 0xe7,	/*   USAGE_MAXIMUM (Keyboard Right GUI)   */
		0x15, 0x00,	/*   LOGICAL_MINIMUM (0)                  */
		0x25, 0x01,	/*   LOGICAL_MAXIMUM (1)                  */
		0x75, 0x01,	/*   REPORT_SIZE (1)                      */
		0x95, 0x08,	/*   REPORT_COUNT (8)                     */
		0x81, 0x02,	/*   INPUT (Data,Var,Abs)                 */
		0x95, 0x01,	/*   REPORT_COUNT (1)                     */
		0x75, 0x08,	/*   REPORT_SIZE (8)                      */
		0x81, 0x03,	/*   INPUT (Cnst,Var,Abs)                 */
		0x95, 0x05,	/*   REPORT_COUNT (5)                     */
		0x75, 0x01,	/*   REPORT_SIZE (1)                      */
		0x05, 0x08,	/*   USAGE_PAGE (LEDs)                    */
		0x19, 0x01,	/*   USAGE_MINIMUM (Num Lock)             */
		0x29, 0x05,	/*   USAGE_MAXIMUM (Kana)                 */
		0x91, 0x02,	/*   OUTPUT (Data,Var,Abs)                */
		0x95, 0x01,	/*   REPORT_COUNT (1)                     */
		0x75, 0x03,	/*   REPORT_SIZE (3)                      */
		0x91, 0x03,	/*   OUTPUT (Cnst,Var,Abs)                */
		0x95, 0x06,	/*   REPORT_COUNT (6)                     */
		0x75, 0x08,	/*   REPORT_SIZE (8)                      */
		0x15, 0x00,	/*   LOGICAL_MINIMUM (0)                  */
		0x25, 0x65,	/*   LOGICAL_MAXIMUM (101)                */
		0x05, 0x07,	/*   USAGE_PAGE (Keyboard)                */
		0x19, 0x00,	/*   USAGE_MINIMUM (Reserved)             */
		0x29, 0x65,	/*   USAGE_MAXIMUM (Keyboard Application) */
		0x81, 0x00,	/*   INPUT (Data,Ary,Abs)                 */
		0xc0		/* END_COLLECTION                         */
	}
};

static struct platform_device s3c_udc_hid_kbd = {
	.name		= "hidg",
	.id		= 0,
	.num_resources	= 0,
	.resource	= 0,
	.dev 		= {
		.platform_data	= &s3c_udc_hid_kbd_data,
	}
};

static struct platform_device *tq2440_devices[] __initdata = {
	&s3c_device_ohci,
	&s3c_device_lcd,
	&s3c_device_wdt,
	&s3c_device_i2c0,
	&s3c_device_iis,
	&s3c_device_rtc,
	&s3c_device_dm9000,
	&s3c_device_usbgadget,
	&s3c_device_uda134x,
	&s3c_device_uda134x_codec,
	&samsung_asoc_dma,
	&s3c_udc_hid_kbd,
};

static void __init tq2440_map_io(void)
{
	s3c24xx_init_io(tq2440_iodesc, ARRAY_SIZE(tq2440_iodesc));
	s3c24xx_init_clocks(12000000);
	s3c24xx_init_uarts(tq2440_uartcfgs, ARRAY_SIZE(tq2440_uartcfgs));
}

static void __init tq2440_machine_init(void)
{
	s3c24xx_fb_set_platdata(&tq2440_fb_info);
	s3c_i2c0_set_platdata(NULL);

	platform_add_devices(tq2440_devices, ARRAY_SIZE(tq2440_devices));
	EmbedSky_machine_init();
	s3c24xx_udc_set_platdata(&EmbedSky_udc_cfg);
}

MACHINE_START(S3C2440, "TQ2440")
	.boot_params	= S3C2410_SDRAM_PA + 0x100,

	.init_irq	= s3c24xx_init_irq,
	.map_io		= tq2440_map_io,
	.init_machine	= tq2440_machine_init,
	.timer		= &s3c24xx_timer,
MACHINE_END
