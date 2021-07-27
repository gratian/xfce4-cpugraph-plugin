/*  hid.c
 *  Part of xfce4-cpugraph-plugin
 *
 *  Copyright (c) Gratian Crisan <gratian@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <libxfce4ui/libxfce4ui.h>
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <locale.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <syslog.h>

#include "hid.h"

#ifndef _
# include <libintl.h>
# define _(String) gettext (String)
#endif

int print_hidraw_dev(struct udev_device *dev)
{
	struct udev_device *parent_dev;

	if (!dev)
		return -1;

	printf("Device Node Path: %s\n", udev_device_get_devnode(dev));

	parent_dev = udev_device_get_parent_with_subsystem_devtype(dev,
								"usb", "usb_device");
	if (!parent_dev)
		return -1;

	printf("  VID/PID: %s %s\n",
		udev_device_get_sysattr_value(parent_dev,"idVendor"),
		udev_device_get_sysattr_value(parent_dev, "idProduct"));
	printf("  %s\n  %s\n",
		udev_device_get_sysattr_value(parent_dev,"manufacturer"),
		udev_device_get_sysattr_value(parent_dev,"product"));
	printf("  serial: %s\n",
		udev_device_get_sysattr_value(parent_dev, "serial"));

	return 0;
}

static bool match_vid_pid(struct udev_device *dev, long vid, long pid)
{
	struct udev_device *parent_dev;
	long _vid;
	long _pid;

	if (!dev)
		return false;

	parent_dev = udev_device_get_parent_with_subsystem_devtype(dev,
								"usb",
								"usb_device");
	if (!parent_dev)
		return false;
	_vid = strtol(udev_device_get_sysattr_value(parent_dev,"idVendor"), NULL, 16);
	_pid = strtol(udev_device_get_sysattr_value(parent_dev, "idProduct"), NULL, 16);

	return (vid == _vid && pid == _pid);
}

struct udev_device* find_hidraw_dev(long vid, long pid)
{
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;
	const char *path;
	bool found = false;

	udev = udev_new();
	if (!udev) {
		printf("[error] Can't create udev\n");
		return NULL;
	}

	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "hidraw");
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);

	udev_list_entry_foreach(dev_list_entry, devices) {
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);

		found = match_vid_pid(dev, vid, pid);
		if (found)
			break;
	}

	udev_enumerate_unref(enumerate);
	udev_unref(udev);

	return (found ? dev : NULL);
}

int write_hidraw_dev(struct udev_device *dev, uint8_t* data, uint8_t len)
{
	int fd;
	int ret;

	if (!dev)
		return -1;

	fd = open(udev_device_get_devnode(dev), O_RDWR|O_NONBLOCK);
	if (fd < 0) {
		perror("Unable to open device");
		return fd;
	}

	/* Send a Report to the Device */
	ret = write(fd, data, len);
	if (ret < 0) {
		perror("write() to hidraw dev");
	}
	close(fd);

	return ret;
}
