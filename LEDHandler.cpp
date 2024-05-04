#include "LEDHandler.h"

#include <iostream>

#define GPIO_PIN 18
#define LED_COUNT 16
#define STRIP_TYPE WS2811_STRIP_GRB
#define TARGET_FREQ WS2811_TARGET_FREQ
#define DMA 10

LEDHandler::LEDHandler() : effectRunning {false} {
    this->ledstring = {
        .freq = TARGET_FREQ,
        .dmanum = DMA,
        .channel =
            {
                [0] =
                    {
                        .gpionum = GPIO_PIN,
                        .invert = 0,
                        .count = LED_COUNT,
                        .strip_type = STRIP_TYPE,
                        .brightness = 255,
                    },
                [1] =
                    {
                        .gpionum = 0,
                        .invert = 0,
                        .count = 0,
                        .brightness = 0,
                    },
            },
    };
}

LEDHandler::~LEDHandler() { ws2811_fini(&this->ledstring); }

void LEDHandler::clear() {
    for (int i = 0; i < LED_COUNT; i++) {
        this->ledstring.channel[0].leds[i] = 0x00000000;  // off
    }
    ws2811_render(&this->ledstring);
}

void LEDHandler::ok() {
    int pattern[] = {15, 2, 6, 7, 8, 9, 10, 11};
    for (int i = 0; i < LED_COUNT; i++) {
        this->ledstring.channel[0].leds[i] = 0x00000000;  // off
    }
    for (int i : pattern) {
        this->ledstring.channel[0].leds[i] = 0x00002020;  // white
    }
    ws2811_render(&this->ledstring);
}

void LEDHandler::error() {
    for (int i = 0; i < LED_COUNT; i++) {
        this->ledstring.channel[0].leds[i] = 0x00200000;  // red
    }
    ws2811_render(&this->ledstring);
}

void LEDHandler::init() {
    ws2811_return_t ret;
    if ((ret = ws2811_init(&this->ledstring)) != WS2811_SUCCESS) {
        std::cout << "ws2811_init failed: " << ws2811_get_return_t_str(ret)
                  << std::endl;
    }

    clear();
}


void LEDHandler::waitingEffect() {
    int start = 0;
    int length = 7;
    while(effectRunning) {
        for (int i = 0; i < LED_COUNT; i++) {
            this->ledstring.channel[0].leds[i] = 0x00000000;
        }
        uint32_t color = 0x00000000;
        for (int i=0; i<length; i++){
            int index = (start + i) % LED_COUNT;
            color = color + (255 / length);
            this->ledstring.channel[0].leds[index] = color;
        }
        ws2811_render(&this->ledstring);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        start = (start + 1) % LED_COUNT;
    }
    clear();
}

void LEDHandler::startWaiting() {
    if(!effectRunning) {
        effectRunning = true;
        effectThread = std::thread(&LEDHandler::waitingEffect, this);
    }
}

void LEDHandler::stopWaiting() {
    effectRunning = false;
    if (this->effectThread.joinable()) {
        this->effectThread.join();
    }
}