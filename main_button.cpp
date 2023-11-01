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
 * @brief main function for demonstrating WaitOnButton
 * @date 2022-09-01
 * @version 0.1.0
 ***************************************************************************/

#include "mbed.h"
#include "mbed_trace.h"
#include "wait_on_button.hpp"
#include "memory_logger.hpp"
#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "main"
#endif  // MBED_CONF_MBED_TRACE_ENABLE

int main() {
    // use trace library for console output
    mbed_trace_init();

    tr_debug("EventFlags program started\n");

    // log thread statistics
    advembsof::MemoryLogger memoryLogger;
    memoryLogger.getAndPrintThreadStatistics();

    // create the WaitOnButton instance and start it
    multi_tasking::WaitOnButton waitOnButton("ButtonThread");
    waitOnButton.start();

    // wait that the WaitOnButton thread started 
    waitOnButton.wait_started();

    // log thread statistics
    memoryLogger.getAndPrintThreadStatistics();

    // wait for the thread to exit (will not because of infinite loop)
    waitOnButton.wait_exit();
    // or do busy waiting
    // while (true) {
    //}

    return 0;
}
