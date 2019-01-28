#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <RH_RF95.h>
#include <RHSoftwareSPI.h>
#include <RTClib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Deep Sleep
#define SENSOR_INTERVAL_uS (60 * 1000000)

// OneWire Bus
#define ONE_WIRE_BUS 15
#define TEMPERATURE_SENSORS_MAX 4

// SD SPI pins can be chosen freely, but cannot overlap with other ports!
#define SD_CS 23
#define SD_SCK 17
#define SD_MOSI 12
#define SD_MISO 13

#define LOG_PATH "/batcave.log"

// The sd card can also use a virtual SPI bus
SPIClass sd_spi(HSPI);

RTC_PCF8523 rtc;

// DS18B20 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void log(char *logstring) {
    File file = SD.open(LOG_PATH, FILE_APPEND);
    if (!file) {
        Serial.printf("Logging error: couldn't open logfile.");
        Serial.printf(logstring);
    } else {
        file.printf("%i,%s", rtc.now().unixtime(), logstring);
    }

    file.close();
}

void addReading(uint32_t time, DeviceAddress deviceAddress, float tempC) {
    char deviceString[17];
    snprintf(deviceString, 17, "%02X%02X%02X%02X%02X%02X%02X%02X", 
        deviceAddress[0], deviceAddress[1], deviceAddress[2], deviceAddress[3],
        deviceAddress[4], deviceAddress[5], deviceAddress[6], deviceAddress[7]);

    Serial.printf("%i, %s, %f\n", time, deviceString, tempC);

    char csv_path[27];
    snprintf(csv_path, 27, "/temp_%s.csv", deviceString);
    Serial.printf("CSV: writing to %s\n", csv_path);

    File csv = SD.open(csv_path, FILE_APPEND);
    if (!csv) {
        Serial.printf("CSV: couldn't open file %s\n", csv_path);
    } else {
        csv.printf("%i,%f\n", time, tempC);
    }
    csv.close();

}

void printOneWireAddress(DeviceAddress deviceAddress) {
    for (uint8_t i = 0; i < 8; i++) {
        Serial.print(deviceAddress[i], HEX);
    }
}

void setup() {
    // Builtin LED
    pinMode(LED_BUILTIN, OUTPUT);

    // Serial output
    Serial.begin(115200);

    // init SD card
    sd_spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

    if (!SD.begin(SD_CS, sd_spi))
        Serial.println("SD Card: mounting failed.");
    else 
        Serial.println("SD Card: mounted.");


    // RTC
    rtc.begin();
    if (!rtc.initialized()) {
        log("RTC_INIT_FAILED");
        Serial.println("RTC: couldn't initialize");
    } else {
        Serial.println("RTC: initialized");
    }


    // DS18B20: Init
    sensors.begin();
    sensors.requestTemperatures();
    oneWire.reset_search();

    DeviceAddress deviceAddress;
    uint32_t unixtime = rtc.now().unixtime();

    for (int i = 0; i < TEMPERATURE_SENSORS_MAX; i++) {
        if (!oneWire.search(deviceAddress)) {
            Serial.printf("DS18B20: Read %i sensors.\n", i);
            break;
        }

        float reading = sensors.getTempC(deviceAddress);
        addReading(unixtime, deviceAddress, reading);
    }
}

void loop() {
    Serial.println("Entering deep sleep.");
    esp_sleep_enable_timer_wakeup(SENSOR_INTERVAL_uS);
    esp_deep_sleep_start();
}