# BatCave
[![Build Status](https://travis-ci.org/Nature40/Satellite-BatCave.svg?branch=master)](https://travis-ci.org/Nature40/Satellite-BatCave)

BatCave is a sattelite (low-power sensor) of the Nature 4.0 project. It is designed to monitor the temperature inside of tree holes in low frequencies (> 1min) over a long period of time without changing the Battery. 

## Components 

The chosen components are listed here, each with it's important features named.

- TTGO LoRa 868 MHz
  - ESP32: WiFi, DeepSleep
  - SX1278 LoRa Modem
- SD Card Breakout Board (pick any)
- PCF8523 Real-Time-Clock (exact measurements without further infrastructure)
- DS18B20 Temperature Sensors
  - no deviations with long cables (digital protocol)
  - OneWire protocol (simply scaleable)
 
![TTGO LoRa Pinout](TTGO-LoRa.jpg)

### Connections

```
#define ONE_WIRE_BUS 15

#define SD_CS 23
#define SD_SCK 17
#define SD_MOSI 12
#define SD_MISO 13

#define SDA 21
#define SCL 22
```

The TTGO LoRa provides 3x GND, 2x 5V and 2x 3.3V. Most SD card breakouts, the RTC and the temperature sensors can run on 5V and can therefore be chosen freely. However I'd recommend to use 5V for the temperature sensors, especially when using longer (> 1m) cables. 

## Operation

The normal operation consists of:

- **initialize** all the hardware
- list and **read** all connected **temperature** sensors
- log the readings to one **CSV** file per sensor
- send the readings via **WiFi** to the next BaumBox
- send the readings via **LoRa** to a gateway (also used as status-update)
- fall into **deep sleep**

If one step fails, this error needs to be logged, and the rest of the operation shall continue, for example:

- If the SD is not available, the values shall still be send.
- If the RTC is n/a, the values shall still be logged and send, since a less accurate timestamp can still be assigned at the receiver.

## Implementation Details

The TTGO LoRa is kind of picky when using together with an SD breakout, the actual solution is to use multiple SPI busses, the rerouted Hardware-SPI for the SD and a Software-SPI for the SX1278. Details are explained here: [http://github.com/jonashoechst/ttgo-lora-sd](http://github.com/jonashoechst/ttgo-lora-sd).

## Known bugs

The deep sleep function of the ESP boards is very low power, which can lead to a shutdown of powerbanks (a function useful, when a charging smartphones is filled).

### SD Breakout Power Consumption

The SD Card Breakout also uses quite an amount of power:

```
     ms, shunt_mV,    bus_V, current_mA,  power_mW
2876943,   -0.630, 3.332000,  -6.300000, 20.000000
```

A suggested solution is, to use a GPIO pin for powering the SD card, that is on low during deep sleep.

### TTGO LoRa Power Consumption

The TTGO LoRa v1 has trouble with power consumption. As discussed in [this forum](https://www.thethingsnetwork.org/forum/t/big-esp32-sx127x-topic-part-1/10247), the CP2102 usb-serial chip is in an active state when turned on and only going to suspend after beeing disconnected from a USB host. Possible fixes are suggested [here](https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/issues/6#issuecomment-403254130).

The board will draw ~3 mAh with disabled CP2102 in contrast to ~10 mAh with a suspended CP2102.

```
     ms, shunt_mV,    bus_V, current_mA,  power_mW,     state
  80305,   -0.290, 3.336000,  -3.100000, 10.000000, suspended
 121786,   -1.020, 3.328000, -10.200000, 36.000000,    active
```

Also, the SX1276 LoRa modem has a low power mode which ineffective when the TTGO is in deep sleep (resulting from a high pin in deep sleep). 

Possible alternatives:

 - Heltec Wireless Stick (~25€)
 - TTGO T-Fox (~25€)