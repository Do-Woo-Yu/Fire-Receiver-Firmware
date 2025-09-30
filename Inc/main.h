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
#define EN_H_24V_Pin GPIO_PIN_2
#define EN_H_24V_GPIO_Port GPIOE
#define BAT_V_H_Pin GPIO_PIN_3
#define BAT_V_H_GPIO_Port GPIOE
#define FIRE_RY_Pin GPIO_PIN_4
#define FIRE_RY_GPIO_Port GPIOE
#define LOCAL_RY_Pin GPIO_PIN_5
#define LOCAL_RY_GPIO_Port GPIOE
#define MAIN_RY_Pin GPIO_PIN_6
#define MAIN_RY_GPIO_Port GPIOE
#define KEY_SW14_Pin GPIO_PIN_13
#define KEY_SW14_GPIO_Port GPIOC
#define BUZ_OUT_Pin GPIO_PIN_3
#define BUZ_OUT_GPIO_Port GPIOC
#define CHG_H_Pin GPIO_PIN_7
#define CHG_H_GPIO_Port GPIOE
#define GUID_RY_Pin GPIO_PIN_8
#define GUID_RY_GPIO_Port GPIOE
#define KEY_SW13_Pin GPIO_PIN_9
#define KEY_SW13_GPIO_Port GPIOE
#define KEY_SW12_Pin GPIO_PIN_10
#define KEY_SW12_GPIO_Port GPIOE
#define KEY_SW10_Pin GPIO_PIN_12
#define KEY_SW10_GPIO_Port GPIOE
#define KEY_SW9_Pin GPIO_PIN_13
#define KEY_SW9_GPIO_Port GPIOE
#define KEY_SW8_Pin GPIO_PIN_14
#define KEY_SW8_GPIO_Port GPIOE
#define KEY_SW7_Pin GPIO_PIN_15
#define KEY_SW7_GPIO_Port GPIOE
#define KEY_SW6_Pin GPIO_PIN_10
#define KEY_SW6_GPIO_Port GPIOB
#define KEY_SW5_Pin GPIO_PIN_11
#define KEY_SW5_GPIO_Port GPIOB
#define KEY_SW4_Pin GPIO_PIN_12
#define KEY_SW4_GPIO_Port GPIOB
#define KEY_SW3_Pin GPIO_PIN_13
#define KEY_SW3_GPIO_Port GPIOB
#define KEY_SW2_Pin GPIO_PIN_14
#define KEY_SW2_GPIO_Port GPIOB
#define KEY_SW1_Pin GPIO_PIN_15
#define KEY_SW1_GPIO_Port GPIOB
#define MOD_LED9_Pin GPIO_PIN_8
#define MOD_LED9_GPIO_Port GPIOD
#define MOD_LED8_Pin GPIO_PIN_9
#define MOD_LED8_GPIO_Port GPIOD
#define MOD_LED7_Pin GPIO_PIN_10
#define MOD_LED7_GPIO_Port GPIOD
#define MOD_LED6_Pin GPIO_PIN_11
#define MOD_LED6_GPIO_Port GPIOD
#define MOD_LED5_Pin GPIO_PIN_12
#define MOD_LED5_GPIO_Port GPIOD
#define MOD_LED4_Pin GPIO_PIN_13
#define MOD_LED4_GPIO_Port GPIOD
#define MOD_LED3_Pin GPIO_PIN_14
#define MOD_LED3_GPIO_Port GPIOD
#define MOD_LED2_Pin GPIO_PIN_15
#define MOD_LED2_GPIO_Port GPIOD
#define MOD_LED1_Pin GPIO_PIN_6
#define MOD_LED1_GPIO_Port GPIOC
#define SW_LED10_Pin GPIO_PIN_7
#define SW_LED10_GPIO_Port GPIOC
#define SW_LED9_Pin GPIO_PIN_8
#define SW_LED9_GPIO_Port GPIOC
#define SW_LED8_Pin GPIO_PIN_9
#define SW_LED8_GPIO_Port GPIOC
#define W_CS1_Pin GPIO_PIN_15
#define W_CS1_GPIO_Port GPIOA
#define SW_LED4_Pin GPIO_PIN_12
#define SW_LED4_GPIO_Port GPIOC
#define SW_LED3_Pin GPIO_PIN_0
#define SW_LED3_GPIO_Port GPIOD
#define SW_LED2_Pin GPIO_PIN_1
#define SW_LED2_GPIO_Port GPIOD
#define SW_LED1_Pin GPIO_PIN_2
#define SW_LED1_GPIO_Port GPIOD
#define IN_LED10_Pin GPIO_PIN_3
#define IN_LED10_GPIO_Port GPIOD
#define IN_LED9_Pin GPIO_PIN_4
#define IN_LED9_GPIO_Port GPIOD
#define IN_LED8_Pin GPIO_PIN_5
#define IN_LED8_GPIO_Port GPIOD
#define IN_LED7_Pin GPIO_PIN_6
#define IN_LED7_GPIO_Port GPIOD
#define IN_LED6_Pin GPIO_PIN_7
#define IN_LED6_GPIO_Port GPIOD
#define W_CLK1_Pin GPIO_PIN_3
#define W_CLK1_GPIO_Port GPIOB
#define W_MISO1_Pin GPIO_PIN_4
#define W_MISO1_GPIO_Port GPIOB
#define W_MOSI1_Pin GPIO_PIN_5
#define W_MOSI1_GPIO_Port GPIOB
#define IN_LED5_Pin GPIO_PIN_6
#define IN_LED5_GPIO_Port GPIOB
#define IN_LED4_Pin GPIO_PIN_7
#define IN_LED4_GPIO_Port GPIOB
#define IN_LED3_Pin GPIO_PIN_8
#define IN_LED3_GPIO_Port GPIOB
#define IN_LED2_Pin GPIO_PIN_9
#define IN_LED2_GPIO_Port GPIOB
#define IN_LED1_Pin GPIO_PIN_0
#define IN_LED1_GPIO_Port GPIOE
#define GUID_RY_X_Pin GPIO_PIN_1
#define GUID_RY_X_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
