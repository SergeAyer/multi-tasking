#include "mbed.h"

#include "wait_on_button.hpp"

#include "mbed_trace.h"
#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "main"
#endif // MBED_CONF_MBED_TRACE_ENABLE

#include "wait_on_button.hpp"

// main() runs in its own thread in the OS
int main()
{
    // use trace library for console output
    mbed_trace_init();

    tr_debug("EventFlags program started\n");

    // create the WaitOnButton instance and start it
    multi_tasking::WaitOnButton waitOnButton("ButtonThread");
    waitOnButton.start();
  
    // wait for the thread to exit (will not because of infinite loop)
    waitOnButton.wait();
    // or do busy waiting
    //while (true) {
    //}
}

