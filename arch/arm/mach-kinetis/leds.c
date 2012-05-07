/*
 * LEDs initialization for the Kinetis-based boards
 *
 * Copyright (C) 2012
 * Linh Nguyen <nvl1109@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/init.h>
#include <linux/platform_device.h>

#include <mach/kinetis.h>
#include <mach/platform.h>
#include <mach/gpio.h>

/*
 * Provide support for LEDs control.
 */


/*
 * Register the Flash platform device with the kernel.
 */
void __init kinetis_gpio_init(void)
{
	unsigned int size = 0;

	/*
	 * Calculate Flash and partition sizes at run time
	 */
	switch (kinetis_platform_get()) {
	case PLATFORM_KINETIS_TWR_K70F120M:
		size = 256*1024*1024;
		break;
	case PLATFORM_KINETIS_K70_SOM:
		size = 128*1024*1024;
		/* The NAND flash chip is 8-bit */
		flash_data.flags |= FSL_NFC_NAND_FLAGS_BUSWIDTH_8;
		break;
	default:
		printk(KERN_ERR "%s: Unknown platform %#x, exit\n", __func__,
			kinetis_platform_get());
		goto xit;
	}

#ifdef CONFIG_MTD_PARTITIONS
	flash_partitions[2].size = size - FLASH_JFFS2_OFFSET;
#endif /* CONFIG_MTD_PARTITIONS */

	/*
	 * Register a platform device for the external Flash.
	 * If there is no external Flash in your design, or
	 * you don't want to use it in your application, just
	 * comment out the line below.
	 */
	platform_device_register(&nfc_device);
xit:
	return;
}
