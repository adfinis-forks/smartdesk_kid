/*
 * GPIO user space helpers
 *
 * Copyright 2009 Analog Devices Inc.
 * Michael Hennerich (hennerich@blackfin.uclinux.org)
 *
 * Licensed under the GPL-2 or later
 */

#ifndef GPIO_H
#define GPIO_H
#ifdef __cplusplus
extern "C" {
#endif
int gpio_export(unsigned int gpio);  //export pin to user space
int gpio_unexport(unsigned int gpio); //unexport pin from user space
int gpio_dir_out(unsigned int gpio); //Config pin as output
int gpio_dir_in(unsigned int gpio); //Config pin as input
int gpio_set_value(unsigned int gpio, unsigned int value); //Ghi gia tri cua mot pin gpio output
int gpio_get_value(unsigned int gpio, unsigned int *value); //Doc gia tri cua mot pin gpio input

int gpio_set_edge(unsigned int gpio, char *edge);
#ifdef __cplusplus
}
#endif

#endif


