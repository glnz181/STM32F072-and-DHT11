#ifndef __DHT_H
#define __DHT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"
#include "tim.h"
#include "i2c_lcd.h"

// DHT Veri Yapısı
typedef struct {
	float Temperature;
	float Humidity;
} DHT_DataTypedef;

// DHT Durum Kodları
typedef enum {
	DHT_OK = 0, DHT_NO_RESPONSE, DHT_CHECKSUM_ERROR, DHT_TIMEOUT_ERROR
} DHT_Status;

// DHT Pin Tanımlamaları
#define DHT_PORT GPIOA
#define DHT_PIN GPIO_PIN_0

// Fonksiyon Prototipleri
void DHT_Start(void);
int8_t DHT_Check_Response(void);
uint8_t DHT_Read(void);
DHT_Status DHT_GetData(DHT_DataTypedef *DHT_Data);

// Yardımcı Fonksiyonlar
void delay_us(uint32_t microseconds);
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

// External değişkenler
extern I2C_LCD_HandleTypeDef lcd_handle;
extern TIM_HandleTypeDef htim6;

#ifdef __cplusplus
}
#endif

#endif /* __DHT_H */
