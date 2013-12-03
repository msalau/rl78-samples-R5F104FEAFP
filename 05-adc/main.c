#include "uart0.h"
#include "adc.h"

int main(void)
{
    unsigned int Vdd;
    char buf[6];
    adc_init();
    uart0_init();
    Vdd = adc_get_vdd();
    buf[0] = '0' + Vdd / 1000;
    buf[1] = '.';
    buf[2] = '0' + (Vdd % 1000) / 100;
    buf[3] = '0' + (Vdd % 100) / 10;
    buf[4] = '0' + (Vdd % 10);
    buf[5] = '\0';
    (void)uart0_puts(buf);
    for(;;);
}
