# MHZ16_uart

Arduino IDE library for the MH-Z16 CO2 tested with ESP32 (but might just work for Arduino too)

Version 0.1

## Credits and License

By: Intar BV  

Based on: https://github.com/nara256/MH-Z16_uart  

License: MIT

## Usage

1. Add this library to your Arduino IDE's libraries folder.
1. Include the library in your program
1. Connect your MH-Z16 sensor to your ESP32

    MH-Z16 Vcc to 5V power supply
    MH-Z16 GND  to common GND  
    MH-Z16 Tx   to ESP32 GPIO pin to use as _Rx_
    MH-Z16 Rx   to ESP32 GPIO pin to use as _Tx_
    Other MH-Z16 pins are not supported
1. Upload your program to the ESP32

## Warnings

This library only supports serial UART communication with the MH-Z16.

This library has only been tested on ESP32S.

The `isWarming()` function does not indicate whether the MH-Z16 is warming up.

## Reference

### `MHZ16_uart()`

Default constructor. You must call _begin(int rx, int tx)_ before you can use the MH-Z16

### `MHZ16_uart(int rx, int tx)`

Constructor that sets Rx and Tx pin, and initialises the serial interface.

### `void begin(int rx, int tx)`

Set the Rx and Tx pin, and initialise the serial interface.
  
### `void calibrateZero()`

Calibrate the zero-point. According to the documentation the MH-Z16 sensor must have been working in a stable gas environment with a CO₂ concentration of 400ppm for 20 minutes before you call this function.

### `void calibrateSpan(int ppm)`

Calibrate the span point. According to the documentation the MH-Z16 sensor must have been working in a stable gas environment  with a CO₂ concentration of `ppm` for 20 minutes before you call this function.

### `int getPPM()`

Read the CO₂ concentration in PPM from the sensor. On a read error the return value is -1.
  
### `bool isWarming()`

The return value indicates whether the documenation mandated 3 minutes of warm-up time have passed since the call to either `MHZ16_uart(int rx, int tx)` or `begin(int rx, int tx)`.  
**Caution:** This is not a value read from the MH-Z16

## MH-Z16 Datasheet

http://www.winsen-sensor.com/d/files/PDF/Infrared%20Gas%20Sensor/NDIR%20CO2%20SENSOR/MH-Z16%20CO2%20V2.1.pdf

## Change log

### Version 0.1

First release
