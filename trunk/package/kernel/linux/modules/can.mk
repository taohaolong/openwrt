#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

CAN_MENU:=CAN Support

define KernelPackage/can
  SUBMENU:=$(CAN_MENU)
  TITLE:=CAN bus support
  KCONFIG:=\
	CONFIG_CAN=m \
	CONFIG_CAN_DEV \
	CONFIG_CAN_CALC_BITTIMING=y \
	CONFIG_CAN_LEDS=y \
	CONFIG_CAN_AT91=n \
	CONFIG_CAN_TI_HECC=n \
	CONFIG_CAN_MCP251X=n \
	CONFIG_CAN_BFIN=n \
	CONFIG_CAN_JANZ_ICAN3=n \
	CONFIG_PCH_CAN=n \
	CONFIG_CAN_GRCAN=n \
	CONFIG_CAN_CC770=n \
	CONFIG_CAN_C_CAN=n \
	CONFIG_CAN_MSCAN=n \
	CONFIG_CAN_SJA1000=n \
	CONFIG_CAN_SOFTING=n \
	CONFIG_NET_EMATCH_CANID=n \
	CONFIG_CAN_DEBUG_DEVICES=n
  FILES:=$(LINUX_DIR)/drivers/net/can/can-dev.ko \
	 $(LINUX_DIR)/net/can/can.ko
  AUTOLOAD:=$(call AutoProbe,can can-dev)
endef

define KernelPackage/can/description
 Kernel module for CAN bus support.
endef

$(eval $(call KernelPackage,can))


define AddDepends/can
  SUBMENU:=$(CAN_MENU)
  DEPENDS+=kmod-can $(1)
endef


define KernelPackage/can-raw
  TITLE:=Raw CAN Protcol
  KCONFIG:=CONFIG_CAN_RAW
  FILES:=$(LINUX_DIR)/net/can/can-raw.ko
  AUTOLOAD:=$(call AutoProbe,can-raw)
  $(call AddDepends/can)
endef

define KernelPackage/can-raw/description
 The raw CAN protocol option offers access to the CAN bus via
 the BSD  socket API.
endef

$(eval $(call KernelPackage,can-raw))


define KernelPackage/can-bcm
  TITLE:=Broadcast Manager CAN Protcol
  KCONFIG:=CONFIG_CAN_BCM
  FILES:=$(LINUX_DIR)/net/can/can-bcm.ko
  AUTOLOAD:=$(call AutoProbe,can-bcm)
  $(call AddDepends/can)
endef

define KernelPackage/can-bcm/description
 The Broadcast Manager offers content filtering, timeout monitoring,
 sending of RTR frames, and cyclic CAN messages without permanent user
 interaction.
endef

$(eval $(call KernelPackage,can-bcm))


define KernelPackage/can-gw
  TITLE:=CAN Gateway/Router
  KCONFIG:=CONFIG_CAN_GW
  FILES:=$(LINUX_DIR)/net/can/can-gw.ko
  AUTOLOAD:=$(call AutoProbe,can-gw)
  $(call AddDepends/can)
endef

define KernelPackage/can-gw/description
 The CAN Gateway/Router is used to route (and modify) CAN frames.
endef

$(eval $(call KernelPackage,can-gw))


define KernelPackage/can-vcan
  TITLE:=Virtual Local CAN Interface (vcan)
  KCONFIG:=CONFIG_CAN_VCAN
  FILES:=$(LINUX_DIR)/drivers/net/can/vcan.ko
  AUTOLOAD:=$(call AutoProbe,vcan)
  $(call AddDepends/can)
endef

define KernelPackage/can-vcan/description
 Similar to the network loopback devices, vcan offers a
 virtual local CAN interface.
endef

$(eval $(call KernelPackage,can-vcan))


define KernelPackage/can-slcan
  TITLE:=Serial / USB serial CAN Adaptors (slcan)
  KCONFIG:=CONFIG_CAN_SLCAN
  FILES:=$(LINUX_DIR)/drivers/net/can/slcan.ko
  AUTOLOAD:=$(call AutoProbe,slcan)
  $(call AddDepends/can)
