#include "mbed.h"

#include "priority_inversion.hpp"

#include "mbed_trace.h"
#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "main"
#endif // MBED_CONF_MBED_TRACE_ENABLE

// declare static variables
Mutex multi_tasking::PriorityInversion::_mutex;

int main()
{
    // use trace library for console output
    mbed_trace_init();

    tr_debug("Priority inversion program started");

    // create a PriorityInversion instance with normal priority
    multi_tasking::PriorityInversion priorityNormal(osPriorityNormal, "NPThread");
    priorityNormal.start();

    // wait for normal priority thread to enter the critical section
    wait_us(multi_tasking::PriorityInversion::kProcessingWaitTime.count() * 2);

    // create a PriorityInversion instance with higher priority
    multi_tasking::PriorityInversion priorityAboveNormal(osPriorityAboveNormal, "HPThread");
    priorityAboveNormal.start();

    // wait for both threads to terminate
    priorityNormal.wait();
    priorityAboveNormal.wait();

    return 0;
}