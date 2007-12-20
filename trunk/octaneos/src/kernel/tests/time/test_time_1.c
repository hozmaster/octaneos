/*
 * Copyright (C) 2003, 2007 Berlin Brown (berlin.brown@gmail.com)
 *
 * File: block_devices.c (see ll_rw_block in linux kernel)
 *
 * Octane OS (Operating System)
 * Copyright (C) 2007 Berlin Brown
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See LICENSE.OCTANE for more details
 */
#include <system/system.h>
#include <linux/time.h>
#include <linux/sched.h>

static void do_simple_request(unsigned long nr) {
	printk("##\n");
}

static struct timer_list simple_timer[2] = {
	{ NULL, NULL, 0, 0, do_simple_request },
	{ NULL, NULL, 0, 0, do_simple_request }
};

void test_time_1() {
	int i = 0;
	printk("*** Running time test 1\n");
	del_timer(simple_timer + 0);

	for (i = 0; i < 2; i++) {
		simple_timer[i].expires = 10000;
		add_timer(simple_timer + i);
	}
}
