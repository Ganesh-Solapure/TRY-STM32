#include "main.h"

ADC_HandleTypeDef hadc1; // ADC handle structure for ADC1

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);

int main(void)
{
    int adcResult; // Variable to store ADC conversion result

    HAL_Init(); // Initialize the HAL library and reset peripherals

    SystemClock_Config(); // Configure the system clock

    MX_GPIO_Init(); // Initialize GPIO pins
    MX_ADC1_Init(); // Initialize ADC1

    while (1)
    {
        HAL_ADC_Start(&hadc1); // Start ADC conversion
        HAL_ADC_PollForConversion(&hadc1, 100); // Wait for ADC conversion to complete with a timeout of 100ms
        adcResult = HAL_ADC_GetValue(&hadc1); // Get the ADC conversion result
        HAL_ADC_Stop(&hadc1); // Stop the ADC conversion

        if (adcResult < 2048) // Check if the ADC result is below the threshold (mid-range for 12-bit ADC)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET); // Turn off the LED connected to PA5
        else
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET); // Turn on the LED connected to PA5
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // Structure to configure the oscillator
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; // Structure to configure the clock tree

    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_1); // Set flash latency for 1 wait state

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // Use HSI oscillator
    RCC_OscInitStruct.HSIState = RCC_HSI_ON; // Enable HSI oscillator
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1; // Set HSI division factor to 1
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // Default HSI calibration
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler(); // Handle configuration error
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; // Enable clocks for HCLK, SYSCLK, and PCLK1
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; // Use HSI as the system clock source
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1; // No division for SYSCLK
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1; // No division for AHB
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1; // No division for APB1

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler(); // Handle configuration error
    }
}

static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0}; // Structure to configure ADC channels

    hadc1.Instance = ADC1; // Set the ADC instance to ADC1
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2; // ADC clock prescaler
    hadc1.Init.Resolution = ADC_RESOLUTION_12B; // 12-bit resolution
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT; // Right-aligned data
    hadc1.Init.ScanConvMode = ADC_SCAN_SEQ_FIXED; // Fixed sequence scanning mode
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV; // End of conversion flag after each conversion
    hadc1.Init.LowPowerAutoWait = DISABLE; // Disable low power auto wait
    hadc1.Init.LowPowerAutoPowerOff = DISABLE; // Disable low power auto power-off
    hadc1.Init.ContinuousConvMode = DISABLE; // Single conversion mode
    hadc1.Init.NbrOfConversion = 1; // One conversion at a time
    hadc1.Init.DiscontinuousConvMode = DISABLE; // Disable discontinuous mode
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // Start conversion via software
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; // No external trigger edge
    hadc1.Init.DMAContinuousRequests = DISABLE; // Disable continuous DMA requests
    hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED; // Preserve data in case of overrun
    hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5; // Sampling time of 1.5 ADC clock cycles
    hadc1.Init.OversamplingMode = DISABLE; // Disable oversampling
    hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH; // High trigger frequency mode

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler(); // Handle initialization error
    }

    sConfig.Channel = ADC_CHANNEL_0; // Select channel 0
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER; // Rank the channel by number

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler(); // Handle configuration error
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0}; // Structure to configure GPIO pins

    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable clock for GPIO port A

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // Set PA5 to low (LED off)

    GPIO_InitStruct.Pin = GPIO_PIN_5; // Configure PA5
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Set mode to push-pull output
    GPIO_InitStruct.Pull = GPIO_NOPULL; // No internal pull-up or pull-down resistor
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Set output speed to low
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // Initialize PA5 with the configuration
}

void Error_Handler(void)
{
    __disable_irq(); // Disable interrupts
    while (1) // Infinite loop to indicate error
    {
    }
}
