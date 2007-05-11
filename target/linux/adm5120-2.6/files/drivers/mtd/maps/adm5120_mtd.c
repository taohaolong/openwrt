/*
 *  Copyright (C) 2006 Felix Fietkau <nbd@openwrt.org>
 *  Copyright (C) 2005 Waldemar Brodkorb <wbx@openwrt.org>
 *  Copyright (C) 2004 Florian Schirmer (jolt@tuxbox.org)
 *
 *  original functions for finding root filesystem from Mike Baker
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Copyright 2001-2003, Broadcom Corporation
 *  All Rights Reserved.
 *
 *  THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 *  KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 *  SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 *  FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 *  Flash mapping for ADM5120 boards
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#ifdef CONFIG_MTD_PARTITIONS
#include <linux/mtd/partitions.h>
#endif
#include <linux/squashfs_fs.h>
#include <linux/jffs2.h>
#include <linux/crc32.h>
#include <asm/io.h>
#include <asm/mach-adm5120/myloader.h>
#include <asm/mach-adm5120/adm5120_info.h>

extern int parse_myloader_partitions(struct mtd_info *master,
                        struct mtd_partition **pparts,
                        unsigned long origin);

/* Macros for switching flash bank
   ADM5120 only support 2MB flash address space
   so GPIO5 is used as A20
 */
#define GPIO_IO        ((volatile unsigned long *)0xb20000b8)
#define FLASH_A20_GPIO  5
#define FLASH_BOUNDARY  0x200000


#define TRX_MAGIC	0x30524448	/* "HDR0" */
#define TRX_VERSION	1
#define TRX_MAX_LEN	0x3A0000
#define TRX_NO_HEADER	1		/* Do not write TRX header */
#define TRX_GZ_FILES	0x2     /* Contains up to TRX_MAX_OFFSET individual gzip files */
#define TRX_MAX_OFFSET	3

struct trx_header {
	u32 magic;		/* "HDR0" */
	u32 len;		/* Length of file including header */
	u32 crc32;		/* 32-bit CRC from flag_version to end of file */
	u32 flag_version;	/* 0:15 flags, 16:31 version */
	u32 offsets[TRX_MAX_OFFSET];	/* Offsets of partitions from start of header */
};

#define ROUNDUP(x, y) ((((x)+((y)-1))/(y))*(y))
#define NVRAM_SPACE 0x8000
#define WINDOW_ADDR 0x1fc00000
#define WINDOW_SIZE 0x400000
#define BUSWIDTH 2

static struct mtd_info *adm5120_mtd;

static struct map_info adm5120_map = {
	name: "adm5120 physically mapped flash",
	size: WINDOW_SIZE,
	bankwidth: BUSWIDTH,
	phys: WINDOW_ADDR,
};

#ifdef CONFIG_MTD_PARTITIONS

static struct mtd_partition adm5120_cfe_parts[] = {
	{ name: "cfe",	offset: 0, size: 0, mask_flags: MTD_WRITEABLE, },
	{ name: "linux", offset: 0, size: 0, },
	{ name: "rootfs", offset: 0, size: 0, },
	{ name: "nvram", offset: 0, size: 0, },
	{ name: NULL, },
};


static void flash_switch_bank(unsigned long addr)
{
	unsigned long val;

	/* Set GPIO as output */
	val = *GPIO_IO | (1 << (FLASH_A20_GPIO+16));
	if ( addr & FLASH_BOUNDARY ) {
		val |= 1 << (FLASH_A20_GPIO + 24);
	} else {
		val &= ~(1 << (FLASH_A20_GPIO + 24));
	}
	*GPIO_IO = val;
}

static map_word adm5120_map_read(struct map_info *map, unsigned long ofs)
{
    flash_switch_bank(ofs);
	return inline_map_read(map, ofs);
}

static void adm5120_map_write(struct map_info *map, const map_word datum, unsigned long ofs)
{
    flash_switch_bank(ofs);
	inline_map_write(map, datum, ofs);
}

