/*
rpxxxx_am2320.h
Header file for AM2320 driver compatible with pico c sdk.

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
*/

#ifndef RPXXXX_AM2320_H
#define RPXXXX_AM2320_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Send dummy byte to wake up AM2320 sensor
// Assumes that i2c initialization is already done (in the first 500ms after AM2320 receives power)
// Includes 2ms sleep to wait for AM2320 sensor to wake up.
void rpxxxx_am2320_wake(i2c_inst_t *i2c);

// Send read request and wait for AM2320 sensor to send data (temperature and humidity), via i2c
// Assumes that i2c initialization is already done (in the first 500ms after AM2320 receives power)
// Includes 2ms sleep to wait for AM2320 sensor to wake up.
// byte 0-1 i2c header
// byte 2-3 temperature
// byte 4-5 humidity
// byte 6-7 crc
// Buffer length must be at least 8 bytes
// Returns Number of bytes written, or PICO_ERROR_GENERIC if address not acknowledged, no device present (same as i2c_read_blocking())
int rpxxxx_am2320_read_blocking_raw(i2c_inst_t *i2c, uint8_t *buffer);

// Call rpxxxx_am2320_read_blocking_raw, and parse output
// Temperature is in degrees Celsius with 0.1°C precision
// Humidity is in % with 0.1% precision
// Return 1 if crc in buffer matches calculated crc and i2c header is correct
// Return 0 if data is corrupted or if i2c header is not correct
int rpxxxx_am2320_convert(float *temperature, float *humidity, uint8_t *raw_data);

// Calculate crc for given data (CRC-16-IBM)
uint16_t rpxxxx_am2320_calc_crc(const uint8_t *data, uint8_t len);

#endif
