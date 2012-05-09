/*
 * (C) Copyright 2012
 * Emcraft Systems, <www.emcraft.com>
 * Alexander Potashev <aspotashev@emcraft.com>
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

/*
 * Freescale Kinetis processor definitions
 */
#ifndef _MACH_KINETIS_H_
#define _MACH_KINETIS_H_

#include <asm/byteorder.h>

/*
 * This Kinetis port assumes that the CPU works in little-endian mode.
 * Switching to big-endian will require different bit offsets in peripheral
 * devices' registers. Also, some bit groups may lay on byte edges, so issue
 * with big-endian cannot be fixed only by defining bit offsets differently
 * for the big-endian mode.
 */
#ifndef __LITTLE_ENDIAN
#error This Kinetis port assumes that the CPU works in little-endian mode
#endif

/*
 * Peripheral memory map
 */
#define KINETIS_AIPS0PERIPH_BASE	0x40000000
#define KINETIS_AIPS1PERIPH_BASE	0x40080000

#ifndef __ASSEMBLY__

#include <asm/types.h>

/*
 * Limits for the `kinetis_periph_enable()` function:
 *     1. The number of SIM_SCGC[] registers
 *     2. The number of bits in those registers
 */
#define KINETIS_SIM_CG_NUMREGS	7
#define KINETIS_SIM_CG_NUMBITS	32

/*
 * System Integration Module (SIM) register map
 *
 * This map actually covers two hardware modules:
 *     1. SIM low-power logic, at 0x40047000
 *     2. System integration module (SIM), at 0x40048000
 */
struct kinetis_sim_regs {
	u32 sopt1;	/* System Options Register 1 */
	u32 rsv0[1024];
	u32 sopt2;	/* System Options Register 2 */
	u32 rsv1;
	u32 sopt4;	/* System Options Register 4 */
	u32 sopt5;	/* System Options Register 5 */
	u32 sopt6;	/* System Options Register 6 */
	u32 sopt7;	/* System Options Register 7 */
	u32 rsv2[2];
	u32 sdid;	/* System Device Identification Register */
	u32 scgc[KINETIS_SIM_CG_NUMREGS];	/* Clock Gating Regs 1...7 */
	u32 clkdiv1;	/* System Clock Divider Register 1 */
	u32 clkdiv2;	/* System Clock Divider Register 2 */
	u32 fcfg1;	/* Flash Configuration Register 1 */
	u32 fcfg2;	/* Flash Configuration Register 2 */
	u32 uidh;	/* Unique Identification Register High */
	u32 uidmh;	/* Unique Identification Register Mid-High */
	u32 uidml;	/* Unique Identification Register Mid Low */
	u32 uidl;	/* Unique Identification Register Low */
	u32 clkdiv3;	/* System Clock Divider Register 3 */
	u32 clkdiv4;	/* System Clock Divider Register 4 */
	u32 mcr;	/* Misc Control Register */
};

enum sim_csgc_number{
	csgc1 = 0,
	csgc2,
	csgc3,
	csgc4,
	csgc5,
	csgc6,
	csgc7
};

/*
 * SIM registers base
 */
#define KINETIS_SIM_BASE		(KINETIS_AIPS0PERIPH_BASE + 0x00047000)
#define KINETIS_SIM			((volatile struct kinetis_sim_regs *) \
					KINETIS_SIM_BASE)
	// SCGC5 Bit Fields
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_LPTIMER_M         0x1
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_LPTIMER_S         0
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_REGFILE_M         0x2
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_REGFILE_S         1
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_TSI_M             0x20
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_TSI_S             5
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTA_M           0x200
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTA_S           9
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTB_M           0x400
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTB_S           10
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTC_M           0x800
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTC_S           11
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTD_M           0x1000
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTD_S           12
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTE_M           0x2000
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTE_S           13
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_DRYICE_M                   0x4u
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_DRYICE_S                   2
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_DRYICESECREG_M             0x8u
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_DRYICESECREG_S             3
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTF_M                    0x4000u
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTF_S                    14
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_ALL_M            \
				(CYGHWR_HAL_KINETIS_SIM_SCGC5_LPTIMER_M | \
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_REGFILE_M | \
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_TSI_M |	  \
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTA_M |   \
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTB_M |   \
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTC_M |   \
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTD_M |   \
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTE_M |   \
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_DRYICE_M |  \
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_DRYICESECREG_M |	\
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTF_M )
#define CYGHWR_HAL_KINETIS_SIM_SCGC5_ALLPORT_M 			\
				(CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTA_M	|	\
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTB_M	|	\
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTC_M	|	\
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTD_M	|	\
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTE_M	|	\
				 CYGHWR_HAL_KINETIS_SIM_SCGC5_PORTF_M)

#endif /* __ASSEMBLY__ */

#endif /* _MACH_KINETIS_H_ */
