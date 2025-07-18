/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DHT1.h"
#include "i2c_lcd.h"
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LCD_I2C_ADDRESS (0x27 << 1) // En yaygın I2C adresi (8-bit formunda) veya 0x3F << 1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
I2C_LCD_HandleTypeDef lcd_handle = { .hi2c = &hi2c1, .address = (0x27 << 1) };
DHT_DataTypedef dht_data; // DHT verilerini tutmak için bir değişken oluşturduk

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Display_Temp(float Temp);
void Display_Rh(float Rh);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// LCD Display fonksiyonları
void Display_Temp(float Temp) {
	char str[20] = { 0 };
	lcd_gotoxy(&lcd_handle, 0, 0); // Satır 0, Sütun 0
	sprintf(str, "Sicaklik:%.1f", Temp); // Küsurat için .1f kullandım
	lcd_puts(&lcd_handle, str);
	lcd_putchar(&lcd_handle, 0b11011111); // Derece sembolü için ASCII kodu
	lcd_putchar(&lcd_handle, 'C');
}

void Display_Rh(float Rh) {
	char str[20] = { 0 };
	lcd_gotoxy(&lcd_handle, 0, 1); // Satır 1, Sütun 0
	sprintf(str, "Nem:      %.1f", Rh); // Küsurat için .1f kullandım ve boşluklar ekledim
	lcd_puts(&lcd_handle, str);
	lcd_putchar(&lcd_handle, '%');
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM6_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim6); // TIM6'yı başlat, böylece delay_us kullanılabilir
	lcd_init(&lcd_handle);
	lcd_puts(&lcd_handle, "DHT11 Basliyor");
	HAL_Delay(3000);
	lcd_clear(&lcd_handle);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		DHT_Status status;
		uint8_t retry_count = 0;
		const uint8_t max_retries = 3;

		// Maksimum 3 kez deneme yap
		do {
			status = DHT_GetData(&dht_data);
			if (status == DHT_OK) {
				break; // Başarılı okuma, döngüden çık
			}
			retry_count++;
			HAL_Delay(500); // Yeniden deneme arasında kısa bekleme
		} while (retry_count < max_retries);

		// LCD'yi temizle
		lcd_clear(&lcd_handle);

		switch (status) {
		case DHT_OK:
			// Başarılı okuma - verileri göster
			Display_Temp(dht_data.Temperature);
			Display_Rh(dht_data.Humidity);
			break;

		case DHT_NO_RESPONSE:
			// 3 deneme sonunda hala yanıt yok
			lcd_gotoxy(&lcd_handle, 0, 0);
			lcd_puts(&lcd_handle, "Sensor Problemi!");
			lcd_gotoxy(&lcd_handle, 0, 1);
			lcd_puts(&lcd_handle, "Kablo Kontrol Et");
			break;

		case DHT_CHECKSUM_ERROR:
			// 3 deneme sonunda hala checksum hatası
			lcd_gotoxy(&lcd_handle, 0, 0);
			lcd_puts(&lcd_handle, "Surekli Veri Hata");
			lcd_gotoxy(&lcd_handle, 0, 1);
			lcd_puts(&lcd_handle, "Sensor Arizali?");
			break;

		default:
			// Bilinmeyen hata
			lcd_gotoxy(&lcd_handle, 0, 0);
			lcd_puts(&lcd_handle, "Bilinmeyen Hata!");
			break;
		}

		HAL_Delay(2000); // Ana bekleme süresi

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
