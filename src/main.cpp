#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <ESP8266Wifi.h>
#include "DHT.h"

#define ss 15 // GPIO15 / D8
#define rst 5 // GPIO5 / D1
#define dio0 4 // GPIO4 / D2 (INTERRUP) // Unused

#define DHTTYPE DHT11
const uint8_t DHTpin = 2; // GPIO2 / D4
DHT dht(DHTpin, DHTTYPE);

float temperature;
float humidity;
String data = "";

// put function declarations here:

void setup() {
    Serial.begin(9600);
    Serial.println("");

    Serial.println("LoRa init");
    LoRa.setPins(ss, rst, dio0);

    if (!LoRa.begin(433E6)) {
      Serial.println("LoRa Start failed");
      while (1);
    }

    pinMode(DHTpin, INPUT_PULLUP);
    dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
    delay(59000);

    temperature = dht.readTemperature(false);
    humidity = dht.readHumidity();

    data = "";
    data = WiFi.macAddress();
    data.concat(";T:");
    data.concat(temperature);
    data.concat(";H:");
    data.concat(humidity);
    Serial.print("Sending data: ");
    Serial.println(data);

    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();
}