/*
 * Copyright (C) Citrix Systems Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2.1 only
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "libvhd.h"

int
vhd_util_create(int argc, char **argv)
{
	char *name;
	uint64_t size, msize;
	int c, sparse, err;
	vhd_flag_creat_t flags;
#ifdef XS_VHD
	uint8_t encrypt_method;
#endif

	err       = -EINVAL;
	size      = 0;
	msize     = 0;
	sparse    = 1;
	name      = NULL;
	flags     = 0;
#ifdef XS_VHD
	encrypt_method	= 0;
#endif

	if (!argc || !argv)
		goto usage;

	optind = 0;
#ifndef XS_VHD
	while ((c = getopt(argc, argv, "n:s:S:rh")) != -1) {
#else
	while ((c = getopt(argc, argv, "n:s:S:E:rh")) != -1) {
#endif
		switch (c) {
		case 'n':
			name = optarg;
			break;
		case 's':
			err  = 0;
			size = strtoull(optarg, NULL, 10);
			break;
		case 'S':
			err = 0;
			msize = strtoull(optarg, NULL, 10);
			break;
		case 'r':
			sparse = 0;
			break;
#ifdef XS_VHD
		case 'E':
			encrypt_method = atoi(optarg);
			break;
#endif
		case 'h':
		default:
			goto usage;
		}
	}

	if (err || !name || optind != argc)
		goto usage;

	if (msize && msize < size) {
		printf("Error: <-S size> must be greater than <-s size>\n");
		return -EINVAL;
	}

#ifndef XS_VHD
	return vhd_create(name, size << 20,
				  (sparse ? HD_TYPE_DYNAMIC : HD_TYPE_FIXED),
				  msize << 20, flags);
#else
	return vhd_create(name, size << 20,
				  (sparse ? HD_TYPE_DYNAMIC : HD_TYPE_FIXED),
				  msize << 20, flags, encrypt_method);
#endif

usage:
	printf("options: <-n name> <-s size (MB)> [-r reserve] [-h help] "
			"[<-S size (MB) for metadata preallocation "
			"(see vhd-util resize)>]\n");
	return -EINVAL;
}
