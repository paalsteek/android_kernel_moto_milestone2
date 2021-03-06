/*
 * OMAP Power Management debug routines
 *
 * Copyright (C) 2005 Texas Instruments, Inc.
 * Copyright (C) 2006-2008 Nokia Corporation
 *
 * Written by:
 * Richard Woodruff <r-woodruff2@ti.com>
 * Tony Lindgren
 * Juha Yrjola
 * Amit Kucheria <amit.kucheria@nokia.com>
 * Igor Stoppa <igor.stoppa@nokia.com>
 * Jouni Hogander
 *
 * Based on pm.c for omap2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>

#include <plat/clock.h>
#include <plat/board.h>
#include <plat/powerdomain.h>
#include <plat/clockdomain.h>

#include "prm.h"
#include "cm.h"
#include "pm.h"
#include "smartreflex.h"
#include "prm-regbits-34xx.h"

int omap2_pm_debug;

#define DUMP_PRM_MOD_REG(mod, reg)    \
	regs[reg_count].name = #mod "." #reg; \
	regs[reg_count++].val = prm_read_mod_reg(mod, reg)
#define DUMP_CM_MOD_REG(mod, reg)     \
	regs[reg_count].name = #mod "." #reg; \
	regs[reg_count++].val = cm_read_mod_reg(mod, reg)
#define DUMP_PRM_REG(reg) \
	regs[reg_count].name = #reg; \
	regs[reg_count++].val = __raw_readl(reg)
#define DUMP_CM_REG(reg) \
	regs[reg_count].name = #reg; \
	regs[reg_count++].val = __raw_readl(reg)
#define DUMP_INTC_REG(reg, off) \
	regs[reg_count].name = #reg; \
	regs[reg_count++].val = \
			 __raw_readl(OMAP2_L4_IO_ADDRESS(0x480fe000 + (off)))


void omap2_pm_dump(int mode, int resume, unsigned int us)
{
	struct reg {
		const char *name;
		u32 val;
	} regs[32];
	int reg_count = 0, i;
	const char *s1 = NULL, *s2 = NULL;

	if (!resume) {
#if 0
		/* MPU */
		DUMP_PRM_MOD_REG(OCP_MOD, OMAP2_PRM_IRQENABLE_MPU_OFFSET);
		DUMP_CM_MOD_REG(MPU_MOD, CM_CLKSTCTRL);
		DUMP_PRM_MOD_REG(MPU_MOD, PM_PWSTCTRL);
		DUMP_PRM_MOD_REG(MPU_MOD, PM_PWSTST);
		DUMP_PRM_MOD_REG(MPU_MOD, PM_WKDEP);
#endif
#if 0
		/* INTC */
		DUMP_INTC_REG(INTC_MIR0, 0x0084);
		DUMP_INTC_REG(INTC_MIR1, 0x00a4);
		DUMP_INTC_REG(INTC_MIR2, 0x00c4);
#endif
#if 0
		DUMP_CM_MOD_REG(CORE_MOD, CM_FCLKEN1);
		if (cpu_is_omap24xx()) {
			DUMP_CM_MOD_REG(CORE_MOD, OMAP24XX_CM_FCLKEN2);
			DUMP_PRM_MOD_REG(OMAP24XX_GR_MOD,
					OMAP2_PRCM_CLKEMUL_CTRL_OFFSET);
			DUMP_PRM_MOD_REG(OMAP24XX_GR_MOD,
					OMAP2_PRCM_CLKSRC_CTRL_OFFSET);
		}
		DUMP_CM_MOD_REG(WKUP_MOD, CM_FCLKEN);
		DUMP_CM_MOD_REG(CORE_MOD, CM_ICLKEN1);
		DUMP_CM_MOD_REG(CORE_MOD, CM_ICLKEN2);
		DUMP_CM_MOD_REG(WKUP_MOD, CM_ICLKEN);
		DUMP_CM_MOD_REG(PLL_MOD, CM_CLKEN);
		DUMP_CM_MOD_REG(PLL_MOD, CM_AUTOIDLE);
		DUMP_PRM_MOD_REG(CORE_MOD, PM_PWSTST);
