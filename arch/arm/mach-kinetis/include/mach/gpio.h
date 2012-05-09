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

#define K70_LED0_IDX		10
#define K70_LED1_IDX		11
#define K70_LED2_IDX		28
#define K70_LED3_IDX		29

/** GPIO - Peripheral register structure */
typedef struct GPIO_MemMap {
  unsigned long PDOR;                                   /**< Port Data Output Register, offset: 0x0 */
  unsigned long PSOR;                                   /**< Port Set Output Register, offset: 0x4 */
  unsigned long PCOR;                                   /**< Port Clear Output Register, offset: 0x8 */
  unsigned long PTOR;                                   /**< Port Toggle Output Register, offset: 0xC */
  unsigned long PDIR;                                   /**< Port Data Input Register, offset: 0x10 */
  unsigned long PDDR;                                   /**< Port Data Direction Register, offset: 0x14 */
} volatile *GPIO_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- GPIO - Register accessor macros
   ---------------------------------------------------------------------------- */

/* GPIO - Register accessors */
#define GPIO_PDOR_REG(base)                      ((base)->PDOR)
#define GPIO_PSOR_REG(base)                      ((base)->PSOR)
#define GPIO_PCOR_REG(base)                      ((base)->PCOR)
#define GPIO_PTOR_REG(base)                      ((base)->PTOR)
#define GPIO_PDIR_REG(base)                      ((base)->PDIR)
#define GPIO_PDDR_REG(base)                      ((base)->PDDR)

/* ----------------------------------------------------------------------------
   -- GPIO Register Masks
   ---------------------------------------------------------------------------- */

/* PDOR Bit Fields */
#define GPIO_PDOR_PDO_MASK                       0xFFFFFFFFu
#define GPIO_PDOR_PDO_SHIFT                      0
#define GPIO_PDOR_PDO(x)                         (((unsigned long)(((unsigned long)(x))<<GPIO_PDOR_PDO_SHIFT))&GPIO_PDOR_PDO_MASK)
/* PSOR Bit Fields */
#define GPIO_PSOR_PTSO_MASK                      0xFFFFFFFFu
#define GPIO_PSOR_PTSO_SHIFT                     0
#define GPIO_PSOR_PTSO(x)                        (((unsigned long)(((unsigned long)(x))<<GPIO_PSOR_PTSO_SHIFT))&GPIO_PSOR_PTSO_MASK)
/* PCOR Bit Fields */
#define GPIO_PCOR_PTCO_MASK                      0xFFFFFFFFu
#define GPIO_PCOR_PTCO_SHIFT                     0
#define GPIO_PCOR_PTCO(x)                        (((unsigned long)(((unsigned long)(x))<<GPIO_PCOR_PTCO_SHIFT))&GPIO_PCOR_PTCO_MASK)
/* PTOR Bit Fields */
#define GPIO_PTOR_PTTO_MASK                      0xFFFFFFFFu
#define GPIO_PTOR_PTTO_SHIFT                     0
#define GPIO_PTOR_PTTO(x)                        (((unsigned long)(((unsigned long)(x))<<GPIO_PTOR_PTTO_SHIFT))&GPIO_PTOR_PTTO_MASK)
/* PDIR Bit Fields */
#define GPIO_PDIR_PDI_MASK                       0xFFFFFFFFu
#define GPIO_PDIR_PDI_SHIFT                      0
#define GPIO_PDIR_PDI(x)                         (((unsigned long)(((unsigned long)(x))<<GPIO_PDIR_PDI_SHIFT))&GPIO_PDIR_PDI_MASK)
/* PDDR Bit Fields */
#define GPIO_PDDR_PDD_MASK                       0xFFFFFFFFu
#define GPIO_PDDR_PDD_SHIFT                      0
#define GPIO_PDDR_PDD(x)                         (((unsigned long)(((unsigned long)(x))<<GPIO_PDDR_PDD_SHIFT))&GPIO_PDDR_PDD_MASK)

/* GPIO - Peripheral instance base addresses */
/** Peripheral PTA base pointer */
#define PTA_BASE_PTR                             ((GPIO_MemMapPtr)0x400FF000u)
/** Peripheral PTB base pointer */
#define PTB_BASE_PTR                             ((GPIO_MemMapPtr)0x400FF040u)
/** Peripheral PTC base pointer */
#define PTC_BASE_PTR                             ((GPIO_MemMapPtr)0x400FF080u)
/** Peripheral PTD base pointer */
#define PTD_BASE_PTR                             ((GPIO_MemMapPtr)0x400FF0C0u)
/** Peripheral PTE base pointer */
#define PTE_BASE_PTR                             ((GPIO_MemMapPtr)0x400FF100u)
/** Peripheral PTF base pointer */
#define PTF_BASE_PTR                             ((GPIO_MemMapPtr)0x400FF140u)

