#!/usr/bin/env python3
#
# Copyright 2020-present Facebook. All Rights Reserved.
#
# This program file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program in a file named COPYING; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA
#
import unittest

from common.base_kv_test import BaseKvTest


class KvTest(BaseKvTest, unittest.TestCase):
    def set_kv_keys(self):
        self.kv_keys = [
            "pim1_sensor_health",
            "pim2_sensor_health",
            "pim3_sensor_health",
            "pim4_sensor_health",
            "pim5_sensor_health",
            "pim6_sensor_health",
            "pim7_sensor_health",
            "pim8_sensor_health",
            "psu1_sensor_health",
            "psu2_sensor_health",
            "psu3_sensor_health",
            "psu4_sensor_health",
            "pwr_server_last_state",
            "scm_sensor_health",
            "server_boot_order",
            "server_cpu_ppin",
            "server_por_cfg",
            "server_restart_cause",
            "server_sel_error",
            "smb_sensor_health",
            "sysfw_ver_server",
            "timestamp_sled",
        ]