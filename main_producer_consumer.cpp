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
 * @file main.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief main function for demonstrating Producer/Consumer
 * @date 2022-09-01
 * @version 0.1.0
 ***************************************************************************/

#include "buffer_solution.hpp"
#include "consumer.hpp"
#include "mbed.h"
#include "mbed_trace.h"
#include "producer.hpp"
#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "main"
#endif  // MBED_CONF_MBED_TRACE_ENABLE

// declare static variables
const std::chrono::microseconds multi_tasking::Buffer::kApppendWaitTime   = 500000us;
const std::chrono::microseconds multi_tasking::Buffer::kExtractWaitTime   = 50000us;
const std::chrono::microseconds multi_tasking::Producer::kProduceWaitTime = 500000us;
const std::chrono::microseconds multi_tasking::Consumer::kConsumeWaitTime = 50000us;

int main() {
    // use trace library for console output
    mbed_trace_init();

    tr_debug("Consumer producer program started");
    multi_tasking::Buffer buffer;
    multi_tasking::Producer producer(buffer);
    multi_tasking::Consumer consumer(buffer);

    producer.start();
    consumer.start();

    // wait for threads to terminate (will not)
    producer.wait();
    consumer.wait();

    return 0;
}
