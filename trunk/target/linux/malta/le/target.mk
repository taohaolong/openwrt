ARCH:=mipsel
ARCH_PACKAGES:=malta_mipsel
SUBTARGET:=le
BOARDNAME:=Little Endian
FEATURES:=ramdisk mips16
CFLAGS:=-Os -pipe -mips32r2 -mtune=mips32r2 -fno-caller-saves

define Target/Description
	Build LE firmware images for MIPS Malta CoreLV board running in
	little-endian mode
endef
