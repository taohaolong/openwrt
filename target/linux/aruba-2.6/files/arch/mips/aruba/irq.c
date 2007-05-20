#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/timex.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/delay.h>

#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/io.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <asm/idt-boards/rc32434/rc32434.h>
#include <asm/idt-boards/rc32434/rc32434_gpio.h>

#include <asm/irq.h>

extern void aruba_timer_interrupt(struct pt_regs *regs);

typedef struct {
	u32 mask;
	volatile u32 *base_addr;
} intr_group_t;

static const intr_group_t intr_group_merlot[NUM_INTR_GROUPS] = {
	{0x00000000, (u32 *) KSEG1ADDR(IC_GROUP0_PEND + 0)},
};

#define READ_PEND_MERLOT(base) (*((volatile unsigned long *)(0xbc003010)))
#define READ_MASK_MERLOT(base) (*((volatile unsigned long *)(0xbc003014)))
#define WRITE_MASK_MERLOT(base, val) ((*((volatile unsigned long *)(0xbc003014))) = (val), READ_MASK_MERLOT())

static const intr_group_t intr_group_muscat[NUM_INTR_GROUPS] = {
	{0x0000efff, (u32 *) KSEG1ADDR(IC_GROUP0_PEND + 0 * IC_GROUP_OFFSET)},
	{0x00001fff, (u32 *) KSEG1ADDR(IC_GROUP0_PEND + 1 * IC_GROUP_OFFSET)},
	{0x00000007, (u32 *) KSEG1ADDR(IC_GROUP0_PEND + 2 * IC_GROUP_OFFSET)},
	{0x0003ffff, (u32 *) KSEG1ADDR(IC_GROUP0_PEND + 3 * IC_GROUP_OFFSET)},
	{0xffffffff, (u32 *) KSEG1ADDR(IC_GROUP0_PEND + 4 * IC_GROUP_OFFSET)}
};

#define READ_PEND_MUSCAT(base) (*(base))
#define READ_MASK_MUSCAT(base) (*(base + 2))
#define WRITE_MASK_MUSCAT(base, val) (*(base + 2) = (val))

static inline int group_to_ip(unsigned int group)
{
	switch (mips_machtype) {
		case MACH_ARUBA_AP70:
			return group + 2;
		case MACH_ARUBA_AP65:
		case MACH_ARUBA_AP60:
		default:
			return 6;
	}
}

static inline void enable_local_irq(unsigned int irq)
{
	clear_c0_cause(0x100 << irq);
	set_c0_status(0x100 << irq);
	irq_enable_hazard();
}

static inline void disable_local_irq(unsigned int irq)
{
	clear_c0_status(0x100 << irq);
	clear_c0_cause(0x100 << irq);
	irq_disable_hazard();
}

static inline void aruba_irq_enable(unsigned int irq)
{
	unsigned long flags;
	unsigned int  group, intr_bit;
	volatile unsigned int  *addr;

	local_irq_save(flags);

	if (irq < GROUP0_IRQ_BASE) {
		enable_local_irq(irq);
	} else {
		int ip = irq - GROUP0_IRQ_BASE;
		switch (mips_machtype) {
			case MACH_ARUBA_AP70:
				if (irq >= GROUP4_IRQ_BASE)
					idt_gpio->gpioistat &= ~(1 << (irq - GROUP4_IRQ_BASE));

				// irqs are in groups of 32
				// ip is set to the remainder
				group = ip >> 5;
				ip &= 0x1f;

				// bit -> 0 = unmask
				intr_bit = 1 << ip;
				addr = intr_group_muscat[group].base_addr;
				WRITE_MASK_MUSCAT(addr, READ_MASK_MUSCAT(addr) & ~intr_bit);
				break;

			case MACH_ARUBA_AP65:
			case MACH_ARUBA_AP60:
				group = 0;

				// bit -> 1 = unmasked
				intr_bit = 1 << ip;
				addr = intr_group_merlot[group].base_addr;
				WRITE_MASK_MERLOT(addr, READ_MASK_MERLOT(addr) | intr_bit);
				break;
		}
		enable_local_irq(group_to_ip(group));
	}

	back_to_back_c0_hazard();
	local_irq_restore(flags);
}

