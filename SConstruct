#
# Copyright (C) 2015 Focalcrest, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import os

env = Environment()

# sunxi_gpio
sunxi_gpio_dst = "sunxi_gpio"
sunxi_gpio_srcs = ["util_gpio.c", "sunxi_gpio.c"]
sunxi_gpio_libs = []

env.Program(target=sunxi_gpio_dst, source=sunxi_gpio_srcs, LIBS=sunxi_gpio_libs)