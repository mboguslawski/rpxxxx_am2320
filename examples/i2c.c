/*
i2c.c
Example usage of AM2320 i2c driver.

Copyright (C) 2025 Mateusz Bogusławski, E: mateusz.boguslawski@ibnet.pl

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see https://www.gnu.org/licenses/.

Connections:
AM2320   RPxxxx
 GND  <->  GND
 VCC  <->  +3.3V
 SCL  <-> GPIO 5 (attach 10k pull-up resistor)
 SDA  <-> GPIO 4 (attach 10k pull-up resistor)

In CMakeLists.txt:
- Add rpxxxx_am2320 directory with:
	add_subdirectory(lib/rpxxxx_am2320/src)
- Add rpxxxx_am2320 library:
	add "rpxxxx_am2320" entry inside target_link_libraries()
- Make sure USB is enabled (to see output in serial monitor)
	pico_enable_stdio_usb(temp2320 1)

Expected behaviour:
Every 500ms temperature value in 'C and humidity in % will be send via usb (and printed in serial monitor).
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "rpxxxx_am2320.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5
#define SLEEP_MS 500

int main() {
    stdio_init_all();

    // Initializes i2c
    i2c_init(I2C_PORT, 100 * 1000);  // 100kHz I2C
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    sleep_ms(3000);  // Wait for USB serial to be ready
    printf("AM2320 example using i2c\n");

    uint8_t buffer[8];
    float temperature;
    float humidity;

    while (true) {
        sleep_ms(SLEEP_MS);

        // Read raw data from sensor
        if (rpxxxx_am2320_read_blocking_raw(I2C_PORT, buffer) <= 0) {
            printf("Read failed!\n");
            continue;
        }

        // Convert raw data temperature in 'C and humidity in %
        if (!rpxxxx_am2320_convert(&temperature, &humidity, buffer)) {
            printf("Data corrupted!\n");
            continue;
        }

        printf("Temperature: %.1f'C\n", temperature);
        printf("Humidity: %.1f%%\n", humidity);
    }

    return 0;
}
