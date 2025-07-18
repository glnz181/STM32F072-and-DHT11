# STM32F072 ile DHT11 Sıcaklık-Nem Ölçümü ve I2C LCD Gösterimi 🌡️💧📟

Bu projede, **STM32F072** mikrodenetleyicisi kullanılarak **DHT11 sensöründen** sıcaklık ve nem verileri okunmuş ve bu veriler **I2C protokolü ile bağlı LCD ekranda** gösterilmiştir. Bu uygulama, temel sensör okuma, I2C haberleşme ve LCD ile veri görselleştirme gibi konuları kapsar.

## 🛠️ Kullanılan Donanımlar

- STM32F072 Geliştirme Kartı
- DHT11 Sıcaklık ve Nem Sensörü
- I2C LCD Ekran (16x2 veya 20x4, PCF8574 modülü ile)
- Breadboard ve jumper kablolar
- Gerekirse USB - UART (debug için)

## ⚙️ Kullanılan Yazılım ve Araçlar

- STM32CubeIDE
- HAL (Hardware Abstraction Layer) kütüphaneleri
- DHT11 sürücüsü
- I2C LCD sürücüsü (`lcd_i2c.c/.h`)
- (Opsiyonel) UART üzerinden seri monitör desteği

## 🔌 Donanım Bağlantıları

### DHT11 Bağlantısı

| DHT11 Pinleri | STM32F072 Pinleri |
|---------------|-------------------|
| VCC           | 3.3V              |
| GND           | GND               |
| DATA          | PA1 (örnek)       |

### LCD (I2C) Bağlantısı

| LCD Pinleri | STM32F072 Pinleri |
|-------------|-------------------|
| SDA         | PB7 (örnek)       |
| SCL         | PB6 (örnek)       |
> Not: LCD'nin I2C adresi genellikle `0x27` veya `0x3F` olabilir. Kod içinde buna dikkat edilmelidir.

## 🧪 Projenin Çalışması

1. STM32 başlatıldığında, DHT11 sensöründen sıcaklık ve nem verileri okunur.
2. Okunan değerler I2C ile bağlı olan LCD ekranın birinci ve ikinci satırında gösterilir:
3. Okumalar belirli aralıklarla (örneğin her 2 saniyede bir) yenilenir.

## 📦 Proje Dosya Yapısı

- `Core/` → STM32CubeIDE kaynak dosyaları (`main.c`, `gpio.c`, `i2c.c`, vs.)
- `Drivers/` → HAL ve kullanıcı sürücüleri (`dht11.c/.h`, `lcd_i2c.c/.h`)
- `README.md` → Bu dosya

## 📸 Ekran Görüntüsü (Opsiyonel)

İstersen LCD ekranın gösterdiği sıcaklık-nem verilerinin fotoğrafını veya bağlantı şemasını buraya ekleyebilirsin.

## 🚀 Kurulum ve Derleme

1. Bu repoyu klonlayın:
```bash
git clone https://github.com/kullanici-adi/stm32f072-dht11-lcd.git
