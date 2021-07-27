/*  hid.h
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
#ifndef _XFCE_HID_H_
#define _XFCE_HID_H_

#include <libudev.h>
#include <stdint.h>

struct udev_device* find_hidraw_dev(long vid, long pid);
int write_hidraw_dev(struct udev_device *dev, uint8_t* data, uint8_t len);

#endif /* !_XFCE_HID_H_ */
