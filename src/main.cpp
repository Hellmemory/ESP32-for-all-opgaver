#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <sntp.h>

const char* ssid = "Wifo19-1-10";
const char* password = "880002770";
const char* weekdays_da[] = {
  "søndag",
  "mandag",
  "tirsdag",
  "onsdag",
  "torsdag",
  "fredag",
  "lørdag"
};

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  configTime(3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Timezone: GMT+1");

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
}

void loop() {
  static unsigned long lastPrintMillis = 0;
  const unsigned long intervalMillis = 5000;

  if (millis() - lastPrintMillis < intervalMillis) {
    return;
  }
  lastPrintMillis = millis();

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  char dateBuffer[32];
  strftime(dateBuffer, sizeof(dateBuffer), "%d-%m-%Y", &timeinfo);
  char timeBuffer[16];
  strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeinfo);

  const char* weekday = weekdays_da[timeinfo.tm_wday];
  Serial.printf("Dato: %s, Ugedag: %s, Tid: %s\n", dateBuffer, weekday, timeBuffer);
}