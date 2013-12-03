#include <iodefine.h>
#include <iodefine_ext.h>

void adc_init (void)
{
    ADCEN = 1;
    ADM0 = 0x38;                                            /* Select mode, fclk/2, 2.375 μs */
    ADM1 = 0x20;                                            /* Software trigger, one-shot conversion */
    ADM2 = 0x00;                                            /* 10-bit mode, Vdd/Vss are used as reference */
    ADS  = 0x81;                                            /* Input source - 1.45V internal reference */
    ADLL = 0x00;                                            /* Lower threshold level */
    ADUL = 0xFF;                                            /* Upper threshold level */
}

unsigned int adc_get_vdd (void)
{
    unsigned int i;
    unsigned long voltage;
    ADCE = 1;
    /* The reference voltage stabilization wait time (1 μs) */
    for (i = 16; i; --i)
        asm("");
    /* First conversion (result is undefined)
     * 14.3.7 Analog input channel specification register (ADS) Caution 8
     * 14.7.4 Setup when temperature sensor output voltage/internal reference voltage is selected
     * (example for software trigger mode and one-shot conversion mode)
     */
    ADIF = 0;
    ADCS = 1;
    while (ADIF == 0);
    /* Seconds conversion */
    ADIF = 0;
    ADCS = 1;
    while (ADIF == 0);
    /* Convert conversion result into voltage */
    voltage = (1024UL * 1450UL) / (ADCR / 64);
    return voltage;
}
