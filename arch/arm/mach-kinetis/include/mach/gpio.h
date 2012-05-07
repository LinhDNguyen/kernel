/*
 * (C) Copyright 2012
 * Linh Nguyen <nvl1109@gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _MACH_KINETIS_GPIO_H_
#define _MACH_KINETIS_GPIO_H_

#ifndef __ASSEMBLY__

#include <mach/kinetis.h>

/*
 * GPIO registers bases
 */
#define KINETIS_PORTA_BASE	(KINETIS_AIPS0PERIPH_BASE + 0x00049000)
#define KINETIS_PORTB_BASE	(KINETIS_AIPS0PERIPH_BASE + 0x0004A000)
#define KINETIS_PORTC_BASE	(KINETIS_AIPS0PERIPH_BASE + 0x0004B000)
#define KINETIS_PORTD_BASE	(KINETIS_AIPS0PERIPH_BASE + 0x0004C000)
#define KINETIS_PORTE_BASE	(KINETIS_AIPS0PERIPH_BASE + 0x0004D000)
#define KINETIS_PORTF_BASE	(KINETIS_AIPS0PERIPH_BASE + 0x0004E000)

void __init kinetis_gpio_init(void);

#endif /* __ASSEMBLY__ */

#endif	/*_MACH_KINETIS_GPIO_H_ */
