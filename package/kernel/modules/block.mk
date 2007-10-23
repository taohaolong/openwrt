#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id$

BLOCK_MENU:=Block Devices

define KernelPackage/ata-core
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=Serial and Parallel ATA support
  DEPENDS:=@PCI_SUPPORT @LINUX_2_6
  KCONFIG:=CONFIG_ATA
  FILES:=$(LINUX_DIR)/drivers/ata/libata.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,21,libata)
endef

$(eval $(call KernelPackage,ata-core))


define KernelPackage/ata-artop
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=ARTOP 6210/6260 PATA support
  DEPENDS:=kmod-ata-core +kmod-scsi-core
  KCONFIG:=CONFIG_PATA_ARTOP
  FILES:=$(LINUX_DIR)/drivers/ata/pata_artop.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,41,pata_artop)
endef

define KernelPackage/ata-artop/description
 PATA support for ARTOP 6210/6260 host controllers.
endef

$(eval $(call KernelPackage,ata-artop))


define KernelPackage/ata-piix
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=Intel PIIX PATA/SATA support
  DEPENDS:=kmod-ata-core +kmod-ide-core +kmod-scsi-core
  KCONFIG:=CONFIG_ATA_PIIX
  FILES:=$(LINUX_DIR)/drivers/ata/ata_piix.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,41,ata_piix)
endef

define KernelPackage/ata-piix/description
 SATA support for Intel ICH5/6/7/8 series host controllers and 
 PATA support for Intel ESB/ICH/PIIX3/PIIX4 series host controllers.
endef

$(eval $(call KernelPackage,ata-piix))


define KernelPackage/ide-core
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=IDE (ATA/ATAPI) device support
  KCONFIG:= \
	CONFIG_IDE \
	CONFIG_IDE_GENERIC \
	CONFIG_BLK_DEV_IDE \
	CONFIG_IDE_GENERIC \
	CONFIG_BLK_DEV_IDEDISK \
	CONFIG_BLK_DEV_IDEDMA_PCI=y \
	CONFIG_BLK_DEV_IDEPCI=y
  FILES:= \
	$(LINUX_DIR)/drivers/ide/ide-core.$(LINUX_KMOD_SUFFIX) \
	$(LINUX_DIR)/drivers/ide/ide-disk.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,20,ide-core) $(call AutoLoad,40,ide-disk)
endef

define KernelPackage/ide-core/2.4
  FILES+=$(LINUX_DIR)/drivers/ide/ide-detect.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD+=$(call AutoLoad,30,ide-detect)
endef

define KernelPackage/ide-core/2.6
  FILES+=$(LINUX_DIR)/drivers/ide/ide-generic.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD+=$(call AutoLoad,30,ide-generic)
endef

define KernelPackage/ide-core/description
 Kernel support for IDE, useful for usb mass storage devices (e.g. on WL-HDD)
 Includes:
 - ide-core
 - ide-detect
 - ide-disk
endef

$(eval $(call KernelPackage,ide-core))


define KernelPackage/ide-aec62xx
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=Acard AEC62xx IDE driver
  DEPENDS:=@PCI_SUPPORT +kmod-ide-core
  KCONFIG:=CONFIG_BLK_DEV_AEC62XX 
  FILES:=$(LINUX_DIR)/drivers/ide/pci/aec62xx.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,30,aec62xx)
endef

define KernelPackage/ide-aec62xx/description
 Support for Acard AEC62xx (Artop ATP8xx) IDE controllers.
endef

$(eval $(call KernelPackage,ide-aec62xx))


define KernelPackage/ide-pdc202xx
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=Promise PDC202xx IDE driver
  DEPENDS:=@LINUX_2_4 +kmod-ide-core
  KCONFIG:=CONFIG_BLK_DEV_PDC202XX_OLD
  FILES:=$(LINUX_DIR)/drivers/ide/pci/pdc202xx_old.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,30,pdc202xx_old)
endef

define KernelPackage/ide-pdc202xx/description
 Support for the Promise Ultra 33/66/100 (PDC202{46|62|65|67|68}) IDE 
 controllers.
endef

$(eval $(call KernelPackage,ide-pdc202xx))


define KernelPackage/scsi-core
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=SCSI device support
  KCONFIG:= \
	CONFIG_SCSI \
	CONFIG_BLK_DEV_SD
  FILES:= \
	$(LINUX_DIR)/drivers/scsi/scsi_mod.$(LINUX_KMOD_SUFFIX) \
	$(LINUX_DIR)/drivers/scsi/sd_mod.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,20,scsi_mod) $(call AutoLoad,40,sd_mod)
endef

$(eval $(call KernelPackage,scsi-core))


define KernelPackage/scsi-generic
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=Kernel support for SCSI generic
  KCONFIG:= \
	CONFIG_CHR_DEV_SG
  FILES:= \
	$(LINUX_DIR)/drivers/scsi/sg.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,65,sg)
endef

$(eval $(call KernelPackage,scsi-generic))


define KernelPackage/loop
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=Loopback device support
  KCONFIG:= \
	CONFIG_BLK_DEV_LOOP \
	CONFIG_BLK_DEV_CRYPTOLOOP=n
  FILES:=$(LINUX_DIR)/drivers/block/loop.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,30,loop)
endef

define KernelPackage/loop/description
 Kernel module for loopback device support
endef

$(eval $(call KernelPackage,loop))


define KernelPackage/nbd
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=Network block device support
  KCONFIG:=CONFIG_BLK_DEV_NBD
  FILES:=$(LINUX_DIR)/drivers/block/nbd.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,30,nbd)
endef

define KernelPackage/nbd/description
 Kernel module for network block device support
endef

$(eval $(call KernelPackage,nbd))


define KernelPackage/rb153-cf
  SUBMENU:=$(BLOCK_MENU)
  TITLE:=RouterBoard 153 CF Slot
  KCONFIG:=CONFIG_BLK_DEV_CF_MIPS
  FILES:=$(LINUX_DIR)/drivers/block/rb1xx/rb153-cf.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,30,rb153-cf)
endef

define KernelPackage/rb153-cf/description
  Kernel module for the RouterBoard 153 CF slot
endef

$(eval $(call KernelPackage,rb153-cf))
