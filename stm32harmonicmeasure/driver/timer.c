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
* Description    : �趨TIM2_CC2Ƶ��
* Input          : frequency :��������Ƶ��,Ĭ��Ϊ50Hz
*                  dotspercycle :ÿ���ڲ�������,Ĭ��Ϊ256,������ѡΪ512,1024
* Output         : None
* Return         : None
*******************************************************************************/
void  TIM2Init(u32 frequency,u32 dotspercycle)
{
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  //TIM_OCInitTypeDef         TIM_OCInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel  = TIM2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//  TIM_OCInitTypeDef     TIM_OCInitStructure;
  TIM_Cmd(TIM2, DISABLE);//��ֹͣTIM2ʱ�ӣ���׼�����������

  if(frequency == 0 ) frequency = 5000;//50Hz
  if( (dotspercycle != 256 ) && (dotspercycle != 512 ) && (dotspercycle != 1024 )) dotspercycle =256;
  /* -----------------------------------------------------------------------
  TIM2 ����: ����TIM2_CC2ʱ�ӿ����ź����ڿ���ADC����
  1.�趨TIMԤ��ƵΪ72,��ʱ��ÿ���δ���ʱ��Ϊ72/72M=1us
  2.
  ----------------------------------------------------------------------- */
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1000; //����ֵ
  TIM_TimeBaseStructure.TIM_Prescaler = 35999; //Ԥ��Ƶ,��ֵ+1,ʱ�ӵδ�Ϊ72/72M=1us
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  /* TIM_OCMode_Toggle Mode configuration: Channel2 */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 10;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
//
//  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    //Ԥ����������ж�λ��ֹһ���þ����ж�/
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

  //4��ͨ��������������ж�/
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  /*---------------------------------------*/
  //TIM_ARRPreloadConfig(TIM2, ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}





/* End of file ---------------------------------------------------------------*/