#endif
#if 0
		/* DSP */
		if (cpu_is_omap24xx()) {
			DUMP_CM_MOD_REG(OMAP24XX_DSP_MOD, CM_FCLKEN);
			DUMP_CM_MOD_REG(OMAP24XX_DSP_MOD, CM_ICLKEN);
			DUMP_CM_MOD_REG(OMAP24XX_DSP_MOD, CM_IDLEST);
			DUMP_CM_MOD_REG(OMAP24XX_DSP_MOD, CM_AUTOIDLE);
			DUMP_CM_MOD_REG(OMAP24XX_DSP_MOD, CM_CLKSEL);
			DUMP_CM_MOD_REG(OMAP24XX_DSP_MOD, CM_CLKSTCTRL);
			DUMP_PRM_MOD_REG(OMAP24XX_DSP_MOD, RM_RSTCTRL);
			DUMP_PRM_MOD_REG(OMAP24XX_DSP_MOD, RM_RSTST);
			DUMP_PRM_MOD_REG(OMAP24XX_DSP_MOD, PM_PWSTCTRL);
			DUMP_PRM_MOD_REG(OMAP24XX_DSP_MOD, PM_PWSTST);
		}
#endif
	} else {
		DUMP_PRM_MOD_REG(CORE_MOD, PM_WKST1);
		if (cpu_is_omap24xx())
			DUMP_PRM_MOD_REG(CORE_MOD, OMAP24XX_PM_WKST2);
		DUMP_PRM_MOD_REG(WKUP_MOD, PM_WKST);
		DUMP_PRM_MOD_REG(OCP_MOD, OMAP2_PRCM_IRQSTATUS_MPU_OFFSET);
#if 1
		DUMP_INTC_REG(INTC_PENDING_IRQ0, 0x0098);
		DUMP_INTC_REG(INTC_PENDING_IRQ1, 0x00b8);
		DUMP_INTC_REG(INTC_PENDING_IRQ2, 0x00d8);
#endif
	}

	switch (mode) {
	case 0:
		s1 = "full";
		s2 = "retention";
		break;
	case 1:
		s1 = "MPU";
		s2 = "retention";
		break;
	case 2:
		s1 = "MPU";
		s2 = "idle";
		break;
	}

	if (!resume)
#ifdef CONFIG_NO_HZ
		printk(KERN_INFO
		       "--- Going to %s %s (next timer after %u ms)\n", s1, s2,
		       jiffies_to_msecs(get_next_timer_interrupt(jiffies) -
					jiffies));
#else
		printk(KERN_INFO "--- Going to %s %s\n", s1, s2);
#endif
	else
		printk(KERN_INFO "--- Woke up (slept for %u.%03u ms)\n",
			us / 1000, us % 1000);

	for (i = 0; i < reg_count; i++)
		printk(KERN_INFO "%-20s: 0x%08x\n", regs[i].name, regs[i].val);
}

#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#include <linux/seq_file.h>

static int __init pm_dbg_init(void);

static void pm_dbg_regset_store(u32 *ptr);

struct dentry *pm_dbg_dir;

static int pm_dbg_init_done;

enum {
	DEBUG_FILE_COUNTERS = 0,
	DEBUG_FILE_TIMERS,
};

struct pm_module_def {
	char name[8]; /* Name of the module */
	short type; /* CM or PRM */
	unsigned short offset;
	int low; /* First register address on this module */
	int high; /* Last register address on this module */
};

#define MOD_CM 0
#define MOD_PRM 1

