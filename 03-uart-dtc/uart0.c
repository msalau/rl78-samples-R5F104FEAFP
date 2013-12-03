#include "uart0.h"
#include "dtc.h"
#include <iodefine.h>
#include <iodefine_ext.h>
#include <string.h>

void uart0_init (void)
{
    dtc_disable_vector(DTC_VECTOR_ST0_CSI00);
    dtc_set_vector(DTC_VECTOR_ST0_CSI00, 0);
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
    POM5 |= (1U << 1);
    P5 |= (1U << 1);
    PM5 &= ~(1U << 1);

    SS0 = 0x01U;                                            /* Enable UART0 transmitter operation (channel 0) */
    STIF0 = 1;
}

int uart0_puts (const char * s)
{
    const unsigned int length = strlen(s);

    /* Prepare transmit buffer */
    char data[length + 1];
    memcpy(data, s, length);
    data[length] = '\n';

    /* Transmit data with DTC (all bytes except the first one) */
    dtc_disable_vector(DTC_VECTOR_ST0_CSI00);
    dtc_set_data(0, &data[1], &SDR00, 1, sizeof data - 1, DTC_MODE_8_BITS | DTC_MODE_FIX_DST | DTC_MODE_INC_SRC);
    dtc_enable_vector(DTC_VECTOR_ST0_CSI00);
    /* Send first byte */
    while (STIF0 == 0);
    SDR00 = data[0];
    /* Wait till the end of DTC operation */
    dtc_wait(DTC_VECTOR_ST0_CSI00);

    return 1;
}
