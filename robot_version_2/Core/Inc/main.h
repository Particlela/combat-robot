/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Motor_PWM_TIM9_CH1_Pin GPIO_PIN_5
#define Motor_PWM_TIM9_CH1_GPIO_Port GPIOE
#define Motor_PWM_TIM9_CH2_Pin GPIO_PIN_6
#define Motor_PWM_TIM9_CH2_GPIO_Port GPIOE
#define Engine_Right_Yellow_Pin GPIO_PIN_0
#define Engine_Right_Yellow_GPIO_Port GPIOC
#define Engine_Right_Green_Pin GPIO_PIN_1
#define Engine_Right_Green_GPIO_Port GPIOC
#define BLE_TX_Pin GPIO_PIN_2
#define BLE_TX_GPIO_Port GPIOA
#define BLE_RX_Pin GPIO_PIN_3
#define BLE_RX_GPIO_Port GPIOA
#define Engine_PWM_Left_Pin GPIO_PIN_6
#define Engine_PWM_Left_GPIO_Port GPIOA
#define Gray_R2_Pin GPIO_PIN_7
#define Gray_R2_GPIO_Port GPIOE
#define Gray_R1_Pin GPIO_PIN_8
#define Gray_R1_GPIO_Port GPIOE
#define Gray_M_Pin GPIO_PIN_9
#define Gray_M_GPIO_Port GPIOE
#define Gray_L1_Pin GPIO_PIN_10
#define Gray_L1_GPIO_Port GPIOE
#define Gray_L2_Pin GPIO_PIN_11
#define Gray_L2_GPIO_Port GPIOE
#define Engine_PWM_Right_Pin GPIO_PIN_12
#define Engine_PWM_Right_GPIO_Port GPIOD
#define Engine_Rightcode1_Pin GPIO_PIN_6
#define Engine_Rightcode1_GPIO_Port GPIOC
#define Engine_Rightcode2_Pin GPIO_PIN_7
#define Engine_Rightcode2_GPIO_Port GPIOC
#define Engine_Leftcode1_Pin GPIO_PIN_8
#define Engine_Leftcode1_GPIO_Port GPIOA
#define Engine_Leftcode2_Pin GPIO_PIN_9
#define Engine_Leftcode2_GPIO_Port GPIOA
#define Engine_Right2_Pin GPIO_PIN_0
#define Engine_Right2_GPIO_Port GPIOD
#define Engine_Right1_Pin GPIO_PIN_1
#define Engine_Right1_GPIO_Port GPIOD
#define Engine_Left2_Pin GPIO_PIN_2
#define Engine_Left2_GPIO_Port GPIOD
#define Engine_Left1_Pin GPIO_PIN_3
#define Engine_Left1_GPIO_Port GPIOD
#define Engine_Left_Green_Pin GPIO_PIN_3
#define Engine_Left_Green_GPIO_Port GPIOB
#define Engine_Left_Yellow_Pin GPIO_PIN_5
#define Engine_Left_Yellow_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
