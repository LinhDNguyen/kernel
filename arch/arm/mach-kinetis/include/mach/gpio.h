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

#define KINETIS_GPIO_PORTA_BASE	0x400FF000
#define KINETIS_GPIO_PORTB_BASE	0x400FF040
#define KINETIS_GPIO_PORTC_BASE	0x400FF080
#define KINETIS_GPIO_PORTD_BASE	0x400FF0C0
#define KINETIS_GPIO_PORTE_BASE	0x400FF100
#define KINETIS_GPIO_PORTF_BASE	0x400FF140

#define GPIO_PORT_PDOR			0x00
#define GPIO_PORT_PSOR			0x04
#define GPIO_PORT_PCOR			0x08
#define GPIO_PORT_PTOR			0x0C
#define GPIO_PORT_PDIR			0x10
#define GPIO_PORT_PDDR			0x14

#define KINETIS_GPIO_PORT_PDO(PORT_BASE)	(PORT_BASE + GPIO_PORT_PDOR)
#define KINETIS_GPIO_PORT_PSO(PORT_BASE)	(PORT_BASE + GPIO_PORT_PSOR)
#define KINETIS_GPIO_PORT_PCO(PORT_BASE)	(PORT_BASE + GPIO_PORT_PCOR)
#define KINETIS_GPIO_PORT_PTO(PORT_BASE)	(PORT_BASE + GPIO_PORT_PTOR)
#define KINETIS_GPIO_PORT_PDI(PORT_BASE)	(PORT_BASE + GPIO_PORT_PDIR)
#define KINETIS_GPIO_PORT_PDD(PORT_BASE)	(PORT_BASE + GPIO_PORT_PDDR)

#define KINETIS_SIM_SCGC5_PORTA	9
#define KINETIS_SIM_SCGC5_PORTB	10
#define KINETIS_SIM_SCGC5_PORTC	11
#define KINETIS_SIM_SCGC5_PORTD	12
#define KINETIS_SIM_SCGC5_PORTE	13
#define KINETIS_SIM_SCGC5_PORTF	14

#define KINETIS_SIM_ALL_PORT_M	((1 << KINETIS_SIM_SCGC5_PORTA) | (1 << KINETIS_SIM_SCGC5_PORTB) | (1 << KINETIS_SIM_SCGC5_PORTC) \
								 (1 << KINETIS_SIM_SCGC5_PORTD) | (1 << KINETIS_SIM_SCGC5_PORTE) | (1 << KINETIS_SIM_SCGC5_PORTF))

void __init kinetis_gpio_init(void);

#endif /* _MACH_KINETIS_GPIO_H_ */

