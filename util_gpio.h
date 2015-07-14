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