static void adm5120_map_copy_from(struct map_info *map, void *to, unsigned long from, ssize_t len)
{
	ssize_t tmp;

	if (from < FLASH_BOUNDARY) {
		tmp = (len < (FLASH_BOUNDARY - from)) ? len : (FLASH_BOUNDARY - from);
		flash_switch_bank(0);
		inline_map_copy_from(map, to, from, tmp);
		to = (void *)((char *)to + tmp);
		from += tmp;
		len -= tmp;
	}
	if (len > 0) {
		flash_switch_bank(FLASH_BOUNDARY);
		inline_map_copy_from(map, to, from, len);
	}

}

static int __init
find_cfe_size(struct mtd_info *mtd, size_t size)
{
	struct trx_header *trx;
	unsigned char buf[512];
	int off;
	size_t len;
	int blocksize;

	trx = (struct trx_header *) buf;

	blocksize = mtd->erasesize;
	if (blocksize < 0x10000)
		blocksize = 0x10000;

	for (off = (128*1024); off < size; off += blocksize) {
		memset(buf, 0xe5, sizeof(buf));

		/*
		 * Read into buffer
		 */
		if (mtd->read(mtd, off, sizeof(buf), &len, buf) ||
		    len != sizeof(buf))
			continue;

		/* found a TRX header */
		if (le32_to_cpu(trx->magic) == TRX_MAGIC) {
			goto found;
		}
	}

	printk(KERN_NOTICE
	       "%s: Couldn't find bootloader size\n",
	       mtd->name);
	return -1;

 found:
	printk(KERN_NOTICE "bootloader size: %d\n", off);
	return off;

}

/*
 * Copied from mtdblock.c
 *
 * Cache stuff...
 *
 * Since typical flash erasable sectors are much larger than what Linux's
 * buffer cache can handle, we must implement read-modify-write on flash
 * sectors for each block write requests.  To avoid over-erasing flash sectors
 * and to speed things up, we locally cache a whole flash sector while it is
 * being written to until a different sector is required.
 */

static void erase_callback(struct erase_info *done)
{
	wait_queue_head_t *wait_q = (wait_queue_head_t *)done->priv;
	wake_up(wait_q);
}

static int erase_write (struct mtd_info *mtd, unsigned long pos,
			int len, const char *buf)
{
	struct erase_info erase;
	DECLARE_WAITQUEUE(wait, current);
	wait_queue_head_t wait_q;
	size_t retlen;
	int ret;

	/*
	 * First, let's erase the flash block.
	 */

	init_waitqueue_head(&wait_q);
	erase.mtd = mtd;
	erase.callback = erase_callback;
	erase.addr = pos;
	erase.len = len;
	erase.priv = (u_long)&wait_q;

	set_current_state(TASK_INTERRUPTIBLE);
	add_wait_queue(&wait_q, &wait);

	ret = mtd->erase(mtd, &erase);
	if (ret) {
		set_current_state(TASK_RUNNING);
		remove_wait_queue(&wait_q, &wait);
		printk (KERN_WARNING "erase of region [0x%lx, 0x%x] "
				     "on \"%s\" failed\n",
			pos, len, mtd->name);
		return ret;
	}

	schedule();  /* Wait for erase to finish. */
	remove_wait_queue(&wait_q, &wait);

	/*
	 * Next, writhe data to flash.
	 */

	ret = mtd->write (mtd, pos, len, &retlen, buf);
	if (ret)
		return ret;
	if (retlen != len)
		return -EIO;
	return 0;
}




