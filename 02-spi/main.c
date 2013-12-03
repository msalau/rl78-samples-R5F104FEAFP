#include "csi11.h"

int main (void)
{
    csi11_init();
    (void)csi11_send("Hello, RL78 [:\n", 14);
    for(;;);
}
