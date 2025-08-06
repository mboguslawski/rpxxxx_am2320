/*
rpxxxx_am2320.c
Implementation of AM2320 driver compatible with pico c sdk.

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

#include "rpxxxx_am2320.h"

#define AM2320_ADDR 0x5C
#define CMD_READ 0x03
#define TEMP_REG 0x00

void rpxxxx_am2320_wake(i2c_inst_t *i2c) {
	uint8_t dummy = 0xAA; // 10101010
	int ret = i2c_write_blocking(i2c, AM2320_ADDR, &dummy, 1, true);
	sleep_ms(2);  // Wait >1.5ms for sensor to wake up
}

int rpxxxx_am2320_read_blocking_raw(i2c_inst_t *i2c, uint8_t *buffer) {
	rpxxxx_am2320_wake(i2c);
	
	uint8_t cmd[3] = {CMD_READ, TEMP_REG, 0x04}; // Read 4 bytes starting at 0x00
	int ret = i2c_write_blocking(i2c, AM2320_ADDR, cmd, 3, false);
	
	if (ret < 0) return false;
	
	// Read 8 bytes in total
	// 2 bytes for temperature, 2 bytes for humidity and 4 bytes for CRC
	ret = i2c_read_blocking(i2c, AM2320_ADDR, buffer, 8, false);
	uint64_t t1 = time_us_64();

	return ret;
}

int rpxxxx_am2320_convert(float *temperature, float *humidity, uint8_t *raw_data) {
	// Check i2c header
	if (raw_data[0] != CMD_READ || raw_data[1] != 0x04) {
		return 0;
	}
	
	// Verify CRC (last two bytes)
    int16_t crc_calculated = rpxxxx_am2320_calc_crc(raw_data, 6); // Do not include received crc in calculations
    int16_t crc_received = raw_data[7] << 8 | raw_data[6];
    
	if (crc_calculated != crc_received) {
       return 0;
	}
    
	// Convert temperature
	uint16_t t = ((raw_data[4] << 8) | raw_data[5]);
	// Most significant is set when temperature is negative
	(*temperature) = (t & 0x8000) ? (-1 * (float)((t & 0x7FFF) / 10.0f)) : ((float)t / 10.0f);

	// Convert humidity
	(*humidity) = (float)(raw_data[2] << 8 | raw_data[3]) / 10.0f;

	return 1;
}

uint16_t rpxxxx_am2320_calc_crc(const uint8_t *data, uint8_t len) {
    // CRC16-IBM calculation
	uint16_t crc = 0xFFFF;
    
	for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}
