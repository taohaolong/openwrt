/*
 * Copyright (C) 2010 Scott Nicholas <neutronscott@scottn.us>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/mtd/physmap.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <asm/reboot.h>
#include <asm/time.h>
#include <asm/addrspace.h>
#include <asm/bootinfo.h>
#include <asm/io.h>
#include <adm8668.h>

static struct resource uart_resources[] = {
	{
		.start		= ADM8668_UART0_BASE,
		.end		= ADM8668_UART0_BASE + 0xF,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= INT_LVL_UART0,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct platform_device adm8668_uart_device = {
	.name		= "adm8668_uart",
	.id		= 0,
	.resource	= uart_resources,
	.num_resources	= ARRAY_SIZE(uart_resources),
};

static struct resource eth0_resources[] = {
	{
		.start		= ADM8668_LAN_BASE,
		.end		= ADM8668_LAN_BASE + 256,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= INT_LVL_LAN,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct platform_device adm8668_eth0_device = {
	.name		= "adm8668_eth",
	.id		= 0,
	.resource	= eth0_resources,
	.num_resources	= ARRAY_SIZE(eth0_resources),
};

static struct resource eth1_resources[] = {
	{
		.start		= ADM8668_WAN_BASE,
		.end		= ADM8668_WAN_BASE + 256,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= INT_LVL_WAN,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct platform_device adm8668_eth1_device = {
	.name		= "adm8668_eth",
	.id		= 1,
	.resource	= eth1_resources,
	.num_resources	= ARRAY_SIZE(eth1_resources),
};

int __devinit adm8668_devs_register(void)
{
	platform_device_register(&adm8668_uart_device);
	platform_device_register(&adm8668_eth0_device);
	platform_device_register(&adm8668_eth1_device);

	return 0;
}

void __init plat_time_init(void)
{
	int adj = (ADM8668_CONFIG_REG(ADM8668_CR3) >> 11) & 0xf;

	/* adjustable clock selection
	   CR3 bit 14~11, 0000 -> 175MHz, 0001 -> 180MHz, etc... */

	mips_hpt_frequency = (SYS_CLOCK + adj * 5000000) / 2;
	printk("ADM8668 CPU clock: %d MHz\n", 2*mips_hpt_frequency / 1000000);
}

arch_initcall(adm8668_devs_register);
