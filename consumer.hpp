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
 * @file consumer.hpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief Consumer class used for demonstrating producer/consumer
 * @date 2022-09-01
 * @version 0.1.0
 ***************************************************************************/

#pragma once

#include "mbed_trace.h"

#include "buffer_solution.hpp"

#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "Consumer"
#endif // MBED_CONF_MBED_TRACE_ENABLE

namespace multi_tasking {

class Consumer {
public:
    explicit Consumer(Buffer &buffer) :
        _buffer(buffer),
        _consumerThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "ConsumerThread") {
    }

    void start() {
        _consumerThread.start(callback(this, &Consumer::consumerMethod));
    }

    void wait() {
        _consumerThread.join();
    }

private:

    void consume(int datum) {
        // does nothing
        wait_us(_buffer.computeRandomWaitTime(kConsumeWaitTime));
    }

    void consumerMethod(void) {
        while (true) {
            int consumerDatum = _buffer.extract();
            consume(consumerDatum);
            tr_debug("Consumer datum is %d (index %d)", consumerDatum, _buffer.count());
        }
    }

    int computeRandomWaitTime(const std::chrono::microseconds &waitTime) {
        return rand() % waitTime.count() + waitTime.count();
    }

private:
    static const std::chrono::microseconds kConsumeWaitTime;
    Buffer &_buffer;
    Thread _consumerThread;
};

} // namespace multi_tasking
