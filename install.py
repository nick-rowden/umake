#!/usr/bin/python3

# This file is part of the hid_bootloader_console_client distribution
# (https: // github.com/nimo-labs/hid_bootloader_console_client).
# Copyright(c) 2021 Nimolabs Ltd. www.nimo.uk
#
# This program is free software: you can redistribute it and / or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see < http: // www.gnu.org/licenses/>.

import platform
import os

osType = platform.system()

if 'Linux' == osType:
    os.system('cp -f ./umake.py ~/bin/umake')
else:
    print("Sorry, your operating system isn't supported just yet")
    exit(1)
