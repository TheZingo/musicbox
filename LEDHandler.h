#ifndef INCLUDE_LEDHANDLER_H_
#define INCLUDE_LEDHANDLER_H_

#include "ws2811.h"
#include <thread>
#include <atomic>

class LEDHandler {
    private:
        ws2811_t ledstring;
        std::thread effectThread;
        std::atomic<bool> effectRunning;
        void waitingEffect();

   public:
    LEDHandler();
    ~LEDHandler();
    void clear();
    void init();
    void ok();
    void error();
    void startWaiting();
    void stopWaiting();
};

#endif