#include "uart0.h"

int main(void)
{
    uart0_init();
    (void)uart0_puts("Hello, RL78! [:");
    for(;;);
}