/* ----------------------------------------------------------------------------
   -- GPIO - Register accessor macros
   ---------------------------------------------------------------------------- */

/* GPIO - Register instance definitions */
/* PTA */
#define GPIOA_PDOR                               GPIO_PDOR_REG(PTA_BASE_PTR)
#define GPIOA_PSOR                               GPIO_PSOR_REG(PTA_BASE_PTR)
#define GPIOA_PCOR                               GPIO_PCOR_REG(PTA_BASE_PTR)
#define GPIOA_PTOR                               GPIO_PTOR_REG(PTA_BASE_PTR)
#define GPIOA_PDIR                               GPIO_PDIR_REG(PTA_BASE_PTR)
#define GPIOA_PDDR                               GPIO_PDDR_REG(PTA_BASE_PTR)
/* PTB */
#define GPIOB_PDOR                               GPIO_PDOR_REG(PTB_BASE_PTR)
#define GPIOB_PSOR                               GPIO_PSOR_REG(PTB_BASE_PTR)
#define GPIOB_PCOR                               GPIO_PCOR_REG(PTB_BASE_PTR)
#define GPIOB_PTOR                               GPIO_PTOR_REG(PTB_BASE_PTR)
#define GPIOB_PDIR                               GPIO_PDIR_REG(PTB_BASE_PTR)
#define GPIOB_PDDR                               GPIO_PDDR_REG(PTB_BASE_PTR)
/* PTC */
#define GPIOC_PDOR                               GPIO_PDOR_REG(PTC_BASE_PTR)
#define GPIOC_PSOR                               GPIO_PSOR_REG(PTC_BASE_PTR)
#define GPIOC_PCOR                               GPIO_PCOR_REG(PTC_BASE_PTR)
#define GPIOC_PTOR                               GPIO_PTOR_REG(PTC_BASE_PTR)
#define GPIOC_PDIR                               GPIO_PDIR_REG(PTC_BASE_PTR)
#define GPIOC_PDDR                               GPIO_PDDR_REG(PTC_BASE_PTR)
/* PTD */
#define GPIOD_PDOR                               GPIO_PDOR_REG(PTD_BASE_PTR)
#define GPIOD_PSOR                               GPIO_PSOR_REG(PTD_BASE_PTR)
#define GPIOD_PCOR                               GPIO_PCOR_REG(PTD_BASE_PTR)
#define GPIOD_PTOR                               GPIO_PTOR_REG(PTD_BASE_PTR)
#define GPIOD_PDIR                               GPIO_PDIR_REG(PTD_BASE_PTR)
#define GPIOD_PDDR                               GPIO_PDDR_REG(PTD_BASE_PTR)
/* PTE */
#define GPIOE_PDOR                               GPIO_PDOR_REG(PTE_BASE_PTR)
#define GPIOE_PSOR                               GPIO_PSOR_REG(PTE_BASE_PTR)
#define GPIOE_PCOR                               GPIO_PCOR_REG(PTE_BASE_PTR)
#define GPIOE_PTOR                               GPIO_PTOR_REG(PTE_BASE_PTR)
#define GPIOE_PDIR                               GPIO_PDIR_REG(PTE_BASE_PTR)
#define GPIOE_PDDR                               GPIO_PDDR_REG(PTE_BASE_PTR)
/* PTF */
#define GPIOF_PDOR                               GPIO_PDOR_REG(PTF_BASE_PTR)
#define GPIOF_PSOR                               GPIO_PSOR_REG(PTF_BASE_PTR)
#define GPIOF_PCOR                               GPIO_PCOR_REG(PTF_BASE_PTR)
#define GPIOF_PTOR                               GPIO_PTOR_REG(PTF_BASE_PTR)
#define GPIOF_PDIR                               GPIO_PDIR_REG(PTF_BASE_PTR)
#define GPIOF_PDDR                               GPIO_PDDR_REG(PTF_BASE_PTR)


void __init kinetis_gpio_init(void);

#endif /* __ASSEMBLY__ */

#endif /* _MACH_KINETIS_GPIO_H_ */