static const struct pm_module_def *pm_dbg_reg_modules;
static const struct pm_module_def omap3_pm_reg_modules[] = {
	{ "IVA2", MOD_CM, OMAP3430_IVA2_MOD, 0, 0x4c },
	{ "OCP", MOD_CM, OCP_MOD, 0, 0x10 },
	{ "MPU", MOD_CM, MPU_MOD, 4, 0x4c },
	{ "CORE", MOD_CM, CORE_MOD, 0, 0x4c },
	{ "SGX", MOD_CM, OMAP3430ES2_SGX_MOD, 0, 0x4c },
	{ "WKUP", MOD_CM, WKUP_MOD, 0, 0x40 },
	{ "CCR", MOD_CM, PLL_MOD, 0, 0x70 },
	{ "DSS", MOD_CM, OMAP3430_DSS_MOD, 0, 0x4c },
	{ "CAM", MOD_CM, OMAP3430_CAM_MOD, 0, 0x4c },
	{ "PER", MOD_CM, OMAP3430_PER_MOD, 0, 0x4c },
	{ "EMU", MOD_CM, OMAP3430_EMU_MOD, 0x40, 0x54 },
	{ "NEON", MOD_CM, OMAP3430_NEON_MOD, 0x20, 0x48 },
	{ "USB", MOD_CM, OMAP3430ES2_USBHOST_MOD, 0, 0x4c },

	{ "IVA2", MOD_PRM, OMAP3430_IVA2_MOD, 0x50, 0xfc },
	{ "OCP", MOD_PRM, OCP_MOD, 4, 0x1c },
	{ "MPU", MOD_PRM, MPU_MOD, 0x58, 0xe8 },
	{ "CORE", MOD_PRM, CORE_MOD, 0x58, 0xf8 },
	{ "SGX", MOD_PRM, OMAP3430ES2_SGX_MOD, 0x58, 0xe8 },
	{ "WKUP", MOD_PRM, WKUP_MOD, 0xa0, 0xb0 },
	{ "CCR", MOD_PRM, PLL_MOD, 0x40, 0x70 },
	{ "DSS", MOD_PRM, OMAP3430_DSS_MOD, 0x58, 0xe8 },
	{ "CAM", MOD_PRM, OMAP3430_CAM_MOD, 0x58, 0xe8 },
	{ "PER", MOD_PRM, OMAP3430_PER_MOD, 0x58, 0xe8 },
	{ "EMU", MOD_PRM, OMAP3430_EMU_MOD, 0x58, 0xe4 },
	{ "GLBL", MOD_PRM, OMAP3430_GR_MOD, 0x20, 0xe4 },
	{ "NEON", MOD_PRM, OMAP3430_NEON_MOD, 0x58, 0xe8 },
	{ "USB", MOD_PRM, OMAP3430ES2_USBHOST_MOD, 0x58, 0xe8 },
	{ "", 0, 0, 0, 0 },
};

#define OMAP3_PADCONF_BASE		0x48002000
#define PADCONF_WAKEUP_ENABLE_MASK	(1 << 14)

#define PAD_ADDRESS_SECTION 5
static struct {
	unsigned int start;
	unsigned int end;
} pad_range[PAD_ADDRESS_SECTION] = {
	{ 0x0030, 0x014C },
	{ 0x0158, 0x0264 },
	{ 0x05A0, 0x05F8 },
	{ 0x0A00, 0x0A24 },
	{ 0x0A4C, 0x0A58 },
};

#define PM_DBG_MAX_REG_SETS 4

static void *pm_dbg_reg_set[PM_DBG_MAX_REG_SETS];

static int pm_dbg_get_regset_size(void)
{
	static int regset_size;

	if (regset_size == 0) {
		int i = 0;
		while (pm_dbg_reg_modules[i].name[0] != 0) {
			regset_size += pm_dbg_reg_modules[i].high +
				4 - pm_dbg_reg_modules[i].low;
			i++;
		}
	}
	return regset_size;
}

