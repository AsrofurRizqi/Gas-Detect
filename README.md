## ESP8266 Gas Detector

Projek ini adalah contoh penggunaan ESP8266 untuk mendeteksi gas berdasarkan nilai dari sensor MQ-135. Nilai akan ditampilkan pada oled display dengan kalibrasi yang sudah ditentukan. data kalibrasi didapatkan dari sensor suhu ruangan pada RTC DS3231. Ketika nilai gas melebihi batas yang ditentukan maka relay akan aktif.

## Komponen
- ESP8266
- MQ-135
- OLED Display (SSD1306)
- Relay
- RTC DS3231

## Library
- MQ135
- Adafruit_GFX
- Adafruit_SSD1306
- Wire
- RTClib

## Preview