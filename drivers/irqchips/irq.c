#include <yios/arm-gic.h>

void irq_handle(void)
{
    gic_handle_irq();
}
