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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#if 0
#define UTIL_GPIO_DEBUG
#else
#define UTIL_GPIO_DEBUG printf
#endif

#define SYS_GPIO_EXPORT_PATH    "/sys/class/gpio/export"
#define SYS_GPIO_UNEXPORT_PATH    "/sys/class/gpio/unexport"
#define SYS_GPIO_PATH           "/sys/class/gpio"

#ifdef SUNXI_A31
#define GPIO_MAP(zone, idx) _sunxi_gpio_map_a31(zone, idx)
#else
#define GPIO_MAP(zone, idx) _sunxi_gpio_map(zone, idx)
#endif

typedef struct
{
    char base;
    u_int16_t digi_base;
}_stGPIO_MAP_T, *_pstGPIO_MAP_T;

typedef struct
{
    int fd_export;
    int fd_unexport;

    char path_buf[2048];
    int init_flag;

}_stSUNXI_GPIOS_T, *_pstSUNXI_GPIOS_T;

static _stSUNXI_GPIOS_T _gdc;
static _pstSUNXI_GPIOS_T _gpdc = &_gdc;

static int _sunxi_gpio_map(char zone, int idx)
{
    if( zone >= 'a' && zone <= 'z')
    {
        return (zone-'a')*32 + idx;
    }

    if(zone >= 'A' && zone <= 'Z')
    {
        return (zone-'A')*32 + idx;
    }

    return -1;
}

static int _sunxi_gpio_map_a31(char zone, int idx)
{
    static _stGPIO_MAP_T a31_maps[] = 
    {
        {'A', 0},
        {'B', 30},
        {'C', 40},
        {'D', 70},
        {'E', 100},
        {'F', 119},
        {'G', 127},
        {'H', 148},
        {'L', 181},
        {'M', 192},
    };
    
    if( zone >= 'a' && zone <= 'z')
    {
        zone = 'A' + zone - 'a';
    }
    
    if(zone >= 'A' && zone <= 'Z')
    {
        int ii = 0;
        for(ii=0; ii<sizeof(a31_maps)/sizeof(a31_maps[0]); ii++)
        {
            if(a31_maps[ii].base == zone)
            {
                return a31_maps[ii].digi_base + idx;
            }
        }
    }
    
    return -1;
}

int sunxi_gpio_init()
{
    _pstSUNXI_GPIOS_T pdc = _gpdc;
    pdc->fd_export = open(SYS_GPIO_EXPORT_PATH, O_WRONLY);
    pdc->fd_unexport = open(SYS_GPIO_UNEXPORT_PATH, O_WRONLY);

    if(pdc->fd_export && pdc->fd_unexport)
    {
        pdc->init_flag = 1;
        UTIL_GPIO_DEBUG("sunxi gpio init success! \n");
        return 0;
    }

    close(pdc->fd_export);
    close(pdc->fd_unexport);

    UTIL_GPIO_DEBUG("sunxi gpio init failed! \n");

    return 0;
}

int sunxi_gpio_export(char io[16])
{
    int id = 0;
    char id_str[8];
    _pstSUNXI_GPIOS_T pdc = _gpdc;
    if(0 == pdc->init_flag)
    {
        UTIL_GPIO_DEBUG("sunxi gpio export failed! not inited! \n");
        return -1;
    }
    UTIL_GPIO_DEBUG("sunxi gpio export %s \n", io);
    if('p' == io[0] || 'P' == io[0])
    {
        id = GPIO_MAP(io[1], atoi(&io[2]));
        snprintf(id_str, 8, "%d", id);

        write(pdc->fd_export, id_str, strlen(id_str));
    }
    else
    {
        fprintf(stderr, "sunxi gpio export failed, gpio is invalid: %s \n", io);
    }

    return 0;
}

int sunxi_gpio_unexport(char io[16])
{
    int id = 0;
    char id_str[8];
    _pstSUNXI_GPIOS_T pdc = _gpdc;
    if(0 == pdc->init_flag)
    {
        UTIL_GPIO_DEBUG("sunxi gpio export failed! not inited! \n");
        return -1;
    }
    UTIL_GPIO_DEBUG("sunxi gpio export %s \n", io);
    if('p' == io[0] || 'P' == io[0])
    {
        id = GPIO_MAP(io[1], atoi(&io[2]));
        snprintf(id_str, 8, "%d", id);

        write(pdc->fd_unexport, id_str, strlen(id_str));
    }
    else
    {
        fprintf(stderr, "sunxi gpio export failed, gpio is invalid: %s \n", io);
    }

    return 0;
}

