/*
 * Copyright (C) 2015 Focalcrest, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UTIL_GPIO_H
#define UTIL_GPIO_H

int sunxi_gpio_init();
int sunxi_gpio_export(char io[16]);
int sunxi_gpio_unexport(char io[16]);
int sunxi_gpio_enable_out(char io[16]);
int sunxi_gpio_enable_in(char io[16]);
int sunxi_gpio_set_out(char io[16], int bon);
int sunxi_gpio_get_in(char io[16]);
int sunxi_gpio_fini();

#endif // UTIL_GPIO_H
