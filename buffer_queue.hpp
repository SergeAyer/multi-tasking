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
        _consumerLed(YELLOW_LED) {
        // initialize random seed
        srand(time(NULL));

        _producerLed = LED_OFF;
        _consumerLed = LED_OFF;
    }

    void append(uint32_t datum) {
        _producerLed = LED_ON;
        uint32_t datum_uint32t = datum;
        _producerConsumerQueue.try_put_for(Kernel::wait_for_u32_forever, (uint32_t*) datum); 
        wait_us(computeRandomWaitTime(kApppendWaitTime));
        _producerLed = LED_OFF;
    }

    uint32_t extract(void) {
        _consumerLed = LED_ON;
        wait_us(computeRandomWaitTime(kExtractWaitTime));
        uint32_t datum = 0;
        _producerConsumerQueue.try_get_for(Kernel::wait_for_u32_forever, (uint32_t**) &datum);  
        _consumerLed = LED_OFF;
        return datum;
    }

    int computeRandomWaitTime(const std::chrono::microseconds &waitTime) {
        return rand() % waitTime.count() + waitTime.count();
    }

    int count() {
        return _producerConsumerQueue.count() - 1;
    }

private:
    static constexpr uint8_t kBufferSize = 10;
    static const std::chrono::microseconds kApppendWaitTime;
    static const std::chrono::microseconds kExtractWaitTime;
    DigitalOut _producerLed;
    DigitalOut _consumerLed;
    Queue<uint32_t, kBufferSize> _producerConsumerQueue;
};

} // namespace multi_tasking
