#include "uart0.h"
#include "dtc.h"
#include <iodefine.h>
#include <iodefine_ext.h>

int main (void)
{
    dtc_init();
    uart0_init();
    (void)uart0_puts("Hello, RL78 [:\n");
    P1 |= (1U << 5);                                        /* Set P15 high */
    POM1 &= ~(1U << 5);                                     /* Make P15 a CMOS output */
    PM1 &= ~(1U << 5);                                      /* Make P15 an output */
    for(;;);
}
