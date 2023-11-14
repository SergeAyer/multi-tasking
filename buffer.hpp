// Copyright 2022 Haute école d'ingénierie et d'architecture de Fribourg
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/****************************************************************************
 * @file buffer.hpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief Buffer class used for demonstrating consumer/producer
 * @date 2022-09-01
 * @version 0.1.0
 ***************************************************************************/

#pragma once

#include "mbed.h"

namespace multi_tasking {

#if defined(TARGET_DISCO_H747I)
#define GREEN_LED LED1
#define BLUE_LED LED4
static constexpr uint8_t kLedOn = 0;
static constexpr uint8_t kLedOff = 1;
#endif

class Buffer {
public:
    Buffer() :
        _producerLed(GREEN_LED),
        _consumerLed(BLUE_LED) {
        // initialize random seed
        srand(time(NULL));

        _producerLed = kLedOff;
        _consumerLed = kLedOff;
    }

    void append(uint32_t datum) {
        _producerLed = kLedOn;
        _buffer[_index] = datum;
        _index++;
        wait_us(computeRandomWaitTime(kApppendWaitTime));
        _producerLed = kLedOff;
    }

    uint32_t extract(void) {
        _consumerLed = kLedOn;
        _index--;
        wait_us(computeRandomWaitTime(kExtractWaitTime));
        int datum = _buffer[_index];
        _consumerLed = kLedOff;
        return datum;
    }

    int computeRandomWaitTime(const std::chrono::microseconds &waitTime) {
        return rand() % waitTime.count() + waitTime.count();
    }

    int count() {
        return _index;
    }

private:
    static constexpr uint8_t kBufferSize = 10;
    static const std::chrono::microseconds kApppendWaitTime;
    static const std::chrono::microseconds kExtractWaitTime;
    DigitalOut _producerLed;
    DigitalOut _consumerLed;
    uint32_t _buffer[kBufferSize] = {0};
    int _index = 0;
};

} // namespace multi_tasking
