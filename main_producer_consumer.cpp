#include "mbed.h"

#include "buffer.hpp"
#include "producer.hpp"
#include "consumer.hpp"

#include "mbed_trace.h"
#include <chrono>
#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "main"
#endif // MBED_CONF_MBED_TRACE_ENABLE

// declare static variables
const std::chrono::microseconds multi_tasking::Buffer::kApppendWaitTime = 500000us;
const std::chrono::microseconds multi_tasking::Buffer::kExtractWaitTime = 500000us;
const std::chrono::microseconds multi_tasking::Producer::kProduceWaitTime = 500000us;
const std::chrono::microseconds multi_tasking::Consumer::kConsumeWaitTime = 500000us;

int main()
{
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