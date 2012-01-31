/*
 *  D-Link DIR-615 rev. E4 board support
 *
 *  Copyright (C) 2010 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2012 Vadim Girlin <vadimgirlin@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <asm/mach-ath79/ath79.h>

#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "machtypes.h"
#include "nvram.h"

#define DIR_615_E4_GPIO_LED_WPS			0
#define DIR_615_E4_GPIO_LED_POWER_AMBER		1
#define DIR_615_E4_GPIO_LED_POWER_GREEN		6
#define DIR_615_E4_GPIO_LED_WAN_AMBER		7
#define DIR_615_E4_GPIO_LED_WAN_GREEN		17
#define DIR_615_E4_GPIO_LED_LAN1_GREEN		13
#define DIR_615_E4_GPIO_LED_LAN2_GREEN		14
#define DIR_615_E4_GPIO_LED_LAN3_GREEN		15
#define DIR_615_E4_GPIO_LED_LAN4_GREEN		16

#define DIR_615_E4_GPIO_BTN_RESET		8
#define DIR_615_E4_GPIO_BTN_WPS			12

#define DIR_615_E4_KEYS_POLL_INTERVAL	20
#define DIR_615_E4_KEYS_DEBOUNCE_INTERVAL (3 * DIR_615_E4_KEYS_POLL_INTERVAL)

#define DIR_615_E4_NVRAM_ADDR	0x1f030000
#define DIR_615_E4_NVRAM_SIZE	0x10000

static struct mtd_partition dir_615_e4_partitions[] = {
	{
		.name		= "u-boot",
		.offset		= 0,
		.size		= 0x030000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "nvram",
		.offset		= 0x030000,
		.size		= 0x010000,
	}, {
		.name		= "kernel",
		.offset		= 0x040000,
		.size		= 0x0e0000,
	}, {
		.name		= "rootfs",
		.offset		= 0x120000,
		.size		= 0x2c0000,
	}, {
		.name		= "mac",
		.offset		= 0x3e0000,
		.size		= 0x010000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "art",
		.offset		= 0x3f0000,
		.size		= 0x010000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "firmware",
		.offset		= 0x040000,
		.size		= 0x3a0000,
	}
};

static struct flash_platform_data dir_615_e4_flash_data = {
	.parts		= dir_615_e4_partitions,
	.nr_parts	= ARRAY_SIZE(dir_615_e4_partitions),
};


static struct gpio_led dir_615_e4_leds_gpio[] __initdata = {
	{
		.name		= "dir-615-e4:green:power",
		.gpio		= DIR_615_E4_GPIO_LED_POWER_GREEN,
	}, {
		.name		= "dir-615-e4:amber:power",
		.gpio		= DIR_615_E4_GPIO_LED_POWER_AMBER,
	}, {
		.name		= "dir-615-e4:green:wan",
		.gpio		= DIR_615_E4_GPIO_LED_WAN_GREEN,
		.active_low	= 1,
	}, {
		.name		= "dir-615-e4:amber:wan",
		.gpio		= DIR_615_E4_GPIO_LED_WAN_AMBER,
	}, {
		.name		= "dir-615-e4:green:lan1",
		.gpio		= DIR_615_E4_GPIO_LED_LAN1_GREEN,
		.active_low	= 1,
	}, {
		.name		= "dir-615-e4:green:lan2",
		.gpio		= DIR_615_E4_GPIO_LED_LAN2_GREEN,
		.active_low	= 1,
	}, {
		.name		= "dir-615-e4:green:lan3",
		.gpio		= DIR_615_E4_GPIO_LED_LAN3_GREEN,
		.active_low	= 1,
	}, {
		.name		= "dir-615-e4:green:lan4",
		.gpio		= DIR_615_E4_GPIO_LED_LAN4_GREEN,
		.active_low	= 1,
	}, {
		.name		= "dir-615-e4:blue:wps",
		.gpio		= DIR_615_E4_GPIO_LED_WPS,
		.active_low	= 1,
	}
};

static struct gpio_keys_button dir_615_e4_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = DIR_615_E4_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DIR_615_E4_GPIO_BTN_RESET,
		.active_low	= 1,
	}, {
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = DIR_615_E4_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DIR_615_E4_GPIO_BTN_WPS,
		.active_low	= 1,
	}
};

static void __init dir_615_e4_setup(void)
{
	const char *nvram = (char *) KSEG1ADDR(DIR_615_E4_NVRAM_ADDR);
	u8 *ee = (u8 *) KSEG1ADDR(0x1fff1000);
	u8 mac_buff[6];
	u8 *mac = NULL;

	if (ath79_nvram_parse_mac_addr(nvram, DIR_615_E4_NVRAM_SIZE,
				       "lan_mac=", mac_buff) == 0) {
		ath79_init_mac(ath79_eth0_data.mac_addr, mac_buff, 0);
		ath79_init_mac(ath79_eth1_data.mac_addr, mac_buff, 1);
		mac = mac_buff;
	}

	ath79_register_m25p80(&dir_615_e4_flash_data);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(dir_615_e4_leds_gpio),
				 dir_615_e4_leds_gpio);

	ath79_register_gpio_keys_polled(-1, DIR_615_E4_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(dir_615_e4_gpio_keys),
					dir_615_e4_gpio_keys);

	ath79_init_mac(ath79_eth0_data.mac_addr, mac, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, mac, 1);

	ath79_register_mdio(0, 0x0);

	/* LAN ports */
	ath79_register_eth(1);

	/* WAN port */
	ath79_register_eth(0);

	ap9x_pci_setup_wmac_led_pin(0, 1);
	ap91_pci_init(ee, mac);
}

MIPS_MACHINE(ATH79_MACH_DIR_615_E4, "DIR-615-E4", "D-Link DIR-615 rev. E4",
	     dir_615_e4_setup);
