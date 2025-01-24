#include "main.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
  HAL_Init(); // Initialize the HAL library and reset peripherals
  SystemClock_Config(); // Configure the system clock
  MX_GPIO_Init(); // Initialize GPIO peripherals

  while (1)
  {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET); // Set GPIO pin PA5 to HIGH
    HAL_Delay(500); // Delay for 500 milliseconds
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET); // Set GPIO pin PA5 to LOW
    HAL_Delay(500); // Delay for 500 milliseconds
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_1); // Set flash latency

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // Use High-Speed Internal oscillator
  RCC_OscInitStruct.HSIState = RCC_HSI_ON; // Enable HSI
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1; // No division for HSI
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // Default HSI calibration

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(); // Handle errors during RCC configuration
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; // Configure clocks for HCLK, SYSCLK, and PCLK1
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; // Set SYSCLK source to HSI
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1; // No division for SYSCLK
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1; // No division for HCLK
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1; // No division for APB1

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler(); // Handle errors during clock configuration
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable clock for GPIOA

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // Initialize GPIO pin PA5 to LOW

  GPIO_InitStruct.Pin = GPIO_PIN_5; // Configure pin PA5
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Set mode to push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL; // No pull-up or pull-down resistor
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Set low speed for the pin
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // Initialize the GPIO pin with the configuration
}

void Error_Handler(void)
{
  __disable_irq(); // Disable interrupts
  while (1) {} // Enter an infinite loop to signal an error
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  // Report file name and line number where assert_param error occurred
}
#endif
