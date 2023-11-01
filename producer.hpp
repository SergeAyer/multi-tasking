#pragma once

#include "mbed_trace.h"

#include "buffer_solution.hpp"

#if MBED_CONF_MBED_TRACE_ENABLE
#undef TRACE_GROUP
#define TRACE_GROUP "Producer"
#endif // MBED_CONF_MBED_TRACE_ENABLE

namespace multi_tasking {

class Producer {
public:
    explicit Producer(Buffer &buffer) :
        _buffer(buffer),
        _producerThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "ProducerThread") {
        // initialize random seed
        srand(time(NULL));
    }

    void start() {
        _producerThread.start(callback(this, &Producer::producerMethod));
    }

    void wait() {
        _producerThread.join();
    }

private:
    int produce(void) {
        wait_us(_buffer.computeRandomWaitTime(kProduceWaitTime));
        // Produce a random number ranging from 0 to kMaxRandomValue - 1
        return rand() % kMaxRandomValue;
    }

    void producerMethod() {
        while (true) {
            int producerDatum = produce();
            _buffer.append(producerDatum);
            tr_debug("Producer datum is %d (index %d)", producerDatum, _buffer.count());
        }
    }

private:
    static const std::chrono::microseconds kProduceWaitTime;
    static constexpr uint8_t kMaxRandomValue = 20;
    Buffer &_buffer;
    Thread _producerThread;
};

} // namespace multi_tasking
