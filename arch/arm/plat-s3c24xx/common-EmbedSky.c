/* linux/arch/arm/plat-s3c24xx/common-EmbedSky.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/sysdev.h>
#include <linux/platform_device.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
#include <linux/io.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <asm/mach-types.h>
#include <mach/hardware.h>
#include <asm/irq.h>

#include <mach/regs-gpio.h>
#include <mach/leds-gpio.h>

#include <plat/nand.h>

#include <plat/common-EmbedSky.h>
#include <plat/devs.h>
#include <plat/pm.h>


/* NAND parititon from 2.4.18-swl5 */

static struct mtd_partition EmbedSky_default_nand_part[] = {
#if	defined(CONFIG_EmbedSky_64MB_NAND)
	[0] = {
		.name	= "Boardcon_Board_uboot",
		.offset	= 0x00000000,
		.size	= 0x00040000,
	},
	[1] = {
		.name	= "Boardcon_Board_kernel",
		.offset	= 0x00200000,
		.size	= 0x00200000,
	},
	[2] = {
		.name	= "Boardcon_Board_yaffs2",
		.offset	= 0x00400000,
		.size	= 0x03BF8000,
	}
#elif	defined(CONFIG_EmbedSky_128MB_NAND)
	[0] = {
		.name	= "Boardcon_Board_uboot",
		.offset	= 0x00000000,
		.size	= 0x00040000,
	},
	[1] = {
		.name	= "Boardcon_Board_kernel",
		.offset	= 0x00200000,
		.size	= 0x00200000,
	},
	[2] = {
		.name	= "Boardcon_Board_yaffs2",
		.offset	= 0x00400000,
		.size	= 0x07BA0000,
	}
#elif	defined(CONFIG_EmbedSky_more_than_256MB_NAND)
	[0] = {
		.name	= "Boardcon_Board_uboot",
		.offset	= 0x00000000,
		.size	= 0x00040000,
	},
	[1] = {
		.name	= "Boardcon_Board_kernel",
		.offset	= 0x00200000,
		.size	= 0x00200000,
	},
	[2] = {
		.name	= "Boardcon_Board_yaffs2",
		.offset	= 0x00400000,
		.size	= 0x0FB80000,
	}
#elif	defined(CONFIG_EmbedSky_1GB_NAND)
	[0] = {
		.name	= "Boardcon_Board_uboot",
		.offset	= 0x00000000,
		.size	= 0x00040000,
	},
	[1] = {
		.name	= "Boardcon_Board_kernel",
		.offset	= 0x00200000,
		.size	= 0x00200000,
	},
	[2] = {
		.name	= "Boardcon_Board_yaffs2",
		.offset	= 0x00400000,
		.size	= 0x3FB80000,
	}
#endif
};

static struct s3c2410_nand_set EmbedSky_nand_sets[] = {
	[0] = {
		.name		= "NAND",
		.nr_chips		= 1,
		.nr_partitions	= ARRAY_SIZE(EmbedSky_default_nand_part),
		.partitions		= EmbedSky_default_nand_part,
	},
};

/* choose a set of timings which should suit most 512Mbit
 * chips and beyond.
*/

static struct s3c2410_platform_nand EmbedSky_nand_info = {
	.tacls		= 10,
	.twrph0		= 25,
	.twrph1		= 10,
	.nr_sets		= ARRAY_SIZE(EmbedSky_nand_sets),
	.sets		= EmbedSky_nand_sets,
};

/* devices we initialise */

static struct platform_device __initdata *EmbedSky_devs[] = {
	&s3c_device_nand,
	&s3c_device_sdi,
};

void __init EmbedSky_machine_init(void)
{
	s3c_device_nand.dev.platform_data = &EmbedSky_nand_info;

	platform_add_devices(EmbedSky_devs, ARRAY_SIZE(EmbedSky_devs));

	s3c_pm_init();
}