int sunxi_gpio_enable_out(char io[16])
{
    int fd = 0;
    int id = 0;
    _pstSUNXI_GPIOS_T pdc = _gpdc;
    if(0 == pdc->init_flag)
    {
        return -1;
    }

    UTIL_GPIO_DEBUG("sunxi gpio enable out %s \n", io);

    if('p' == io[0] || 'P' == io[0])
    {
        id = GPIO_MAP(io[1], atoi(&io[2]));
        snprintf(pdc->path_buf, sizeof(pdc->path_buf), "%s/gpio%d/direction", SYS_GPIO_PATH, id);

        fd = open(pdc->path_buf, O_WRONLY);

        if(fd)
        {
            write(fd, "out", strlen("out"));
            close(fd);
            UTIL_GPIO_DEBUG("sunxi gpio enable out %s success! \n", pdc->path_buf);
            return 0;
        }

         UTIL_GPIO_DEBUG("sunxi gpio enable out %s failed! \n", pdc->path_buf);
    }
    else
    {
        fprintf(stderr, "sunxi gpio enable output failed, gpio is invalid: %s \n", io);
    }

    return -1;
}

int sunxi_gpio_enable_in(char io[16])
{
    int fd = 0;
    int id = 0;
    _pstSUNXI_GPIOS_T pdc = _gpdc;
    if(0 == pdc->init_flag)
    {
        return -1;
    }

    UTIL_GPIO_DEBUG("sunxi gpio enable out %s \n", io);

    if('p' == io[0] || 'P' == io[0])
    {
        id = GPIO_MAP(io[1], atoi(&io[2]));
        snprintf(pdc->path_buf, sizeof(pdc->path_buf), "%s/gpio%d/direction", SYS_GPIO_PATH, id);

        fd = open(pdc->path_buf, O_WRONLY);

        if(fd)
        {
            write(fd, "in", strlen("in"));
            close(fd);
            UTIL_GPIO_DEBUG("sunxi gpio enable out %s success! \n", pdc->path_buf);
            return 0;
        }

         UTIL_GPIO_DEBUG("sunxi gpio enable out %s failed! \n", pdc->path_buf);
    }
    else
    {
        fprintf(stderr, "sunxi gpio enable output failed, gpio is invalid: %s \n", io);
    }

    return -1;
}

int sunxi_gpio_set_out(char io[16], int bon)
{
    int fd = 0;
    int id = 0;
    _pstSUNXI_GPIOS_T pdc = _gpdc;
    if(0 == pdc->init_flag)
    {
        return -1;
    }

    UTIL_GPIO_DEBUG("sunxi gpio set out %s to %d \n", io, bon);

    if('p' == io[0] || 'P' == io[0])
    {
        id = GPIO_MAP(io[1], atoi(&io[2]));
        snprintf(pdc->path_buf, sizeof(pdc->path_buf), "%s/gpio%d/value", SYS_GPIO_PATH, id);

        fd = open(pdc->path_buf, O_WRONLY);

        if(fd)
        {
            write(fd, (bon ? "1" : "0"), 1);
            close(fd);
            UTIL_GPIO_DEBUG("sunxi gpio set out %s success! \n", pdc->path_buf);
            return 0;
        }

        UTIL_GPIO_DEBUG("sunxi gpio set out %s failed! \n", pdc->path_buf);
    }
    else
    {
        fprintf(stderr, "sunxi gpio set out failed, gpio is invalid: %s \n", io);
    }

    return -1;
}

int sunxi_gpio_get_in(char io[16])
{
    int fd = 0;
    int id = 0;
    char buf[4] = {0,0,0,0};
    _pstSUNXI_GPIOS_T pdc = _gpdc;
    if(0 == pdc->init_flag)
    {
        return -1;
    }

    UTIL_GPIO_DEBUG("sunxi gpio in from %s \n", io);

    if('p' == io[0] || 'P' == io[0])
    {
        id = GPIO_MAP(io[1], atoi(&io[2]));
        snprintf(pdc->path_buf, sizeof(pdc->path_buf), "%s/gpio%d/value", SYS_GPIO_PATH, id);

        fd = open(pdc->path_buf, O_RDONLY);

        if(fd)
        {
            read(fd, buf, 4);
            close(fd);
            UTIL_GPIO_DEBUG("sunxi gpio get in %s success! \n", pdc->path_buf);
            return atoi(buf);
        }

        UTIL_GPIO_DEBUG("sunxi gpio set out %s failed! \n", pdc->path_buf);
    }
    else
    {
        fprintf(stderr, "sunxi gpio set out failed, gpio is invalid: %s \n", io);
    }

    return -1;
}

int sunxi_gpio_fini()
{
    _pstSUNXI_GPIOS_T pdc = _gpdc;
    if(pdc->init_flag)
    {
        close(pdc->fd_export);
        close(pdc->fd_unexport);
    }
    return 0;
}
