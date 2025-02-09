#include "drv_supc.h"
#include "sam.h"

void drv_supc_init(void)
{
    MCLK_REGS->MCLK_APBAMASK |= MCLK_APBAMASK_SUPC(1);

    SUPC_REGS->SUPC_VREF = SUPC_VREF_VREFOE(1);
}