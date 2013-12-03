#include "dtc.h"
#include <iodefine.h>
#include <iodefine_ext.h>
#include <string.h>

static dtc_area dtc_ctrl;

void dtc_init (void)
{
    memset(&dtc_ctrl, 0, sizeof dtc_ctrl);
    DTCEN = 1;
    DTCEN0 = 0;
    DTCEN1 = 0;
    DTCEN2 = 0;
    DTCEN3 = 0;
    DTCEN4 = 0;
    DTCBAR = (unsigned int)&dtc_ctrl >> 8;
}

void dtc_set_data (unsigned char data_n, const volatile void *src, volatile void *dst, unsigned char size, unsigned char length, unsigned char mode)
{
    if (data_n < (sizeof(dtc_ctrl.data) / sizeof(dtc_ctrl.data[0])))
    {
        dtc_control_data *data = &(dtc_ctrl.data[data_n]);
        data->DTCCR = mode & DTC_MODE_MASK;
        data->DTSAR = (unsigned int)src;
        data->DTDAR = (unsigned int)dst;
        data->DTCCT = length;
        data->DTBLS = size;
        data->DTRLD = 0;
    }
}

void dtc_set_vector (unsigned char vector_n, unsigned char data_n)
{
    if (vector_n < (sizeof(dtc_ctrl.vector) / sizeof(dtc_ctrl.vector[0]))
        && data_n < (sizeof(dtc_ctrl.data) / sizeof(dtc_ctrl.data[0])))
    {
        dtc_ctrl.vector[vector_n] = (unsigned char)((unsigned int)&dtc_ctrl.data[data_n] & 0x00FFU);
    }
}

void dtc_enable_vector (unsigned char vector_n)
{
    if (vector_n < (sizeof(dtc_ctrl.vector) / sizeof(dtc_ctrl.vector[0])))
    {
        const unsigned char reg_n = vector_n / 8;
        volatile unsigned char *reg = &DTCEN0 + reg_n;
        const unsigned char bit_n = 7 - vector_n % 8;
        *reg |= (1U << bit_n);
    }
}

void dtc_disable_vector (unsigned char vector_n)
{
    if (vector_n < (sizeof(dtc_ctrl.vector) / sizeof(dtc_ctrl.vector[0])))
    {
        const unsigned char reg_n = vector_n / 8;
        volatile unsigned char *reg = &DTCEN0 + reg_n;
        const unsigned char bit_n = 7 - vector_n % 8;
        *reg &= ~(1U << bit_n);
    }
}

void dtc_wait (unsigned char vector_n)
{
    if (vector_n < (sizeof(dtc_ctrl.vector) / sizeof(dtc_ctrl.vector[0])))
    {
        const unsigned char reg_n = vector_n / 8U;
        const unsigned char bit = 1U << (7U - vector_n % 8U);
        volatile unsigned char *reg = &DTCEN0 + reg_n;
        /*@i@*/ while ((*reg & bit) != (unsigned char)0U);
    }
}