static int pm_dbg_show_regs(struct seq_file *s, void *unused)
{
	int i, j;
	unsigned long val;
	int reg_set = (int)s->private;
	u32 *ptr;
	void *store = NULL;
	int regs;
	int linefeed;

	if (reg_set == 0) {
		store = kmalloc(pm_dbg_get_regset_size(), GFP_KERNEL);
		if (!store) {
			WARN_ON(1);
			return -ENOMEM;
		}
		ptr = store;
		pm_dbg_regset_store(ptr);
	} else {
		ptr = pm_dbg_reg_set[reg_set - 1];
	}

	i = 0;

	while (pm_dbg_reg_modules[i].name[0] != 0) {
		regs = 0;
		linefeed = 0;
		if (pm_dbg_reg_modules[i].type == MOD_CM)
			seq_printf(s, "MOD: CM_%s (%08x)\n",
				pm_dbg_reg_modules[i].name,
				(u32)(OMAP3430_CM_BASE +
				pm_dbg_reg_modules[i].offset));
		else
			seq_printf(s, "MOD: PRM_%s (%08x)\n",
				pm_dbg_reg_modules[i].name,
				(u32)(OMAP3430_PRM_BASE +
				pm_dbg_reg_modules[i].offset));

		for (j = pm_dbg_reg_modules[i].low;
			j <= pm_dbg_reg_modules[i].high; j += 4) {
			val = *(ptr++);
			if (val != 0) {
				regs++;
				if (linefeed) {
					seq_printf(s, "\n");
					linefeed = 0;
				}
				seq_printf(s, "  %02x => %08lx", j, val);
				if (regs % 4 == 0)
					linefeed = 1;
			}
		}
		seq_printf(s, "\n");
		i++;
	}

	if (store != NULL)
		kfree(store);

	return 0;
}

static void pm_dbg_regset_store(u32 *ptr)
{
	int i, j;
	u32 val;

	i = 0;

	while (pm_dbg_reg_modules[i].name[0] != 0) {
		for (j = pm_dbg_reg_modules[i].low;
			j <= pm_dbg_reg_modules[i].high; j += 4) {
			if (pm_dbg_reg_modules[i].type == MOD_CM)
				val = cm_read_mod_reg(
					pm_dbg_reg_modules[i].offset, j);
			else
				val = prm_read_mod_reg(
					pm_dbg_reg_modules[i].offset, j);
			*(ptr++) = val;
		}
		i++;
	}
}

int pm_dbg_regset_save(int reg_set)
{
	if (pm_dbg_reg_set[reg_set-1] == NULL)
		return -EINVAL;

	pm_dbg_regset_store(pm_dbg_reg_set[reg_set-1]);

	return 0;
}

#define CORE_REGS_LEN 360
void pm_dbg_show_core_regs(void)
{
	int i = 0;
	int j = 0;
	unsigned long val = 0;
	int regs;
	u32 *ptr;
	static char core_buf[CORE_REGS_LEN];
	char *pbuf;

	if (pm_dbg_reg_set[0] == NULL)
		return;
	else
		ptr = pm_dbg_reg_set[0];

	memset(core_buf, 0, CORE_REGS_LEN);

	while (pm_dbg_reg_modules[i].name[0] != 0) {
		if (strcmp(pm_dbg_reg_modules[i].name, "CORE") ||
			pm_dbg_reg_modules[i].type != MOD_CM) {
			ptr += ((pm_dbg_reg_modules[i].high + 4
				- pm_dbg_reg_modules[i].low)/4);
			i++;
			continue;
		}
		/* print the core domain CM registers only.*/
		printk(KERN_INFO "MOD: CM_%s (%08x)\n",
			pm_dbg_reg_modules[i].name,
			(u32)(OMAP3430_CM_BASE +
			pm_dbg_reg_modules[i].offset));

		regs = 0;
		pbuf = core_buf;
		for (j = pm_dbg_reg_modules[i].low;
			j <= pm_dbg_reg_modules[i].high; j += 4) {
			val = *(ptr++);
			if (val && (CORE_REGS_LEN-(pbuf-core_buf)) > 20) {
				pbuf += snprintf(pbuf,
					CORE_REGS_LEN-(pbuf-core_buf),
					"%02x => %08lx ", j, val);
				regs++;
				if (regs % 4 == 0)
					pbuf += snprintf(pbuf,
					CORE_REGS_LEN-(pbuf-core_buf), "\n");
			}
		}
		printk(KERN_INFO "%s\n", core_buf);
		break;
	}
}

