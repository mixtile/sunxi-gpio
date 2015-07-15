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

#include "util_gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static void usage(FILE* fp, int argc, char** argv)
{
    fprintf (fp,
    "Usage: %s [options]\n\n"
    "Options:\n"
    "-i | --io name   io name, like PH01 \n"
    "-w | --write     write operation \n"
    "-r | --read      read operation \n"
    "-v | --value     value set, 0/1 \n"
    "-h | --help      Print this message\n"
    "",
    argv[0]);
}

static const char short_options [] = "i:wrv:h";
static const struct option long_options [] =
{
        { "write",       no_argument,        NULL,  'w'},
        { "read",        no_argument,        NULL,  'r'},
        { "io",          required_argument,  NULL,  'i' },
        { "value",       required_argument,  NULL,  'v' },
        { "help",        no_argument,        NULL,  'h' },
        { 0, 0, 0, 0 }
};

int main(int argc, char **argv)
{
    char* gpio_str = NULL;
    char* value_str = NULL;
    int value = 0;
    char operation = 0;
    
    for (;;)
    {
        int index, c = 0;
        c = getopt_long(argc, argv, short_options, long_options, &index);

        if (-1 == c)
        break;

        switch (c)
        {
            case 0: /* getopt_long() flag */
                break;
            case 'i':
                gpio_str = optarg;
                break;
            case 'v':
                value_str = optarg;
                value = atoi(value_str);
                break;
            case 'h':
                usage (stdout, argc, argv);
                exit(EXIT_SUCCESS);
                break;
            case 'w':
                if(0 != operation)
                {
                    usage (stdout, argc, argv);
                    return -1;
                }
                operation = 'w';
                break;
            case 'r':
                if(0 != operation)
                {
                    usage (stdout, argc, argv);
                    return -1;
                }
                operation = 'r';
                break;
            default:
                usage(stderr, argc, argv);
                exit(EXIT_FAILURE);
                break;
        }
    }
    
    if(gpio_str && value_str)
    {
        int get_value = -1;
        sunxi_gpio_init();
        switch(operation)
        {
            case 'w':
                sunxi_gpio_export(gpio_str);
                sunxi_gpio_enable_out(gpio_str);
                sunxi_gpio_set_out(gpio_str, value);
                break;
            case 'r':
                sunxi_gpio_export(gpio_str);
                sunxi_gpio_enable_in(gpio_str);
                get_value = sunxi_gpio_get_in(gpio_str);
                if(-1 == get_value)
                {
                    fprintf(stderr, "sunxi_gpio get value from %s failed \n", gpio_str);
                    break;
                }
                fprintf(stdout, "sunxi_gpio get value %d from %s \n", get_value, gpio_str);
            default:
                break;
        }
    
        sunxi_gpio_fini();
    }

    return 0;
}
