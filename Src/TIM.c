#include "TIM.h"

void init_TIM2()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 = 0;
  TIM2->PSC = 960 - 1;
  TIM2->ARR = 0xFFFFFFFF;
  TIM2->CNT = 0;
  TIM2->EGR |= TIM_EGR_UG;
  TIM2->CR1 |= TIM_CR1_CEN;
}

void init_TIM3()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  TIM3->PSC = (96-1);
  TIM3->ARR = 10000;
  TIM3->DIER |= TIM_DIER_UIE;
  TIM3->CR1 |= (1<<0);
  while(!(TIM3->SR & (1<<0)));
}

void init_TIM()
{
  init_TIM2();
  init_TIM3();
}

void delay(uint32_t ms) {
  TIM2->CNT = 0;
  while(TIM2->CNT < ms*100);
}
