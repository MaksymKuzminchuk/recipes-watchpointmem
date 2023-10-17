/******************************************************************************
 *
 *   Copyright (C) 2011  Intel Corporation. All rights reserved.
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; version 2 of the License.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *****************************************************************************/

#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_INFO */
#include <linux/init.h>    /* Needed for the macros */
#include <linux/kallsyms.h>

#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>

struct perf_event *__percpu *watchpoint_hbp;
static unsigned long watch_addr = 0x1234;
module_param(watch_addr, ulong, 0660);
MODULE_PARM_DESC(watch_addr, "Set watch point on current address");

static void watchpoint_hbp_handler(struct perf_event *bp,
			       struct perf_sample_data *data,
			       struct pt_regs *regs)
{
    printk(KERN_INFO "Watchpointmem start dump\n");
    dump_stack();
    printk(KERN_INFO "Watchpointmem finish dump\n");
}

static int __init watchpointmem_init(void)
{
    int ret;
    struct perf_event_attr attr;
    hw_breakpoint_init(&attr);
    attr.bp_addr = (unsigned long)watch_addr;
    attr.bp_len = HW_BREAKPOINT_LEN_4;
    attr.bp_type = HW_BREAKPOINT_RW;

    watchpoint_hbp = register_wide_hw_breakpoint(&attr, watchpoint_hbp_handler, NULL);
    if (IS_ERR((void __force *)watchpoint_hbp)) {
	ret = PTR_ERR((void __force *)watchpoint_hbp);
	goto fail;
    }

    printk(KERN_INFO "Watchpoint set on 0x%x\n", watch_addr);

    return 0;

fail:
    printk(KERN_INFO "Watchpoint registration failed\n");

    return ret;
}

static void __exit watchpointmem_exit(void)
{
    unregister_wide_hw_breakpoint(watchpoint_hbp);
    printk(KERN_INFO "Watchpointmem module: Terminating...\n");
}

module_init(watchpointmem_init);
module_exit(watchpointmem_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("M.Kuzminchuk");
