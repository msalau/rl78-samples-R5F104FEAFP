#include <iodefine.h>
#include <iodefine_ext.h>

static void clock_init (void)
{
    OSMC = (1 << 7);                                        /* Set RTCLPC bit
                                                               don't supply fsub to peripherals
                                                               other than RTC and IT */
    CMC = 0x10;                                             /* Enable XT pins, medium drive */
    CSC &= ~(1U << 6);                                      /* Start XT oscillator */
}

static void it_init (void)
{
    RTCEN = 1;                                              /* Enable Interval Timer (IT) */
    ITMK = 1;                                               /* Disable IT interrupt */
    ITMC = 0x8FFFU;                                         /* Set maximum period of IT */
    ITIF = 0;                                               /* Clear interrupt flag */
    ITMK = 0;                                               /* Enable IT interrupt */
}

static void io_init (void)
{
    P1 &= ~(1U << 5);                                       /* Set P15 low */
    PM1 &= ~(1U << 5);                                      /* Make P15 an output */
    POM1 &= ~(1U << 5);                                     /* Make P15 a CMOS output */
}

int main (void)
{
    clock_init();
    it_init();
    io_init();

    for (;;)
    {
        /* Since bit EI is not set (interrupts are disabled)
         * any interrupt request will release sleep mode
         */
        STOP();
        ITIF = 0;                                           /* Clear IT interrupt flag */
        P1 ^= (1U << 5);                                    /* Toggle LED state */
    }
}
