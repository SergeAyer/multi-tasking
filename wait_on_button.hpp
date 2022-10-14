#pragma once

#include "mbed.h"

#include "mbed_trace.h"

#if MBED_CONF_MBED_TRACE_ENABLE
#define TRACE_GROUP "WaitOnButton"
#endif // MBED_CONF_MBED_TRACE_ENABLE
namespace multi_tasking {

#if defined(TARGET_DISCO_L475VG_IOT01A)
#define PUSH_BUTTON BUTTON1
#define POLARITY_PRESSED 0
#endif

class WaitOnButton 
{
public:
    WaitOnButton(const char* threadName) :
        _thread(osPriorityBelowNormal, OS_STACK_SIZE, nullptr, threadName),
        _pushButton(PUSH_BUTTON) 
    {
        _pushButton.fall(callback(this, &WaitOnButton::buttonPressed));
        _timer.start();
        _pressedTime = std::chrono::microseconds::zero();
    }

    void start() 
    {
        osStatus status = _thread.start(callback(this, &WaitOnButton::waitForButtonEvent));
        tr_debug("Thread %s started with status %d\n", _thread.get_name(), status);
    }

    void wait() {
        _thread.join();
    }

private:
    void waitForButtonEvent() 
    {
        while (true) {
            tr_debug("Waiting for button press\n");
            _eventFlags.wait_all(kPressedEventFlag);
            std::chrono::microseconds time = _timer.elapsed_time();
            std::chrono::microseconds latency = time - _pressedTime;
            tr_debug("Button pressed with response time: %lld usecs\n", latency.count());
        }
    }

    void buttonPressed() {    
        _pressedTime = _timer.elapsed_time();
        _eventFlags.set(kPressedEventFlag);
    }

    static constexpr uint8_t kPressedEventFlag = (1UL << 0);
    Thread _thread;
    Timer _timer;
    std::chrono::microseconds _pressedTime;
    EventFlags _eventFlags;
    InterruptIn _pushButton;
};

} // namespace multi_tasking