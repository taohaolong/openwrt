# 
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

HOST_BUILD_DIR ?= $(BUILD_DIR_HOST)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
HOST_INSTALL_DIR ?= $(HOST_BUILD_DIR)/host-install

include $(INCLUDE_DIR)/host.mk
include $(INCLUDE_DIR)/unpack.mk
include $(INCLUDE_DIR)/depends.mk

HOST_STAMP_PREPARED=$(HOST_BUILD_DIR)/.prepared$(if $(QUILT)$(DUMP),,$(shell $(call find_md5,${CURDIR} $(PKG_FILE_DEPEND),)))
HOST_STAMP_CONFIGURED:=$(HOST_BUILD_DIR)/.configured
HOST_STAMP_BUILT:=$(HOST_BUILD_DIR)/.built
HOST_STAMP_INSTALLED:=$(STAGING_DIR_HOST)/stamp/.$(PKG_NAME)_installed

override MAKEFLAGS=

include $(INCLUDE_DIR)/download.mk
include $(INCLUDE_DIR)/quilt.mk

Host/Patch:=$(Host/Patch/Default)
ifneq ($(strip $(HOST_UNPACK)),)
  define Host/Prepare/Default
  	$(HOST_UNPACK)
	$(Host/Patch)
  endef
endif

define Host/Prepare
  $(call Host/Prepare/Default)
endef

define Host/Configure/Default
	@(cd $(HOST_BUILD_DIR)/$(3); \
	[ -x configure ] && \
		$(CP) $(SCRIPT_DIR)/config.{guess,sub} $(HOST_BUILD_DIR)/$(3)/ && \
		$(2) \
		CPPFLAGS="$(HOST_CFLAGS)" \
		LDFLAGS="$(HOST_LDFLAGS)" \
		SHELL="$(BASH)" \
		./configure \
		--target=$(GNU_HOST_NAME) \
		--host=$(GNU_HOST_NAME) \
		--build=$(GNU_HOST_NAME) \
		--program-prefix="" \
		--program-suffix="" \
		--prefix=$(STAGING_DIR_HOST) \
		--exec-prefix=$(STAGING_DIR_HOST) \
		--sysconfdir=$(STAGING_DIR_HOST)/etc \
		--localstatedir=$(STAGING_DIR_HOST)/var \
		$(DISABLE_NLS) \
		$(1); \
		true; \
	)
endef

define Host/Configure
  $(call Host/Configure/Default)
endef

define Host/Compile/Default
	$(MAKE) -C $(HOST_BUILD_DIR) $(1)
endef

define Host/Compile
  $(call Host/Compile/Default)
endef

ifneq ($(if $(QUILT),,$(CONFIG_AUTOREBUILD)),)
  define HostHost/Autoclean
    $(call rdep,${CURDIR} $(PKG_FILE_DEPEND),$(HOST_STAMP_PREPARED))
    $(if $(if $(Host/Compile),$(filter prepare,$(MAKECMDGOALS)),1),,$(call rdep,$(HOST_BUILD_DIR),$(HOST_STAMP_BUILT)))
  endef
endif

define Download/default
  FILE:=$(PKG_SOURCE)
  URL:=$(PKG_SOURCE_URL)
  PROTO:=$(PKG_SOURCE_PROTO)
  SUBDIR:=$(PKG_SOURCE_SUBDIR)
  VERSION:=$(PKG_SOURCE_VERSION)
  MD5SUM:=$(PKG_MD5SUM)
endef

ifndef DUMP
  define HostBuild
  $(if $(QUILT),$(Host/Quilt))
  $(if $(STAMP_PREPARED),,$(if $(strip $(PKG_SOURCE_URL)),$(call Download,default)))
  $(if $(DUMP),,$(call HostHost/Autoclean))
  
  $(HOST_STAMP_PREPARED):
	@-rm -rf $(HOST_BUILD_DIR)
	@mkdir -p $(HOST_BUILD_DIR)
	$(call Host/Prepare)
	touch $$@

  $(HOST_STAMP_CONFIGURED): $(HOST_STAMP_PREPARED)
	$(call Host/Configure)
	touch $$@

  $(HOST_STAMP_BUILT): $(HOST_STAMP_CONFIGURED)
	$(call Host/Compile)
	touch $$@

  $(HOST_STAMP_INSTALLED): $(HOST_STAMP_BUILT)
	$(call Host/Install)
	mkdir -p $$(shell dirname $$@)
	touch $$@
	
  ifdef Host/Install
    host-install: $(HOST_STAMP_INSTALLED)
  endif

  download:
  host-prepare: $(HOST_STAMP_PREPARED)
  host-configure: $(HOST_STAMP_CONFIGURED)
  host-compile: $(HOST_STAMP_BUILT)
  host-install:
  host-clean: FORCE
	$(call Host/Clean)
	$(call Host/Uninstall)
	rm -rf $(HOST_BUILD_DIR) $(HOST_STAMP_INSTALLED) $(HOST_STAMP_BUILT)

  endef

  prepare: host-prepare
  compile: host-compile
  install: host-install
  clean: host-clean

endif

