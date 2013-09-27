#include "uart0.h"
#include <iodefine.h>
#include <iodefine_ext.h>

void uart0_init(void)
{
    /* Configure UART0 */
    SAU0EN = 1;                                             /* Supply clock to serial array unit 0 */
    {
        /* >=4 cycle delay required by manual */
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    }
    SPS0 = 0x00U;                                           /* Set input clock (CK00 and CK01) to fclk = 16MHz */
    /* Setup operation mode for transmitter (channel 0) */
    SMR00 = 0x0023U;                                        /* Operation clock : CK00,
                                                               Transfer clock : division of CK00
                                                               Start trigger : software
                                                               Detect falling edge as start bit
                                                               Operation mode : UART
                                                               Interrupt source : buffer empty
                                                            */
    SCR00 = 0x8097U;                                        /* Transmission only
                                                               Reception error interrupt masked
                                                               Phase clock : type 1
                                                               No parity
                                                               LSB first
                                                               1 stop bit
                                                               8-bit data length
                                                            */
    SDR00 = (7 << 9);                                       /* transfer clock : 16 MHz / (7 + 1) / 2 = 1 Mbps */
    SOL0 &= ~0x01U;                                         /* Output is not inverted */
    SO0 |= 0x01U;                                           /* Initial output level is 1 */
    SOE0 |= 0x01U;                                          /* Enable data output */

    /* Configure PM51 as open-drain output */
    POM5 |= (1 << 1);
    P5 |= (1 << 1);
    PM5 &= ~(1 << 1);

    SS0 = 0x01U;                                            /* Enable UART0 transmitter operation (channel 0) */
    STIF0 = 1;
}

int uart0_puts(const char __far * s)
{
    while ('\0' != *s)
    {
        while (0 == STIF0);
        STIF0 = 0;
        SDR00 = *s++;
    }
    while (0 == STIF0);
    STIF0 = 0;
    SDR00 = '\n';
    return 1;
}

