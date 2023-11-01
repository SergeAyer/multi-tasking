#pragma once

#include "mbed_trace.h"

#include "buffer_solution.hpp"

#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "Consumer"
#endif // MBED_CONF_MBED_TRACE_ENABLE

namespace multi_tasking {

class Consumer {
public:
    explicit Consumer(Buffer &buffer) :
        _buffer(buffer),
        _consumerThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "ConsumerThread") {
    }

    void start() {
        _consumerThread.start(callback(this, &Consumer::consumerMethod));
    }

    void wait() {
        _consumerThread.join();
    }

private:

    void consume(int datum) {
        // does nothing
        wait_us(_buffer.computeRandomWaitTime(kConsumeWaitTime));
    }

    void consumerMethod(void) {
        while (true) {
            int consumerDatum = _buffer.extract();
            consume(consumerDatum);
            tr_debug("Consumer datum is %d (index %d)", consumerDatum, _buffer.count());
        }
    }

    int computeRandomWaitTime(const std::chrono::microseconds &waitTime) {
        return rand() % waitTime.count() + waitTime.count();
    }

private:
    static const std::chrono::microseconds kConsumeWaitTime;
    Buffer &_buffer;
    Thread _consumerThread;
};

} // namespace multi_tasking