static void aruba_irq_disable(unsigned int irq)
{
	unsigned long flags;
	unsigned int  group, intr_bit, mask;
	volatile unsigned int  *addr;

	local_irq_save(flags);

	if (irq < GROUP0_IRQ_BASE) {
		disable_local_irq(irq);
	} else {
		int ip = irq - GROUP0_IRQ_BASE;
		switch (mips_machtype) {
			case MACH_ARUBA_AP70:
				idt_gpio->gpioistat &= ~(1 << ip);

				// irqs are in groups of 32
				// ip is set to the remainder
				group = ip >> 5;
				ip &= 0x1f;

				// bit -> 1 = mask
				intr_bit = 1 << ip;
				addr = intr_group_muscat[group].base_addr;

				mask = READ_MASK_MUSCAT(addr);
				mask |= intr_bit;
				WRITE_MASK_MUSCAT(addr, mask);

				if (mask == intr_group_muscat[group].mask) {
					disable_local_irq(group_to_ip(group));
				}
				break;

			case MACH_ARUBA_AP65:
			case MACH_ARUBA_AP60:
				group = 0;

				// bit -> 0 = masked
				intr_bit = 1 << ip;
				addr = intr_group_merlot[group].base_addr;

				mask = READ_MASK_MERLOT(addr);
				mask &= ~intr_bit;
				WRITE_MASK_MERLOT(addr, mask);

				if (mask == intr_group_merlot[group].mask) {
					disable_local_irq(group_to_ip(group));
				}
				break;
		}
	}

	back_to_back_c0_hazard();
	local_irq_restore(flags);
}

static unsigned int aruba_irq_startup(unsigned int irq)
{
	aruba_irq_enable(irq);
	return 0;
}

#define aruba_irq_shutdown aruba_irq_disable

static void aruba_irq_ack(unsigned int irq)
{
	aruba_irq_disable(irq);
}

static void aruba_irq_end(unsigned int irq)
{
	if (!(irq_desc[irq].status & (IRQ_DISABLED | IRQ_INPROGRESS)))
		aruba_irq_enable(irq);
}

static struct hw_interrupt_type aruba_irq_type = {
	.typename	= "ARUBA",
	.startup	= aruba_irq_startup,
	.shutdown	= aruba_irq_shutdown,
	.enable		= aruba_irq_enable,
	.disable	= aruba_irq_disable,
	.ack		= aruba_irq_ack,
	.end		= aruba_irq_end,
};

void __init arch_init_irq(void)
{
	int i;
	printk("Initializing IRQ's: %d out of %d\n", RC32434_NR_IRQS, NR_IRQS);
	memset(irq_desc, 0, sizeof(irq_desc));

	for (i = 0; i < RC32434_NR_IRQS; i++) {
		irq_desc[i].status = IRQ_DISABLED;
		irq_desc[i].action = NULL;
		irq_desc[i].depth = 1;
		irq_desc[i].chip = &aruba_irq_type;
		spin_lock_init(&irq_desc[i].lock);
	}
}

/* Main Interrupt dispatcher */

void plat_irq_dispatch(struct pt_regs *regs)
{
	unsigned int pend, group, ip;
	volatile unsigned int *addr;
	unsigned long cp0_cause = read_c0_cause() & read_c0_status() & CAUSEF_IP;

	if (cp0_cause & CAUSEF_IP7)
		return aruba_timer_interrupt(regs);

	if(cp0_cause == 0) {
		printk("INTERRUPT(S) FIRED WHILE MASKED\n");
#ifdef ARUBA_DEBUG
		// debuging use -- figure out which interrupt(s) fired
		cp0_cause = read_c0_cause() & CAUSEF_IP;
		while (cp0_cause) {
			unsigned long intr_bit;
			unsigned int irq_nr;
			intr_bit = (31 - rc32434_clz(cp0_cause));
			irq_nr = intr_bit - GROUP0_IRQ_BASE;
			printk(" ---> MASKED IRQ %d\n",irq_nr);
			cp0_cause &= ~(1 << intr_bit);
		}
#endif
		return;
	}

	switch (mips_machtype) {
		case MACH_ARUBA_AP70:
			if ((ip = (cp0_cause & 0x7c00))) {
				group = 21 - rc32434_clz(ip);
		
				addr = intr_group_muscat[group].base_addr;
		
				pend = READ_PEND_MUSCAT(addr);
				pend &= ~READ_MASK_MUSCAT(addr); // only unmasked interrupts
				pend = 39 - rc32434_clz(pend);
				do_IRQ(pend + (group << 5));
			}
			break;
		case MACH_ARUBA_AP65:
		case MACH_ARUBA_AP60:
		default:
			if (cp0_cause & 0x4000) { // 1 << (8 +6) == irq 6
				// Misc Interrupt
				group = 0;

				addr = intr_group_merlot[group].base_addr;

				pend = READ_PEND_MERLOT(addr);
				pend &= READ_MASK_MERLOT(addr);	// only unmasked interrupts
				pend = 31 - rc32434_clz(pend);
				do_IRQ(pend + GROUP0_IRQ_BASE);
			}
			if ((ip = (cp0_cause & 0x3c00))) { // irq 2-5
				pend = 31 - rc32434_clz(ip);
				do_IRQ(pend - GROUP0_IRQ_BASE);
			}
			break;
	}
}
