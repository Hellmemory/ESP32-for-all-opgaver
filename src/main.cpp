/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <Arduino.h>
#include <WiFi.h>
#include "time.h"

// Forward declaration
void printLocalTime();

const char* ssid     = "Wifi19-1-10";
const char* password = "880002770";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void setup(){
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop(){
  delay(5000);
  printLocalTime();
}

String getDanskWeekday(int wday) {
  const char* dage[] = {"Søndag", "Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag"};
  return String(dage[wday]);
}

String getDanskMonth(int mon) {
  const char* maaneder[] = {"Januar", "Februar", "Marts", "April", "Maj", "Juni", "Juli", "August", "September", "Oktober", "November", "December"};
  return String(maaneder[mon]);
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Kunne ikke få tid");
    return;
  }
  
  Serial.println("=== AKTUELT TIDSPUNKT ===");
  
  char buffer[100];
  sprintf(buffer, "År: %d", timeinfo.tm_year + 1900);
  Serial.println(buffer);
  
  sprintf(buffer, "Måned: %s (%d)", getDanskMonth(timeinfo.tm_mon).c_str(), timeinfo.tm_mon + 1);
  Serial.println(buffer);
  
  sprintf(buffer, "Dag: %d", timeinfo.tm_mday);
  Serial.println(buffer);
  
  sprintf(buffer, "Ugedag: %s", getDanskWeekday(timeinfo.tm_wday).c_str());
  Serial.println(buffer);
  
  sprintf(buffer, "Time: %02d", timeinfo.tm_hour);
  Serial.println(buffer);
  
  sprintf(buffer, "Minut: %02d", timeinfo.tm_min);
  Serial.println(buffer);
  
  sprintf(buffer, "Sekund: %02d", timeinfo.tm_sec);
  Serial.println(buffer);
  
  sprintf(buffer, "Dag i år: %d", timeinfo.tm_yday);
  Serial.println(buffer);
  
  sprintf(buffer, "Tidspunkt: %s %d. %s %d kl. %02d:%02d:%02d", 
    getDanskWeekday(timeinfo.tm_wday).c_str(),
    timeinfo.tm_mday,
    getDanskMonth(timeinfo.tm_mon).c_str(),
    timeinfo.tm_year + 1900,
    timeinfo.tm_hour,
    timeinfo.tm_min,
    timeinfo.tm_sec);
  Serial.println(buffer);
  
  Serial.println("========================\n");
}