#ifndef INCLUDE_RFIDHANDLER_H_
#define INCLUDE_RFIDHANDLER_H_

#include <string>
#include "thirdparty/MFRC522.h"

class RFIDHandler {

    private:
        MFRC522 mfrc;

    public:
        RFIDHandler();
        std::string waitForTag();
        void waitForTagRemoved();
};

#endif