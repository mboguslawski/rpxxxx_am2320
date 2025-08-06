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
