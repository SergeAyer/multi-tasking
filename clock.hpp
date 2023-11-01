#pragma once

#include "mbed.h"

namespace multi_tasking {

class Clock {
public:
    struct DateTimeType {
        uint32_t day;
        uint32_t hour;
        uint32_t minute;
        uint32_t second;
    };

    Clock() {
        // initialize current time
        _currentTime.day = 0;
        _currentTime.hour = 10;
        _currentTime.minute = 59;
        _currentTime.second = 59;
    }

    void start() {
        // start a ticker thread for dispatching events that are queued in the tickerUpdate() method
        _tickerThread.start(callback(&_tickerQueue, &EventQueue::dispatch_forever));
        // call the tickerUpdate() method every second, for queueing an event to be dispatched by the ticker thread
        _ticker.attach(callback(this, &Clock::tickerUpdate), clockUpdateTimeout);

        // schedule an event every second for displaying the time on the console
        _clockDisplayQueue.call_every(clockDisplayTimeout, 
                                      callback(this, &Clock::getAndPrintDateTime));
        // dispatch events from the thread calling the start() method (main thread)
        _clockDisplayQueue.dispatch_forever();
    }

private:
    void getAndPrintDateTime() {
        DateTimeType dt = {0};

        dt.day = _currentTime.day;
        dt.hour = _currentTime.hour;
        
        static constexpr std::chrono::microseconds waitTime = 1s;
        wait_us(waitTime.count());

        dt.minute = _currentTime.minute;
        dt.second = _currentTime.second;

        printf("Day %u Hour %u min %u sec %u\n", dt.day, dt.hour, dt.minute, dt.second);
    }

    void tickerUpdate() {
        _tickerQueue.call(callback(this, &Clock::updateCurrentTime));
    }

    void updateCurrentTime() {
        _currentTime.second += std::chrono::duration_cast<std::chrono::seconds>(
          clockUpdateTimeout).count();

        if (_currentTime.second > 59) {
            _currentTime.second = 0;
            _currentTime.minute++;
            if (_currentTime.minute > 59) {
                _currentTime.minute = 0;
                _currentTime.hour++;
                if (_currentTime.hour > 23) {
                    _currentTime.hour = 0;
                    _currentTime.day++;
                }
            }
        }
    }

    EventQueue _clockDisplayQueue;
    Ticker _ticker;
    EventQueue _tickerQueue;
    Thread _tickerThread;
    DateTimeType _currentTime;
    static constexpr std::chrono::milliseconds clockUpdateTimeout = 1000ms;
    static constexpr std::chrono::milliseconds clockDisplayTimeout = 1000ms;
};

} // namespace multi_tasking