/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

#include "FreeRTOS.h" 
#include "task.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while(1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while(1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while(1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while(1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//extern void xPortSysTickHandler(void);
// //systick ?????? 
////void SysTick_Handler(void) 
// { 
//#if (INCLUDE_xTaskGetSchedulerState == 1 ) 
//	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { 
//#endif /* INCLUDE_xTaskGetSchedulerState */ 
//	xPortSysTickHandler(); 
//#if (INCLUDE_xTaskGetSchedulerState == 1 ) 
// } 
//#endif
// }



/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

//extern unsigned char rx4buf[UART4_RXSIZE];
//extern unsigned char rx4head;
//extern unsigned char rx4tail;
//extern unsigned char tx4buf[UART4_TXSIZE];
//extern unsigned char tx4head;
//extern unsigned char tx4tail;

//void UART4_IRQHandler(void)
//{
//    if(USART_GetFlagStatus(UART4, USART_FLAG_ORE) == SET)
//    {
//        //…œ“Á÷–∂œ
//        USART_ClearFlag(UART4,USART_FLAG_ORE);
//        USART_ReceiveData(UART4);
//    }
//
//    if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)   //Receive Data register not empty interrupt.
//    {
//        USART_ClearITPendingBit(UART4,USART_IT_RXNE);
//
//        rx4head++;
//        rx4head %= UART4_RXSIZE;
//        rx4buf[rx4head] = USART_ReceiveData(UART4);
//    }
//
//    if(USART_GetITStatus(UART4, USART_IT_TXE) == SET)
//    {
//        USART_ClearITPendingBit(UART4,USART_IT_TXE);
//
//        if(tx4tail != tx4head)
//        {
//            tx4tail++;
//            tx4tail %= UART4_TXSIZE;
//            USART_SendData(UART4, tx4buf[tx4tail]);
//        }
//        else
//        {
//            USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
//        }
//    }
//}
/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
