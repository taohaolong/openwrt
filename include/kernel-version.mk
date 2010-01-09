# Use the default kernel version if the Makefile doesn't override it

ifeq ($(KERNEL),2.4)
  LINUX_VERSION?=2.4.37.5
else
  LINUX_VERSION?=2.6.21.7
endif
LINUX_RELEASE?=1

ifeq ($(LINUX_VERSION),2.4.37.5)
  LINUX_KERNEL_MD5SUM:=cb221187422acaf6c63a40c646e5e476
endif
ifeq ($(LINUX_VERSION),2.6.21.7)
  LINUX_KERNEL_MD5SUM:=bc15fad1487336d5dcb0945cd039d8ed
endif
ifeq ($(LINUX_VERSION),2.6.25.20)
  LINUX_KERNEL_MD5SUM:=0da698edccf03e2235abc2830a495114
endif
ifeq ($(LINUX_VERSION),2.6.30.10)
  LINUX_KERNEL_MD5SUM:=eb6be465f914275967a5602cb33662f5
endif
ifeq ($(LINUX_VERSION),2.6.31.11)
  LINUX_KERNEL_MD5SUM:=39e34c9a6439bcd4c088abe631302a99
endif
ifeq ($(LINUX_VERSION),2.6.32.3)
  LINUX_KERNEL_MD5SUM:=730045c2c7f7e6618db3c4d4d7094853
endif

# disable the md5sum check for unknown kernel versions
LINUX_KERNEL_MD5SUM?=x

split_version=$(subst ., ,$(1))
merge_version=$(subst $(space),.,$(1))
KERNEL_BASE=$(firstword $(subst -, ,$(LINUX_VERSION)))
KERNEL=$(call merge_version,$(wordlist 1,2,$(call split_version,$(KERNEL_BASE))))
KERNEL_PATCHVER=$(call merge_version,$(wordlist 1,3,$(call split_version,$(KERNEL_BASE))))

