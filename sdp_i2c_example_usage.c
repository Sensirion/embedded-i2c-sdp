/*
 * I2C-Generator: 0.2.0
 * Yaml Version: 0.1.0
 * Template Version: 0.7.0-32-g5b8f0ff
 */
/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>  // printf

#include "sdp_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"

/*
 * TO USE CONSOLE OUTPUT (PRINTF) YOU MAY NEED TO ADAPT THE INCLUDE ABOVE OR
 * DEFINE IT ACCORDING TO YOUR PLATFORM:
 * #define printf(...)
 */

// TODO: DRIVER_GENERATOR Add missing commands and make prints more pretty

int main(void) {
    int16_t error = 0;

    sensirion_i2c_hal_init();

    uint32_t product_number;
    uint8_t serial_number[8];
    uint8_t serial_number_size = 8;

    error = sdp_read_product_identifier(&product_number, serial_number,
                                        serial_number_size);

    if (error) {
        printf("Error executing sdp_read_product_identifier(): %i\n", error);
    } else {
        printf("Product number: %u\n", product_number);
        printf("Serial number: ");
        for (size_t i = 0; i < serial_number_size; i++) {
            printf("%u, ", serial_number[i]);
        }
        printf("\n");
    }

    // Start Measurement

    for (;;) {
        // Read Measurement
        // TODO: DRIVER_GENERATOR check and update measurement interval
        sensirion_i2c_hal_sleep_usec(1000000);
        // TODO: DRIVER_GENERATOR Add scaling and offset to printed measurement
        // values

        int16_t differential_pressure;
        int16_t temperature;
        int16_t scaling_factor;

        error = sdp_read_measurement(&differential_pressure, &temperature,
                                     &scaling_factor);

        if (error) {
            printf("Error executing sdp_read_measurement(): %i\n", error);
        } else {
            printf("Differential pressure: %i\n", differential_pressure);
            printf("Temperature: %i\n", temperature);
            printf("Scaling factor: %i\n", scaling_factor);
        }
    }

    return 0;
}