#define WAKEUP_SOURCE_LEN 512
void pm_dbg_show_wakeup_source(void)
{
	u32 val = 0;
	int len = 0;
	static char buf[WAKEUP_SOURCE_LEN];
	char *pbuf;
	u32 gpio_bit = 0;

	/* print the real wkup sources */
	memset(buf, 0, WAKEUP_SOURCE_LEN);
	pbuf = buf;
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (len > 16)
		pbuf += snprintf(pbuf, len, "WAKEDUP BY: ");

	val = prm_read_mod_reg(WKUP_MOD, PM_WKST);
	val &= prm_read_mod_reg(WKUP_MOD, OMAP3430_PM_MPUGRPSEL);
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (val && len > 30)
		pbuf += snprintf(pbuf, len, "WKUP_MOD(0x%x), ", val);

	val = prm_read_mod_reg(CORE_MOD, PM_WKST1);
	val &= prm_read_mod_reg(CORE_MOD, OMAP3430_PM_MPUGRPSEL);
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (val && len > 30)
		pbuf += snprintf(pbuf, len, "CORE_MOD(0x%x), ", val);

	val = prm_read_mod_reg(CORE_MOD, OMAP3430ES2_PM_WKST3);
	val &= prm_read_mod_reg(CORE_MOD, OMAP3430ES2_PM_MPUGRPSEL3);
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (val && len > 30)
		pbuf += snprintf(pbuf, len, "CORE3_MOD(0x%x), ", val);

	val = prm_read_mod_reg(OMAP3430_PER_MOD, PM_WKST);
	val &= prm_read_mod_reg(OMAP3430_PER_MOD, OMAP3430_PM_MPUGRPSEL);
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (val && len > 30)
		pbuf += snprintf(pbuf, len, "PER_MOD(0x%x), ", val);

	val = prm_read_mod_reg(OMAP3430ES2_USBHOST_MOD, PM_WKST);
	val &= prm_read_mod_reg(OMAP3430ES2_USBHOST_MOD, OMAP3430_PM_MPUGRPSEL);
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (val && len > 30)
		pbuf += snprintf(pbuf, len, "USBHOST(0x%x), ", val);

	val = prm_read_mod_reg(OCP_MOD, OMAP3_PRM_IRQSTATUS_MPU_OFFSET);
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (val && len > 30)
		pbuf += snprintf(pbuf, len, "MPU_IRQSTATUS(0x%x), ", val);

	val = __raw_readl(OMAP2_L4_IO_ADDRESS(OMAP34XX_IC_BASE + (0x0098)));
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (val && len > 30)
		pbuf += snprintf(pbuf, len, "INTC_IRQ0(0x%x), ", val);

	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if ((val & (1<<29)) && len > 20) {
		gpio_bit = __raw_readl(OMAP2_L4_IO_ADDRESS(0x48310018)) &
			__raw_readl(OMAP2_L4_IO_ADDRESS(0x4831001C));
		pbuf += snprintf(pbuf, len, "GPIO1(0x%x), ", gpio_bit);
	}
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if ((val & (1<<30)) && len > 20) {
		gpio_bit = __raw_readl(OMAP2_L4_IO_ADDRESS(0x49050018)) &
			__raw_readl(OMAP2_L4_IO_ADDRESS(0x4905001C));
		pbuf += snprintf(pbuf, len, "GPIO2(0x%x), ", gpio_bit);
	}
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if ((val & (1<<31)) && len > 20) {
		gpio_bit = __raw_readl(OMAP2_L4_IO_ADDRESS(0x49052018)) &
			__raw_readl(OMAP2_L4_IO_ADDRESS(0x4905201C));
		pbuf += snprintf(pbuf, len, "GPIO3(0x%x), ", gpio_bit);
	}

	val = __raw_readl(OMAP2_L4_IO_ADDRESS(OMAP34XX_IC_BASE + (0x00b8)));
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (val && len > 30)
		pbuf += snprintf(pbuf, len, "INTC_IRQ1(0x%x), ", val);

	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if ((val & (1<<0)) && len > 20) {
		gpio_bit = __raw_readl(OMAP2_L4_IO_ADDRESS(0x49054018)) &
			__raw_readl(OMAP2_L4_IO_ADDRESS(0x4905401C));
		pbuf += snprintf(pbuf, len, "GPIO4(0x%x), ", gpio_bit);
	}
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if ((val & (1<<1)) && len > 20) {
		gpio_bit = __raw_readl(OMAP2_L4_IO_ADDRESS(0x49056018)) &
			__raw_readl(OMAP2_L4_IO_ADDRESS(0x4905601C));
		pbuf += snprintf(pbuf, len, "GPIO5(0x%x), ", gpio_bit);
	}
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if ((val & (1<<2)) && len > 20) {
		gpio_bit = __raw_readl(OMAP2_L4_IO_ADDRESS(0x49058018)) &
			__raw_readl(OMAP2_L4_IO_ADDRESS(0x4905801C));
		pbuf += snprintf(pbuf, len, "GPIO6(0x%x), ", gpio_bit);
	}

	val = __raw_readl(OMAP2_L4_IO_ADDRESS(OMAP34XX_IC_BASE + (0x00d8)));
	len = WAKEUP_SOURCE_LEN - (pbuf - buf);
	if (val && len > 30)
		pbuf += snprintf(pbuf, len, "INTC_IRQ2(0x%x)", val);

	printk(KERN_INFO "%s\n", buf);
}

