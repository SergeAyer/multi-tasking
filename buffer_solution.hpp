#pragma once

#include "mbed.h"

namespace multi_tasking {

#if defined(TARGET_DISCO_L475VG_IOT01A)
#define GREEN_LED LED2
#define YELLOW_LED LED1
#define LED_ON 1
#define LED_OFF 0
#endif

class Buffer {
public:
    Buffer() :
        _producerLed(GREEN_LED),
        _consumerLed(YELLOW_LED)
    {
        // initialize random seed
        srand(time(NULL));

        _producerLed = LED_OFF;
        _consumerLed = LED_OFF;
    }

    void append(int datum)
    {
        // make sure that we can produce without overflow
        _inSemaphore.acquire();
        
        // lock buffer
        _producerConsumerMutex.lock();

        _producerLed = LED_ON;
        _buffer[_index] = datum;
        _index++;

        // unlock buffer 
        _producerConsumerMutex.unlock();

        // tell that one element is available for consumer
        _outSemaphore.release();
        
        wait_us(computeRandomWaitTime(kApppendWaitTime));        
        _producerLed = LED_OFF;
    }

    int extract(void)
    {      
        // make sure that we can consume without underflow
        _outSemaphore.acquire();
        
        // lock buffer
        _producerConsumerMutex.lock();

        _consumerLed = LED_ON;
        _index--;
        wait_us(computeRandomWaitTime(kExtractWaitTime));
        int datum = _buffer[_index];
        
        // unlock buffer 
        _producerConsumerMutex.unlock();

        // tell that one element is available for producer
        _inSemaphore.release();
        
        _consumerLed = LED_OFF;
        
        return datum;
    }

    int computeRandomWaitTime(const std::chrono::microseconds &waitTime)
    {
        return rand() % waitTime.count() + waitTime.count();
    }

    int count()
    {
        return _index;
    }

private:
    static constexpr uint8_t kBufferSize = 10;
    static const std::chrono::microseconds kApppendWaitTime;
    static const std::chrono::microseconds kExtractWaitTime;
    DigitalOut _producerLed;
    DigitalOut _consumerLed;
    Mutex _producerConsumerMutex;
    Semaphore _outSemaphore {0};
    Semaphore _inSemaphore {kBufferSize - 1};
    uint8_t _buffer[kBufferSize] = {0};
    int _index = 0;
};

} // namespace multi_tasking
