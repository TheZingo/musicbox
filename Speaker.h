#ifndef INCLUDE_SPEAKER_H_
#define INCLUDE_SPEAKER_H_

#include<cstdint>

typedef struct _bluetoothMac {
  union {
    unsigned long long ullLong;
    uint8_t    bytes[6];
  };
} BluetoothMAC;

class Speaker {

    private:
        BluetoothMAC address;
    public:
        Speaker(BluetoothMAC address);
        
};

#endif