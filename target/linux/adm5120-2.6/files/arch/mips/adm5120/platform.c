/*
 *  $Id$
 *
 *  Generic ADM5120 platform devices
 *
 *  Copyright (C) 2007 OpenWrt.org
 *  Copyright (C) 2007 Gabor Juhos <juhosg@freemail.hu>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the
 *  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#include <asm/bootinfo.h>
#include <asm/gpio.h>

#include <asm/mach-adm5120/adm5120_defs.h>
#include <asm/mach-adm5120/adm5120_info.h>
#include <asm/mach-adm5120/adm5120_irq.h>
#include <asm/mach-adm5120/adm5120_switch.h>
#include <asm/mach-adm5120/adm5120_platform.h>

static void adm5120_uart_set_mctrl(struct amba_device *dev, void __iomem *base,
		unsigned int mctrl);

#if 1
/*
 * TODO:remove global adm5120_eth* variables when the switch driver will be
 *	converted into a real platform driver
 */
unsigned int adm5120_eth_num_ports = 6;
EXPORT_SYMBOL_GPL(adm5120_eth_num_ports);

unsigned char adm5120_eth_macs[6][6] = {
	{'\00', 'A', 'D', 'M', '\x51', '\x20' },
	{'\00', 'A', 'D', 'M', '\x51', '\x21' },
	{'\00', 'A', 'D', 'M', '\x51', '\x22' },
	{'\00', 'A', 'D', 'M', '\x51', '\x23' },
	{'\00', 'A', 'D', 'M', '\x51', '\x24' },
	{'\00', 'A', 'D', 'M', '\x51', '\x25' }
};
EXPORT_SYMBOL_GPL(adm5120_eth_macs);

unsigned char adm5120_eth_vlans[6] = {
	0x41, 0x42, 0x44, 0x48, 0x50, 0x60
};
EXPORT_SYMBOL_GPL(adm5120_eth_vlans);

#else
/* Built-in ethernet switch */
struct adm5120_switch_platform_data adm5120_switch_data;
struct platform_device adm5120_switch_device = {
	.name	= "adm5120-switch",
	.id	= -1,
	.dev.platform_data = &adm5120_switch_data,
};
#endif

/* PCI Host Controller */
struct adm5120_pci_platform_data adm5120_pci_data;
struct platform_device adm5120_pci_device = {
	.name	= "adm5120-pci",
	.id	= -1,
	.dev.platform_data = &adm5120_pci_data,
};

/* USB Host Controller */
struct resource adm5120_usbc_resources[] = {
	[0] = {
		.start	= ADM5120_USBC_BASE,
		.end	= ADM5120_USBC_BASE+ADM5120_USBC_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= ADM5120_IRQ_USBC,
		.end	= ADM5120_IRQ_USBC,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device adm5120_usbc_device = {
	.name		= "adm5120-usbc",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(adm5120_usbc_resources),
	.resource	= adm5120_usbc_resources,
};

/* NOR flash 0 */
struct adm5120_flash_platform_data adm5120_flash0_data;
struct platform_device adm5120_flash0_device =	{
	.name	= "adm5120-flash",
	.id	= 0,
	.dev.platform_data = &adm5120_flash0_data,
};

/* NOR flash 1 */
struct adm5120_flash_platform_data adm5120_flash1_data;
struct platform_device adm5120_flash1_device =	{
	.name	= "adm5120-flash",
	.id	= 1,
	.dev.platform_data = &adm5120_flash1_data,
};

/* NAND flash */
struct resource adm5120_nand_resource[] = {
	[0] = {
		.start	= ADM5120_SRAM1_BASE,
		.end	= ADM5120_SRAM1_BASE+ADM5120_MPMC_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
};

struct adm5120_nand_platform_data adm5120_nand_data;

struct platform_device adm5120_nand_device = {
	.name 		= "adm5120-nand",
	.id		= -1,
	.dev.platform_data = &adm5120_nand_data,
	.num_resources	= ARRAY_SIZE(adm5120_nand_resource),
	.resource	= adm5120_nand_resource,
};

/* built-in UARTs */
struct amba_pl010_data adm5120_uart0_data = {
	.set_mctrl = adm5120_uart_set_mctrl
};

struct amba_device adm5120_uart0_device = {
	.dev		= {
		.bus_id	= "APB:UART0",
		.platform_data = &adm5120_uart0_data,
	},
	.res		= {
		.start	= ADM5120_UART0_BASE,
		.end	= ADM5120_UART0_BASE + ADM5120_UART_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { ADM5120_IRQ_UART0, -1 },
	.periphid	= 0x0041010,
};

struct amba_pl010_data adm5120_uart1_data = {
	.set_mctrl = adm5120_uart_set_mctrl
};

struct amba_device adm5120_uart1_device = {
	.dev		= {
		.bus_id	= "APB:UART1",
		.platform_data = &adm5120_uart1_data,
	},
	.res		= {
		.start	= ADM5120_UART1_BASE,
		.end	= ADM5120_UART1_BASE + ADM5120_UART_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { ADM5120_IRQ_UART1, -1 },
	.periphid	= 0x0041010,
};

static void adm5120_uart_set_mctrl(struct amba_device *dev, void __iomem *base,
		unsigned int mctrl)
{
}
