/*
 *  Atheros AR71xx SoC platform devices
 *
 *  Copyright (C) 2008 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *
 *  Parts of this file are based on Atheros' 2.6.15 BSP
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>

#include <asm/mips_machine.h>
#include <asm/mach-ar71xx/ar71xx.h>
#include <asm/mach-ar71xx/platform.h>

/*
 * OHCI (USB full speed host controller)
 */
static struct resource ar71xx_usb_ohci_resources[] = {
	[0] = {
		.start	= AR71XX_OHCI_BASE,
		.end	= AR71XX_OHCI_BASE + AR71XX_OHCI_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AR71XX_MISC_IRQ_OHCI,
		.end	= AR71XX_MISC_IRQ_OHCI,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 ar71xx_ohci_dmamask = DMA_BIT_MASK(32);
static struct platform_device ar71xx_usb_ohci_device = {
	.name		= "ar71xx-ohci",
	.id		= 0,
	.resource	= ar71xx_usb_ohci_resources,
	.num_resources	= ARRAY_SIZE(ar71xx_usb_ohci_resources),
	.dev = {
		.dma_mask		= &ar71xx_ohci_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
};

/*
 * EHCI (USB full speed host controller)
 */
static struct resource ar71xx_usb_ehci_resources[] = {
	[0] = {
		.start	= AR71XX_EHCI_BASE,
		.end	= AR71XX_EHCI_BASE + AR71XX_EHCI_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AR71XX_CPU_IRQ_USB,
		.end	= AR71XX_CPU_IRQ_USB,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 ar71xx_ehci_dmamask = DMA_BIT_MASK(32);
static struct platform_device ar71xx_usb_ehci_device = {
	.name		= "ar71xx-ehci",
	.id		= 0,
	.resource	= ar71xx_usb_ehci_resources,
	.num_resources	= ARRAY_SIZE(ar71xx_usb_ehci_resources),
	.dev = {
		.dma_mask		= &ar71xx_ehci_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
};

#define AR71XX_USB_RESET_MASK \
	(RESET_MODULE_USB_HOST | RESET_MODULE_USB_PHY \
	| RESET_MODULE_USB_OHCI_DLL)

void __init ar71xx_add_device_usb(void)
{
	ar71xx_device_stop(AR71XX_USB_RESET_MASK);
	mdelay(1000);
	ar71xx_device_start(AR71XX_USB_RESET_MASK);

	/* Turning on the Buff and Desc swap bits */
	ar71xx_usb_ctrl_wr(USB_CTRL_REG_CONFIG, 0xf0000);

	/* WAR for HW bug. Here it adjusts the duration between two SOFS */
	ar71xx_usb_ctrl_wr(USB_CTRL_REG_FLADJ, 0x20c00);

	mdelay(900);

	platform_device_register(&ar71xx_usb_ohci_device);
	platform_device_register(&ar71xx_usb_ehci_device);
}

#ifdef CONFIG_AR71XX_EARLY_SERIAL
static void __init ar71xx_add_device_uart(void) {};
#else
static struct resource ar71xx_uart_resources[] = {
	{
		.start	= AR71XX_UART_BASE,
		.end	= AR71XX_UART_BASE + AR71XX_UART_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};

#define AR71XX_UART_FLAGS (UPF_BOOT_AUTOCONF | UPF_SKIP_TEST | UPF_IOREMAP)
static struct plat_serial8250_port ar71xx_uart_data[] = {
	{
		.mapbase	= AR71XX_UART_BASE,
		.irq		= AR71XX_MISC_IRQ_UART,
		.flags		= AR71XX_UART_FLAGS,
		.iotype		= UPIO_MEM32,
		.regshift	= 2,
	}, {
		/* terminating entry */
	}
};

static struct platform_device ar71xx_uart_device = {
	.name		= "serial8250",
	.id		= PLAT8250_DEV_PLATFORM,
	.resource	= ar71xx_uart_resources,
	.num_resources	= ARRAY_SIZE(ar71xx_uart_resources),
	.dev = {
		.platform_data	= ar71xx_uart_data
	},
};

static void __init ar71xx_add_device_uart(void)
{
	ar71xx_uart_data[0].uartclk = ar71xx_ahb_freq;
	platform_device_register(&ar71xx_uart_device);
}
#endif /* CONFIG_AR71XX_EARLY_SERIAL */

static struct resource ar71xx_eth0_resources[] = {
	{
		.name	= "mac_base",
		.flags	= IORESOURCE_MEM,
		.start	= AR71XX_GE0_BASE,
		.end	= AR71XX_GE0_BASE + AR71XX_GE0_SIZE - 1,
	}, {
		.name	= "mii_ctrl",
		.flags	= IORESOURCE_MEM,
		.start	= AR71XX_MII_BASE + MII_REG_MII0_CTRL,
		.end	= AR71XX_MII_BASE + MII_REG_MII0_CTRL + 3,
	}, {
		.name	= "mac_irq",
		.flags	= IORESOURCE_IRQ,
		.start	= AR71XX_CPU_IRQ_GE0,
		.end	= AR71XX_CPU_IRQ_GE0,
	},
};

static struct ag71xx_platform_data ar71xx_eth0_data = {
	.reset_bit	= RESET_MODULE_GE0_MAC,
	.flush_reg	= DDR_REG_FLUSH_GE0,
};

static struct platform_device ar71xx_eth0_device = {
	.name		= "ag71xx",
	.id		= 0,
	.resource	= ar71xx_eth0_resources,
	.num_resources	= ARRAY_SIZE(ar71xx_eth0_resources),
	.dev = {
		.platform_data = &ar71xx_eth0_data,
	},
};

static struct resource ar71xx_eth1_resources[] = {
	{
		.name	= "mac_base",
		.flags	= IORESOURCE_MEM,
		.start	= AR71XX_GE1_BASE,
		.end	= AR71XX_GE1_BASE + AR71XX_GE1_SIZE - 1,
	}, {
		.name	= "mii_ctrl",
		.flags	= IORESOURCE_MEM,
		.start	= AR71XX_MII_BASE + MII_REG_MII1_CTRL,
		.end	= AR71XX_MII_BASE + MII_REG_MII1_CTRL + 3,
	}, {
		.name	= "mac_irq",
		.flags	= IORESOURCE_IRQ,
		.start	= AR71XX_CPU_IRQ_GE1,
		.end	= AR71XX_CPU_IRQ_GE1,
	},
};

static struct ag71xx_platform_data ar71xx_eth1_data = {
	.reset_bit	= RESET_MODULE_GE1_MAC,
	.flush_reg	= DDR_REG_FLUSH_GE1,
};

static struct platform_device ar71xx_eth1_device = {
	.name		= "ag71xx",
	.id		= 1,
	.resource	= ar71xx_eth1_resources,
	.num_resources	= ARRAY_SIZE(ar71xx_eth1_resources),
	.dev = {
		.platform_data = &ar71xx_eth1_data,
	},
};

void __init ar71xx_add_device_eth(unsigned int id, phy_interface_t phy_if_mode,
				u32 phy_mask)
{
	struct platform_device *pdev;

	switch (id) {
	case 0:
		switch (phy_if_mode) {
		case PHY_INTERFACE_MODE_MII:
			ar71xx_eth0_data.mii_if = MII0_CTRL_IF_MII;
			break;
		case PHY_INTERFACE_MODE_GMII:
			ar71xx_eth0_data.mii_if = MII0_CTRL_IF_GMII;
			break;
		case PHY_INTERFACE_MODE_RGMII:
			ar71xx_eth0_data.mii_if = MII0_CTRL_IF_RGMII;
			break;
		case PHY_INTERFACE_MODE_RMII:
			ar71xx_eth0_data.mii_if = MII0_CTRL_IF_RMII;
			break;
		default:
			BUG();
		}
		ar71xx_eth0_data.phy_if_mode = phy_if_mode;
		ar71xx_eth0_data.phy_mask = phy_mask;
		pdev = &ar71xx_eth0_device;
		break;
	case 1:
		switch (phy_if_mode) {
		case PHY_INTERFACE_MODE_RMII:
			ar71xx_eth1_data.mii_if = MII1_CTRL_IF_RMII;
			break;
		case PHY_INTERFACE_MODE_RGMII:
			ar71xx_eth1_data.mii_if = MII1_CTRL_IF_RGMII;
			break;
		default:
			BUG();
		}
		ar71xx_eth1_data.phy_if_mode = phy_if_mode;
		ar71xx_eth1_data.phy_mask = phy_mask;
		pdev = &ar71xx_eth1_device;
		break;
	default:
		pdev = NULL;
		break;
	}

	if (pdev)
		platform_device_register(pdev);
}

static struct resource ar71xx_spi_resources[] = {
	[0] = {
		.start	= AR71XX_SPI_BASE,
		.end	= AR71XX_SPI_BASE + AR71XX_SPI_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device ar71xx_spi_device = {
	.name		= "ar71xx-spi",
	.id		= -1,
	.resource	= ar71xx_spi_resources,
	.num_resources	= ARRAY_SIZE(ar71xx_spi_resources),
};

void __init ar71xx_add_device_spi(struct ar71xx_spi_platform_data *pdata,
				struct spi_board_info const *info,
				unsigned n)
{
	ar71xx_gpio_function_enable(GPIO_FUNC_SPI_EN);

	spi_register_board_info(info, n);
	ar71xx_spi_device.dev.platform_data = pdata;
	platform_device_register(&ar71xx_spi_device);
}

static int __init ar71xx_machine_setup(void)
{
	ar71xx_print_cmdline();

	ar71xx_gpio_init();

	ar71xx_add_device_uart();

	mips_machine_setup();
	return 0;
}

arch_initcall(ar71xx_machine_setup);
