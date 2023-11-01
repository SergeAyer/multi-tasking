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
 * @brief main function for demonstrating Priority Inversion
 * @date 2022-09-01
 * @version 0.1.0
 ***************************************************************************/

#include "mbed.h"
#include "mbed_trace.h"
#include "priority_inversion.hpp"
#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "main"
#endif  // MBED_CONF_MBED_TRACE_ENABLE

// declare static variables
Mutex multi_tasking::PriorityInversion::_mutex;

int main() {
    // use trace library for console output
    mbed_trace_init();

    tr_debug("Priority inversion program started");

    // create a PriorityInversion instance with normal priority
    multi_tasking::PriorityInversion priorityNormal(osPriorityNormal, "NPThread");
    priorityNormal.start();

    // wait for normal priority thread to enter the critical section
    wait_us(multi_tasking::PriorityInversion::kProcessingWaitTime.count() * 2);

    // create a PriorityInversion instance with higher priority
    multi_tasking::PriorityInversion priorityAboveNormal(osPriorityAboveNormal,
                                                         "HPThread");
    priorityAboveNormal.start();

    // wait for both threads to terminate
    priorityNormal.wait();
    priorityAboveNormal.wait();

    return 0;
}
