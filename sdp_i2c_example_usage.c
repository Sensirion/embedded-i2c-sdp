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
#include <inttypes.h>

/*
 * TO USE CONSOLE OUTPUT (PRINTF) YOU MAY NEED TO ADAPT THE INCLUDE ABOVE OR
 * DEFINE IT ACCORDING TO YOUR PLATFORM:
 * #define printf(...)
 */

int main(void) {
    int16_t error = 0;

    sensirion_i2c_hal_init();

    // Change the used i2c address 'SDP_I2C_ADDRESS' in sdp_i2c.h
    printf("Launching SDP driver with I2C address: 0x%x\n", SDP_I2C_ADDRESS);

    sdp_stop_continuous_measurement();
    // ignore error if no measurement running

    uint32_t product_number;
    uint8_t serial[8];
    uint8_t serial_size = 8;

    error = sdp_prepare_product_identifier();
    if (error) {
        printf("Error executing sdp_prepare_product_identifier(): %i\n", error);
    }

    error = sdp_read_product_identifier(&product_number, serial, serial_size);

    if (error) {
        printf("Error executing sdp_read_product_identifier(): %i\n", error);
    } else {
        // uint64_t serial_number =
        //    (uint64_t)serial[0] << 56 | (uint64_t)serial[1] << 48 |
        //    (uint64_t)serial[2] << 40 | (uint64_t)serial[3] << 32 |
        //    (uint64_t)serial[4] << 24 | (uint64_t)serial[5] << 16 |
        //    (uint64_t)serial[6] << 8 | (uint64_t)serial[7];
        printf("Product Number: 0x%08x\n", product_number);
        // printf("Serial Number: %" PRIu64 "\n", serial_number);
    }

    // Start Measurement
    error =
        sdp_start_continuous_measurement_with_diff_pressure_t_comp_and_averaging();
    if (error) {
        printf("Error executing "
               "sdp_start_continuous_measurement_with_diff_pressure_t_comp_"
               "and_averaging(): %i\n",
               error);
    }

    for (int i = 0; i < 10000; i++) {
        // Read Measurement
        sensirion_i2c_hal_sleep_usec(10000);

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