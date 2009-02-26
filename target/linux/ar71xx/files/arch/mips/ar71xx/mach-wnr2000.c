/*
 *  NETGEAR WNR2000 board support
 *
 *  Copyright (C) 2008-2009 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *  Copyright (C) 2008-2009 Andy Boyett <agb@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/input.h>

#include <asm/mips_machine.h>

#include <asm/mach-ar71xx/ar71xx.h>

#include "devices.h"

#define WNR2000_GPIO_LED_PWR_GREEN	14
#define WNR2000_GPIO_LED_PWR_AMBER	7
#define WNR2000_GPIO_LED_WPS		4
#define WNR2000_GPIO_LED_WLAN		6
#define WNR2000_GPIO_BTN_RESET		21
#define WNR2000_GPIO_BTN_WPS		8

#define WNR2000_BUTTONS_POLL_INTERVAL	20

#ifdef CONFIG_MTD_PARTITIONS
static struct mtd_partition wnr2000_partitions[] = {
	{
		.name		= "u-boot",
		.offset		= 0,
		.size		= 0x040000,
		.mask_flags	= MTD_WRITEABLE,
	} , {
		.name		= "u-boot-env",
		.offset		= 0x040000,
		.size		= 0x010000,
	} , {
		.name		= "rootfs",
		.offset		= 0x050000,
		.size		= 0x240000,
	} , {
		.name		= "user-config",
		.offset		= 0x290000,
		.size		= 0x010000,
	} , {
		.name		= "uImage",
		.offset		= 0x2a0000,
		.size		= 0x120000,
	} , {
		.name		= "language_table",
		.offset		= 0x3c0000,
		.size		= 0x020000,
	} , {
		.name		= "rootfs_checksum",
		.offset		= 0x3e0000,
		.size		= 0x010000,
	} , {
		.name		= "art",
		.offset		= 0x3f0000,
		.size		= 0x010000,
		.mask_flags	= MTD_WRITEABLE,
	}
};
#endif /* CONFIG_MTD_PARTITIONS */

static struct flash_platform_data wnr2000_flash_data = {
#ifdef CONFIG_MTD_PARTITIONS
        .parts          = wnr2000_partitions,
        .nr_parts       = ARRAY_SIZE(wnr2000_partitions),
#endif
};

static struct spi_board_info wnr2000_spi_info[] = {
	{
		.bus_num	= 0,
		.chip_select	= 0,
		.max_speed_hz	= 25000000,
		.modalias	= "m25p80",
		.platform_data  = &wnr2000_flash_data,
	}
};

static struct gpio_led wnr2000_leds_gpio[] __initdata = {
	{
		.name		= "wnr2000:green:power",
		.gpio		= WNR2000_GPIO_LED_PWR_GREEN,
		.active_low	= 1,
	}, {
		.name		= "wnr2000:amber:power",
		.gpio		= WNR2000_GPIO_LED_PWR_AMBER,
		.active_low	= 1,
	}, {
		.name		= "wnr2000:green:wps",
		.gpio		= WNR2000_GPIO_LED_WPS,
		.active_low	= 1,
	}, {
		.name		= "wnr2000:blue:wlan",
		.gpio		= WNR2000_GPIO_LED_WLAN,
		.active_low	= 1,
	}
};

static struct gpio_button wnr2000_gpio_buttons[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= BTN_0,
		.threshold	= 5,
		.gpio		= WNR2000_GPIO_BTN_RESET,
	}, {
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= BTN_1,
		.threshold	= 5,
		.gpio		= WNR2000_GPIO_BTN_WPS,
	}
};

static void __init wnr2000_setup(void)
{
	ar71xx_add_device_spi(NULL, wnr2000_spi_info,
					ARRAY_SIZE(wnr2000_spi_info));

	ar71xx_add_device_leds_gpio(-1, ARRAY_SIZE(wnr2000_leds_gpio),
					wnr2000_leds_gpio);

	ar71xx_add_device_gpio_buttons(-1, WNR2000_BUTTONS_POLL_INTERVAL,
					ARRAY_SIZE(wnr2000_gpio_buttons),
					wnr2000_gpio_buttons);


	ar91xx_add_device_wmac();
}

MIPS_MACHINE(AR71XX_MACH_WNR2000, "NETGEAR WNR2000", wnr2000_setup);
