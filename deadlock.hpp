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
 * @file deadlock.hpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief Deadlock class used for demonstrating deadlock
 * @date 2022-09-01
 * @version 0.1.0
 ***************************************************************************/

#pragma once

#include "mbed.h"

#include "mbed_trace.h"
#if MBED_CONF_MBED_TRACE_ENABLE
#define TRACE_GROUP "Deadlock"
#endif // MBED_CONF_MBED_TRACE_ENABLE

namespace multi_tasking {

class Deadlock {
public:
    Deadlock(int index, const char *threadName) :
        _index(index),
        _thread(osPriorityNormal, OS_STACK_SIZE, nullptr, threadName) {
    }

    void start() {
        osStatus status = _thread.start(callback(this, &Deadlock::execute));
        tr_debug("Thread %s started with status %d", _thread.get_name(), status);
    }

    void wait() {
        _thread.join();
    }

private:
    void execute() {
        // enter the first critical section
        _mutex[_index].lock();
        tr_debug("Thread %d entered critical section %d", _index, _index);

        // perform some operations
        wait_us(kProcessingWaitTime.count());
        tr_debug("Thread %d processing in mutex %d done", _index, _index);

        // enter the second critical section
        int secondIndex = (_index + 1) % kNbrOfMutexes;
        tr_debug("Thread %d trying to enter critical section %d", _index, secondIndex);
        _mutex[secondIndex].lock();
        tr_debug("Thread %d entered critical section %d", _index, secondIndex);

        // perform some operations
        wait_us(kProcessingWaitTime.count());
        tr_debug("Thread %d processing in mutex %d and %d done", 
                 _index, _index, secondIndex);

        // exit the second critical section
        _mutex[secondIndex].unlock();
        
        // perform some operations
        wait_us(kProcessingWaitTime.count());
        tr_debug("Thread %d processing in mutex %d done", 
                 _index, _index);

        // exit the first critical section
        _mutex[_index].unlock();
    }

    // time that the threads should spend processing (e.g. wait in our case)
    static constexpr std::chrono::microseconds kProcessingWaitTime = 1000000us;
    static constexpr int kNbrOfMutexes = 2;
    int _index;
    Thread _thread;
    // the mutex must be declared as static for being a class instance
    static Mutex _mutex[kNbrOfMutexes];
};

} // namespace multi_tasking