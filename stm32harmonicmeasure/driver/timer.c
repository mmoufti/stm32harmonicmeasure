/******************** (C) COPYRIGHT 2010 ***************************************
* File Name          : timer.c
* Date First Issued  : 06/29/10
* Description        : 
********************************************************************************
* History:
* 
* 
* 
* 06/29/10: V0.01     Created by heroxx
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "timer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : ADC_Sample_Frequency_Set
* Description    : 设定TIM2_CC2频率
* Input          : frequency :采样波形频率,默认为50Hz
*                  dotspercycle :每周期采样点数,默认为256,其他可选为512,1024
* Output         : None
* Return         : None
*******************************************************************************/
void  TIM2Init(u32 frequency,u32 dotspercycle)
{
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_OCInitTypeDef         TIM_OCInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

//  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure TIM1_CH4 (PA11) as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel  = TIM2_IRQChannel;//TIM1_CC_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_Cmd(TIM2, DISABLE);//先停止TIM2时钟，以准备下面的设置

  if(frequency == 0 ) frequency = 5000;//50Hz
  if( (dotspercycle != 256 ) && (dotspercycle != 512 ) && (dotspercycle != 1024 )) dotspercycle =256;
  /* -----------------------------------------------------------------------
  TIM2 配置: 产生TIM2_CC2时钟控制信号用于控制ADC采样
  1.设定TIM预分频为72,则定时器每个滴答间隔时间为72/72M=1us
  2.
  ----------------------------------------------------------------------- */
  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 0xffff; //计数值
  TIM_TimeBaseStructure.TIM_Prescaler = 55; //预分频,此值+1,时钟滴答为56/56M=0.001ms=1us
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM_OCMode_Toggle Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 5000;     //0.5ms
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  //TIM_ARRPreloadConfig(TIM2,ENABLE);

//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 200;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
//
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 300;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
//
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 400;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  
    //预先清除所有中断位防止一启用就有中断/
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update|TIM_IT_CC2);

  //4个通道和溢出都配置中断/
  TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC2, ENABLE);
  /*---------------------------------------*/
  /* TIM1 main Output Enable */
  //TIM_CtrlPWMOutputs(TIM1, ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}





/* End of file ---------------------------------------------------------------*/
