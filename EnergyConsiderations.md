# DeepSleep Energy Consumption of ESP32 Boards

Energy Source: 18650 3.7V Li-Ion
Measurement directly at the battery.

Battery Shield (nothing connected): 0.34 mA

Modes:

 1. Board BatteryConnector <-> 18650 (regBoard)
 2. BatteryConnector <-> regulated Output (regDouble)
 3. 3.3 V ESP32 Input <-> regulated Output (regShield)

Alt. 1: Measurement without battery shield (plain)

## Measurements

### Himalaya Basic Matrix Core

- Mode 1 (regBoard): 0.26 mA
- Mode 2 (regDouble): 0.5 mA
- Mode 3 (regShield): -

### Sparkfun ESP32 Thing

- Mode 1 (regBoard): 2.53 mA
- Mode 2 (regDouble): 2.85 mA
- Mode 3 (regShield): -

### ESP32 Plain

- Mode 1 (regBoard): -
- Mode 2 (regDouble): -
- Mode 3 (regShield): 0.34 mA
- Alt. 1 (plain): 4.3 uA == 0.0043 mA

## Energy Considerations

The minimal power (with 0.26 mA) was taken by the Himalaya Basic Matrix Core board. With a 3000 mAh 18650 cell, the board can be in deep sleep for 11538 h = 480 d = 68 w. However the measurements need still to be added up. 

### Alternative setup

There are LiFePO4 cells, which have a cell voltage of 3.2V and therefore  can directly power ESP32 boards. 

## Long-Term Measurements

From 2019-01-25 until 2018-01-28 a BatCave logger was running. The configuration consisted of:

 - Himalaya Basic Matrix Core Board
 - Adafruit microSD breakout board
 - PCF8523 real-time clock
 - 2x DS18B20 temperature sensors
 - 3000 mAh 18650 Li-Ion, fully charged

The battery was connected through the LiPo connecter provided by the Matrix Core Board. The energy measurements were taken by an INA219 Adafruit board.  

```
			shunt_mV			bus_V		current_mA	power_mW
count	2.174920e+06	2.174920e+06	2.174920e+06 2.174920e+06
mean	1.795176e-01	4.170158e+00	1.793891e+00 7.627134e+00
std		5.767216e-01	2.793915e-02	5.765419e+00 2.350034e+01
min		3.000000e-02	3.956000e+00	3.000000e-01 0.000000e+00
25%		1.000000e-01	4.148000e+00	1.000000e+00 4.000000e+00
50%		1.100000e-01	4.168000e+00	1.100000e+00 4.000000e+00
75%		1.300000e-01	4.192000e+00	1.300000e+00 6.000000e+00
max		1.066000e+01	4.232000e+00	1.021000e+02 4.160000e+02
```

The bus voltage dropped linearly from 4.23V to 3.96V, still way higher than the nominal voltage.

The mean current was 1.79 mA over the three days running. In the deep sleep phase an average of 1.13 mA (196782 measurements, 98.391%), while during the measurements an average of 42.78 mA (3218 == 1.609% of the measurements) were drawn.

With a capacity of 3000 mAh such a device can theoretically be powered for 3000 mAh / 1.79 mA = 1676 h = 70 d = 2.2 M
