#include "mbed.h"

#include "clock.hpp"

#include "mbed_trace.h"
#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "main"
#endif // MBED_CONF_MBED_TRACE_ENABLE

int main()
{    
    // use trace library for console output
    mbed_trace_init();

    tr_debug("Clock update program started");

    // create and start a clock
    multi_tasking::Clock clock;
    clock.start();

    return 0;
}