endef

define KernelPackage/can-slcan/description
 CAN driver for several 'low cost' CAN interfaces that are attached
 via serial lines or via USB-to-serial adapters using the LAWICEL
 ASCII protocol.
endef

$(eval $(call KernelPackage,can-slcan))


define KernelPackage/can-flexcan
  TITLE:=Support for Freescale FLEXCAN based chips
  KCONFIG:=CONFIG_CAN_FLEXCAN
  FILES:=$(LINUX_DIR)/drivers/net/can/flexcan.ko
  AUTOLOAD:=$(call AutoProbe,flexcan)
  $(call AddDepends/can,@TARGET_imx6)
endef

define KernelPackage/can-flexcan/description
 Freescale FLEXCAN CAN bus controller implementation.
endef

$(eval $(call KernelPackage,can-flexcan))


define KernelPackage/can-usb-ems
  TITLE:=EMS CPC-USB/ARM7 CAN/USB interface
  KCONFIG:=CONFIG_CAN_EMS_USB
  FILES:=$(LINUX_DIR)/drivers/net/can/usb/ems_usb.ko
  AUTOLOAD:=$(call AutoProbe,ems_usb)
  $(call AddDepends/can,+kmod-usb-core)
endef

define KernelPackage/can-usb-ems/description
 This driver is for the one channel CPC-USB/ARM7 CAN/USB interface
 from EMS Dr. Thomas Wuensche (http://www.ems-wuensche.de).
endef

$(eval $(call KernelPackage,can-usb-ems))


define KernelPackage/can-usb-esd
  TITLE:=ESD USB/2 CAN/USB interface
  KCONFIG:=CONFIG_CAN_ESD_USB2
  FILES:=$(LINUX_DIR)/drivers/net/can/usb/esd_usb2.ko
  AUTOLOAD:=$(call AutoProbe,esd_usb2)
  $(call AddDepends/can,+kmod-usb-core)
endef

define KernelPackage/can-usb-esd/description
 This driver supports the CAN-USB/2 interface
 from esd electronic system design gmbh (http://www.esd.eu).
endef

$(eval $(call KernelPackage,can-usb-esd))


define KernelPackage/can-usb-kvaser
  TITLE:=Kvaser CAN/USB interface
  KCONFIG:=CONFIG_CAN_KVASER_USB
  FILES:=$(LINUX_DIR)/drivers/net/can/usb/kvaser_usb.ko
  AUTOLOAD:=$(call AutoProbe,kvaser_usb)
  $(call AddDepends/can,+kmod-usb-core)
endef

define KernelPackage/can-usb-kvaser/description
 This driver adds support for Kvaser CAN/USB devices like Kvaser
 Leaf Light.
endef

$(eval $(call KernelPackage,can-usb-kvaser))


define KernelPackage/can-usb-peak
  TITLE:=PEAK PCAN-USB/USB Pro interfaces
  KCONFIG:=CONFIG_CAN_PEAK_USB
  FILES:=$(LINUX_DIR)/drivers/net/can/usb/peak_usb/peak_usb.ko
  AUTOLOAD:=$(call AutoProbe,peak_usb)
  $(call AddDepends/can,+kmod-usb-core)
endef

define KernelPackage/can-usb-peak/description
 This driver supports the PCAN-USB and PCAN-USB Pro adapters
 from PEAK-System Technik (http://www.peak-system.com).
endef

$(eval $(call KernelPackage,can-usb-peak))


define KernelPackage/can-usb-8dev
  TITLE:=8 devices USB2CAN interface
  KCONFIG:=CONFIG_CAN_8DEV_USB
  FILES:=$(LINUX_DIR)/drivers/net/can/usb/usb_8dev.ko
  AUTOLOAD:=$(call AutoProbe,usb_8dev)
  $(call AddDepends/can,+kmod-usb-core)
endef

define KernelPackage/can-usb-8dev/description
 This driver supports the USB2CAN interface
 from 8 devices (http://www.8devices.com).
endef

$(eval $(call KernelPackage,can-usb-8dev))
