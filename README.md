# STM32F072 - Temperature & Humidity Monitoring with DHT11 and I2C LCD 🌡️💧📟

This project demonstrates how to interface a **DHT11 temperature and humidity sensor** and display the readings on an **I2C LCD** using the **STM32F072** microcontroller. It's a basic embedded systems project focused on sensor reading, I2C communication, and LCD visualization.

## 🛠️ Hardware Components

- STM32F072 Development Board
- DHT11 Temperature & Humidity Sensor
- I2C LCD Display (16x2 or 20x4 with PCF8574 backpack)
- Breadboard and jumper wires
- (Optional) USB-to-Serial adapter for debugging

## ⚙️ Software & Tools Used

- STM32CubeIDE
- STM32 HAL Libraries
- Custom DHT11 driver
- I2C LCD driver (`lcd_i2c.c/.h`)
- (Optional) Tera Term / PuTTY for UART debugging

## 🔌 Connections

### DHT11 Sensor

| DHT11 Pins | STM32F072 Pins |
|------------|----------------|
| VCC        | 3.3V           |
| GND        | GND            |
| DATA       | PA1 (example)  |

### I2C LCD Display

| LCD Pins | STM32F072 Pins |
|----------|----------------|
| SDA      | PB7 (example)  |
| SCL      | PB6 (example)  |

> Note: I2C LCD address is usually `0x27` or `0x3F`. Make sure to configure it correctly in your code.

## 📋 Project Description

- On boot, the STM32F072 reads temperature and humidity data from the DHT11 sensor.
- The data is then displayed on the LCD via I2C in the following format:
- Measurements are updated periodically (e.g., every 2 seconds).

## 📁 Project Structure
stm32f072-dht11-lcd/
│
├── Core/ # STM32CubeIDE source files (main.c, gpio.c, i2c.c, etc.)
├── Drivers/ # HAL libraries and custom drivers
│ ├── dht11.c/.h # DHT11 sensor driver
│ └── lcd_i2c.c/.h # I2C LCD display driver
├── README.md # This file
## 🚀 Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/stm32f072-dht11-lcd.git