static int pm_dbg_show_padconf_regs(struct seq_file *s, void *unused)
{
	unsigned int i, j;
	int counts;
	unsigned short val;

	for (i = 0; i < PAD_ADDRESS_SECTION; i++) {
		seq_printf(s, "padconf[ %04x --> %04x]",
				pad_range[i].start, pad_range[i].end);
		counts = 0;
		for (j = pad_range[i].start; j <= pad_range[i].end; j += 4) {
			if (counts++ % 4 == 0)
				seq_printf(s, "\n%04X:  ", j);

			/* read the low 16 bits */
			val = omap_readw(OMAP3_PADCONF_BASE + j);
			seq_printf(s, " %04x%s", val,
				(val & PADCONF_WAKEUP_ENABLE_MASK) ? "*" : " ");

			/* read the high 16 bits */
			val = omap_readw(OMAP3_PADCONF_BASE + j + 2);
			seq_printf(s, "	%04x%s", val,
				(val & PADCONF_WAKEUP_ENABLE_MASK) ? "*" : " ");
		}
		seq_printf(s, "\n");
	}

	return 0;
}

static const char pwrdm_state_names[][PWRDM_MAX_PWRSTS] = {
	"OFF",
	"RET",
	"INA",
	"ON"
};

void pm_dbg_update_time(struct powerdomain *pwrdm, int prev)
{
	s64 t;

	if (!pm_dbg_init_done)
		return ;

	/* Update timer for previous state */
	t = sched_clock();

	pwrdm->state_timer[prev] += t - pwrdm->timer;

	pwrdm->timer = t;
}

static int clkdm_dbg_show_counter(struct clockdomain *clkdm, void *user)
{
	struct seq_file *s = (struct seq_file *)user;

	if (strcmp(clkdm->name, "emu_clkdm") == 0 ||
		strcmp(clkdm->name, "wkup_clkdm") == 0 ||
		strncmp(clkdm->name, "dpll", 4) == 0)
		return 0;

	seq_printf(s, "%s->%s (%d)", clkdm->name,
			clkdm->pwrdm.ptr->name,
			atomic_read(&clkdm->usecount));
	seq_printf(s, "\n");

	return 0;
}

static int pwrdm_dbg_show_counter(struct powerdomain *pwrdm, void *user)
{
	struct seq_file *s = (struct seq_file *)user;
	int i;

	if (strcmp(pwrdm->name, "emu_pwrdm") == 0 ||
		strcmp(pwrdm->name, "wkup_pwrdm") == 0 ||
		strncmp(pwrdm->name, "dpll", 4) == 0)
		return 0;

	if (pwrdm->state != pwrdm_read_pwrst(pwrdm))
		printk(KERN_ERR "pwrdm state mismatch(%s) %d != %d\n",
			pwrdm->name, pwrdm->state, pwrdm_read_pwrst(pwrdm));

	seq_printf(s, "%s (%s)", pwrdm->name,
			pwrdm_state_names[pwrdm->state]);
	for (i = 0; i < PWRDM_MAX_PWRSTS; i++)
		seq_printf(s, ",%s:%d", pwrdm_state_names[i],
			pwrdm->state_counter[i]);

	seq_printf(s, ",RET-LOGIC-OFF:%d", pwrdm->ret_logic_off_counter);
	for (i = 0; i < pwrdm->banks; i++)
		seq_printf(s, ",RET-MEMBANK%d-OFF:%d", i + 1,
				pwrdm->ret_mem_off_counter[i]);

	seq_printf(s, "\n");

	return 0;
}

