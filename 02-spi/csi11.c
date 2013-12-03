#include "csi11.h"
#include <iodefine.h>
#include <iodefine_ext.h>

void csi11_init (void)
{
    /* Configure CSI11 (SAU0, channel 3) */
    SAU0EN = 1;                                             /* Enable Serial Array Unit 0 */
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = 0;                                               /* Set CK00 = CK01 = fclk = 16 Mhz */
    SMR03 = 0x0021U;                                        /* Operation clock : CK00
                                                               Transfer clock : division of CK00
                                                               Start trigger : software
                                                               Operation mode : CSI
                                                               Interrupt source : buffer empty
                                                            */
    SCR03 = 0xB087U;                                        /* Transmission only
                                                               Clock and data phase #4
                                                               Reception error interrupt masked
                                                               No parity
                                                               LSB first
                                                               8-bit data length
                                                            */
    SDR03 = (7U << 9);                                      /* transfer clock : 16 MHz / (7 + 1) / 2 = 1 Mbps */
    SO0 &= ~((1U << 3) | (1U << 11));                       /* Initial output level is 0 for clock and data */

    /* Configure P10/SCK11 and P12/SO11 as push-pull outputs */
    POM1 &= ~0x05U;
    P1 |= 0x05U;
    PM1 &= ~0x05U;
    /* Configure P27 as push-pull output */
    P2 &= ~(1U << 7);
    PM2 &= ~(1U << 7);
    ADPC = 0x01U;                                           /* Switch all ports, multiplexed with analog function, to digital mode */
}

int csi11_send (const void *src, int n)
{
    int i;
    unsigned char *p = (unsigned char *)src;
    P2 |= (1U << 7);                                        /* Set CS */
    SOE0 |= (1U << 3);                                      /* Enable data output */
    SS0 = (1U << 3);                                        /* Enable CSI11 transmitter operation (channel 3) */
    CSIIF11 = 1;
    for (i = n; i; --i)                                     /* Send bytes */
    {
        while (0 == CSIIF11);
        CSIIF11 = 0;
        SDR03 = *p++;
    }
    while (SSR03 & (1U << 6));                              /* Wait for transmission completion */
    ST0 = (1U << 3);                                        /* Disable CSI11 transmitter operation (channel 3) */
    SOE0 &= ~(1U << 3);                                     /* Disable data output */
    SO0 &= ~((1U << 3) | (1U << 11));                       /* Initial output level is 0 for clock and data */
    P2 &= ~(1U << 7);                                       /* Clear CS */
    return n;
}
