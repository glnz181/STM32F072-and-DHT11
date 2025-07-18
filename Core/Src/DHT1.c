#include "DHT1.h"

// Timer ve LCD handle'larını dışarıdan alıyoruz
extern TIM_HandleTypeDef htim6;
extern I2C_LCD_HandleTypeDef lcd_handle;

// Global değişkenler
uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM;

/**
 * @brief  Mikrosaniye cinsinden gecikme sağlar.
 * @param  microseconds: Gecikme süresi (mikrosaniye).
 * @retval None
 */
void delay_us(uint32_t microseconds) {
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim6)) < microseconds)
		;
}

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP; // DHT11 için pull-up gerekli
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DHT_Start(void) {
	Set_Pin_Output(DHT_PORT, DHT_PIN);
	HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_RESET);
	delay_us(18000); // 18ms
	HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_SET);
	delay_us(30); // 20-40us arası
	Set_Pin_Input(DHT_PORT, DHT_PIN);
}

int8_t DHT_Check_Response(void) {
	int8_t Response = 0;
	uint32_t timeout = 0;

	delay_us(40);

	if (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))) {
		delay_us(80);
		if ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))) {
			Response = 1;
		} else {
			Response = 0;
		}
	}

	// High sinyalinin bitmesini bekle (timeout ile)
	timeout = 0;
	while ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) && timeout < 100) {
		delay_us(1);
		timeout++;
	}

	return Response;
}

uint8_t DHT_Read(void) {
	uint8_t i = 0;
	uint8_t j;
	uint32_t timeout;

	for (j = 0; j < 8; j++) {
		// Low sinyalinin bitmesini bekle
		timeout = 0;
		while (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) && timeout < 100) {
			delay_us(1);
			timeout++;
		}

		delay_us(40);

		if (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))) {
			i &= ~(1 << (7 - j)); // 0 bit
		} else {
			i |= (1 << (7 - j));  // 1 bit
		}

		// High sinyalinin bitmesini bekle
		timeout = 0;
		while ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) && timeout < 100) {
			delay_us(1);
			timeout++;
		}
	}
	return i;
}

DHT_Status DHT_GetData(DHT_DataTypedef *DHT_Data) {
	int8_t Presence_check;

	DHT_Start();
	Presence_check = DHT_Check_Response();

	if (Presence_check == 1) {
		Rh_byte1 = DHT_Read();
		Rh_byte2 = DHT_Read();
		Temp_byte1 = DHT_Read();
		Temp_byte2 = DHT_Read();
		SUM = DHT_Read();

		// Checksum kontrolü
		if (SUM == (Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2)) {
			// DHT11 için sadece tam sayı kısımları kullanılır
			DHT_Data->Temperature = (float) Temp_byte1;
			DHT_Data->Humidity = (float) Rh_byte1;
			return DHT_OK;
		} else {
			// Checksum hatası
			DHT_Data->Temperature = 0.0f;
			DHT_Data->Humidity = 0.0f;
			return DHT_CHECKSUM_ERROR;
		}
	} else {
		// Sensör yanıt vermedi
		DHT_Data->Temperature = 0.0f;
		DHT_Data->Humidity = 0.0f;
		return DHT_NO_RESPONSE;
	}
}
