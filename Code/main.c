#include "main.h"

int main()
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13_1;
    while (1)
    {
        GPIOC->ODR ^= GPIO_ODR_ODR13;
    }
    return 0;
}