#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

// ❗ Не включай sntp.h — он устарел для Arduino-ESP32.
// #include <sntp.h>  // УДАЛЕНО

const char* ssid     = "Wifo19-1-10";
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
  delay(100);

  Serial.println("Tilslutter til WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long startAttempt = millis();
  const unsigned long wifiTimeout = 20000; // 20 sek.
  while (WiFi.status() != WL_CONNECTED && (millis() - startAttempt) < wifiTimeout) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Kunne ikke forbinde til WiFi (timeout).");
    return;
  }

  Serial.print("Forbundet til WiFi, IP: ");
  Serial.println(WiFi.localIP());

  // Настройка SNTP через Arduino API (без sntp.h)
  // Смещение (сек) использовать не обязательно, если задать TZ.
  // Для Дании правильнее задать TZ, чтобы летнее время учитывалось автоматически.
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  // TZ для Копенгагена (CET/CEST)
  // Формат: STD<offset>DST,start,end. Ниже — эквивалент Europe/Copenhagen.
  setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);
  tzset();

  // Подождём синхронизации времени (не обязательно, но полезно)
  struct tm timeinfo;
  int retries = 0;
  const int maxRetries = 20; // ~10 секунд при задержке 500 мс
  while (!getLocalTime(&timeinfo) && retries < maxRetries) {
    retries++;
    delay(500);
  }

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println("Tid synkroniseret.");
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