static int pwrdm_dbg_show_timer(struct powerdomain *pwrdm, void *user)
{
	struct seq_file *s = (struct seq_file *)user;
	int i;

	if (strcmp(pwrdm->name, "emu_pwrdm") == 0 ||
		strcmp(pwrdm->name, "wkup_pwrdm") == 0 ||
		strncmp(pwrdm->name, "dpll", 4) == 0)
		return 0;

	pwrdm_state_switch(pwrdm);

	seq_printf(s, "%s (%s)", pwrdm->name,
		pwrdm_state_names[pwrdm->state]);

	for (i = 0; i < 4; i++)
		seq_printf(s, ",%s:%lld", pwrdm_state_names[i],
			pwrdm->state_timer[i]);

	seq_printf(s, "\n");
	return 0;
}

static int pm_dbg_show_counters(struct seq_file *s, void *unused)
{
	pwrdm_for_each(pwrdm_dbg_show_counter, s);
	clkdm_for_each(clkdm_dbg_show_counter, s);

	return 0;
}

static int pm_dbg_show_timers(struct seq_file *s, void *unused)
{
	pwrdm_for_each(pwrdm_dbg_show_timer, s);
	return 0;
}

static int pm_dbg_open(struct inode *inode, struct file *file)
{
	switch ((int)inode->i_private) {
	case DEBUG_FILE_COUNTERS:
		return single_open(file, pm_dbg_show_counters,
			&inode->i_private);
	case DEBUG_FILE_TIMERS:
	default:
		return single_open(file, pm_dbg_show_timers,
			&inode->i_private);
	};
}

static int pm_dbg_reg_open(struct inode *inode, struct file *file)
{
	return single_open(file, pm_dbg_show_regs, inode->i_private);
}

static int pm_dbg_reg_padconf_open(struct inode *inode, struct file *file)
{
	return single_open(file, pm_dbg_show_padconf_regs, inode->i_private);
}

static const struct file_operations debug_fops = {
	.open           = pm_dbg_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};

static const struct file_operations debug_reg_fops = {
	.open           = pm_dbg_reg_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};

