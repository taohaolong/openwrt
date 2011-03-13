/*
 *  Atheros AR71xx machine type definitions
 *
 *  Copyright (C) 2008-2010 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#ifndef _AR71XX_MACHTYPE_H
#define _AR71XX_MACHTYPE_H

#include <asm/mips_machine.h>

enum ar71xx_mach_type {
	AR71XX_MACH_GENERIC = 0,
	AR71XX_MACH_AP81,	/* Atheros AP81 */
	AR71XX_MACH_AP83,	/* Atheros AP83 */
	AR71XX_MACH_AW_NR580,	/* AzureWave AW-NR580 */
	AR71XX_MACH_DIR_600_A1,	/* D-Link DIR-600 rev. A1 */
	AR71XX_MACH_DIR_615_C1,	/* D-Link DIR-615 rev. C1 */
	AR71XX_MACH_DIR_825_B1,	/* D-Link DIR-825 rev. B1 */
	AR71XX_MACH_JA76PF,	/* jjPlus JA76PF */
	AR71XX_MACH_JWAP003,	/* jjPlus JWAP003 */
	AR71XX_MACH_RB_411,	/* MikroTik RouterBOARD 411/411A/411AH */
	AR71XX_MACH_RB_411U,	/* MikroTik RouterBOARD 411U */
	AR71XX_MACH_RB_433,	/* MikroTik RouterBOARD 433/433AH */
	AR71XX_MACH_RB_433U,	/* MikroTik RouterBOARD 433UAH */
	AR71XX_MACH_RB_450,	/* MikroTik RouterBOARD 450 */
	AR71XX_MACH_RB_450G,	/* MikroTik RouterBOARD 450G */
	AR71XX_MACH_RB_493,	/* Mikrotik RouterBOARD 493/493AH */
	AR71XX_MACH_RB_750,	/* MikroTik RouterBOARD 750 */
	AR71XX_MACH_PB42,	/* Atheros PB42 */
	AR71XX_MACH_PB44,	/* Atheros PB44 */
	AR71XX_MACH_PB92,	/* Atheros PB92 */
	AR71XX_MACH_MZK_W04NU,	/* Planex MZK-W04NU */
	AR71XX_MACH_MZK_W300NH,	/* Planex MZK-W300NH */
	AR71XX_MACH_NBG460N,	/* Zyxel NBG460N/550N/550NH */
	AR71XX_MACH_TEW_632BRP,	/* TRENDnet TEW-632BRP */
	AR71XX_MACH_TL_MR3220,	/* TP-LINK TL-MR3220 */
	AR71XX_MACH_TL_MR3420,	/* TP-LINK TL-MR3420 */
	AR71XX_MACH_TL_WA901ND,	/* TP-LINK TL-WA901ND */
	AR71XX_MACH_TL_WA901ND_V2, /* TP-LINK TL-WA901ND v2 */
	AR71XX_MACH_TL_WR741ND,	/* TP-LINK TL-WR741ND */
	AR71XX_MACH_TL_WR841N_V1, /* TP-LINK TL-WR841N v1 */
	AR71XX_MACH_TL_WR941ND,	/* TP-LINK TL-WR941ND */
	AR71XX_MACH_TL_WR1043ND, /* TP-LINK TL-WR1041ND */
	AR71XX_MACH_UBNT_LSSR71, /* Ubiquiti LS-SR71 */
	AR71XX_MACH_UBNT_LSX,	/* Ubiquiti LSX */
	AR71XX_MACH_UBNT_RS,	/* Ubiquiti RouterStation */
	AR71XX_MACH_UBNT_RSPRO,	/* Ubiquiti RouterStation Pro */
	AR71XX_MACH_UBNT_BULLET_M, /* Ubiquiti Bullet M */
	AR71XX_MACH_UBNT_ROCKET_M, /* Ubiquiti Rocket M */
	AR71XX_MACH_UBNT_NANO_M, /* Ubiquiti NanoStation M */
	AR71XX_MACH_WNDR3700,	/* NETGEAR WNDR3700 */
	AR71XX_MACH_WNDR3700V2,	/* NETGEAR WNDR3700v2 */
	AR71XX_MACH_WNR2000,	/* NETGEAR WNR2000 */
	AR71XX_MACH_WP543,	/* Compex WP543 */
	AR71XX_MACH_WRT160NL,	/* Linksys WRT160NL */
	AR71XX_MACH_WRT400N,	/* Linksys WRT400N */
	AR71XX_MACH_WZR_HP_G300NH, /* Buffalo WZR-HP-G300NH */
	AR71XX_MACH_EAP7660D,	/* Senao EAP7660D */
	AR71XX_MACH_ZCN_1523H_2, /* Zcomax ZCN-1523H-2-xx */
	AR71XX_MACH_ZCN_1523H_5, /* Zcomax ZCN-1523H-5-xx */
	AR71XX_MACH_AP96,	/* Atheros AP96 */
	AR71XX_MACH_UBNT_UNIFI, /* Unifi */
};

#endif /* _AR71XX_MACHTYPE_H */
