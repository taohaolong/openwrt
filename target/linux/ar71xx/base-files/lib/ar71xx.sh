#!/bin/sh
#
# Copyright (C) 2009 OpenWrt.org
#

ar71xx_board_name() {
	local machine
	local name

	machine=$(awk 'BEGIN{FS="[ \t]+:[ \t]"} /machine/ {print $2}' /proc/cpuinfo)

	case "$machine" in
	*LS-SR71)
		name="ls-sr71"
		;;
	*MZK-W04NU)
		name="mzk-w04nu"
		;;
	*RB-411)
		name="rb-411"
		;;
	*RB-433)
		name="rb-433"
		;;
	*RB-450)
		name="rb-450"
		;;
	*RB-493)
		name="rb-493"
		;;
	*RouterStation)
		name="routerstation"
		;;
	*TEW-632BRP)
		name="tew-632brp"
		;;
	*TL-WR941ND)
		name="tl-wr941nd"
		;;
	*WP543)
		name="wp543"
		;;
	*)
		name="generic"
		;;
	esac

	echo $name
}
