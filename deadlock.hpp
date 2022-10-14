#pragma once

#include "mbed_trace.h"

#if MBED_CONF_MBED_TRACE_ENABLE
#define TRACE_GROUP "Deadlock"
#endif // MBED_CONF_MBED_TRACE_ENABLE

namespace multi_tasking {

class Deadlock {
public:
    Deadlock(int index, const char *threadName) :
        _index(index),
        _thread(osPriorityNormal, OS_STACK_SIZE, nullptr, threadName)
    {
    }

    void start()
    {
        osStatus status = _thread.start(callback(this, &Deadlock::execute));
        tr_debug("Thread %s started with status %d\n", _thread.get_name(), status);
    }

    void wait()
    {
        _thread.join();
    }

private:
    void execute()
    {
        // enter the first critical section
        _mutex[_index].lock();
        printf("Thread %d entered critical section %d\n", _index, _index);

        // perform some operations
        wait_us(kProcessingWaitTime.count());
        printf("Thread %d processing in mutex %d done\n", _index, _index);

        // enter the second critical
        int secondIndex = (_index + 1) % kNbrOfMutexes;
        _mutex[secondIndex].lock();
        printf("Thread %d entered critical section %d\n", _index, secondIndex);

        // perform some operations
        wait_us(kProcessingWaitTime.count());
        printf("Thread %d processing in mutex %d and %d done\n", _index, _index, secondIndex);

        // exit critical sections
        _mutex[secondIndex].unlock();
        _mutex[_index].unlock();
    }

    // time that the threads should spend processing (e.g. wait in our case)
    static constexpr std::chrono::microseconds kProcessingWaitTime = 1000000us;
    static constexpr int kNbrOfMutexes = 2;
    int _index;
    Thread _thread;
    // the mutex must be declared as static for being a class instance
    static Mutex _mutex[kNbrOfMutexes];
};

} // namespace multi_tasking