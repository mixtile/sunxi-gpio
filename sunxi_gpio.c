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
    "-v | --value     value set, 0/1 \n"
    "-h | --help      Print this message\n"
    "",
    argv[0]);
}

static const char short_options [] = "i:v:h";
static const struct option long_options [] =
{
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
            default:
                usage(stderr, argc, argv);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if(gpio_str && value_str)
    {
        sunxi_gpio_init();
        sunxi_gpio_export(gpio_str);
        sunxi_gpio_enable_out(gpio_str);
        sunxi_gpio_set_out(gpio_str, value);
        sunxi_gpio_fini();
    }

    return 0;
}
