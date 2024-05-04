# Music Box


The Music Box is a project that allows you to control a music player using RFID tags. By placing figures on the box, you can start playing the associated song, playlist or audio book. The playback is automatically stopped when removing the figure, providing a toddler-proof interface. You can find more information in the following article: [Please tell me more about Music Box](https://ptmm.de/about/musicbox)

## Features

* Control music playback using RFID-tagged figures
* Automatically start and stop playback when adding or removing figures
* Connect to a Bluetooth speaker for audio output
* Configurable through a local web interface
* Supports custom 3D printed figures

## How it Works
The Music Box project uses a Bluetooth speaker to play the audio. When an RFID-tagged figure is placed on the box, the associated song, playlist or audio book starts playing. Removing the figure stops the playback automatically. The project includes a local web interface for configuration, content and tag management, as well as additional playback controls. The interface is designed as a single-page application (SPA) for a smooth user experience. Users can create custom 3D printed figures and associate them with audio content. The figures contain an RFID tag that is detected by the Music Box, allowing for easy and intuitive interaction.
Content management, configuration and additional play controls are provided via a web interface running on port 18080.

## Settings 

A settings file in the root is used to store persistent settings. At the moment it is just used for the Bluetooth speaker device path:

```json
{
    "speaker": "/org/bluez/hci0/dev_88_C6_26_57_95_B9"
}
```

### Pair, trust & connect a Bluetooth speaker

```sh
systemctl --user start pulseaudio
bluetoothctl
>> scan on
```

bring speaker into pair mode and wait for it to show up
find the 48bit MAC address.

```sh
>> pair XX:XX:XX:XX:XX:XX
>> trust XX:XX:XX:XX:XX:XX
>> connect XX:XX:XX:XX:XX:XX
```

## Update the application

1. Stop the service
   ```sh
   systemctl --user stop musicbox
   ```
2. Copy the new executable into the user directory of user "music"
3. Grant the executable CAP_SYS_RAWIO capability to perform I/O operations in user context
   ```sh
   sudo setcap cap_sys_rawio+ep MusicBox
   ```
4. Start the service
   ```sh
   systemctl --user start musicbox
   ```

## Setup Raspberry Pi Zero

Prerequisite: Rasberry Pi OS with Wifi enabled and "music" as standard user with sudo permissions.

1. Boot Config
    ```sh
    sudo nano /boot/config.txt
    ```
    add in [all] section
    ```ini
    device_tree_param=spi=on
    dtoverlay=spi-bcm2708
    dtparam=act_led_trigger=none
    dtparam=act_led_activelow=on    
    ```
    change  ```dtparam=audio=on``` to ```dtparam=audio=off```.
    
    These settings are for the RFID reader, to turn off the onboard LEDs and deactivate onboard sound to use the pins for controlling the LED ring.
2. Enable SPI
    ```sh
    sudo raspi-config
    ```
    activate "Interface Options" > "SPI"
3. Install some additional packages
   ```sh
   sudo apt install pulseaudio-module-bluetooth libcap2
   ```
4. Assign some additional groups for the "music" user
   ```sh
   sudo usermod -a -G sudo music
   sudo usermod -a -G bluetooth music
   sudo usermod -a -G gpio music
   sudo usermod -a -G spi music
   sudo usermod -a -G kmem music
   sudo usermod -a -G video music   
   ```
5. Udev rule for allowing memory access as non-root user (for the RFID reader)
   ```sh
   echo 'SUBSYSTEM=="mem", KERNEL=="mem", GROUP="kmem", MODE="0660"' | sudo tee /etc/udev/rules.d/98-mem.rules
   ```
6. Enable auto start of the app in user mode
   ```sh
   loginctl enable-linger music
   mkdir ~/.config/systemd/user
   cp ~/app/source/service/musicbox.service ~/.config/systemd/user/
   systemctl --user enable musicbox.service
   ```
7. Enable and configure Pulse Audio
   ```sh
   sudo nano /usr/lib/systemd/user/pulseaudio.service
   ```
   ```ini
   #Restart=on-failure
   Restart=always
   ```
   ```sh
   systemctl --user enable pulseaudio
   ```

## Web Frontend

Prerequisites
* Node.js and npm installed on your system


### Project setup
```sh
npm install
```

### Deploy
To build the web root call the following command in ```/webroot```
```sh
npm run build
```

copy ```/webroot/dist``` to the Raspberry Pi ```~/webroot```

## Contributing
If you would like to contribute to the Music Box project, please feel free to submit issues or pull requests on the GitHub repository. Contributions are welcome for features, bug fixes, documentation, and more.


## Thanks 
MusicBox is built using great open source projects including ....

* RPi-RFID
* BCM2835
* rpi_ws281x 
* CrowCpp
* nlohmann_json
* libmpg123-0 
* libsdbus-c++0 
* libcap2
* pulse audio