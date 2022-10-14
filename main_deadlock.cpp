#include "mbed.h"

#include "deadlock.hpp"

#include "mbed_trace.h"
#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "main"
#endif // MBED_CONF_MBED_TRACE_ENABLE

// declare static variables
Mutex multi_tasking::Deadlock::_mutex[kNbrOfMutexes];

// main() runs in its own thread in the OS
int main()
{
    // use trace library for console output
    mbed_trace_init();

    tr_debug("Deadlock program started\n");

    // create a first deadlock instance
    multi_tasking::Deadlock deadlock0(0, "Thread0");
    deadlock0.start();

    // create a second deadlock instance
    multi_tasking::Deadlock deadlock1(1, "Thread1");
    deadlock1.start();

    // wait for both threads to terminate (will not because of deadlock)
    deadlock0.wait();
    deadlock1.wait();

    return 0;
}