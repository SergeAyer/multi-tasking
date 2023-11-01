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
 * @file priority_inversion.hpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief Consumer class used for demonstrating priority inversion
 * @date 2022-09-01
 * @version 0.1.0
 ***************************************************************************/

#pragma once

#include "mbed.h"

#include "mbed_trace.h"

#if MBED_CONF_MBED_TRACE_ENABLE
#define TRACE_GROUP "PriorityInversion"
#endif // MBED_CONF_MBED_TRACE_ENABLE

namespace multi_tasking {

class PriorityInversion {
public:    
    // time that the threads should spend processing (e.g. wait in our case)
    static constexpr std::chrono::microseconds kProcessingWaitTime = 1000000us;

    PriorityInversion(osPriority priority, const char *threadName) :
        _thread(priority, OS_STACK_SIZE, nullptr, threadName) {
    }

    void start() {
        osStatus status = 
        _thread.start(callback(this, &PriorityInversion::execute));
        tr_debug("Thread %s started with status %d", _thread.get_name(), status);
    }

    void wait() {
        _thread.join();
    }

private:
    void execute() {
        tr_debug("Thread %s about to start processing with priority %d", 
                 _thread.get_name(), _thread.get_priority());

        // perform some operations
        wait_us(kProcessingWaitTime.count());

        tr_debug("Thread %s about to enter critical section with priority %d", 
                 _thread.get_name(), _thread.get_priority());

        // enter the critical section
        _mutex.lock();

        tr_debug("Thread %s entered critical section, priority %d",
                 _thread.get_name(), _thread.get_priority());

        // perform some operations
        wait_us(kProcessingWaitTime.count());

        tr_debug("Thread %s about to exit critical section, priority %d", 
                 _thread.get_name(), _thread.get_priority());

        // exit the critical section
        _mutex.unlock();

        tr_debug("Thread %s exited critical section, priority %d",
                 _thread.get_name(), _thread.get_priority());
    }

    Thread _thread;
    // the mutex must be declared as static for being a class instance
    static Mutex _mutex;
};

} // namespace multi_tasking