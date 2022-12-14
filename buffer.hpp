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

    void append(uint32_t datum)
    {
        _producerLed = LED_ON;
        _buffer[_index] = datum;
        _index++;
        wait_us(computeRandomWaitTime(kApppendWaitTime));
        _producerLed = LED_OFF;
    }

    uint32_t extract(void)
    {
        _consumerLed = LED_ON;
        _index--;
        wait_us(computeRandomWaitTime(kExtractWaitTime));
        int datum = _buffer[_index];
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
    uint32_t _buffer[kBufferSize] = {0};
    int _index = 0;
};

} // namespace multi_tasking
