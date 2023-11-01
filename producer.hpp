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
 * @file producer.hpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief Producer class used for demonstrating producer/consumer
 * @date 2022-09-01
 * @version 0.1.0
 ***************************************************************************/

#pragma once

#include "mbed_trace.h"

#include "buffer_solution.hpp"

#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "Producer"
#endif // MBED_CONF_MBED_TRACE_ENABLE

namespace multi_tasking {

class Producer {
public:
    explicit Producer(Buffer &buffer) :
        _buffer(buffer),
        _producerThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "ProducerThread") {
        // initialize random seed
        srand(time(NULL));
    }

    void start() {
        _producerThread.start(callback(this, &Producer::producerMethod));
    }

    void wait() {
        _producerThread.join();
    }

private:
    int produce(void) {
        wait_us(_buffer.computeRandomWaitTime(kProduceWaitTime));
        // Produce a random number ranging from 0 to kMaxRandomValue - 1
        return rand() % kMaxRandomValue;
    }

    void producerMethod() {
        while (true) {
            int producerDatum = produce();
            _buffer.append(producerDatum);
            tr_debug("Producer datum is %d (index %d)", producerDatum, _buffer.count());
        }
    }

private:
    static const std::chrono::microseconds kProduceWaitTime;
    static constexpr uint8_t kMaxRandomValue = 20;
    Buffer &_buffer;
    Thread _producerThread;
};

} // namespace multi_tasking
