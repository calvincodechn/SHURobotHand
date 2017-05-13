#include "drv_sysclk.h"

volatile uint64_t g_ulMsCounters;

void systick_init(void)
{
	RCC_ClocksTypeDef    RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
}

void SysTickIntHandler(void)
{
    g_ulMsCounters++;    
}

uint64_t millis(void)
{	
	return g_ulMsCounters;
}
