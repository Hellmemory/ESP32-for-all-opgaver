#include <WiFi.h>
#include <time.h>

// ---------------- WiFi ----------------
const char* ssid = "Wifi19-1-10";
const char* password = "880002770";

// ---------------- NTP ----------------
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// ---------------- Danish names ----------------
String weekdayName(int wday) {
  const char* names[] = {
    "Søndag", "Mandag", "Tirsdag", "Onsdag",
    "Torsdag", "Fredag", "Lørdag"
  };
  return names[wday];
}

String monthName(int month) {
  const char* names[] = {
    "Januar", "Februar", "Marts", "April", "Maj", "Juni",
    "Juli", "August", "September", "Oktober", "November", "December"
  };
  return names[month];
}

void setup() {
  Serial.begin(115200);
  delay(300);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Time synchronized");
}

void loop() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    delay(5000);
    return;
  }

  Serial.print("Tidspunkt: ");
  Serial.printf("%02d:%02d:%02d",
                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  Serial.print("  |  Dato: ");
  Serial.printf("%02d ", timeinfo.tm_mday);
  Serial.print(monthName(timeinfo.tm_mon));
  Serial.print(" ");
  Serial.print(timeinfo.tm_year + 1900);

  Serial.print("  |  Ugedag: ");
  Serial.println(weekdayName(timeinfo.tm_wday));

  delay(5000);
}