static int __init
find_root(struct mtd_info *mtd, size_t size, struct mtd_partition *part)
{
	struct trx_header trx, *trx2;
	unsigned char buf[512], *block;
	int off, blocksize;
	u32 i, crc = ~0;
	size_t len;
	struct squashfs_super_block *sb = (struct squashfs_super_block *) buf;

	blocksize = mtd->erasesize;
	if (blocksize < 0x10000)
		blocksize = 0x10000;

	for (off = (128*1024); off < size; off += blocksize) {
		memset(&trx, 0xe5, sizeof(trx));

		/*
		 * Read into buffer
		 */
		if (mtd->read(mtd, off, sizeof(trx), &len, (char *) &trx) ||
		    len != sizeof(trx))
			continue;

		/* found a TRX header */
		if (le32_to_cpu(trx.magic) == TRX_MAGIC) {
			part->offset = le32_to_cpu(trx.offsets[2]) ? :
				le32_to_cpu(trx.offsets[1]);
			part->size = le32_to_cpu(trx.len);

			part->size -= part->offset;
			part->offset += off;

			goto found;
		}
	}

	printk(KERN_NOTICE
	       "%s: Couldn't find root filesystem\n",
	       mtd->name);
	return -1;

 found:
	if (part->size == 0)
		return 0;

	if (mtd->read(mtd, part->offset, sizeof(buf), &len, buf) || len != sizeof(buf))
		return 0;

	/* Move the fs outside of the trx */
	part->size = 0;

	if (trx.len != part->offset + part->size - off) {
		/* Update the trx offsets and length */
		trx.len = part->offset + part->size - off;

		/* Update the trx crc32 */
		for (i = (u32) &(((struct trx_header *)NULL)->flag_version); i <= trx.len; i += sizeof(buf)) {
			if (mtd->read(mtd, off + i, sizeof(buf), &len, buf) || len != sizeof(buf))
				return 0;
			crc = crc32_le(crc, buf, min(sizeof(buf), trx.len - i));
		}
		trx.crc32 = crc;

		/* read first eraseblock from the trx */
		block = kmalloc(mtd->erasesize, GFP_KERNEL);
		trx2 = (struct trx_header *) block;
		if (mtd->read(mtd, off, mtd->erasesize, &len, block) || len != mtd->erasesize) {
			printk("Error accessing the first trx eraseblock\n");
			return 0;
		}

		printk("Updating TRX offsets and length:\n");
		printk("old trx = [0x%08x, 0x%08x, 0x%08x], len=0x%08x crc32=0x%08x\n", trx2->offsets[0], trx2->offsets[1], trx2->offsets[2], trx2->len, trx2->crc32);
		printk("new trx = [0x%08x, 0x%08x, 0x%08x], len=0x%08x crc32=0x%08x\n",   trx.offsets[0],   trx.offsets[1],   trx.offsets[2],   trx.len, trx.crc32);

		/* Write updated trx header to the flash */
		memcpy(block, &trx, sizeof(trx));
		if (mtd->unlock)
			mtd->unlock(mtd, off, mtd->erasesize);
		erase_write(mtd, off, mtd->erasesize, block);
		if (mtd->sync)
			mtd->sync(mtd);
		kfree(block);
		printk("Done\n");
	}

	return part->size;
}

struct mtd_partition * __init
init_mtd_partitions(struct mtd_info *mtd, size_t size)
{
	int cfe_size;

	if ((cfe_size = find_cfe_size(mtd,size)) < 0)
		return NULL;

	/* boot loader */
	adm5120_cfe_parts[0].offset = 0;
	adm5120_cfe_parts[0].size   = cfe_size;

	/* nvram */
	if (cfe_size != 384 * 1024) {
		adm5120_cfe_parts[3].offset = size - ROUNDUP(NVRAM_SPACE, mtd->erasesize);
		adm5120_cfe_parts[3].size   = ROUNDUP(NVRAM_SPACE, mtd->erasesize);
	} else {
		/* nvram (old 128kb config partition on netgear wgt634u) */
		adm5120_cfe_parts[3].offset = adm5120_cfe_parts[0].size;
		adm5120_cfe_parts[3].size   = ROUNDUP(NVRAM_SPACE, mtd->erasesize);
	}

	/* linux (kernel and rootfs) */
	if (cfe_size != 384 * 1024) {
		adm5120_cfe_parts[1].offset = adm5120_cfe_parts[0].size;
		adm5120_cfe_parts[1].size   = adm5120_cfe_parts[3].offset -
			adm5120_cfe_parts[1].offset;
	} else {
		/* do not count the elf loader, which is on one block */
		adm5120_cfe_parts[1].offset = adm5120_cfe_parts[0].size +
			adm5120_cfe_parts[3].size + mtd->erasesize;
		adm5120_cfe_parts[1].size   = size -
			adm5120_cfe_parts[0].size -
			(2*adm5120_cfe_parts[3].size) -
			mtd->erasesize;
	}

	/* find and size rootfs */
	if (find_root(mtd,size,&adm5120_cfe_parts[2])==0) {
		adm5120_cfe_parts[2].size = size - adm5120_cfe_parts[2].offset -
				adm5120_cfe_parts[3].size;
	}

	return adm5120_cfe_parts;
}
#endif