static const struct file_operations debug_reg_padconf_fops = {
	.open		= pm_dbg_reg_padconf_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

int pm_dbg_regset_init(int reg_set)
{
	char name[2];

	if (!pm_dbg_init_done)
		pm_dbg_init();

	if (reg_set < 1 || reg_set > PM_DBG_MAX_REG_SETS ||
		pm_dbg_reg_set[reg_set-1] != NULL)
		return -EINVAL;

	pm_dbg_reg_set[reg_set-1] =
		kmalloc(pm_dbg_get_regset_size(), GFP_KERNEL);

	if (pm_dbg_reg_set[reg_set-1] == NULL)
		return -ENOMEM;

	if (pm_dbg_dir != NULL) {
		sprintf(name, "%d", reg_set);

		(void) debugfs_create_file(name, S_IRUGO,
			pm_dbg_dir, (void *)reg_set, &debug_reg_fops);
	}

	return 0;
}

static int pwrdm_suspend_get(void *data, u64 *val)
{
	*val = omap3_pm_get_suspend_state((struct powerdomain *)data);

	if (*val >= 0)
		return 0;
	return *val;
}

static int pwrdm_suspend_set(void *data, u64 val)
{
	return omap3_pm_set_suspend_state((struct powerdomain *)data, (int)val);
}

DEFINE_SIMPLE_ATTRIBUTE(pwrdm_suspend_fops, pwrdm_suspend_get,
			pwrdm_suspend_set, "%llu\n");

static int __init pwrdms_setup(struct powerdomain *pwrdm, void *dir)
{
	int i;
	s64 t;
	struct dentry *d;

	t = sched_clock();

	for (i = 0; i < 4; i++)
		pwrdm->state_timer[i] = 0;

	pwrdm->timer = t;

	if (strncmp(pwrdm->name, "dpll", 4) == 0)
		return 0;

	d = debugfs_create_dir(pwrdm->name, (struct dentry *)dir);

	(void) debugfs_create_file("suspend", S_IRUGO|S_IWUSR, d,
			(void *)pwrdm, &pwrdm_suspend_fops);

	return 0;
}

static int option_get(void *data, u64 *val)
{
	u32 *option = data;

	*val = *option;

	return 0;
}

static int option_set(void *data, u64 val)
{
	u32 *option = data;

	*option = val;

	if (option == &enable_off_mode)
		omap3_pm_off_mode_enable(val);
	if (option == &voltage_off_while_idle) {
		if (voltage_off_while_idle)
			prm_set_mod_reg_bits(OMAP3430_SEL_OFF, OMAP3430_GR_MOD,
					     OMAP3_PRM_VOLTCTRL_OFFSET);
		else
			prm_clear_mod_reg_bits(OMAP3430_SEL_OFF,
					       OMAP3430_GR_MOD,
					       OMAP3_PRM_VOLTCTRL_OFFSET);
	}

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(pm_dbg_option_fops, option_get, option_set, "%llu\n");

static int __init pm_dbg_init(void)
{
	int i;
	struct dentry *d;
	char name[2];

	if (pm_dbg_init_done)
		return 0;

	if (cpu_is_omap34xx())
		pm_dbg_reg_modules = omap3_pm_reg_modules;
	else {
		printk(KERN_ERR "%s: only OMAP3 supported\n", __func__);
		return -ENODEV;
	}

	d = debugfs_create_dir("pm_debug", NULL);
	if (IS_ERR(d))
		return PTR_ERR(d);

	(void) debugfs_create_file("count", S_IRUGO,
		d, (void *)DEBUG_FILE_COUNTERS, &debug_fops);
	(void) debugfs_create_file("time", S_IRUGO,
		d, (void *)DEBUG_FILE_TIMERS, &debug_fops);

	pwrdm_for_each_nolock(pwrdms_setup, (void *)d);

	pm_dbg_dir = debugfs_create_dir("registers", d);
	if (IS_ERR(pm_dbg_dir))
		return PTR_ERR(pm_dbg_dir);

	(void) debugfs_create_file("current", S_IRUGO,
		pm_dbg_dir, (void *)0, &debug_reg_fops);

	for (i = 0; i < PM_DBG_MAX_REG_SETS; i++)
		if (pm_dbg_reg_set[i] != NULL) {
			sprintf(name, "%d", i+1);
			(void) debugfs_create_file(name, S_IRUGO,
				pm_dbg_dir, (void *)(i+1), &debug_reg_fops);

		}

	(void) debugfs_create_file("padconf", S_IRUGO,
		pm_dbg_dir, NULL, &debug_reg_padconf_fops);

	(void) debugfs_create_file("enable_off_mode", S_IRUGO | S_IWUGO, d,
				   &enable_off_mode, &pm_dbg_option_fops);
	(void) debugfs_create_file("sleep_while_idle", S_IRUGO | S_IWUGO, d,
				   &sleep_while_idle, &pm_dbg_option_fops);
	(void) debugfs_create_file("wakeup_timer_seconds", S_IRUGO | S_IWUGO, d,
				   &wakeup_timer_seconds, &pm_dbg_option_fops);

	if (cpu_is_omap3630())
		(void) debugfs_create_file("enable_abb_mode", S_IRUGO | S_IWUGO,
				d, &enable_abb_mode, &pm_dbg_option_fops);

	/* Only enable for >= ES2.1 . Going to 0V on anything under
	 * ES2.1 will eventually cause a crash */
	if (omap_rev() > OMAP3430_REV_ES2_0)
		(void) debugfs_create_file("voltage_off_while_idle",
					   S_IRUGO | S_IWUGO, d,
					   &voltage_off_while_idle,
					   &pm_dbg_option_fops);

	(void)sr_debugfs_create_entries(d);

	pm_dbg_init_done = 1;

	return 0;
}
arch_initcall(pm_dbg_init);

#endif
