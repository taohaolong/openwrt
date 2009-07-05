/*
 *  Atheros AR71xx PCI setup code
 *
 *  Copyright (C) 2008-2009 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *
 *  Parts of this file are based on Atheros' 2.6.15 BSP
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <asm/mach-ar71xx/ar71xx.h>
#include <asm/mach-ar71xx/pci.h>

unsigned ar71xx_pci_nr_irqs __initdata;
struct ar71xx_pci_irq *ar71xx_pci_irq_map __initdata;

int (*ar71xx_pci_plat_dev_init)(struct pci_dev *dev);

static int ar71xx_be_handler(struct pt_regs *regs, int is_fixup)
{
	int err = 0;

	err = ar71xx_pci_be_handler(is_fixup);

	return (is_fixup && !err) ? MIPS_BE_FIXUP : MIPS_BE_FATAL;
}

int pcibios_plat_dev_init(struct pci_dev *dev)
{
	if (ar71xx_pci_plat_dev_init)
		return ar71xx_pci_plat_dev_init(dev);

	return 0;
}

int __init pcibios_map_irq(const struct pci_dev *dev, uint8_t slot, uint8_t pin)
{
	return ar71xx_pcibios_map_irq(dev, slot, pin);
}

int __init ar71xx_pci_init(unsigned nr_irqs, struct ar71xx_pci_irq *map)
{
	ar71xx_pci_nr_irqs = nr_irqs;
	ar71xx_pci_irq_map = map;

	board_be_handler = ar71xx_be_handler;

	return ar71xx_pcibios_init();
}