int __init init_adm5120_map(void)
{
	size_t size;
	int ret = 0;
#if defined (CONFIG_MTD_PARTITIONS) || (CONFIG_MTD_MYLOADER_PARTS)
	struct mtd_partition *parts;
	int i, parsed_nr_parts = 0;
#endif
	printk("adm5120 : flash init : 0x%08x 0x%08x\n", WINDOW_ADDR, WINDOW_SIZE);
	adm5120_map.virt = ioremap_nocache(WINDOW_ADDR, WINDOW_SIZE);

	if (!adm5120_map.virt) {
		printk("Failed to ioremap\n");
		return -EIO;
	}
	simple_map_init(&adm5120_map);
	adm5120_map.read = adm5120_map_read;
	adm5120_map.write = adm5120_map_write;
	adm5120_map.copy_from = adm5120_map_copy_from;

	if (!(adm5120_mtd = do_map_probe("cfi_probe", &adm5120_map))) {
		printk("Failed to do_map_probe\n");
		iounmap((void *)adm5120_map.virt);
		return -ENXIO;
	}

	adm5120_mtd->owner = THIS_MODULE;

	size = adm5120_mtd->size;

	printk(KERN_NOTICE "Flash device: 0x%x at 0x%x\n", size, WINDOW_ADDR);

#ifdef CONFIG_MTD_PARTITIONS

	if (adm5120_boot_loader == BOOT_LOADER_CFE)
	{
		printk(KERN_NOTICE "adm5120 : using CFE flash mapping\n");
		parts = init_mtd_partitions(adm5120_mtd, size);

		for (i = 0; parts[i].name; i++);
			ret = add_mtd_partitions(adm5120_mtd, parts, i);

		if (ret) {
			printk(KERN_ERR "Flash: add_mtd_partitions failed\n");
			goto fail;
		}
	}
#endif
#ifdef CONFIG_MTD_MYLOADER_PARTS
	if (adm5120_boot_loader == BOOT_LOADER_MYLOADER)
	{
		printk(KERN_NOTICE "adm5120 : using MyLoader flash mapping\n");
		char *part_type;

		if (parsed_nr_parts == 0) {
			ret = parse_myloader_partitions(adm5120_mtd, &parts, 0);

			if (ret  > 0) {
				part_type ="MyLoader";
				parsed_nr_parts = ret;
			}
		}
		ret = add_mtd_partitions(adm5120_mtd, parts, parsed_nr_parts);

		if (ret) {
			printk(KERN_ERR "Flash: add_mtd_partitions failed\n");
			goto fail;
		}
	}
#endif
	return 0;

 fail:
	if (adm5120_mtd)
		map_destroy(adm5120_mtd);
	if (adm5120_map.virt)
		iounmap((void *)adm5120_map.virt);
	adm5120_map.virt = 0;
	return ret;
}

void __exit cleanup_adm5120_map(void)
{
#ifdef CONFIG_MTD_PARTITIONS
	del_mtd_partitions(adm5120_mtd);
#endif
	map_destroy(adm5120_mtd);
	iounmap((void *)adm5120_map.virt);
}

module_init(init_adm5120_map);
module_exit(cleanup_adm5120_map);
