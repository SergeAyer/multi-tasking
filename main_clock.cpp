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
 * @brief main function for demonstrating Clock
 * @date 2022-09-01
 * @version 0.1.0
 ***************************************************************************/

#include "clock.hpp"
#include "mbed.h"
#include "mbed_trace.h"
#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "main"
#endif  // MBED_CONF_MBED_TRACE_ENABLE

int main() {
    // use trace library for console output
    mbed_trace_init();

    tr_debug("Clock update program started");

    // create and start a clock
    multi_tasking::Clock clock;
    clock.start();

    return 0;
}
