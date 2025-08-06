# AM2320 temperature and humidity sensor driver dedicated for RPxxxx microcontrollers.

### Features
- Temperature and humidity read and parsing to celcious degrees (temperature) or % (humidity)
- Calculating and chceckong CRC16-IBM checksum to ensure data correctness.
- Currently this driver supports communication only via i2c bus.

---

### API
- **void rpxxxx_am2320_wake(i2c_inst_t\*)** \
AM2320 sensors enters low power mode to save energy. In order for it to be able to communicate, it must leave low power mode. It is done by sending dummy byte to the sensor. Above method sends dummy byte via i2c port passed as function argument and waits 2ms (time for AM2320 sensor to fully wake up).

- **int rpxxxx_am2320_read_blocking_raw(i2c_inst_t\*, uint8_t\*)** \
Sends temperature and humidity read request to AM2320 via i2c port passed as function arguments. Then waits till data from sensor is available and stores it in buffer, which is passed as the uint8_t*. Returns number of bytes written, or PICO_ERROR_GENERIC if address not acknowledged, no device present (same as i2c_read_blocking() from pico c sdk).

- **int rpxxxx_am2320_convert(float\*, float\*, uint8_t\*);** \
Parse raw data passed as uint8_t* to get temperature as float in celcious degrees and humidity in %.

- **uint16_t rpxxxx_am2320_calc_crc(const uint8_t\*, uint8_t);**
Calculates and returns CRC16-IBM (the same that is calculated and send by AM2320) from data passed as uint8* and length passed as uint8_t.

---
### Quickstart
Go to [examples](./examples/) directory to try driver yourself.

---
### Connecting to microcontroller

- **i2c**

| AM2320 | RPxxxx | Comments |
|--------|--------|----------|
| GND   |  GND   | None|
| VCC | +3.3V| None |
| SCL | Chosen pin that can function as SCL in chosen i2c port | Connect external 10k pull-up resistor |
|SDA|Chosen pin that can function as SDA in chosen i2c port| Connect external 10k pull-up resistor |
---

### Author
Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl

---

Feel free to ask questions, and contribute to this driver development!
