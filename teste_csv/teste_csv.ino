
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt
 *
 * Copyright (c) 2023 mobizt
 *
 */

// This example shows how to create the spreadsheet, update and read the values.

#include <Arduino.h>
#include <WiFi.h>
#include "esp_sntp.h"
#include "esp_netif_sntp.h"
#include <freertos/task.h>
#include <ESP_Google_Sheet_Client.h>

// For SD/SD_MMC mounting helper
#include <GS_SDHelper.h>

#include "time.h"
// parametros de wifi e api do google planilhas
#include "config.h"

bool taskComplete = false;
uint count = 0;
bool ready = false;
struct tm timeinfo;
  FirebaseJson valueRange;
  FirebaseJson response;
void tokenStatusCallback(TokenInfo info);

void send_data(void *pv) {
  time_t now;
  char horario[9];
  while (true) {
    ready = GSheet.ready();
    if (ready) {
      
      time_t tt = time(NULL);
      timeinfo = *gmtime(&tt);

      Serial.println("Gsheet ready!");
      snprintf(horario, 9, "%d:%d:%d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      Serial.println(horario);
      valueRange.add("majorDimension", "COLUMNS");
      valueRange.set("values/[0]/[0]", horario);
      valueRange.set("values/[1]/[0]", String(count + 1));  // column 1/row 1
      valueRange.set("values/[2]/[0]", String(2 * count));

      count = count + 1;
      bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to update */, "Sheet1!A1" /* range to update */, &valueRange /* data to update */);

      response.toString(Serial, true);
      Serial.println();

      if (success) {
        valueRange.clear();
        response.clear();
        vTaskDelay(pdMS_TO_TICKS(1000));

      }
    }
  }
}

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

  WiFi.setAutoReconnect(true);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // In case SD/SD_MMC storage file access, mount the SD/SD_MMC card.
  // SD_Card_Mounting(); // See src/GS_SDHelper.h

  GSheet.setCert(rootCACert);  // or GSheet.setCertFile("path/to/certificate/file.pem", esp_google_sheet_file_storage_type_flash /* or esp_google_sheet_file_storage_type_sd */);

  // Set the callback for Google API access token generation status (for debug only)
  GSheet.setTokenCallback(tokenStatusCallback);

  // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
  GSheet.setPrerefreshSeconds(10 * 60);

  // Begin the access token generation for Google API authentication
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

  //sincronizando relogios
  configTime(0, 3600, ntpServer);
  bool time_ok = getLocalTime(&timeinfo);
  
  if(!time_ok){
    Serial.println("ATT: FALHA EM OBTER TEMPO LOCAL");
  }

  Serial.print("Spreedsheet id: ");
  Serial.println(spreadsheetId);

  Serial.println("Lauching tasks ...");

  xTaskCreate(send_data, "send_data", 40096, NULL, 1, NULL);
}

void loop() {
    
  // Call ready() repeatedly in loop for authentication checking and processing
  vTaskDelay(100);
}

void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error) {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  } else {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}